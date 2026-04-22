#include "scalehls/Transforms/Passes.h"
#include "scalehls/Transforms/Utils.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/IR/AffineMap.h"
#include "mlir/IR/Builders.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

static SmallVector<int64_t> inferTileShape(ITensorType iTensorType) {
    return SmallVector<int64_t>(iTensorType.getElementShape());
}

/// Annotate a kernel input operand index as needing tiled packing.
/// MaterializeKernel will read this attribute to generate tiling loops.
static void annotateInputPack(
    STKernelOp kernel,
    unsigned operandIdx,
    SmallVector<int64_t> tileShape,
    IRRewriter &rewriter)
{
    auto key = rewriter.getStringAttr("pack_input_" + std::to_string(operandIdx));
    auto val = rewriter.getDenseI64ArrayAttr(tileShape);
    kernel->setAttr(key, val);
}

/// Annotate a kernel result index as needing tiled unpacking.
/// MaterializeKernel will read this attribute to generate tiling loops.
static void annotateOutputUnpack(
    STKernelOp kernel,
    unsigned resultIdx,
    SmallVector<int64_t> tileShape,
    IRRewriter &rewriter)
{
    auto key = rewriter.getStringAttr("unpack_output_" + std::to_string(resultIdx));
    auto val = rewriter.getDenseI64ArrayAttr(tileShape);
    kernel->setAttr(key, val);
}

static void packKernelInterface(STKernelOp kernel, IRRewriter &rewriter) {

    for (auto operand : kernel.getInputs()) {

        if (auto iTensorType = dyn_cast<ITensorType>(operand.getType())) {
            auto dataShape = iTensorType.getDataShape();
            bool isStatic = llvm::none_of(dataShape, ShapedType::isDynamic);

            if (isStatic) {
                continue;
            } else {

                bool canFuse = false;

                for (auto* user : operand.getUsers()) {
                    if (isa<STKernelOp>(user)) {
                        canFuse = true;
                        break;
                    }
                }

                if (!canFuse) {
                    auto tileShape = inferTileShape(iTensorType);
                    unsigned idx = llvm::find(kernel.getInputs(), operand) - kernel.getInputs().begin();
                    annotateInputPack(kernel, idx, tileShape, rewriter);
                }
            }
        }
    }

    for (auto operand : kernel.getResults()) {

        if (auto iTensorType = dyn_cast<ITensorType>(operand.getType())) {
            auto shape = iTensorType.getDataShape();
            bool isStatic = llvm::none_of(shape, ShapedType::isDynamic);

            if (isStatic) continue;
            else {

                auto tileShape = inferTileShape(iTensorType);
                unsigned idx = operand.getResultNumber();
                annotateOutputUnpack(kernel, idx, tileShape, rewriter);
            }
        }
    }

}

struct PackKernelInterfacePass
    : public PackKernelInterfaceBase<PackKernelInterfacePass> {
  void runOnOperation() override {
    func::FuncOp func = getOperation();
    IRRewriter rewriter(&getContext());

    func.walk([&](STKernelOp kernel) {
      packKernelInterface(kernel, rewriter);
    });
  }
};

}

std::unique_ptr<Pass> scalehls::createPackKernelInterfacePass() {
    return std::make_unique<PackKernelInterfacePass>();
}
