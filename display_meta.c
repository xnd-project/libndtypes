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
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <errno.h>
#include "ndtypes.h"


#undef buf_t
typedef struct {
    size_t count; /* count the required size */
    size_t size;  /* buffer size (0 for the count phase) */
    char *cur;    /* buffer data (NULL for the count phase) */
} buf_t;


static int datashape(buf_t *buf, const ndt_t *t, int d, int cont, ndt_context_t *ctx);


static int
_snprintf(ndt_context_t *ctx, buf_t *buf, const char *fmt, va_list ap)
{
    int n;

    errno = 0;
    n = vsnprintf(buf->cur, buf->size, fmt, ap);
    if (buf->cur && n < 0) {
        if (errno == ENOMEM) {
            ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        }
        else {
            ndt_err_format(ctx, NDT_OSError, "ndt_as_string: output error");
        }
        return -1;
    }

    if (buf->cur && (size_t)n >= buf->size) {
         ndt_err_format(ctx, NDT_ValueError,
                        "ndt_as_string: insufficient buffer size");
        return -1;
    }

    buf->count += n;

    if (buf->cur) {
        buf->cur += n;
        buf->size -= n;
    }

    return 0;
}

static int
ndt_snprintf(ndt_context_t *ctx, buf_t *buf, const char *fmt, ...)
{
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = _snprintf(ctx, buf, fmt, ap);
    va_end(ap);

    return n;
}

static int
indent(ndt_context_t *ctx, buf_t *buf, int n)
{
    int i;

    if (n < 0) {
        return 0;
    }

    for (i = 0; i < n; i++) {
        if (ndt_snprintf(ctx, buf, " ") < 0) {
            return -1;
        }
    }

    return 0;
}

static int
ndt_snprintf_d(ndt_context_t *ctx, buf_t *buf, int d, const char *fmt, ...)
{
    va_list ap;
    int n;

    n = indent(ctx, buf, d);
    if (n < 0) return -1;

    va_start(ap, fmt);
    n = _snprintf(ctx, buf, fmt, ap);
    va_end(ap);

    return n;
}

static int
flags(buf_t *buf, const ndt_t *t, ndt_context_t *ctx)
{
    bool cont = 0;
    int n;

    if (t->flags & NDT_Abstract) {
        n = ndt_snprintf(ctx, buf, "Abstract");
        if (n > 0) return -1;
        cont = 1;
    }
    if (t->flags & NDT_Column_major) {
        n = ndt_snprintf(ctx, buf, "%sColumn_major", cont ? ", " : "");
        if (n > 0) return -1;
        cont = 1;
    }
    if (t->flags & NDT_Contiguous) {
        n = ndt_snprintf(ctx, buf, "%sContiguous", cont ? ", " : "");
        if (n > 0) return -1;
        cont = 1;
    }
    if (t->flags & NDT_Big_endian) {
        n = ndt_snprintf(ctx, buf, "%sBig_endian", cont ? ", " : "");
        if (n > 0) return -1;
    }

    return 0;
}

static int
common_attributes(buf_t *buf, const  ndt_t *t, int d, ndt_context_t *ctx)
{
    int n;

    n = ndt_snprintf_d(ctx, buf, d,
                       "ndim=%" PRIu8 ", size=%zu, align=%" PRIu8 ", flags=[",
                       t->ndim, t->size, t->align);
    if (n < 0) return -1;

    n = flags(buf, t, ctx);
    if (n < 0) return -1;

    return ndt_snprintf(ctx, buf, "]");
}

static int
common_attributes_with_newline(buf_t *buf, const ndt_t *t, int d, ndt_context_t *ctx)
{
    int n;

    n = common_attributes(buf, t, d, ctx);
    if (n < 0) return -1;

    return ndt_snprintf(ctx, buf, "\n");
}

static int
tuple_fields(buf_t *buf, ndt_tuple_field_t *fields, size_t nfields, int d,
             ndt_context_t *ctx)
{
    size_t i;
    int n;

    for (i = 0; i < nfields; i++) {
        if (i >= 1) {
            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;
        }

        n = ndt_snprintf_d(ctx, buf, d, "TupleField(\n");
        if (n < 0) return -1;

        n = ndt_snprintf_d(ctx, buf, d+2, "type=");
        if (n < 0) return -1;

        n = datashape(buf, fields[i].type, d+5+2, 1, ctx);
        if (n < 0) return -1;

        n = ndt_snprintf(ctx, buf, ",\n");
        if (n < 0) return -1;

        n = ndt_snprintf_d(ctx, buf, d+2, "offset=%zu, align=%" PRIu8 ", pad=%" PRIu8 "\n",
                           fields[i].offset, fields[i].align, fields[i].pad);

        n = ndt_snprintf_d(ctx, buf, d, ")");
        if (n < 0) return -1;
    }

    return 0;
}

