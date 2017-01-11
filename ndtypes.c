/*
 * Copyright (c) 2016, Continuum Analytics, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * 
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
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


char *
ndt_strdup(const char *s, ndt_context_t *ctx)
{
    size_t len = strlen(s);
    char *cp;

    cp = ndt_alloc(1, len+1);
    if (cp == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    memcpy(cp, s, len);
    cp[len] = '\0';
    return cp;
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

ndt_tuple_field_t *
ndt_tuple_field(ndt_t *type, ndt_context_t *ctx)
{
    ndt_tuple_field_t *field;

    if (type == NULL) {
        return NULL;
    }
    
    field = ndt_alloc(1, sizeof *field);
    if (field == NULL) {
        ndt_del(type);
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    field->type = type;
    field->offset = 0;
    field->align = 1;
    field->pad = 0;

    return field;
}

void
ndt_tuple_field_del(ndt_tuple_field_t *field)
{
    if (field) {
        ndt_del(field->type);
        ndt_free(field);
    }
}

void
ndt_tuple_field_array_del(ndt_tuple_field_t *fields, size_t shape)
{
    size_t i;

    if (fields == NULL) {
        return;
    }

    for (i = 0; i < shape; i++) {
        ndt_del(fields[i].type);
    }

    ndt_free(fields);
}

ndt_record_field_t *
ndt_record_field(char *name, ndt_t *type, ndt_context_t *ctx)
{
    ndt_record_field_t *field;

    field = ndt_alloc(1, sizeof *field);
    if (field == NULL) {
        ndt_free(name);
        ndt_del(type);
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    field->name = name;
    field->type = type;
    field->offset = 0;
    field->align = type->align;
    field->pad = 0;

    return field;
}

void
ndt_record_field_del(ndt_record_field_t *field)
{
    if (field) {
        ndt_free(field->name);
        ndt_del(field->type);
        ndt_free(field);
    }
}

void
ndt_record_field_array_del(ndt_record_field_t *fields, size_t shape)
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

ndt_dim_t *
ndt_dim_new(enum ndt_dim tag, ndt_context_t *ctx)
{
    ndt_dim_t *d;

    d = ndt_alloc(1, sizeof *d);
    if (d == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    d->tag = tag;
    d->itemsize = 0;
    d->itemalign = 1;
    d->abstract = 1;

    return d;
}

void
ndt_dim_del(ndt_dim_t *dim)
{
    if (dim == NULL) {
        return;
    }

    switch (dim->tag) {
    case SymbolicDim:
        ndt_free(dim->SymbolicDim.name);
        break;
    default:
        break;
    }

    ndt_free(dim);
}

void
ndt_dim_array_del(ndt_dim_t *dim, size_t shape)
{
    size_t i;

    if (dim == NULL) {
        return;
    }

    for (i = 0; i < shape; i++) {
        switch (dim[i].tag) {
        case SymbolicDim:
            ndt_free(dim[i].SymbolicDim.name);
            break;
        default:
            break;
        }
    }

    ndt_free(dim);
}


/******************************************************************************/
/*                                   Tags                                     */
/******************************************************************************/

