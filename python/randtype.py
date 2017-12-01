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

# Functions for generating test cases.

from itertools import accumulate, count, product
from collections import namedtuple
from random import randrange
from _testbuffer import get_sizeof_void_p


PTR_SIZE = get_sizeof_void_p()
Mem = namedtuple("Mem", "itemsize align")


# ======================================================================
#                             Typed values
# ======================================================================

NDARRAY_TEST_CASES = [
   # Tuples
   ("()", Mem(itemsize=0, align=1)),
   ("(complex128)", Mem(itemsize=16, align=8)),

   ("(int8, int64)", Mem(itemsize=16, align=8)),
   ("(int8, int64, pack=1)", Mem(itemsize=9, align=1)),
   ("(int8, int64, pack=2)", Mem(itemsize=10, align=2)),
   ("(int8, int64, pack=4)", Mem(itemsize=12, align=4)),
   ("(int8, int64, pack=8)", Mem(itemsize=16, align=8)),
   ("(int8, int64, pack=16)", Mem(itemsize=32, align=16)),

   ("(int8, int64, align=1)", Mem(itemsize=16, align=8)),
   ("(int8, int64, align=2)", Mem(itemsize=16, align=8)),
   ("(int8, int64, align=4)", Mem(itemsize=16, align=8)),
   ("(int8, int64, align=8)", Mem(itemsize=16, align=8)),
   ("(int8, int64, align=16)", Mem(itemsize=16, align=16)),

   ("(int8 |align=1|, int64)", Mem(itemsize=16, align=8)),
   ("(int8 |align=2|, int64)", Mem(itemsize=16, align=8)),
   ("(int8 |align=4|, int64)", Mem(itemsize=16, align=8)),
   ("(int8 |align=8|, int64)", Mem(itemsize=16, align=8)),
   ("(int8 |align=16|, int64)", Mem(itemsize=16, align=16)),

   ("(uint16, (complex64))", Mem(itemsize=12, align=4)),
   ("(uint16, (complex64), pack=1)", Mem(itemsize=10, align=1)),
   ("(uint16, (complex64), pack=2)", Mem(itemsize=10, align=2)),
   ("(uint16, (complex64), pack=4)", Mem(itemsize=12, align=4)),
   ("(uint16, (complex64), pack=8)", Mem(itemsize=16, align=8)),

   ("(uint16, (complex64), align=1)", Mem(itemsize=12, align=4)),
   ("(uint16, (complex64), align=2)", Mem(itemsize=12, align=4)),
   ("(uint16, (complex64), align=4)", Mem(itemsize=12, align=4)),
   ("(uint16, (complex64), align=8)", Mem(itemsize=16, align=8)),

   # Records
   ("{}", Mem(itemsize=0, align=1)),
   ("{x: complex128}", Mem(itemsize=16, align=8)),

   ("{x: int8, y: int64}", Mem(itemsize=16, align=8)),
   ("{x: int8, y: int64, pack=1}", Mem(itemsize=9, align=1)),
   ("{x: int8, y: int64, pack=2}", Mem(itemsize=10, align=2)),
   ("{x: int8, y: int64, pack=4}", Mem(itemsize=12, align=4)),
   ("{x: int8, y: int64, pack=8}", Mem(itemsize=16, align=8)),
   ("{x: int8, y: int64, pack=16}", Mem(itemsize=32, align=16)),

   ("{x: uint16, y: {z: complex128}}", Mem(itemsize=24, align=8)),
   ("{x: uint16, y: {z: complex128, align=16}}", Mem(itemsize=32, align=16)),
   ("{x: uint16, y: {z: complex128}, align=16}", Mem(itemsize=32, align=16)),

   # Primitive types
   ("bool", Mem(itemsize=1, align=1)),

   ("int8", Mem(itemsize=1, align=1)),
   ("int16", Mem(itemsize=2, align=2)),
   ("int32", Mem(itemsize=4, align=4)),
   ("int64", Mem(itemsize=8, align=8)),

   ("uint8", Mem(itemsize=1, align=1)),
   ("uint16", Mem(itemsize=2, align=2)),
   ("uint32", Mem(itemsize=4, align=4)),
   ("uint64", Mem(itemsize=8, align=8)),

   ("float32", Mem(itemsize=4, align=4)),
   ("float64", Mem(itemsize=8, align=8)),

   ("complex64", Mem(itemsize=8, align=4)),
   ("complex128", Mem(itemsize=16, align=8)),

   # References
   ("&bool", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),

   ("&int8", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("&int16", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("&int32", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("&int64", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),

   ("ref(uint8)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("ref(uint16)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("ref(uint32)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("ref(uint64)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),

   ("ref(float32)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("ref(float64)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),

   ("ref(complex64)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),
   ("ref(complex128)", Mem(itemsize=PTR_SIZE, align=PTR_SIZE)),

   # Constructors
   ("Some(bool)", Mem(itemsize=1, align=1)),
   ("Some(bool)", Mem(itemsize=1, align=1)),

   ("Some(int8)", Mem(itemsize=1, align=1)),
   ("Some(int16)", Mem(itemsize=2, align=2)),
   ("Some(int32)", Mem(itemsize=4, align=4)),
   ("Some(int64)", Mem(itemsize=8, align=8)),

   ("Some(uint8)", Mem(itemsize=1, align=1)),
   ("Some(uint16)", Mem(itemsize=2, align=2)),
   ("Some(uint32)", Mem(itemsize=4, align=4)),
   ("Some(uint64)", Mem(itemsize=8, align=8)),

   ("Some(float32)", Mem(itemsize=4, align=4)),
   ("Some(float64)", Mem(itemsize=8, align=8)),

   ("Some(complex64)", Mem(itemsize=8, align=4)),
   ("Some(complex128)", Mem(itemsize=16, align=8))
]
