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


#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "ndtypes.h"
#include "seq.h"
#include "attr.h"


/*****************************************************************************/
/*                        Functions used in the lexer                        */
/*****************************************************************************/

char *
mk_stringlit(const char *lexeme, ndt_context_t *ctx)
{
    size_t len;
    char *s, *result;

    len = strlen(lexeme);
    if (len < 2) {
        ndt_err_format(ctx, NDT_RuntimeError, "invalid string literal");
        return NULL;
    }

    s = ndt_strdup(lexeme, ctx);
    if (s == NULL) {
        return NULL;
    }

    s[len-1] = '\0';
    result = ndt_asprintf(ctx, "%s", s+1);

    ndt_free(s);
    return result;
}


/*****************************************************************************/
/*                        Functions used in the parser                       */
/*****************************************************************************/

ndt_t *
mk_fixed_dim_from_shape(char *v, ndt_t *type, ndt_context_t *ctx)
{
    int64_t shape;

    shape = ndt_strtoll(v, 0, INT64_MAX, ctx);
    ndt_free(v);

    if (ctx->err != NDT_Success) {
        ndt_del(type);
        return NULL;
    }

    return ndt_fixed_dim(shape, type, 'C', ctx);
}

ndt_t *
mk_fixed_dim_from_attrs(ndt_attr_seq_t *attrs, ndt_t *type, ndt_context_t *ctx)
{
    int64_t shape;
    int64_t stride = -1;
    char order = 'C';
    int ret;

    ret = ndt_parse_attr(FixedDim, ctx, attrs, &shape, &stride, &order);
    ndt_attr_seq_del(attrs);
    if (ret < 0) {
        ndt_del(type);
        return NULL;
    }

    return ndt_fixed_dim(shape, type, order, ctx);
}

static int64_t *
mk_offsets(const int64_t *shapes, int64_t nshapes, ndt_context_t *ctx)
{
    int64_t *offsets;
    int64_t i;

    offsets = ndt_alloc(nshapes+1, sizeof *offsets);
    if (offsets == NULL) {
        return ndt_memory_error(ctx);
    }

    offsets[0] = 0;
    for (i = 0; i < nshapes; i++) {
        offsets[i+1] = offsets[i] + shapes[i];
    }

    return offsets;
}

static uint8_t *
mk_bitmap(const int64_t *valid, int64_t nvalid, ndt_context_t *ctx)
{
    uint8_t *bitmap;
    int64_t i;

    bitmap = ndt_calloc((nvalid+7) / 8, sizeof *bitmap);
    if (bitmap == NULL) {
        return ndt_memory_error(ctx);
    }

    for (i = 0; i < nvalid; i++) {
        if (valid[i]) {
            bitmap[i / 8] |= ((uint8_t )1 << (i % 8));
        }
    }

    return bitmap;
}

ndt_t *
mk_var_dim(ndt_attr_seq_t *attrs, ndt_t *type, ndt_context_t *ctx)
{
    if (attrs) {
        ndt_t *t;
        int64_t *shapes = NULL;
        int64_t *offsets = NULL;
        int64_t *valid = NULL;
        uint8_t *bitmap = NULL;
        int64_t nshapes = 0;
        int64_t noffsets = 0;
        int64_t nvalid = 0;
        int ret;

        ret = ndt_parse_attr(VarDim, ctx, attrs, &shapes, &nshapes,
                             &offsets, &noffsets, &valid, &nvalid);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            ndt_del(type);
            return NULL;
        }

        if (shapes == NULL) {
            ndt_free(offsets);
            ndt_free(valid);
            ndt_del(type);
            return NULL;
        }

        if ((offsets && noffsets != nshapes+1) ||
            (valid && nvalid != nshapes)) {
            ndt_err_format(ctx, NDT_ValueError,
                           "invalid number of elements in offsets or bitmap");
            ndt_free(shapes);
            ndt_free(offsets);
            ndt_free(valid);
            ndt_del(type);
            return NULL;
        }

        if (offsets == NULL) {
            offsets = mk_offsets(shapes, nshapes, ctx);
            if (offsets == NULL) {
                ndt_free(shapes);
                ndt_free(bitmap);
                ndt_free(valid);
                ndt_del(type);
                return NULL;
            }
        }

        if (valid) {
            bitmap = mk_bitmap(valid, nvalid, ctx);
            ndt_free(valid);
            if (bitmap == NULL) {
                ndt_free(shapes);
                ndt_free(bitmap);
                ndt_del(type);
                return NULL;
            }
        }

        t = ndt_var_dim(type, true, Int32, nshapes, shapes, offsets, bitmap, ctx);
        ndt_free(shapes);
        ndt_free(offsets);
        ndt_free(bitmap);
        return t;

    }
    else {
        return ndt_var_dim(type, false, Void, 0, NULL, NULL, NULL, ctx);
    }
}

ndt_t *
mk_primitive(enum ndt tag, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    char endian = 'L';

    if (attrs) {
        int ret = ndt_parse_attr(tag, ctx, attrs, &endian);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            return NULL;
        }
    }

    return ndt_primitive(tag, endian, ctx);
}

