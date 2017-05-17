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
mk_fixed_dim(char *v, ndt_t *type, ndt_context_t *ctx)
{
    size_t shape;

    shape = (size_t)ndt_strtoull(v, SIZE_MAX, ctx);
    ndt_free(v);

    if (ctx->err != NDT_Success) {
        return NULL;
    }

    return ndt_fixed_dim(shape, type, ctx);
}

ndt_t *
mk_var_dim(ndt_string_seq_t *seq, ndt_t *type, ndt_context_t *ctx)
{
    int64_t *shapes = NULL;
    int64_t nshapes = 0;

    if (seq) {
        int64_t k;
        shapes = ndt_alloc(seq->len, sizeof *shapes);
        if (shapes == NULL) {
            ndt_string_seq_del(seq);
            ndt_del(type);
            return ndt_memory_error(ctx);
        }

        for (k = 0; k < (int64_t)seq->len; k++) {
            shapes[k] = (int64_t)ndt_strtoll(seq->ptr[k], 0, INT64_MAX, ctx);
            if (ctx->err != NDT_Success) {
                ndt_string_seq_del(seq);
                ndt_del(type);
                ndt_free(shapes);
                return NULL;
            }
        }

        nshapes = seq->len;
        ndt_string_seq_del(seq);
    }

    return ndt_var_dim(shapes, nshapes, type, ctx);
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

ndt_t *
mk_array(ndt_t *array, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    int64_t *strides = NULL;
    int16_t strides_len = 0;
    int64_t *offsets = NULL; /* vararray */
    int16_t offsets_len = 0; /* vararray */
    char_opt_t order = {None, '\0'};
    char *style = NULL;

    if (attrs) {
        int ret = ndt_parse_attr(Array, ctx, attrs, &strides, &strides_len,
                                 &offsets, &offsets_len, &order, &style);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            goto error;
        }

        if (strides && strides_len != array->ndim) {
            ndt_err_format(ctx, NDT_ValueError,
                           "strides must have length ndim");
            goto error;
        }

        if (offsets && offsets_len != array->ndim) {
            ndt_err_format(ctx, NDT_ValueError,
                           "offsets must have length ndim");
            goto error;
        }
    }

    if (style == NULL || strcmp(style, "array") == 0) {
        ndt_free(style);
        return ndt_array(array, strides, offsets, order, ctx);
    }
    else {
        ndt_err_format(ctx, NDT_ValueError, "invalid array style: '%s'", style);
        goto error;
    }

error:
    ndt_free(style);
    ndt_free(offsets);
    ndt_free(strides);
    ndt_del(array);
    return NULL;
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
