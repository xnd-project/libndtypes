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


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>
#include "ndtypes.h"
#include "overflow.h"
#include "slice.h"


/*****************************************************************************/
/*                           Static helper functions                         */
/*****************************************************************************/

#undef max
static inline uint16_t
max(uint16_t x, uint16_t y)
{
    return x >= y ? x : y;
}

static inline int
ispower2(uint16_t n)
{
    return n != 0 && (n & (n-1)) == 0;
}

static int64_t
round_up(int64_t offset, uint16_t align, bool *overflow)
{
    int64_t size;

    assert(align > 0);
    size = ADDi64(offset, align-1, overflow);
    return (size / align) * align;
}


/*****************************************************************************/
/*                                   Flags                                   */
/*****************************************************************************/

/* Determine general subtree flags. */
static uint32_t
ndt_subtree_flags(const ndt_t *type)
{
    if (type && type->flags & (NDT_OPTION|NDT_SUBTREE_OPTION)) {
        return NDT_SUBTREE_OPTION;
    }

    return 0;
}

/* Determine general subtree and ellipsis flags. */
static uint32_t
ndt_dim_flags(const ndt_t *type)
{
    uint32_t flags = ndt_subtree_flags(type);
    flags |= (type->flags & NDT_ELLIPSIS);
    return flags;
}


/*****************************************************************************/
/*                      Common fields for concrete types                     */
/*****************************************************************************/

/* Itemsize of a concrete type.  Undefined for abstract types. */
int64_t
ndt_itemsize(const ndt_t *t)
{
    assert(ndt_is_concrete(t));

    switch (t->tag) {
    case FixedDim:
        return t->Concrete.FixedDim.itemsize;
    case VarDim:
        return t->Concrete.VarDim.itemsize;
    default:
        return t->datasize;
    }
}


/*****************************************************************************/
/*                                Predicates                                 */
/*****************************************************************************/

/* Type field access */
int
ndt_is_abstract(const ndt_t *t)
{
    return t->access == Abstract;
}

int
ndt_is_concrete(const ndt_t *t)
{
    return t->access == Concrete;
}

/* Type flags */
int
ndt_is_optional(const ndt_t *t)
{
    return t->flags & NDT_OPTION;
}

int
ndt_subtree_is_optional(const ndt_t *t)
{
    return t->flags & NDT_SUBTREE_OPTION;
}

/* Array predicates */
int
ndt_is_ndarray(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim:
        return 1;
    default:
        return t->ndim == 0;
    }
}

int
ndt_is_c_contiguous(const ndt_t *t)
{
    const ndt_t *dims[NDT_MAX_DIM];
    const ndt_t *dtype;
    int64_t shape, step;
    int ndim, i;

    if (ndt_is_abstract(t)) {
        return 0;
    }
    if (!ndt_is_ndarray(t)) {
        return 0;
    }
    if (t->ndim == 0) {
        return 1;
    }

    ndim = ndt_dims_dtype(dims, &dtype, t);

    step = 1;
    for (i = ndim-1; i >= 0; i--) {
        shape = dims[i]->FixedDim.shape;
        if (shape > 1 && dims[i]->Concrete.FixedDim.step != step) {
            return 0;
        }
        step *= shape;
    }

    return 1;
}

int
ndt_is_f_contiguous(const ndt_t *t)
{
    const ndt_t *dims[NDT_MAX_DIM];
    const ndt_t *dtype;
    int64_t shape, step;
    int ndim, i;

    if (ndt_is_abstract(t)) {
        return 0;
    }
    if (!ndt_is_ndarray(t)) {
        return 0;
    }
    if (t->ndim == 0) {
        return 1;
    }

    ndim = ndt_dims_dtype(dims, &dtype, t);
    step = 1;
    for (i = 0; i < ndim; i++) {
        shape = dims[i]->FixedDim.shape;
        if (shape > 1 && dims[i]->Concrete.FixedDim.step != step) {
            return 0;
        }
        step *= shape;
    }

    return 1;
}

int
ndt_really_fortran(const ndt_t *t)
{
    return ndt_is_f_contiguous(t) && !ndt_is_c_contiguous(t);
}

/* Scalar */
int
ndt_is_scalar(const ndt_t *t)
{
    switch (t->tag) {
    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case FixedString: case FixedBytes:
    case String: case Bytes:
    case Char:
        return 1;
    default:
        return 0;
    }
}

/* Primitive type predicates */
int
ndt_is_signed(const ndt_t *t)
{
    switch (t->tag) {
    case Int8: case Int16: case Int32: case Int64:
        return 1;
    default:
        return 0;
    }
}

int
ndt_is_unsigned(const ndt_t *t)
{
    switch (t->tag) {
    case Uint8: case Uint16: case Uint32: case Uint64:
        return 1;
    default:
        return 0;
    }
}

int
ndt_is_float(const ndt_t *t)
{
    switch (t->tag) {
    case Float16: case Float32: case Float64:
        return 1;
    default:
        return 0;
    }
}

int
ndt_is_complex(const ndt_t *t)
{
    switch (t->tag) {
    case Complex32: case Complex64: case Complex128:
        return 1;
    default:
        return 0;
    }
}

int
ndt_endian_is_set(const ndt_t *t)
{
    return t->flags & (NDT_LITTLE_ENDIAN|NDT_BIG_ENDIAN);
}

int
ndt_is_little_endian(const ndt_t *t)
{
    switch (t->tag) {
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
        if (t->flags & NDT_LITTLE_ENDIAN) {
            return 1;
        }
        if (t->flags & NDT_BIG_ENDIAN) {
            return 0;
        }
        return !NDT_SYS_BIG_ENDIAN;
    default:
        return 0;
    }
}

int
ndt_is_big_endian(const ndt_t *t)
{
    switch (t->tag) {
    case Int16: case Int32: case Int64:
    case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
        if (t->flags & NDT_BIG_ENDIAN) {
            return 1;
        }
        if (t->flags & NDT_LITTLE_ENDIAN) {
            return 0;
        }
        return NDT_SYS_BIG_ENDIAN;
    default:
        return 0;
    }
}


/*****************************************************************************/
/*                           Alignment and packing                           */
/*****************************************************************************/

/* Check that 'align' is a power of two. */
static inline bool
align_ispower2(uint16_t align, ndt_context_t *ctx)
{
    if (!ispower2(align)) {
        ndt_err_format(ctx, NDT_ValueError,
            "'align' must be a power of two, got %" PRIu16, align);
        return false;
    }

    return true;
}

