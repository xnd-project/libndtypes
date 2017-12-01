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


static inline void
copy_common(ndt_t *u, const ndt_t *t)
{
    assert(u->tag = t->tag);
    u->access = t->access;
    u->ndim = t->ndim;
    u->hash = t->hash;
    u->data_size = t->data_size;
    u->data_align = t->data_align;
}

static ndt_t *
ndt_copy_tuple(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Tuple);

    u = ndt_tuple_alloc(t->Tuple.flag, t->Tuple.shape, ctx);
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);

    for (i = 0; i < t->Tuple.shape; i++) {
        u->Tuple.types[i] = ndt_copy(t->Tuple.types[i], ctx);
        if (u->Tuple.types[i] == NULL) {
            ndt_del(u);
            return NULL;
        }

        u->Concrete.Tuple.offset[i] = t->Concrete.Tuple.offset[i];
        u->Concrete.Tuple.align[i] = t->Concrete.Tuple.align[i];
        u->Concrete.Tuple.pad[i] = t->Concrete.Tuple.pad[i];
    }

    return u;
}

static ndt_t *
ndt_copy_record(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Record);

    u = ndt_record_alloc(t->Record.flag, t->Record.shape, ctx);
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);

    for (i = 0; i < t->Record.shape; i++) {
        u->Record.names[i] = ndt_strdup(t->Record.names[i], ctx);
        if (u->Record.names[i] == NULL) {
            ndt_del(u);
            return NULL;
        }

        u->Record.types[i] = ndt_copy(t->Record.types[i], ctx);
        if (u->Record.types[i] == NULL) {
            ndt_del(u);
            return NULL;
        }

        u->Concrete.Record.offset[i] = t->Concrete.Record.offset[i];
        u->Concrete.Record.align[i] = t->Concrete.Record.align[i];
        u->Concrete.Record.pad[i] = t->Concrete.Record.pad[i];
    }

    return u;
}

