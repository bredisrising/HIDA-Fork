// Phase 3 pass tests: PackKernelInterface
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-pack-kernel-interface \
// RUN:   | FileCheck %s --check-prefix=PACK

// ============================================================
// Step 3.3: PackKernelInterfacePass
// Input:  hls.st.kernel with dynamic tensor inputs
// Output: kernel annotated with pack_input_N / unpack_output_N attributes
// ============================================================

// PACK-LABEL: func.func @pack_kernel_interface
func.func @pack_kernel_interface(%input: tensor<?x?xf32>) {
  // After PackKernelInterfacePass, kernel passes through (dynamic tensor
  // annotation for MaterializeKernel is added as attributes).
  // PACK: hls.st.kernel
  hls.st.kernel @matmul inputs(%input : tensor<?x?xf32>) {
    hls.st.yield
  }
}
