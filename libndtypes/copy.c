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
#include <string.h>
#include <assert.h>
#include "ndtypes.h"


static inline void
copy_common(ndt_t *u, const ndt_t *t)
{
    assert(u->tag == t->tag);
    u->access = t->access;
    u->flags = t->flags;
    u->ndim = t->ndim;
    u->datasize = t->datasize;
    u->align = t->align;
}

static ndt_t *
ndt_copy_var_dim(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *type;
    ndt_slice_t *slices;
    int nslices;

    assert(t->tag == VarDim);

    type = ndt_copy(t->VarDim.type, ctx);
    if (type == NULL) {
        return NULL;
    }

    if (ndt_is_abstract(t))  {
        return ndt_abstract_var_dim(type, ctx);
    }

    slices = NULL;
    nslices = t->Concrete.VarDim.nslices;

    if (nslices > 0) {
        slices = ndt_alloc(nslices, sizeof *slices);
        if (slices == NULL) {
            return ndt_memory_error(ctx);
        }
        memcpy(slices, t->Concrete.VarDim.slices,
               nslices * (sizeof *slices));
    }

    return ndt_var_dim(type, ExternalOffsets,
                       t->Concrete.VarDim.noffsets,
                       t->Concrete.VarDim.offsets,
                       nslices,
                       slices, ctx);
}

static ndt_t *
ndt_copy_function(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Function);

    u = ndt_function_new(t->Function.nargs, ctx);
    if (u == NULL) {
        return NULL;
    }
    u->Function.nin = t->Function.nin;
    u->Function.nout = t->Function.nout;

    copy_common(u, t);

    for (i = 0; i < t->Function.nargs; i++) {
        u->Function.types[i] = ndt_copy(t->Function.types[i], ctx);
        if (u->Function.types[i] == NULL) {
            ndt_del(u);
            return NULL;
        }
    }

    return u;
}

static ndt_t *
ndt_copy_tuple(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Tuple);

    u = ndt_tuple_new(t->Tuple.flag, t->Tuple.shape, ctx);
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

    u = ndt_record_new(t->Record.flag, t->Record.shape, ctx);
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

static int
ndt_copy_value(ndt_value_t *v, const ndt_value_t *u, ndt_context_t *ctx)
{
    *v = *u;

    switch (u->tag) {
    case ValString:
        v->ValString = ndt_strdup(u->ValString, ctx);
        if (v->ValString == NULL) {
            return -1;
        }
        return 0;
    case ValNA: case ValBool: case ValInt64: case ValFloat64:
        return 0;
    }

    /* NOT REACHED: tags should be exhaustive */
    ndt_err_format(ctx, NDT_RuntimeError, "ndt_copy_value: unexpected tag");
    return -1;
}

static ndt_t *
ndt_copy_categorical(const ndt_t *t, ndt_context_t *ctx)
{
    int64_t ntypes = t->Categorical.ntypes;
    ndt_value_t *types;
    int64_t i;

    assert(t->tag == Categorical);

    types = ndt_alloc(ntypes, sizeof(ndt_value_t));
    if (types == NULL) {
        return ndt_memory_error(ctx);
    }

    for (i = 0; i < ntypes; i++) {
        types[i].tag = ValNA;
    }

    for (i = 0; i < ntypes; i++) {
        if (ndt_copy_value(types+i, t->Categorical.types+i, ctx) < 0) {
            ndt_value_array_del(types, ntypes);
            return NULL;
        }
    }

    return ndt_categorical(types, ntypes, ctx);
}

