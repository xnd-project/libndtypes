#
# BSD 3-Clause License
#
# Copyright (c) 2017, plures
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import unittest, sys, argparse, gc
from copy import copy
from ndtypes import ndt, MAX_DIM
from randtype import *


parser = argparse.ArgumentParser()
parser.add_argument("-f", "--failfast", action="store_true",
                    help="stop the test run on first error")
ARGS = parser.parse_args()


class TestModule(unittest.TestCase):

    def test_module_predicates(self):
        # Namespaces are not yet supported in xnd. One can construct the
        # types, however.  Modules are for pattern matching only, so they 
        # are abstract.
        t = ndt("SomeNamespace:: 2 * 3 * float64")

        self.assertTrue(t.is_abstract())
        self.assertFalse(t.is_array())
        self.assertFalse(t.is_c_contiguous())
        self.assertFalse(t.is_complex())
        self.assertFalse(t.is_concrete())
        self.assertFalse(t.is_f_contiguous())
        self.assertFalse(t.is_float())
        self.assertFalse(t.is_optional())
        self.assertFalse(t.is_scalar())
        self.assertFalse(t.is_signed())
        self.assertFalse(t.is_unsigned())

    def test_module_common_fields(self):
        t = ndt("SomeNamespace:: 2 * 3 * float64")

        # Common type fields are undefined.
        self.assertRaises(TypeError, getattr, t, 'ndim')
        self.assertRaises(TypeError, getattr, t, 'align')
        self.assertRaises(TypeError, getattr, t, 'itemsize')

        # Cannot be represented as an ndarray.
        self.assertRaises(TypeError, getattr, t, 'shape')
        self.assertRaises(TypeError, getattr, t, 'strides')


class TestFunction(unittest.TestCase):

    def test_function_predicates(self):
        t = ndt("(10 * float64, string) -> float64")
 
        self.assertTrue(t.is_abstract())
        self.assertFalse(t.is_array())
        self.assertFalse(t.is_c_contiguous())
        self.assertFalse(t.is_complex())
        self.assertFalse(t.is_concrete())
        self.assertFalse(t.is_f_contiguous())
        self.assertFalse(t.is_float())
        self.assertFalse(t.is_optional())
        self.assertFalse(t.is_scalar())
        self.assertFalse(t.is_signed())
        self.assertFalse(t.is_unsigned())

    def test_function_common_fields(self):
        t = ndt("(10 * float64, string) -> float64")

        # Common type fields are undefined.
        self.assertRaises(TypeError, getattr, t, 'ndim')
        self.assertRaises(TypeError, getattr, t, 'align')
        self.assertRaises(TypeError, getattr, t, 'itemsize')

        # Cannot be represented as an ndarray.
        self.assertRaises(TypeError, getattr, t, 'shape')
        self.assertRaises(TypeError, getattr, t, 'strides')


class TestAny(unittest.TestCase):

    def test_any_predicates(self):
        t = ndt("Any")

        self.assertTrue(t.is_abstract())
        self.assertFalse(t.is_array())
        self.assertFalse(t.is_c_contiguous())
        self.assertFalse(t.is_complex())
        self.assertFalse(t.is_concrete())
        self.assertFalse(t.is_f_contiguous())
        self.assertFalse(t.is_float())
        self.assertFalse(t.is_optional())
        self.assertFalse(t.is_scalar())
        self.assertFalse(t.is_signed())
        self.assertFalse(t.is_unsigned())

    def test_any_common_fields(self):
        t = ndt("Any")

        # Common type fields are undefined.
        self.assertRaises(TypeError, getattr, t, 'ndim')
        self.assertRaises(TypeError, getattr, t, 'align')
        self.assertRaises(TypeError, getattr, t, 'itemsize')

        # Cannot be represented as an ndarray.
        self.assertRaises(TypeError, getattr, t, 'shape')
        self.assertRaises(TypeError, getattr, t, 'strides')


