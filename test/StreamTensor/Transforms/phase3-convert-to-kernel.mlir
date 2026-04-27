// Phase 3 pass tests: ConvertLinalgToKernel + ConvertTensorToKernel
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-convert-linalg-to-kernel \
// RUN:   --scalehls-convert-tensor-to-kernel \
// RUN:   | FileCheck %s --check-prefix=TRANSFORM

// ============================================================
// Section 1: tiled linalg.generic → hls.st.kernel
// ConvertLinalgToKernel fires; ConvertTensorToKernel is a no-op.
// ============================================================

// TRANSFORM-LABEL: func.func @linalg_to_kernel
func.func @linalg_to_kernel(
    %A: tensor<64x64xf32>,
    %C: tensor<64x64xf32>) -> tensor<64x64xf32> {

  %c0 = arith.constant 0 : index
  %c16 = arith.constant 16 : index
  %c64 = arith.constant 64 : index

  // TRANSFORM: hls.st.kernel
  // TRANSFORM: hls.itensor
  %result = scf.for %i = %c0 to %c64 step %c16
      iter_args(%C_i = %C) -> tensor<64x64xf32> {
    %result_j = scf.for %j = %c0 to %c64 step %c16
        iter_args(%C_j = %C_i) -> tensor<64x64xf32> {
      %tile_A = tensor.extract_slice %A[%i, %j][16, 16][1, 1]
          : tensor<64x64xf32> to tensor<16x16xf32>
      %tile_C = tensor.extract_slice %C_j[%i, %j][16, 16][1, 1]
          : tensor<64x64xf32> to tensor<16x16xf32>
      %new_tile = linalg.generic {
          indexing_maps = [
            affine_map<(d0, d1) -> (d0, d1)>,
            affine_map<(d0, d1) -> (d0, d1)>
          ],
          iterator_types = ["parallel", "parallel"]
        } ins(%tile_A : tensor<16x16xf32>) outs(%tile_C : tensor<16x16xf32>) {
        ^bb0(%a: f32, %c: f32):
          %sum = arith.addf %a, %c : f32
          linalg.yield %sum : f32
        } -> tensor<16x16xf32>
      %updated = tensor.insert_slice %new_tile into %C_j[%i, %j][16, 16][1, 1]
          : tensor<16x16xf32> into tensor<64x64xf32>
      scf.yield %updated : tensor<64x64xf32>
    }
    scf.yield %result_j : tensor<64x64xf32>
  }
  return %result : tensor<64x64xf32>
}

// -----

// ============================================================
// Section 2: standalone collapse_shape → hls.st.kernel
// ConvertTensorToKernel fires; ConvertLinalgToKernel is a no-op.
// ============================================================

// TRANSFORM-LABEL: func.func @tensor_collapse_to_kernel
func.func @tensor_collapse_to_kernel(%src: tensor<4x4x16x16xf32>) -> tensor<256x16xf32> {
  // TRANSFORM: hls.st.kernel
  // TRANSFORM: tensor.collapse_shape
  %collapsed = tensor.collapse_shape %src [[0, 1, 2], [3]]
      : tensor<4x4x16x16xf32> into tensor<256x16xf32>
  return %collapsed : tensor<256x16xf32>
}

// -----

// ============================================================
// Section 3: standalone expand_shape → hls.st.kernel
// ============================================================

// TRANSFORM-LABEL: func.func @tensor_expand_to_kernel
func.func @tensor_expand_to_kernel(%src: tensor<256x16xf32>) -> tensor<4x4x16x16xf32> {
  // TRANSFORM: hls.st.kernel
  // TRANSFORM: tensor.expand_shape
  %expanded = tensor.expand_shape %src [[0, 1, 2], [3]]
      : tensor<256x16xf32> into tensor<4x4x16x16xf32>
  return %expanded : tensor<4x4x16x16xf32>
}

// -----

// ============================================================
// Section 4: pipeline — linalg kernel feeds a collapse_shape kernel.
// Both passes fire in the same function.
// ============================================================

// TRANSFORM-LABEL: func.func @pipeline_linalg_then_collapse
func.func @pipeline_linalg_then_collapse(
    %A: tensor<64x64xf32>,
    %C: tensor<64x64xf32>) -> tensor<4096xf32> {

  %c0 = arith.constant 0 : index
  %c16 = arith.constant 16 : index
  %c64 = arith.constant 64 : index

  // TRANSFORM: hls.st.kernel
  %result = scf.for %i = %c0 to %c64 step %c16
      iter_args(%C_i = %C) -> tensor<64x64xf32> {
    %result_j = scf.for %j = %c0 to %c64 step %c16
        iter_args(%C_j = %C_i) -> tensor<64x64xf32> {
      %tile_A = tensor.extract_slice %A[%i, %j][16, 16][1, 1]
          : tensor<64x64xf32> to tensor<16x16xf32>
      %tile_C = tensor.extract_slice %C_j[%i, %j][16, 16][1, 1]
          : tensor<64x64xf32> to tensor<16x16xf32>
      %new_tile = linalg.generic {
          indexing_maps = [
            affine_map<(d0, d1) -> (d0, d1)>,
            affine_map<(d0, d1) -> (d0, d1)>
          ],
          iterator_types = ["parallel", "parallel"]
        } ins(%tile_A : tensor<16x16xf32>) outs(%tile_C : tensor<16x16xf32>) {
        ^bb0(%a: f32, %c: f32):
          %sum = arith.addf %a, %c : f32
          linalg.yield %sum : f32
        } -> tensor<16x16xf32>
      %updated = tensor.insert_slice %new_tile into %C_j[%i, %j][16, 16][1, 1]
          : tensor<16x16xf32> into tensor<64x64xf32>
      scf.yield %updated : tensor<64x64xf32>
    }
    scf.yield %result_j : tensor<64x64xf32>
  }

  // TRANSFORM: tensor.collapse_shape
  %collapsed = tensor.collapse_shape %result [[0, 1]]
      : tensor<64x64xf32> into tensor<4096xf32>
  return %collapsed : tensor<4096xf32>
}
