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
#include <inttypes.h>
#include <stdbool.h>
#include <complex.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include "ndtypes.h"


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
        min_align = max(align.Some, t->Concrete.align);
    }
    else if (pack.tag == Some) {
        if (t->access == Abstract) {
            goto access_error;
        }
        min_align = pack.Some;
    }
    else {
        if (t->access == Concrete) {
            min_align = t->Concrete.align;
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

static size_t
round_up(size_t offset, uint16_t align)
{
    return ((offset + align - 1) / align) * align;
}

void *
ndt_memory_error(ndt_context_t *ctx)
{
    ndt_err_format(ctx, NDT_MemoryError, "out of memory");
    return NULL;
}

char *
ndt_strdup(const char *s, ndt_context_t *ctx)
{
    size_t len = strlen(s);
    char *cp;

    cp = ndt_alloc(1, len+1);
    if (cp == NULL) {
        return ndt_memory_error(ctx);
    }

    memcpy(cp, s, len);
    cp[len] = '\0';
    return cp;
}

/* Sum an array of positive int64_t. */
static int64_t
sum_pos_int64(int64_t *values, int64_t len, ndt_context_t *ctx)
{
    int64_t sum = 0;
    int64_t i;

    for (i = 0; i < len; i++) {
        sum += values[i];
        if (sum < 0) {
            ndt_err_format(ctx, NDT_ValueError,
                "overflow: shape values too large");
            return -1;
        }
    }

    return sum;
}


/******************************************************************************/
/*                             Sequence elements                              */
/******************************************************************************/

void
ndt_memory_del(ndt_memory_t *mem)
{
    if (mem == NULL) {
        return;
    }

    if (mem->t->tag == String) {
        ndt_free(mem->v.String);
    }
    ndt_del(mem->t);
    ndt_free(mem);
}

void
ndt_memory_array_del(ndt_memory_t *mem, size_t ntypes)
{
    size_t i;

    if (mem == NULL) {
        return;
    }

    for (i = 0; i < ntypes; i++) {
        if (mem[i].t->tag == String) {
            ndt_free(mem[i].v.String);
        }
        ndt_del(mem[i].t);
    }

    ndt_free(mem);
}

void
ndt_attr_del(ndt_attr_t *attr)
{
    size_t i;

    if (attr == NULL) {
        return;
    }

    ndt_free(attr->name);

    switch (attr->tag) {
    case AttrValue:
        ndt_free(attr->AttrValue);
        break;
    case AttrList:
        for (i = 0; i < attr->AttrList.len; i++) {
            ndt_free(attr->AttrList.items[i]);
        }
        ndt_free(attr->AttrList.items);
        break;
    default:
        abort(); /* NOT REACHED */
    }

    ndt_free(attr);
}

void
ndt_attr_array_del(ndt_attr_t *attr, size_t nattr)
{
    size_t i, k;

    if (attr == NULL) {
        return;
    }

    for (i = 0; i < nattr; i++) {
        ndt_free(attr[i].name);

        switch (attr[i].tag) {
        case AttrValue:
            ndt_free(attr[i].AttrValue);
            break;
        case AttrList:
            for (k = 0; k < attr[i].AttrList.len; k++) {
                ndt_free(attr[i].AttrList.items[k]);
            }
            ndt_free(attr[i].AttrList.items);
            break;
        default:
            abort(); /* NOT REACHED */
        }
    }

    ndt_free(attr);
}

/*
 * align = n: minimum alignment for the field; the resulting alignment is
 * guaranteed to be at least the maximum of n and the natural alignment of
 * the field member.
 *
 * pack = n: minimum alignment for the field; the resulting alignment is
 * guaranteed to be at least n.
 *
 * 'name' is NULL for a tuple field.
 */
ndt_field_t *
ndt_field(char *name, ndt_t *type, uint16_opt_t align, uint16_opt_t pack,
          ndt_context_t *ctx)
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
    field = ndt_alloc(1, sizeof *field);
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
ndt_field_array_del(ndt_field_t *fields, size_t shape)
{
    size_t i;

    if (fields == NULL) {
        return;
    }

    for (i = 0; i < shape; i++) {
        ndt_free(fields[i].name);
        ndt_del(fields[i].type);
    }

    ndt_free(fields);
}


/******************************************************************************/
/*                                   Tags                                     */
/******************************************************************************/

const char *
ndt_tag_as_string(enum ndt tag)
{
    switch (tag) {
    case AnyKind: return "Any";
    case Option: return "option";
    case Nominal: return "nominal";
    case Constr: return "constr";

    case Tuple: return "tuple";
    case Record: return "record";
    case Function: return "function";
    case Typevar: return "typevar";

    case ScalarKind: return "ScalarKind";
    case Void: return "void";
    case Bool: return "bool";

    case SignedKind: return "SignedKind";
    case Int8: return "int8";
    case Int16: return "int16";
    case Int32: return "int32";
    case Int64: return "int64";

    case UnsignedKind: return "UnsignedKind";
    case Uint8: return "uint8";
    case Uint16: return "uint16";
    case Uint32: return "uint32";
    case Uint64: return "uint64";

    case RealKind: return "RealKind";
    case Float16: return "float16";
    case Float32: return "float32";
    case Float64: return "float64";

    case ComplexKind: return "ComplexKind";
    case Complex64: return "complex64";
    case Complex128: return "complex128";

    case Char: return "char";

    case String: return "string";
    case FixedStringKind: return "FixedStringKind";
    case FixedString: return "FixedString";

    case Bytes: return "bytes";
    case FixedBytesKind: return "FixedBytesKind";
    case FixedBytes: return "FixedBytes";

    case Categorical: return "categorical";
    case Pointer: return "pointer";

    default: return "unknown tag";
    }
}


/******************************************************************************/
/*                                 Encodings                                  */
/******************************************************************************/

enum ndt_encoding
ndt_encoding_from_string(char *s, ndt_context_t *ctx)
{
    enum ndt_encoding ret = ErrorEncoding;

    if (strcmp(s, "A") == 0 ||
        strcmp(s, "ascii") == 0 ||
        strcmp(s, "us-ascii") == 0) {
        ret = Ascii;
    }
    else if (strcmp(s, "U8") == 0 ||
             strcmp(s, "utf8") == 0 ||
             strcmp(s, "utf-8") == 0) {
        ret = Utf8;
    }
    else if (strcmp(s, "U16") == 0 ||
             strcmp(s, "utf16") == 0 ||
             strcmp(s, "utf-16") == 0) {
        ret = Utf16;
    }
    else if (strcmp(s, "U32") == 0 ||
             strcmp(s, "utf32") == 0 ||
             strcmp(s, "utf-32") == 0) {
        ret = Utf32;
    }
    else if (strcmp(s, "ucs2") == 0 ||
             strcmp(s, "ucs-2") == 0 ||
             strcmp(s, "ucs_2") == 0) {
        ret = Ucs2;
    }
    else {
        ndt_err_format(ctx, NDT_ValueError, "invalid encoding: '%s'", s);
    }

    ndt_free(s);
    return ret;
}

const char *
ndt_encoding_as_string(enum ndt_encoding encoding)
{
    switch (encoding) {
    case Ascii: return "'ascii'";
    case Utf8: return "'utf8'";
    case Utf16: return "'utf16'";
    case Utf32: return "'utf32'";
    case Ucs2: return "'ucs2'";
    default: return "'unknown'";
    }
}

size_t
ndt_sizeof_encoding(enum ndt_encoding encoding)
{
    switch (encoding) {
    case Ascii: case Utf8:
        return 1;
    case Utf16: case Ucs2:
        return 2;
    case Utf32:
        return 4;
    default: /* NOT REACHED */
        abort();
    }
}

uint16_t
ndt_alignof_encoding(enum ndt_encoding encoding)
{
    return (uint16_t)ndt_sizeof_encoding(encoding);
}

/******************************************************************************/
/*                                 Datashape                                  */
/******************************************************************************/

ndt_t *
ndt_new(enum ndt tag, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_alloc(1, sizeof *t);
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->tag = tag;
    t->access = Abstract;
    t->ndim = 0;

    return t;
}

static ndt_t *
ndt_new_extra(enum ndt tag, size_t n, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_alloc(1, offsetof(ndt_t, extra) + n);
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->tag = tag;
    t->access = Abstract;
    t->ndim = 0;

    return t;
}

void
ndt_del(ndt_t *t)
{
    if (t == NULL) {
        return;
    }

    switch (t->tag) {
    case Array:
        ndt_del(t->Array.type);
        break;
    case FixedDim:
        ndt_del(t->FixedDim.type);
        break;
    case VarDim:
        ndt_del(t->VarDim.type);
        if (ndt_is_concrete(t)) {
            ndt_free(t->Concrete.VarDim.shapes);
        }
        break;
    case SymbolicDim:
        ndt_free(t->SymbolicDim.name);
        ndt_del(t->SymbolicDim.type);
        break;
    case EllipsisDim:
        ndt_del(t->EllipsisDim.type);
        break;
    case Option:
        ndt_del(t->Option.type);
        break;
    case Nominal:
        ndt_free(t->Nominal.name);
        break;
    case Constr:
        ndt_free(t->Constr.name);
        ndt_del(t->Constr.type);
        break;
    case Tuple: {
        int64_t i;
        for (i = 0; i < t->Tuple.shape; i++) {
            ndt_del(t->Tuple.types[i]);
        }
        break;
    }
    case Record: {
        int64_t i;
        for (i = 0; i < t->Record.shape; i++) {
            ndt_free(t->Record.names[i]);
            ndt_del(t->Record.types[i]);
        }
        break;
    }
    case Function:
        ndt_del(t->Function.ret);
        ndt_del(t->Function.pos);
        ndt_del(t->Function.kwds);
        break;
    case Typevar:
        ndt_free(t->Typevar.name);
        break;
    case Categorical:
        ndt_memory_array_del(t->Categorical.types, t->Categorical.ntypes);
        break;
    case Pointer:
        ndt_del(t->Pointer.type);
        break;
    default:
        break;
    }

    ndt_free(t);
}

ndt_t *
ndt_any_kind(ndt_context_t *ctx)
{
    return ndt_new(AnyKind, ctx);
}

uint32_t
ndt_dim_stride(const ndt_t *t)
{
    assert(ndt_is_concrete(t));

    switch (t->tag) {
    case FixedDim:
        return t->Concrete.FixedDim.stride;
    case VarDim:
        return t->Concrete.size;
    default:
        return t->Concrete.size;
    }
}

uint32_t
ndt_dim_flags(const ndt_t *t)
{
    switch (t->tag) {
    case Array:
        return t->Array.flags;
    case FixedDim:
        return t->FixedDim.flags;
    case VarDim:
        return t->VarDim.flags;
    case SymbolicDim:
        return t->SymbolicDim.flags;
    case EllipsisDim:
        return t->EllipsisDim.flags;
    default:
        return 0;
    }
}

uint32_t
ndt_common_flags(const ndt_t *t)
{
    return ndt_dim_flags(t) & ~NDT_Dim_option;
}

uint32_t
ndt_select_dim_size(int64_t n)
{

    if (n <= UINT8_MAX) {
        assert(NDT_Dim_uint8 == 1);
        return NDT_Dim_uint8;
    }
    if (n <= UINT16_MAX) {
        assert(NDT_Dim_uint16 == 2);
        return NDT_Dim_uint16;
    }
    if (n <= UINT32_MAX) {
        assert(NDT_Dim_uint32 == 4);
        return NDT_Dim_uint32;
    }

    assert(NDT_Dim_uint64 == 8);
    return NDT_Dim_uint64;
}

uint32_t
ndt_dim_size(const ndt_t *t)
{
    return ndt_dim_flags(t) & NDT_Dim_size;
}

uint32_t
ndt_dim_align(const ndt_t *t)
{
    switch (ndt_dim_size(t)) {
    case 1: return alignof(uint8_t);
    case 2: return alignof(uint16_t);
    case 4: return alignof(uint32_t);
    case 8: return alignof(uint64_t);
    default: return 1;
    }
}

enum ndt_dim
ndt_dim_type(const ndt_t *t)
{
    switch (ndt_dim_size(t)) {
    case 0: return DimNone;
    case 1: return DimUint8;
    case 2: return DimUint16;
    case 4: return DimUint32;
    case 8: return DimUint64;
    default: abort();
    }
}

const char *
ndt_dim_type_as_string(enum ndt_dim tag)
{
    switch (tag) {
    case DimNone: return "none";
    case DimUint8: return "uint8";
    case DimUint16: return "uint16";
    case DimUint32: return "uint32";
    case DimUint64: return "uint64";
    case DimInt32: return "int32";
    }

    return "not reached";
}

int
ndt_is_ndarray(const ndt_t *t)
{
    switch (t->tag) {
    case Array:
        return t->Array.flags & NDT_Ndarray;
    default:
        return 0;
    }
}

ndt_t *
ndt_fixed_dim(int64_t shape, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    if (type->ndim > NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_ValueError, "ndim > %u", NDT_MAX_DIM);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(FixedDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->FixedDim.flags = ndt_common_flags(type);
    t->FixedDim.shape = shape;
    t->FixedDim.type = type;
    t->ndim = type->ndim + 1;

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        t->Concrete.FixedDim.offset = 0;
        t->Concrete.FixedDim.itemsize = type->Concrete.size;
        t->Concrete.FixedDim.stride = shape * ndt_dim_stride(type);
        t->Concrete.size = 0;
        t->Concrete.align = 1;
    }

    return t;
}

ndt_t *
ndt_symbolic_dim(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    if (type->ndim > NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_ValueError, "ndim > %u", NDT_MAX_DIM);
        ndt_del(type);
        return NULL;
    }

    if (ndt_dim_size(type) != 0) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "var-shapes given for abstract type");
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
    t->SymbolicDim.flags = ndt_common_flags(type);
    t->SymbolicDim.name = name;
    t->SymbolicDim.type = type;
    t->ndim = type->ndim + 1;

    return t;
}

