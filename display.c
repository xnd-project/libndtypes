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


static int datashape(buf_t *buf, const ndt_t *t, int d, ndt_context_t *ctx);


static int
ndt_snprintf(ndt_context_t *ctx, buf_t *buf, const char *fmt, ...)
{
    va_list ap;
    int n;

    va_start(ap, fmt);
    errno = 0;
    n = vsnprintf(buf->cur, buf->size, fmt, ap);
    va_end(ap);
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
tuple_fields(buf_t *buf, ndt_tuple_field_t *fields, size_t nfields, int d,
             ndt_context_t *ctx)
{
    size_t i;
    int n;

    for (i = 0; i < nfields; i++) {
        if (i >= 1) {
            n = ndt_snprintf(ctx, buf, ", ");
            if (n < 0) return -1;
        }
  
        n = datashape(buf, fields[i].type, d, ctx);
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
            if (d >= 0) {
                n = ndt_snprintf(ctx, buf, ",\n");
                if (n < 0) return -1;

                n = indent(ctx, buf, d);
                if (n < 0) return -1;
            }
            else {
                n = ndt_snprintf(ctx, buf, ", ");
                if (n < 0) return -1;
            }
        }
  
        n = ndt_snprintf(ctx, buf, "%s : ", fields[i].name);
        if (n < 0) return -1;

        n = datashape(buf, fields[i].type, d, ctx);
        if (n < 0) return -1;
    }

    return 0;
}

static int
variadic_flag(buf_t *buf, enum ndt_variadic_flag flag, ndt_context_t *ctx)
{
    if (flag == Variadic) {
        return ndt_snprintf(ctx, buf, "...");
    }

    return 0;
}

static int
comma_variadic_flag(buf_t *buf, enum ndt_variadic_flag flag, int d, ndt_context_t *ctx)
{
    int n;

    if (flag == Variadic) {
        if (d >= 0) {
            n = ndt_snprintf(ctx, buf, ",\n");
            if (n < 0) return -1;

            n = indent(ctx, buf, d);
            if (n < 0) return -1;

            return ndt_snprintf(ctx, buf, "...");
        }
        else {
            return ndt_snprintf(ctx, buf, ", ...");
        }
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

        n = datashape(buf, mem[i].t, d, ctx);
        if (n < 0) return -1;
    }

    return 0;
}