const char *
ndt_tag_as_string(enum ndt tag)
{
    switch (tag) {
    case AnyKind: return "Any";
    case Array: return "array";
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

uint8_t
ndt_alignof_encoding(enum ndt_encoding encoding)
{
    return (uint8_t)ndt_sizeof_encoding(encoding);
}


/******************************************************************************/
/*                                 Dimensions                                 */
/******************************************************************************/

ndt_dim_t *
ndt_fixed_dim_kind(ndt_context_t *ctx)
{
    return ndt_dim_new(FixedDimKind, ctx);
}

ndt_dim_t *
ndt_fixed_dim(size_t shape, ndt_context_t *ctx)
{
    ndt_dim_t *d;

    d = ndt_dim_new(FixedDim, ctx);
    if (d == NULL) {
        return NULL;
    }
    d->FixedDim.shape = shape;
    d->FixedDim.stride = 0;
    d->itemsize = 0;
    d->itemalign = 1;

    return d;
}

ndt_dim_t *
ndt_var_dim(ndt_context_t *ctx)
{
    ndt_dim_t *d;

    d = ndt_dim_new(VarDim, ctx);
    if (d == NULL) {
        return NULL;
    }

    return d;
}

ndt_dim_t *
ndt_symbolic_dim(char *name, ndt_context_t *ctx)
{
    ndt_dim_t *d;

    d = ndt_dim_new(SymbolicDim, ctx);
    if (d == NULL) {
        ndt_free(name);
        return NULL;
    }
    d->SymbolicDim.name = name;

    return d;
}

ndt_dim_t *
ndt_ellipsis_dim(ndt_context_t *ctx)
{
    return ndt_dim_new(EllipsisDim, ctx);
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
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    t->tag = tag;
    t->size = 0;
    t->align = 1;
    t->abstract = 1;

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
        ndt_dim_array_del(t->Array.dim, t->Array.ndim);
        ndt_del(t->Array.dtype);
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
    case Tuple:
        ndt_tuple_field_array_del(t->Tuple.fields, t->Tuple.shape);
        break;
    case Record:
        ndt_record_field_array_del(t->Record.fields, t->Record.shape);
        break;
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

static int
init_dimensions(size_t *size, uint8_t *itemalign, bool *abstract,
                ndt_dim_t *dim, size_t ndim, ndt_t *dtype, ndt_context_t *ctx)
{
    int ellipsis_count = 0;
    size_t shape, i;

    *size = dtype->size;
    *itemalign = dtype->align;
    *abstract = dtype->abstract;

    for (i = ndim-1; i != SIZE_MAX; i--) {
        switch (dim[i].tag) {
        case FixedDim:
            shape = dim[i].FixedDim.shape;
            dim[i].FixedDim.stride = shape <= 1 ? 0 : *size;
            dim[i].itemsize = *size;
            *size *= shape;
            dim[i].itemalign = *itemalign;
            dim[i].abstract = 0;
            break;
        case VarDim:
            dim[i].VarDim.stride = *size;
            dim[i].itemsize = *size;
            *itemalign = dim[i].itemalign;
            dim[i].abstract = 0;
            break;
        case FixedDimKind: case SymbolicDim:
            *abstract = 1;
            break;
        case EllipsisDim:
            *abstract = 1;
            if (++ellipsis_count > 1) {
                ndt_err_format(ctx, NDT_ValueError,
                               "more than one ellipsis dimension");
                return -1;
            }
            break;
        }
    }

    return 0;
}

ndt_t *
ndt_array(ndt_dim_t *dim, size_t ndim, ndt_t *dtype, ndt_context_t *ctx)
{
    ndt_t *t;
    size_t size;
    uint8_t align;
    bool abstract;

    if (init_dimensions(&size, &align, &abstract, dim, ndim, dtype, ctx) < 0) {
        ndt_dim_array_del(dim, ndim);
        ndt_del(dtype);
        return NULL;
    }

    t = ndt_new(Array, ctx);
    if (t == NULL) {
        ndt_dim_array_del(dim, ndim);
        ndt_del(dtype);
        return NULL;
    }
    t->Array.ndim = ndim;
    t->Array.dim = dim;
    t->Array.dtype = dtype;
    t->size = size;
    t->align = align;
    t->abstract = abstract;

    return t;
}

ndt_t *
ndt_option(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Option, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->Option.type = type;
    t->size = type->size;
    t->align = type->align;
    t->abstract = type->abstract;

    return t;
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

    t = ndt_new(Nominal, ctx);
    if (t == NULL) {
        ndt_free(name);
        return NULL;
    }
    t->Nominal.name = name;
    t->size = type->size;
    t->align = type->align;
    t->abstract = type->abstract;

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
    t->Constr.name = name;
    t->Constr.type = type;
    t->size = type->size;
    t->align = type->align;
    t->abstract = type->abstract;

    return t;
}

#undef max
static uint8_t
max(uint8_t x, uint8_t y)
{
    return x >= y ? x : y;
}

static size_t
round_up(size_t offset, uint8_t align)
{
    return ((offset + align - 1) / align) * align;
}

static void
init_tuple(ndt_t *t, enum ndt_variadic_flag flag, ndt_tuple_field_t *fields,
           size_t shape)
{
    size_t offset = 0;
    uint8_t maxalign = 1;
    size_t size;
    size_t i;
    bool abstract = 0;

    for (i = 0; i < shape; i++) {
        offset = round_up(offset, fields[i].type->align);
        maxalign = max(fields[i].type->align, maxalign);
        fields[i].offset = offset;
        offset += fields[i].type->size;
        if (fields[i].type->abstract) {
            abstract = 1;
        }
    }

    size = round_up(offset, maxalign);

    for (i = 0; i+1 < shape; i++) {
        size_t pad = (fields[i+1].offset-fields[i].offset)-fields[i].type->size;
        fields[i].pad = (uint8_t)pad;
    }

    if (shape) {
        size_t pad = (size - fields[i].offset) - fields[i].type->size;
        fields[i].pad = pad;
    }

    t->Tuple.flag = flag;
    t->Tuple.fields = fields;
    t->Tuple.shape = shape;
    t->size = size;
    t->align = maxalign;
    t->abstract = abstract || flag == Variadic;
}

ndt_t *
ndt_tuple(enum ndt_variadic_flag flag, ndt_tuple_field_t *fields, size_t shape,
          ndt_context_t *ctx)
{
    ndt_t *t;

    assert((fields == NULL) == (shape == 0));

    t = ndt_new(Tuple, ctx);
    if (t == NULL) {
        ndt_tuple_field_array_del(fields, shape);
        return NULL;
    }
    init_tuple(t, flag, fields, shape);

    return t;
}

static void
init_record(ndt_t *t, enum ndt_variadic_flag flag, ndt_record_field_t *fields,
            size_t shape)
{
    size_t offset = 0;
    uint8_t maxalign = 1;
    size_t size = 0;
    size_t i;
    bool abstract = 0;

    for (i = 0; i < shape; i++) {
        offset = round_up(offset, fields[i].type->align);
        maxalign = max(fields[i].type->align, maxalign);
        fields[i].offset = offset;
        offset += fields[i].type->size;
        if (fields[i].type->abstract) {
            abstract = 1;
        }
    }

    size = round_up(offset, maxalign);

    for (i = 0; i+1 < shape; i++) {
        size_t pad = (fields[i+1].offset-fields[i].offset)-fields[i].type->size;
        fields[i].pad = (uint8_t)pad;
    }

    if (shape) {
        size_t pad = (size - fields[i].offset) - fields[i].type->size;
        fields[i].pad = (uint8_t)pad;
    }

    t->Record.flag = flag;
    t->Record.fields = fields;
    t->Record.shape = shape;
    t->size = size;
    t->align = maxalign;
    t->abstract = abstract || flag == Variadic;
}

ndt_t *
ndt_record(enum ndt_variadic_flag flag, ndt_record_field_t *fields, size_t shape,
           ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Record, ctx);
    if (t == NULL) {
        ndt_record_field_array_del(fields, shape);
        return NULL;
    }
    init_record(t, flag, fields, shape);

    return t;
}

ndt_t *
ndt_function(ndt_t *ret, ndt_t *pos, ndt_t *kwds, ndt_context_t *ctx)
{
    ndt_t *t;

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
    t->size = sizeof(void *);
    t->align = alignof(void *);
    t->abstract = ret->abstract || pos->abstract || kwds->abstract;

    return t;
}

ndt_t *
ndt_typevar(char *name, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Typevar, ctx);
    if (t == NULL) {
        ndt_free(name);
        return NULL;
    }
    t->Typevar.name = name;
    t->size = 0;
    t->align = 1;
    t->abstract = 1;

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
ndt_primitive(enum ndt tag, ndt_context_t *ctx)
{
    ndt_t *t = ndt_new(tag, ctx);
    if (t == NULL) {
        return NULL;
    }

    switch(tag) {
    case Void:
        t->size = 0;
        t->align = 1;
        break;
    case Bool:
        t->size = sizeof(bool);
        t->align = alignof(bool);
        break;
    case Int8:
        t->size = sizeof(int8_t);
        t->align = alignof(int8_t);
        break;
    case Int16:
        t->size = sizeof(int16_t);
        t->align = alignof(int16_t);
        break;
    case Int32:
        t->size = sizeof(int32_t);
        t->align = alignof(int32_t);
        break;
    case Int64:
        t->size = sizeof(int64_t);
        t->align = alignof(int64_t);
        break;
    case Uint8:
        t->size = sizeof(uint8_t);
        t->align = alignof(uint8_t);
        break;
    case Uint16:
        t->size = sizeof(uint16_t);
        t->align = alignof(uint16_t);
        break;
    case Uint32:
        t->size = sizeof(uint32_t);
        t->align = alignof(uint32_t);
        break;
    case Uint64:
        t->size = sizeof(uint64_t);
        t->align = alignof(uint64_t);
        break;
    case Float32:
        t->size = sizeof(float);
        t->align = alignof(float);
        break;
    case Float64:
        t->size = sizeof(double);
        t->align = alignof(double);
        break;
    case Complex64:
        t->size = sizeof(ndt_complex64_t);
        t->align = alignof(ndt_complex64_t);
        break;
    case Complex128:
        t->size = sizeof(ndt_complex128_t);
        t->align = alignof(ndt_complex128_t);
        break;
    default:
        ndt_err_format(ctx, NDT_ValueError, "invalid tag: '%s'",
                       ndt_tag_as_string(tag));
        ndt_free(t);
        return NULL;
    }

    t->abstract = 0;

    return t;
}

ndt_t *
ndt_signed(int size, ndt_context_t *ctx)
{
    switch (size) {
    case  1: return ndt_primitive(Int8, ctx);
    case  2: return ndt_primitive(Int16, ctx);
    case  4: return ndt_primitive(Int32, ctx);
    case  8: return ndt_primitive(Int64, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for signed integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_unsigned(int size, ndt_context_t *ctx)
{
    switch (size) {
    case  1: return ndt_primitive(Uint8, ctx);
    case  2: return ndt_primitive(Uint16, ctx);
    case  4: return ndt_primitive(Uint32, ctx);
    case  8: return ndt_primitive(Uint64, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError,
                       "invalid size for unsigned integer: '%d'", size);
        return NULL;
    }
}

ndt_t *
ndt_from_alias(enum ndt_alias tag, ndt_context_t *ctx)
{
    switch (tag) {
    case Size: return ndt_unsigned(sizeof(size_t), ctx);
    case Intptr: return ndt_signed(sizeof(intptr_t), ctx);
    case Uintptr: return ndt_unsigned(sizeof(uintptr_t), ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError, "invalid alias tag");
        return NULL;
    }
}
     
ndt_t *
ndt_char(enum ndt_encoding encoding, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Char, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Char.encoding = encoding;
    t->size = ndt_sizeof_encoding(encoding);
    t->align = ndt_alignof_encoding(encoding);
    t->abstract = 0;

    return t;
}

ndt_t *
ndt_string(ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(String, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->size = sizeof(ndt_sized_string_t);
    t->align = alignof(ndt_sized_string_t);
    t->abstract = 0;

    return t;
}

ndt_t *
ndt_fixed_string(size_t size, enum ndt_encoding encoding, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(FixedString, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedString.size = size;
    t->FixedString.encoding = encoding;
    t->size = ndt_sizeof_encoding(encoding) * size;
    t->align = ndt_alignof_encoding(encoding);
    t->abstract = 0;

    return t;
}

ndt_t *
ndt_bytes(uint8_t target_align, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Bytes, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Bytes.target_align = target_align;
    t->size = sizeof(ndt_bytes_t);
    t->align = alignof(ndt_bytes_t);
    t->abstract = 0;

    return t;
}

ndt_t *
ndt_fixed_bytes(size_t size, uint8_t align, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(FixedBytes, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedBytes.size = size;
    t->FixedBytes.align = align;
    t->size = size;
    t->align = align;
    t->abstract = 0;

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

    t = ndt_new(Categorical, ctx);
    if (t == NULL) {
        ndt_memory_array_del(types, ntypes);
        return NULL;
    }

    t->Categorical.ntypes = ntypes;
    t->Categorical.types = types;
    t->size = sizeof(ndt_memory_t);
    t->align = alignof(ndt_memory_t);
    t->abstract = 0;

    return t;
}

ndt_t *
ndt_pointer(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;

    t = ndt_new(Pointer, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->Pointer.type = type;
    t->size = sizeof(void *);
    t->align = alignof(void *);
    t->abstract = type->abstract;

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
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        ndt_free(v);
        ndt_del(t);
        return NULL;
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
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        ndt_free(v);
        ndt_del(t);
        return NULL;
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
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
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
