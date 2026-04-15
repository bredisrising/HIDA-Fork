#include "scalehls/Transforms/Passes.h"
#include "scalehls/Transforms/Utils.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/IR/OpFoldResult.h"
#include "mlir/IR/AffineExpr.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
    struct ConvertLinalgToKernelPass : public ConvertLinalgToKernelBase<ConvertLinalgToKernelPass> {
        void runOnOperation() override {
            func::FuncOp func = getOperation();
        }
    

        ITensorType inferITensorType (
            SmallVectorImpl<scf::ForOp> &loopBand, tensor::ExtractSliceOp extract
        ) {

            MLIRContext *ctx = extract.getContext();

            auto srcType = cast<RankedTensorType>(extract.getResult().getType());
            Type elementType = srcType.getElementType();

            SmallVector<OpFoldResult> sizes = extract.getMixedSizes();

            SmallVector<int64_t> elementShape;
            for (auto s : sizes) {
                auto cst = getConstantIntValue(s);
                assert(cst && "dynamic tile sizes not supported yet");
                elementShape.push_back(*cst);
            }


            SmallVector<int64_t> tripCounts, stepSizes;
            SmallVector<AffineExpr> dimExprs;
            unsigned dimIdx = 0;

            for (scf::ForOp op : loopBand) {
                int64_t lb, ub, step;
                matchPattern(op.getLowerBound(), m_ConstantIndex(&lb));
                matchPattern(op.getUpperBound(), m_ConstantIndex(&ub));
                matchPattern(op.getStep(),       m_ConstantIndex(&step));
                int64_t tripCount = (ub - lb) / step;

                tripCounts.push_back(tripCount);
                stepSizes.push_back(step);
                dimExprs.push_back(getAffineDimExpr(dimIdx++, ctx));

            }     
            
            AffineMap iterMap = AffineMap::get(loopBand.size(), 0, dimExprs, ctx);

            return ITensorType::get(ctx, elementType, elementShape, tripCounts, stepSizes, iterMap);
            

        }

        void convertTiledLinalgToKernel(
            scf::ForOp outerLoop, linalg::GenericOp generic, PatternRewriter &rewriter
        ) {
            SmallVector<tensor::ExtractSliceOp> extracts;
            SmallVector<tensor::InsertSliceOp> inserts;

            for (Value op : generic.getInputs()) {
                auto extract = op.getDefiningOp<tensor::ExtractSliceOp>();
                if (!extract) continue;

                extracts.push_back(extract);
            }

            for (Value result : generic.getResults()) {
                for (Operation* user : result.getUsers()) {
                    auto insert = dyn_cast<tensor::InsertSliceOp>(user);
                    if (!insert) continue;

                    inserts.push_back(insert);
                }
            }

            SmallVector<scf::ForOp> loopBand;
            scf::ForOp current = outerLoop;
            scf::ForOp prev = outerLoop;

            while (current) {
                loopBand.push_back(current);
                
                prev = current;
                current = nullptr;
                for (auto &op : prev.getBody()->getOperations()) {
                    if (auto nested = dyn_cast<scf::ForOp>(&op)) {
                        current = nested;
                        break;
                    }
                }
            }


            SmallVector<ITensorType> types;

            for (auto extract : extracts) {
                types.push_back(inferITensorType(loopBand, extract));
            }

            for (auto insert : inserts) {
                types.push_back(
                    inferITensorType(loopBand, insert.getSource()
                    .getDefiningOp<tensor::ExtractSliceOp>()));
            }

            OpBuilder builder(outerLoop);
            Location loc = outerLoop.getLoc();

            auto inputTensors = outerLoop->getOperands();
            SmallVector<Value> outputInits;

            auto kernel = builder.create<STKernelOp>(
                loc,
                builder.getStringAttr("kernel"),
                inputTensors,
                outputInits,
                nullptr,
                nullptr
            );

            Block* kernelBlock = new Block();
            for (auto itensorType : types) {
                kernelBlock->addArgument(itensorType, loc);
            }
            kernel.getBody().push_back(kernelBlock);

            
        }
    };
}