static int
datashape(buf_t *buf, const ndt_t *t, int d, ndt_context_t *ctx)
{
    int n;

    switch (t->tag) {
        case FixedDimKind:
            n = ndt_snprintf(ctx, buf, "Fixed * ");
            if (n < 0) return -1;

            n = datashape(buf, t->FixedDimKind.type, d, ctx);
            return n;

        case FixedDim:
            n = ndt_snprintf(ctx, buf, "%" PRIi64 " * ", t->FixedDim.shape);
            if (n < 0) return -1;

            n = datashape(buf, t->FixedDim.type, d, ctx);
            return n;

        case VarDim:
            n = ndt_snprintf(ctx, buf, "var * ");
            if (n < 0) return -1;

            n = datashape(buf, t->VarDim.type, d, ctx);
            return n;

        case SymbolicDim:
            n = ndt_snprintf(ctx, buf, "%s * ", t->SymbolicDim.name);
            if (n < 0) return -1;

            n = datashape(buf, t->SymbolicDim.type, d, ctx);
            return n;

        case EllipsisDim:
            n = ndt_snprintf(ctx, buf, "... * ", t->SymbolicDim.name);
            if (n < 0) return -1;

            n = datashape(buf, t->EllipsisDim.type, d, ctx);
            return n;

        case Ndarray: {
            int i;

            for (i = 0; i < t->Array.ndim; i++) {
                n = ndt_snprintf(ctx, buf, "%" PRIi64 " * ", t->Array.shape[i]);
                if (n < 0) return -1;
            }

            n = datashape(buf, t->Array.dtype, d, ctx);
            return n;
        }

        case Option:
            n = ndt_snprintf(ctx, buf, "?");
            if (n < 0) return -1;

            n = datashape(buf, t->Option.type, d, ctx);
            return n;

        case Nominal:
            n = ndt_snprintf(ctx, buf, "%s", t->Nominal.name);
            return n;
 
        case Constr:
            n = ndt_snprintf(ctx, buf, "%s(", t->Constr.name);
            if (n < 0) return -1;

            n = datashape(buf, t->Constr.type, d, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ")");
            return n;
 
        case Tuple:
            n = ndt_snprintf(ctx, buf, "(");
            if (n < 0) return -1;

            if (t->Tuple.fields) {
                n = tuple_fields(buf, t->Tuple.fields, t->Tuple.shape, d, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, t->Tuple.flag, INT_MIN, ctx);
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, t->Tuple.flag, ctx);
                if (n < 0) return -1;
            }

            n = ndt_snprintf(ctx, buf, ")");
            return n;
 
        case Record:
            n = ndt_snprintf(ctx, buf, "{");
            if (n < 0) return -1;

            if (d >= 0) {
                n = ndt_snprintf(ctx, buf, "\n");
                if (n < 0) return -1;
                n = indent(ctx, buf, d+2);
                if (n < 0) return -1;
            }

            if (t->Record.fields) {
                n = record_fields(buf, t->Record.fields, t->Record.shape, d+2, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, t->Record.flag, d+2, ctx);
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, t->Record.flag, ctx);
                if (n < 0) return -1;

            }

            if (d >= 0) {
                n = ndt_snprintf(ctx, buf, "\n");
                if (n < 0) return -1;
                n = indent(ctx, buf, d);
                if (n < 0) return -1;
            }

            n = ndt_snprintf(ctx, buf, "}");
            return n;
 
        case Function: {
            ndt_t *pos = t->Function.pos;
            ndt_t *kwds = t->Function.kwds;

            n = ndt_snprintf(ctx, buf, "(");
            if (n < 0) return -1;

            if (pos->Tuple.fields) {
                n = tuple_fields(buf, pos->Tuple.fields, pos->Tuple.shape, d, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, pos->Tuple.flag, INT_MIN, ctx);
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, pos->Tuple.flag, ctx);
                if (n < 0) return -1;
            }

            if (kwds->Record.fields) {
                if (pos->Tuple.flag == Variadic || pos->Tuple.fields) {
                    n = ndt_snprintf(ctx, buf, ", ");
                    if (n < 0) return -1;
                }

                n = record_fields(buf, kwds->Record.fields, kwds->Record.shape, INT_MIN, ctx);
                if (n < 0) return -1;

                n = comma_variadic_flag(buf, kwds->Record.flag, INT_MIN, ctx);
                if (n < 0) return -1;
            }
            else {
                n = variadic_flag(buf, kwds->Record.flag, ctx);
                if (n < 0) return -1;
            }

            n = ndt_snprintf(ctx, buf, ") -> ");
            if (n < 0) return -1;

            n = datashape(buf, t->Function.ret, d, ctx);
            return n;
        }

        case Typevar:
            n = ndt_snprintf(ctx, buf, "%s", t->Typevar.name);
            return n;

        case AnyKind:
        case ScalarKind:
        case Void: case Bool:
        case SignedKind:
        case Int8: case Int16: case Int32: case Int64:
        case UnsignedKind:
        case Uint8: case Uint16: case Uint32: case Uint64:
        case RealKind:
        case Float32: case Float64:
        case ComplexKind:
        case Complex64: case Complex128:
        case FixedStringKind:
        case FixedBytesKind:
        case String:
            n = ndt_snprintf(ctx, buf, "%s", ndt_tag_as_string(t->tag));
            return n;

        case FixedString:
            n = ndt_snprintf(ctx, buf, "fixed_string(%zu, %s)",
                          t->FixedString.size,
                          ndt_encoding_as_string(t->FixedString.encoding));
            return n;

        case Char:
            n = ndt_snprintf(ctx, buf, "char(%s)",
                          ndt_encoding_as_string(t->Char.encoding));
            return n;

        case Bytes:
            n = ndt_snprintf(ctx, buf, "bytes(align=%" PRIu8 ")", t->Bytes.target_align);
            return n;

        case FixedBytes:
            n = ndt_snprintf(ctx, buf, "fixed_bytes(size=%zu, align=%" PRIu8 ")",
                          t->FixedBytes.size, t->FixedBytes.align);
            return n;

        case Categorical:
            n = ndt_snprintf(ctx, buf, "categorical(");
            if (n < 0) return -1;

            n = categorical(buf, t->Categorical.types, t->Categorical.ntypes, d, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ")");
            return n;

        case Pointer:
            n = ndt_snprintf(ctx, buf, "pointer(");
            if (n < 0) return -1;

            n = datashape(buf, t->Pointer.type, d, ctx);
            if (n < 0) return -1;

            n = ndt_snprintf(ctx, buf, ")");
            return n;

        default:
            ndt_err_format(ctx, NDT_ValueError, "invalid tag");
            return -1;
    }
}

char *
ndt_as_string(ndt_t *t, ndt_context_t *ctx)
{
    buf_t buf = {0, 0, NULL};
    char *s;
    size_t count;

    if (datashape(&buf, t, INT_MIN, ctx) < 0) {
        return NULL;
    }

    count = buf.count;
    buf.count = 0;
    buf.size = count+1;

    buf.cur = s = ndt_alloc(1, count+1);
    if (buf.cur == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    if (datashape(&buf, t, INT_MIN, ctx) < 0) {
        ndt_free(s);
        return NULL;
    }
    s[count] = '\0';

    return s;
}

char *
ndt_indent(ndt_t *t, ndt_context_t *ctx)
{
    buf_t buf = {0, 0, NULL};
    char *s;
    size_t count;

    if (datashape(&buf, t, 0, ctx) < 0) {
        return NULL;
    }

    count = buf.count;
    buf.count = 0;
    buf.size = count+1;

    buf.cur = s = ndt_alloc(1, count+1);
    if (buf.cur == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    if (datashape(&buf, t, 0, ctx) < 0) {
        ndt_free(s);
        return NULL;
    }
    s[count] = '\0';

    return s;
}
