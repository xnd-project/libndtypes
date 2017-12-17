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
#include "slice.h"


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

uint32_t
ndt_subtree_flags(const ndt_t *type)
{
    if (type && type->flags & (NDT_OPTION|NDT_SUBTREE_OPTION)) {
        return NDT_SUBTREE_OPTION;
    }

    return 0;
}

uint32_t
ndt_dim_flags(const ndt_t *type)
{
    uint32_t flags = ndt_subtree_flags(type);
    flags |= (type->flags & NDT_ELLIPSIS);
    return flags;
}


/******************************************************************************/
/*                             Sequence elements                              */
/******************************************************************************/

void
ndt_value_del(ndt_value_t *mem)
{
    if (mem == NULL) {
        return;
    }

    if (mem->tag == ValString) {
        ndt_free(mem->ValString);
    }

    ndt_free(mem);
}

void
ndt_value_array_del(ndt_value_t *mem, size_t ntypes)
{
    size_t i;

    if (mem == NULL) {
        return;
    }

    for (i = 0; i < ntypes; i++) {
        if (mem[i].tag == ValString) {
            ndt_free(mem[i].ValString);
        }
    }

    ndt_free(mem);
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

    case FloatKind: return "FloatKind";
    case Float16: return "float16";
    case Float32: return "float32";
    case Float64: return "float64";

    case ComplexKind: return "ComplexKind";
    case Complex32: return "complex32";
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
    case Ref: return "ref";

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

/* Invariants for all types except for var dimensions. */
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
        ndt_err_format(ctx, NDT_TypeError, "ndim > %u", NDT_MAX_DIM);
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
        ndt_err_format(ctx, NDT_TypeError, "ndim > %u", NDT_MAX_DIM);
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
        ndt_err_format(ctx, NDT_TypeError, "ndim > %u", NDT_MAX_DIM);
        return 0;
    }

    return 1;
}

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
    t->flags = 0;
    t->ndim = 0;
    t->hash = -1;

    t->datasize = -1;
    t->align = UINT16_MAX;

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
    t->flags = 0;
    t->ndim = 0;
    t->hash = -1;

    t->datasize = -1;
    t->align = UINT16_MAX;

    return t;
}

ndt_t *
ndt_tuple_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx)
{
    ndt_t *t;
    size_t offset_offset;
    size_t align_offset;
    size_t pad_offset;
    size_t extra;
    int64_t i;

    offset_offset = round_up(shape * sizeof(ndt_t *), alignof(int64_t));
    align_offset = offset_offset + shape * sizeof(int64_t);
    pad_offset = align_offset + shape * sizeof(uint16_t);
    extra = pad_offset + shape * sizeof(uint16_t);

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
    ndt_t *t;
    size_t types_offset;
    size_t offset_offset;
    size_t align_offset;
    size_t pad_offset;
    size_t extra;
    int64_t i;

    types_offset = round_up(shape * sizeof(char *), alignof(ndt_t *));
    offset_offset = types_offset + round_up(shape * sizeof(ndt_t *), alignof(int64_t));
    align_offset = offset_offset + shape * sizeof(int64_t);
    pad_offset = align_offset + shape * sizeof(uint16_t);
    extra = pad_offset + shape * sizeof(uint16_t);

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
    case Module:
        ndt_free(t->Module.name);
        ndt_del(t->Module.type);
        break;
    case FixedDim:
        ndt_del(t->FixedDim.type);
        break;
    case VarDim:
        ndt_del(t->VarDim.type);
        if (ndt_is_concrete(t)) {
            if (t->Concrete.VarDim.flag == InternalOffsets) {
                ndt_free((int32_t *)t->Concrete.VarDim.offsets);
            }
            ndt_free(t->Concrete.VarDim.slices);
        }
        break;
    case SymbolicDim:
        ndt_free(t->SymbolicDim.name);
        ndt_del(t->SymbolicDim.type);
        break;
    case EllipsisDim:
        ndt_free(t->EllipsisDim.name);
        ndt_del(t->EllipsisDim.type);
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
        ndt_value_array_del(t->Categorical.types, t->Categorical.ntypes);
        break;
    case Ref:
        ndt_del(t->Ref.type);
        break;
    default:
        break;
    }

    ndt_free(t);
}

