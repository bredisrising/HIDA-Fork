//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//

#include "scalehls/Dialect/HLS/HLS.h"
#include "scalehls/Transforms/Passes.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {
struct ConvertEmptyToInstance
    : public ConvertEmptyToInstanceBase<ConvertEmptyToInstance> {
  void runOnOperation() override {
    auto func = getOperation();
    IRRewriter rewriter(func->getContext());

    // Collect all ITensorEmptyOps first — we cannot mutate while walking.
    SmallVector<ITensorEmptyOp> empties;
    func.walk([&](ITensorEmptyOp op) { empties.push_back(op); });

    // Replace each placeholder with a concrete ITensorInstanceOp (depth 2).
    // Depth 2 = double-buffering: producer fills one buffer while the consumer
    // drains the other. This is the minimal safe depth for pipeline overlap.
    for (auto emptyOp : empties) {
      rewriter.setInsertionPoint(emptyOp);
      auto inst = rewriter.create<ITensorInstanceOp>(
          emptyOp.getLoc(), emptyOp.getType(), /*depth=*/2);
      rewriter.replaceOp(emptyOp, inst.getResult());
    }
  }
};
} // namespace

std::unique_ptr<Pass> scalehls::createConvertEmptyToInstancePass() {
  return std::make_unique<ConvertEmptyToInstance>();
}
