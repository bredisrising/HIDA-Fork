#include "scalehls/Transforms/Passes.h"
#include "scalehls/Transforms/Utils.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Utils/StaticValueUtils.h"
#include "mlir/IR/AffineExpr.h"
#include "mlir/IR/Matchers.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
    struct ConvertLinalgToKernelPass : public ConvertLinalgToKernelBase<ConvertLinalgToKernelPass> {
        void runOnOperation() override {
            func::FuncOp func = getOperation();
            func.walk([&](linalg::GenericOp generic) {
                auto outerLoop = generic->getParentOfType<scf::ForOp>();
                if (!outerLoop) return;

                while (auto parent = outerLoop->getParentOfType<scf::ForOp>()) {
                    outerLoop = parent;
                }
                IRRewriter rewriter(&getContext());
                convertTiledLinalgToKernel(outerLoop, generic, rewriter);
            });
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
                APInt lbAP, ubAP, stepAP;
                matchPattern(op.getLowerBound(), m_ConstantInt(&lbAP));
                matchPattern(op.getUpperBound(), m_ConstantInt(&ubAP));
                matchPattern(op.getStep(),       m_ConstantInt(&stepAP));
                int64_t lb = lbAP.getSExtValue();
                int64_t ub = ubAP.getSExtValue();
                int64_t step = stepAP.getSExtValue();
                int64_t tripCount = (ub - lb) / step;

                tripCounts.push_back(tripCount);
                stepSizes.push_back(step);
                dimExprs.push_back(getAffineDimExpr(dimIdx++, ctx));

            }     
            
            AffineMap iterMap = AffineMap::get(loopBand.size(), 0, dimExprs, ctx);

            return ITensorType::get(ctx, elementType, elementShape, tripCounts, stepSizes, iterMap);
            

        }

        void convertTiledLinalgToKernel(
            scf::ForOp outerLoop, linalg::GenericOp generic, IRRewriter &rewriter
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

            SmallVector<Value> inputTensors;
            for (auto extract : extracts) {
                inputTensors.push_back(extract.getSource());
            }

            SmallVector<Value> outputInits;

            for (auto insert : inserts) {
                outputInits.push_back(insert.getDest());
            }

            auto kernel = builder.create<STKernelOp>(
                loc,
                /*results=*/TypeRange{},
                builder.getStringAttr("kernel"),
                inputTensors,
                outputInits,
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
                loc, /*results=*/TypeRange{},
                /*sym_name=*/StringAttr{}, /*inits=*/ValueRange{},
                /*kernel=*/true, /*dataDriven=*/false);

            Block* taskBlock = new Block();
            task.getBody().push_back(taskBlock);
            builder.setInsertionPointToStart(taskBlock);

            for (unsigned i = 0; i < extracts.size(); i++) {
                Value itensorArg = kernelBlock->getArgument(i);
                auto tileType = extracts[i].getResult().getType();
                auto read = builder.create<ITensorReadOp>(loc, tileType, itensorArg, /*init=*/Value());

                extracts[i].getResult().replaceAllUsesWith(read.getResult());
            }

            generic->moveBefore(taskBlock, taskBlock->end());

            unsigned outputStart = extracts.size();
            for (unsigned i = 0; i < inserts.size(); i++) {
                Value itensorArg = kernelBlock->getArgument(outputStart + i);
                Value tile = generic.getResult(i);
                builder.create<ITensorWriteOp>(loc, itensorArg.getType(), tile, itensorArg);
            }

            builder.create<STYieldOp>(loc);

            builder.setInsertionPointToEnd(kernelBlock);
            builder.create<STYieldOp>(loc, outputInits);
            
            rewriter.eraseOp(outerLoop);
        }
    };
} // namespace

std::unique_ptr<Pass> mlir::scalehls::createConvertLinalgToKernelPass() {
    return std::make_unique<ConvertLinalgToKernelPass>();
}