class TestFixedDim(unittest.TestCase):

    def test_fixed_dim_predicates(self):
        t = ndt("10 * 20 * uint8")

        self.assertFalse(t.is_abstract())
        self.assertTrue(t.is_array())
        self.assertTrue(t.is_c_contiguous())
        self.assertFalse(t.is_complex())
        self.assertTrue(t.is_concrete())
        self.assertFalse(t.is_f_contiguous())
        self.assertFalse(t.is_float())
        self.assertFalse(t.is_optional())
        self.assertFalse(t.is_scalar())
        self.assertFalse(t.is_signed())
        self.assertFalse(t.is_unsigned())

        t = ndt("20 * uint8")
        self.assertTrue(t.is_c_contiguous())
        self.assertTrue(t.is_f_contiguous())

        t = ndt("1 * 10 * uint8")
        self.assertTrue(t.is_c_contiguous())
        self.assertTrue(t.is_f_contiguous())

    def test_fixed_dim_common_fields(self):
        dt = "{a: complex64, b: string}"
        t = ndt("2 * 3 * %s" % dt)
        dtype = ndt(dt)

        self.assertEqual(t.ndim, 2)
        self.assertEqual(t.align, 8)
        self.assertEqual(t.itemsize, dtype.itemsize)
        self.assertEqual(t.shape, (2, 3))
        self.assertEqual(t.strides, (3 * dtype.itemsize, dtype.itemsize))

    def test_fixed_invariants(self):
        # Mixing var and fixed is disallowed.
        self.assertRaises(TypeError, ndt, "10 * var * int8")
        self.assertRaises(TypeError, ndt, "var * 10 * int16")
        self.assertRaises(TypeError, ndt, "10 * var * 10 * int32")
        self.assertRaises(TypeError, ndt, "var * 10 * var * int64")

    def test_fixed_dim(self):
        for dtype, mem in DTYPE_TEST_CASES:
            t = ndt(dtype)
            self.assertEqual(t.ndim, 0)
            self.assertEqual(t.itemsize, mem.itemsize)
            self.assertEqual(t.shape, ())
            self.assertEqual(t.strides, ())
            self.assertEqual(t.align, mem.align)

            for i in range(10):
                t = ndt("%d * %s" % (i, dtype))
                shape = (i,)
                strides = (mem.itemsize,)

                self.assertEqual(t.ndim, 1)
                self.assertEqual(t.itemsize, mem.itemsize)
                self.assertEqual(t.shape, shape)
                self.assertEqual(t.strides, strides)
                self.assertEqual(t.align, mem.align)

            for i in range(10):
                for j in range(10):
                    t = ndt("%d * %d * %s" % (i, j, dtype))
                    shape = (i, j)
                    strides = (j * mem.itemsize, mem.itemsize)

                    self.assertEqual(t.ndim, 2)
                    self.assertEqual(t.itemsize, mem.itemsize)
                    self.assertEqual(t.shape, shape)
                    self.assertEqual(t.strides, strides)
                    self.assertEqual(t.align, mem.align)

            for i in range(5):
                for j in range(5):
                    for k in range(5):
                        t = ndt("%d * %d * %d * %s" % (i, j, k, dtype))
                        shape = (i, j, k)
                        strides = (j * k * mem.itemsize, k * mem.itemsize, mem.itemsize)

                        self.assertEqual(t.ndim, 3)
                        self.assertEqual(t.itemsize, mem.itemsize)
                        self.assertEqual(t.shape, shape)
                        self.assertEqual(t.strides, strides)
                        self.assertEqual(t.align, mem.align)


class TestFortran(unittest.TestCase):

    def test_fortran(self):
        for dtype, mem in DTYPE_TEST_CASES:
            t = ndt(dtype)
            self.assertEqual(t.ndim, 0)
            self.assertEqual(t.itemsize, mem.itemsize)
            self.assertEqual(t.shape, ())
            self.assertEqual(t.strides, ())
            self.assertEqual(t.align, mem.align)

            for i in range(10):
                t = ndt("!%d * %s" % (i, dtype))
                shape = (i,)
                strides = (mem.itemsize,)

                self.assertEqual(t.ndim, 1)
                try:
                    self.assertEqual(t.itemsize, mem.itemsize)
                except AssertionError:
                    print(i, dtype)
                    raise

                self.assertEqual(t.shape, shape)
                self.assertEqual(t.strides, strides)
                self.assertEqual(t.align, mem.align)

            for i in range(10):
                for j in range(10):
                    t = ndt("!%d * %d * %s" % (i, j, dtype))
                    shape = (i, j)
                    strides = (mem.itemsize, i * mem.itemsize)

                    self.assertEqual(t.ndim, 2)
                    self.assertEqual(t.itemsize, mem.itemsize)
                    self.assertEqual(t.shape, shape)
                    self.assertEqual(t.strides, strides)
                    self.assertEqual(t.align, mem.align)

            for i in range(5):
                for j in range(5):
                    for k in range(5):
                        t = ndt("!%d * %d * %d * %s" % (i, j, k, dtype))
                        shape = (i, j, k)
                        strides = (mem.itemsize, i * mem.itemsize, i * j * mem.itemsize)

                        self.assertEqual(t.ndim, 3)
                        self.assertEqual(t.itemsize, mem.itemsize)
                        self.assertEqual(t.shape, shape)
                        self.assertEqual(t.strides, strides)
                        self.assertEqual(t.align, mem.align)