/*
 * Check that at most one of 'align' and 'pack' is user-specified. If
 * 'align' is specified, choose the larger value of 'align' and the
 * natural alignment of the field member.
 *
 * If 'pack' is specified, use that value, regardless of whether it
 * is smaller than the natural alignment of the field member.
 *
 * If type access is restricted and explicit values have been specified,
 * raise an error.
 *
 * Return value: The extracted alignment if the type is concrete and no
 * error occurred.  1 if the type is abstract and no error occurred (that
 * value is unused later).
 *
 * Return UINT16_MAX if an error occurred.
 */
static uint16_t
min_field_align(const ndt_t *t, uint16_opt_t align, uint16_opt_t pack,
                ndt_context_t *ctx)
{
    uint16_t min_align = 1;

    if (align.tag == Some) {
        if (pack.tag == Some) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                           "field has both 'align' and 'pack' attributes");
            return UINT16_MAX;
        }
        if (t->access == Abstract) {
            goto access_error;
        }
        min_align = max(align.Some, t->align);
    }
    else if (pack.tag == Some) {
        if (t->access == Abstract) {
            goto access_error;
        }
        min_align = pack.Some;
    }
    else {
        if (t->access == Concrete) {
            min_align = t->align;
        }
    }

    if (!align_ispower2(min_align, ctx)) {
        return UINT16_MAX;
    }

    return min_align;


access_error:
    ndt_err_format(ctx, NDT_InvalidArgumentError,
        "'align' or 'pack' attribute given for abstract type");
    return UINT16_MAX;
}

/* Extract and validate the alignment value. */
static inline uint16_t
get_align(uint16_opt_t align, uint16_t default_align, ndt_context_t *ctx)
{
    switch (align.tag) {
    case Some:
        if (!align_ispower2(align.Some, ctx)) {
            return UINT16_MAX;
        }
        return align.Some;
    default: /* None */
        return default_align;
    }
}


/******************************************************************************/
/*                                     Fields                                 */
/******************************************************************************/

/*
 * align = n: minimum alignment for the field; the resulting alignment is
 * guaranteed to be at least the maximum of n and the natural alignment of
 * the field member.
 *
 * pack = n: minimum alignment for the field; the resulting alignment is
 * guaranteed to be at least n.
 *
 * pad = n: requested padding for a field; used for checking if an explicitly
 * given padding is equal to the computed padding.
 *
 * 'name' is NULL for a tuple field.
 */
ndt_field_t *
ndt_field(char *name, ndt_t *type, uint16_opt_t align, uint16_opt_t pack,
          uint16_opt_t pad, ndt_context_t *ctx)
{
    ndt_field_t *field;
    uint16_t min_align;

    if (type == NULL) {
        return NULL;
    }

    min_align = min_field_align(type, align, pack, ctx);
    if (min_align == UINT16_MAX) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract field */
    field = ndt_alloc_size(sizeof *field);
    if (field == NULL) {
        ndt_free(name);
        ndt_del(type);
        return ndt_memory_error(ctx);
    }
    field->name = name;
    field->type = type;

    /* concrete access */
    field->access = type->access;
    if (field->access == Concrete) {
        field->Concrete.align = min_align;
        field->Concrete.explicit_align = (align.tag==Some || pack.tag==Some);
        field->Concrete.pad = (pad.tag==Some) ? pad.Some : UINT16_MAX;
        field->Concrete.explicit_pad = (pad.tag==Some);
    }

    return field;
}

void
ndt_field_del(ndt_field_t *field)
{
    if (field) {
        ndt_free(field->name);
        ndt_del(field->type);
        ndt_free(field);
    }
}

void
ndt_field_array_del(ndt_field_t *fields, int64_t shape)
{
    int64_t i;

    if (fields == NULL) {
        return;
    }

    for (i = 0; i < shape; i++) {
        ndt_free(fields[i].name);
        ndt_del(fields[i].type);
    }

    ndt_free(fields);
}

void
ndt_type_array_clear(ndt_t **types, int64_t shape)
{
    int64_t i;

    if (types == NULL) {
        return;
    }

    for (i = 0; i < shape; i++) {
        ndt_del(types[i]);
    }
}

void
ndt_type_array_del(ndt_t **types, int64_t shape)
{
    ndt_type_array_clear(types, shape);
    ndt_free(types);
}


/*****************************************************************************/
/*                                Typedef                                    */
/*****************************************************************************/

/*
 * Add a typedef name -> type to the symbol table.  Steals the 'type'
 * reference.
 */
int
ndt_typedef(const char *name, ndt_t *type, const ndt_methods_t *m, ndt_context_t *ctx)
{
    if (ndt_typedef_add(name, type, m, ctx) < 0) {
        return -1;
    }

    return 0;
}

int
ndt_typedef_from_string(const char *name, const char *type, const ndt_methods_t *m,
                        ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_from_string(type, ctx);
    if (t == NULL) {
        return -1;
    }

    if (ndt_typedef_add(name, t, m, ctx) < 0) {
        return -1;
    }

    return 0;
}


/******************************************************************************/
/*                              Type invariants                               */
/******************************************************************************/

/* Invariants for all types except for var and ellipsis dimensions. */
static int
check_type_invariants(const ndt_t *type, ndt_context_t *ctx)
{
    if (type->tag == Module) {
        ndt_err_format(ctx, NDT_TypeError,
            "nested module types are not supported");
        return 0;
    }

    if (type->tag == VarDim) {
        ndt_err_format(ctx, NDT_TypeError,
            "nested or non-uniform var dimensions are not supported");
        return 0;
    }

    if (type->ndim >= NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_TypeError, "ndim > %d", NDT_MAX_DIM);
        return 0;
    }

    return 1;
}

/* Invariants for abstract var dimensions. */
static int
check_abstract_var_invariants(const ndt_t *type, ndt_context_t *ctx)
{
    if (type->tag == Module) {
        ndt_err_format(ctx, NDT_TypeError,
            "nested module types are not supported");
        return 0;
    }

    if (type->tag == FixedDim || type->tag == SymbolicDim) {
        ndt_err_format(ctx, NDT_TypeError,
            "mixed fixed and var dim are not supported");
        return 0;
    }

    if (type->tag == VarDim && ndt_is_concrete(type)) {
        ndt_err_format(ctx, NDT_TypeError,
            "mixing abstract and concrete var dimensions is not allowed");
        return 0;
    }

    if (type->ndim >= NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_TypeError, "ndim > %d", NDT_MAX_DIM);
        return 0;
    }

    return 1;
}

