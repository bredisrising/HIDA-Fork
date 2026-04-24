#include "scalehls/Transforms/Passes.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

/// Return how many scalar elements fit in one AXI bus word.
static unsigned computeVectorLen(Type elementType, unsigned busWidthBits) {
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

/// Update the block-argument types of `kernel` to use widened ITensor types.
static void widenKernelInterface(STKernelOp kernel, unsigned busWidthBits,
                                 IRRewriter &rewriter) {
  Block &body = kernel.getBody().front();
  for (BlockArgument arg : body.getArguments()) {
    auto itType = dyn_cast<ITensorType>(arg.getType());
    if (!itType)
      continue;
    ITensorType widened = widenITensorType(itType, busWidthBits,
                                           kernel.getContext());
    if (widened != itType)
      arg.setType(widened);
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