static int
record_fields(buf_t *buf, ndt_record_field_t *fields, size_t nfields, int d,
              ndt_context_t *ctx)
{
    size_t i;
    int n;

    for (i = 0; i < nfields; i++) {
        if (i >= 1) {
            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;
        }

        n = ndt_snprintf_d(ctx, buf, d, "RecordField(\n", fields[i].name);
        if (n < 0) return -1;

        n = ndt_snprintf_d(ctx, buf, d+2, "name='%s',\n", fields[i].name);
        if (n < 0) return -1;

        n = ndt_snprintf_d(ctx, buf, d+2, "type=");
        if (n < 0) return -1;

        n = datashape(buf, fields[i].type, d+5+2, 1, ctx);
        if (n < 0) return -1;

        n = ndt_snprintf(ctx, buf, ",\n");
        if (n < 0) return -1;

        n = ndt_snprintf_d(ctx, buf, d+2, "offset=%zu, align=%" PRIu8 ", pad=%" PRIu8 "\n",
                           fields[i].offset, fields[i].align, fields[i].pad);

        n = ndt_snprintf_d(ctx, buf, d, ")");
        if (n < 0) return -1;
    }

    return 0;
}

static int
variadic_flag(buf_t *buf, enum ndt_variadic_flag flag, int d, ndt_context_t *ctx)
{
    if (flag == Variadic) {
        return ndt_snprintf_d(ctx, buf, d, "variadic=true,\n");
    }

    return 0;
}

static int
comma_variadic_flag(buf_t *buf, enum ndt_variadic_flag flag, int d, ndt_context_t *ctx)
{
    if (flag == Variadic) {
        int n = ndt_snprintf(ctx, buf, ",\n");
        if (n < 0) return -1;

        return ndt_snprintf_d(ctx, buf, d, "variadic=true");
    }

    return 0;
}

static int
value(buf_t *buf, const ndt_memory_t *mem, ndt_context_t *ctx)
{
    switch (mem->t->tag) {
    case Bool:
        return ndt_snprintf(ctx, buf, mem->v.Bool ? "true" : "false");
    case Int8:
        return ndt_snprintf(ctx, buf, "%" PRIi8, mem->v.Int8);
    case Int16:
        return ndt_snprintf(ctx, buf, "%" PRIi16, mem->v.Int16);
    case Int32:
        return ndt_snprintf(ctx, buf, "%" PRIi32, mem->v.Int32);
    case Int64:
        return ndt_snprintf(ctx, buf, "%" PRIi64, mem->v.Int64);
    case Uint8:
        return ndt_snprintf(ctx, buf, "%" PRIu8, mem->v.Uint8);
    case Uint16:
        return ndt_snprintf(ctx, buf, "%" PRIu16, mem->v.Uint16);
    case Uint32:
        return ndt_snprintf(ctx, buf, "%" PRIu32, mem->v.Uint32);
    case Uint64:
        return ndt_snprintf(ctx, buf, "%" PRIu64, mem->v.Uint64);
    case Float32:
        return ndt_snprintf(ctx, buf, "%g", mem->v.Float32);
    case Float64:
        return ndt_snprintf(ctx, buf, "%g", mem->v.Float64);
    case String:
        return ndt_snprintf(ctx, buf, "'%s'", mem->v.String);
    default:
        ndt_err_format(ctx, NDT_NotImplementedError,
                       "unsupported type: '%s'", ndt_tag_as_string(mem->t->tag));
        return 0;
    }
}

static int
categorical(buf_t *buf, ndt_memory_t *mem, size_t ntypes, int d, ndt_context_t *ctx)
{
    size_t i;
    int n;

    for (i = 0; i < ntypes; i++) {
        if (i >= 1) {
            n = ndt_snprintf(ctx, buf, ", ");
            if (n < 0) return -1;
        }

        n = value(buf, &mem[i], ctx);
        if (n < 0) return -1;

        n = ndt_snprintf(ctx, buf, " : ");
        if (n < 0) return -1;

        n = datashape(buf, mem[i].t, d, 1, ctx);
        if (n < 0) return -1;
    }

    return 0;
}

