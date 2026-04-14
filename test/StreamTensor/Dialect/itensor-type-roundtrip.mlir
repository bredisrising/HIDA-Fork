// RUN: scalehls-opt %s | scalehls-opt | FileCheck %s

// Test basic ITensorType roundtrip with identity iteration map.
// CHECK-LABEL: func.func @itensor_basic
// CHECK-SAME: %{{.*}}: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
func.func @itensor_basic(%arg0: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d0, d1)>>) -> !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d0, d1)>> {
  return %arg0 : !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
}

// Test ITensorType with integer element type.
// CHECK-LABEL: func.func @itensor_i8
// CHECK-SAME: %{{.*}}: !hls.itensor<i8, [8], [32], [8], affine_map<(d0) -> (d0)>>
func.func @itensor_i8(%arg0: !hls.itensor<i8, [8], [32], [8], affine_map<(d0) -> (d0)>>) -> !hls.itensor<i8, [8], [32], [8], affine_map<(d0) -> (d0)>> {
  return %arg0 : !hls.itensor<i8, [8], [32], [8], affine_map<(d0) -> (d0)>>
}

// Test ITensorType with 3D element shape and non-trivial map.
// CHECK-LABEL: func.func @itensor_3d
// CHECK-SAME: %{{.*}}: !hls.itensor<f16, [4, 4, 8], [2, 2, 1], [4, 4, 8], affine_map<(d0, d1, d2) -> (d0, d1, d2)>>
func.func @itensor_3d(%arg0: !hls.itensor<f16, [4, 4, 8], [2, 2, 1], [4, 4, 8], affine_map<(d0, d1, d2) -> (d0, d1, d2)>>) -> !hls.itensor<f16, [4, 4, 8], [2, 2, 1], [4, 4, 8], affine_map<(d0, d1, d2) -> (d0, d1, d2)>> {
  return %arg0 : !hls.itensor<f16, [4, 4, 8], [2, 2, 1], [4, 4, 8], affine_map<(d0, d1, d2) -> (d0, d1, d2)>>
}

// Test ITensorType with permuted iteration map (transposed access).
// CHECK-LABEL: func.func @itensor_permuted_map
// CHECK-SAME: %{{.*}}: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d1, d0)>>
func.func @itensor_permuted_map(%arg0: !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d1, d0)>>) -> !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d1, d0)>> {
  return %arg0 : !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d1, d0)>>
}

// Test 1D ITensorType (single iteration dimension).
// CHECK-LABEL: func.func @itensor_1d
// CHECK-SAME: %{{.*}}: !hls.itensor<f32, [64], [16], [64], affine_map<(d0) -> (d0)>>
func.func @itensor_1d(%arg0: !hls.itensor<f32, [64], [16], [64], affine_map<(d0) -> (d0)>>) -> !hls.itensor<f32, [64], [16], [64], affine_map<(d0) -> (d0)>> {
  return %arg0 : !hls.itensor<f32, [64], [16], [64], affine_map<(d0) -> (d0)>>
}

// Test KernelTimingAttr roundtrip.
// CHECK-LABEL: func.func @kernel_timing_attr
// CHECK-SAME: attributes {kernel_timing = #hls.kernel_timing<ii = 1, delay = 5, latency = 100>}
func.func @kernel_timing_attr() attributes {kernel_timing = #hls.kernel_timing<ii = 1, delay = 5, latency = 100>} {
  return
}

// Test FIFOSizingResultAttr roundtrip.
// CHECK-LABEL: func.func @fifo_sizing_attr
// CHECK-SAME: attributes {fifo_sizing = #hls.fifo_sizing<depth = 16, max_tokens = 8>}
func.func @fifo_sizing_attr() attributes {fifo_sizing = #hls.fifo_sizing<depth = 16, max_tokens = 8>} {
  return
}
