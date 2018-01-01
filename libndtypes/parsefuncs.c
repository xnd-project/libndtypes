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

enum ndt_encoding
encoding_from_string(char *s, ndt_context_t *ctx)
{
    enum ndt_encoding ret = ndt_encoding_from_string(s, ctx);
    ndt_free(s);
    return ret;
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
mk_fortran(ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t = ndt_to_fortran(type, ctx);

    ndt_del(type);
    return t;
}

ndt_t *
mk_fixed_dim_from_shape(char *v, ndt_t *type, ndt_context_t *ctx)
{
    int64_t shape;

    shape = ndt_strtoll(v, 0, INT64_MAX, ctx);
    ndt_free(v);

    if (ndt_err_occurred(ctx)) {
        ndt_del(type);
        return NULL;
    }

    return ndt_fixed_dim(type, shape, INT64_MAX, ctx);
}

ndt_t *
mk_fixed_dim_from_attrs(ndt_attr_seq_t *attrs, ndt_t *type, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {1, 2, {"shape", "step"}, {AttrInt64, AttrInt64}};
    int64_t shape;
    int64_t step = INT64_MAX;
    int ret;

    ret = ndt_parse_attr(&kwlist, ctx, attrs, &shape, &step);
    ndt_attr_seq_del(attrs);
    if (ret < 0) {
        ndt_del(type);
        return NULL;
    }

    return ndt_fixed_dim(type, shape, step, ctx);
}

ndt_t *
mk_var_dim(ndt_meta_t *m, ndt_attr_seq_t *attrs, ndt_t *type, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {1, 2, {"offsets", "_noffsets"}, {AttrInt32List, AttrInt64}};

    if (attrs) {
        int32_t *offsets = NULL;
        int64_t noffsets = 0;
        ndt_t *t;
        int ret;

        ret = ndt_parse_attr(&kwlist, ctx, attrs, &offsets, &noffsets);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            ndt_del(type);
            return NULL;
        }

        if (noffsets > INT32_MAX) {
            ndt_err_format(ctx, NDT_ValueError, "too many offsets");
            ndt_del(type);
            ndt_free(offsets);
            return NULL;
        }

        if (m == NULL) {
            t = ndt_var_dim(type, InternalOffsets, (int32_t)noffsets, offsets, 0, NULL, ctx);
        }
        else {
            int i = m->num_offset_arrays;
            if (i >= NDT_MAX_DIM) {
                ndt_err_format(ctx, NDT_RuntimeError,
                               "too many offsets arrays");
                ndt_del(type);
                ndt_free(offsets);
                return NULL;
            }
            m->num_offsets[i] = (int32_t)noffsets;
            m->offset_arrays[i] = offsets;
            m->num_offset_arrays++;

            t = ndt_var_dim(type, ExternalOffsets, (int32_t)noffsets, offsets, 0, NULL, ctx);
        }

        return t;
    }
    else {
        return ndt_abstract_var_dim(type, ctx);
    }
}

ndt_t *
mk_fixed_string(char *v, enum ndt_encoding encoding, ndt_context_t *ctx)
{
    int64_t size;

    size = ndt_strtoll(v, 0, INT64_MAX, ctx);
    ndt_free(v);

    if (ndt_err_occurred(ctx)) {
        return NULL;
    }

    return ndt_fixed_string(size, encoding, ctx);
}

ndt_t *
mk_bytes(ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 1, {"align"}, {AttrUint16Opt}};
    uint16_opt_t target_align = {None, 0};

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &target_align);
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
    static const attr_spec kwlist = {1, 2, {"size", "align"}, {AttrInt64, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    int64_t datasize = 0;

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &datasize, &align);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            return NULL;
        }
    }

    return ndt_fixed_bytes(datasize, align, ctx);
}

ndt_field_t *
mk_field(char *name, ndt_t *type, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    uint16_opt_t pad = {None, 0};

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &align, &pack);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            ndt_free(name);
            ndt_del(type);
            return NULL;
        }
    }

    return ndt_field(name, type, align, pack, pad, ctx);
}

ndt_t *
mk_tuple(enum ndt_variadic flag, ndt_field_seq_t *fields,
         ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    ndt_t *t;

    fields = ndt_field_seq_finalize(fields);

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &align, &pack);
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
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    ndt_t *t;

    fields = ndt_field_seq_finalize(fields);

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &align, &pack);
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
mk_categorical(ndt_value_seq_t *seq, ndt_context_t *ctx)
{
    ndt_t *t;

    seq = ndt_value_seq_finalize(seq);
    t = ndt_categorical(seq->ptr, seq->len, ctx);

    ndt_free(seq);
    return t;
}

ndt_attr_t *
mk_attr(char *name, char *value, ndt_context_t *ctx)
{
    ndt_attr_t *attr;

    attr = ndt_alloc_size(sizeof *attr);
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

    attr = ndt_alloc_size(sizeof *attr);
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
