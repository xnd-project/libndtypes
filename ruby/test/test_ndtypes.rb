require 'test_helper'

class TestVarDim < Minitest::Test
  def test_var_dim_predicates
    t = NDT.new "var(offsets=[0,2]) * var(offsets=[0,3,10]) * complex128"
    assert_serialize t

    
  end
end


