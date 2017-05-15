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
#include <string.h>
#include <stdarg.h>
#include "ndtypes.h"
#include "seq.h"


#define MAX_ATTR 8

typedef struct {
   const size_t min;
   const size_t max;
   const char *names[MAX_ATTR];
   const enum ndt_attr tags[MAX_ATTR];
} attr_spec;

/* Container attributes */
static const attr_spec array_attr = {0, 6,
    {"strides", "_strides_len", "offsets", "_offsets_len", "order", "style"},
    {AttrInt64List, AttrInt16, AttrInt64List, AttrInt16, AttrChar, AttrString}};
static const attr_spec tuple_record_attr = {0, 2, {"align", "pack"}, {AttrUint16_Opt, AttrUint16_Opt}};
static const attr_spec field_attr = {0, 2, {"align", "pack"}, {AttrUint16_Opt, AttrUint16_Opt}};

/* Type constructor attributes */
static const attr_spec prim_attr = {0, 1, {"endian"}, {AttrChar}};
static const attr_spec char_attr = {0, 1, {"encoding"}, {AttrString}};
static const attr_spec bytes_attr = {0, 1, {"align"}, {AttrUint16_Opt}};
static const attr_spec fixed_bytes_attr = {1, 2, {"size", "align"}, {AttrSize, AttrUint16_Opt}};


const attr_spec *
ndt_get_attr_spec(enum ndt tag, ndt_context_t *ctx)
{
    switch(tag) {
    case Ndarray:
        return &array_attr;
    case Tuple: case Record:
        return &tuple_record_attr;
    case Field:
        return &field_attr;
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float32: case Float64: case Complex64: case Complex128:
        return &prim_attr;
    case Char:
        return &char_attr;
    case Bytes:
        return &bytes_attr;
    case FixedBytes:
        return &fixed_bytes_attr;
    default:
        ndt_err_format(ctx, NDT_RuntimeError, "'%s' does not take attributes",
                       ndt_tag_as_string(tag));
        return NULL;
    }
}

int
ndt_parse_attr(enum ndt tag, ndt_context_t *ctx, const ndt_attr_seq_t *seq, ...)
{
    va_list ap;
    const attr_spec *spec;
    ndt_attr_t const *v[MAX_ATTR] = {NULL};
    int found;
    size_t i, k;

    spec = ndt_get_attr_spec(tag, ctx);
    if (spec == NULL) {
        return -1;
    }

    if (seq->len < spec->min || seq->len > spec->max) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "too %s arguments", seq->len < spec->min ? "few" : "many");
        return -1;
    }

    /* Quadratic time algorithm, we have a small number of args. */
    for (i = 0; i < seq->len; i++) {
        found = 0;
        for (k = 0; k < spec->max; k++) {
            if (strcmp(seq->ptr[i].name, spec->names[k]) == 0) {
                if (v[k]) {
                    ndt_err_format(ctx, NDT_InvalidArgumentError,
                        "duplicate argument: %s", spec->names[k]);
                    return -1;
                }
                found = 1;
                v[k] = &seq->ptr[i];
            }
        }
        if (!found) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "invalid argument: %s", seq->ptr[i].name);
            return -1;
        }
    }

    for (i = 0; i < spec->min; i++) {
        if (v[i] == NULL) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                           "missing required keyword: '%s'", spec->names[i]);
            return -1;
        }
    }

    va_start(ap, seq);
    for (i = 0; i < spec->max; i++) {
        void *ptr = va_arg(ap, void *);
        if (v[i] == NULL) {
            continue;
        }

        switch(spec->tags[i]) {
        case AttrBool: *(bool *)ptr = ndt_strtobool(v[i]->AttrValue, ctx); break;
        case AttrChar: *(char *)ptr = ndt_strtochar(v[i]->AttrValue, ctx); break;
        case AttrInt8: *(int8_t *)ptr = (int8_t)ndt_strtol(v[i]->AttrValue, INT8_MIN, INT8_MAX, ctx); break;
        case AttrInt16: *(int16_t *)ptr = (int16_t)ndt_strtol(v[i]->AttrValue, INT16_MIN, INT16_MAX, ctx); break;
        case AttrInt32: *(int32_t *)ptr = (int32_t)ndt_strtol(v[i]->AttrValue, INT32_MIN, INT32_MAX, ctx); break;
        case AttrInt64: *(int64_t *)ptr = (int64_t)ndt_strtoll(v[i]->AttrValue, INT64_MIN, INT64_MAX, ctx); break;
        case AttrUint8: *(uint8_t *)ptr = (uint8_t)ndt_strtoul(v[i]->AttrValue, UINT8_MAX, ctx); break;
        case AttrUint16: *(uint16_t *)ptr = (uint16_t)ndt_strtoul(v[i]->AttrValue, UINT16_MAX, ctx); break;
        case AttrUint32: *(uint32_t *)ptr = (uint32_t)ndt_strtoul(v[i]->AttrValue, UINT32_MAX, ctx); break;
        case AttrUint64: *(uint64_t *)ptr = (uint64_t)ndt_strtoull(v[i]->AttrValue, UINT64_MAX, ctx); break;
        case AttrSize: *(size_t *)ptr = (size_t)ndt_strtoull(v[i]->AttrValue, SIZE_MAX, ctx); break;
        case AttrFloat32: *(float *)ptr = ndt_strtof(v[i]->AttrValue, ctx); break;
        case AttrFloat64: *(double *)ptr = ndt_strtod(v[i]->AttrValue, ctx); break;

        case AttrUint16_Opt:
            ((uint16_opt_t *)ptr)->tag = Some;
            ((uint16_opt_t *)ptr)->Some = (uint16_t)ndt_strtoul(v[i]->AttrValue, UINT16_MAX, ctx);
            break;

        case AttrString: {
            char *value = ndt_strdup(v[i]->AttrValue, ctx);
            if (value == NULL) {
                return -1;
            }
            *(char **)ptr = value;
            break;
        }

        case AttrInt64List: {
            int64_t *values = ndt_alloc(v[i]->AttrList.len, sizeof(int64_t));

            if (values == NULL) {
                ndt_err_format(ctx, NDT_MemoryError, "out of memory");
                return -1;
            }

            for (k = 0; k < v[i]->AttrList.len; k++) {
                values[k] = (int64_t)ndt_strtoll(v[i]->AttrList.items[k], INT64_MIN, INT64_MAX, ctx);
                if (ctx->err != NDT_Success) {
                    ndt_free(values);
                    return -1;
                }
            }

            *(int64_t **)ptr = values;

            ptr = va_arg(ap, void *);
            *(int16_t *)ptr = v[i]->AttrList.len;
            i++;
            break;
        }

        default:
            ndt_err_format(ctx, NDT_RuntimeError,
                           "invalid attribute type", v[i]);
            break;
        }

        if (ctx->err != NDT_Success) {
            return -1;
        }
    }
    va_end(ap);

    return 0;
}