class TestVarDim(unittest.TestCase):

    def test_var_invariants(self):
        # Mixing var and fixed is disallowed.
        self.assertRaises(TypeError, ndt, "10 * var * int64")
        self.assertRaises(TypeError, ndt, "var * 10 * int64")
        self.assertRaises(TypeError, ndt, "10 * var * 10 * int64")
        self.assertRaises(TypeError, ndt, "var * 10 * var * int64")
        self.assertRaises(TypeError, ndt, "N * var * int64")
        self.assertRaises(TypeError, ndt, "var * N * int64")
        self.assertRaises(TypeError, ndt, "N * var * N * int64")
        self.assertRaises(TypeError, ndt, "var * N * var * int64")

        # Too many dimensions.
        self.assertRaises(TypeError, ndt, "var * " * (MAX_DIM + 1) + "float64")

        # Nested var is disallowed.
        self.assertRaises(TypeError, ndt, "2 * {a: var * complex128}")
        self.assertRaises(TypeError, ndt, "var * {a: var * complex128}")
        self.assertRaises(TypeError, ndt, "var * ref(var * string)")
        self.assertRaises(TypeError, ndt, "var * SomeConstr(var * string)")

    def test_external_offsets(self):
        # Invalid offsets.
        self.assertRaises(TypeError, ndt, "int8", [""])
        self.assertRaises(TypeError, ndt, "int8", [0])
        self.assertRaises(TypeError, ndt, "int8", [0, 2])
        self.assertRaises(TypeError, ndt, "int8", {})
        self.assertRaises(TypeError, ndt, "int8", ())
        self.assertRaises(TypeError, ndt, "int8", [(), ()])

        self.assertRaises(ValueError, ndt, "int8", [])
        self.assertRaises(ValueError, ndt, "int8", [[0]])
        self.assertRaises(ValueError, ndt, "int8", [[0], [0]])

        self.assertRaises(ValueError, ndt, "int8", [[-1, 2]])
        self.assertRaises(ValueError, ndt, "int8", [[0, 2147483648]])

        # Invalid combinations.
        self.assertRaises(ValueError, ndt, "int8", [[0, 2], [0, 10]])
        self.assertRaises(ValueError, ndt, "int8", [[0, 2], [0, 10, 30, 40]])

        # Implicit mixing of var and fixed.
        self.assertRaises(TypeError, ndt, "10 * int8", [[0, 2], [0, 10, 20]])

        # Abstract dtype.
        self.assertRaises(ValueError, ndt, "N * int8", [[0, 2], [0, 10, 20]])
        self.assertRaises(ValueError, ndt, "var * int8", [[0, 2], [0, 10, 20]])

        # Mixing external and internal offsets.
        self.assertRaises(TypeError, ndt, "var(offsets=[0,2,10]) * int8", [[0, 1], [0, 2]])


class TestCopy(unittest.TestCase):

    def test_copy(self):
        for dtype, mem in DTYPE_TEST_CASES:
            t = ndt(dtype)
            u = copy(t)
            self.assertEqual(u, t)
            self.assertEqual(u.ast_repr(), t.ast_repr())

    def test_copy_gc(self):
        x = ndt("var(offsets=[0,2]) * var(offsets=[0,3,10]) * int8")
        y = copy(x)
        del x
        gc.collect()
        del y
        gc.collect()

        x = ndt("{z: 10 * int8}", [[0, 2], [0, 10, 20]])
        y = copy(x)
        del x
        gc.collect()
        del y
        gc.collect()


class TestError(unittest.TestCase):

    def test_exceptions(self):
        self.assertRaises(TypeError, ndt, None)
        self.assertRaises(ValueError, ndt, "")
        self.assertRaises(ValueError, ndt, "xyz")
        self.assertRaises(ValueError, ndt, "var() * int64")


class TestConstruction(unittest.TestCase):

    def test_roundtrip(self):
        test_cases = [
            "2 * 3 * float64",
            "2 * 3 * {a : uint8, b : complex64}",
        ]
        for s in test_cases:
            t = ndt(s)
            self.assertEqual(str(t), s)

class TestApply(unittest.TestCase):

    def test_apply(self):
        # Type checking and return type inference for function applications.

        # Function type:
        f = ndt("(Dims... * N * M * int64, Dims... * M * P * int64) -> Dims... * N * P * float64")

        # Argument types:
        args = ndt("(20 * 2 * 3 * int64, 20 * 3 * 4 * int64)")

        spec = f.apply(args)
        self.assertEqual(spec.func, f)
        self.assertEqual(spec.args, args)
        self.assertEqual(spec.ret, ndt("20 * 2 * 4 * float64"))
        self.assertEqual(spec.outer_dims, 1)

    def test_apply_error(self):

        f = ndt("(Dims... * N * M * int64, Dims... * M * P * int64) -> Dims... * N * P * float64")

        lst = ["(20 * 2 * 3 * int8, 20 * 3 * 4 * int64)",
               "(10 * 2 * 3 * int64, 20 * 3 * 4 * int64)",
               "(20 * 2 * 100 * int64, 20 * 3 * 4 * int64)"]

        for s in lst:
            args = ndt(s)
            self.assertRaises(TypeError, f.apply, args)


ALL_TESTS = [
  TestModule,
  TestFunction,
  TestAny,
  TestFixedDim,
  TestFortran,
  TestVarDim,
  TestCopy,
  TestConstruction,
  TestError,
  TestApply,
]

if __name__ == '__main__':
    suite = unittest.TestSuite()
    loader = unittest.TestLoader()

    for case in ALL_TESTS:
        s = loader.loadTestsFromTestCase(case)
        suite.addTest(s)

    runner = unittest.TextTestRunner(failfast=ARGS.failfast, verbosity=2)
    result = runner.run(suite)
    ret = not result.wasSuccessful()

    sys.exit(ret)