ndt_t *
ndt_var_dim(int64_t *shapes, int64_t nshapes, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    uint32_t dim_size;

    assert((shapes==NULL) == (nshapes==0));

    if (type->ndim > NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_ValueError, "ndim > %u", NDT_MAX_DIM);
        ndt_free(shapes);
        ndt_del(type);
        return NULL;
    }

    dim_size = ndt_dim_size(type);
    if (shapes) {
        if (ndt_is_abstract(type)) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                           "var-shapes given for abstract type");
            ndt_free(shapes);
            ndt_del(type);
            return NULL;
        }

        if (dim_size == 0) {
            int64_t n = sum_pos_int64(shapes, nshapes, ctx);
            if (n < 0) {
                ndt_free(shapes);
                ndt_del(type);
                return NULL;
            }
            dim_size = ndt_select_dim_size(n);
        }
    }

    /* abstract type */
    t = ndt_new(VarDim, ctx);
    if (t == NULL) {
        ndt_free(shapes);
        ndt_del(type);
        return NULL;
    }
    t->VarDim.flags = ndt_common_flags(type) | dim_size;
    t->VarDim.type = type;
    t->ndim = type->ndim + 1;

    /* concrete access */
    t->access = shapes ? type->access : Abstract;
    if (t->access == Concrete) {
        t->Concrete.VarDim.offset = 0;
        t->Concrete.VarDim.itemsize = type->Concrete.size;
        t->Concrete.VarDim.stride = type->Concrete.size;
        t->Concrete.VarDim.nshapes = nshapes;
        t->Concrete.VarDim.shapes = shapes;
        t->Concrete.size = dim_size;
        t->Concrete.align = dim_size;
    }

    return t;
}

