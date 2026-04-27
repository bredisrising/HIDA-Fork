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
            SmallVector<scf::ForOp> outerLoops;
            SmallVector<linalg::GenericOp> generics;

            func.walk([&](linalg::GenericOp generic) {
                auto outerLoop = generic->getParentOfType<scf::ForOp>();
                if (!outerLoop) return;

                while (auto parent = outerLoop->getParentOfType<scf::ForOp>()) {
                    outerLoop = parent;
                }
                
                outerLoops.push_back(outerLoop);
                generics.push_back(generic);
            });

            for (unsigned i = 0; i < outerLoops.size(); i++) {
                IRRewriter rewriter(&getContext());
                convertTiledLinalgToKernel(outerLoops[i], generics[i], rewriter);
            }
        }
    

        ITensorType inferITensorType (
            SmallVectorImpl<scf::ForOp> &loopBand, tensor::ExtractSliceOp extract
        ) {

            MLIRContext *ctx = extract.getContext();

            auto srcType = cast<RankedTensorType>(extract.getResult().getType());
            Type elementType = srcType.getElementType();

            SmallVector<OpFoldResult> sizes = extract.getMixedSizes();

            SmallVector<int64_t> elementShape;

            // TODO: Handle dynamic tile sizes

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


            SmallVector<tensor::ExtractSliceOp> outputExtracts;
            for (Value output : generic.getOutputs()) {
                auto extract = output.getDefiningOp<tensor::ExtractSliceOp>();
                if (!extract) continue;
                outputExtracts.push_back(extract);
            }

            SmallVector<ITensorType> types;

            for (auto extract : extracts) {
                types.push_back(inferITensorType(loopBand, extract));
            }

            // Output types are inferred from the linalg.generic's outs operands
            // (each should be an extract_slice of the destination tensor).
            for (auto extract : outputExtracts) {
                types.push_back(inferITensorType(loopBand, extract));
            }

            OpBuilder builder(outerLoop);
            Location loc = outerLoop.getLoc();

            SmallVector<Value> inputTensors;
            for (auto extract : extracts) {
                inputTensors.push_back(extract.getSource());
            }

            // Use the outermost loop's iter_arg initial values as output inits.
            // These are defined outside the loop and remain valid after erasure.
            // (insert.getDest() would yield inner iter-args that are erased
            // along with the outer loop, causing use-after-free.)
            SmallVector<Value> outputInits(outerLoop.getInitArgs());

            // The kernel's result types mirror the outer loop's result types so
            // that we can replace the loop's results with the kernel's results.
            SmallVector<Type> kernelResultTypes(outerLoop.getResultTypes());

            auto kernel = builder.create<STKernelOp>(
                loc,
                /*results=*/kernelResultTypes,
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
            // Thread output init tensors as block args so they can be yielded
            // without violating the IsolatedFromAbove constraint.
            for (Value init : outputInits) {
                kernelBlock->addArgument(init.getType(), loc);
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

            unsigned outputStart = extracts.size();
            for (unsigned i = 0; i < outputExtracts.size(); i++) {
                Value itensorArg = kernelBlock->getArgument(outputStart + i);
                auto tileType = outputExtracts[i].getResult().getType();
                auto read = builder.create<ITensorReadOp>(loc, tileType, itensorArg, /*init=*/Value());
                outputExtracts[i].getResult().replaceAllUsesWith(read.getResult());
            }

            generic->moveBefore(taskBlock, taskBlock->end());

            builder.setInsertionPointToEnd(taskBlock);
            for (unsigned i = 0; i < inserts.size(); i++) {
                Value itensorArg = kernelBlock->getArgument(outputStart + i);
                Value tile = generic.getResult(i);
                builder.create<ITensorWriteOp>(loc, itensorArg.getType(), tile, itensorArg);
            }

            builder.create<STYieldOp>(loc);

            builder.setInsertionPointToEnd(kernelBlock);
            // Yield the tensor block args (not external outputInits — that
            // would violate IsolatedFromAbove).
            SmallVector<Value> kernelYieldVals;
            for (unsigned i = 0; i < outputInits.size(); i++)
                kernelYieldVals.push_back(kernelBlock->getArgument(types.size() + i));
            builder.create<STYieldOp>(loc, kernelYieldVals);
            
            // Replace all uses of the outer loop's results (e.g. the function
            // return value) with the kernel's results before erasing.
            outerLoop->replaceAllUsesWith(kernel.getResults());
            rewriter.eraseOp(outerLoop);
        }
    };
} // namespace

std::unique_ptr<Pass> mlir::scalehls::createConvertLinalgToKernelPass() {
    return std::make_unique<ConvertLinalgToKernelPass>();
}