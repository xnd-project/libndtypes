require_relative 'test_helper'

class TestModule < Minitest::Test
  def test_module_predicates
    # Namespaces are not yet supported in xnd. One can construct the
    # types, however.  Modules are for pattern matching only, so they 
    # are abstract.
    t = NDT.new("SomeNamespace:: 2 * 3 * float64")
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

  def test_module_common_fields
    t = NDT.new "SomeNamespace:: 2 * 3 * float64"
    assert_serialize t

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }
    
    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestModule

class TestFunction < Minitest::Test
  def test_function_predicates
    t = NDT.new("(10 * float64, string) -> float64")
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

  def test_function_common_fields
    t = NDT.new("(10 * float64, string) -> float64")
    assert_serialize(t)

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }
    
    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestFunction

class TestVoid < Minitest::Test
  def test_void_as_return_value
    t = NDT.new "() -> void"
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

  def test_void_exception
    assert_raises(ValueError) { NDT.new("void") }
  end
end # class TestVoid

class TestAny < Minitest::Test
  def test_any_predicates
    t = NDT.new "Any"
    
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

  def test_any_common_fields
    t = NDT.new "Any"
    assert_serialize t

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }
    
    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestAny

# Put FixedDim here

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

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

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

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

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

#    assert_raises(TypeError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
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

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
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

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
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

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end
end # class TestConstr

class TestNominal < Minitest::Test
  include Minitest::Hooks

  def before_all
    NDT.typedef "something_t", "2 * 10 * complex128"
  end

  def test_nominal_predicates
    t = NDT.new "something_t"
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
    t = NDT.new "something_t"
    dtype = NDT.new "complex128"

    # The opaque type is treated as a dtype with ndim==0, same as
    # for constructor types.
    assert_equal t.ndim, 0

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end

  def test_nominal_exceptions
    # not in the typedef table
    assert_raises(ValueError) { NDT.new("undefined_t") }

    # duplicate typedef
    assert_raises(ValueError) { NDT.typedef("something_t", "int64") }
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

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

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

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
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

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestFixedStringKind

class TestFixedString < Minitest::Test
  def test_fixed_string_predicates
    t = NDT.new "fixed_string(380, 'utf16')"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_fixed_string_common_fields
    [
      ['ascii', 1],
      ['utf8', 1],
      ['utf16', 2],
      ['utf32', 4]
    ].each do |encoding, codepoint_size|
      t = NDT.new "fixed_string(20, '#{encoding}')"
      assert_serialize t

      assert_equal t.itemsize, 20 * codepoint_size
      assert_equal t.align, codepoint_size

#      assert_raises(NoMethodError) { t.shape }
#      assert_raises(NoMethodError) { t.strides }
    end
  end
end # class TestFixedString

class TestFixedBytesKind < Minitest::Test
  def test_fixed_bytes_kind_predicates
    t = NDT.new "FixedBytes"
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
    t = NDT.new "FixedBytes"
    
    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestFixedBytesKind

class TestFixedBytes < Minitest::Test
  def test_fixed_bytes_predicates
    t = NDT.new "fixed_bytes(size=1020)"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?    
  end

  def test_fixed_bytes_common_fields
    [1,2,4,8,16,32,64].each do |align|
      t = NDT.new "fixed_bytes(size=1024, align=#{align})"
      assert_serialize t

      assert_equal t.ndim, 0

      assert_equal t.itemsize, 1024
      assert_equal t.align, align

#      assert_raises(NoMethodError) { t.shape }
#      assert_raises(NoMethodError) { t.strides }
    end
  end

  def test_fixed_bytes_exceptions
    # data size must be a mutliple of align
    assert_raises(ValueError) { NDT.new("fixed_bytes(size=20, align=8)")}
  end
end # class TestFixedBytes

class TestString < Minitest::Test
  def test_string_predicates
    t = NDT.new "string"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_string_common_fields
    t = NDT.new "string"

    assert_equal t.ndim, 0
    assert_equal t.itemsize, SIZEOF_PTR
    assert_equal t.align, SIZEOF_PTR

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end
end # class TestString

class TestBytes < Minitest::Test
  def test_bytes_predicates
    t = NDT.new "bytes"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_bytes_common_fields
    t = NDT.new "bytes"

    assert_equal t.ndim, 0

    assert_equal t.itemsize, 16
    assert_equal t.align, 8

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end
end # class TestBytes

