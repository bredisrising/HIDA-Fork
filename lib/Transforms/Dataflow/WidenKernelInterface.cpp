#include "scalehls/Transforms/Passes.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Builders.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

/// Return how many scalar elements fit in one AXI bus word.
/// Returns 1 for non-scalar element types (already widened or incompatible).
static unsigned computeVectorLen(Type elementType, unsigned busWidthBits) {
  if (!elementType.isIntOrFloat())
    return 1;
  unsigned elemBits = elementType.getIntOrFloatBitWidth();
  return busWidthBits / elemBits;
}

/// Widen the innermost element dimension of an ITensor by packing `vectorLen`
/// scalars into a vector, shrinking that dimension accordingly.
static ITensorType widenITensorType(ITensorType type, unsigned busWidthBits,
                                    MLIRContext *ctx) {
  unsigned vectorLen = computeVectorLen(type.getElementType(), busWidthBits);
  if (vectorLen <= 1)
    return type;

  SmallVector<int64_t> elemShape(type.getElementShape());
  if (elemShape.empty() || elemShape.back() % vectorLen != 0)
    return type; // cannot widen – leave unchanged

  elemShape.back() /= vectorLen;
  auto vecType = VectorType::get({(int64_t)vectorLen}, type.getElementType());
  return ITensorType::get(ctx, vecType, elemShape, type.getIterTripCounts(),
                          type.getIterStepSizes(), type.getIterMap());
}

/// Widen block args, read results, and write values of `kernel` to pack scalar
/// elements into vectors for maximum memory bandwidth.
///
/// For each ITensor block arg that widens:
///  - ITensorReadOp: insert unrealized_conversion_cast after so downstream
///    scalar compute ops still see the original tile type; then update the
///    read result type to the widened tile type.
///  - ITensorWriteOp: insert unrealized_conversion_cast before the value to
///    produce the widened tile type; update the result type to match dest.
///
/// The casts are placeholders for VectorizeITensorPass (Phase 4), which
/// replaces them by vectorizing the compute bodies.
static void widenKernelInterface(STKernelOp kernel, unsigned busWidthBits,
                                 IRRewriter &rewriter) {
  Block &body = kernel.getBody().front();
  MLIRContext *ctx = kernel.getContext();

  // Collect widened args before mutating ops inside the body.
  SmallVector<std::pair<BlockArgument, ITensorType>> widenedArgs;
  for (BlockArgument arg : body.getArguments()) {
    auto itType = dyn_cast<ITensorType>(arg.getType());
    if (!itType)
      continue;
    ITensorType widened = widenITensorType(itType, busWidthBits, ctx);
    if (widened == itType)
      continue;
    arg.setType(widened);
    widenedArgs.push_back({arg, widened});
  }

  for (auto [arg, newITType] : widenedArgs) {
    auto newTileType = RankedTensorType::get(
        newITType.getElementShape(), newITType.getElementType());

    for (OpOperand &use : llvm::make_early_inc_range(arg.getUses())) {
      Operation *user = use.getOwner();

      if (auto readOp = dyn_cast<ITensorReadOp>(user)) {
        auto oldTileType =
            cast<RankedTensorType>(readOp.getResult().getType());
        if (oldTileType == newTileType)
          continue;

        // 1. Create cast BEFORE changing any types so the op is created with
        //    consistent types. After setType below, the cast's input becomes
        //    newTileType, giving the conversion: newTileType → oldTileType.
        rewriter.setInsertionPointAfter(readOp);
        auto bridgeCast = rewriter.create<UnrealizedConversionCastOp>(
            readOp.getLoc(), TypeRange{oldTileType},
            ValueRange{readOp.getResult()});

        // 2. Replace all uses of the read result except the cast itself so
        //    compute ops continue to use the original scalar tile type.
        readOp.getResult().replaceAllUsesExcept(bridgeCast.getOutputs()[0],
                                               bridgeCast);

        // 3. Update the read result type; the cast is now newTileType→oldTileType.
        readOp.getResult().setType(newTileType);

      } else if (auto writeOp = dyn_cast<ITensorWriteOp>(user)) {
        auto oldTileType =
            cast<RankedTensorType>(writeOp.getValue().getType());

        if (oldTileType != newTileType) {
          // Insert cast before the write to convert the compute output
          // (scalar tile type) to the widened tile type.
          rewriter.setInsertionPoint(writeOp);
          auto bridgeCast = rewriter.create<UnrealizedConversionCastOp>(
              writeOp.getLoc(), TypeRange{newTileType},
              ValueRange{writeOp.getValue()});
          writeOp.getValueMutable().assign(bridgeCast.getOutputs()[0]);
        }

        // AllTypesMatch["dest", "result"] requires result type == dest type;
        // dest (the block arg) is already widened so update result to match.
        writeOp.getResult().setType(newITType);
      }
    }
  }
}

struct WidenKernelInterfacePass
    : public WidenKernelInterfaceBase<WidenKernelInterfacePass> {
  void runOnOperation() override {
    auto func = getOperation();
    IRRewriter rewriter(func.getContext());
    func.walk([&](STKernelOp kernel) {
      widenKernelInterface(kernel, busWidth, rewriter);
    });
  }
};

} // namespace

std::unique_ptr<Pass> scalehls::createWidenKernelInterfacePass() {
  return std::make_unique<WidenKernelInterfacePass>();
}
