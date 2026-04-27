// Phase 3 pass test: PackKernelInterfacePass
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-pack-kernel-interface \
// RUN:   | FileCheck %s --check-prefix=PACK

// ============================================================
// Section 1: static-shaped kernel input — no annotation added.
// ============================================================

#map = affine_map<(d0, d1) -> (d0, d1)>

// PACK-LABEL: func.func @pack_static
func.func @pack_static(
    %A: tensor<64x64xf32>,
    %C: tensor<64x64xf32>) -> tensor<64x64xf32> {
  // Input pack: expand_shape → linalg transpose (no pad needed for static)
  // PACK:      tensor.expand_shape
  // PACK-SAME: tensor<64x64xf32> into tensor<4x16x4x16xf32>
  // PACK:      ins(%{{.*}} : tensor<4x16x4x16xf32>) outs(%{{.*}} : tensor<4x4x16x16xf32>)
  // Output init pack: same
  // PACK:      tensor.expand_shape
  // PACK-SAME: tensor<64x64xf32> into tensor<4x16x4x16xf32>
  // PACK:      ins(%{{.*}} : tensor<4x16x4x16xf32>) outs(%{{.*}} : tensor<4x4x16x16xf32>)
  // Kernel takes fully-static packed tensors
  // PACK:      hls.st.kernel @kernel
  // PACK-SAME: tensor<4x4x16x16xf32>
  // PACK-SAME: -> tensor<4x4x16x16xf32>
  // Carried block arg updated to packed type
  // PACK:      %arg{{.*}}: tensor<4x4x16x16xf32>
  // PACK:      hls.st.yield %{{.*}} : tensor<4x4x16x16xf32>
  // Unpack: linalg transpose → collapse_shape (no extract_slice for static)
  // PACK:      ins(%{{.*}} : tensor<4x4x16x16xf32>) outs(%{{.*}} : tensor<4x16x4x16xf32>)
  // PACK:      tensor.collapse_shape
  // PACK-SAME: tensor<4x16x4x16xf32> into tensor<64x64xf32>
  // PACK-NOT:  tensor.extract_slice
  %0 = hls.st.kernel @kernel inputs(%A : tensor<64x64xf32>) output_inits(%C : tensor<64x64xf32>) -> tensor<64x64xf32> {
  ^bb0(%arg0: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map>,
       %arg1: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map>,
       %arg2: tensor<64x64xf32>):
    hls.st.task kernel {
      %1 = hls.itensor.read %arg0 : <f32, [16, 16], [4, 4], [16, 16], #map> -> tensor<16x16xf32>
      %2 = hls.itensor.read %arg1 : <f32, [16, 16], [4, 4], [16, 16], #map> -> tensor<16x16xf32>
      %3 = linalg.generic {
          indexing_maps = [#map, #map],
          iterator_types = ["parallel", "parallel"]
        } ins(%1 : tensor<16x16xf32>) outs(%2 : tensor<16x16xf32>) {
        ^bb0(%in: f32, %out: f32):
          %sum = arith.addf %in, %out : f32
          linalg.yield %sum : f32
        } -> tensor<16x16xf32>
      %4 = hls.itensor.write %3 into %arg1 : tensor<16x16xf32> into <f32, [16, 16], [4, 4], [16, 16], #map>
    }
    hls.st.yield %arg2 : tensor<64x64xf32>
  }
  return %0 : tensor<64x64xf32>
}

// -----

// ============================================================
// Section 2: dynamic-shaped kernel input — pack_input_0 annotation added.
// ============================================================

#map2 = affine_map<(d0, d1) -> (d0, d1)>

// PACK-LABEL: func.func @pack_dynamic
func.func @pack_dynamic(
    %A: tensor<?x?xf32>,
    %C: tensor<?x?xf32>) -> tensor<?x?xf32> {
  // Input pack: pad → expand_shape → linalg transpose
  // PACK:      tensor.pad
  // PACK:      tensor.expand_shape
  // PACK:      ins(%{{.*}} : tensor<?x16x?x16xf32>) outs(%{{.*}} : tensor<?x?x16x16xf32>)
  // Output init pack: pad → expand_shape → linalg transpose
  // PACK:      tensor.pad
  // PACK:      tensor.expand_shape
  // PACK:      ins(%{{.*}} : tensor<?x16x?x16xf32>) outs(%{{.*}} : tensor<?x?x16x16xf32>)
  // Kernel takes packed tensors
  // PACK:      hls.st.kernel @kernel
  // PACK-SAME: tensor<?x?x16x16xf32>
  // PACK-SAME: -> tensor<?x?x16x16xf32>
  // Carried block arg is updated to packed type
  // PACK:      %arg{{.*}}: tensor<?x?x16x16xf32>
  // PACK:      hls.st.yield %{{.*}} : tensor<?x?x16x16xf32>
  // Unpack after kernel: linalg transpose → collapse_shape → extract_slice
  // PACK:      ins(%{{.*}} : tensor<?x?x16x16xf32>) outs(%{{.*}} : tensor<?x16x?x16xf32>)
  // PACK:      tensor.collapse_shape
  // PACK:      tensor.extract_slice
  %0 = hls.st.kernel @kernel inputs(%A : tensor<?x?xf32>) output_inits(%C : tensor<?x?xf32>) -> tensor<?x?xf32> {
  ^bb0(%arg0: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map2>,
       %arg1: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map2>,
       %arg2: tensor<?x?xf32>):
    hls.st.task kernel {
      %1 = hls.itensor.read %arg0 : <f32, [16, 16], [4, 4], [16, 16], #map2> -> tensor<16x16xf32>
      %2 = hls.itensor.read %arg1 : <f32, [16, 16], [4, 4], [16, 16], #map2> -> tensor<16x16xf32>
      %3 = linalg.generic {
          indexing_maps = [#map2, #map2],
          iterator_types = ["parallel", "parallel"]
        } ins(%1 : tensor<16x16xf32>) outs(%2 : tensor<16x16xf32>) {
        ^bb0(%in: f32, %out: f32):
          %sum = arith.addf %in, %out : f32
          linalg.yield %sum : f32
        } -> tensor<16x16xf32>
      %4 = hls.itensor.write %3 into %arg1 : tensor<16x16xf32> into <f32, [16, 16], [4, 4], [16, 16], #map2>
    }
    hls.st.yield %arg2 : tensor<?x?xf32>
  }
  return %0 : tensor<?x?xf32>
}