/* Invariants for concrete var dimensions. */
static int
check_var_invariants(enum ndt_offsets flag, const ndt_t *type, ndt_context_t *ctx)
{
    if (type->tag == Module) {
        ndt_err_format(ctx, NDT_TypeError,
            "nested module types are not supported");
        return 0;
    }

    if (type->tag == FixedDim || type->tag == SymbolicDim) {
        ndt_err_format(ctx, NDT_TypeError,
            "mixed fixed and var dim are not supported");
        return 0;
    }

    if (type->tag == VarDim) {
        if (ndt_is_abstract(type)) {
            ndt_err_format(ctx, NDT_TypeError,
                "mixing abstract and concrete var dimensions is not allowed");
            return 0;
        }
        if (flag != type->Concrete.VarDim.flag) {
            ndt_err_format(ctx, NDT_TypeError,
                "mixing internal and external offsets is not allowed");
            return 0;
        }
    }

    if (type->ndim >= NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_TypeError, "ndim > %d", NDT_MAX_DIM);
        return 0;
    }

    return 1;
}

/* Invariants for ellipsis dimensions. */
static int
check_ellipsis_invariants(const ndt_t *type, ndt_context_t *ctx)
{
    if (type->tag == Module) {
        ndt_err_format(ctx, NDT_TypeError,
            "nested module types are not supported");
        return 0;
    }

    if (type->ndim >= NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_TypeError, "ndim > %d", NDT_MAX_DIM);
        return 0;
    }

    return 1;
}

static int
strcmp_null(const char *s, const char *t)
{
    if (s == NULL) {
        return t == NULL;
    }
    return t != NULL && strcmp(s, t) == 0;
}

static int
check_function_invariants(ndt_t * const *types, int64_t nin, int64_t nargs,
                          ndt_context_t *ctx)
{
    int64_t count = 0;
    int i;

    if (nargs == 0) {
        return 1;
    }

    if (nin == 0) {
        for (i = 0; i < nargs; i++) {
            if (ndt_is_abstract(types[i])) {
                ndt_err_format(ctx, NDT_ValueError,
                    "output types cannot be inferred for function with "
                    "no arguments");
                return 0;
            }
        }
    }

    for (i = 0; i < nargs; i++) {
        if (types[i]->tag == EllipsisDim) {
            count++;
        }
    }

    if (count == 0) {
        return 1;
    }
    if (count != nargs) {
        goto error;
    }

    for (i = 1; i < nargs; i++) {
        if (!strcmp_null(types[0]->EllipsisDim.name, types[i]->EllipsisDim.name)) {
            goto error;
        }
    }

    return 1;

error:
    ndt_err_format(ctx, NDT_ValueError,
        "invalid combination of ellipsis dimensions");
    return 0;
}

static bool
is_elemwise(ndt_t * const *types, int64_t nargs)
{
    for (int64_t i = 0; i < nargs; i++) {
        if ((types[i]->ndim == 1 && types[i]->tag == EllipsisDim) ||
            types[i]->ndim == 0) {
            continue;
        }
        return false;
    }

    return true;
}


/******************************************************************************/
/*                         Type allocation/deallocation                       */
/******************************************************************************/

ndt_t *
ndt_new(enum ndt tag, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_alloc_size(sizeof *t);
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->tag = tag;
    t->access = Abstract;
    t->flags = 0;
    t->ndim = 0;

    t->datasize = 0;
    t->align = UINT16_MAX;

    return t;
}

static ndt_t *
ndt_new_extra(enum ndt tag, int64_t n, ndt_context_t *ctx)
{
    bool overflow = 0;
    ndt_t *t;
    int64_t size;

    size = ADDi64(offsetof(ndt_t, extra), n, &overflow);
    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "type too large");
        return NULL;
    }

    t = ndt_alloc(1, size);
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->tag = tag;
    t->access = Abstract;
    t->flags = 0;
    t->ndim = 0;

    t->datasize = 0;
    t->align = UINT16_MAX;

    return t;
}

ndt_t *
ndt_function_new(int64_t nargs, ndt_context_t *ctx)
{
    ndt_t *t = NULL;
    bool overflow = 0;
    int64_t extra, i;

    extra = MULi64(nargs, sizeof(ndt_t *), &overflow);

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "function size too large");
        return NULL;
    }

    t = ndt_new_extra(Function, extra, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Function.nargs = nargs;
    t->Function.types = (ndt_t **)t->extra;

    for (i = 0; i < nargs; i++) {
        t->Function.types[i] = NULL;
    }

    return t;
}

ndt_t *
ndt_tuple_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx)
{
    ndt_t *t = NULL;
    bool overflow = 0;
    int64_t offset_offset;
    int64_t align_offset;
    int64_t pad_offset;
    int64_t extra;
    int64_t size;
    int64_t i;

    size = MULi64(shape, sizeof(ndt_t *), &overflow);
    offset_offset = round_up(size, alignof(int64_t), &overflow);

    size = MULi64(shape, sizeof(int64_t), &overflow);
    align_offset = ADDi64(offset_offset, size, &overflow);

    size = MULi64(shape, sizeof(uint16_t), &overflow);
    pad_offset = ADDi64(align_offset, size, &overflow);

    extra = ADDi64(pad_offset, size, &overflow);

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "tuple size too large");
        return NULL;
    }

    t = ndt_new_extra(Tuple, extra, ctx);
    if (t == NULL) {
        return NULL;
    }

    t->Tuple.flag = flag;
    t->Tuple.shape = shape;
    t->Tuple.types = (ndt_t **)t->extra;
    t->Concrete.Tuple.offset = (int64_t *)(t->extra + offset_offset);
    t->Concrete.Tuple.align = (uint16_t *)(t->extra + align_offset);
    t->Concrete.Tuple.pad = (uint16_t *)(t->extra + pad_offset);

    for (i = 0; i < shape; i++) {
        t->Tuple.types[i] = NULL;
        t->Concrete.Tuple.offset[i] = 0;
        t->Concrete.Tuple.align[i] = 1;
        t->Concrete.Tuple.pad[i] = 0;
    }

    return t;
}

ndt_t *
ndt_record_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx)
{
    ndt_t *t = NULL;
    bool overflow = 0;
    int64_t types_offset;
    int64_t offset_offset;
    int64_t align_offset;
    int64_t pad_offset;
    int64_t extra;
    int64_t size;
    int64_t i;

    size = types_offset = MULi64(shape, sizeof(char *), &overflow);

    offset_offset = ADDi64(types_offset, size, &overflow);
    offset_offset = round_up(offset_offset, alignof(int64_t), &overflow);

    size = MULi64(shape, sizeof(int64_t), &overflow);
    align_offset = ADDi64(offset_offset, size, &overflow);

    size = MULi64(shape, sizeof(uint16_t), &overflow);
    pad_offset = ADDi64(align_offset, size, &overflow);

    extra = ADDi64(pad_offset, size, &overflow);

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "record size too large");
        return NULL;
    }

    t = ndt_new_extra(Record, extra, ctx);
    if (t == NULL) {
        return NULL;
    }

    t->Record.flag = flag;
    t->Record.shape = shape;
    t->Record.names = (char **)t->extra;
    t->Record.types = (ndt_t **)(t->extra + types_offset);
    t->Concrete.Record.offset = (int64_t *)(t->extra + offset_offset);
    t->Concrete.Record.align = (uint16_t *)(t->extra + align_offset);
    t->Concrete.Record.pad = (uint16_t *)(t->extra + pad_offset);

    for (i = 0; i < shape; i++) {
        t->Record.names[i] = NULL;
        t->Record.types[i] = NULL;
        t->Concrete.Record.offset[i] = 0;
        t->Concrete.Record.align[i] = 1;
        t->Concrete.Record.pad[i] = 0;
    }

    return t;
}

