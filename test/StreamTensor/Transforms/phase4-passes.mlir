// Tests for Phase 4 StreamTensor optimization passes.

// ============================================================
// Test 1: ConvertEmptyToInstance
// Run: scalehls-opt %s -scalehls-convert-empty-to-instance | FileCheck %s --check-prefix=EMPTY
// ============================================================

// EMPTY-LABEL: func.func @convert_empty
// EMPTY:       hls.itensor.instance depth 2
// EMPTY-NOT:   hls.itensor.empty

// RUN: scalehls-opt %s -scalehls-convert-empty-to-instance | FileCheck %s --check-prefix=EMPTY
func.func @convert_empty() -> !hls.itensor<f32, [4, 4], [2, 2], [2, 2], affine_map<(d0, d1) -> (d0, d1)>> {
  %0 = hls.itensor.empty : !hls.itensor<f32, [4, 4], [2, 2], [2, 2], affine_map<(d0, d1) -> (d0, d1)>>
  return %0 : !hls.itensor<f32, [4, 4], [2, 2], [2, 2], affine_map<(d0, d1) -> (d0, d1)>>
}

// ============================================================
// Test 2: EnsureITensorSingleUse — insert fork for multi-consumer
// RUN: scalehls-opt %s -scalehls-ensure-itensor-single-use | FileCheck %s --check-prefix=FORK
// ============================================================

// FORK-LABEL: func.func @ensure_single_use
// FORK:       hls.itensor.fork
// FORK-NOT:   hls.itensor.empty

!it = !hls.itensor<f32, [4], [4], [1], affine_map<(d0) -> (d0)>>

func.func @ensure_single_use(%a: !it, %b: !it) -> (!it, !it) {
  // %inst has two consumers: the two write ops below. A fork must be inserted.
  %inst = hls.itensor.instance depth 4 : !it
  %r0 = hls.itensor.cast %inst : !it to !it
  %r1 = hls.itensor.cast %inst : !it to !it
  return %r0, %r1 : !it, !it
}

// ============================================================
// Test 3: FoldITensor — matching access patterns fold away the FIFO
// RUN: scalehls-opt %s -scalehls-fold-itensor | FileCheck %s --check-prefix=FOLD
// ============================================================

// FOLD-LABEL: func.func @fold_matching
// FOLD-NOT:   hls.itensor.write
// FOLD-NOT:   hls.itensor.read
// FOLD:       return %{{.*}} : tensor<4xf32>

func.func @fold_matching(%val: tensor<4xf32>, %fifo: !it) -> tensor<4xf32> {
  %written = hls.itensor.write %val into %fifo : tensor<4xf32> into !it
  %out = hls.itensor.read %written : !it -> tensor<4xf32>
  return %out : tensor<4xf32>
}

// ============================================================
// Test 4: SimplifyTaskStructure — inline passthrough task
// RUN: scalehls-opt %s -scalehls-simplify-task-structure | FileCheck %s --check-prefix=SIMPLIFY
// ============================================================

// SIMPLIFY-LABEL: func.func @simplify_passthrough
// SIMPLIFY-NOT:   hls.st.task
// SIMPLIFY:       return %{{.*}} : !{{.*}}itensor

func.func @simplify_passthrough(%it: !it) -> !it {
  %0 = hls.st.task inits(%it : !it) -> !it {
    hls.st.yield %it : !it
  }
  return %0 : !it
}
