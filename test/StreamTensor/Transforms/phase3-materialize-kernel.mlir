// Phase 3 pass tests: MaterializeKernel (roundtrip)
//
// Step 3.5: roundtrip parse/print of kernel structure (pass not yet implemented).
// RUN: scalehls-opt %s --split-input-file | FileCheck %s --check-prefix=ROUNDTRIP

// ============================================================
// Step 3.5: MaterializeKernelPass
// Input:  hls.st.kernel after PackKernel + WidenKernel
// Output: kernel body contains DMA-in and DMA-out hls.st.task ops
//         wrapping scf.for loops with ITensorWriteOp / ITensorReadOp
//
// Before: kernel with ITensorType block args, no DMA tasks
// After:  kernel body = [dma_in_task, compute_task, dma_out_task]
// ============================================================

// ROUNDTRIP-LABEL: func.func @materialize_kernel
func.func @materialize_kernel(%input: tensor<64x64xf32>) {
  // Roundtrip test: kernel with no tasks parses and prints correctly.
  // (After MaterializeKernelPass this body would contain dma_in/dma_out tasks.)
  // ROUNDTRIP: hls.st.kernel @conv
  hls.st.kernel @conv inputs(%input : tensor<64x64xf32>) {
    hls.st.yield
  }
}
