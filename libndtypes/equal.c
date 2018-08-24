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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ndtypes.h"


/*****************************************************************************/
/*                          Structural equality                              */
/*****************************************************************************/

static inline int
ndt_common_equal(const ndt_t *t, const ndt_t *u)
{
    return t->tag == u->tag &&
           t->access == u->access &&
           t->flags == u->flags &&
           t->ndim == u->ndim &&
           t->datasize == u->datasize &&
           t->align == u->align;
}

static int
function_types_equal(const ndt_t *t, const ndt_t *u, int64_t nargs)
{
    int64_t i;

    assert(t->tag == Function && u->tag == Function);

    for (i = 0; i < nargs; i++) {
        if (!ndt_equal(t->Function.types[i], u->Function.types[i])) {
            return 0;
        }
    }

    return 1;
}

static int
tuple_fields_equal(const ndt_t *t, const ndt_t *u, int64_t shape)
{
    int64_t i;

    assert(t->tag == Tuple && u->tag == Tuple);

    for (i = 0; i < shape; i++) {
        if (t->Concrete.Tuple.offset[i] != u->Concrete.Tuple.offset[i] ||
            t->Concrete.Tuple.align[i] != u->Concrete.Tuple.align[i] ||
            t->Concrete.Tuple.pad[i] != u->Concrete.Tuple.pad[i]) {
            return 0;
        }
    }

    for (i = 0; i < shape; i++) {
        if (!ndt_equal(t->Tuple.types[i], u->Tuple.types[i])) {
            return 0;
        }
    }

    return 1;
}

static int
record_fields_equal(const ndt_t *t, const ndt_t *u, int64_t shape)
{
    int64_t i;

    assert(t->tag == Record && u->tag == Record);

    for (i = 0; i < shape; i++) {
        if (t->Concrete.Record.offset[i] != u->Concrete.Record.offset[i] ||
            t->Concrete.Record.align[i] != u->Concrete.Record.align[i] ||
            t->Concrete.Record.pad[i] != u->Concrete.Record.pad[i]) {
            return 0;
        }
    }

    for (i = 0; i < shape; i++) {
        if (strcmp(t->Record.names[i], u->Record.names[i]) != 0) {
            return 0;
        }

        if (!ndt_equal(t->Record.types[i], u->Record.types[i])) {
            return 0;
        }
    }

    return 1;
}

static int
categorical_equal(ndt_value_t *t, ndt_value_t *u, int64_t shape)
{
    int64_t i;

    for (i = 0; i < shape; i++) {
        if (!ndt_value_mem_equal(t+i, u+i)) {
            return 0;
        }
    }

    return 1;
}

int
ndt_equal(const ndt_t *t, const ndt_t *u)
{
    if (!ndt_common_equal(t, u)) {
        return 0;
    }

    switch (t->tag) {
    case Module: {
        return strcmp(t->Module.name, u->Module.name) == 0 &&
               ndt_equal(t->Module.type, u->Module.type);
    }

    case Function: {
        if (t->Function.nin != u->Function.nin ||
            t->Function.nout != u->Function.nout ||
            t->Function.nargs != u->Function.nargs) {
            return 0;
        }

        return function_types_equal(t, u, t->Function.nargs);
    }

    case FixedDim: {
        return t->FixedDim.tag == u->FixedDim.tag &&
               t->FixedDim.shape == u->FixedDim.shape &&
               t->Concrete.FixedDim.itemsize == u->Concrete.FixedDim.itemsize &&
               t->Concrete.FixedDim.step == u->Concrete.FixedDim.step &&
               ndt_equal(t->FixedDim.type, u->FixedDim.type);
    }

    case VarDim: {
        if (t->Concrete.VarDim.itemsize != u->Concrete.VarDim.itemsize ||
            t->Concrete.VarDim.noffsets != u->Concrete.VarDim.noffsets ||
            t->Concrete.VarDim.nslices != u->Concrete.VarDim.nslices) {
            return 0;
        }

        if (memcmp(t->Concrete.VarDim.offsets, u->Concrete.VarDim.offsets,
                   t->Concrete.VarDim.noffsets * (sizeof *t->Concrete.VarDim.offsets)) ||
            memcmp(t->Concrete.VarDim.slices, u->Concrete.VarDim.slices,
                   t->Concrete.VarDim.nslices * (sizeof *t->Concrete.VarDim.slices))) {
            return 0;
        }

        return ndt_equal(t->VarDim.type, u->VarDim.type);
    }

    case SymbolicDim: {
        return t->SymbolicDim.tag == u->SymbolicDim.tag &&
               strcmp(t->SymbolicDim.name, u->SymbolicDim.name) == 0 &&
               ndt_equal(t->SymbolicDim.type, u->SymbolicDim.type);
    }

    case EllipsisDim: {
        if (t->EllipsisDim.tag != u->EllipsisDim.tag) {
            return 0;
        }

        if (!!t->EllipsisDim.name != !!u->EllipsisDim.name) {
            return 0;
        }

        if (t->EllipsisDim.name &&
            strcmp(t->EllipsisDim.name, u->EllipsisDim.name) != 0) {
            return 0;
        }

        return ndt_equal(t->EllipsisDim.type, u->EllipsisDim.type);
    }

    case Tuple: {
        if (t->Tuple.flag != u->Tuple.flag ||
            t->Tuple.shape != u->Tuple.shape) {
            return 0;
        }

        return tuple_fields_equal(t, u, t->Tuple.shape);
    }

    case Record: {
        if (t->Record.flag != u->Record.flag ||
            t->Record.shape != u->Record.shape) {
            return 0;
        }

        return record_fields_equal(t, u, t->Record.shape);
    }

    case Ref: {
        return ndt_equal(t->Ref.type, u->Ref.type);
    }

    case Constr: {
        return strcmp(t->Constr.name, u->Constr.name) == 0 &&
               ndt_equal(t->Constr.type, u->Constr.type);
    }

    case Nominal: {
        return strcmp(t->Nominal.name, u->Nominal.name) == 0 &&
               ndt_equal(t->Nominal.type, u->Nominal.type);
    }

    case Categorical: {
        if (t->Categorical.ntypes != u->Categorical.ntypes) {
            return 0;
        }

        return categorical_equal(t->Categorical.types, u->Categorical.types,
                                 t->Categorical.ntypes);
    }

    case FixedString: {
        return t->FixedString.size == u->FixedString.size &&
               t->FixedString.encoding == u->FixedString.encoding;
    }

    case FixedBytes: {
        return t->FixedBytes.size == u->FixedBytes.size &&
               t->FixedBytes.align == u->FixedBytes.align;
    }

    case Bytes: {
        return t->Bytes.target_align == u->Bytes.target_align;
    }

    case Char: {
        return t->Char.encoding == u->Char.encoding;
    }

    case Typevar: {
        return strcmp(t->Typevar.name, u->Typevar.name) == 0;
    }

    case AnyKind:
    case ScalarKind:
    case SignedKind: case UnsignedKind:
    case FloatKind: case ComplexKind:
    case FixedStringKind: case FixedBytesKind:
    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case String:
        return 1;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid type");
}
