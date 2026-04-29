  #include "scalehls/Transforms/Passes.h"
  #include "scalehls/Dialect/HLS/HLS.h"
  #include "mlir/Dialect/Arith/IR/Arith.h"
  #include "mlir/Dialect/SCF/IR/SCF.h"
  #include "mlir/Dialect/Tensor/IR/Tensor.h"
  #include "mlir/IR/AffineExpr.h"
  #include <functional>

  using namespace mlir;
  using namespace scalehls;
  using namespace hls;

  namespace {

  //===----------------------------------------------------------------------===//
  // Offset helpers
  //===----------------------------------------------------------------------===//

  // Translate an ITensor's iterMap into concrete extract/insert offsets.
  // iterMap result d = AffineDimExpr(j)  →  offset[d] = ivs[j]
  // Constant or unrecognised expression  →  offset[d] = %c0
  static SmallVector<OpFoldResult>
  offsetsFromIterMap(AffineMap iterMap, ArrayRef<Value> ivs,
                    OpBuilder &b, Location loc) {
    Value c0 = b.create<arith::ConstantIndexOp>(loc, 0);
    SmallVector<OpFoldResult> offsets;
    for (unsigned d = 0; d < iterMap.getNumResults(); d++) {
      AffineExpr expr = iterMap.getResult(d);
      if (auto dimExpr = expr.dyn_cast<AffineDimExpr>())
        offsets.push_back(ivs[dimExpr.getPosition()]);
      else
        offsets.push_back(OpFoldResult(c0));
    }
    return offsets;
  }

  //===----------------------------------------------------------------------===//
  // DMA-in: external tensor → ITensor FIFO
  //===----------------------------------------------------------------------===//

  // Build nested scf.for loops inside the DMA-in task body.
  // Each loop dimension uses [0, tripCount*stepSize) step stepSize so that the
  // induction variable equals the byte offset directly — no multiplication
  // needed when forming the extract_slice.
  static void buildDMAInBody(OpBuilder &b, Location loc,
                            Value extTensor, Value itensorArg,
                            ITensorType itType) {
    auto tripCounts = itType.getIterTripCounts();
    auto stepSizes  = itType.getIterStepSizes();
    auto elemShape  = itType.getElementShape();
    AffineMap iterMap = itType.getIterMap();
    unsigned iterRank = tripCounts.size();
    unsigned elemRank  = elemShape.size();

    SmallVector<Value> ivs;

    std::function<void(unsigned, OpBuilder &, Location)> buildLoop =
        [&](unsigned depth, OpBuilder &b2, Location loc2) {
          if (depth == iterRank) {
            // Innermost body: extract tile and push into the FIFO.
            auto offsets = offsetsFromIterMap(iterMap, ivs, b2, loc2);
            SmallVector<OpFoldResult> sizes, strides;
            for (int64_t s : elemShape) sizes.push_back(b2.getIndexAttr(s));
            for (unsigned d = 0; d < elemRank; d++) strides.push_back(b2.getIndexAttr(1));

            auto tileType = RankedTensorType::get(
                SmallVector<int64_t>(elemShape), itType.getElementType());
            Value tile = b2.create<tensor::ExtractSliceOp>(
                loc2, tileType, extTensor, offsets, sizes, strides);
            b2.create<ITensorWriteOp>(loc2, itensorArg.getType(), tile, itensorArg);
            return;
          }

          Value c0   = b2.create<arith::ConstantIndexOp>(loc2, 0);
          Value ub   = b2.create<arith::ConstantIndexOp>(
              loc2, (int64_t)tripCounts[depth] * (int64_t)stepSizes[depth]);
          Value step = b2.create<arith::ConstantIndexOp>(loc2, (int64_t)stepSizes[depth]);

          b2.create<scf::ForOp>(loc2, c0, ub, step, ValueRange{},
              [&](OpBuilder &body, Location bodyLoc, Value iv, ValueRange) {
                ivs.push_back(iv);
                buildLoop(depth + 1, body, bodyLoc);
                body.create<scf::YieldOp>(bodyLoc);
                ivs.pop_back();
              });
        };

    buildLoop(0, b, loc);
  }

  //===----------------------------------------------------------------------===//
  // DMA-out: ITensor FIFO → output tensor
  //===----------------------------------------------------------------------===//

  // Build nested scf.for loops with iter_args inside the DMA-out task body.
  // Reads one tile per iteration via hls.itensor.read and inserts it into the
  // accumulator tensor via tensor.insert_slice.  Returns the fully assembled
  // output tensor (the outermost for loop result).
  static Value buildDMAOutBody(OpBuilder &b, Location loc,
                              Value itensorArg, Value carryTensor,
                              ITensorType itType) {
    auto tripCounts = itType.getIterTripCounts();
    auto stepSizes  = itType.getIterStepSizes();
    auto elemShape  = itType.getElementShape();
    AffineMap iterMap = itType.getIterMap();
    unsigned iterRank = tripCounts.size();
    unsigned elemRank  = elemShape.size();

    SmallVector<Value> ivs;

    std::function<Value(unsigned, Value, OpBuilder &, Location)> buildLoop =
        [&](unsigned depth, Value acc,
            OpBuilder &b2, Location loc2) -> Value {
          if (depth == iterRank) {
            // Innermost body: pop a tile and insert it into the accumulator.
            auto tileType = RankedTensorType::get(
                SmallVector<int64_t>(elemShape), itType.getElementType());
            Value tile = b2.create<ITensorReadOp>(
                loc2, tileType, itensorArg, /*init=*/Value{});

            auto offsets = offsetsFromIterMap(iterMap, ivs, b2, loc2);
            SmallVector<OpFoldResult> sizes, strides;
            for (int64_t s : elemShape) sizes.push_back(b2.getIndexAttr(s));
            for (unsigned d = 0; d < elemRank; d++) strides.push_back(b2.getIndexAttr(1));

            return b2.create<tensor::InsertSliceOp>(
                loc2, tile, acc, offsets, sizes, strides);
          }

          Value c0   = b2.create<arith::ConstantIndexOp>(loc2, 0);
          Value ub   = b2.create<arith::ConstantIndexOp>(
              loc2, (int64_t)tripCounts[depth] * (int64_t)stepSizes[depth]);
          Value step = b2.create<arith::ConstantIndexOp>(loc2, (int64_t)stepSizes[depth]);

          scf::ForOp forOp = b2.create<scf::ForOp>(
              loc2, c0, ub, step, ValueRange{acc},
              [&](OpBuilder &body, Location bodyLoc, Value iv, ValueRange regionArgs) {
                ivs.push_back(iv);
                Value inner = buildLoop(depth + 1, regionArgs[0], body, bodyLoc);
                body.create<scf::YieldOp>(bodyLoc, inner);
                ivs.pop_back();
              });
          return forOp.getResult(0);
        };

    return buildLoop(0, carryTensor, b, loc);
  }

  //===----------------------------------------------------------------------===//
  // Per-kernel transformation
  //===----------------------------------------------------------------------===//

  static void materializeKernelOp(STKernelOp kernel, IRRewriter &rewriter) {
    Block &body = kernel.getBody().front();
    Location loc = kernel.getLoc();
    unsigned numInputs     = kernel.getNumInputs();
    unsigned numOutputInits = kernel.getNumOutputInits();
    // Block arg layout: [input ITensors | output ITensors | carry tensors]
    unsigned numITensorArgs = numInputs + numOutputInits;

    // Find the compute task (marked with kernel=true).
    STTaskOp computeTask;
    for (auto &op : body.getOperations()) {
      if (auto task = dyn_cast<STTaskOp>(&op); task && task.getKernel()) {
        computeTask = task;
        break;
      }
    }
    if (!computeTask)
      return;

    // Warn and skip kernels where the external tensors are packed (rank >
    // elementRank), since packed DMA materialisation is not yet supported.
    for (unsigned i = 0; i < numInputs; i++) {
      auto extType = cast<RankedTensorType>(kernel.getInputs()[i].getType());
      auto itType  = cast<ITensorType>(body.getArgument(i).getType());
      if ((unsigned)extType.getRank() != itType.getElementRank()) {
        kernel.emitWarning(
            "MaterializeKernelPass: packed kernel interface not yet supported; "
            "skipping kernel");
        return;
      }
    }
    for (unsigned i = 0; i < numOutputInits; i++) {
      auto extType = cast<RankedTensorType>(kernel.getOutputInits()[i].getType());
      auto itType  = cast<ITensorType>(body.getArgument(numInputs + i).getType());
      if ((unsigned)extType.getRank() != itType.getElementRank()) {
        kernel.emitWarning(
            "MaterializeKernelPass: packed kernel interface not yet supported; "
            "skipping kernel");
        return;
      }
    }

    //------------------------------------------------------------------------
    // 1. DMA-in tasks — inserted before the compute task, one per input.
    //------------------------------------------------------------------------
    rewriter.setInsertionPoint(computeTask);
    for (unsigned i = 0; i < numInputs; i++) {
      Value extInput  = kernel.getInputs()[i];
      Value itensorArg = body.getArgument(i);
      auto itType = cast<ITensorType>(itensorArg.getType());

      auto dmaIn = rewriter.create<STTaskOp>(
          loc, TypeRange{}, StringAttr{}, ValueRange{},
          /*kernel=*/false, /*dataDriven=*/false);
      Block *taskBlock = new Block();
      dmaIn.getBody().push_back(taskBlock);
      rewriter.setInsertionPointToStart(taskBlock);

      buildDMAInBody(rewriter, loc, extInput, itensorArg, itType);
      rewriter.create<STYieldOp>(loc);

      // Restore insertion point to just before the compute task for the next
      // DMA-in task (or for the compute task itself if this is the last input).
      rewriter.setInsertionPoint(computeTask);
    }

    //------------------------------------------------------------------------
    // 2. DMA-out tasks — inserted after the compute task, one per output.
    //------------------------------------------------------------------------
    // Track where to insert the next DMA-out task (after the previous one).
    Operation *insertAfter = computeTask;
    STYieldOp kernelYield = kernel.getYieldOp();

    for (unsigned i = 0; i < numOutputInits; i++) {
      Value itensorArg = body.getArgument(numInputs + i);
      Value carryArg   = body.getArgument(numITensorArgs + i);
      auto itType      = cast<ITensorType>(itensorArg.getType());
      auto tensorType  = cast<RankedTensorType>(carryArg.getType());

      rewriter.setInsertionPointAfter(insertAfter);

      // The task has one result: the assembled output tensor.
      auto dmaOut = rewriter.create<STTaskOp>(
          loc, TypeRange{tensorType}, StringAttr{}, ValueRange{},
          /*kernel=*/false, /*dataDriven=*/false);
      Block *taskBlock = new Block();
      dmaOut.getBody().push_back(taskBlock);
      rewriter.setInsertionPointToStart(taskBlock);

      Value assembled = buildDMAOutBody(rewriter, loc, itensorArg, carryArg, itType);
      rewriter.create<STYieldOp>(loc, ValueRange{assembled});

      // Replace the i-th carry arg in the kernel yield with the task result.
      kernelYield->setOperand(i, dmaOut.getResult(0));

      insertAfter = dmaOut;
    }
  }

  //===----------------------------------------------------------------------===//
  // Pass
  //===----------------------------------------------------------------------===//

  struct MaterializeKernelPass
      : public MaterializeKernelBase<MaterializeKernelPass> {
    void runOnOperation() override {
      func::FuncOp func = getOperation();
      IRRewriter rewriter(&getContext());
      func.walk([&](STKernelOp kernel) {
        materializeKernelOp(kernel, rewriter);
      });
    }
  };

  } // namespace

  std::unique_ptr<Pass> scalehls::createMaterializeKernelPass() {
    return std::make_unique<MaterializeKernelPass>();
  }
