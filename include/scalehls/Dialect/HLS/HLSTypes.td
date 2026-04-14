//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//

#ifndef SCALEHLS_DIALECT_HLS_HLSTYPES_TD
#define SCALEHLS_DIALECT_HLS_HLSTYPES_TD

include "scalehls/Dialect/HLS/HLSAttributes.td"
include "mlir/IR/BuiltinTypes.td"

class HLSType<string name, list<Trait> traits = []> :
    TypeDef<HLSDialect, name, traits>;

//===----------------------------------------------------------------------===//
// ITensor Type
//===----------------------------------------------------------------------===//

def ITensorType : HLSType<"ITensor"> {
  let summary = "An iterative tensor (streaming tile) type";
  let description = [{
    Represents a streaming tensor tile type that encodes the element shape,
    iteration space, and an affine iteration map. This is the core type for
    StreamTensor dataflow communication.

    The type is parameterized by:
    - elementType: the scalar data type of each element (e.g., f16, i8)
    - elementShape: the shape of each tensor tile produced per iteration
    - iterTripCounts: the trip count for each iteration dimension
    - iterSteps: the step size for each iteration dimension
    - iterationMap: an affine map from iteration indices to data-space indices

    Syntax:
      `!hls.itensor<` element-type `,` `[` element-shape `]` `,`
        `[` iter-trip-counts `]` `*` `[` iter-steps `]` `,`
         affine-map `>`

    Example:
      !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  }];
  let mnemonic = "itensor";

  let parameters = (ins
    "mlir::Type":$elementType,
    ArrayRefParameter<"int64_t">:$elementShape,
    ArrayRefParameter<"int64_t">:$iterTripCounts,
    ArrayRefParameter<"int64_t">:$iterSteps,
    "mlir::AffineMap":$iterationMap
  );

  let hasCustomAssemblyFormat = 1;
  let genVerifyDecl = 1;

  let extraClassDeclaration = [{
    /// Return the number of iteration dimensions.
    unsigned getIterRank() const { return getIterTripCounts().size(); }

    /// Return the number of element shape dimensions (data dimensions).
    unsigned getElementRank() const { return getElementShape().size(); }

    /// Compute the full data shape from element shape and iteration space.
    SmallVector<int64_t> getDataShape() const;

    /// Return the total number of tensor tiles produced.
    int64_t getTotalTiles() const;

    /// Check whether two itensor types are layout-compatible (can connect
    /// directly via FIFO without a converter).
    bool isLayoutCompatible(ITensorType other) const;
  }];
}

def AnyITensor : Type<
    CPred<"$_self.isa<mlir::scalehls::hls::ITensorType>()">,
    "itensor type",
    "mlir::scalehls::hls::ITensorType"
>;

//===----------------------------------------------------------------------===//
// Stream Type
//===----------------------------------------------------------------------===//

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

#endif // SCALEHLS_DIALECT_HLS_HLSTYPES_TD