ndt_t *
ndt_ellipsis_dim(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    uint32_t flags;

    if (type->ndim > NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_ValueError, "ndim > %u", NDT_MAX_DIM);
        ndt_del(type);
        return NULL;
    }

    if (ndt_dim_size(type) != 0) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "var-shapes given for abstract type");
        ndt_del(type);
        return NULL;
    }

    flags = ndt_common_flags(type);
    if (flags & NDT_Dim_ellipsis) {
        ndt_err_format(ctx, NDT_ValueError, "more than one ellipsis");
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(EllipsisDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->EllipsisDim.flags = flags | NDT_Dim_ellipsis;
    t->EllipsisDim.type = type;
    t->ndim = type->ndim + 1;

    return t;
}

static int
init_strides_order(ndt_t *a, ndt_t *type, char_opt_t order, ndt_context_t *ctx)
{
    ndt_t *dims[NDT_MAX_DIM];
    ndt_t *dtype;
    uint32_t cflag;
    int32_t stride;
    int i;

    assert(a->tag == Array);
    assert(ndt_is_concrete(a));
    assert(type->ndim > 0);
    assert(ndt_is_concrete(type));

    ndt_dims_dtype(dims, &dtype, type);

    cflag = NDT_C_contiguous;
    if (order.tag == Some && order.Some == 'F') {
        cflag = NDT_F_contiguous;
    }

    /* check if subarrays are proper ndarrays */
    for (i = type->ndim-1; i >= 0; i--) {
        switch (dims[i]->tag) {
        case FixedDim:
            dims[i]->FixedDim.flags |= cflag;
            break;
        default:
            goto endloop;
        }
    }
endloop:
    if (dims[0]->FixedDim.flags & NDT_Contiguous) { /* only fixed dimensions */
        a->Array.flags |= (dims[0]->FixedDim.flags & NDT_Contiguous);
        a->Array.flags |= NDT_Ndarray;
    }

    if (order.tag == Some) {
        if (!ndt_is_contiguous(dims[0])) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "order '%c' specified but array is not %c_contiguous",
                order.Some, order.Some);
            return -1;
        }

        if (order.Some == 'F') {
            stride = dtype->Concrete.size;
            for (i = 0; i < type->ndim-1; i++) {
                switch (dims[i]->tag) {
                case FixedDim:
                    dims[i]->Concrete.FixedDim.stride = stride;
                    stride *= dims[i]->FixedDim.shape;
                    break;
                default:
                    abort(); /* NOT REACHED */
                }
            }
        }
    }

    return 0;
}

ndt_t *
ndt_next_dim(ndt_t *a)
{
    assert(a->ndim > 0);

    switch (a->tag) {
    case FixedDim: return a->FixedDim.type;
    case VarDim: return a->VarDim.type;
    case SymbolicDim: return a->SymbolicDim.type;
    case EllipsisDim: return a->EllipsisDim.type;
    default: abort();
    }
}

int
ndt_validate_var_shapes(ndt_t *t, int64_t n, ndt_context_t *ctx)
{
    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "should have invariant var-shapes <==> concrete type");
        return -1;
    }

    switch (t->tag) {
    case FixedDim:
        return ndt_validate_var_shapes(t->FixedDim.type, n * t->FixedDim.shape, ctx);
    case VarDim:
        if (t->Concrete.VarDim.nshapes == 0) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "shape arguments must be given for all var dimensions or none");
            return -1;
        }
        else if (t->Concrete.VarDim.nshapes != n) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "number of shape arguments not compatible with previous dimension");
            return -1;
        }
        else {
            n = sum_pos_int64(t->Concrete.VarDim.shapes, t->Concrete.VarDim.nshapes, ctx);
            if (n < 0) {
                return -1;
            }
            return ndt_validate_var_shapes(t->VarDim.type, n, ctx);
        }
    default:
        return 0;
    }
}

/*
 * (data0, bitmap0, dim_data1, bitmap1, dim_data2, bitmap2, ...)
 *  |-- ndim=0 --|  |---- ndim=1 ----|  |---- ndim=2 ----|
 *
 * len(offsets) == len(dim_sizes) == noffsets
 */