ndt_t *
ndt_copy(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *u = NULL;
    ndt_t *type;

    switch (t->tag) {
    case FixedDim: {
        type = ndt_copy(t->FixedDim.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        u = ndt_fixed_dim_tag(type, t->FixedDim.tag, t->FixedDim.shape,
                              t->Concrete.FixedDim.step, ctx);
        goto copy_common_fields;
    }

    case VarDim: {
        u = ndt_copy_var_dim(t, ctx);
        goto copy_common_fields;
    }

    case SymbolicDim: {
        char *name;

        type = ndt_copy(t->SymbolicDim.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        name = ndt_strdup(t->SymbolicDim.name, ctx);
        if (name == NULL) {
            ndt_del(type);
            return NULL;
        }

        u = ndt_symbolic_dim_tag(name, type, t->SymbolicDim.tag, ctx);
        goto copy_common_fields;
    }

    case EllipsisDim: {
        char *name = NULL;

        type = ndt_copy(t->EllipsisDim.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        if (t->EllipsisDim.name != NULL) {
            name = ndt_strdup(t->SymbolicDim.name, ctx);
            if (name == NULL) {
                ndt_del(type);
                return NULL;
            }
        }

        u = ndt_ellipsis_dim_tag(name, type, t->EllipsisDim.tag, ctx);
        goto copy_common_fields;
    }

    case Tuple: {
        return ndt_copy_tuple(t, ctx);
    }

    case Record: {
        return ndt_copy_record(t, ctx);
    }

    case Ref: {
        type = ndt_copy(t->Ref.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        u = ndt_ref(type, ctx);
        goto copy_common_fields;
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

        u = ndt_constr(name, type, ctx);
        goto copy_common_fields;
    }

    case Nominal: {
        char *name = ndt_strdup(t->Nominal.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        type = ndt_copy(t->Nominal.type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        u = ndt_nominal(name, type, ctx);
        goto copy_common_fields;
    }

    case Categorical: {
        u = ndt_copy_categorical(t, ctx);
        goto copy_common_fields;
    }

    case Typevar: {
        char *name;

        name = ndt_strdup(t->Typevar.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = ndt_typevar(name, ctx);
        goto copy_common_fields;
    }

    case Function: {
        return ndt_copy_function(t, ctx);
    }

    case Module: {
        char *name;

        type = ndt_copy(t->Module.type, ctx);
        if (type == NULL) {
            return NULL;
        }

        name = ndt_strdup(t->Module.name, ctx);
        if (name == NULL) {
            ndt_del(type);
            return NULL;
        }

        u = ndt_module(name, type, ctx);
        goto copy_common_fields;
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
    }

    goto invalid_tag;


copy_common_fields:
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);
    return u;

invalid_tag:
    /* NOT REACHED: tags should be exhaustive */
    ndt_err_format(ctx, NDT_RuntimeError, "ndt_copy: unexpected tag");
    return NULL;
}

static ndt_t *
fixed_copy_contiguous(const ndt_t *t, ndt_t *type, ndt_context_t *ctx)
{
    if (t->ndim == 0) {
        return type;
    }

    assert(t->tag == FixedDim);
    assert(ndt_is_concrete(t));
    type = fixed_copy_contiguous(t->FixedDim.type, type, ctx);
    if (type == NULL) {
        ndt_del(type);
        return NULL;
    }

    return ndt_fixed_dim_tag(type, t->FixedDim.tag, t->FixedDim.shape,
                             INT64_MAX, ctx);
}

typedef struct {
    int maxdim;
    int32_t index[NDT_MAX_DIM+1];
    int32_t noffsets[NDT_MAX_DIM+1];
    int32_t *offsets[NDT_MAX_DIM+1];
} offsets_t;

static void
clear_offsets(offsets_t *m)
{
    for (int i = 0; i < NDT_MAX_DIM+1; i++) {
        ndt_free(m->offsets[i]);
    }
}

static int
var_init_offsets(offsets_t *m, const ndt_t *t, int32_t noffsets, ndt_context_t *ctx)
{
    int32_t *offsets;
    int64_t shape, start, step;
    int64_t sum;
    int32_t i;

    assert(t->ndim >= 1);

    offsets = ndt_alloc(noffsets, sizeof *offsets);
    if (offsets == NULL) {
        clear_offsets(m);
        (void)ndt_memory_error(ctx);
        return -1;
    }
    m->noffsets[t->ndim] = noffsets;
    m->offsets[t->ndim] = offsets;

    if (t->ndim == 1) {
        return 0;
    }

    for (i=0, sum=0; i < noffsets-1; i++) {
        shape = ndt_var_indices(&start, &step, t, i, ctx);
        if (shape == -1) {
            clear_offsets(m);
            return -1;
        }
        sum += shape;
    }

    return var_init_offsets(m, t->VarDim.type, (int32_t)sum+1, ctx);
}

static int
var_copy_shapes(offsets_t *m, int64_t src_index, const ndt_t *t, ndt_context_t *ctx)
{
    int64_t shape, start, step;

    if (t->ndim == 0) {
        return 0;
    }

    shape = ndt_var_indices(&start, &step, t, src_index, ctx);
    if (shape < 0) {
        clear_offsets(m);
        return -1;
    }
    int32_t dst_index = m->index[t->ndim]++;
    m->offsets[t->ndim][dst_index] = (int32_t)shape;

    for (int64_t i = 0; i < shape; i++) {
        int64_t src_next = start + i * step;
        if (var_copy_shapes(m, src_next, t->VarDim.type, ctx) < 0) {
            clear_offsets(m);
            return -1;
        }
    }

    return 0;
}

static void
var_sum_shapes(offsets_t *m)
{
    int i, k;

    for (i = 1; i <= m->maxdim; i++) {
        int32_t sum = 0;
        for (k = 0; k < m->noffsets[i]; k++) {
            int32_t s = m->offsets[i][k];
            m->offsets[i][k] = sum;
            sum += s;
        }
   }
}

ndt_t *
var_from_offsets_and_dtype(offsets_t *m, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    int i;

    for (i=1, t=type; i <= m->maxdim; i++, type=t) {
        t = ndt_var_dim(type, InternalOffsets, m->noffsets[i], m->offsets[i],
                        0, NULL, ctx);
        m->offsets[i] = NULL;
        if (t == NULL) {
            clear_offsets(m);
            return NULL;
        }
    }

    return t;
}
 
static ndt_t *
var_copy_contiguous(const ndt_t *t, ndt_t *dtype, ndt_context_t *ctx)
{
    offsets_t m = {.maxdim=0, .index={0}, .noffsets={0}, .offsets={NULL}};

    assert(t->tag == VarDim);
    assert(ndt_is_concrete(t));
    assert(t->Concrete.VarDim.noffsets == 2);

    if (var_init_offsets(&m, t, 2, ctx) < 0) {
        ndt_del(dtype);
        return NULL;
    }
    m.maxdim = t->ndim;

    if (var_copy_shapes(&m, 0, t, ctx) < 0) {
        ndt_del(dtype);
        return NULL;
    }

    var_sum_shapes(&m);

    return var_from_offsets_and_dtype(&m, dtype, ctx);
}

ndt_t *
ndt_copy_contiguous_dtype(const ndt_t *t, ndt_t *dtype, ndt_context_t *ctx)
{
    if (ndt_is_abstract(t) || ndt_is_abstract(dtype)) {
        ndt_err_format(ctx, NDT_ValueError,
            "copy_new_dtype() called on abstract type");
        return NULL;
    }

    switch (t->tag) {
    case FixedDim: {
        return fixed_copy_contiguous(t, dtype, ctx);
    }
    case VarDim: {
        return var_copy_contiguous(t, dtype, ctx);
    }
    default:
        return dtype;
    }
}

ndt_t *
ndt_copy_contiguous(const ndt_t *t, ndt_context_t *ctx)
{
    ndt_t *dtype = ndt_copy(ndt_dtype(t), ctx);
    if (dtype == NULL) {
        return NULL;
    }

    return ndt_copy_contiguous_dtype(t, dtype, ctx);
}

ndt_t *
ndt_copy_abstract_var_dtype(const ndt_t *t, ndt_t *dtype, ndt_context_t *ctx)
{
    if (t->ndim == 0) {
        return dtype;
    }

    switch (t->tag) {
    case VarDim: {
        if (!ndt_is_abstract(t)) {
            ndt_err_format(ctx, NDT_ValueError,
                "ndt_copy_abstract_var_dtype() called on concrete type");
            return NULL;
        }
        ndt_t *u = ndt_copy_abstract_var_dtype(t->VarDim.type, dtype, ctx);
        if (u == NULL) {
            return NULL;
        }

        return ndt_abstract_var_dim(u, ctx);
    }
    default:
        ndt_err_format(ctx, NDT_ValueError,
            "ndt_copy_abstract_var_dtype(): not a var dimension");
        return NULL;
    }
}
