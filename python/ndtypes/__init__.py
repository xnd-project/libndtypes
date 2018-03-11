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
#
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

"""
A module for typing memory blocks using a close variant of the datashape type
language.

The underlying library -- libndtypes -- implements the type part of a compiler
frontend.  It can describe C types needed for array computing and additionally
includes symbolic types for dynamic type checking.

libndtypes has the concept of abstract and concrete types. Concrete types
contain the exact data layout and all sizes that are required to access
subtypes or individual elements in memory.

Abstract types are for type checking and include functions, symbolic dimensions
and type variables. Module support is planned at a later stage.

Concrete types with rich layout information make it possible to write
relatively small container libraries that can traverse memory without
type erasure.

The xnd module uses ndtypes to implement a general container for mapping most
Python types relevant for scientific computing directly to memory.
"""

def _pretty_repr(self):
    return """\
ApplySpec(
  tag=%r,
  sig=%r,
  in_types=%r,
  in_broadcast=%r,
  out_types=%r,
  outer_dims=%r
)""" % (self.tag, self.sig, self.in_types, self.in_broadcast, self.out_types, self.outer_dims)

from . import _ndtypes
_ndtypes.ApplySpec.__repr__ = _pretty_repr
from ._ndtypes import *