static size_t
init_offsets(size_t *offsets, uint16_t *array_align,
             size_t *dim_sizes, int noffsets,
             uint16_t dtype_align, uint16_t bitmap_align, uint16_t dim_align)
{
    size_t offset = 0;
    uint16_t maxalign;
    int i;

    maxalign = max(dtype_align, bitmap_align);
    maxalign = max(maxalign, dim_align);

    for (i = 0; i < noffsets; i++) {
        if (i > 0) {
            if (i % 2) {
                offset = round_up(offset, bitmap_align);
            }
            else {
                offset = round_up(offset, dim_align);
            }
        }

        offsets[i] = offset;
        offset += dim_sizes[i];
    }

    *array_align = maxalign;

    return round_up(offset, maxalign);
}

static int
init_concrete_array(ndt_t *a, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *dims[NDT_MAX_DIM];
    const ndt_t *dtype;
    size_t dim_sizes[NDT_MAX_DIM+1];
    size_t size, nitems;
    uint16_t array_align, bitmap_align, dim_align;
    int64_t n;
    int i;

    assert(a->tag == Array);
    assert(ndt_is_concrete(a));
    assert(a->ndim == type->ndim);
    assert(a->ndim > 0);
    assert(a->Concrete.Array.noffsets == 2 * (a->ndim + 1));

    ndt_get_dims_dtype(dims, &dtype, type);
    assert(ndt_is_concrete(dtype));

    nitems = 1;
    for (i = 0; i < type->ndim; i++) {
        assert(ndt_is_concrete(dims[i]));

        switch (dims[i]->tag) {
        case FixedDim:
            n = dims[i]->FixedDim.shape;
            nitems *= n;
            dim_sizes[2 * dims[i]->ndim] = 0;
            size = ndt_is_optional(dims[i]) ? round_up(n, 8) : 0;
            dim_sizes[2 * dims[i]->ndim + 1] = size;
            break;
        case VarDim:
            if ((size_t)dims[i]->Concrete.VarDim.nshapes != nitems) {
                ndt_err_format(ctx, NDT_InvalidArgumentError,
                    "missing or invalid number of var-dim shape arguments");
                return -1;
            }
            n = sum_pos_int64(dims[i]->Concrete.VarDim.shapes,
                              dims[i]->Concrete.VarDim.nshapes, ctx);
            if (n < 0) {
                return -1;
            }
            nitems = n;
            dim_sizes[2 * dims[i]->ndim] = dims[i]->Concrete.VarDim.nshapes * dims[i]->Concrete.size;
            size = ndt_is_optional(dims[i]) ? round_up(n, 8) : 0;
            dim_sizes[2 * dims[i]->ndim + 1] = size;
            break;
        default:
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "var shape arguments given for abstract array");
            return -1;
        }
    }

    size = nitems * dtype->Concrete.size;
    dim_sizes[0] = size;

    size = ndt_is_optional(dtype) ? round_up(nitems, 8) : 0;
    dim_sizes[1] = size;

    bitmap_align = alignof(uint64_t);
    dim_align = ndt_dim_align(type);
    size = init_offsets(a->Concrete.Array.offsets, &array_align, dim_sizes,
                        a->Concrete.Array.noffsets,
                        dtype->Concrete.align,
                        bitmap_align,
                        dim_align);

    a->Concrete.size = size;
    a->Concrete.align = array_align;

    return 0;
}

/*
 * Assumption:
 *   (strides==NULL || len(strides)==t->ndim) &&
 *   (offsets==NULL || len(offsets)==t->ndim)
 */
ndt_t *
ndt_array(ndt_t *type, int64_t *strides, int64_t *offsets, char_opt_t order,
          ndt_context_t *ctx)
{
    ndt_t *t;
    size_t noffsets;

    assert(type->ndim > 0);
    noffsets = 2 * (type->ndim + 1);

    if (strides || offsets) {
        /* What should happen with arbitrary user-supplied strides/offsets? */
        ndt_err_format(ctx, NDT_NotImplementedError,
                       "semantics need to be defined first");
        ndt_del(type);
        ndt_free(strides);
        ndt_free(offsets);
        return NULL;
    }

    /* abstract type */
    t = ndt_new_extra(Array, noffsets * sizeof(size_t), ctx);
    if (t == NULL) {
        ndt_del(type);
        ndt_free(strides);
        ndt_free(offsets);
        return NULL;
    }
    t->ndim = type->ndim;
    t->Array.flags = ndt_dim_flags(type);
    t->Array.type = type;

    t->access = type->access;
    if (t->access == Concrete) {
        t->Concrete.Array.dim_type = ndt_dim_type(type);
        t->Concrete.Array.noffsets = noffsets;
        t->Concrete.Array.offsets = (size_t *)t->extra;

        if (init_concrete_array(t, type, ctx) < 0) {
            ndt_del(t);
            return NULL;
        }

        if (init_strides_order(t, type, order, ctx) < 0) {
            ndt_del(t);
            return NULL;
        }
    }

    return t;
}

ndt_t *
ndt_option(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    switch (type->tag) {
    case FixedDim:
        type->FixedDim.flags |= NDT_Dim_option;
        return type;
    case VarDim:
        type->VarDim.flags |= NDT_Dim_option;
        return type;
    case SymbolicDim:
        type->SymbolicDim.flags |= NDT_Dim_option;
        return type;
    case EllipsisDim:
        ndt_err_format(ctx, NDT_InvalidArgumentError,
            "ellipsis dimension cannot be optional");
        ndt_del(type);
        return NULL;
    case Array:
        ndt_err_format(ctx, NDT_InvalidArgumentError,
            "arrays are optional iff the first dimension is optional");
        ndt_del(type);
        return NULL;
    case Option:
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "cannot create an option option");
        ndt_del(type);
    default:
        /* abstract type */
        t = ndt_new(Option, ctx);
        if (t == NULL) {
            ndt_del(type);
            return NULL;
        }
        t->Option.type = type;

        /* concrete access */
        t->access = type->access;
        if (t->access == Concrete) {
            t->Concrete.size = type->Concrete.size;
            t->Concrete.align = type->Concrete.align;
        }
 
        return t;
     }
}

int
ndt_is_optional(const ndt_t *t)
{
    switch (t->tag) {
    case Array:
        return t->Array.flags & NDT_Dim_option;
    case FixedDim:
        return t->FixedDim.flags & NDT_Dim_option;
    case VarDim:
        return t->VarDim.flags & NDT_Dim_option;
    case SymbolicDim:
        return t->SymbolicDim.flags & NDT_Dim_option;
    case EllipsisDim:
        return t->EllipsisDim.flags & NDT_Dim_option;
    case Option:
        return 1;
    default:
        return 0;
    }
}

