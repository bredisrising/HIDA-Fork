//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//

#ifndef SCALEHLS_DIALECT_HLS_HLSTYPES_TD
#define SCALEHLS_DIALECT_HLS_HLSTYPES_TD

include "scalehls/Dialect/HLS/HLSAttributes.td"

class HLSType<string name, list<Trait> traits = []> :
    TypeDef<HLSDialect, name, traits>;

def StreamType : HLSType<"Stream"> {
  let summary = "An HLS stream type";
  let description = [{
    Represents a stream of any type that can be transfered between HLS modules.
    This type is equal to the hls::stream<> type in Xilinx Vivado HLS.
  }];
  let mnemonic = "stream";

  let parameters = (ins "mlir::Type":$elementType, "unsigned":$depth);
  let assemblyFormat = "`<` qualified($elementType) `,` $depth `>`";
}

def AxiType : HLSType<"Axi"> {
  let summary = "An HLS AXI interface type";
  let mnemonic = "axi";

  let parameters = (ins "mlir::Type":$elementType);
  let assemblyFormat = "`<` qualified($elementType) `>`";

  let genVerifyDecl = 1;
  let extraClassDeclaration = [{
    Type getDataType();
  }];
}

def BundleType : HLSType<"Bundle"> {
  let summary = "An HLS AXI bundle type";
  let mnemonic = "bundle";

  let parameters = (ins "mlir::Type":$dataType, AxiKindParam:$kind);
  let assemblyFormat = "`<` qualified($dataType) `,` $kind `>`";
}

//===----------------------------------------------------------------------===//
// StreamTensor types
//===----------------------------------------------------------------------===//

def ITensorType : HLSType<"ITensor"> {
  let summary = "An iteration tensor type for streaming dataflow";
  let description = [{
    Represents a streaming FIFO channel that transfers tiles of a tensor in a
    prescribed iteration order. An ITensor encodes:
    - elementType / elementShape: the type and shape of each tile transferred
      per FIFO transaction (e.g., f32 / [16, 16] for 16x16 float tiles).
    - iterTripCounts / iterStepSizes: for each iteration dimension, the number
      of steps and the step size (derived from tiling loop bounds).
    - iterMap: an AffineMap that maps iteration indices to data tensor
      dimensions, describing the order in which tiles cover the full tensor.

    Syntax:
      !hls.itensor<elementType, [elementShape], [iterTripCounts],
                    [iterStepSizes], iterMap>

    Example:
      !hls.itensor<f32, [16, 16], [4, 4], [16, 16],
                    affine_map<(d0, d1) -> (d0, d1)>>
  }];
  let mnemonic = "itensor";

  let parameters = (ins
    "mlir::Type":$elementType,
    ArrayRefParameter<"int64_t">:$elementShape,
    ArrayRefParameter<"int64_t">:$iterTripCounts,
    ArrayRefParameter<"int64_t">:$iterStepSizes,
    "mlir::AffineMap":$iterMap
  );

  // AffineMap parameters cannot use auto-generated assemblyFormat.
  let hasCustomAssemblyFormat = 1;
  let genVerifyDecl = 1;

  let extraClassDeclaration = [{
    /// Return the number of iteration dimensions.
    unsigned getIterRank() const { return getIterTripCounts().size(); }

    /// Return the number of element (tile) dimensions.
    unsigned getElementRank() const { return getElementShape().size(); }

    /// Compute the full data tensor shape by multiplying element shape
    /// dimensions by the corresponding trip counts through the iteration map.
    llvm::SmallVector<int64_t> getDataShape() const;

    /// Return the total number of tiles (product of all trip counts).
    int64_t getTotalElements() const;

    /// Check whether this ITensor is layout-compatible with another (all fields
    /// match), meaning they can be directly connected via a FIFO.
    bool isLayoutCompatible(ITensorType other) const;
  }];
}

#endif // SCALEHLS_DIALECT_HLS_HLSTYPES_TD
