// Phase 3 pass tests: WidenKernelInterface
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-widen-kernel-interface \
// RUN:   | FileCheck %s --check-prefix=WIDEN

// ============================================================
// Step 3.4: WidenKernelInterfacePass
// Input:  hls.st.kernel with ITensorType block args with scalar elementType
// Output: same kernel but block args have vector<64xi8> elementType,
//         elementShape.back() divided by 64
//
// Before: !hls.itensor<i8, [16, 64], [4, 1], [16, 64], affine_map<(d0,d1)->(d0,d1)>>
// After:  !hls.itensor<vector<64xi8>, [16, 1], [4, 1], [16, 64], affine_map<(d0,d1)->(d0,d1)>>
// ============================================================

// WIDEN-LABEL: func.func @widen_kernel_interface
func.func @widen_kernel_interface(%input: tensor<64x64xi8>) {
  hls.st.kernel @elementwise inputs(%input : tensor<64x64xi8>) {
    // After WidenKernelInterfacePass (bus-width=512, i8 → vector<64xi8>):
    //   elementShape [16, 64] → [16, 1], elementType i8 → vector<64xi8>
    // WIDEN: !hls.itensor<vector<64xi8>, [16, 1],
    ^bb0(%in: !hls.itensor<i8, [16, 64], [4, 1], [16, 64],
               affine_map<(d0, d1) -> (d0, d1)>>):
      hls.st.yield
  }
}
