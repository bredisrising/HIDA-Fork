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

    auto makePassThroughITensor = [&](RankedTensorType srcType) -> ITensorType {
        auto elementType = srcType.getElementType();
        auto elementShape = SmallVector<int64_t>(srcType.getShape());
        unsigned rank = elementShape.size();
        // One trip of 1 per dimension; step = full dimension size (whole tensor
        // consumed in a single pass).  Identity map: (d0,...,dn) -> (d0,...,dn).
        SmallVector<int64_t> tripCounts(rank, 1);
        SmallVector<int64_t> stepSizes(elementShape);
        SmallVector<AffineExpr> identityExprs;
        for (unsigned j = 0; j < rank; j++)
            identityExprs.push_back(getAffineDimExpr(j, ctx));
        auto iterMap = AffineMap::get(rank, 0, identityExprs, ctx);
        return ITensorType::get(ctx, elementType, elementShape, tripCounts, stepSizes, iterMap);
    };

    for (Value v : inputs)
        types.push_back(makePassThroughITensor(cast<RankedTensorType>(v.getType())));

    for (Value v : outputs)
        types.push_back(makePassThroughITensor(cast<RankedTensorType>(v.getType())));

    // Kernel result types match the op's output tensor types so callers can
    // use the kernel results in place of the original op results.
    SmallVector<Type> kernelResultTypes;
    for (Value v : outputs)
        kernelResultTypes.push_back(v.getType());

    OpBuilder builder(op);
    Location loc = op->getLoc();

    auto kernel = builder.create<STKernelOp>(
        loc,
        /*results=*/kernelResultTypes,
        builder.getStringAttr("kernel"),
        inputs,
        /*outputInits=*/ValueRange{},
        DenseBoolArrayAttr{},
        DenseBoolArrayAttr{}
    );

    // Replace external uses BEFORE moving op into the kernel body.  At this
    // point the only uses of op->getResult(i) are outside the kernel (e.g.
    // "return %collapsed"), so replaceAllUsesWith is safe.
    for (unsigned i = 0; i < outputs.size(); i++)
        outputs[i].replaceAllUsesWith(kernel.getResult(i));

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

    // In the kernel body (after the task), read each output ITensor back to
    // tensor and yield them so callers can use the kernel results.
    builder.setInsertionPointToEnd(kernelBlock);
    SmallVector<Value> kernelYields;
    for (unsigned i = 0; i < outputs.size(); i++) {
        unsigned outIdx = inputs.size() + i;
        auto read = builder.create<ITensorReadOp>(
            loc, outputs[i].getType(), kernelBlock->getArgument(outIdx), Value{});
        kernelYields.push_back(read.getResult());
    }
    builder.create<STYieldOp>(loc, kernelYields);
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