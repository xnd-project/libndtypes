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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"


/*****************************************************************************/
/*                          Structural equality                              */
/*****************************************************************************/

static inline int
ndt_common_equal(const ndt_t *p, const ndt_t *c)
{
    return c->ndim == p->ndim;
}

static int
tuple_fields_equal(const ndt_t *p, const ndt_t *c)
{
    int64_t i;

    assert(p->tag == Tuple && c->tag == Tuple);

    if (p->Tuple.shape != c->Tuple.shape) {
        return 0;
    }

    for (i = 0; i < p->Tuple.shape; i++) {
        if (!ndt_equal(p->Tuple.types[i], c->Tuple.types[i])) {
            return 0;
        }
    }

    return 1;
}

static int
record_fields_equal(const ndt_t *p, const ndt_t *c)
{
    int64_t i;

    assert(p->tag == Record && c->tag == Record);

    if (p->Record.shape != c->Record.shape) {
        return 0;
    }

    for (i = 0; i < p->Record.shape; i++) {
        if (strcmp(p->Record.names[i], c->Record.names[i]) != 0) {
            return 0;
        }

        if (!ndt_equal(p->Record.types[i], c->Record.types[i])) {
            return 0;
        }
    }

    return 1;
}

static int
categorical_equal(ndt_memory_t *p, size_t plen,
                  ndt_memory_t *c, size_t clen)
{
    size_t i;

    if (plen != clen) {
        return 0;
    }

    for (i = 0; i < plen; i++) {
        if (!ndt_memory_equal(&p[i], &c[i])) {
            return 0;
        }
    }

    return 1;
}

int
ndt_equal(const ndt_t *p, const ndt_t *c)
{
    if (!ndt_common_equal(p, c)) {
        return 0;
    }

    switch (p->tag) {
    case AnyKind:
    case ScalarKind:
    case SignedKind: case UnsignedKind: case RealKind: case ComplexKind:
    case FixedStringKind: case FixedBytesKind:
    case Void: case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex64: case Complex128:
    case String:
        return c->tag == p->tag;
    case FixedString:
        return c->tag == FixedString &&
               c->FixedString.size == p->FixedString.size &&
               c->FixedString.encoding == p->FixedString.encoding;
    case FixedBytes:
        return c->tag == FixedBytes &&
               c->FixedBytes.size == p->FixedBytes.size &&
               c->FixedBytes.align == p->FixedBytes.align;
    case Char:
        return c->tag == Char && c->Char.encoding == p->Char.encoding;
    case Bytes:
        return c->tag == Bytes && c->Bytes.target_align == p->Bytes.target_align;
    case Categorical:
        return c->tag == Categorical &&
               categorical_equal(p->Categorical.types, p->Categorical.ntypes,
                                 c->Categorical.types, c->Categorical.ntypes);
    case Pointer:
        if (c->tag != Pointer) return 0;
        return ndt_equal(p->Pointer.type, c->Pointer.type);
    case FixedDim:
        return c->tag == FixedDim && c->FixedDim.shape == p->FixedDim.shape &&
               ndt_equal(c->FixedDim.type, p->FixedDim.type);
    case SymbolicDim:
        return c->tag == SymbolicDim &&
               strcmp(c->SymbolicDim.name, p->SymbolicDim.name) == 0 &&
               ndt_equal(c->SymbolicDim.type, p->SymbolicDim.type);
    case VarDim:
        return c->tag == VarDim &&
               ndt_equal(c->VarDim.type, p->VarDim.type);
    case EllipsisDim:
        return c->tag == EllipsisDim &&
               ndt_equal(c->EllipsisDim.type, p->EllipsisDim.type);
    case Tuple:
        if (c->tag != Tuple || c->Tuple.flag != p->Tuple.flag) return 0;
        return tuple_fields_equal(p, c);
    case Record:
        if (c->tag != Record || c->Tuple.flag != p->Tuple.flag) return 0;
        return record_fields_equal(p, c);
    case Function:
        return c->tag == Function &&
               ndt_equal(p->Function.ret, c->Function.ret) &&
               ndt_equal(p->Function.pos, c->Function.pos) &&
               ndt_equal(p->Function.kwds, c->Function.kwds);
    case Typevar:
        return c->tag == Typevar && strcmp(c->Typevar.name, p->Typevar.name) == 0;
    case Option:
        return c->tag == Option && ndt_equal(p->Option.type, c->Option.type);
    case Nominal:
        /* Assume that the type has been created through ndt_nominal(), in
           which case the name is guaranteed to be unique and present in the
           typedef table. */
        return c->tag == Nominal && strcmp(p->Nominal.name, c->Nominal.name) == 0;
    case Constr:
        return c->tag == Constr && strcmp(p->Constr.name, c->Constr.name) == 0 &&
               ndt_equal(p->Constr.type, c->Constr.type);
    default: /* NOT REACHED */
        abort();
    }
}
