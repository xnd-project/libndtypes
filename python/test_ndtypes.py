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

import unittest
from ndtypes import ndt


class ErrorTest(unittest.TestCase):

    def test_exceptions(self):
        self.assertRaises(TypeError, ndt, None)
        self.assertRaises(ValueError, ndt, "")
        self.assertRaises(ValueError, ndt, "xyz")
        self.assertRaises(ValueError, ndt, "var() * int64")

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


