

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Vector/IR/VectorOps.h"
#include "mlir/IR/BuiltinTypes.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
    struct VectorizeITensor : public VectorizeITensorBase<VectorizeITensor> {
        void runOnOperation() override {
            auto func = getOperation();
            auto *ctx = func->getContext();
            IRRewriter rewriter(ctx);

            // pack last dim into vector<vw x T>. null = skip.
            auto convertType = [&](ITensorType oldType) -> ITensorType {
                auto shape = oldType.getElementShape();
                if (shape.empty() || shape.back() % vectorWidth != 0) return nullptr;
                if (oldType.getElementType().isa<VectorType>()) return nullptr; // already vec

                auto vecElem = VectorType::get({(int64_t)vectorWidth}, oldType.getElementType());
                SmallVector<int64_t> newShape(shape.begin(), shape.end());
                newShape.back() /= vectorWidth;

                return ITensorType::get(ctx, vecElem, newShape,
                    oldType.getIterTripCounts(), oldType.getIterStepSizes(), oldType.getIterMap());
            };

            // turn one itensor.write into a loop nest that pushes vectors.
            auto buildPackLoop = [&](ITensorWriteOp w, ITensorType newType) -> Value {
                auto loc = w.getLoc();
                auto newShape = newType.getElementShape();
                auto vecType = newType.getElementType().cast<VectorType>();
                Value src = w.getValue();          // tensor we're packing from
                Value runningFifo = w.getDest();   // FIFO threaded through iter_args

                Value c0 = rewriter.create<arith::ConstantIndexOp>(loc, 0);
                Value c1 = rewriter.create<arith::ConstantIndexOp>(loc, 1);
                Value cVw = rewriter.create<arith::ConstantIndexOp>(loc, vectorWidth);

                // build nested for loops, one per element-shape dim
                SmallVector<scf::ForOp> loops;
                SmallVector<Value> ivs;
                for (size_t d = 0; d < newShape.size(); ++d) {
                    Value ub = rewriter.create<arith::ConstantIndexOp>(loc, newShape[d]);
                    auto loop = rewriter.create<scf::ForOp>(loc, c0, ub, c1, ValueRange{runningFifo});
                    loops.push_back(loop);
                    ivs.push_back(loop.getInductionVar());
                    runningFifo = loop.getRegionIterArgs()[0];
                    // scf.for w/ iter_args needs us to add the yield manually
                    rewriter.setInsertionPointToEnd(loop.getBody());
                    rewriter.create<scf::YieldOp>(loc, ValueRange{runningFifo});
                    rewriter.setInsertionPoint(loop.getBody()->getTerminator());
                }

                // grab vw scalars from src, push as one vector token
                SmallVector<Value> idx(ivs.begin(), ivs.end());
                idx.back() = rewriter.create<arith::MulIOp>(loc, ivs.back(), cVw); // last dim * vw
                Value pad = rewriter.create<arith::ConstantOp>(loc,
                    vecType.getElementType(), rewriter.getZeroAttr(vecType.getElementType()));
                Value vec = rewriter.create<vector::TransferReadOp>(loc, vecType, src, idx, pad);
                Value pushed = rewriter.create<ITensorWriteOp>(loc, newType, vec, runningFifo);

                // patch yields so FIFO threads out
                cast<scf::YieldOp>(loops.back().getBody()->getTerminator())
                    .getResultsMutable().assign(ValueRange{pushed});
                for (int d = (int)loops.size() - 2; d >= 0; --d) {
                    cast<scf::YieldOp>(loops[d].getBody()->getTerminator())
                        .getResultsMutable().assign(ValueRange{loops[d+1].getResult(0)});
                }
                return loops.front().getResult(0);
            };

            // mirror: pop vectors and stitch back into a scalar tensor
            auto buildUnpackLoop = [&](ITensorReadOp r, ITensorType newType) -> Value {
                auto loc = r.getLoc();
                auto newShape = newType.getElementShape();
                auto vecType = newType.getElementType().cast<VectorType>();
                auto outType = r.getResult().getType().cast<RankedTensorType>();
                Value fifo = r.getSource();

                Value c0 = rewriter.create<arith::ConstantIndexOp>(loc, 0);
                Value c1 = rewriter.create<arith::ConstantIndexOp>(loc, 1);
                Value cVw = rewriter.create<arith::ConstantIndexOp>(loc, vectorWidth);

                // start with empty tensor, fill it as we go
                Value runningT = rewriter.create<tensor::EmptyOp>(loc,
                    outType.getShape(), outType.getElementType());

                SmallVector<scf::ForOp> loops;
                SmallVector<Value> ivs;
                for (size_t d = 0; d < newShape.size(); ++d) {
                    Value ub = rewriter.create<arith::ConstantIndexOp>(loc, newShape[d]);
                    auto loop = rewriter.create<scf::ForOp>(loc, c0, ub, c1, ValueRange{runningT});
                    loops.push_back(loop);
                    ivs.push_back(loop.getInductionVar());
                    runningT = loop.getRegionIterArgs()[0];
                    rewriter.setInsertionPointToEnd(loop.getBody());
                    rewriter.create<scf::YieldOp>(loc, ValueRange{runningT});
                    rewriter.setInsertionPoint(loop.getBody()->getTerminator());
                }

                // pop one vec, write it back into the accumulator
                SmallVector<Value> idx(ivs.begin(), ivs.end());
                idx.back() = rewriter.create<arith::MulIOp>(loc, ivs.back(), cVw);
                Value vec = rewriter.create<ITensorReadOp>(loc, vecType, fifo, /*init=*/Value{});
                auto tw = rewriter.create<vector::TransferWriteOp>(loc, vec, runningT, idx);
                Value newT = tw.getResult();

                cast<scf::YieldOp>(loops.back().getBody()->getTerminator())
                    .getResultsMutable().assign(ValueRange{newT});
                for (int d = (int)loops.size() - 2; d >= 0; --d) {
                    cast<scf::YieldOp>(loops[d].getBody()->getTerminator())
                        .getResultsMutable().assign(ValueRange{loops[d+1].getResult(0)});
                }
                return loops.front().getResult(0);
            };

            // collect first, mutate after
            SmallVector<ITensorInstanceOp> instances;
            func.walk([&](ITensorInstanceOp op) { instances.push_back(op); });

            for (auto instOp : instances) {
                auto newType = convertType(instOp.getType());
                if (!newType) continue;

                // walk forward through write chain to find every read/write
                SmallVector<ITensorWriteOp> writes;
                SmallVector<ITensorReadOp> reads;
                SmallVector<Value> frontier{instOp.getResult()};
                bool unsupported = false;
                while (!frontier.empty()) {
                    Value v = frontier.pop_back_val();
                    for (auto &use : v.getUses()) {
                        Operation *user = use.getOwner();
                        if (auto w = dyn_cast<ITensorWriteOp>(user)) {
                            writes.push_back(w);
                            frontier.push_back(w.getResult()); // chase the chain
                        } else if (auto rd = dyn_cast<ITensorReadOp>(user)) {
                            reads.push_back(rd);
                        } else {
                            unsupported = true; // cast/fork/yield etc — bail for now
                        }
                    }
                    if (unsupported) break;
                }
                if (unsupported) continue;

                // writes -> reads -> instance (instance last keeps types valid)
                for (auto w : writes) {
                    rewriter.setInsertionPoint(w);
                    rewriter.replaceOp(w, buildPackLoop(w, newType));
                }
                for (auto rd : reads) {
                    rewriter.setInsertionPoint(rd);
                    rewriter.replaceOp(rd, buildUnpackLoop(rd, newType));
                }
                rewriter.setInsertionPoint(instOp);
                auto newInst = rewriter.create<ITensorInstanceOp>(
                    instOp.getLoc(), newType, instOp.getDepth());
                rewriter.replaceOp(instOp, newInst.getResult());
            }
        }
    };
} // namespace

std::unique_ptr<Pass> scalehls::createVectorizeITensorPass() {
    return std::make_unique<VectorizeITensor>();
}
