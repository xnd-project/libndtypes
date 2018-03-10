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
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"
#include "symtable.h"
#include "substitute.h"


static ndt_t *
substitute_named_ellipsis(const ndt_t *t, const symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    const ndt_t *w;
    ndt_t *u;
    int i;

    assert(t->tag == EllipsisDim && t->EllipsisDim.name != NULL);

    u = ndt_substitute(t->EllipsisDim.type, tbl, ctx);
    if (u == NULL) {
        return NULL;
    }

    v = symtable_find(tbl, t->EllipsisDim.name);

    switch (v.tag) {
    case DimListEntry: {
        for (i = v.DimListEntry.size-1; i >= 0; i--) {
            w = v.DimListEntry.dims[i];
            switch (w->tag) {
            case FixedDim:
                u = ndt_fixed_dim(u, w->FixedDim.shape, INT64_MAX, ctx);
                if (u == NULL) {
                    return NULL;
                }
                break;

           default:
               ndt_err_format(ctx, NDT_NotImplementedError,
                   "substitution not implemented for this type");
               ndt_del(u);
               return NULL;
            }
        }

        return u;
    }

    default:
        ndt_err_format(ctx, NDT_ValueError,
            "variable not found or has incorrect type");
        return NULL;
    }
}

ndt_t *
ndt_substitute(const ndt_t *t, const symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    ndt_t *u;

    switch (t->tag) {
    case FixedDim:
        u = ndt_substitute(t->FixedDim.type, tbl, ctx);
        if (u == NULL) {
            return NULL;
        }

        assert(ndt_is_concrete(u));

        return ndt_fixed_dim(u, t->FixedDim.shape, t->Concrete.FixedDim.step,
                             ctx);

    case SymbolicDim:
        v = symtable_find(tbl, t->SymbolicDim.name);

        switch (v.tag) {
        case ShapeEntry:
            u = ndt_substitute(t->SymbolicDim.type, tbl, ctx);
            if (u == NULL) {
                return NULL;
            }

            assert(ndt_is_concrete(u));

            return ndt_fixed_dim(u, v.ShapeEntry, INT64_MAX, ctx);

        default:
            ndt_err_format(ctx, NDT_ValueError,
                "variable is not found or has incorrect type");
            return NULL;
        }

    case EllipsisDim:
        if (t->EllipsisDim.name == NULL) {
            return ndt_substitute(t->EllipsisDim.type, tbl, ctx);
        }
        else {
            return substitute_named_ellipsis(t, tbl, ctx);
        }

    case Typevar:
        v = symtable_find(tbl, t->Typevar.name);
        switch (v.tag) {
        case TypeEntry:
            return ndt_substitute(v.TypeEntry, tbl, ctx);

        default:
            ndt_err_format(ctx, NDT_ValueError,
                "variable is not found or has incorrect type");
            return NULL;
        }

    case Ref:
        u = ndt_substitute(t->Ref.type, tbl, ctx);
        if (u == NULL) {
            return NULL;
        }

        return ndt_ref(u, ctx);

    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case FixedString: case FixedBytes:
    case String: case Bytes:
    case Char: {
        u = ndt_new(t->tag, ctx);
        if (u == NULL) {
            return NULL;
        }
        *u = *t;
        return u;
    }

    default:
        ndt_err_format(ctx, NDT_NotImplementedError,
            "substitution not implemented for this type");
        return NULL;
    }
}
