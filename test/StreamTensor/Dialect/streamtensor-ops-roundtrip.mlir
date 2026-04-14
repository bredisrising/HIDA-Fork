// RUN: scalehls-opt %s | scalehls-opt | FileCheck %s

// Type aliases for readability.
!it2d = !hls.itensor<f32, [16, 16], [4, 4], [16, 16], affine_map<(d0, d1) -> (d0, d1)>>
!it1d = !hls.itensor<f32, [64], [16], [64], affine_map<(d0) -> (d0)>>

// ============================================================
// ITensor data operations
// ============================================================

// CHECK-LABEL: func.func @itensor_empty
func.func @itensor_empty() -> !it2d {
  // CHECK: hls.itensor.empty
  %0 = hls.itensor.empty : !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_instance
func.func @itensor_instance() -> !it2d {
  // CHECK: hls.itensor.instance depth 16
  %0 = hls.itensor.instance depth 16 : !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_read_no_init
func.func @itensor_read_no_init(%src: !it2d) -> tensor<16x16xf32> {
  // CHECK: hls.itensor.read %{{.*}} :
  %0 = hls.itensor.read %src : !it2d -> tensor<16x16xf32>
  return %0 : tensor<16x16xf32>
}

// CHECK-LABEL: func.func @itensor_read_with_init
func.func @itensor_read_with_init(%src: !it2d, %init: tensor<16x16xf32>) -> tensor<16x16xf32> {
  // CHECK: hls.itensor.read %{{.*}}, %{{.*}} : tensor<16x16xf32>
  %0 = hls.itensor.read %src, %init : tensor<16x16xf32> : !it2d -> tensor<16x16xf32>
  return %0 : tensor<16x16xf32>
}

// CHECK-LABEL: func.func @itensor_write
func.func @itensor_write(%val: tensor<16x16xf32>, %dest: !it2d) -> !it2d {
  // CHECK: hls.itensor.write %{{.*}} into %{{.*}} : tensor<16x16xf32> into
  %0 = hls.itensor.write %val into %dest : tensor<16x16xf32> into !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_cast
func.func @itensor_cast(%src: !it2d) -> !it2d {
  // CHECK: hls.itensor.cast %{{.*}} :
  %0 = hls.itensor.cast %src : !it2d to !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_reassociate
func.func @itensor_reassociate(%src: !it1d) -> !it1d {
  // CHECK: hls.itensor.reassociate %{{.*}} :
  %0 = hls.itensor.reassociate %src : !it1d to !it1d
  return %0 : !it1d
}

// CHECK-LABEL: func.func @itensor_buffer
func.func @itensor_buffer(%src: !it2d) -> !it2d {
  // CHECK: hls.itensor.buffer %{{.*}} loop_index 0 dim_index 1
  %0 = hls.itensor.buffer %src loop_index 0 dim_index 1 : !it2d to !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_chunk
func.func @itensor_chunk(%src: !it2d) -> (!it2d, !it2d) {
  // CHECK: hls.itensor.chunk %{{.*}} dim 0 size 2
  %0, %1 = hls.itensor.chunk %src dim 0 size 2 : !it2d -> !it2d, !it2d
  return %0, %1 : !it2d, !it2d
}

// CHECK-LABEL: func.func @itensor_concat
func.func @itensor_concat(%a: !it2d, %b: !it2d) -> !it2d {
  // CHECK: hls.itensor.concat %{{.*}}, %{{.*}} dim 0
  %0 = hls.itensor.concat %a, %b dim 0 : !it2d, !it2d -> !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @itensor_fork
func.func @itensor_fork(%src: !it2d) -> (!it2d, !it2d) {
  // CHECK: hls.itensor.fork %{{.*}} :
  %0, %1 = hls.itensor.fork %src : !it2d -> !it2d, !it2d
  return %0, %1 : !it2d, !it2d
}

// CHECK-LABEL: func.func @itensor_join
func.func @itensor_join(%a: !it2d, %b: !it2d) -> !it2d {
  // CHECK: hls.itensor.join %{{.*}}, %{{.*}} :
  %0 = hls.itensor.join %a, %b : !it2d, !it2d -> !it2d
  return %0 : !it2d
}

// ============================================================
// Stream bridge operations
// ============================================================

// CHECK-LABEL: func.func @itensor_to_stream
func.func @itensor_to_stream(%src: !it2d) -> !hls.stream<f32, 1> {
  // CHECK: hls.itensor.to_stream %{{.*}} :
  %0 = hls.itensor.to_stream %src : !it2d -> !hls.stream<f32, 1>
  return %0 : !hls.stream<f32, 1>
}

// CHECK-LABEL: func.func @stream_to_itensor
func.func @stream_to_itensor(%src: !hls.stream<f32, 1>) -> !it2d {
  // CHECK: hls.stream.to_itensor %{{.*}} :
  %0 = hls.stream.to_itensor %src : !hls.stream<f32, 1> -> !it2d
  return %0 : !it2d
}

// CHECK-LABEL: func.func @stream_buffer
func.func @stream_buffer() -> memref<16x16xf32> {
  // CHECK: hls.stream.buffer depth 2
  %0 = hls.stream.buffer depth 2 : memref<16x16xf32>
  return %0 : memref<16x16xf32>
}

// CHECK-LABEL: func.func @stream_cast
func.func @stream_cast(%src: !hls.stream<f32, 1>) -> !hls.stream<f32, 4> {
  // CHECK: hls.stream.cast %{{.*}} :
  %0 = hls.stream.cast %src : !hls.stream<f32, 1> to !hls.stream<f32, 4>
  return %0 : !hls.stream<f32, 4>
}

// ============================================================
// Structure operations
// ============================================================

// CHECK-LABEL: func.func @st_task_basic
func.func @st_task_basic(%it: !it2d) -> !it2d {
  // CHECK: hls.st.task inits
  %0 = hls.st.task inits(%it : !it2d) -> !it2d {
    hls.st.yield %it : !it2d
  }
  return %0 : !it2d
}

// CHECK-LABEL: func.func @st_task_kernel_flag
func.func @st_task_kernel_flag(%it: !it2d) -> !it2d {
  // CHECK: hls.st.task kernel inits
  %0 = hls.st.task kernel inits(%it : !it2d) -> !it2d {
    hls.st.yield %it : !it2d
  }
  return %0 : !it2d
}

// CHECK-LABEL: func.func @st_task_data_driven
func.func @st_task_data_driven() {
  // CHECK: hls.st.task data_driven
  hls.st.task data_driven {
    hls.st.yield
  }
  return
}

// CHECK-LABEL: func.func @st_kernel_basic
func.func @st_kernel_basic(%in: tensor<64x64xf32>) -> tensor<64x64xf32> {
  // CHECK: hls.st.kernel @my_kernel inputs
  %0 = hls.st.kernel @my_kernel inputs(%in : tensor<64x64xf32>) -> tensor<64x64xf32> {
    hls.st.yield %in : tensor<64x64xf32>
  }
  return %0 : tensor<64x64xf32>
}