static const char *
tag_as_constr(enum ndt tag)
{
    switch (tag) {
    case AnyKind: return "Any";

    case FixedDim: return "FixedDim";
    case SymbolicDim: return "SymbolicDim";
    case VarDim: return "VarDim";
    case EllipsisDim: return "EllipsisDim";
    case Ndarray: return "Ndarray";

    case Option: return "Option";
    case Nominal: return "Nominal";
    case Constr: return "Constr";

    case Tuple: return "Tuple";
    case Record: return "Record";
    case Function: return "Function";
    case Typevar: return "Typevar";

    case ScalarKind: return "ScalarKind";
    case Void: return "Void";
    case Bool: return "Bool";

    case SignedKind: return "SignedKind";
    case Int8: return "Int8";
    case Int16: return "Int16";
    case Int32: return "Int32";
    case Int64: return "Int64";

    case UnsignedKind: return "UnsignedKind";
    case Uint8: return "Uint8";
    case Uint16: return "Uint16";
    case Uint32: return "Uint32";
    case Uint64: return "Uint64";

    case RealKind: return "RealKind";
    case Float16: return "Float16";
    case Float32: return "Float32";
    case Float64: return "Float64";

    case ComplexKind: return "ComplexKind";
    case Complex64: return "Complex64";
    case Complex128: return "Complex128";

    case Char: return "Char";

    case String: return "String";
    case FixedStringKind: return "FixedStringKind";
    case FixedString: return "FixedString";

    case Bytes: return "Bytes";
    case FixedBytesKind: return "FixedBytesKind";
    case FixedBytes: return "FixedBytes";

    case Categorical: return "Categorical";
    case Pointer: return "Pointer";

    default: return "unknown tag";
    }
}


