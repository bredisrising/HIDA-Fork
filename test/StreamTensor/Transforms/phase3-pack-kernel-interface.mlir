// Phase 3 pass test: PackKernelInterfacePass
//
// NOTE: tensor.pack/unpack are not available in this MLIR version.
// The pass currently acts as a no-op — kernels pass through unchanged.
//
// RUN: scalehls-opt %s \
// RUN:   --scalehls-pack-kernel-interface \
// RUN:   | FileCheck %s --check-prefix=PACK

#map = affine_map<(d0, d1) -> (d0, d1)>

// PACK-LABEL: func.func @pack_kernel_interface
func.func @pack_kernel_interface(
    %A: tensor<64x64xf32>,
    %C: tensor<64x64xf32>) -> tensor<64x64xf32> {
  // PACK: hls.st.kernel
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

