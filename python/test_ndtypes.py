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

import unittest, gc
from copy import copy
from ndtypes import ndt
from ndtypes import MAX_DIM


class ErrorTest(unittest.TestCase):

    def test_exceptions(self):
        self.assertRaises(TypeError, ndt, None)
        self.assertRaises(ValueError, ndt, "")
        self.assertRaises(ValueError, ndt, "xyz")
        self.assertRaises(ValueError, ndt, "var() * int64")

class VarDimTest(unittest.TestCase):

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
        self.assertRaises(TypeError, ndt, "var * pointer(var * string)")
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

    def test_copy(self):
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


class ConstructionTest(unittest.TestCase):

    def test_roundtrip(self):
        test_cases = [
            "2 * 3 * float64",
            "2 * 3 * {a : uint8, b : complex64}",
        ]
        for s in test_cases:
            t = ndt(s)
            self.assertEqual(str(t), s)

class ApplyTest(unittest.TestCase):

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


unittest.main(verbosity=2)


