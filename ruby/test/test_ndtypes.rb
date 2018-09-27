require 'test_helper'

class TestVarDim < Minitest::Test
  def test_var_dim_predicates
    t = NDT.new "var(offsets=[0,2]) * var(offsets=[0,3,10]) * complex128"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_false t.c_contiguous?
    assert_false t.f_contiguous?
  end

  def test_var_dim_common_fields
    dt = "{a: complex64, b: string}"
    t = NDT.new "var(offsets=[0,2]) * var(offsets=[0,3,10]) * #{dt}"
    assert_serialize t
    dtype = NDT.new dt

    assert_equal t.ndim, 2

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end

  def test_var_dim_invariants
    # mixing var and fixed is disallowed.
    assert_raises(TypeError) { NDT.new("10 * var * int64") }
    assert_raises(TypeError) { NDT.new("var * 10 * int64") }
    assert_raises(TypeError) { NDT.new("10 * var * 10 * int64") }
    assert_raises(TypeError) { NDT.new("var * 10 * var * int64") }
    assert_raises(TypeError) { NDT.new("N * var * int64") }
    assert_raises(TypeError) { NDT.new("var * N * int64") }
    assert_raises(TypeError) { NDT.new("N * var * N * int64") }
    assert_raises(TypeError) { NDT.new("var * N * var * int64") }

    # too many dimensions.
    assert_raises(TypeError) { NDT.new("var * " * (MAX_DIM + 1) + "float64") }

    # nested var is disallowed.
    assert_raises(TypeError) { NDT.new("2 * {a: var * complex128}") }
    assert_raises(TypeError) { NDT.new("var * {a: var * complex128}") }
    assert_raises(TypeError) { NDT.new("var * ref(var * string)") }
    assert_raises(TypeError) { NDT.new("var * SomeConstr(var * string)") }
  end

  def test_var_dim_external_offsets
    # Invalid offsets.
    assert_raises(TypeError) { NDT.new( "int8", [""]) }
    assert_raises(TypeError) { NDT.new( "int8", [0]) }
    assert_raises(TypeError) { NDT.new( "int8", [0, 2]) }
    assert_raises(TypeError) { NDT.new( "int8", {}) }
    assert_raises(TypeError) { NDT.new( "int8", ()) }
    assert_raises(TypeError) { NDT.new( "int8", [(), ()]) }

    assert_raises(ValueError) { NDT.new( "int8", []) }
    assert_raises(ValueError) { NDT.new( "int8", [[0]]) }
    assert_raises(ValueError) { NDT.new( "int8", [[0], [0]]) }

    assert_raises(ValueError) { NDT.new( "int8", [[-1, 2]]) }
    assert_raises(ValueError) { NDT.new( "int8", [[0, 2147483648]]) }

    # Invalid combinations.
    assert_raises(ValueError) { NDT.new( "int8", [[0, 2], [0, 10]]) }
    assert_raises(ValueError) { NDT.new( "int8", [[0, 2], [0, 10, 30, 40]]) }

    # Implicit mixing of var and fixed.
    assert_raises(TypeError) { NDT.new( "10 * int8", [[0, 2], [0, 10, 20]]) }

    # Abstract dtype.
    assert_raises(ValueError) { NDT.new( "N * int8", [[0, 2], [0, 10, 20]]) }
    assert_raises(ValueError) { NDT.new( "var * int8", [[0, 2], [0, 10, 20]]) }

    # Mixing external and internal offsets.
    assert_raises(TypeError) { NDT.new( "var(offsets=[0,2,10]) * int8", [[0, 1], [0, 2]])}
  end
end

