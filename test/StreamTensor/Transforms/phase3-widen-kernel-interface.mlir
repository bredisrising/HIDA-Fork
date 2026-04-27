// Phase 3 pass tests: WidenKernelInterface
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-widen-kernel-interface \
// RUN:   | FileCheck %s --check-prefix=WIDEN

// ============================================================
// Section 1: block arg type update only (no read/write ops)
//
// Before: !hls.itensor<i8, [16, 64], [4, 1], [16, 64], (d0,d1)->(d0,d1)>
// After:  !hls.itensor<vector<64xi8>, [16, 1], [4, 1], [16, 64], (d0,d1)->(d0,d1)>
// ============================================================

// WIDEN-LABEL: func.func @widen_block_arg_only
func.func @widen_block_arg_only(%input: tensor<64x64xi8>) {
  hls.st.kernel @elementwise inputs(%input : tensor<64x64xi8>) {
    // WIDEN: ^bb0(%{{.*}}: !hls.itensor<vector<64xi8>, [16, 1],
    ^bb0(%in: !hls.itensor<i8, [16, 64], [4, 1], [16, 64],
               affine_map<(d0, d1) -> (d0, d1)>>):
      hls.st.yield
  }
  return
}

// -----

// ============================================================
// Section 2: ITensorReadOp — result type updated, cast inserted
//
// After widen the read returns tensor<16x1xvector<64xi8>>.
// An unrealized_conversion_cast bridges back to tensor<16x64xi8>
// so downstream compute ops are unaffected.
// ============================================================

#map = affine_map<(d0, d1) -> (d0, d1)>

// WIDEN-LABEL: func.func @widen_with_read
func.func @widen_with_read(%input: tensor<64x64xi8>) {
  hls.st.kernel @read_kernel inputs(%input : tensor<64x64xi8>) {
    // WIDEN: ^bb0(%[[ARG:.*]]: !hls.itensor<vector<64xi8>, [16, 1],
    ^bb0(%in: !hls.itensor<i8, [16, 64], [4, 1], [16, 64], #map>):
      hls.st.task kernel {
        // Read result type is updated to match widened element type.
        // WIDEN: hls.itensor.read %[[ARG]] : <vector<64xi8>, [16, 1],
        // WIDEN-SAME: -> tensor<16x1xvector<64xi8>>
        // Bridge cast converts back to the original scalar tile type.
        // WIDEN: unrealized_conversion_cast
        // WIDEN-SAME: tensor<16x1xvector<64xi8>> to tensor<16x64xi8>
        %tile = hls.itensor.read %in : <i8, [16, 64], [4, 1], [16, 64], #map>
                  -> tensor<16x64xi8>
        hls.st.yield
      }
      hls.st.yield
  }
  return
}

// -----

// ============================================================
// Section 3: ITensorWriteOp — cast inserted before value, result type updated
//
// After widen the write expects tensor<16x1xvector<64xi8>>.
// An unrealized_conversion_cast widens the compute output
// tensor<16x64xi8> before the write op.
// ============================================================

#map2 = affine_map<(d0, d1) -> (d0, d1)>

// WIDEN-LABEL: func.func @widen_with_write
func.func @widen_with_write(%input: tensor<64x64xi8>) {
  hls.st.kernel @write_kernel inputs(%input : tensor<64x64xi8>) {
    // WIDEN: ^bb0(%[[ARG:.*]]: !hls.itensor<vector<64xi8>, [16, 1],
    ^bb0(%out: !hls.itensor<i8, [16, 64], [4, 1], [16, 64], #map2>):
      hls.st.task kernel {
        // WIDEN: unrealized_conversion_cast
        // WIDEN-SAME: tensor<16x64xi8> to tensor<16x1xvector<64xi8>>
        // WIDEN: hls.itensor.write %{{.*}} into %[[ARG]]
        // WIDEN-SAME: tensor<16x1xvector<64xi8>> into <vector<64xi8>, [16, 1],
        %tile = tensor.empty() : tensor<16x64xi8>
        hls.itensor.write %tile into %out
          : tensor<16x64xi8> into <i8, [16, 64], [4, 1], [16, 64], #map2>
        hls.st.yield
      }
      hls.st.yield
  }
  return
}

// -----

// ============================================================
// Section 4: Non-widenable types are left unchanged
//
// f32 with bus-width=512 → vectorLen=16; element shape [16, 15] — last dim
// is not divisible by 16, so the itensor is left unchanged.
// ============================================================

#map3 = affine_map<(d0, d1) -> (d0, d1)>

// WIDEN-LABEL: func.func @no_widen_indivisible
func.func @no_widen_indivisible(%input: tensor<64x60xf32>) {
  hls.st.kernel @no_widen inputs(%input : tensor<64x60xf32>) {
    // WIDEN: ^bb0(%{{.*}}: !hls.itensor<f32, [16, 15],
    ^bb0(%in: !hls.itensor<f32, [16, 15], [4, 4], [16, 15], #map3>):
      hls.st.yield
  }
  return
}
