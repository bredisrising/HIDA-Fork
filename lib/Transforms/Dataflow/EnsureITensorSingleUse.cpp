//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//
//
// A FIFO (itensor) is fundamentally a single-producer, single-consumer
// channel. If an itensor value is consumed by N > 1 ops, we must insert an
// ITensorForkOp to create N duplicate streams. This must run before FoldITensor
// so that fold only ever sees structurally valid (single-consumer) FIFOs.
//
//===----------------------------------------------------------------------===//

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
struct EnsureITensorSingleUse
    : public EnsureITensorSingleUseBase<EnsureITensorSingleUse> {
  void runOnOperation() override {
    auto func = getOperation();
    IRRewriter rewriter(func->getContext());

    bool changed = true;
    while (changed) {
      changed = false;
      func.walk([&](Operation *op) {
        for (auto result : op->getResults()) {
          if (!llvm::isa<ITensorType>(result.getType()))
            continue;

          // Collect all uses into a stable list before we modify anything.
          SmallVector<OpOperand *> uses;
          for (auto &use : result.getUses())
            uses.push_back(&use);

          if (uses.size() < 2)
            continue;

          // Insert a fork immediately after the producing op. The fork takes
          // one itensor in and emits N identical itensor streams out.
          rewriter.setInsertionPointAfter(op);
          SmallVector<Type> forkResultTypes(uses.size(), result.getType());
          auto forkOp = rewriter.create<ITensorForkOp>(
              op->getLoc(), forkResultTypes, result);

          // Redirect each consumer to its dedicated fork output.
          for (auto [use, forkOut] :
               llvm::zip(uses, forkOp.getResults()))
            use->set(forkOut);

          changed = true;
          // Restart the walk — we modified the IR.
          return WalkResult::interrupt();
        }
        return WalkResult::advance();
      });
    }
  }
};
} // namespace

std::unique_ptr<Pass> scalehls::createEnsureITensorSingleUsePass() {
  return std::make_unique<EnsureITensorSingleUse>();
}