int
ndt_typedef(const char *name, ndt_t *type, ndt_context_t *ctx)
{
    if (ndt_typedef_add(name, type, ctx) < 0) {
        ndt_del(type);
        return -1;
    }

    return 0;
}

ndt_t *
ndt_nominal(char *name, ndt_context_t *ctx)
{
    const ndt_t *type;
    ndt_t *t;

    type = ndt_typedef_find(name, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    if (ndt_is_abstract(type)) {
        ndt_err_format(ctx, NDT_ValueError,
                       "nominal type must be a concrete type");
        ndt_free(name);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(Nominal, ctx);
    if (t == NULL) {
        ndt_free(name);
        return NULL;
    }
    t->Nominal.name = name;

    /* concrete access */
    t->access = type->access;
    t->Concrete.size = type->Concrete.size;
    t->Concrete.align = type->Concrete.align;

    return t;
}

ndt_t *
ndt_constr(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Constr, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t->Constr.name = name;
    t->Constr.type = type;

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        t->Concrete.size = type->Concrete.size;
        t->Concrete.align = type->Concrete.align;
    }

    return t;
}

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
                     const ndt_field_t *fields, size_t shape,
                     uint16_opt_t align_attr, uint16_opt_t pack,
                     ndt_context_t *ctx)
{
    size_t offset = 0;
    size_t size = 0;
    uint16_t maxalign;
    size_t i;

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
            size_t n = offset;
            offset = round_up(offset, align[i]);
            pad[i-1] = offset - n;
        }

        offsets[i] = offset;
        offset += fields[i].type->Concrete.size;
    }

    size = round_up(offset, maxalign);

    if (shape > 0) {
        pad[shape-1] = (size - offsets[shape-1]) - fields[shape-1].type->Concrete.size;
    }

    assert(t->access == Concrete);
    t->Concrete.align = maxalign;
    t->Concrete.size = size;

    return 0;
}

ndt_t *
ndt_tuple(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
          uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx)
{
    ndt_t *t;
    size_t offset_offset;
    size_t align_offset;
    size_t pad_offset;
    size_t extra;
    int64_t i;

    assert((fields == NULL) == (shape == 0));

    offset_offset = round_up(shape * sizeof(ndt_t *), alignof(int64_t));
    align_offset = offset_offset + shape * sizeof(int64_t);
    pad_offset = align_offset + shape * sizeof(uint16_t);
    extra = pad_offset + shape * sizeof(uint16_t);

    /* abstract type */
    t = ndt_new_extra(Tuple, extra, ctx);
    if (t == NULL) {
        ndt_field_array_del(fields, shape);
        return NULL;
    }
    t->Tuple.flag = flag;
    t->Tuple.shape = shape;
    t->Tuple.types = (ndt_t **)t->extra;

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
        }
        ndt_free(fields);
        return t;
    }
    else {
        t->Concrete.Tuple.offset = (int64_t *)(t->extra + offset_offset);
        t->Concrete.Tuple.align = (uint16_t *)(t->extra + align_offset);
        t->Concrete.Tuple.pad = (uint16_t *)(t->extra + pad_offset);

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
    size_t types_offset;
    size_t offset_offset;
    size_t align_offset;
    size_t pad_offset;
    size_t extra;
    int64_t i;

    assert((fields == NULL) == (shape == 0));

    types_offset = round_up(shape * sizeof(char *), alignof(ndt_t *));
    offset_offset = types_offset + round_up(shape * sizeof(ndt_t *), alignof(int64_t));
    align_offset = offset_offset + shape * sizeof(int64_t);
    pad_offset = align_offset + shape * sizeof(uint16_t);
    extra = pad_offset + shape * sizeof(uint16_t);

    /* abstract type */
    t = ndt_new_extra(Record, extra, ctx);
    if (t == NULL) {
        ndt_field_array_del(fields, shape);
        return NULL;
    }
    t->Record.flag = flag;
    t->Record.shape = shape;
    t->Record.names = (char **)t->extra;
    t->Record.types = (ndt_t **)(t->extra + types_offset);

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
        }
        ndt_free(fields);
        return t;
    }
    else {
        t->Concrete.Record.offset = (int64_t *)(t->extra + offset_offset);
        t->Concrete.Record.align = (uint16_t *)(t->extra + align_offset);
        t->Concrete.Record.pad = (uint16_t *)(t->extra + pad_offset);

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
        }
        ndt_free(fields);
        return t;
    }
}

