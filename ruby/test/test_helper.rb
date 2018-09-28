$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'ndtypes'

require 'minitest/autorun'

# Constants

MAX_DIM = NDTypes::MAX_DIM

# Assertions

def assert_serialize t
  assert_equal NDT.deserialize(t.serialize), t
end

def assert_false val
  assert !val
end

def assert_true val
  assert val
end

# ======================================================================
#              Check fixed dimensions with arbitary strides
# ======================================================================

# Verify the datasize of fixed dimensions with arbitrary strides.
def verify_datasize t
  return t.datasize == 0 if t.itemsize == 0
  return false if t.datasize % t.itemsize != 0
  return t.ndim == 0 && !t.shape && !t.strides if t.ndim <= 0
  return false if t.shape.any? { |v| v < 0 }
  return false if t.strides.any? { |v| v % t.itemsize }
  return t.datasize == 0 if t.shape.include?(0)

  imin = t.ndim.times.
         map { |j| t.strides[j]*(t.shape[j]-1) if t.strides[j] <= 0}.
         inject(:+)
  imax = t.ndim.times.
         map { |j| t.strides[j]*(t.shape[j]-1) if t.strides[j] > 0 }.
         inject(:+)

  return t.datasize == (imin.abs + imax + t.itemsize)
end

BROADCAST_TEST_CASES = [
  NDT::ApplySpec.new(
    'C|Fortran|Strided|Xnd',
    NDT.new("uint8 -> float64"),
    [NDT.new("uint8")],
    [NDT.new("float64")],
    [],
     0),

  NDT::ApplySpec.new(
    'Elemwise1D|C|Fortran|Strided|Xnd',
    NDT.new("... * uint8 -> ... * float64"),
    [NDT.new("2 * uint8")],
    [NDT.new("2 * float64")],
    [NDT.new("2 * uint8")],
     1),

  NDT::ApplySpec.new(
    'C|Fortran|Strided|Xnd',
    NDT.new("F[... * uint8] -> F[... * float64]"),
    [NDT.new("!2 * 3 * uint8")],
    [NDT.new("!2 * 3 * float64")],
    [NDT.new("!2 * 3 * uint8")],
     2),

  NDT::ApplySpec.new(
    'C|Fortran|Strided|Xnd',
    NDT.new("... * uint8 -> ... * float64"),
    [NDT.new("fixed(shape=2, step=10) * uint8")],
    [NDT.new("2 * float64")],
    [NDT.new("fixed(shape=2, step=10) * uint8")],
     1),

  NDT::ApplySpec.new(
    'Strided|Xnd',
    NDT.new("... * N * uint8 -> ... * N * float64"),
    [NDT.new("fixed(shape=2, step=10) * uint8")],
    [NDT.new("2 * float64")],
    [NDT.new("fixed(shape=2, step=10) * uint8")],
     0),

  NDT::ApplySpec.new(
    'C|Fortran|Strided|Xnd',
    NDT.new("... * N * uint8 -> ... * N * float64"),
    [NDT.new("2 * 3 * uint8")],
    [NDT.new("2 * 3 * float64")],
    [NDT.new("2 * 3 * uint8")],
     1),

  NDT::ApplySpec.new(
    'C|Strided|Xnd',
    NDT.new("... * N * M * uint8 -> ... * N * M * float64"),
    [NDT.new("2 * 3 * uint8")],
    [NDT.new("2 * 3 * float64")],
    [NDT.new("2 * 3 * uint8")],
     0),

  NDT::ApplySpec.new(
    'Xnd',
    NDT.new("N * D * float64 -> P * float64"),
    [NDT.new("2 * 10 * float64")],
    [NDT.new("P * float64")],
    [],
     0),

  NDT::ApplySpec.new(
    'C|Fortran|Xnd',
    NDT.new("var... * float64 -> var... * float64"),
    [NDT.new("var(offsets=[0,2]) * var(offsets=[0,4,11]) * float64")],
    [NDT.new("var(offsets=[0,2]) * var(offsets=[0,4,11]) * float64")],
    [],
     2)
]
