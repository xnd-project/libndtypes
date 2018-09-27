$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'ndtypes'

require 'minitest/autorun'

MAX_DIM = NDTypes::MAX_DIM

def assert_serialize t
  assert_equal NDT.deserialize(t.serialize), t
end

def assert_false val
  assert !val
end

def assert_true val
  assert val
end
