/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017-2018, plures
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "pymacro.h"


/******************************************************************************/
/*                                Module                                      */
/******************************************************************************/


PyDoc_STRVAR(doc_module, "ndtypes module");



/******************************************************************************/
/*                             ndt properties                                 */
/******************************************************************************/

PyDoc_STRVAR(doc_align,
"\n\
Return the alignment specified by the type.\n\
\n\
    >>> t = ndt(\"{a: 100000 * uint8, b: 200000 * uint8, align=512}\")\n\
    >>> t.align\n\
    512\n\
\n");

PyDoc_STRVAR(doc_datasize,
"\n\
Return the datasize required by the type.  The type must be concrete.\n\
\n\
    >>> t = ndt(\"10 * 2 * int64\")\n\
    >>> t.datasize\n\
    160\n\
    >>>\n\
    >>> t = ndt(\"10 * N * int64\")\n\
    >>> t.datasize\n\
    Traceback (most recent call last):\n\
      File \"<stdin>\", line 1, in <module>\n\
    TypeError: abstract type has no datasize\n\
\n");

PyDoc_STRVAR(doc_itemsize,
"\n\
Return the itemsize of the type. For types with ndim==0, itemsize==datasize.\n\
\n\
   >>> t = ndt(\"2 * 10 * complex128\")\n\
   >>> t.itemsize\n\
   16\n\
\n");

PyDoc_STRVAR(doc_ndim,
"\n\
Return the number of dimensions of the type.\n\
\n\
    >>> t = ndt(\"2 * 3 * 4 * 5 * complex64\")\n\
    >>> t.ndim\n\
    4\n\
\n");

PyDoc_STRVAR(doc_shape,
"\n\
Return the shape of the type. For NumPy compatibility, return the empty\n\
tuple if ndim==0.\n\
\n\
    >>> t = ndt(\"2 * 3 * 4 * 5 * complex64\")\n\
    >>> t.shape\n\
    (2, 3, 4, 5)\n\
\n\
    >>> t = ndt(\"int64\")\n\
    >>> t.shape\n\
    ()\n\
\n");

PyDoc_STRVAR(doc_strides,
"\n\
Return the strides of the type. For NumPy compatibility, return the empty\n\
tuple if ndim==0.  Internally, ndtypes uses steps instead of strides. The\n\
equality stride==step*itemsize always holds.\n\
\n\
    >>> t = ndt(\"2 * 3 * 4 * 5 * complex64\")\n\
    >>> t.strides\n\
    (480, 160, 40, 8)\n\
\n\
    >>> t = ndt(\"int64\")\n\
    >>> t.strides\n\
    ()\n\
\n");


/******************************************************************************/
/*                                ndt methods                                 */
/******************************************************************************/

PyDoc_STRVAR(doc_ast_repr,
"ast_repr($self, /)\n--\n\n\
Return the string representation of the internal abstract syntax tree.\n\
\n\
    >>> t = ndt(\"2 * {a: uint8, b: int32, pack=1}\")\n\
    >>> print(t.ast_repr())\n\
    FixedDim(\n\
      Record(\n\
        RecordField(\n\
          name='a',\n\
          type=Uint8(access=Concrete, ndim=0, datasize=1, align=1, flags=[]),\n\
          offset=0, align=1, pad=0\n\
        ),\n\
        RecordField(\n\
          name='b',\n\
          type=Int32(access=Concrete, ndim=0, datasize=4, align=4, flags=[]),\n\
          offset=1, align=1, pad=0\n\
        ),\n\
        access=Concrete, ndim=0, datasize=5, align=1, flags=[]\n\
      ),\n\
      shape=2, itemsize=5, step=1,\n\
      access=Concrete, ndim=1, datasize=10, align=1, flags=[]\n\
    )\n\
\n");

PyDoc_STRVAR(doc_from_format,
"from_format($self, fmt, /)\n--\n\n\
Create a type from a buffer protocol format string.\n\
\n\
    >>> ndt.from_format(\"T{B:a:L:b:}\")\n\
    ndt(\"{a : uint8, b : uint64}\")\n\
\n");

PyDoc_STRVAR(doc_deserialize,
"deserialize($self, bytes, /)\n--\n\n\
Deserialize a bytes object to a type .\n\
\n\
    >>> t = ndt(\"int64\")\n\
    >>> b = t.serialize()\n\
    >>> ndt.deserialize(b)\n\
    ndt(\"int64\")\n\
\n");