ndt_t *
ndt_copy(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *type;

    switch (t->tag) {
    case AnyKind:
        return ndt_any_kind(ctx);

    case ScalarKind:
        return ndt_scalar_kind(ctx);

    case SignedKind:
        return ndt_signed_kind(ctx);

    case UnsignedKind:
        return ndt_unsigned_kind(ctx);

    case FloatKind:
        return ndt_float_kind(ctx);

    case ComplexKind:
        return ndt_complex_kind(ctx);

    case FixedStringKind:
        return ndt_fixed_string_kind(ctx);

    case FixedBytesKind:
        return ndt_fixed_bytes_kind(ctx);

    /* XXX endian ignored */
    case Void: case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
        return ndt_primitive(t->tag, 'L', ctx);

    case String:
        return ndt_string(ctx);

    case FixedString:
        return ndt_fixed_string(t->FixedString.size, t->FixedString.encoding, ctx);

    case FixedBytes: {
        uint16_opt_t align = {Some, t->FixedBytes.align};
        return ndt_fixed_bytes(t->FixedBytes.size, align, ctx);
    }

    case Char:
        return ndt_char(t->Char.encoding, ctx);

    case Bytes: { /* XXX change API */
        uint16_opt_t target_align = {Some, t->Bytes.target_align};
        return ndt_bytes(target_align, ctx);
    }

    case Categorical: {
        size_t ntypes = t->Categorical.ntypes;
        ndt_value_t *types;
        size_t i, k;

        types = ndt_alloc(ntypes, sizeof(ndt_value_t));
        if (types == NULL) {
            return ndt_memory_error(ctx);
        }

        for (i = 0; i < ntypes; i++) {
            types[i] = t->Categorical.types[i];
            switch (types[i].tag) {
            case ValNA: case ValBool: case ValInt64: case ValFloat64:
                break;
            case ValString:
                types[i].ValString = ndt_strdup(t->Categorical.types[i].ValString, ctx);
                if (types[i].ValString == NULL) {
                    goto err_categorical;
                }
                break;
            }

            continue;

        err_categorical:
            for (k = 0; k < i; k++) {
                switch (types[k].tag) {
                case ValNA: case ValBool: case ValInt64: case ValFloat64:
                    break;
                case ValString:
                    ndt_free(types[k].ValString);
                    break;
                }
            }
            ndt_free(types);
            return NULL;
        }

        return ndt_categorical(types, ntypes, ctx);
    }

    case Ref: {
        type = ndt_copy(t->Ref.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        return ndt_ref(type, ctx);
    }

    case FixedDim: {
        type = ndt_copy(t->FixedDim.type, ctx);
        if (type == NULL) {
            assert(ctx->err != NDT_Success);
            return NULL;
        }

        if (ndt_is_abstract(t)) {
            return ndt_fixed_dim(type, t->FixedDim.shape, INT64_MAX, ctx);
        }
        else {
            return ndt_fixed_dim(type, t->FixedDim.shape, t->Concrete.FixedDim.stride, ctx);
        }
    }

    case SymbolicDim: {
        char *name = ndt_strdup(t->SymbolicDim.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        type = ndt_copy(t->SymbolicDim.type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        return ndt_symbolic_dim(name, type, ctx);
    }

    case VarDim: {
        type = ndt_copy(t->VarDim.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        if (ndt_is_abstract(t))  {
            return ndt_var_dim(type, NoOffsets, 0, NULL, 0, NULL, ctx);
        }
        else {
            int32_t noffsets = t->Concrete.VarDim.noffsets;
            const int32_t *offsets = t->Concrete.VarDim.offsets;
            int32_t nslices = t->Concrete.VarDim.nslices;
            ndt_slice_t *slices = NULL;

            if (nslices > 0) {
                slices = ndt_alloc(nslices, sizeof *slices);
                if (slices == NULL) {
                    return ndt_memory_error(ctx);
                }
                memcpy(slices, t->Concrete.VarDim.slices,
                       nslices * (sizeof *slices));
            }

            return ndt_var_dim(type, ExternalOffsets, noffsets, offsets,
                               nslices, slices, ctx);
        }
    }

    case EllipsisDim: {
        char *name = NULL;

        if (t->EllipsisDim.name != NULL) {
            name = ndt_strdup(t->SymbolicDim.name, ctx);
            if (name == NULL) {
                return NULL;
            }
        }

        type = ndt_copy(t->EllipsisDim.type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        return ndt_ellipsis_dim(name, type, ctx);
    }

    case Tuple: {
        return ndt_copy_tuple(t, ctx);
    }

    case Record: {
        return ndt_copy_record(t, ctx);
    }

    case Function: {
        ndt_t *ret, *pos, *kwds;

        ret = ndt_copy(t->Function.ret, ctx);
        if (ret == NULL) {
            return NULL;
        }

        pos = ndt_copy(t->Function.pos, ctx);
        if (pos == NULL) {
            ndt_del(ret);
            return NULL;
        }

        kwds = ndt_copy(t->Function.kwds, ctx);
        if (pos == NULL) {
            ndt_del(ret);
            ndt_del(kwds);
            return NULL;
        }

        return ndt_function(ret, pos, kwds, ctx);
    }

    case Typevar: {
        char *name = ndt_strdup(t->Typevar.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        return ndt_typevar(name, ctx);
    }

    case Option: {
        type = ndt_copy(t->Option.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        return ndt_option(type, ctx);
    }

    case OptionItem: {
        type = ndt_copy(t->OptionItem.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        return ndt_item_option(type, ctx);
    }

    case Nominal: {
        char *name = ndt_strdup(t->Nominal.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        return ndt_nominal(name, ctx);
    }

    case Module: {
        char *name = ndt_strdup(t->Module.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        type = ndt_copy(t->Module.type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        return ndt_module(name, type, ctx);
    }

    case Constr: {
        char *name = ndt_strdup(t->Constr.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        type = ndt_copy(t->Constr.type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        return ndt_constr(name, type, ctx);
    }

    default: /* NOT REACHED */
        abort();
    }
}