class TestChar < Minitest::Test
  def test_char_predicates
    t = NDT.new "char"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_char_common_fields
    t = NDT.new "char('utf32')"
    assert_serialize t

    assert_equal t.ndim, 0
    assert_equal t.itemsize, 4
    assert_equal t.align, 4

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end
end # class TestChar

class TestBool < Minitest::Test
  def test_bool_predicates
    t = NDT.new "bool"
    assert_serialize t

    assert_false t.abstract?
    assert_false t.complex?
    assert_true t.concrete?
    assert_false t.float?
    assert_false t.optional?
    assert_true t.scalar?
    assert_false t.signed?
    assert_false t.unsigned?

    assert_true t.c_contiguous?
    assert_true t.f_contiguous?
  end

  def test_bool_common_fields
    t = NDT.new "bool"

    assert_equal t.ndim, 0
    assert_equal t.itemsize, 1
    assert_equal t.align, 1

#    assert_raises(NoMethodError) { t.shape }
#    assert_raises(NoMethodError) { t.strides }
  end
end # class TestBool

class TestSignedKind < Minitest::Test
  def test_signed_kind_predicates
    t = NDT.new "Signed"
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

  def test_signed_kind_common_fields
    t = NDT.new "Signed"

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestSignedKind

class TestSigned < Minitest::Test
  def test_signed_predicates
    signed = ['int8', 'int16', 'int32', 'int64']

    signed.each do |s|
      t = NDT.new s
      assert_serialize t
      
      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_true t.scalar?
      assert_true t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end
  end

  def test_signed_common_fields
    [
      ['int8', 1],
      ['int16', 2],
      ['int32', 4],
      ['int64', 8]
    ].each do |s, itemsize|
      t = NDT.new s

      assert_equal t.ndim, 0

      assert_equal t.itemsize, itemsize
      assert_equal t.align, itemsize

#      assert_raises(NoMethodError) { t.shape }
    end
  end
end # class TestSigned

class TestUnsignedKind < Minitest::Test
  def test_unsigned_kind_predicates
    t = NDT.new "Unsigned"
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

  def test_unsigned_kind_common_fields
    t = NDT.new "Unsigned"

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestUnsignedKind


class TestUnsigned < Minitest::Test
  def test_unsigned_predicates
    unsigned = ['uint8', 'uint16', 'uint32', 'uint64']

    unsigned.each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_true t.scalar?
      assert_false t.signed?
      assert_true t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end
  end

  def test_unsigned_common_fields
    [
      ['uint8', 1],
      ['uint16', 2],
      ['uint32', 4],
      ['uint64', 8]
    ].each do |s, itemsize|
      t = NDT.new s

      assert_equal t.ndim, 0

      assert_equal t.itemsize, itemsize
      assert_equal t.align, itemsize

#      assert_raises(NoMethodError) { t.shape }
    end
  end
end # class TestUnsigned


class TestFloatKind < Minitest::Test
  def test_float_kind_predicates
    t = NDT.new "Float"
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

  def test_float_kind_common_fields
    t = NDT.new "Float"
    
    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestFloatKind

class TestFloat < Minitest::Test
  def test_float_predicates
    _float = ['float32', 'float64']

    _float.each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_false t.complex?
      assert_true t.concrete?
      assert_true t.float?
      assert_false t.optional?
      assert_true t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end
  end

  def test_float_common_fields
    _float = [['float32', 4], ['float64', 8]]

    _float.each do |s, itemsize|
      t = NDT.new s

      assert_equal t.ndim, 0

      assert_equal t.itemsize, itemsize
      assert_equal t.align, itemsize

#      assert_raises(NoMethodError) { t.shape }
    end
  end
end # class TestFloat

class TestComplexKind < Minitest::Test
  def test_complex_kind_predicates
    t = NDT.new "Complex"
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

  def test_complex_kind_common_fields
    t = NDT.new "Complex"

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestComplexKind

class TestComplex < Minitest::Test
  def test_complex_predicates
    _complex = ['complex64', 'complex128']

    _complex.each do |s|
      t = NDT.new s
      assert_serialize t

      assert_false t.abstract?
      assert_true t.complex?
      assert_true t.concrete?
      assert_false t.float?
      assert_false t.optional?
      assert_true t.scalar?
      assert_false t.signed?
      assert_false t.unsigned?

      assert_true t.c_contiguous?
      assert_true t.f_contiguous?
    end
  end

  def test_complex_common_fields
    _complex = [['complex64', 8], ['complex128', 16]]

    _complex.each do |s, itemsize|
      t = NDT.new s

      assert_equal t.ndim, 0

      assert_equal t.itemsize, itemsize
      assert_equal t.align, itemsize / 2

