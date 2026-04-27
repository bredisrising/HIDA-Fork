// Demo inputs for VectorizeITensor.
//
// Run each test case with (example):
//   build/bin/scalehls-opt test/StreamTensor/Transforms/vectorize-demo.mlir \
//     -scalehls-vectorize-itensor="vector-width=4"
//
// What you'll see:
//   1. The ITensorInstanceOp's result type changes from
//        !hls.itensor<f32, [16, 16], ...>
//      to
//        !hls.itensor<vector<4xf32>, [16, 4], ...>
//
//   2. BUT the surrounding itensor.write / itensor.read ops still reference
//      tensor<16x16xf32> for their $value / $result, which no longer matches
//      the new FIFO element type. Verification will either fail or the output
//      will be visibly inconsistent.
//
//   3. There are NO vector.transfer_read / vector.transfer_write ops inserted
//      on either side. Those are the pack/unpack bridges the full pass should
//      add. Their absence is the core "incomplete" symptom.

// ============================================================
// Case 1 — Isolated instance (this one the starter pass handles cleanly)
// ============================================================
// No reads or writes touch %fifo, so there's no type inconsistency to show.
// You should see ONE line change: the instance's result type.

func.func @case1_isolated_instance()
    -> !hls.itensor<f32, [16, 16], [4, 8], [16, 16], affine_map<(d0,d1)->(d0,d1)>> {
  %fifo = hls.itensor.instance depth 2
        : !hls.itensor<f32, [16, 16], [4, 8], [16, 16], affine_map<(d0,d1)->(d0,d1)>>
  return %fifo
       : !hls.itensor<f32, [16, 16], [4, 8], [16, 16], affine_map<(d0,d1)->(d0,d1)>>
}

// ============================================================
// Case 2 — Write + Read around the instance (shows the missing pack/unpack)
// ============================================================
// The FIFO gets widened, but %tile is still tensor<16x16xf32> going into a
// FIFO that now expects vector<4xf32> elements. A full pass would wrap the
// write in a pack loop and the read in an unpack loop.

!it_scalar = !hls.itensor<f32, [16, 16], [4, 8], [16, 16],
                          affine_map<(d0,d1)->(d0,d1)>>

func.func @case2_write_then_read(%tile: tensor<16x16xf32>)
    -> tensor<16x16xf32> {
  %fifo = hls.itensor.instance depth 2 : !it_scalar

  // Producer: tile -> FIFO.
  // After the pass: the FIFO's element type is vector<4xf32>, but this write
  // still says "I'm giving you a tensor<16x16xf32>" — mismatch.
  %written = hls.itensor.write %tile into %fifo
           : tensor<16x16xf32> into !it_scalar

  // Consumer: FIFO -> tile.
  // After the pass: reads from a vector-carrying FIFO but still returns a
  // scalar tensor<16x16xf32> — mismatch on the other side.
  %out = hls.itensor.read %written : !it_scalar -> tensor<16x16xf32>

  return %out : tensor<16x16xf32>
}

// ============================================================
// Case 3 — Inner dim not divisible by vector width (pass should skip)
// ============================================================
// Innermost elementShape dim is 15, which isn't divisible by 4. The safety
// check in the pass should leave this instance untouched. Good negative test.

func.func @case3_non_divisible()
    -> !hls.itensor<f32, [16, 15], [4, 8], [16, 15], affine_map<(d0,d1)->(d0,d1)>> {
  %fifo = hls.itensor.instance depth 2
        : !hls.itensor<f32, [16, 15], [4, 8], [16, 15], affine_map<(d0,d1)->(d0,d1)>>
  return %fifo
       : !hls.itensor<f32, [16, 15], [4, 8], [16, 15], affine_map<(d0,d1)->(d0,d1)>>
}

// ============================================================
// Case 4 — Multiple instances, shows the pass hits every one it can
// ============================================================
// Two instances with the same shape. Both should get vectorized.

!it_a = !hls.itensor<f32, [8, 8], [2, 2], [8, 8], affine_map<(d0,d1)->(d0,d1)>>
!it_b = !hls.itensor<f32, [4, 16], [1, 1], [4, 16], affine_map<(d0,d1)->(d0,d1)>>

func.func @case4_multi() -> (!it_a, !it_b) {
  %fa = hls.itensor.instance depth 2 : !it_a
  %fb = hls.itensor.instance depth 4 : !it_b
  return %fa, %fb : !it_a, !it_b
}

// ============================================================
// Case 5 — Tile going through a task boundary (realistic shape)
// ============================================================
// This is closer to what Phase 3 would actually emit: a task with an init,
// a write into it, and a yield. Exercises the full structure: STTaskOp,
// itensor.write, STYieldOp all in one go. The write's type will become
// inconsistent with the instance — you'll see the problem clearly.

!it_c = !hls.itensor<f32, [16, 16], [4, 4], [16, 16],
                     affine_map<(d0,d1)->(d0,d1)>>

func.func @case5_task(%tile: tensor<16x16xf32>) -> !it_c {
  %fifo = hls.itensor.instance depth 2 : !it_c
  %done = hls.st.task inits(%fifo : !it_c) -> !it_c {
    %w = hls.itensor.write %tile into %fifo : tensor<16x16xf32> into !it_c
    hls.st.yield %w : !it_c
  }
  return %done : !it_c
}
