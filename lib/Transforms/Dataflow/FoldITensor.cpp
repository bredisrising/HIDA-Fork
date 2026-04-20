//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//
//
// When a write op and a read op share the same itensor type (same iter map,
// same element shape, same trip counts), the FIFO is redundant — the producer
// already generates tiles in the exact order the consumer expects. We can
// eliminate the FIFO and replace the read's result tensor with the write's
// source tensor, merging the two computation regions.
//
// Access pattern match criteria:
//   1. Same element type and element shape
//   2. Same iteration trip counts and step sizes
//   3. Same affine iter map (structural equality via AffineMap::operator==)
//
//===----------------------------------------------------------------------===//

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

// Fold: write(%val, %fifo) -> read(%fifo) into direct use of %val.
// This pattern fires when write and read have identical itensor types, meaning
// the access pattern (order of tile traversal) is the same on both sides.
struct FoldMatchingITensor : public OpRewritePattern<ITensorReadOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(ITensorReadOp readOp,
                                PatternRewriter &rewriter) const override {
    auto src = readOp.getSource();
    auto writeOp = src.getDefiningOp<ITensorWriteOp>();
    if (!writeOp)
      return failure();

    auto writeType = llvm::cast<ITensorType>(writeOp.getDest().getType());
    auto readType = llvm::cast<ITensorType>(src.getType());

    // Access patterns match when both itensor types are identical.
    // ITensorType equality is structural: same element shape, same trip counts,
    // same step sizes, same affine iter map.
    if (writeType != readType)
      return failure();

    // The read produces a tensor<...> from the FIFO. After folding, we skip
    // the FIFO entirely and give the consumer the tensor that was written.
    rewriter.replaceOp(readOp, writeOp.getValue());
    return success();
  }
};

struct FoldITensor : public FoldITensorBase<FoldITensor> {
  void runOnOperation() override {
    auto func = getOperation();
    auto context = func.getContext();

    RewritePatternSet patterns(context);
    patterns.add<FoldMatchingITensor>(context);
    (void)applyPatternsAndFoldGreedily(func, std::move(patterns));
  }
};
} // namespace

std::unique_ptr<Pass> scalehls::createFoldITensorPass() {
  return std::make_unique<FoldITensor>();
}