ndt_t *
ndt_function(ndt_t *ret, ndt_t *pos, ndt_t *kwds, ndt_context_t *ctx)
{
    ndt_t *t;

    /* abstract type */
    t = ndt_new(Function, ctx);
    if (t == NULL) {
        ndt_del(ret);
        ndt_del(pos);
        ndt_del(kwds);
        return NULL;
    }
    t->Function.ret = ret;
    t->Function.pos = pos;
    t->Function.kwds = kwds;

    return t;
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


/******************************************************************************/
/*                                  Scalars                                   */
/******************************************************************************/

ndt_t *
ndt_scalar_kind(ndt_context_t *ctx)
{
    return ndt_new(ScalarKind, ctx);
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
ndt_real_kind(ndt_context_t *ctx)
{
    return ndt_new(RealKind, ctx);
}

ndt_t *
ndt_complex_kind(ndt_context_t *ctx)
{
    return ndt_new(ComplexKind, ctx);
}

ndt_t *
ndt_fixed_bytes_kind(ndt_context_t *ctx)
{
    return ndt_new(FixedBytesKind, ctx);
}

ndt_t *
ndt_fixed_string_kind(ndt_context_t *ctx)
{
    return ndt_new(FixedStringKind, ctx);
}

ndt_t *
ndt_primitive(enum ndt tag, char endian, ndt_context_t *ctx)
{
    ndt_t *t;

    if (endian != 'L' && endian != 'B') {
        ndt_err_format(ctx, NDT_ValueError, "endian must be 'L' or 'B'");
        return NULL;
    }

    /* abstract type */
    t = ndt_new(tag, ctx);
    if (t == NULL) {
        return NULL;
    }

    /* concrete access */
    t->access = Concrete;

    switch(tag) {
    case Void:
        t->Concrete.size = 0;
        t->Concrete.align = 1;
        break;
    case Bool:
        t->Concrete.size = sizeof(bool);
        t->Concrete.align = alignof(bool);
        break;
    case Int8:
        t->Concrete.size = sizeof(int8_t);
        t->Concrete.align = alignof(int8_t);
        break;
    case Int16:
        t->Concrete.size = sizeof(int16_t);
        t->Concrete.align = alignof(int16_t);
        break;
    case Int32:
        t->Concrete.size = sizeof(int32_t);
        t->Concrete.align = alignof(int32_t);
        break;
    case Int64:
        t->Concrete.size = sizeof(int64_t);
        t->Concrete.align = alignof(int64_t);
        break;
    case Uint8:
        t->Concrete.size = sizeof(uint8_t);
        t->Concrete.align = alignof(uint8_t);
        break;
    case Uint16:
        t->Concrete.size = sizeof(uint16_t);
        t->Concrete.align = alignof(uint16_t);
        break;
    case Uint32:
        t->Concrete.size = sizeof(uint32_t);
        t->Concrete.align = alignof(uint32_t);
        break;
    case Uint64:
        t->Concrete.size = sizeof(uint64_t);
        t->Concrete.align = alignof(uint64_t);
        break;
    case Float32:
        t->Concrete.size = sizeof(float);
        t->Concrete.align = alignof(float);
        break;
    case Float64:
        t->Concrete.size = sizeof(double);
        t->Concrete.align = alignof(double);
        break;
    case Complex64:
        t->Concrete.size = sizeof(ndt_complex64_t);
        t->Concrete.align = alignof(ndt_complex64_t);
        break;
    case Complex128:
        t->Concrete.size = sizeof(ndt_complex128_t);
        t->Concrete.align = alignof(ndt_complex128_t);
        break;
    default:
        ndt_err_format(ctx, NDT_ValueError, "invalid tag: '%s'",
                       ndt_tag_as_string(tag));
        ndt_free(t);
        return NULL;
    }

    return t;
}

ndt_t *
ndt_signed(int size, char endian, ndt_context_t *ctx)
{
    switch (size) {
    case  1: return ndt_primitive(Int8, endian, ctx);
    case  2: return ndt_primitive(Int16, endian, ctx);
    case  4: return ndt_primitive(Int32, endian, ctx);
    case  8: return ndt_primitive(Int64, endian, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for signed integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_unsigned(int size, char endian, ndt_context_t *ctx)
{
    switch (size) {
    case  1: return ndt_primitive(Uint8, endian, ctx);
    case  2: return ndt_primitive(Uint16, endian, ctx);
    case  4: return ndt_primitive(Uint32, endian, ctx);
    case  8: return ndt_primitive(Uint64, endian, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for unsigned integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_from_alias(enum ndt_alias tag, char endian, ndt_context_t *ctx)
{
    switch (tag) {
    case Size: return ndt_unsigned(sizeof(size_t), endian, ctx);
    case Intptr: return ndt_signed(sizeof(intptr_t), endian, ctx);
    case Uintptr: return ndt_unsigned(sizeof(uintptr_t), endian, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError, "invalid alias tag");
        return NULL;
    }
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
    t->Concrete.size = ndt_sizeof_encoding(encoding);
    t->Concrete.align = ndt_alignof_encoding(encoding);

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
    t->Concrete.size = sizeof(ndt_sized_string_t);
    t->Concrete.align = alignof(ndt_sized_string_t);

    return t;
}

ndt_t *
ndt_fixed_string(size_t size, enum ndt_encoding encoding, ndt_context_t *ctx)
{
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
    t->Concrete.size = ndt_sizeof_encoding(encoding) * size;
    t->Concrete.align = ndt_alignof_encoding(encoding);

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
    t->Concrete.size = sizeof(ndt_bytes_t);
    t->Concrete.align = alignof(ndt_bytes_t);

    return t;
}

ndt_t *
ndt_fixed_bytes(size_t size, uint16_opt_t align_attr, ndt_context_t *ctx)
{
    ndt_t *t;
    uint16_t align;

    align = get_align(align_attr, 1, ctx);
    if (align == UINT16_MAX) {
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
    t->Concrete.size = size;
    t->Concrete.align = align;

    return t;
}

/* Define a sort order for the typed values in the categorical set. */
static int
cmp(const void *x, const void *y)
{
    const ndt_memory_t *p = (const ndt_memory_t *)x;
    const ndt_memory_t *q = (const ndt_memory_t *)y;

    if (p->t->tag == q->t->tag) {
        return ndt_memory_compare(p, q);
    }
    return p->t->tag - q->t->tag;
}

ndt_t *
ndt_categorical(ndt_memory_t *types, size_t ntypes, ndt_context_t *ctx)
{
    ndt_t *t;
    size_t i;

    qsort(types, ntypes, sizeof *types, cmp);

    for (i = 0; i+1 < ntypes; i++) {
        if (ndt_memory_equal(&types[i], &types[i+1])) {
            ndt_memory_array_del(types, ntypes);
            ndt_err_format(ctx, NDT_ValueError, "duplicate category entries");
            return NULL;
        }
    }

    /* abstract type */
    t = ndt_new(Categorical, ctx);
    if (t == NULL) {
        ndt_memory_array_del(types, ntypes);
        return NULL;
    }
    t->Categorical.ntypes = ntypes;
    t->Categorical.types = types;

    /* concrete access */
    t->access = Concrete;
    t->Concrete.size = sizeof(ndt_memory_t);
    t->Concrete.align = alignof(ndt_memory_t);

    return t;
}

ndt_t *
ndt_pointer(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    /* abstract type */
    t = ndt_new(Pointer, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->Pointer.type = type;

    /* concrete access */
    t->access = Concrete;
    t->Concrete.size = sizeof(void *);
    t->Concrete.align = alignof(void *);

    return t;
}

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
ndt_is_real(const ndt_t *t)
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
    case Complex64: case Complex128:
        return 1;
    default:
        return 0;
    }
}

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

int
ndt_is_array(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim: case SymbolicDim: case VarDim: case EllipsisDim:
        return 1;
    default:
        return 0;
    }
}

int
ndt_is_concrete_array(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim: case VarDim:
        return ndt_is_concrete(t);
    default:
        return 0;
    }
}

/* XXX */
int
ndt_is_column_major(const ndt_t *t)
{
    (void)t;
    return 0;
}

int
ndt_is_contiguous(const ndt_t *t)
{
    (void)t;
    return 1;
}

int
ndt_is_c_contiguous(const ndt_t *t)
{
    (void)t;
    return 1;
}

int
ndt_is_f_contiguous(const ndt_t *t)
{
    (void)t;
    return 0;
}

int
ndt_dims_dtype(ndt_t *dims[NDT_MAX_DIM], ndt_t **dtype, ndt_t *array)
{
    ndt_t *a = array;
    int n = 0;

    assert(array->ndim <= NDT_MAX_DIM);

    while (a->ndim > 0) {
        dims[n++] = a;
        a = ndt_next_dim((ndt_t *)a);
    }

    *dtype = a;

    return n;
}

int
ndt_get_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *array)
{
    const ndt_t *a = array;
    int n = 0;

    assert(array->ndim <= NDT_MAX_DIM);

    while (a->ndim > 0) {
        dims[n++] = a;
        a = ndt_next_dim((ndt_t *)a);
    }

    *dtype = a;

    return n;
}

/* XXX: Semantics are not clear: Anything that is not a compound type?
        What about pointers? Should it be application specific? */
int
ndt_is_scalar(const ndt_t *t)
{
    switch (t->tag) {
    case Void: case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex64: case Complex128:
    case String:
    case FixedString:
    case FixedBytes:
        return 1;
    default:
        return 0;
    }
}


/**********************************************************************/
/*                            memory type                             */
/**********************************************************************/

/* Return a new ndt memory buffer. Input types are restricted. */
ndt_memory_t *
ndt_memory_from_number(char *v, ndt_t *t, ndt_context_t *ctx)
{
    ndt_memory_t *mem;

    if (v[0] == '-' && ndt_is_unsigned(t)) {
        ndt_err_format(ctx, NDT_ValueError,
                       "expected unsigned value, got: '%s'", ndt_tag_as_string(t->tag));
        ndt_free(v);
        ndt_del(t);
        return NULL;
    }

    mem = ndt_alloc(1, sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        ndt_del(t);
        return ndt_memory_error(ctx);
    }

    switch (t->tag) {
    case Bool:
         mem->v.Bool = ndt_strtobool(v, ctx); break;
    case Int8:
        mem->v.Int8 = (int8_t)ndt_strtol(v, INT8_MIN, INT8_MAX, ctx); break;
    case Int16:
        mem->v.Int16 = (int16_t)ndt_strtol(v, INT16_MIN, INT16_MAX, ctx); break;
    case Int32:
        mem->v.Int32 = (int32_t)ndt_strtol(v, INT32_MIN, INT32_MAX, ctx); break;
    case Int64:
        mem->v.Int64 = (int64_t)ndt_strtoll(v, INT64_MIN, INT64_MAX, ctx); break;
    case Uint8:
        mem->v.Uint8 = (uint8_t)ndt_strtoul(v, UINT8_MAX, ctx); break;
    case Uint16:
        mem->v.Uint16 = (uint16_t)ndt_strtoul(v, UINT16_MAX, ctx); break;
    case Uint32:
        mem->v.Uint32 = (uint32_t)ndt_strtoul(v, UINT32_MAX, ctx); break;
    case Uint64:
        mem->v.Uint64 = (uint64_t)ndt_strtoull(v, UINT64_MAX, ctx); break;
    case Float32:
        mem->v.Float32 = ndt_strtof(v, ctx); break;
    case Float64:
        mem->v.Float64 = ndt_strtod(v, ctx); break;
    default:
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "expected number type, got: '%s'", ndt_tag_as_string(t->tag));
        break;
    }

    ndt_free(v);
    if (ctx->err != NDT_Success) {
        ndt_free(mem);
        ndt_del(t);
        return NULL;
    }
    mem->t = t;

    return mem;
}

/* Return a new ndt memory buffer. The input type is 'string'. */
ndt_memory_t *
ndt_memory_from_string(char *v, ndt_t *t, ndt_context_t *ctx)
{
    ndt_memory_t *mem;

    if (t->tag != String) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "expected string type, got: '%s'", ndt_tag_as_string(t->tag));
        ndt_free(v);
        ndt_del(t);
        return NULL;
    }

    mem = ndt_alloc(1, sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        ndt_del(t);
        return ndt_memory_error(ctx);
    }

    /* XXX: check utf8 */
    mem->v.String = v;
    mem->t = t;

    return mem;
}

int
ndt_memory_equal(const ndt_memory_t *x, const ndt_memory_t *y)
{
    if (x->t->tag != y->t->tag) {
        return 0;
    }

    switch(x->t->tag) {
    case Bool:
        return x->v.Bool == y->v.Bool;
    case Int8:
        return x->v.Int8 == y->v.Int8;
    case Int16:
        return x->v.Int16 == y->v.Int16;
    case Int32:
        return x->v.Int32 == y->v.Int32;
    case Int64:
        return x->v.Int64 == y->v.Int64;
    case Uint8:
        return x->v.Uint8 == y->v.Uint8;
    case Uint16:
        return x->v.Uint16 == y->v.Uint16;
    case Uint32:
        return x->v.Uint32 == y->v.Uint32;
    case Uint64:
        return x->v.Uint64 == y->v.Uint64;
    case Float32:
        return x->v.Float32 == y->v.Float32;
    case Float64:
        return x->v.Float64 == y->v.Float64;
    case String:
        return strcmp(x->v.String, y->v.String) == 0;
    default:
        return 0;
    }
}

int
ndt_memory_compare(const ndt_memory_t *x, const ndt_memory_t *y)
{
    assert(x->t->tag == y->t->tag);

    switch(x->t->tag) {
    case Bool:
        return x->v.Bool < y->v.Bool ? -1 : x->v.Bool != y->v.Bool;
    case Int8:
        return x->v.Int8 < y->v.Int8 ? -1 : x->v.Int8 != y->v.Int8;
    case Int16:
        return x->v.Int16 < y->v.Int16 ? -1 : x->v.Int16 != y->v.Int16;
    case Int32:
        return x->v.Int32 < y->v.Int32 ? -1 : x->v.Int32 != y->v.Int32;
    case Int64:
        return x->v.Int64 < y->v.Int64 ? -1 : x->v.Int64 != y->v.Int64;
    case Uint8:
        return x->v.Uint8 < y->v.Uint8 ? -1 : x->v.Uint8 != y->v.Uint8;
    case Uint16:
        return x->v.Uint16 < y->v.Uint16 ? -1 : x->v.Uint16 != y->v.Uint16;
    case Uint32:
        return x->v.Uint32 < y->v.Uint32 ? -1 : x->v.Uint32 != y->v.Uint32;
    case Uint64:
        return x->v.Uint64 < y->v.Uint64 ? -1 : x->v.Uint64 != y->v.Uint64;
    case Float32:
        return x->v.Float32 < y->v.Float32 ? -1 : x->v.Float32 != y->v.Float32;
    case Float64:
        return x->v.Float64 < y->v.Float64 ? -1 : x->v.Float64 != y->v.Float64;
    case String:
        return strcmp(x->v.String, y->v.String);
    default:
        abort(); /* NOT REACHED */
    }
}


/******************************************************************************/
/*                              String conversion                             */
/******************************************************************************/

bool
ndt_strtobool(const char *v, ndt_context_t *ctx)
{
    if (strcmp(v, "true") == 0) {
        return 1;
    }
    else if (strcmp(v, "false") == 0) {
        return 0;
    }
    else {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "valid values for bool are 'true' or 'false'");
        return 0;
    }
}

char
ndt_strtochar(const char *v, ndt_context_t *ctx)
{
    if (strlen(v) == 1) {
        return v[0];
    }
    else {
        ndt_err_format(ctx, NDT_InvalidArgumentError, "invalid char");
        return 0;
    }
}

long
ndt_strtol(const char *v, long min, long max, ndt_context_t *ctx)
{
    char *endptr;
    long ld;

    errno = 0;
    ld = strtol(v, &endptr, 10);

    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid integer: '%s'", v);
    }
    if (errno == ERANGE || ld < min || ld > max) {
        ndt_err_format(ctx, NDT_ValueError,
                       "out of range: '%s'", v);
    }

    return ld;
}

