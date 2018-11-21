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

static const ndt_t *
ndt_copy_var_dim(const ndt_t *t, bool opt, ndt_context_t *ctx)
{
    ndt_slice_t *slices;
    int nslices;

    assert(t->tag == VarDim);

    if (ndt_is_abstract(t))  {
        return ndt_abstract_var_dim(t->VarDim.type, opt, ctx);
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

    return ndt_var_dim(t->VarDim.type, t->Concrete.VarDim.offsets,
                       nslices, slices,
                       opt, ctx);
}

static const ndt_t *
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
        ndt_incref(t->Function.types[i]);
        u->Function.types[i] = t->Function.types[i];
    }

    return u;
}

static const ndt_t *
ndt_copy_tuple(const ndt_t *t, bool opt, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Tuple);

    u = ndt_tuple_new(t->Tuple.flag, t->Tuple.shape, opt, ctx);
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);

    for (i = 0; i < t->Tuple.shape; i++) {
        ndt_incref(t->Tuple.types[i]);
        u->Tuple.types[i] = t->Tuple.types[i];

        u->Concrete.Tuple.offset[i] = t->Concrete.Tuple.offset[i];
        u->Concrete.Tuple.align[i] = t->Concrete.Tuple.align[i];
        u->Concrete.Tuple.pad[i] = t->Concrete.Tuple.pad[i];
    }

    return u;
}

static const ndt_t *
ndt_copy_record(const ndt_t *t, bool opt, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Record);

    u = ndt_record_new(t->Record.flag, t->Record.shape, opt, ctx);
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);

    for (i = 0; i < t->Record.shape; i++) {
        u->Record.names[i] = ndt_strdup(t->Record.names[i], ctx);
        if (u->Record.names[i] == NULL) {
            ndt_decref(u);
            return NULL;
        }

        ndt_incref(t->Record.types[i]);
        u->Record.types[i] = t->Record.types[i];

        u->Concrete.Record.offset[i] = t->Concrete.Record.offset[i];
        u->Concrete.Record.align[i] = t->Concrete.Record.align[i];
        u->Concrete.Record.pad[i] = t->Concrete.Record.pad[i];
    }

    return u;
}