PyDoc_STRVAR(doc_is_c_contiguous,
"is_c_contiguous($self, /)\n--\n\n\
Return True if the type is a C-contiguous array and False otherwise.\n\
\n\
    >>> t = ndt(\"3 * 10 * float64\")\n\
    >>> t.is_c_contiguous()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_is_f_contiguous,
"is_f_contiguous($self, /)\n--\n\n\
Return True if the type is a Fortran-contiguous array and False otherwise.\n\
\n\
    >>> t = ndt(\"!3 * 10 * float64\")\n\
    >>> t.is_c_contiguous()\n\
    False\n\
    >>> t.is_f_contiguous()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_isabstract,
"isabstract($self, /)\n--\n\n\
Return True if the type is abstract and False otherwise.\n\
\n\
    >>> t = ndt(\"(int8, string)\")\n\
    >>> t.isabstract()\n\
    False\n\
\n\
    >>> t = ndt(\"(int8, T)\")\n\
    >>> t.isabstract()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_iscomplex,
"iscomplex($self, /)\n--\n\n\
Return True if the type is complex and False otherwise.\n\
\n\
    >>> t = ndt(\"complex128\")\n\
    >>> t.iscomplex()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_isconcrete,
"isconcrete($self, /)\n--\n\n\
Return True if the type is concrete and False otherwise.\n\
\n\
    >>> t = ndt(\"2 * int64\")\n\
    >>> t.isconcrete()\n\
    True\n\
    >>>\n\
    >>> t = ndt(\"N * int64\")\n\
    >>> t.isconcrete()\n\
    False\n\
\n");

PyDoc_STRVAR(doc_isfloat,
"isfloat($self, /)\n--\n\n\
Return True if the type is a float type and False otherwise.\n\
\n\
   >>> ndt(\"float16\").isfloat()\n\
   True\n\
   >>> ndt(\"float32\").isfloat()\n\
   True\n\
   >>> ndt(\"float64\").isfloat()\n\
   True\n\
\n");

PyDoc_STRVAR(doc_isoptional,
"isoptional($self, /)\n--\n\n\
Return True if the type is optional and False otherwise. Optional types\n\
support missing values (NA).\n\
\n\
    >>> ndt(\"bytes\").isoptional()\n\
    False\n\
    >>> ndt(\"?bytes\").isoptional()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_isscalar,
"isscalar($self, /)\n--\n\n\
Return True if the type is a scalar and False otherwise.\n\
\n\
   >>> ndt(\"uint32\").isscalar()\n\
   True\n\
   >>> ndt(\"2 * uint32\").isscalar()\n\
   False\n\
\n");

PyDoc_STRVAR(doc_issigned,
"issigned($self, /)\n--\n\n\
Return True if the type is a signed integer and False otherwise.\n\
\n\
    >>> ndt(\"int8\").issigned()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_isunsigned,
"isunsigned($self, /)\n--\n\n\
Return True if the type is an unsigned integer and False otherwise.\n\
\n\
    >>> ndt(\"uint8\").isunsigned()\n\
    True\n\
\n");

PyDoc_STRVAR(doc_match,
"match($self, pattern, candidate, /)\n--\n\n\
Determine whether the concrete candidate type matches the (possibly abstract)\n\
pattern type.  Used in type checking.\n\
\n\
    >>> p = ndt(\"N * M * int64\")\n\
    >>> c1 = ndt(\"2 * 3 * int64\")\n\
    >>> c2 = ndt(\"2 * 3 * int8\")\n\
    >>>\n\
    >>> p.match(c1)\n\
    True\n\
    >>> p.match(c2)\n\
    False\n\
\n");

PyDoc_STRVAR(doc_pformat,
"pformat($self, /)\n--\n\n\
Return a formatted string representation.\n\
\n\
    >>> t = ndt(\"{a: {x: uint8, y: int64}, b: (string, bytes)}\")\n\
    >>> t.pformat()\n\
    '{\\n  a : {\\n    x : uint8,\\n    y : int64\\n  },\\n  b : (string, bytes)\\n}'\n\
\n");

PyDoc_STRVAR(doc_pprint,
"pprint($self, /)\n--\n\n\
Print a formatted string representation to stdout.\n\
\n\
    >>> t = ndt(\"{a: {x: uint8, y: int64}, b: (string, bytes)}\")\n\
    >>> t.pprint()\n\
    {\n\
      a : {\n\
        x : uint8,\n\
        y : int64\n\
      },\n\
      b : (string, bytes)\n\
    }\n\
\n");

PyDoc_STRVAR(doc_serialize,
"serialize($self, /)\n--\n\n\
Serialize a type to a bytes format.\n\
\n\
    >>> t = ndt(\"int64\")\n\
    >>> t.serialize()\n\
    b'\\x1a\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x08\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x08\\x00'\n\
\n");