void
ndt_del(ndt_t *t)
{
    if (t == NULL) {
        return;
    }

    switch (t->tag) {
    case Module: {
        ndt_free(t->Module.name);
        ndt_del(t->Module.type);
        goto free_type;
    }

    case Function: {
        int64_t i;
        for (i = 0; i < t->Function.nargs; i++) {
            ndt_del(t->Function.types[i]);
        }
        goto free_type;
    }

    case FixedDim: {
        ndt_del(t->FixedDim.type);
        goto free_type;
    }

    case VarDim: {
        ndt_del(t->VarDim.type);
        if (ndt_is_concrete(t)) {
            if (t->Concrete.VarDim.flag == InternalOffsets) {
                ndt_free((int32_t *)t->Concrete.VarDim.offsets);
            }
            ndt_free(t->Concrete.VarDim.slices);
        }
        goto free_type;
    }

    case SymbolicDim: {
        ndt_free(t->SymbolicDim.name);
        ndt_del(t->SymbolicDim.type);
        goto free_type;
    }

    case EllipsisDim: {
        ndt_free(t->EllipsisDim.name);
        ndt_del(t->EllipsisDim.type);
        goto free_type;
    }

    case Tuple: {
        int64_t i;
        for (i = 0; i < t->Tuple.shape; i++) {
            ndt_del(t->Tuple.types[i]);
        }
        goto free_type;
    }

    case Record: {
        int64_t i;
        for (i = 0; i < t->Record.shape; i++) {
            ndt_free(t->Record.names[i]);
            ndt_del(t->Record.types[i]);
        }
        goto free_type;
    }

    case Ref: {
        ndt_del(t->Ref.type);
        goto free_type;
    }

    case Constr: {
        ndt_free(t->Constr.name);
        ndt_del(t->Constr.type);
        goto free_type;
    }

    case Nominal: {
        ndt_free(t->Nominal.name);
        ndt_del(t->Nominal.type);
        goto free_type;
    }

    case Categorical: {
        ndt_value_array_del(t->Categorical.types, t->Categorical.ntypes);
        goto free_type;
    }

    case Typevar: {
        ndt_free(t->Typevar.name);
        goto free_type;
    }

    case AnyKind: case ScalarKind:
    case FixedStringKind: case FixedString:
    case FixedBytesKind: case FixedBytes:
    case String: case Bytes: case Char:
    case Bool:
    case SignedKind:
    case Int8: case Int16: case Int32: case Int64:
    case UnsignedKind:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case FloatKind:
    case Float16: case Float32: case Float64:
    case ComplexKind:
    case Complex32: case Complex64: case Complex128:
        goto free_type;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid tag");


free_type:
    ndt_free(t);
}


/******************************************************************************/
/*                               Type functions                               */
/******************************************************************************/

/* Set the option flag of a type */
ndt_t *
ndt_option(ndt_t *t)
{
    t->flags |= NDT_OPTION;
    return t;
}

/* Abstract namespace type */
ndt_t *
ndt_module(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Module, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t->Module.name = name;
    t->Module.type = type;
    t->flags = ndt_subtree_flags(type);

    return t;
}

/* Abstract function signatures */
ndt_t *
ndt_function(ndt_t * const *types, int64_t nargs, int64_t nin, int64_t nout,
             ndt_context_t *ctx)
{
    ndt_t *t;
    int64_t i;

    assert(0 <= nin && 0 <= nout && nargs == nin+nout);

    if (!check_function_invariants(types, nin, nargs, ctx)) {
        ndt_type_array_clear((ndt_t **)types, nargs);
        return NULL;
    }

    /* abstract type */
    t = ndt_function_new(nargs, ctx);
    if (t == NULL) {
        for (i = 0; i < nargs; i++) {
            ndt_del(types[i]);
        }
        return NULL;
    }
    t->Function.elemwise = is_elemwise(types, nargs);
    t->Function.nin = nin;
    t->Function.nout = nout;

    for (i = 0; i < nargs; i++) {
        t->Function.types[i] = types[i];
        t->flags |= ndt_dim_flags(types[i]);
    }

    return t;
}

ndt_t *
ndt_any_kind(ndt_context_t *ctx)
{
    return ndt_new(AnyKind, ctx);
}


/******************************************************************************/
/*                             Dimension types                                */
/******************************************************************************/

/*
 * Return the step in the fixed dimension that contains 'type'.  'type'
 * is assumed to be either a dtype with ndim==0 or a FixedDim.
 */
static inline int64_t
fixed_step(ndt_t *type, int64_t step, bool *overflow)
{
    assert(ndt_is_concrete(type));
    assert(type->tag != VarDim);

    if (step != INT64_MAX) {
        return step;
    }

    switch (type->tag) {
    case FixedDim:
        if (type->Concrete.FixedDim.itemsize == 0) {
            return MULi64(type->FixedDim.shape, type->Concrete.FixedDim.step,
                          overflow);
        }
        else {
            return DIVi64(type->datasize, type->Concrete.FixedDim.itemsize,
                          overflow);
        }
    default:
        return 1;
    }
}

static inline int64_t
fixed_datasize(ndt_t *type, int64_t shape, int64_t step, int64_t itemsize,
               bool *overflow)
{
    int64_t index_range;
    int64_t datasize;
    int64_t abs_step;

    if (shape == 0 || type->datasize == 0) {
        return 0;
    }

    abs_step = ABSi64(step, overflow);
    index_range = MULi64(shape-1, abs_step, overflow);
    datasize = MULi64(index_range, itemsize, overflow);
    return ADDi64(datasize, type->datasize, overflow);
}
 
static ndt_t *
_ndt_to_fortran(const ndt_t *t, int64_t step, ndt_context_t *ctx)
{
    bool overflow = 0;
    ndt_t *dt;
    int64_t next_step;

    if (t->ndim == 0) {
        return ndt_copy(t, ctx);
    }

    next_step = MULi64(step, t->FixedDim.shape, &overflow);
    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError,
            "overflow in converting to Fortran order");
        return NULL;
    }

    assert(t->tag == FixedDim);
    dt = _ndt_to_fortran(t->FixedDim.type, next_step, ctx);
    if (dt == NULL) {
        return NULL;
    }

    return ndt_fixed_dim(dt, t->FixedDim.shape, step, ctx);
}

