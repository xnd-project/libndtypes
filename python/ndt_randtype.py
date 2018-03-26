#
# BSD 3-Clause License
#
# Copyright (c) 2017-2018, plures
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

import sys
from itertools import accumulate, count, product
from collections import namedtuple
from random import randrange
from ndtypes import ndt, ApplySpec
from _testbuffer import get_sizeof_void_p


SIZEOF_PTR = get_sizeof_void_p()

Mem = namedtuple("Mem", "itemsize align")


# ======================================================================
#                    Check contiguous fixed dimensions
# ======================================================================

def c_datasize(t):
    """Check the datasize of contiguous arrays."""
    datasize = t.itemsize
    for v in t.shape:
        datasize *= v
    return datasize


# ======================================================================
#              Check fixed dimensions with arbitary strides
# ======================================================================

def verify_datasize(t):
    """Verify the datasize of fixed dimensions with arbitrary strides."""

    if t.itemsize == 0:
        return t.datasize == 0
    if t.datasize % t.itemsize:
        return False
    if t.ndim <= 0:
        return t.ndim == 0 and not t.shape and not t.strides
    if any(v < 0 for v in t.shape):
        return False
    if any(v % t.itemsize for v in t.strides):
        return False

    if 0 in t.shape:
        return t.datasize == 0

    imin = sum(t.strides[j]*(t.shape[j]-1) for j in range(t.ndim)
               if t.strides[j] <= 0)
    imax = sum(t.strides[j]*(t.shape[j]-1) for j in range(t.ndim)
               if t.strides[j] > 0)

    return t.datasize == (abs(imin) + imax + t.itemsize)


# ======================================================================
#                             Typed values
# ======================================================================

