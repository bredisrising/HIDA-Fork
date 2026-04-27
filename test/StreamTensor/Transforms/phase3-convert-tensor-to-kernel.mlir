// Phase 3 pass test: ConvertTensorToKernel
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-convert-tensor-to-kernel \
// RUN:   | FileCheck %s --check-prefix=TRANSFORM

// ============================================================
// Step 3.2: ConvertTensorToKernelPass
// Input:  non-linalg tensor ops (collapse_shape) between functions
// Output: hls.st.kernel wrapping the tensor op as a pass-through
// ============================================================

// TRANSFORM-LABEL: func.func @convert_tensor_to_kernel
func.func @convert_tensor_to_kernel(%src: tensor<4x4x16x16xf32>) -> tensor<256x16xf32> {
  // After ConvertTensorToKernelPass, this collapse_shape should be
  // wrapped in an hls.st.kernel pass-through kernel.
  // TRANSFORM: hls.st.kernel
  %collapsed = tensor.collapse_shape %src [[0, 1, 2], [3]]
      : tensor<4x4x16x16xf32> into tensor<256x16xf32>
  return %collapsed : tensor<256x16xf32>
}

// -----

// ============================================================
// expand_shape: inverse of collapse_shape — splits a flat dimension
// into multiple dimensions.
// ============================================================

// TRANSFORM-LABEL: func.func @convert_expand_shape
func.func @convert_expand_shape(%src: tensor<256x16xf32>) -> tensor<4x4x16x16xf32> {
  // TRANSFORM: hls.st.kernel
  // TRANSFORM: tensor.expand_shape
  %expanded = tensor.expand_shape %src [[0, 1, 2], [3]]
      : tensor<256x16xf32> into tensor<4x4x16x16xf32>
  return %expanded : tensor<4x4x16x16xf32>
}

// -----

// ============================================================
// pad: adds a constant-value border around a tensor.
// ============================================================

// TRANSFORM-LABEL: func.func @convert_pad
func.func @convert_pad(%src: tensor<14x14xf32>) -> tensor<16x16xf32> {
  %zero = arith.constant 0.0 : f32
  // TRANSFORM: hls.st.kernel
  // TRANSFORM: tensor.pad
  %padded = tensor.pad %src low[1, 1] high[1, 1] {
  ^bb0(%i: index, %j: index):
    tensor.yield %zero : f32
  } : tensor<14x14xf32> to tensor<16x16xf32>
  return %padded : tensor<16x16xf32>
}