/* Unoptimized hash function for experimenting. */
int64_t
ndt_hash(ndt_t *t, ndt_context_t *ctx)
{
    unsigned char *s, *cp;
    size_t len;
    int64_t x;

    if (t->hash != -1) {
        return t->hash;
    }

    cp = s = (unsigned char *)ndt_as_string(t, ctx);
    if (s == NULL) {
        return -1;
    }

    len = strlen((char *)s);

    x = *cp << 7;
    while (*cp != '\0') {
        x = (1000003 * x) ^ *cp++;
    }
    x ^= len;

    if (x == -1) {
        x = -2;
    }

    ndt_free(s);
    t->hash = x;

    return x;
}

ndt_t *
ndt_any_kind(ndt_context_t *ctx)
{
    return ndt_new(AnyKind, ctx);
}

int
ndt_is_ndarray(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim:
        return 1;
    default:
        return 0;
    }
}

ndt_t *
_ndt_to_fortran(const ndt_t *t, int64_t step, ndt_context_t *ctx)
{
    ndt_t *dt;

    assert(ndt_is_concrete(t));
    if (t->ndim == 0) {
        return ndt_copy(t, ctx);
    }

    assert(t->tag == FixedDim);
    dt = _ndt_to_fortran(t->FixedDim.type, step * t->FixedDim.shape, ctx);
    if (dt == NULL) {
        return NULL;
    }

    return ndt_fixed_dim(dt, t->FixedDim.shape, step, ctx);
}

/* Return a copy of a C-contiguous array in Fortran order. */
ndt_t *
ndt_to_fortran(const ndt_t *t, ndt_context_t *ctx)
{
    if (!ndt_is_ndarray(t)) {
        ndt_err_format(ctx, NDT_TypeError,
            "ndt_to_fortran: function expects an ndarray");
        return NULL;
    }

    if (ndt_is_abstract(t)) {
        return ndt_copy(t, ctx);
    }

    return _ndt_to_fortran(t, 1, ctx);
}

int64_t
ndt_itemsize(ndt_t *t)
{
    switch (t->tag) {
    case FixedDim:
        return t->Concrete.FixedDim.itemsize;
    case VarDim:
        return t->Concrete.VarDim.itemsize;
    default:
        return t->datasize;
    }
}

static int64_t
fixed_step(ndt_t *type)
{
    assert(type->tag != VarDim);

    switch (type->tag) {
    case FixedDim:
        return type->FixedDim.shape * type->Concrete.FixedDim.step;
    default:
        return 1;
    }
}