class TestSymbolicDim < Minitest::Test
  def test_symbolic_dim_predicates
    t = NDT.new("N * M * complex128")
    assert_serialize t

    assert_true t.abstract? 
    assert_false t.complex?
    assert_false t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_false t.c_contiguous?
    assert_false t.f_contiguous?
  end

  def test_symbolic_dim_common_fields
    dt = "{a: complex64, b: string}"
    t = NDT.new("N * M * #{dt}")
    assert_serialize(t)
    dtype = NDT.new(dt)

    assert_raises(TypeError) { t.ndim }
    assert_raises(TypeError) { t.itemsize }
    assert_raises(TypeError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestSymbolicDim

class TestEllipsisDim < Minitest::Test
  def test_ellipsis_dim_predicates
    t = NDT.new("... * 2 * complex128")
    assert_serialize(t)

    assert_true t.abstract?
    assert_false t.complex?
    assert_false t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_false t.c_contiguous?
    assert_false t.f_contiguous?
  end

  def test_ellipsis_dim_common_fields
    dt = "{a: complex64, b: string}"
    t = NDT.new("... * 2 * #{dt}")
    assert_serialize(t)
    dtype = NDT.new(dt)

    assert_raises(TypeError) { t.ndim }
    assert_raises(TypeError) { t.itemsize }
    assert_raises(TypeError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestEllipsisDim

class TestTuple < Minitest::Test
  def test_tuple_predicates
    ["()", "(int64)", "(string, bytes, pack=1)"].each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end

    ["(Any)", "(int64, N * M * uint8)", "(string, Float)"].each do |s|
      t = NDT.new s
      assert_serialize t

      assert_true t.abstract?
      assert_false t.complex?
      assert_false t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_false t.c_contiguous?
      assert_false t.f_contiguous?
    end
  end

  def test_tuple_common_fields
    f = "{a: complex64, b: string}"
    t = NDT.new("(#{f}, #{f})")
    assert_serialize t
    field = NDT.new f

    assert_equal t.ndim, 0

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestTuple

class TestRecord < Minitest::Test
  def test_record_predicates
    ["{}", "{a: int64, b: bytes}", "{x: string, y: uint8, pack=1}"].each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?      
    end

    ["{a: Any, b: Complex}", "{x: N * M * T}"].each do |s|
      t = NDT.new s
      assert_serialize t
 
      assert_true t.abstract?
      assert_false t.complex?
      assert_false t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_false t.c_contiguous?
      assert_false t.f_contiguous?
    end
  end

  def test_record_common_fields
    f = "{a: complex64, b: string}"
    t = NDT.new("{x: #{f}, y: #{f}, z: #{f}}")
    assert_serialize t
    field = NDT.new f

    assert_equal t.ndim, 0

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestRecord

class TestRef < Minitest::Test
  def test_ref_predicates
    ["&2 * 3 * float64", "&bool", "&(uint8, int32)"].each do |s|
      t = NDT.new s
      assert_serialize t
 
      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?     
    end
    
    ["&Any", "&(int64, N * M * uint8)"].each do |s|
      t = NDT.new s
      assert_serialize t

      assert_true t.abstract?
      assert_false t.complex?
      assert_false t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_false t.c_contiguous?
      assert_false t.f_contiguous?
    end
  end

  def test_ref_common_fields
    a = "{a: complex64, b: string}"
    t = NDT.new("ref(#{a})")
    assert_serialize(t)
    arg = NDT.new(a)

    assert_equal(t.ndim, 0)
    assert_equal(t.itemsize, SIZEOF_PTR)
    assert_equal(t.align, SIZEOF_PTR)

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestRef

class TestConstr < Minitest::Test
  def test_constr_predicates
    ["Some(int16)", "Maybe(int64)", "Just((string, bytes))"].each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end
  end

  def test_constr_common_fields
    a = "{a: complex64, b: string}"
    t = NDT.new("Just(#{a})")
    assert_serialize t
    arg = NDT.new a

    assert_equal t.ndim, 0

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestConstr

class TestNominal < Minitest::Test
  def setup
    NDT.typedef "some_t", "2 * 10 * complex128"
  end

  def test_nominal_predicates
    t = NDT.new "some_t"
    assert_serialize t

    # The nominal type is opaque. The only thing known is that
    # the typedef is concrete.
    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_nominal_common_fields
    t = NDT.new "some_t"
    dtype = NDT.new "complex128"

    # The opaque type is treated as a dtype with ndim==0, same as
    # for constructor types.
    assert_equal t.ndim, 0

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end

  def test_nominal_exceptions
    # not in the typedef table
    assert_raises(ValueError) { NDT.new("undefined_t") }

    # duplicate typedef
    assert_raises(ValueError) { NDT.typedef("some_t", "int64") }
  end
end # class TestNominal

class TestScalarKind < Minitest::Test
  def test_scalar_kind_predicates
    t = NDT.new "Scalar"
    assert_serialize t

    assert_true t.abstract?
    assert_false t.complex?
    assert_false t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_false t.c_contiguous?
    assert_false t.f_contiguous?
  end

  def test_scalar_kind_common_fields
    t = NDT.new "Scalar"
    assert_serialize t

    assert_raises(TypeError) { t.ndim }
    assert_raises(TypeError) { t.itemsize }
    assert_raises(TypeError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestScalarKind

class TestCategorical < Minitest::Test
  def test_categorical_predicates
    [ 
      "categorical(NA, 1, 100, -29999)",
      "categorical(NA, 1.2, -200.25)",
      "categorical('foo', 'bar')",
      "categorical('foo', NA, 100)"
    ].each do |s|
      t = NDT.new s
      assert_serialize t
      
      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_false t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?     
    end
  end

  def test_categorical_common_fields
    t = NDT.new "categorical(NA, 'something', 'must', 'be', 'done')"
    assert_serialize t

    assert_equal t.ndim, 0

    assert_raises(TypeError) { t.shape }
    assert_raises(NoMethodError) { t.strides }
  end
end # class TestCategorical

class TestFixedStringKind < Minitest::Test
  def test_fixed_string_kind_predicates
    t = NDT.new "FixedString"
    assert_serialize t

    assert_true t.abstract?
    assert_false t.complex?
    assert_false t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_false t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_false t.c_contiguous?
    assert_false t.f_contiguous?
  end

  def test_fixed_string_kind_common_fields
    t = NDT.new "FixedString"

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(NoMethodError) { t.shape }
    assert_raises(NoMethodError) { t.strides }
  end
end # class TestFixedStringKind


