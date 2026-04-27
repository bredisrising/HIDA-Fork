// Realistic single test for VectorizeITensor.
//
// One function: a producer pushes a tile through an ITensor FIFO; a
// consumer pulls it back as a tensor. After running with vectorWidth=4,
// the FIFO should carry vector<4xf32>, and both sides should grow loop
// nests that pack/unpack scalars to/from those vectors.
//
// Run:
//   build/bin/scalehls-opt test/StreamTensor/Transforms/vectorize-pipe.mlir \
//     -scalehls-vectorize-itensor="vectorWidth=4" --verify-each=false

!fifo = !hls.itensor<f32, [8, 8], [4, 4], [8, 8], affine_map<(d0,d1) -> (d0,d1)>>

func.func @stream_pipe(%tile: tensor<8x8xf32>) -> tensor<8x8xf32> {
  %f0  = hls.itensor.instance depth 2 : !fifo
  %f1  = hls.itensor.write %tile into %f0 : tensor<8x8xf32> into !fifo
  %out = hls.itensor.read  %f1            : !fifo -> tensor<8x8xf32>
  return %out : tensor<8x8xf32>
}
