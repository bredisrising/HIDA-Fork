#include "scalehls/Transforms/Passes.h"
#include "scalehls/Transforms/Utils.h"
#include "scalehls/Dialect/HLS/HLS.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Linalg/Utils/Utils.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Tensor/Utils/Utils.h"
#include "mlir/IR/AffineMap.h"
#include "mlir/IR/Builders.h"

using namespace mlir;
using namespace scalehls;
using namespace hls;

namespace {

static SmallVector<int64_t> inferTileShape(ITensorType iTensorType) {
  return SmallVector<int64_t>(iTensorType.getElementShape());
}

//===----------------------------------------------------------------------===//
// Pure type/index helpers
//===----------------------------------------------------------------------===//

// Packed type: outer dims mirror src (static→dim/tile, dynamic→-1), then static tile dims.
// e.g. tensor<64x64xf32> + [16,16] → tensor<4x4x16x16xf32>
//      tensor<?x?xf32>   + [16,16] → tensor<?x?x16x16xf32>
static RankedTensorType buildPackedType(RankedTensorType src,
                                        ArrayRef<int64_t> tile) {
  unsigned rank = src.getRank();
  SmallVector<int64_t> shape;
  for (unsigned i = 0; i < rank; i++) {
    int64_t d = src.getDimSize(i);
    shape.push_back(d == -1 ? -1 : d / tile[i]);
  }
  for (int64_t t : tile)
    shape.push_back(t);
  return RankedTensorType::get(shape, src.getElementType());
}

// Interleaved type: alternating outer/tile dims, same static/dynamic treatment.
// e.g. tensor<64x64xf32> + [16,16] → tensor<4x16x4x16xf32>
//      tensor<?x?xf32>   + [16,16] → tensor<?x16x?x16xf32>
static RankedTensorType buildInterleavedType(RankedTensorType src,
                                             ArrayRef<int64_t> tile) {
  unsigned rank = src.getRank();
  SmallVector<int64_t> shape;
  for (unsigned i = 0; i < rank; i++) {
    int64_t d = src.getDimSize(i);
    shape.push_back(d == -1 ? -1 : d / tile[i]);
    shape.push_back(tile[i]);
  }
  return RankedTensorType::get(shape, src.getElementType());
}

// [[0,1],[2,3],...] for expand_shape / collapse_shape
static SmallVector<ReassociationIndices> buildReassociation(unsigned rank) {
  SmallVector<ReassociationIndices> result;
  for (unsigned i = 0; i < rank; i++)
    result.push_back({static_cast<int64_t>(2 * i),
                      static_cast<int64_t>(2 * i + 1)});
  return result;
}

// [0,2,4,..., 1,3,5,...] — outer dims first, then tile dims (pack direction)
static SmallVector<int64_t> buildPackPerm(unsigned rank) {
  SmallVector<int64_t> perm;
  for (unsigned i = 0; i < rank; i++)
    perm.push_back(static_cast<int64_t>(2 * i));
  for (unsigned i = 0; i < rank; i++)
    perm.push_back(static_cast<int64_t>(2 * i + 1));
  return perm;
}

// [0,rank, 1,rank+1,...] — interleave back (unpack direction)
static SmallVector<int64_t> buildUnpackPerm(unsigned rank) {
  SmallVector<int64_t> perm;
  for (unsigned i = 0; i < rank; i++) {
    perm.push_back(static_cast<int64_t>(i));
    perm.push_back(static_cast<int64_t>(rank + i));
  }
  return perm;
}

//===----------------------------------------------------------------------===//
// packTensor: [pad →] expand_shape → linalg.transpose
// Padding is skipped for fully-static tensors (dims assumed multiples of tile).
//===----------------------------------------------------------------------===//
static Value packTensor(Value src, ArrayRef<int64_t> tile, OpBuilder &builder,
                        Location loc) {
  auto srcType = cast<RankedTensorType>(src.getType());
  unsigned rank = srcType.getRank();
  bool hasDynamic = llvm::any_of(srcType.getShape(),
                                 [](int64_t d) { return d == -1; });

  // 1. Pad each dynamic dim to the next multiple of tile[d]. Skip for static.
  Value toPack = src;
  if (hasDynamic) {
    SmallVector<OpFoldResult> lowPads(rank, builder.getIndexAttr(0));
    SmallVector<OpFoldResult> highPads;
    for (unsigned d = 0; d < rank; d++) {
      Value dimVal = builder.create<tensor::DimOp>(loc, src, d);
      Value tileVal = builder.create<arith::ConstantIndexOp>(loc, tile[d]);
      Value nTiles = builder.create<arith::CeilDivSIOp>(loc, dimVal, tileVal);
      Value paddedSize = builder.create<arith::MulIOp>(loc, nTiles, tileVal);
      Value highPad = builder.create<arith::SubIOp>(loc, paddedSize, dimVal);
      highPads.push_back(highPad);
    }
    Value zero = builder.create<arith::ConstantOp>(
        loc, builder.getZeroAttr(srcType.getElementType()));
    toPack = tensor::createPadScalarOp(
        srcType, src, zero, lowPads, highPads, /*nofold=*/false, loc, builder);
  }

  // 2. expand_shape: flat → interleaved (4x16x4x16 or ?x16x?x16)
  auto interleavedType = buildInterleavedType(srcType, tile);
  auto reassoc = buildReassociation(rank);
  Value expanded = builder.create<tensor::ExpandShapeOp>(
      loc, interleavedType, toPack, reassoc);

  // 3. linalg.transpose: interleaved → packed (4x4x16x16 or ?x?x16x16)
  //    Only pass SSA values for dims that are actually dynamic in packedType.
  auto packedType = buildPackedType(srcType, tile);
  SmallVector<Value> dynDims;
  for (unsigned d = 0; d < rank; d++) {
    if (packedType.getDimSize(d) == -1)
      dynDims.push_back(builder.create<tensor::DimOp>(loc, expanded, 2 * d));
  }
  Value initTensor = builder.create<tensor::EmptyOp>(
      loc, packedType.getShape(), packedType.getElementType(), dynDims);
  auto perm = buildPackPerm(rank);
  return linalg::makeTransposeOp(builder, loc, expanded, initTensor, perm)
      ->getResult(0);
}

//===----------------------------------------------------------------------===//
// unpackTensor: linalg.transpose → collapse_shape [→ extract_slice]
// extract_slice is skipped for fully-static tensors (no padding was added).
//===----------------------------------------------------------------------===//
static Value unpackTensor(Value packed, ArrayRef<int64_t> tile,
                          ArrayRef<Value> origDims, RankedTensorType origType,
                          OpBuilder &builder, Location loc) {
  auto packedType = cast<RankedTensorType>(packed.getType());
  unsigned rank = tile.size();
  auto elemType = packedType.getElementType();

  // 1. linalg.transpose: packed (4x4x16x16 or ?x?x16x16) → interleaved
  auto interleavedType = buildInterleavedType(origType, tile);
  SmallVector<Value> outerDims;
  for (unsigned d = 0; d < rank; d++) {
    if (interleavedType.getDimSize(2 * d) == -1)
      outerDims.push_back(builder.create<tensor::DimOp>(loc, packed, d));
  }
  Value initInterleaved = builder.create<tensor::EmptyOp>(
      loc, interleavedType.getShape(), elemType, outerDims);
  auto perm = buildUnpackPerm(rank);
  Value interleaved =
      linalg::makeTransposeOp(builder, loc, packed, initInterleaved, perm)
          ->getResult(0);

  // 2. collapse_shape: interleaved → flat (origType for static, padded ?x? for dynamic)
  Value flat = builder.create<tensor::CollapseShapeOp>(
      loc, origType, interleaved, buildReassociation(rank));

  // 3. extract_slice: strip padding back to original dims (dynamic only).
  //    For static tensors, collapse_shape already gives origType exactly.
  bool hasDynamic = llvm::any_of(origType.getShape(),
                                 [](int64_t d) { return d == -1; });
  if (!hasDynamic)
    return flat;

  SmallVector<OpFoldResult> offsets(rank, builder.getIndexAttr(0));
  SmallVector<OpFoldResult> sizes;
  for (Value od : origDims)
    sizes.push_back(od);
  SmallVector<OpFoldResult> strides(rank, builder.getIndexAttr(1));
  return builder.create<tensor::ExtractSliceOp>(loc, origType, flat,
                                                offsets, sizes, strides);
}

//===----------------------------------------------------------------------===//
// packKernelInterface: main transformation
//===----------------------------------------------------------------------===//
static void packKernelInterface(STKernelOp kernel, IRRewriter &rewriter) {
  auto loc = kernel.getLoc();
  Block *kernelBlock = &kernel.getBody().front();
  unsigned numInputs = kernel.getNumInputs();
  unsigned numOutputInits = kernel.getNumOutputInits();

  rewriter.setInsertionPoint(kernel);

  // --- Pack all inputs ---
  for (unsigned i = 0; i < numInputs; i++) {
    Value input = kernel.getInputs()[i];
    // Skip if fed directly from another kernel (fusion opportunity).
    if (input.getDefiningOp<STKernelOp>())
      continue;

    auto tile = inferTileShape(
        cast<ITensorType>(kernelBlock->getArgument(i).getType()));
    Value packed = packTensor(input, tile, rewriter, loc);
    kernel->setOperand(i, packed);
  }

  // --- Pack all outputs ---
  // Collect info first, then insert unpack ops after the kernel.
  struct OutputInfo {
    unsigned idx;
    RankedTensorType origType;
    SmallVector<int64_t> tile;
    SmallVector<Value> origDims; // only populated for dynamic-shaped tensors
  };
  SmallVector<OutputInfo> toUnpack;

  for (unsigned i = 0; i < numOutputInits; i++) {
    Value outputInit = kernel.getOutputInits()[i];
    auto tensorType = cast<RankedTensorType>(outputInit.getType());

    auto tile = inferTileShape(
        cast<ITensorType>(kernelBlock->getArgument(numInputs + i).getType()));

    // Save original dims before the kernel (only needed for dynamic dims,
    // so that unpackTensor can trim padding via extract_slice).
    SmallVector<Value> origDims;
    bool hasDynamic = llvm::any_of(tensorType.getShape(),
                                   [](int64_t d) { return d == -1; });
    if (hasDynamic) {
      for (unsigned d = 0; d < (unsigned)tensorType.getRank(); d++)
        origDims.push_back(rewriter.create<tensor::DimOp>(loc, outputInit, d));
    }

    Value packedInit = packTensor(outputInit, tile, rewriter, loc);
    auto packedType = cast<RankedTensorType>(packedInit.getType());

    // Rewrite the output_init operand (follows all inputs).
    kernel->setOperand(numInputs + i, packedInit);
    // Rewrite the result type.
    kernel.getResult(i).setType(packedType);
    // Rewrite the carried tensor block arg type
    // (layout: [numInputs ITensor args][numOutputInits ITensor args]
    //          [numOutputInits tensor args]).
    kernelBlock->getArgument(numInputs + numOutputInits + i).setType(packedType);

    toUnpack.push_back({i, tensorType, std::move(tile), std::move(origDims)});
  }

  // Insert unpack ops after the kernel, one per packed result.
  Operation *insertAfter = kernel;
  for (auto &info : toUnpack) {
    rewriter.setInsertionPointAfter(insertAfter);

    // Collect pre-existing uses BEFORE creating unpack ops (the unpack ops
    // themselves use the packed kernel result; we must not redirect those).
    SmallVector<OpOperand *> usesToReplace;
    for (OpOperand &use : kernel.getResult(info.idx).getUses())
      usesToReplace.push_back(&use);

    Value unpacked = unpackTensor(kernel.getResult(info.idx), info.tile,
                                  info.origDims, info.origType, rewriter, loc);

    // Only replace pre-existing uses, not the ones we just created.
    for (OpOperand *use : usesToReplace)
      use->set(unpacked);

    insertAfter = unpacked.getDefiningOp();
  }
}

struct PackKernelInterfacePass
    : public PackKernelInterfaceBase<PackKernelInterfacePass> {
  void runOnOperation() override {
    func::FuncOp func = getOperation();
    IRRewriter rewriter(&getContext());
    func.walk([&](STKernelOp kernel) {
      packKernelInterface(kernel, rewriter);
    });
  }
};

} // namespace

std::unique_ptr<Pass> scalehls::createPackKernelInterfacePass() {
  return std::make_unique<PackKernelInterfacePass>();
}