long long
ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx)
{
    char *endptr;
    long long lld;

    errno = 0;
    lld = strtoll(v, &endptr, 10);
    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid integer: '%s'", v);
    }
    if (errno == ERANGE || lld < min || lld > max) {
        ndt_err_format(ctx, NDT_ValueError,
                       "out of range: '%s'", v);
    }

    return lld;
}

unsigned long
ndt_strtoul(const char *v, unsigned long max, ndt_context_t *ctx)
{
    char *endptr;
    unsigned long lu;

    errno = 0;
    lu = strtoul(v, &endptr, 10);
    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid integer: '%s'", v);
    }
    if (errno == ERANGE || lu > max) {
        ndt_err_format(ctx, NDT_ValueError,
                       "out of range: '%s'", v);
    }

    return lu;
}

unsigned long long
ndt_strtoull(const char *v, unsigned long long max, ndt_context_t *ctx)
{
    char *endptr;
    unsigned long long llu;

    errno = 0;
    llu = strtoull(v, &endptr, 10);
    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid integer: '%s'", v);
    }
    if (errno == ERANGE || llu > max) {
        ndt_err_format(ctx, NDT_ValueError,
                       "out of range: '%s'", v);
    }

    return llu;
}

/* Read a float.  Overflow or underflow is an error. */
float
ndt_strtof(const char *v, ndt_context_t *ctx)
{
    char *endptr;
    float f;

    errno = 0;
    f = strtof(v, &endptr);
    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid float: '%s'", v);
    }
    if (errno == ERANGE) {
        ndt_err_format(ctx, NDT_ValueError,
                       "%s: '%s'", f == 0 ? "underflow" : "overflow", v);
    }

    return f;
}