static const ndt_t *
ndt_copy_union(const ndt_t *t, bool opt, ndt_context_t *ctx)
{
    ndt_t *u;
    int64_t i;

    assert(t->tag == Union);

    u = ndt_union_new(t->Union.ntypes, opt, ctx);
    if (u == NULL) {
        return NULL;
    }

    copy_common(u, t);

    for (i = 0; i < t->Union.ntypes; i++) {
        ndt_incref(t->Union.types[i]);
        u->Union.types[i] = t->Union.types[i];
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

static const ndt_t *
ndt_copy_categorical(const ndt_t *t, bool opt, ndt_context_t *ctx)
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

    return ndt_categorical(types, ntypes, opt, ctx);
}

/* shallow copy */
const ndt_t *
ndt_copy(const ndt_t *t, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t);
    ndt_t *u = NULL;

    switch (t->tag) {
    case FixedDim: {
        u = (ndt_t *)ndt_fixed_dim_tag(t->FixedDim.type, t->FixedDim.tag, t->FixedDim.shape,
                                       t->Concrete.FixedDim.step, ctx);
        goto copy_common_fields;
    }

    case VarDim: {
        u = (ndt_t *)ndt_copy_var_dim(t, opt, ctx);
        goto copy_common_fields;
    }

    case SymbolicDim: {
        char *name;

        name = ndt_strdup(t->SymbolicDim.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = (ndt_t *)ndt_symbolic_dim_tag(name, t->SymbolicDim.type, t->SymbolicDim.tag, ctx);
        goto copy_common_fields;
    }

    case EllipsisDim: {
        char *name = NULL;

        if (t->EllipsisDim.name != NULL) {
            name = ndt_strdup(t->SymbolicDim.name, ctx);
            if (name == NULL) {
                return NULL;
            }
        }

        u = (ndt_t *)ndt_ellipsis_dim_tag(name, t->EllipsisDim.type, t->EllipsisDim.tag, ctx);
        goto copy_common_fields;
    }

    case Tuple: {
        return ndt_copy_tuple(t, opt, ctx);
    }

    case Record: {
        return ndt_copy_record(t, opt, ctx);
    }

    case Ref: {
        u = (ndt_t *)ndt_ref(t->Ref.type, opt, ctx);
        goto copy_common_fields;
    }

    case Constr: {
        char *name = ndt_strdup(t->Constr.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = (ndt_t *)ndt_constr(name, t->Constr.type, opt, ctx);
        goto copy_common_fields;
    }

    case Nominal: {
        char *name = ndt_strdup(t->Nominal.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = (ndt_t *)ndt_nominal(name, t->Nominal.type, opt, ctx);
        goto copy_common_fields;
    }

    case Categorical: {
        u = (ndt_t *)ndt_copy_categorical(t, opt, ctx);
        goto copy_common_fields;
    }

    case Typevar: {
        char *name;

        name = ndt_strdup(t->Typevar.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = (ndt_t *)ndt_typevar(name, ctx);
        goto copy_common_fields;
    }

    case Union: {
        return ndt_copy_union(t, opt, ctx);
    }

    case Function: {
        return ndt_copy_function(t, ctx);
    }

    case Module: {
        char *name;

        name = ndt_strdup(t->Module.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        u = (ndt_t *)ndt_module(name, t->Module.type, ctx);
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
        u = ndt_new(t->tag, opt, ctx);
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

static const ndt_t *
fixed_copy_contiguous(const ndt_t *t, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *u, *v;

    if (t->ndim == 0) {
        ndt_incref(type);
        return type;
    }

    assert(t->tag == FixedDim);
    assert(ndt_is_concrete(t));

    u = fixed_copy_contiguous(t->FixedDim.type, type, ctx);
    if (u == NULL) {
        return NULL;
    }

    v = ndt_fixed_dim_tag(u, t->FixedDim.tag, t->FixedDim.shape,
                          INT64_MAX, ctx);
    ndt_decref(u);
    return v;
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

const ndt_t *
var_from_offsets_and_dtype(offsets_t *m, const ndt_t *t, ndt_context_t *ctx)
{
    const ndt_t *u;
    int i;

    ndt_incref(t);

    for (i = 1; i <= m->maxdim; i++) {
        ndt_offsets_t *offsets = ndt_offsets_from_ptr(m->offsets[i], m->noffsets[i], ctx);

        m->offsets[i] = NULL;
        if (offsets == NULL) {
            ndt_decref(t);
            clear_offsets(m);
            return NULL;
        }

        u = ndt_var_dim(t, offsets, 0, NULL, false, ctx);
        ndt_move(&t, u);
        ndt_decref_offsets(offsets);

        if (t == NULL) {
            clear_offsets(m);
            return NULL;
        }
    }

    return t;
}
 
static const ndt_t *
var_copy_contiguous(const ndt_t *t, const ndt_t *dtype, ndt_context_t *ctx)
{
    offsets_t m = {.maxdim=0, .index={0}, .noffsets={0}, .offsets={NULL}};

    assert(t->tag == VarDim);
    assert(ndt_is_concrete(t));
    assert(t->Concrete.VarDim.offsets->n == 2);

    if (var_init_offsets(&m, t, 2, ctx) < 0) {
        return NULL;
    }
    m.maxdim = t->ndim;

    if (var_copy_shapes(&m, 0, t, ctx) < 0) {
        return NULL;
    }

    var_sum_shapes(&m);

    return var_from_offsets_and_dtype(&m, dtype, ctx);
}

const ndt_t *
ndt_copy_contiguous_dtype(const ndt_t *t, const ndt_t *dtype, ndt_context_t *ctx)
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
        ndt_incref(dtype);
        return dtype;
    }
}

const ndt_t *
ndt_copy_contiguous(const ndt_t *t, ndt_context_t *ctx)
{
    const ndt_t *dtype = ndt_dtype(t);

    return ndt_copy_contiguous_dtype(t, dtype, ctx);
}

const ndt_t *
ndt_copy_abstract_var_dtype(const ndt_t *t, const ndt_t *dtype, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t);

    if (t->ndim == 0) {
        ndt_incref(dtype);
        return dtype;
    }

    switch (t->tag) {
    case VarDim: {
        if (!ndt_is_abstract(t)) {
            ndt_err_format(ctx, NDT_ValueError,
                "ndt_copy_abstract_var_dtype() called on concrete type");
            return NULL;
        }
        const ndt_t *u = ndt_copy_abstract_var_dtype(t->VarDim.type, dtype, ctx);
        if (u == NULL) {
            return NULL;
        }

        const ndt_t *w = ndt_abstract_var_dim(u, opt, ctx);
        ndt_decref(u);
        return w;
    }
    default:
        ndt_err_format(ctx, NDT_ValueError,
            "ndt_copy_abstract_var_dtype(): not a var dimension");
        return NULL;
    }
}
