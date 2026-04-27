

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "mlir/IR/BuiltinTypes.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
    struct VectorizeITensor : public VectorizeITensorBase<VectorizeITensor> {
        void runOnOperation() override {
            auto func = getOperation();
            IRRewriter rewriter(func->getContext());

            auto convertType = [&](ITensorType oldType) -> ITensorType {
                auto shape = oldType.getElementShape();

                return ITensorType::get(

                )
            }

            SmallVector<ITensorInstanceOp> instances;
            func.walk([&](ITensorInstanceOp op) {instances.push_back(op);});

            for (auto instOp : instances) {

                auto oldType = instOp.getType();
                auto newType = convertType(oldType);
                if (!newType) continue;

                rewriter.setInsertionPoint(instOp);
                
                auto newInst = rewriter.create<ITensorInstanceOp>(
                    instOp.getLoc(), newType, instOp.getDepth());
                rewriter.replaceOp(instOp, newInst.getResult());

                // go to users
                // early inc range allows safe iteratoin when element can be mutated or destroyed
                for (auto &use : llvm::make_early_inc_range(instOp->getUses()))  {
                    Operation *user = use.getOwner();
                    rewriter.setInsertionPoint(user);

                    if (auto readOp = dyn_cast<ITensorReadOp>(user)) {
                        rewriteRead
                    }
                }



                // if (oldShape.empty() || oldShape.back() % vectorWidth != 0) {
                //     continue;
                // }

                // auto newElementType = VectorType::get(
                //     {static_cast<int64_t>(vectorWidth)}, oldType.getElementType());

                // SmallVector<int64_t> newShape(oldShape.begin(), oldShape.end());
                // newShape.back() /= vectorWidth;

                // auto newType = ITensorType::get(
                //     func->getContext(), newElementType, newShape,
                //     oldType.getIterTripCounts(), oldType.getIterStepSizes(),
                //     oldType.getIterMap());

                // rewriter.setInsertionPoint(instOp);
                auto newInst = rewriter.create<ITensorInstanceOp>(
                    instOp.getLoc(), newType, instOp.getDepth());
                rewriter.replaceOp(instOp, newInst.getResult());

                    

            }
            
        }
    };
}

std::unique_ptr<Pass> scalehls::createVectorizeITensorPass() {
    return std::make_unique<VectorizeITensor>();
}