/* Return a copy of a C-contiguous array in Fortran order. */
ndt_t *
ndt_to_fortran(const ndt_t *t, ndt_context_t *ctx)
{
    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_TypeError,
            "cannot convert abstract type to Fortran order");
        return NULL;
    }

    if (!ndt_is_c_contiguous(t)) {
        ndt_err_format(ctx, NDT_TypeError,
            "array must be C-contiguous for conversion to Fortran order");
        return NULL;
    }

    return _ndt_to_fortran(t, 1, ctx);
}

ndt_t *
ndt_fixed_dim(ndt_t *type, int64_t shape, int64_t step, ndt_context_t *ctx)
{
    ndt_t *t;
    bool overflow = 0;

    if (!check_type_invariants(type, ctx)) {
        ndt_del(type);
        return NULL;
    }

    if (shape < 0) {
        ndt_err_format(ctx, NDT_ValueError, "shape must be a natural number");
        return NULL;
    }

    /* abstract type */
    t = ndt_new(FixedDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->FixedDim.tag = RequireNA;
    t->FixedDim.shape = shape;
    t->FixedDim.type = type;
    t->ndim = type->ndim + 1;
    t->flags = ndt_dim_flags(type);

    t->Concrete.FixedDim.itemsize = 0;
    t->Concrete.FixedDim.step = INT64_MAX;

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        int64_t itemsize = ndt_itemsize(type);
        step = fixed_step(type, step, &overflow);

        t->Concrete.FixedDim.itemsize = itemsize;
        t->Concrete.FixedDim.step = step;
        t->datasize = fixed_datasize(type, shape, step, itemsize, &overflow);
        t->align = type->align;
    }

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "data size too large");
        ndt_del(t);
        return NULL;
    }

    return t;
}

ndt_t *
ndt_fixed_dim_tag(ndt_t *type, enum ndt_contig tag, int64_t shape, int64_t step,
                  ndt_context_t *ctx)
{
    ndt_t *t = ndt_fixed_dim(type, shape, step, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedDim.tag = tag;

    if (tag != RequireNA) {
        t->access = Abstract;
    }

    return t;
}

ndt_t *
ndt_abstract_var_dim(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t = NULL;

    if (!check_abstract_var_invariants(type, ctx)) {
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(VarDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->VarDim.type = type;
    t->ndim = type->ndim+1;
    t->flags = ndt_dim_flags(type);

    /* concrete access */
    t->access = Abstract;
    t->Concrete.VarDim.flag = ExternalOffsets;
    t->Concrete.VarDim.itemsize = 0;
    t->Concrete.VarDim.noffsets = 0;
    t->Concrete.VarDim.offsets = NULL;
    t->Concrete.VarDim.nslices = 0;
    t->Concrete.VarDim.slices = NULL;

    return t;
}

/*
 * Compute the current start index, step and shape of a var dimension.
 * Recomputing the values avoids a potentially very large shape array
 * per dimension (same size as the offset array).
 */
int64_t
ndt_var_indices(int64_t *res_start, int64_t *res_step, const ndt_t *t,
                int64_t index, ndt_context_t *ctx)
{
    int64_t list_start, list_stop, list_shape;
    int64_t start, stop, step;
    int64_t res_shape;
    const ndt_slice_t *slices;
    int32_t i;

    assert(ndt_is_concrete(t));
    assert(t->tag == VarDim);

    if (index < 0 || index+1 >= t->Concrete.VarDim.noffsets) {
        ndt_err_format(ctx, NDT_ValueError,
            "var dim index out of range: index=%" PRIi64 ", noffsets=%" PRIi32,
            index, t->Concrete.VarDim.noffsets);
        return -1;
    }

    list_start = t->Concrete.VarDim.offsets[index];
    list_stop = t->Concrete.VarDim.offsets[index+1];
    list_shape = list_stop - list_start;

    *res_start = 0;
    *res_step = 1;
    res_shape = list_shape;
    slices = t->Concrete.VarDim.slices;

    for (i = 0; i < t->Concrete.VarDim.nslices; i++) {
        start = slices[i].start;
        stop = slices[i].stop;
        step = slices[i].step;
        res_shape = ndt_slice_adjust_indices(res_shape, &start, &stop, step);
        *res_start += (start * *res_step);
        *res_step *= step;
    }

    *res_start += list_start;

    return res_shape;
}

ndt_slice_t *
ndt_var_add_slice(int32_t *nslices, const ndt_t *t, 
                  int64_t start, int64_t stop, int64_t step,
                  ndt_context_t *ctx)
{
    int n = t->Concrete.VarDim.nslices;
    ndt_slice_t *slices;

    assert(ndt_is_concrete(t));
    assert(t->tag == VarDim);

    if (n == INT_MAX) {
        ndt_err_format(ctx, NDT_RuntimeError, "slice stack overflow");
        return NULL;
    }

    slices = ndt_alloc(n+1, sizeof *slices);
    if (slices == NULL) {
        return ndt_memory_error(ctx);
    }
    memcpy(slices, t->Concrete.VarDim.slices, n * (sizeof *slices));

    slices[n].start = start;
    slices[n].stop = stop;
    slices[n].step = step;

    *nslices = n+1;

    return slices;
}

ndt_t *
ndt_var_dim(ndt_t *type,
            enum ndt_offsets flag,
            int32_t noffsets, const int32_t *offsets,
            int32_t nslices, ndt_slice_t *slices,
            ndt_context_t *ctx)
{
    bool overflow = 0;
    ndt_t *t;
    int64_t itemsize, datasize;

    assert(offsets != NULL);
    assert(!!nslices == !!slices);

    if (!check_var_invariants(flag, type, ctx)) {
        ndt_del(type);
        goto error;
    }

    if (noffsets < 2) {
        ndt_err_format(ctx, NDT_InvalidArgumentError, "var_dim: noffsets < 2");
        ndt_del(type);
        goto error;
    }

    if (!ndt_is_concrete(type)) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "var_dim: expected concrete type");
        ndt_del(type);
        goto error;
    }

    switch (type->tag) {
    case VarDim:
        if (offsets[noffsets-1] != type->Concrete.VarDim.noffsets-1) {
            ndt_err_format(ctx, NDT_ValueError,
                "var_dim: missing or invalid number of offset arguments");
            ndt_del(type);
            goto error;
        }
        datasize = type->datasize;
        itemsize = type->Concrete.VarDim.itemsize;
        break;
    default:
        datasize = MULi64(offsets[noffsets-1], type->datasize, &overflow);
        itemsize = type->datasize;
        break;
    }

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError,
            "overflow in creating var dimension");
        ndt_del(type);
        goto error;
    }

    /* abstract type */
    t = ndt_new(VarDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        goto error;
    }
    t->VarDim.type = type;
    t->ndim = type->ndim+1;
    t->flags = ndt_dim_flags(type);

    /* concrete access */
    t->access = Concrete;
    t->datasize = datasize;
    t->align = type->align;
    t->Concrete.VarDim.flag = flag;
    t->Concrete.VarDim.itemsize = itemsize;
    t->Concrete.VarDim.noffsets = noffsets;
    t->Concrete.VarDim.offsets = offsets;
    t->Concrete.VarDim.nslices = nslices;
    t->Concrete.VarDim.slices = slices;

    return t;


