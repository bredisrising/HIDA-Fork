//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//
//
// Cleans up the STTask/STKernel hierarchy after ITensor optimizations:
//
// Pattern 1 — Dead task: an STTaskOp whose STYieldOp yields no values and
//   whose body performs no side-effecting work. Remove it entirely.
//
// Pattern 2 — Passthrough task: an STTaskOp whose body consists solely of
//   an STYieldOp that passes all block arguments through unchanged, AND whose
//   itensor types are identical (no type conversion happening). Inline it by
//   replacing the task's results with its init operands.
//
//===----------------------------------------------------------------------===//

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

// Remove an STTaskOp that yields nothing and has no side effects.
struct RemoveDeadTask : public OpRewritePattern<STTaskOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(STTaskOp taskOp,
                                PatternRewriter &rewriter) const override {
    // Must yield nothing.
    if (!taskOp.getResults().empty())
      return failure();

    auto &body = taskOp.getBody();
    auto yieldOp = cast<STYieldOp>(body.front().getTerminator());

    // Body must only contain the yield — nothing else.
    if (yieldOp->getPrevNode() != nullptr)
      return failure();

    rewriter.eraseOp(taskOp);
    return success();
  }
};

// Inline an STTaskOp that is purely a pass-through: the body only contains a
// yield, and the yield operands are exactly the task's init operands in order.
// STTaskOp is not IsolatedFromAbove, so the body captures outer values directly
// (no block arguments). The yield referencing the same outer inits means the
// task does nothing except forward its inputs to its outputs.
struct InlinePassthroughTask : public OpRewritePattern<STTaskOp> {
  using OpRewritePattern::OpRewritePattern;

  LogicalResult matchAndRewrite(STTaskOp taskOp,
                                PatternRewriter &rewriter) const override {
    auto &body = taskOp.getBody();
    auto yieldOp = cast<STYieldOp>(body.front().getTerminator());

    // Body must only contain the yield.
    if (yieldOp->getPrevNode() != nullptr)
      return failure();

    auto inits = taskOp.getInits();
    auto yieldOperands = yieldOp.getOperands();

    // Yield must return exactly the init operands in order.
    if (inits.size() != yieldOperands.size())
      return failure();
    for (auto [init, yielded] : llvm::zip(inits, yieldOperands))
      if (init != yielded)
        return failure();

    // Types must match exactly — if they differ, a type conversion is needed.
    for (auto [init, result] :
         llvm::zip(taskOp.getInits(), taskOp.getResults()))
      if (init.getType() != result.getType())
        return failure();

    // Replace task results with the init operands directly.
    rewriter.replaceOp(taskOp, taskOp.getInits());
    return success();
  }
};

struct SimplifyTaskStructure
    : public SimplifyTaskStructureBase<SimplifyTaskStructure> {
  void runOnOperation() override {
    auto func = getOperation();
    auto context = func.getContext();

    RewritePatternSet patterns(context);
    patterns.add<RemoveDeadTask, InlinePassthroughTask>(context);
    (void)applyPatternsAndFoldGreedily(func, std::move(patterns));
  }
};
} // namespace

std::unique_ptr<Pass> scalehls::createSimplifyTaskStructurePass() {
  return std::make_unique<SimplifyTaskStructure>();
}