#      assert_raises(NoMethodError) { t.shape }
    end
  end
end # class TestComplex

class TestTypevar < Minitest::Test
  def test_typevar_predicates
    t = NDT.new "T"
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

  def test_typevar_common_fields
    t = NDT.new "T"

    assert_raises(NoMethodError) { t.ndim }
    assert_raises(NoMethodError) { t.itemsize }
    assert_raises(NoMethodError) { t.align }

    assert_raises(TypeError) { t.shape }
    assert_raises(TypeError) { t.strides }
  end
end # class TestTypevar

class TestDup < Minitest::Test
  # TODO: Add #dup test here from rspec. Call the method `test_dup`
end

class TestBufferProtocol < Minitest::Test
  def test_array
    test_cases = [
      # format, itemsize, alignment
      ["(0)f", 4, 4],
      ["(1)d", 8, 8],
      ["(125)d", 8, 8],
      ["(2,3)d", 8, 8],
      ["(2,10)T{<b:a:Q:b:}", 9, 1],
      ["(2,19)T{<b:a:xxxQ:b:}", 12, 4],
      ["(31,221)T{<b:a:xxxxxxxQ:b:}", 16, 8],
      ["(2,3,10)T{<b:a:xxxxxxxxxxxxxxxQ:b:xxxxxxxx}", 32, 16],
      ["(2,10)T{=L:a:(2,3)D:b:}", 100, 1]
    ]

    test_error_cases = [
      # empty shape (scalars are not arrays in datashape)
      "()Q",
      # Ambiguous (tuple of 10 or array of 10)?
      "10Q"
    ]

    test_cases.each do |fmt, itemsize, align|
      t = NDT.from_format fmt

      assert_equal t.itemsize, itemsize
      assert_equal t.align, align
    end

    test_error_cases.each do |fmt|
      assert_raises(ValueError) { NDT.from_format(fmt) }
    end
  end

  def test_record
    test_cases = [
      # format, itemsize, alignment
      ["T{<b:a:Q:b:}", 9, 1],
      ["T{<b:a:xQ:b:}", 10, 2],
      ["T{<b:a:xxxQ:b:}", 12, 4],
      ["T{<b:a:xxxxxxxQ:b:}", 16, 8],
      ["T{<b:a:xxxxxxxxxxxxxxxQ:b:xxxxxxxx}", 32, 16],
      ["T{=i:foo:f:bar:10s:baz:}", 18, 1]
    ]

    test_error_cases = [
      # sizeof(signed char) + padding is not a power of two.
      "T{<b:a:xxQ:b:}",
      # Missing padding bytes at the end of the struct.  The only
      # reason a compiler would add three padding bytes to field
      # zero is an artificial forced alignment of four for the short
      # in field one.  This in turn requires that the entire struct
      # has alignment four, which necessitates two padding bytes at
      # the end of the struct.
      "T{<b:a:xxxh:b:}",
      # Unnatural padding at the end of the struct (expect two padding
      # bytes instead of four).
      "T{<b:a:xxxh:b:xxxx}"
    ]

    test_cases.each do |fmt, itemsize, align|
      t = NDT.from_format fmt

      assert_equal t.itemsize, itemsize
      assert_equal t.align, align
    end

    test_error_cases.each do |fmt|
      assert_raises(ValueError) { NDT.from_format(fmt) }
    end
  end

  def test_fixed_bytes
    [['s', 1], ['100s', 100]].each do |fmt, size|
      t = NDT.from_format fmt
      assert_equal t.itemsize, size
    end

    # For consistency (it would be easy to allow, but other dtypes
    # cannot have size 0).
    assert_raises(ValueError) { NDT.from_format('0s') }

    ['0s', 's', '100s'].each do |fmt|
      ['@', '=', '<', '>', '!'].each do |modifier|
        f = modifier + fmt

        assert_raises(ValueError) { NDT.from_format(f) }
      end
    end

    t = NDT.from_format('c')
    assert_equal t, NDT.new("char('ascii')")
  end

  def test_primitive
    standard = [
      '?',
      'c', 'b', 'B',
      'h', 'i', 'l', 'q',
      'H', 'I', 'L', 'Q',
      'f', 'd']

    native = ['n', 'N']

    standard.each do |fmt|
      ['', '@', '=', '<', '>', '!'].each do |modifier|
        f = modifier + fmt
        t = NDT.from_format f
