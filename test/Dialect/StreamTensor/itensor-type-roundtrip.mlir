// RUN: scalehls-opt %s | scalehls-opt | FileCheck %s

// CHECK-LABEL: func.func @itensor_type_roundtrip
func.func @itensor_type_roundtrip(
    %arg0: tensor<64x64xf16>, %arg1: tensor<64x64xf16>
) -> tensor<64x64xf16> {
  // CHECK: hls.dataflow.kernel
  %0 = hls.dataflow.kernel(%arg0, %arg1) : (tensor<64x64xf16>, tensor<64x64xf16>) -> tensor<64x64xf16> {
    // CHECK: hls.itensor.empty
    %it0 = hls.itensor.empty : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>

    // CHECK: hls.itensor.instance
    %it1 = hls.itensor.instance {depth = 2 : i64} : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>

    // CHECK: hls.dataflow.st_task
    %it2 = hls.dataflow.st_task ins(%it0 : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>) : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> {
      // CHECK: hls.itensor.write
      %tile = arith.constant dense<0.0> : tensor<16x16xf16>
      %written = hls.itensor.write %tile, %it0 : tensor<16x16xf16>, !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> -> !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>

      hls.dataflow.st_yield %written : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
    }

    // CHECK: hls.itensor.fork
    %fork:2 = hls.itensor.fork %it2 : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> to !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>, !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>

    // CHECK: hls.itensor.read
    %init = arith.constant dense<0.0> : tensor<16x16xf16>
    %read = hls.itensor.read %fork#0, %init : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>, tensor<16x16xf16> -> tensor<16x16xf16>

    %result = arith.constant dense<0.0> : tensor<64x64xf16>
    hls.dataflow.yield %result : tensor<64x64xf16>
  }
  return %0 : tensor<64x64xf16>
}

// CHECK-LABEL: func.func @itensor_converter
func.func @itensor_converter() {
  // CHECK: hls.itensor.converter
  %src = hls.itensor.empty : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  %conv = hls.itensor.converter %src : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> to !hls.itensor<f16, [8, 8], [8, 8] * [8, 8], affine_map<(d0, d1) -> (d0, d1)>>
  return
}

// CHECK-LABEL: func.func @itensor_cast
func.func @itensor_cast() {
  // CHECK: hls.itensor.cast
  %src = hls.itensor.empty : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  %cast = hls.itensor.cast %src : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> to !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  return
}

// CHECK-LABEL: func.func @itensor_stream_bridge
func.func @itensor_stream_bridge() {
  // CHECK: hls.itensor.to_stream
  %it = hls.itensor.empty : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  %stream = hls.itensor.to_stream %it : !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>> to !hls.stream<f16, 4>

  // CHECK: hls.stream.to_itensor
  %it2 = hls.stream.to_itensor %stream : !hls.stream<f16, 4> to !hls.itensor<f16, [16, 16], [4, 4] * [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
  return
}

// CHECK-LABEL: func.func @stream_buffer_and_cast
func.func @stream_buffer_and_cast() {
  // CHECK: hls.dataflow.stream_buffer
  %buf = hls.dataflow.stream_buffer {depth = 2 : i32} : memref<16x16xf16>

  // CHECK: hls.dataflow.stream_cast
  %s1 = hls.dataflow.stream {depth = 4 : i32} : !hls.stream<f16, 4>
  %s2 = hls.dataflow.stream_cast %s1 : !hls.stream<f16, 4> to !hls.stream<f16, 8>
  return
}

// CHECK-LABEL: func.func @kernel_timing_attr
func.func @kernel_timing_attr() attributes {
  // CHECK: kernel_timing = #hls.kernel_timing<ii = 3, delay = 10, latency = 100>
  kernel_timing = #hls.kernel_timing<ii = 3, delay = 10, latency = 100>
} {
  return
}