DTYPE_TEST_CASES = [
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

   # References to tuples
   ("&(uint16, (complex64), align=1)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("(uint16, &(complex64), pack=1)", Mem(itemsize=2+SIZEOF_PTR, align=1)),

   # Constructor containing references to tuples
   ("Some(&(uint16, (complex64), align=1))", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("Some((uint16, &(complex64), pack=1))", Mem(itemsize=2+SIZEOF_PTR, align=1)),

   # Optional tuples
   ("?(uint16, (complex64), align=1)", Mem(itemsize=12, align=4)),
   ("(uint16, ?(complex64), align=1)", Mem(itemsize=12, align=4)),
   ("?(uint16, ?(complex64), align=1)", Mem(itemsize=12, align=4)),
   ("?(uint16, (complex64), align=2)", Mem(itemsize=12, align=4)),
   ("(uint16, ?(complex64), align=4)", Mem(itemsize=12, align=4)),
   ("?(uint16, ?(complex64), align=8)", Mem(itemsize=16, align=8)),

   # References to optional tuples or tuples with optional subtrees
   ("&?(uint16, (complex64), align=1)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&(uint16, ?(complex64), align=1)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   # Constructor containing optional tuples or tuples with optional subtrees
   ("Some(?(uint16, (complex64), align=1))", Mem(itemsize=12, align=4)),
   ("Some((uint16, ?(complex64), align=1))", Mem(itemsize=12, align=4)),

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

   # Primitive optional types
   ("?bool", Mem(itemsize=1, align=1)),

   ("?int8", Mem(itemsize=1, align=1)),
   ("?int16", Mem(itemsize=2, align=2)),
   ("?int32", Mem(itemsize=4, align=4)),
   ("?int64", Mem(itemsize=8, align=8)),

   ("?uint8", Mem(itemsize=1, align=1)),
   ("?uint16", Mem(itemsize=2, align=2)),
   ("?uint32", Mem(itemsize=4, align=4)),
   ("?uint64", Mem(itemsize=8, align=8)),

   ("?float32", Mem(itemsize=4, align=4)),
   ("?float64", Mem(itemsize=8, align=8)),

   ("?complex64", Mem(itemsize=8, align=4)),
   ("?complex128", Mem(itemsize=16, align=8)),

   # References
   ("&bool", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("&int8", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&int16", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&int32", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&int64", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(uint8)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(uint16)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(uint32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(uint64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(float32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(float64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(complex64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(complex128)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   # Optional references
   ("?&bool", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("?&int8", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?&int16", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?&int32", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?&int64", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("?ref(uint8)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?ref(uint16)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?ref(uint32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?ref(uint64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("?ref(float32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?ref(float64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("?ref(complex64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("?ref(complex128)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   # References to optional types
   ("&?bool", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("&?int8", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&?int16", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&?int32", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("&?int64", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(?uint8)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(?uint16)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(?uint32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(?uint64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(?float32)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(?float64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   ("ref(?complex64)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),
   ("ref(?complex128)", Mem(itemsize=SIZEOF_PTR, align=SIZEOF_PTR)),

   # Constructors
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
   ("Some(complex128)", Mem(itemsize=16, align=8)),

   # Optional constructors
   ("?Some(bool)", Mem(itemsize=1, align=1)),

   ("?Some(int8)", Mem(itemsize=1, align=1)),
   ("?Some(int16)", Mem(itemsize=2, align=2)),
   ("?Some(int32)", Mem(itemsize=4, align=4)),
   ("?Some(int64)", Mem(itemsize=8, align=8)),

   ("?Some(uint8)", Mem(itemsize=1, align=1)),
   ("?Some(uint16)", Mem(itemsize=2, align=2)),
   ("?Some(uint32)", Mem(itemsize=4, align=4)),
   ("?Some(uint64)", Mem(itemsize=8, align=8)),

   ("?Some(float32)", Mem(itemsize=4, align=4)),
   ("?Some(float64)", Mem(itemsize=8, align=8)),

   ("?Some(complex64)", Mem(itemsize=8, align=4)),
   ("?Some(complex128)", Mem(itemsize=16, align=8)),

   # Constructors containing optional types
   ("Some(?bool)", Mem(itemsize=1, align=1)),

   ("Some(?int8)", Mem(itemsize=1, align=1)),
   ("Some(?int16)", Mem(itemsize=2, align=2)),
   ("Some(?int32)", Mem(itemsize=4, align=4)),
   ("Some(?int64)", Mem(itemsize=8, align=8)),

   ("Some(?uint8)", Mem(itemsize=1, align=1)),
   ("Some(?uint16)", Mem(itemsize=2, align=2)),
   ("Some(?uint32)", Mem(itemsize=4, align=4)),
   ("Some(?uint64)", Mem(itemsize=8, align=8)),

   ("Some(?float32)", Mem(itemsize=4, align=4)),
   ("Some(?float64)", Mem(itemsize=8, align=8)),

   ("Some(?complex64)", Mem(itemsize=8, align=4)),
   ("Some(?complex128)", Mem(itemsize=16, align=8)),
]

# ======================================================================
#                              Broadcasting
# ======================================================================

def genindices(factor):
    for i in range(4):
        yield ()
    for i in range(4):
        yield (factor * i,)
    for i in range(4):
        for j in range(4):
            yield (factor * i, factor * j)
    for i in range(4):
        for j in range(4):
            for k in range(4):
                yield (factor * i, factor * j, factor * k)

BROADCAST_TEST_CASES = [
    ApplySpec(
        tag = 'C',
        sig = ndt("uint8 -> float64"),
        in_types = [ndt("uint8")],
        out_types = [ndt("float64")],
        in_broadcast = [],
        outer_dims = 0),

    ApplySpec(
        tag = 'C',
        sig = ndt("... * uint8 -> ... * float64"),
        in_types = [ndt("2 * uint8")],
        out_types = [ndt("2 * float64")],
        in_broadcast = [ndt("2 * uint8")],
        outer_dims = 1),

    ApplySpec(
        tag = 'Fortran',
        sig = ndt("... * uint8 -> ... * float64"),
        in_types = [ndt("!2 * 3 * uint8")],
        out_types = [ndt("!2 * 3 * float64")],
        in_broadcast = [ndt("!2 * 3 * uint8")],
        outer_dims = 2),

    ApplySpec(
        tag = 'Strided',
        sig = ndt("... * uint8 -> ... * float64"),
        in_types = [ndt("fixed(shape=2, step=10) * uint8")],
        out_types = [ndt("2 * float64")],
        in_broadcast = [ndt("fixed(shape=2, step=10) * uint8")],
        outer_dims = 1),

    ApplySpec(
        tag = 'C',
        sig = ndt("... * N * uint8 -> ... * N * float64"),
        in_types = [ndt("2 * 3 * uint8")],
        out_types = [ndt("2 * 3 * float64")],
        in_broadcast = [ndt("2 * 3 * uint8")],
        outer_dims = 1),

    ApplySpec(
        tag = 'Xnd',
        sig = ndt("Dims... * var * float64 -> Dims... * var * float64"),
        in_types = [ndt("var(offsets=[0,2]) * var(offsets=[0,4,11]) * float64")],
        out_types = [ndt("var(offsets=[0,2]) * var(offsets=[0,4,11]) * float64")],
        in_broadcast = [],
        outer_dims = 1),

    ApplySpec(
        tag = 'Xnd',
        sig = ndt("N * D * float64 -> P * float64"),
        in_types = [ndt("2 * 10 * float64")],
        out_types = [ndt("P * float64")],
        in_broadcast = [],
        outer_dims = 0)
]