#        assert_equal t.itemsize, 2 Ruby needs something like Python struct: https://docs.python.org/2/library/struct.html
      end
    end

    native.each do |fmt|
      ['', '@'].each do |modifier|
        f = modifier + fmt
        t = NDT.from_format f

     #   assert_equal t.itemsize, f.size
      end
    end

    native.each do |fmt|
      ['=', '<', '>', '!'].each do |modifier|
        f = modifier + fmt

        assert_raises(ValueError) { NDT.from_format(f) }
      end
    end

    # complex64
    fmt = 'F'
    ['', '@', '=', '<', '>', '!'].each do |modifier|
      f = modifier + fmt
      t = NDT.from_format f

      assert_equal t.itemsize, 8
    end

    # complex128
    fmt = 'D'
    ['', '@', '=', '<', '>', '!'].each do |modifier|
      f = modifier + fmt
      t = NDT.from_format(f)

      assert_equal t.itemsize, 16
    end
  end
end # class TestBufferProtocol

class TestApply < Minitest::Test
  def test_apply
    # Type checking and return type inference for function applications.

    # Function type:
    sig = NDT.new "Dims... * N * M * int64, Dims... * M * P * int64 -> Dims... * N * P * float64"
    assert_serialize sig

    # argument types:
    in_types = [NDT.new("20 * 2 * 3 * int64"), NDT.new("20 * 3 * 4 * int64")]

    spec = sig.apply(in_types)

    assert_equal spec.sig, sig
    assert_equal spec.in_types, in_types
    assert_equal spec.out_types, [NDT.new("20 * 2 * 4 * float64")]
    assert_equal spec.outer_dims, 1
  end

  def test_apply_error
    sig = NDT.new("Dims... * N * M * int64, Dims... * M * P * int64 -> Dims... * N * P * float64")
    lst = [["20 * 2 * 3 * int8", "20 * 3 * 4 * int64"],
           ["10 * 2 * 3 * int64", "20 * 3 * 4 * int64"],
           ["20 * 2 * 100 * int64", "20 * 3 * 4 * int64"]]

    lst.each do |l|
      in_types = l.map { |x| NDT.new(x) }
      assert_raises(TypeError) { sig.apply(in_types) }
    end
  end
end # class TestApply

class TestBroadcast < Minitest::Test
  def test_broadcast
    BROADCAST_TEST_CASES.each do |c|
      spec = c.sig.apply(c.in_types)

      assert_equal spec.size, c.size

      spec.zip(c).each do |v, u|
        assert_equal v, u
      end
    end
  end
end # class TestBroadcast

class LongFixedDimTests < Minitest::Test
  def test_steps_random
    DTYPE_TEST_CASES.each do |dtype, _|
      1000.times do
        s = dtype
        ndim = rand(1...10)

        ndim.times do |n|
          shape = rand(1...100)
          step = rand(-100...100)
          s = "fixed(shape=#{shape}, step=#{step}) * #{s}"
        end

        puts "s :: #{s}."
        t = NDT.new s

        assert_true verify_datasize(t)
      end
    end
  end

  def test_steps_brute_force
    DTYPE_TEST_CASES.each do |dtype, _|
      10.times do |m|
        (-10...10).to_a.each do |i|
          s = "fixed(shape=#{m}, step=#{i}) * #{dtype}"
          t = NDT.new s

          assert_true verify_datasize(t)
        end
      end

      10.times do |m|
        10.times do |n|
          (-10...10).to_a.each do |i|
            (-10...10).to_a.each do |j|
              s = "fixed(shape=#{m}, step=#{i}) * fixed(shape=#{n}, step=#{j}) * #{dtype}"
              t = NDT.new s

              assert_true verify_datasize(t)
            end
          end
        end
      end

      10.times do |m|
        10.times do |n|
          (-10...10).to_a.each do |i|
            (-10...10).to_a.each do |j|
              s = "fixed(shape=#{m}, step=#{i}) * fixed(shape=#{n}, step=#{j}) * #{dtype}"
              t = NDT.new s

              assert_true verify_datasize(t)
            end
          end
        end
      end

      3.times do |m|
        3.times do |n|
          3.times do |p|
            (-5...5).to_a.each do |i|
              (-5...5).to_a.each do |j|
                (-5...5).to_a.each do |k|
                  s = "fixed(shape=#{m}, step=#{i}) * fixed(shape=#{n}, step=#{j}) * fixed(shape=#{p}, step=#{k}) * #{dtype}"
                  t = NDT.new s
                  
                  assert_true verify_datasize(t)
                end
              end
            end
          end
        end
      end
    end
  end
end # class LongFixedDimTests