error:
    if (flag == InternalOffsets) {
        ndt_free((int32_t *)offsets);
    }
    ndt_free(slices);
    return NULL;
}

ndt_t *
ndt_symbolic_dim(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    if (!check_type_invariants(type, ctx)) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(SymbolicDim, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }
    t->SymbolicDim.tag = RequireNA;
    t->SymbolicDim.name = name;
    t->SymbolicDim.type = type;
    t->ndim = type->ndim + 1;
    t->flags = ndt_dim_flags(type);

    return t;
}

ndt_t *
ndt_symbolic_dim_tag(char *name, ndt_t *type, enum ndt_contig tag, ndt_context_t *ctx)
{
    ndt_t *t = ndt_symbolic_dim(name, type, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->SymbolicDim.tag = tag;

    return t;
}

ndt_t *
ndt_ellipsis_dim(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    uint32_t flags;

    if (!check_ellipsis_invariants(type, ctx)) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    flags = ndt_dim_flags(type);
    if (flags & NDT_ELLIPSIS) {
        ndt_err_format(ctx, NDT_ValueError, "more than one ellipsis");
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(EllipsisDim, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }
    t->EllipsisDim.tag = RequireNA;
    t->EllipsisDim.name = name;
    t->EllipsisDim.type = type;
    t->flags = flags | NDT_ELLIPSIS;
    t->ndim = type->ndim + 1;

    return t;
}

ndt_t *
ndt_ellipsis_dim_tag(char *name, ndt_t *type, enum ndt_contig tag, ndt_context_t *ctx)
{
    ndt_t *t = ndt_ellipsis_dim(name, type, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->EllipsisDim.tag = tag;

    return t;
}


/******************************************************************************/
/*                             Container types                                */
/******************************************************************************/

/*
 * Initialize the access information of a concrete tuple or record.
 * Assumptions:
 *   1) t->tag == Tuple || t->tag == Record
 *   2) t->access == Concrete
 *   3) 0 <= i < shape ==> fields[i].access == Concrete
 *   4) len(fields) == len(offsets) == len(align) == len(pad) == shape
 */
static int
init_concrete_fields(ndt_t *t, int64_t *offsets, uint16_t *align, uint16_t *pad,
                     const ndt_field_t *fields, int64_t shape,
                     uint16_opt_t align_attr, uint16_opt_t pack,
                     ndt_context_t *ctx)
{
    bool overflow = 0;
    int64_t offset = 0;
    int64_t size = 0;
    uint16_t maxalign;
    int64_t i;

    maxalign = get_align(align_attr, 1, ctx);
    if (maxalign == UINT16_MAX) {
        return -1;
    }

    if (get_align(pack, 1, ctx) == UINT16_MAX) {
        return -1;
    }

    for (i = 0; i < shape; i++) {
        assert(fields[i].access == Concrete);
        assert(fields[i].type->access == Concrete);

        if (pack.tag == Some) {
            if (fields[i].Concrete.explicit_align) {
                ndt_err_format(ctx, NDT_InvalidArgumentError,
                    "cannot have 'pack' tuple attribute and field attributes");
                return -1;
            }
            align[i] = pack.Some;
        }
        else {
            align[i] = fields[i].Concrete.align;
        }

        maxalign = max(align[i], maxalign);

        if (i > 0) {
            int64_t n = offset;
            offset = round_up(offset, align[i], &overflow);
            pad[i-1] = (uint16_t)(offset - n);
        }

        offsets[i] = offset;
        offset = ADDi64(offset, fields[i].type->datasize, &overflow);
    }

    size = round_up(offset, maxalign, &overflow);

    if (shape > 0) {
        int64_t n = (size - offsets[shape-1]) - fields[shape-1].type->datasize;
        pad[shape-1] = (uint16_t)n;
    }

    assert(t->access == Concrete);
    t->align = maxalign;
    t->datasize = size;

    for (i = 0; i < shape; i++) {
        if (fields[i].Concrete.explicit_pad) {
            if (fields[i].Concrete.pad != pad[i]) {
                ndt_err_format(ctx, NDT_ValueError,
                    "field %" PRIi64 " has invalid padding, natural padding is %" PRIi16
                    ", got %" PRIi16 "\n",
                     i, pad[i], fields[i].Concrete.pad);
                return -1;
            }
        }
    }

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError, "tuple or record too large");
        return -1;
    }

    return 0;
}

ndt_t *
ndt_tuple(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
          uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx)
{
    ndt_t *t;
    int64_t i;

    assert((fields == NULL) == (shape == 0));

    for (i = 0; i < shape; i++) {
        if (!check_type_invariants(fields[i].type, ctx)) {
            ndt_field_array_del(fields, shape);
            return NULL;
        }
    }

    /* abstract type */
    t = ndt_tuple_new(flag, shape, ctx);
    if (t == NULL) {
        ndt_field_array_del(fields, shape);
        return NULL;
    }

    /* check concrete access */
    t->access = (flag == Variadic) ? Abstract : Concrete;
    for (i = 0; i < shape; i++) {
        if (fields[i].access == Abstract) {
            t->access = Abstract;
        }
    }

    if (t->access == Abstract) {
        /* check if any field has explicit 'align' or 'pack' attributes */
        for (i = 0; i < shape; i++) {
            if (fields[i].access == Concrete &&
                fields[i].Concrete.explicit_align) {
                ndt_err_format(ctx, NDT_InvalidArgumentError,
                               "explicit field alignment in abstract tuple");
                ndt_field_array_del(fields, shape);
                ndt_free(t);
                return NULL;
            }
        }
        for (i = 0; i < shape; i++) {
            assert(fields[i].name == NULL);
            t->Tuple.types[i] = fields[i].type;
            t->flags |= ndt_subtree_flags(fields[i].type);
        }
        ndt_free(fields);
        return t;
    }
    else {
        if (init_concrete_fields(t,
                                 t->Concrete.Tuple.offset,
                                 t->Concrete.Tuple.align,
                                 t->Concrete.Tuple.pad,
                                 fields, shape, align, pack, ctx) < 0) {
            ndt_field_array_del(fields, shape);
            ndt_free(t);
            return NULL;
        }
        for (i = 0; i < shape; i++) {
            assert(fields[i].name == NULL);
            t->Tuple.types[i] = fields[i].type;
            t->flags |= ndt_subtree_flags(fields[i].type);
        }
        ndt_free(fields);
        return t;
    }
}

ndt_t *
ndt_record(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
           uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx)
{
    ndt_t *t;
    int64_t i;

    assert((fields == NULL) == (shape == 0));

    for (i = 0; i < shape; i++) {
        if (!check_type_invariants(fields[i].type, ctx)) {
            ndt_field_array_del(fields, shape);
            return NULL;
        }
    }

    /* abstract type */
    t = ndt_record_new(flag, shape, ctx);
    if (t == NULL) {
        ndt_field_array_del(fields, shape);
        return NULL;
    }

    /* check concrete access */
    t->access = (flag == Variadic) ? Abstract : Concrete;
    for (i = 0; i < shape; i++) {
        if (fields[i].access == Abstract) {
            t->access = Abstract;
        }
    }

    if (t->access == Abstract) {
        /* check if any field has explicit 'align' or 'pack' attributes */
        for (i = 0; i < shape; i++) {
            if (fields[i].access == Concrete &&
                fields[i].Concrete.explicit_align) {
                ndt_err_format(ctx, NDT_InvalidArgumentError,
                               "explicit field alignment in abstract tuple");
                /* at this point names and types still belong to the fields */
                ndt_field_array_del(fields, shape);
                ndt_free(t);
                return NULL;
            }
        }
        for (i = 0; i < shape; i++) {
            t->Record.names[i] = fields[i].name;
            t->Record.types[i] = fields[i].type;
            t->flags |= ndt_subtree_flags(fields[i].type);
        }
        ndt_free(fields);
        return t;
    }
    else {
        if (init_concrete_fields(t,
                                 t->Concrete.Record.offset,
                                 t->Concrete.Record.align,
                                 t->Concrete.Record.pad,
                                 fields, shape, align, pack, ctx) < 0) {
            /* at this point names and types still belong to the fields */
            ndt_field_array_del(fields, shape);
            ndt_free(t);
            return NULL;
        }
        for (i = 0; i < shape; i++) {
            t->Record.names[i] = fields[i].name;
            t->Record.types[i] = fields[i].type;
            t->flags |= ndt_subtree_flags(fields[i].type);
        }
        ndt_free(fields);
        return t;
    }
}

ndt_t *
ndt_ref(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    if (!check_type_invariants(type, ctx)) {
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(Ref, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->Ref.type = type;
    t->flags = ndt_subtree_flags(type);

    /* concrete access */
    t->access = type->access;
    t->datasize = sizeof(void *);
    t->align = alignof(void *);

    return t;
}

ndt_t *
ndt_constr(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    if (!check_type_invariants(type, ctx)) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    t = ndt_new(Constr, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t->Constr.name = name;
    t->Constr.type = type;
    t->flags = ndt_subtree_flags(type);

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        t->datasize = type->datasize;
        t->align = type->align;
    }

    return t;
}

ndt_t *
ndt_nominal(char *name, ndt_t *type, ndt_context_t *ctx)
{
    const ndt_typedef_t *d;
    ndt_t *t;

    d = ndt_typedef_find(name, ctx);
    if (d == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    if (type != NULL) {
        int ret = ndt_match(d->type, type, ctx);
        if (ret <= 0) {
            if (ret == 0) {
                ndt_err_format(ctx, NDT_ValueError,
                    "type is not an instance of %s", name);
            }
            ndt_free(name);
            ndt_del(type);
            return NULL;
        }
    }
    else {
        type = ndt_copy(d->type, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }
    }

    /* abstract type */
    t = ndt_new(Nominal, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }
    t->Nominal.name = name;
    t->Nominal.type = type;
    t->Nominal.meth = &d->meth;
    t->flags = ndt_subtree_flags(type);

    /* concrete access */
    t->access = type->access;
    t->datasize = type->datasize;
    t->align = type->align;

    return t;
}


/******************************************************************************/
/*                               Scalar types                                 */
/******************************************************************************/

ndt_t *
ndt_scalar_kind(ndt_context_t *ctx)
{
    return ndt_new(ScalarKind, ctx);
}

/* Define a sort order for the typed values in the categorical set. */
static int
cmp(const void *x, const void *y)
{
    const ndt_value_t *p = (const ndt_value_t *)x;
    const ndt_value_t *q = (const ndt_value_t *)y;

    return ndt_value_compare(p, q);
}

ndt_t *
ndt_categorical(ndt_value_t *types, int64_t ntypes, ndt_context_t *ctx)
{
    ndt_value_t *tmp;
    ndt_t *t;
    int64_t size, i;

    tmp = ndt_alloc(ntypes, sizeof(ndt_value_t));
    if (tmp == NULL) {
        ndt_value_array_del(types, ntypes);
        return ndt_memory_error(ctx);
    }

    /* Successful allocation implies no overflow and size <= SIZE_MAX. */
    size = ntypes * sizeof(ndt_value_t);
    memcpy(tmp, types, (size_t)size);
    qsort(tmp, (size_t)ntypes, sizeof *tmp, cmp);

    for (i = 0; i+1 < ntypes; i++) {
        if (ndt_value_mem_equal(&tmp[i], &tmp[i+1])) {
            ndt_free(tmp);
            ndt_value_array_del(types, ntypes);
            ndt_err_format(ctx, NDT_ValueError, "duplicate category entries");
            return NULL;
        }
    }
    ndt_free(tmp);

    /* abstract type */
    t = ndt_new(Categorical, ctx);
    if (t == NULL) {
        ndt_value_array_del(types, ntypes);
        return NULL;
    }
    t->Categorical.ntypes = ntypes;
    t->Categorical.types = types;

    /* concrete access */
    t->access = Concrete;
    t->datasize = sizeof(ndt_categorical_t);
    t->align = alignof(ndt_categorical_t);

    return t;
}

ndt_t *
ndt_fixed_string_kind(ndt_context_t *ctx)
{
    return ndt_new(FixedStringKind, ctx);
}

ndt_t *
ndt_fixed_string(int64_t size, enum ndt_encoding encoding, ndt_context_t *ctx)
{
    bool overflow = 0;
    ndt_t *t;

    /* abstract type */
    t = ndt_new(FixedString, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedString.size = size;
    t->FixedString.encoding = encoding;

    /* concrete access */
    t->access = Concrete;
    t->datasize = MULi64(ndt_sizeof_encoding(encoding), size, &overflow);
    t->align = ndt_alignof_encoding(encoding);

    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError,
            "overflow while creating fixed string");
        ndt_del(t);
        return NULL;
    }

    return t;
}

ndt_t *
ndt_fixed_bytes_kind(ndt_context_t *ctx)
{
    return ndt_new(FixedBytesKind, ctx);
}

ndt_t *
ndt_fixed_bytes(int64_t size, uint16_opt_t align_attr, ndt_context_t *ctx)
{
    ndt_t *t;
    uint16_t align;

    align = get_align(align_attr, 1, ctx);
    if (align == UINT16_MAX) {
        return NULL;
    }

    if (size % align != 0) {
        ndt_err_format(ctx, NDT_ValueError,
            "data size must be a multiple of alignment");
        return NULL;
    }

    /* abstract type */
    t = ndt_new(FixedBytes, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedBytes.size = size;
    t->FixedBytes.align = align;

    /* concrete access */
    t->access = Concrete;
    t->datasize = size;
    t->align = align;

    return t;
}

ndt_t *
ndt_string(ndt_context_t *ctx)
{
    ndt_t *t;

    /* abstract type */
    t = ndt_new(String, ctx);
    if (t == NULL) {
        return NULL;
    }

    /* concrete access */
    t->access = Concrete;
    t->datasize = sizeof(char *);
    t->align = alignof(char *);

    return t;
}

ndt_t *
ndt_bytes(uint16_opt_t target_align, ndt_context_t *ctx)
{
    ndt_t *t;
    uint16_t align;

    align = get_align(target_align, 1, ctx);
    if (align == UINT16_MAX) {
        return NULL;
    }

    /* abstract type */
    t = ndt_new(Bytes, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Bytes.target_align = align;

    /* concrete access */
    t->access = Concrete;
    t->datasize = sizeof(ndt_bytes_t);
    t->align = alignof(ndt_bytes_t);

    return t;
}

ndt_t *
ndt_char(enum ndt_encoding encoding, ndt_context_t *ctx)
{
    ndt_t *t;

    /* abstract type */
    t = ndt_new(Char, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Char.encoding = encoding;

    /* concrete access */
    t->access = Concrete;
    t->datasize = ndt_sizeof_encoding(encoding);
    t->align = ndt_alignof_encoding(encoding);

    return t;
}

ndt_t *
ndt_signed_kind(ndt_context_t *ctx)
{
    return ndt_new(SignedKind, ctx);
}

ndt_t *
ndt_unsigned_kind(ndt_context_t *ctx)
{
    return ndt_new(UnsignedKind, ctx);
}

ndt_t *
ndt_float_kind(ndt_context_t *ctx)
{
    return ndt_new(FloatKind, ctx);
}

ndt_t *
ndt_complex_kind(ndt_context_t *ctx)
{
    return ndt_new(ComplexKind, ctx);
}

ndt_t *
ndt_primitive(enum ndt tag, uint32_t flags, ndt_context_t *ctx)
{
    ndt_t *t;

    if (flags != 0 && flags != NDT_LITTLE_ENDIAN && flags != NDT_BIG_ENDIAN) {
        ndt_err_format(ctx, NDT_ValueError,
            "flags argument must be 0 or NDT_LITTLE_ENDIAN or NDT_BIG_ENDIAN");
        return NULL;
    }

    /* abstract type */
    t = ndt_new(tag, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->flags |= flags;

    /* concrete access */
    t->access = Concrete;

    switch(tag) {
    case Bool:
        t->datasize = sizeof(bool);
        t->align = alignof(bool);
        break;
    case Int8:
        t->datasize = sizeof(int8_t);
        t->align = alignof(int8_t);
        break;
    case Int16:
        t->datasize = sizeof(int16_t);
        t->align = alignof(int16_t);
        break;
    case Int32:
        t->datasize = sizeof(int32_t);
        t->align = alignof(int32_t);
        break;
    case Int64:
        t->datasize = sizeof(int64_t);
        t->align = alignof(int64_t);
        break;
    case Uint8:
        t->datasize = sizeof(uint8_t);
        t->align = alignof(uint8_t);
        break;
    case Uint16:
        t->datasize = sizeof(uint16_t);
        t->align = alignof(uint16_t);
        break;
    case Uint32:
        t->datasize = sizeof(uint32_t);
        t->align = alignof(uint32_t);
        break;
    case Uint64:
        t->datasize = sizeof(uint64_t);
        t->align = alignof(uint64_t);
        break;
    case Float16:
        t->datasize = 2;
        t->align = 2;
        break;
    case Float32:
        t->datasize = sizeof(float);
        t->align = alignof(float);
        break;
    case Float64:
        t->datasize = sizeof(double);
        t->align = alignof(double);
        break;
    case Complex32:
        t->datasize = 4;
        t->align = 2;
        break;
    case Complex64:
        t->datasize = sizeof(ndt_complex64_t);
        t->align = alignof(ndt_complex64_t);
        break;
    case Complex128:
        t->datasize = sizeof(ndt_complex128_t);
        t->align = alignof(ndt_complex128_t);
        break;
    default:
        ndt_err_format(ctx, NDT_ValueError, "not a primitive type"),
        ndt_free(t);
        return NULL;
    }

    return t;
}

ndt_t *
ndt_signed(int size, uint32_t flags, ndt_context_t *ctx)
{
    switch (size) {
    case 1: return ndt_primitive(Int8, flags, ctx);
    case 2: return ndt_primitive(Int16, flags, ctx);
    case 4: return ndt_primitive(Int32, flags, ctx);
    case 8: return ndt_primitive(Int64, flags, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for signed integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_unsigned(int size, uint32_t flags, ndt_context_t *ctx)
{
    switch (size) {
    case 1: return ndt_primitive(Uint8, flags, ctx);
    case 2: return ndt_primitive(Uint16, flags, ctx);
    case 4: return ndt_primitive(Uint32, flags, ctx);
    case 8: return ndt_primitive(Uint64, flags, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for unsigned integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_from_alias(enum ndt_alias tag, uint32_t flags, ndt_context_t *ctx)
{
    switch (tag) {
    case Size: return ndt_unsigned(sizeof(size_t), flags, ctx);
    case Intptr: return ndt_signed(sizeof(intptr_t), flags, ctx);
    case Uintptr: return ndt_unsigned(sizeof(uintptr_t), flags, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError, "invalid alias tag");
        return NULL;
    }
}

ndt_t *
ndt_typevar(char *name, ndt_context_t *ctx)
{
    ndt_t *t;

    /* abstract type */
    t = ndt_new(Typevar, ctx);
    if (t == NULL) {
        ndt_free(name);
        return NULL;
    }
    t->Typevar.name = name;

    return t;
}