/* Read a double.  Overflow or underflow is an error. */
double
ndt_strtod(const char *v, ndt_context_t *ctx)
{
    char *endptr;
    double d;

    errno = 0;
    d = strtod(v, &endptr);
    if (*v == '\0' || *endptr != '\0') {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "invalid double: '%s'", v);
    }
    if (errno == ERANGE) {
        ndt_err_format(ctx, NDT_ValueError,
                       "%s: '%s'", d == 0 ? "underflow" : "overflow", v);
    }

    return d;
}


/******************************************************************************/
/*                                  Context                                   */
/******************************************************************************/

ndt_context_t *
ndt_context_new(void)
{
    ndt_context_t *ctx;

    ctx = ndt_alloc(1, sizeof *ctx);
    if (ctx == NULL) {
        return NULL;
    }

    ctx->err = NDT_Success;
    ctx->msg = ConstMsg;
    ctx->ConstMsg = "Success";

    return ctx;
}

void
ndt_context_del(ndt_context_t *ctx)
{
    if (ctx) {
        if (ctx->msg == DynamicMsg) {
            ndt_free(ctx->DynamicMsg);
        }
        ndt_free(ctx);
    }
}

const char *
ndt_context_msg(ndt_context_t *ctx)
{
    if (ctx->msg == ConstMsg) {
        return ctx->ConstMsg;
    }
    return ctx->DynamicMsg;
}

void
ndt_err_clear(ndt_context_t *ctx)
{
    ctx->err = NDT_Success;
    if (ctx->msg == DynamicMsg) {
        ndt_free(ctx->DynamicMsg);
        ctx->msg = ConstMsg;
        ctx->ConstMsg = "Success";
    }
}

void
ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...)
{
    va_list ap, aq;
    char *s;
    int n;

    ndt_err_clear(ctx);
    ctx->err = err;

    va_start(ap, fmt);
    va_copy(aq, ap);

    n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0 || n == INT_MAX) {
        ctx->msg = ConstMsg;
        ctx->ConstMsg = \
           "internal error during the handling of the original error";
        return;
    }

    s = ndt_alloc(1, n+1);
    if (s == NULL) {
        va_end(aq);
        ctx->err = NDT_MemoryError;
        ctx->msg = ConstMsg;
        ctx->ConstMsg = "out of memory";
        return;
    }

    n = vsnprintf(s, n+1, fmt, aq);
    va_end(aq);
    if (n < 0) {
        ndt_free(s);
        ctx->msg = ConstMsg;
        ctx->ConstMsg = \
            "internal error during the handling of the original error";
        return;
    }

    ctx->msg = DynamicMsg;
    ctx->DynamicMsg = s;
}

char *
ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...)
{
    va_list ap, aq;
    char *s;
    int n;

    va_start(ap, fmt);
    va_copy(aq, ap);

    n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0 || n == INT_MAX) {
        ndt_err_format(ctx, NDT_RuntimeError, "vsprintf failed");
        return NULL;
    }

    s = ndt_alloc(1, n+1);
    if (s == NULL) {
        va_end(aq);
        return ndt_memory_error(ctx);
    }

    n = vsnprintf(s, n+1, fmt, aq);
    va_end(aq);
    if (n < 0) {
        ndt_free(s);
        return NULL;
    }

    return s;
}

const char *
ndt_err_as_string(enum ndt_error err)
{
    switch (err) {
    case NDT_Success:
        return "Success";
    case NDT_MemoryError:
        return "MemoryError";
    case NDT_ValueError:
        return "ValueError";
    case NDT_TypeError:
        return "TypeError";
    case NDT_InvalidArgumentError:
        return "InvalidArgumentError";
    case NDT_NotImplementedError:
        return "NotImplementedError";
    case NDT_RuntimeError:
        return "RuntimeError";
    case NDT_LexError:
        return "LexError";
    case NDT_ParseError:
        return "ParseError";
    case NDT_OSError:
        return "OSError";
    default:
        return "UnknownError";
    }
}

void
ndt_err_fprint(FILE *fp, ndt_context_t *ctx)
{
    const char *err = ndt_err_as_string(ctx->err);

    fprintf(fp, "%s: %s\n", err, ndt_context_msg(ctx));
}
