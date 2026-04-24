#include "scalehls/Transforms/Passes.h"
#include "scalehls/Transforms/Utils.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/AffineMap.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

static void convertTensorOpToKernel(Operation *op, IRRewriter &rewriter) {
    
    MLIRContext *ctx = op->getContext();

    SmallVector<Value> inputs;
    for (auto v : op->getOperands()) {
        if (isa<RankedTensorType>(v.getType())) {
            inputs.push_back(v);
        }
    }

    SmallVector<Value> outputs;
    for (Value v : op->getResults()) {
        outputs.push_back(v);
    }

    SmallVector<ITensorType> types;

    for (Value v : inputs) {
        auto srcType = cast<RankedTensorType>(v.getType());
        auto elementType = srcType.getElementType();
        auto elementShape = SmallVector<int64_t>(srcType.getShape());
        auto iterMap = AffineMap::get(/*dimCount=*/1, /*symCount=*/0,
            SmallVector<AffineExpr>(elementShape.size(),
                                    getAffineConstantExpr(0, ctx)),
            ctx);
        types.push_back(ITensorType::get(ctx, elementType, elementShape, {1}, {1}, iterMap));
    }

    for (Value v : outputs) {
        auto srcType = cast<RankedTensorType>(v.getType());
        auto elementType = srcType.getElementType();
        auto elementShape = SmallVector<int64_t>(srcType.getShape());
        auto iterMap = AffineMap::get(/*dimCount=*/1, /*symCount=*/0,
            SmallVector<AffineExpr>(elementShape.size(),
                                    getAffineConstantExpr(0, ctx)),
            ctx);
        types.push_back(ITensorType::get(ctx, elementType, elementShape, {1}, {1}, iterMap));
    }

    OpBuilder builder(op);
    Location loc = op->getLoc();

    auto kernel = builder.create<STKernelOp>(
        loc,
        /*results=*/TypeRange{},
        builder.getStringAttr("kernel"),
        inputs,
        /*outputInits=*/ValueRange{},
        DenseBoolArrayAttr{},
        DenseBoolArrayAttr{}
    );

    Block* kernelBlock = new Block();
    for (auto itensorType : types) {
        kernelBlock->addArgument(itensorType, loc);
    }
    kernel.getBody().push_back(kernelBlock);

    builder.setInsertionPointToStart(kernelBlock);
    auto task = builder.create<STTaskOp>(
        loc, TypeRange{}, StringAttr{},
        ValueRange{}, /*kernel=*/true, /*dataDriven=*/false
    );

    Block* taskBlock = new Block();
    task.getBody().push_back(taskBlock);
    builder.setInsertionPointToStart(taskBlock);

    // Create reads first (before op), then move op, then writes, then yield.
    for (unsigned i = 0; i < inputs.size(); i++) {
        auto read = builder.create<ITensorReadOp>(
            loc, inputs[i].getType(), kernelBlock->getArgument(i), Value{});
        op->replaceUsesOfWith(inputs[i], read.getResult());
    }

    op->moveBefore(taskBlock, taskBlock->end());

    builder.setInsertionPointToEnd(taskBlock);
    for (unsigned i = 0; i < outputs.size(); i++) {
        unsigned outIdx = inputs.size() + i;
        builder.create<ITensorWriteOp>(
            loc, kernelBlock->getArgument(outIdx).getType(),
            op->getResult(i), kernelBlock->getArgument(outIdx)
        );
    }

    builder.create<STYieldOp>(loc);

    builder.setInsertionPointToEnd(kernelBlock);
    builder.create<STYieldOp>(loc);
}

namespace {
struct ConvertTensorToKernelPass
    : public ConvertTensorToKernelBase<ConvertTensorToKernelPass> {
    void runOnOperation() override {
        func::FuncOp func = getOperation();

        SmallVector<Operation *> worklist;
        func.walk([&](Operation *op) {
            if (!isa<tensor::PadOp, tensor::CollapseShapeOp,
                     tensor::ExpandShapeOp>(op))
                return;
            if (op->getParentOfType<STKernelOp>())
                return;
            worklist.push_back(op);
        });

        for (Operation *op : worklist) {
            IRRewriter rewriter(&getContext());
            convertTensorOpToKernel(op, rewriter);
        }
    }
};
}

std::unique_ptr<Pass> mlir::scalehls::createConvertTensorToKernelPass() {
    return std::make_unique<ConvertTensorToKernelPass>();
}