// Phase 3 pass tests: MaterializeKernelPass
//
// RUN: scalehls-opt %s --split-input-file \
// RUN:   --scalehls-materialize-kernel \
// RUN:   | FileCheck %s --check-prefix=MAT
//
// What the pass does
// ==================
// Each hls.st.kernel has ITensorType block args that represent implicit DMAs
// at the kernel boundary — the ITensor is a typed FIFO that connects external
// memory to the on-chip compute task.  MaterializeKernelPass makes those DMAs
// explicit by injecting hls.st.task wrappers around scf.for loop nests:
//
//   Input boundary  → DMA-in  task: scf.for over trip counts,
//                               tensor.extract_slice from external tensor,
//                               hls.itensor.write pushes tiles into the FIFO.
//   Output boundary → DMA-out task: scf.for over trip counts,
//                               hls.itensor.read  pulls tiles from the FIFO,
//                               tensor.insert_slice assembles the output tensor.
//
// The final task order inside every kernel body is:
//   [DMA-in task(s)]  →  compute task (kernel)  →  [DMA-out task(s)]
//
// NOTE: --scalehls-materialize-kernel is not yet implemented.
// This file is the specification test that must pass once the pass exists.

// ============================================================
// Section 1: Input-only kernel — DMA-in task generated
//
// ITensor:  <f32, [16], [4], [16], (d0)->(d0)>
//   4 tiles × 16 elements, stride 16, identity map.
//
// Before: one compute task reads tiles from the ITensor block arg.
// After:  a DMA-in task is prepended.  It loops over the 4-trip
//         iteration space, extracts 16-element slices from the
//         external tensor, and pushes each via hls.itensor.write.
//         The compute task follows unchanged.
// ============================================================

#map1 = affine_map<(d0) -> (d0)>

// MAT-LABEL: func.func @materialize_input_only
func.func @materialize_input_only(%A: tensor<64xf32>) {
  hls.st.kernel @kernel inputs(%A : tensor<64xf32>) {
  ^bb0(%arg0: !hls.itensor<f32, [16], [4], [16], #map1>):
    // DMA-in task: not marked kernel, contains a loop and itensor.write.
    // MAT:      hls.st.task {
    // MAT:        scf.for
    // MAT:          tensor.extract_slice
    // MAT:          hls.itensor.write
    // Compute task: kernel attr preserved, body unchanged.
    // MAT:      hls.st.task kernel {
    // MAT:        hls.itensor.read
    hls.st.task kernel {
      %tile = hls.itensor.read %arg0
                : <f32, [16], [4], [16], #map1> -> tensor<16xf32>
      hls.st.yield
    }
    hls.st.yield
  }
  return
}

// -----

// ============================================================
// Section 2: Output-only kernel — DMA-out task generated
//
// ITensor:  <f32, [16], [4], [16], (d0)->(d0)>
//
// Before: compute task writes tiles to the output ITensor.
//         The kernel carries the output tensor and returns it.
// After:  the compute task is followed by a DMA-out task that
//         reads tiles from the FIFO and assembles the result
//         tensor via tensor.insert_slice.
// ============================================================

#map2 = affine_map<(d0) -> (d0)>

// MAT-LABEL: func.func @materialize_output_only
func.func @materialize_output_only(%C_init: tensor<64xf32>) -> tensor<64xf32> {
  %0 = hls.st.kernel @kernel
         output_inits(%C_init : tensor<64xf32>) -> tensor<64xf32> {
  ^bb0(%arg0: !hls.itensor<f32, [16], [4], [16], #map2>,
       %arg1: tensor<64xf32>):
    // Compute task comes first (no input DMA needed).
    // MAT:      hls.st.task kernel {
    // MAT:        hls.itensor.write
    // DMA-out task: no kernel attr, has result type, loop reads FIFO and inserts into tensor.
    // MAT:      hls.st.task ->
    // MAT:        scf.for
    // MAT:          hls.itensor.read
    // MAT:          tensor.insert_slice
    hls.st.task kernel {
      %tile = tensor.empty() : tensor<16xf32>
      hls.itensor.write %tile into %arg0
        : tensor<16xf32> into <f32, [16], [4], [16], #map2>
      hls.st.yield
    }
    hls.st.yield %arg1 : tensor<64xf32>
  }
  return %0 : tensor<64xf32>
}

// -----

// ============================================================
// Section 3: Full 2D kernel — DMA-in + compute + DMA-out
//
// ITensors: <f32, [16, 16], [4, 4], [16, 16], (d0,d1)->(d0,d1)>
//   16 tiles in a 4×4 grid, each 16×16 floats.
//
// Before: single compute task reads from %in, writes to %out.
// After:  three tasks in order:
//   1. DMA-in  (no kernel): 2D scf.for nest, extract_slice → itensor.write
//   2. compute (kernel):    itensor.read → linalg.generic → itensor.write
//   3. DMA-out (no kernel): 2D scf.for nest, itensor.read → insert_slice
//
// The ordering guarantee is the key property — the FileCheck
// directives below enforce DMA-in < compute < DMA-out in the output.
// ============================================================

#map3 = affine_map<(d0, d1) -> (d0, d1)>

// MAT-LABEL: func.func @materialize_full
func.func @materialize_full(
    %A:      tensor<64x64xf32>,
    %C_init: tensor<64x64xf32>) -> tensor<64x64xf32> {
  %0 = hls.st.kernel @kernel
         inputs(%A : tensor<64x64xf32>)
         output_inits(%C_init : tensor<64x64xf32>) -> tensor<64x64xf32> {
  ^bb0(%in:    !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map3>,
       %out:   !hls.itensor<f32, [16, 16], [4, 4], [16, 16], #map3>,
       %carry: tensor<64x64xf32>):
    // 1. DMA-in: two nested loops, one per ITensor dimension.
    //    Each iteration extracts a 16×16 tile and writes it to the input FIFO.
    // MAT:      hls.st.task {
    // MAT:        scf.for
    // MAT:          scf.for
    // MAT:            tensor.extract_slice
    // MAT:            hls.itensor.write
    //
    // 2. Compute task: reads from both ITensors, runs linalg, writes output.
    // MAT:      hls.st.task kernel {
    // MAT:        hls.itensor.read
    // MAT:        hls.itensor.read
    // MAT:        linalg.generic
    // MAT:        hls.itensor.write
    //
    // 3. DMA-out: two nested loops collecting tiles into the result tensor.
    // MAT:      hls.st.task ->
    // MAT:        scf.for
    // MAT:          scf.for
    // MAT:            hls.itensor.read
    // MAT:            tensor.insert_slice
    hls.st.task kernel {
      %tile_in = hls.itensor.read %in
                   : <f32, [16, 16], [4, 4], [16, 16], #map3> -> tensor<16x16xf32>
      %tile_out = hls.itensor.read %out
                    : <f32, [16, 16], [4, 4], [16, 16], #map3> -> tensor<16x16xf32>
      %result = linalg.generic {
          indexing_maps = [#map3, #map3],
          iterator_types = ["parallel", "parallel"]
        } ins(%tile_in : tensor<16x16xf32>) outs(%tile_out : tensor<16x16xf32>) {
        ^bb0(%a: f32, %c: f32):
          %sum = arith.addf %a, %c : f32
          linalg.yield %sum : f32
        } -> tensor<16x16xf32>
      hls.itensor.write %result into %out
        : tensor<16x16xf32> into <f32, [16, 16], [4, 4], [16, 16], #map3>
      hls.st.yield
    }
    hls.st.yield %carry : tensor<64x64xf32>
  }
  return %0 : tensor<64x64xf32>
}
