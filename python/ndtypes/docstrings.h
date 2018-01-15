/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, plures
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