ndt_t *
mk_alias(enum ndt_alias tag, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    char endian = 'L';

    if (attrs) {
        /* Alias attributes are the same as for Int64. */
        int ret = ndt_parse_attr(Int64, ctx, attrs, &endian);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            return NULL;
        }
    }

    return ndt_from_alias(tag, endian, ctx);
}

ndt_t *
mk_fixed_string(char *v, enum ndt_encoding encoding, ndt_context_t *ctx)
{
    size_t size;

    size = (size_t)ndt_strtoull(v, SIZE_MAX, ctx);
    ndt_free(v);

    if (ctx->err != NDT_Success) {
        return NULL;
    }

    return ndt_fixed_string(size, encoding, ctx);
}

ndt_t *
mk_bytes(ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    uint16_opt_t target_align = {None, 0};

    if (attrs) {
        int ret = ndt_parse_attr(Bytes, ctx, attrs, &target_align);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            return NULL;
        }
    }

    return ndt_bytes(target_align, ctx);
}

ndt_t *
mk_fixed_bytes(ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    uint16_opt_t data_align = {None, 0};
    size_t data_size = 0;

    if (attrs) {
        int ret = ndt_parse_attr(FixedBytes, ctx, attrs, &data_size, &data_align);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            return NULL;
        }
    }

    return ndt_fixed_bytes(data_size, data_align, ctx);
}

ndt_field_t *
mk_field(char *name, ndt_t *type, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};

    if (attrs) {
        int ret = ndt_parse_attr(Field, ctx, attrs, &align, &pack);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            ndt_free(name);
            ndt_del(type);
            return NULL;
        }
    }

    return ndt_field(name, type, align, pack, ctx);
}

ndt_t *
mk_tuple(enum ndt_variadic flag, ndt_field_seq_t *fields,
         ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    ndt_t *t;

    fields = ndt_field_seq_finalize(fields);

    if (attrs) {
        int ret = ndt_parse_attr(Tuple, ctx, attrs, &align, &pack);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            ndt_field_seq_del(fields);
            return NULL;
        }
    }

    if (fields == NULL) {
        return ndt_tuple(flag, NULL, 0, align, pack, ctx);
    }

    t = ndt_tuple(flag, fields->ptr, fields->len, align, pack, ctx);
    ndt_free(fields);
    return t;
}

ndt_t *
mk_record(enum ndt_variadic flag, ndt_field_seq_t *fields,
          ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    ndt_t *t;

    fields = ndt_field_seq_finalize(fields);

    if (attrs) {
        int ret = ndt_parse_attr(Record, ctx, attrs, &align, &pack);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            ndt_field_seq_del(fields);
            return NULL;
        }
    }

    if (fields == NULL) {
        return ndt_record(flag, NULL, 0, align, pack, ctx);
    }

    t = ndt_record(flag, fields->ptr, fields->len, align, pack, ctx);
    ndt_free(fields);
    return t;
}

ndt_t *
mk_function(ndt_t *ret,
            enum ndt_variadic tflag, ndt_field_seq_t *tseq,
            enum ndt_variadic rflag, ndt_field_seq_t *rseq,
            ndt_context_t *ctx)
{
    ndt_t *pos = NULL;
    ndt_t *kwds = NULL;

    pos = mk_tuple(tflag, tseq, NULL, ctx);
    if (pos == NULL) {
        ndt_del(ret);
        ndt_field_seq_del(rseq);
        return NULL;
    }

    kwds = mk_record(rflag, rseq, NULL, ctx);
    if (kwds == NULL) {
        ndt_del(ret);
        ndt_del(pos);
        return NULL;
    }

    return ndt_function(ret, pos, kwds, ctx);
}

ndt_t *
mk_function_from_tuple(ndt_t *ret, ndt_t *pos, ndt_context_t *ctx)
{
    ndt_t *kwds = NULL;
    uint16_opt_t align = {None, 0};

    kwds = ndt_record(Nonvariadic, NULL, 0, align, align, ctx);
    if (kwds == NULL) {
        ndt_del(ret);
        ndt_del(pos);
        return NULL;
    }

    return ndt_function(ret, pos, kwds, ctx);
}

ndt_t *
mk_categorical(ndt_memory_seq_t *seq, ndt_context_t *ctx)
{
    ndt_t *t;

    seq = ndt_memory_seq_finalize(seq);
    t = ndt_categorical(seq->ptr, seq->len, ctx);

    ndt_free(seq);
    return t;
}

ndt_attr_t *
mk_attr(char *name, char *value, ndt_context_t *ctx)
{
    ndt_attr_t *attr;

    attr = ndt_alloc(1, sizeof *attr);
    if (attr == NULL) {
        ndt_free(name);
        ndt_free(value);
        return ndt_memory_error(ctx);
    }

    attr->tag = AttrValue;
    attr->name = name;
    attr->AttrValue = value;

    return attr;
}

ndt_attr_t *
mk_attr_from_seq(char *name, ndt_string_seq_t *seq, ndt_context_t *ctx)
{
    ndt_attr_t *attr;

    attr = ndt_alloc(1, sizeof *attr);
    if (attr == NULL) {
        ndt_free(name);
        ndt_string_seq_del(seq);
        return ndt_memory_error(ctx);
    }

    attr->tag = AttrList;
    attr->name = name;
    attr->AttrList.len = seq->len;
    attr->AttrList.items = seq->ptr;

    ndt_free(seq);

    return attr;
}