static int
datashape(buf_t *buf, const ndt_t *t, int d, int cont, ndt_context_t *ctx)
{
    int n;

    switch (t->tag) {
        case FixedDim:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "FixedDim(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->FixedDim.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(
                    ctx, buf, d+2,
                    "shape=%zu, stride=%" PRIi64 ", offset=%" PRIi64 ", itemsize=%zu,\n",
                    t->FixedDim.shape, t->FixedDim.stride, t->FixedDim.offset, t->FixedDim.itemsize);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case SymbolicDim:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "SymbolicDim(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->SymbolicDim.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "name='%s', stride=%" PRIi64 ", itemsize=%zu,\n",
                               t->SymbolicDim.name, t->SymbolicDim.stride, t->SymbolicDim.itemsize);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case VarDim: {
            int i;

            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "VarDim(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->VarDim.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "shapes=[");
            if (n < 0) return -1;

            for (i = 0; i < t->VarDim.nshapes; i++) {
                n = ndt_snprintf(ctx, buf, "%" PRIi64 "%s", t->VarDim.shapes[i],
                                 i==t->VarDim.nshapes-1 ? "" : ", ");
                if (n < 0) return -1;
            }

            n = ndt_snprintf(ctx, buf, "], stride=%" PRIi64 ", offset=%" PRIi64 ", itemsize=%zu,\n",
                             t->VarDim.stride, t->VarDim.offset, t->VarDim.itemsize);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");
        }

        case EllipsisDim:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "EllipsisDim(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->EllipsisDim.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, "\n");
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Ndarray: {
            int i;

            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Ndarray(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->Ndarray.dtype, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, "\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "shape=[");
            if (n < 0) return -1;

            for (i = 0; i < t->ndim; i++) {
                if (t->Ndarray.symbols[i]) {
                    n = ndt_snprintf(ctx, buf, "%s%s", t->Ndarray.symbols[i],
                                     i == t->ndim-1 ? "]\n" : ", ");
                    if (n < 0) return -1;
                }
                else {
                    n = ndt_snprintf(ctx, buf, "%" PRIi64 "%s", t->Ndarray.shape[i],
                                     i == t->ndim-1 ? "]\n" : ", ");
                    if (n < 0) return -1;
                }
            }

            n = ndt_snprintf_d(ctx, buf, d+2, "strides=[");
            if (n < 0) return -1;
            for (i = 0; i < t->ndim-1; i++) {
                n = ndt_snprintf(ctx, buf, "%" PRIi64 ", ", t->Ndarray.strides[i]);
                if (n < 0) return -1;
            }
            n = ndt_snprintf(ctx, buf, "%" PRIi64 "],\n", t->Ndarray.strides[i]);
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "offset=%" PRIi64 ", itemsize=%zu,\n",
                               t->Ndarray.offset, t->Ndarray.itemsize);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");
        }

        case Option:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Option(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->Option.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Nominal:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Nominal(\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "name='%s',\n", t->Nominal.name);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Constr:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Constr(\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "name='%s',\n", t->Constr.name);
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "type=");
            if (n < 0) return -1;

            n = datashape(buf, t->Constr.type, d+5+2, 1, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, "\n");
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d, ")");
            return n;

        case Tuple:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Tuple(\n");
            if (n < 0) return -1;

            if (t->Tuple.fields) {
                n = tuple_fields(buf, t->Tuple.fields, t->Tuple.shape, d+2, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, t->Tuple.flag, d+2, ctx);
                if (n < 0) return -1;

                n = ndt_snprintf(ctx, buf, ",\n");
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, t->Tuple.flag, d+2, ctx);
                if (n < 0) return -1;
            }

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Record:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Record(\n");
            if (n < 0) return -1;

            if (t->Record.fields) {
                n = record_fields(buf, t->Record.fields, t->Record.shape, d+2, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, t->Record.flag, d+2, ctx);
                if (n < 0) return -1;

                n = ndt_snprintf(ctx, buf, ",\n");
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, t->Record.flag, d+2, ctx);
                if (n < 0) return -1;

            }

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Function:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Function(\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "pos=");
            if (n < 0) return -1;

            n = datashape(buf, t->Function.pos, d+4+2, 1, ctx);

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "kwds=");
            if (n < 0) return -1;

            n = datashape(buf, t->Function.kwds, d+5+2, 1, ctx);

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "ret=");
            if (n < 0) return -1;

            n = datashape(buf, t->Function.ret, d+4+2, 1, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Typevar:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Typevar(\n");
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d+2, "name=%s,\n", t->Typevar.name);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case AnyKind: case ScalarKind:
        case Void: case Bool:
        case SignedKind:
        case Int8: case Int16: case Int32: case Int64:
        case UnsignedKind:
        case Uint8: case Uint16: case Uint32: case Uint64:
        case RealKind:
        case Float16: case Float32: case Float64:
        case ComplexKind:
        case Complex64: case Complex128:
        case FixedStringKind: case FixedBytesKind:
        case String: case FixedString: case FixedBytes:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "%s(", tag_as_constr(t->tag));
            if (n < 0) return -1;

            n = common_attributes(buf, t, 0, ctx);
            if (n < 0) return -1;

            return ndt_snprintf(ctx, buf, ")");

        case Char:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Char(%s,\n",
                               ndt_encoding_as_string(t->Char.encoding));

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Bytes:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Bytes(target_align=%,\n", t->Bytes.target_align);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            return ndt_snprintf_d(ctx, buf, d, ")");

        case Categorical:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Categorical(");
            if (n < 0) return -1;

            n = categorical(buf, t->Categorical.types, t->Categorical.ntypes, d, ctx);
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ")");
            return n;

        case Pointer:
            n = ndt_snprintf_d(ctx, buf, cont ? 0 : d, "Pointer(\n");
            if (n < 0) return -1;

            n = datashape(buf, t->Pointer.type, d+2, 0, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = common_attributes_with_newline(buf, t, d+2, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf_d(ctx, buf, d, ")");
            return n;

        default:
            ndt_err_format(ctx, NDT_ValueError, "invalid tag");
            return -1;
    }
}

char *
ndt_as_string_with_meta(ndt_t *t, ndt_context_t *ctx)
{
    buf_t buf = {0, 0, NULL};
    char *s;
    size_t count;

    if (datashape(&buf, t, 0, 0, ctx) < 0) {
        return NULL;
    }

    count = buf.count;
    buf.count = 0;
    buf.size = count+1;

    buf.cur = s = ndt_alloc(1, count+1);
    if (buf.cur == NULL) {
        return ndt_memory_error(ctx);
    }

    if (datashape(&buf, t, 0, 0, ctx) < 0) {
        ndt_free(s);
        return NULL;
    }
    s[count] = '\0';

    return s;
}
