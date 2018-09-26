$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'ndtypes'

require 'minitest/autorun'

def assert_serialize t
  assert_equal NDT.deserialize(t.serialize), t
end
