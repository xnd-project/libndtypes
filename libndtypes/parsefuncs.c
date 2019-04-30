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
#include <limits.h>
#include <assert.h>
#include "ndtypes.h"
#include "parsefuncs.h"
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
/*                          Parser helper functions                          */
/*****************************************************************************/

enum ndt_encoding
encoding_from_string(char *s, ndt_context_t *ctx)
{
    enum ndt_encoding ret = ndt_encoding_from_string(s, ctx);
    ndt_free(s);
    return ret;
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


/*****************************************************************************/
/*                    Parser functions for creating types                    */
/*****************************************************************************/

/*
 * NOTE: These functions should be considered internal.  Many of these
 * deallocate "const ndt_t *type" for convenience in grammar.y.
 */

const ndt_t *
mk_module(char *name, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_module(name, type, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_function(ndt_type_seq_t *in, ndt_type_seq_t *out, ndt_context_t *ctx)
{
    const ndt_t *types[NDT_MAX_ARGS];
    int64_t nin = in->len;
    int64_t nout = out->len;
    int64_t nargs, i;
    const ndt_t *t;

    nargs = nin + nout;

    if (nargs > NDT_MAX_ARGS) {
        ndt_err_format(ctx, NDT_ValueError,
            "maximum number of function arguments is %d", NDT_MAX_ARGS);
        ndt_type_seq_del(in);
        ndt_type_seq_del(out);
        return NULL;
    }

    for (i = 0; i < nin; i++) {
        types[i] = in->ptr[i];
        ndt_incref(types[i]);
    }

    for (i = 0; i < nout; i++) {
        types[nin+i] = out->ptr[i];
        ndt_incref(types[nin+i]);
    }

    ndt_type_seq_del(in);
    ndt_type_seq_del(out);

    t = ndt_function(types, nargs, nin, nout, ctx);

    ndt_type_array_clear(types, nargs);
    return t;
}

const ndt_t *
mk_ellipsis_dim(char *name, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_ellipsis_dim(name, type, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_symbolic_dim(char *name, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_symbolic_dim(name, type, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_constr(char *name, const ndt_t *type, bool opt, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_constr(name, type, opt, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_ref(const ndt_t *type, bool opt, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_ref(type, opt, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_fortran(const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t = ndt_to_fortran(type, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_contig(char *name, ndt_t *type, ndt_context_t *ctx)
{
    enum ndt_contig tag = RequireNA;

    if (strcmp(name, "C") == 0) {
        tag = RequireC;
    }
    else if (strcmp(name, "F") == 0) {
        tag = RequireF;
    }
    ndt_free(name);

    if (tag == RequireNA) {
        ndt_err_format(ctx, NDT_ParseError,
            "valid contiguity modifiers are 'C' or 'F'");
        ndt_decref(type);
        return NULL;
    }

    switch (type->tag) {
    case FixedDim: {
        ndt_t *t = type;

        if (ndt_is_concrete(type)) {
            if (!ndt_is_c_contiguous(type)) {
                ndt_err_format(ctx, NDT_ParseError,
                    "valid contiguity modifiers are 'C' or 'F'");
                ndt_decref(type);
                return NULL;
            }

            if (tag == RequireF) {
                t = (ndt_t *)ndt_to_fortran(type, ctx);
                ndt_decref(type);
                if (t == NULL) {
                    return NULL;
                }
            }
        }

        t->access = Abstract;
        t->FixedDim.tag = tag;
        return t;
    }
    case SymbolicDim: {
        type->SymbolicDim.tag = tag;
        return type;
    }
    case EllipsisDim: {
        type->EllipsisDim.tag = tag;
        return type;
    }
    default: {
        ndt_err_format(ctx, NDT_ParseError,
            "'C' or 'F' can only be applied to fixed or symbolic dimensions");
        ndt_decref(type);
        return NULL;
      }
    }
}

const ndt_t *
mk_fixed_dim_from_shape(char *v, const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t;
    int64_t shape;

    shape = ndt_strtoll(v, 0, INT64_MAX, ctx);
    ndt_free(v);

    if (ndt_err_occurred(ctx)) {
        ndt_decref(type);
        return NULL;
    }

    t = ndt_fixed_dim(type, shape, INT64_MAX, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_fixed_dim_from_attrs(ndt_attr_seq_t *attrs, const ndt_t *type, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {1, 2, {"shape", "step"}, {AttrInt64, AttrInt64}};
    const ndt_t *t;
    int64_t shape;
    int64_t step = INT64_MAX;
    int ret;

    ret = ndt_parse_attr(&kwlist, ctx, attrs, &shape, &step);
    ndt_attr_seq_del(attrs);
    if (ret < 0) {
        ndt_decref(type);
        return NULL;
    }

    t = ndt_fixed_dim(type, shape, step, ctx);
    ndt_decref(type);
    return t;
}

const ndt_t *
mk_var_dim(ndt_attr_seq_t *attrs, const ndt_t *type, bool opt, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {1, 2, {"offsets", "_noffsets"}, {AttrInt32List, AttrInt64}};
    const ndt_t *t;

    if (attrs) {
        ndt_offsets_t *offsets;
        int32_t *ptr;
        int64_t n;
        int ret;

        ret = ndt_parse_attr(&kwlist, ctx, attrs, &ptr, &n);
        ndt_attr_seq_del(attrs);
        if (ret < 0) {
            ndt_decref(type);
            return NULL;
        }

        if (n > INT32_MAX) {
            ndt_err_format(ctx, NDT_ValueError, "too many offsets");
            ndt_free(ptr);
            ndt_decref(type);
            return NULL;
        }

        offsets = ndt_offsets_from_ptr(ptr, (int32_t)n, ctx);
        if (offsets == NULL) {
            ndt_decref(type);
            return NULL;
        }

        t = ndt_var_dim(type, offsets, 0, NULL, opt, ctx);
        ndt_decref_offsets(offsets);
    }
    else {
        t = ndt_abstract_var_dim(type, opt, ctx);
    }

    ndt_decref(type);
    return t;
}

const ndt_t *
mk_var_ellipsis(const ndt_t *type, ndt_context_t *ctx)
{
    const ndt_t *t;
    char *s;

    s = ndt_strdup("var", ctx);
    if (s == NULL) {
        return NULL;
    }

    t = ndt_ellipsis_dim(s, type, ctx);
    ndt_decref(type);
    return t;
}

ndt_field_t *
mk_field(char *name, const ndt_t *type, ndt_attr_seq_t *attrs, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    uint16_opt_t pad = {None, 0};
    ndt_field_t *f;

    if (attrs) {
        int ret = ndt_parse_attr(&kwlist, ctx, attrs, &align, &pack);
        ndt_attr_seq_del(attrs);

        if (ret < 0) {
            ndt_free(name);
            ndt_decref(type);
            return NULL;
        }
    }

    f = ndt_field(name, type, align, pack, pad, ctx);
    ndt_decref(type);
    return f;
}

const ndt_t *
mk_tuple(enum ndt_variadic flag, ndt_field_seq_t *fields,
         ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    const ndt_t *t;

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
        return ndt_tuple(flag, NULL, 0, align, pack, opt, ctx);
    }

    t = ndt_tuple(flag, fields->ptr, fields->len, align, pack, opt, ctx);
    ndt_field_seq_del(fields);
    return t;
}

const ndt_t *
mk_record(enum ndt_variadic flag, ndt_field_seq_t *fields,
          ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx)
{
    static const attr_spec kwlist = {0, 2, {"align", "pack"}, {AttrUint16Opt, AttrUint16Opt}};
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    const ndt_t *t;

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
        return ndt_record(flag, NULL, 0, align, pack, opt, ctx);
    }

    t = ndt_record(flag, fields->ptr, fields->len, align, pack, opt, ctx);
    ndt_field_seq_del(fields);
    return t;
}

const ndt_t *
mk_union(ndt_field_seq_t *fields, bool opt, ndt_context_t *ctx)
{
    const ndt_t *t;

    fields = ndt_field_seq_finalize(fields);
    t = ndt_union(fields->ptr, fields->len, opt, ctx);
    ndt_field_seq_del(fields);
    return t;
}

const ndt_t *
mk_categorical(ndt_value_seq_t *seq, bool opt, ndt_context_t *ctx)
{
    const ndt_t *t;

    seq = ndt_value_seq_finalize(seq);
    t = ndt_categorical(seq->ptr, seq->len, opt, ctx);

    ndt_free(seq);
    return t;
}

const ndt_t *
mk_fixed_string(char *v, enum ndt_encoding encoding, bool opt, ndt_context_t *ctx)
{
    int64_t size;

    size = ndt_strtoll(v, 0, INT64_MAX, ctx);
    ndt_free(v);

    if (ndt_err_occurred(ctx)) {
        return NULL;
    }

    return ndt_fixed_string(size, encoding, opt, ctx);
}

const ndt_t *
mk_fixed_bytes(ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx)
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

    return ndt_fixed_bytes(datasize, align, opt, ctx);
}

const ndt_t *
mk_bytes(ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx)
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

    return ndt_bytes(target_align, opt, ctx);
}