ndt_t *
ndt_fixed_dim(ndt_t *type, int64_t shape, int64_t step, ndt_context_t *ctx)
{
    ndt_t *t;

    if (!check_type_invariants(type, ctx)) {
        ndt_del(type);
        return NULL;
    }

    /* abstract type */
    t = ndt_new(FixedDim, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->FixedDim.shape = shape;
    t->FixedDim.type = type;
    t->ndim = type->ndim + 1;
    t->flags = ndt_dim_flags(type);

    t->Concrete.FixedDim.itemsize = -1;
    t->Concrete.FixedDim.step = INT64_MAX;

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        t->Concrete.FixedDim.itemsize = ndt_itemsize(type);
        t->Concrete.FixedDim.step = step==INT64_MAX ? fixed_step(type) : step;
        t->datasize = shape * type->datasize;
        t->align = type->align;
    }

    return t;
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
    t->SymbolicDim.name = name;
    t->SymbolicDim.type = type;
    t->ndim = type->ndim + 1;
    t->flags =  ndt_dim_flags(type);

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
    t->Concrete.VarDim.itemsize = -1;
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
                int32_t index)
{
    int64_t list_start, list_stop, list_shape;
    int64_t start, stop, step;
    int64_t res_shape;
    const ndt_slice_t *slices;
    int32_t i;

    assert(ndt_is_concrete(t));
    assert(t->tag == VarDim);
    assert(0 <= index && index+1 < t->Concrete.VarDim.noffsets);

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
        datasize = offsets[noffsets-1] * type->datasize;
        itemsize = type->datasize;
        break;
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
ndt_ellipsis_dim(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    uint32_t flags;

    if (!check_type_invariants(type, ctx)) {
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
    t->EllipsisDim.name = name;
    t->EllipsisDim.type = type;
    t->flags = flags | NDT_ELLIPSIS;
    t->ndim = type->ndim + 1;

    return t;
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

ndt_t *
ndt_option(ndt_t *t)
{
    t->flags |= NDT_OPTION;
    return t;
}

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
    t->flags = ndt_subtree_flags(type);
    t->datasize = type->datasize;
    t->align = type->align;

    return t;
}

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

    /* concrete access */
    t->access = type->access;
    if (t->access == Concrete) {
        t->datasize = type->datasize;
        t->align = type->align;
    }

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
            pad[i-1] = (uint16_t)(offset - n);
        }

        offsets[i] = offset;
        offset += fields[i].type->datasize;
    }

    size = round_up(offset, maxalign);

    if (shape > 0) {
        size_t n = (size - offsets[shape-1]) - fields[shape-1].type->datasize;
        pad[shape-1] = (uint16_t)n;
    }

    assert(t->access == Concrete);
    t->align = maxalign;
    t->datasize = size;

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

    t->flags |= ndt_subtree_flags(ret);
    t->flags |= ndt_subtree_flags(pos);
    t->flags |= ndt_subtree_flags(kwds);

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
        t->datasize = 0;
        t->align = 1;
        break;
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
    t->datasize = ndt_sizeof_encoding(encoding);
    t->align = ndt_alignof_encoding(encoding);

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
    t->datasize = ndt_sizeof_encoding(encoding) * size;
    t->align = ndt_alignof_encoding(encoding);

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
    t->datasize = size;
    t->align = align;

    return t;
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
ndt_categorical(ndt_value_t *types, size_t ntypes, ndt_context_t *ctx)
{
    ndt_value_t *tmp;
    ndt_t *t;
    size_t i;

    tmp = ndt_alloc(ntypes, sizeof(ndt_value_t));
    if (tmp == NULL) {
        ndt_value_array_del(types, ntypes);
        return ndt_memory_error(ctx);
    }

    memcpy(tmp, types, ntypes * sizeof(ndt_value_t));
    qsort(tmp, ntypes, sizeof *tmp, cmp);

    for (i = 0; i+1 < ntypes; i++) {
        if (ndt_value_equal(&tmp[i], &tmp[i+1])) {
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

int
ndt_is_c_contiguous(const ndt_t *t)
{
    const ndt_t *dims[NDT_MAX_DIM];
    const ndt_t *dtype;
    int64_t shape, step;
    int ndim, i;

    if (!ndt_is_ndarray(t)) {
        return 0;
    }
    if (!ndt_is_concrete(t)) {
        return 0;
    }

    ndim = ndt_const_dims_dtype(dims, &dtype, t);

    step = 1;
    for (i = ndim-1; i >= 0; i--) {
        shape = t->FixedDim.shape;
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

    if (!ndt_is_ndarray(t)) {
        return 0;
    }
    if (!ndt_is_concrete(t)) {
        return 0;
    }

    ndim = ndt_const_dims_dtype(dims, &dtype, t);
    step = 1;
    for (i = 0; i < ndim; i++) {
        shape = t->FixedDim.shape;
        if (shape > 1 && dims[i]->Concrete.FixedDim.step != step) {
            return 0;
        }
        step *= shape;
    }

    return 1;
}

int
ndt_as_ndarray(ndt_ndarray_t *a, const ndt_t *t, ndt_context_t *ctx)
{
    int ndim;

    assert(t->ndim <= NDT_MAX_DIM);

    if (!ndt_is_concrete(t)) {
        ndt_err_format(ctx, NDT_ValueError,
            "ndt_as_ndarray: type must be concrete");
        return -1;
    }

    if (!ndt_is_ndarray(t)) {
        if (t->ndim == 0) {
            a->ndim = t->ndim;
            a->itemsize = t->datasize;
            return 0;
        }
        ndt_err_format(ctx, NDT_ValueError,
            "ndt_as_ndarray: type must be an ndarray");
        return -1;
    }

    a->ndim = t->ndim;
    a->itemsize = t->Concrete.FixedDim.itemsize;

    for (ndim=0; t->ndim > 0; ndim++, t=t->FixedDim.type) {
        a->shape[ndim] = t->FixedDim.shape;
        a->strides[ndim] = t->Concrete.FixedDim.step * a->itemsize;
    }

    return 0;
}

const ndt_t *
ndt_dtype(const ndt_t *t)
{
    while (t->ndim > 0) {
        t = ndt_next_dim((ndt_t *)t);
    }

    return t;
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
ndt_const_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *array)
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
        What about refs? Should it be application specific? */
int
ndt_is_scalar(const ndt_t *t)
{
    switch (t->tag) {
    case Void: case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
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
ndt_value_t *
ndt_value_from_number(enum ndt_value tag, char *v, ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc(1, sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        return ndt_memory_error(ctx);
    }
    mem->tag = tag;

    switch (tag) {
    case ValBool:
        mem->ValBool = ndt_strtobool(v, ctx); break;
    case ValInt64:
        mem->ValInt64 = (int64_t)ndt_strtoll(v, INT64_MIN, INT64_MAX, ctx); break;
    case ValFloat64:
        mem->ValFloat64 = ndt_strtod(v, ctx); break;
    default:
        ndt_err_format(ctx, NDT_InvalidArgumentError, "expected number tag");
        break;
    }

    ndt_free(v);
    if (ndt_err_occurred(ctx)) {
        ndt_free(mem);
        return NULL;
    }

    return mem;
}

/* Return a new ndt memory buffer. The input type is 'string'. */
ndt_value_t *
ndt_value_from_string(char *v, ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc(1, sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        return ndt_memory_error(ctx);
    }

    /* XXX: check utf8 */
    mem->tag = ValString;
    mem->ValString = v;

    return mem;
}

/* Return a new ndt memory buffer. The input type is 'NA'. */
ndt_value_t *
ndt_value_na(ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc(1, sizeof *mem);
    if (mem == NULL) {
        return ndt_memory_error(ctx);
    }
    mem->tag = ValNA;

    return mem;
}

int
ndt_value_equal(const ndt_value_t *x, const ndt_value_t *y)
{
    if (x->tag != y->tag) {
        return 0;
    }

    switch(x->tag) {
    case ValBool:
        return x->ValBool == y->ValBool;
    case ValInt64:
        return x->ValInt64 == y->ValInt64;
    case ValFloat64:
        return x->ValFloat64 == y->ValFloat64;
    case ValString:
        return strcmp(x->ValString, y->ValString) == 0;
    case ValNA:
        return 1;
    }

    abort(); /* NOT REACHED: tags are exhaustive. */
}

int
ndt_value_compare(const ndt_value_t *x, const ndt_value_t *y)
{
    if (x->tag != y->tag) {
        return x->tag - y->tag;
    }

    switch(x->tag) {
    case ValBool:
        return x->ValBool < y->ValBool ? -1 : x->ValBool != y->ValBool;
    case ValInt64:
        return x->ValInt64 < y->ValInt64 ? -1 : x->ValInt64 != y->ValInt64;
    case ValFloat64:
        return x->ValFloat64 < y->ValFloat64 ? -1 : x->ValFloat64 != y->ValFloat64;
    case ValString:
        return strcmp(x->ValString, y->ValString);
    case ValNA: /* XXX */
        return 0;
    }

    abort(); /* NOT REACHED: tags are exhaustive. */
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

    ctx->flags = NDT_Dynamic;
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
        if (ctx->flags & NDT_Dynamic) {
            ndt_free(ctx);
        }
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

int
ndt_err_occurred(const ndt_context_t *ctx)
{
    return ctx->err != NDT_Success;
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
