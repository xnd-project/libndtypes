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


#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ndtypes.h"
#include "seq.h"


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

ndt_dim_t *
mk_fixed_dim(char *v, ndt_context_t *ctx)
{
    size_t shape;

    shape = (size_t)ndt_strtoull(v, SIZE_MAX, ctx);
    if (ctx->err != NDT_Success) {
        ndt_free(v);
        return NULL;
    }

    ndt_free(v);
    return ndt_fixed_dim(shape, ctx);
}
 
ndt_t *
mk_fixed_string(char *v, enum ndt_encoding encoding, ndt_context_t *ctx)
{
    size_t size;

    size = (size_t)ndt_strtoull(v, SIZE_MAX, ctx);
    if (ctx->err != NDT_Success) {
        ndt_free(v);
        return NULL;
    }

    ndt_free(v);
    return ndt_fixed_string(size, encoding, ctx);
}
 
ndt_t *
mk_bytes(char *v, ndt_context_t *ctx)
{
    uint8_t align;

    align = (uint8_t)ndt_strtoull(v, UINT8_MAX, ctx);
    if (ctx->err != NDT_Success) {
        ndt_free(v);
        return NULL;
    }

    ndt_free(v);
    return ndt_bytes(align, ctx);
}
 
ndt_t *
mk_fixed_bytes(char *size, char *target_align, ndt_context_t *ctx)
{
    size_t sz;
    uint8_t align;

    sz = (size_t)ndt_strtoull(size, SIZE_MAX, ctx);
    if (ctx->err != NDT_Success) {
        ndt_free(size);
        ndt_free(target_align);
        return NULL;
    }

    align = (uint8_t)ndt_strtoull(target_align, UINT8_MAX, ctx);
    if (ctx->err != NDT_Success) {
        ndt_free(size);
        ndt_free(target_align);
        return NULL;
    }

    ndt_free(size);
    ndt_free(target_align);
    return ndt_fixed_bytes(sz, align, ctx);
}

ndt_t *
mk_array(ndt_dim_seq_t *seq, ndt_t *dtype, ndt_context_t *ctx)
{
    ndt_t *t;

    seq = ndt_dim_seq_finalize(seq);
    t = ndt_array(seq->ptr, seq->len, dtype, ctx);

    ndt_free(seq);
    return t;
}

ndt_t *
mk_tuple(enum ndt_variadic_flag flag, ndt_tuple_field_seq_t *seq, ndt_context_t *ctx)
{
    ndt_t *t;

    seq = ndt_tuple_field_seq_finalize(seq);
    t = ndt_tuple(flag, seq->ptr, seq->len, ctx);

    ndt_free(seq);
    return t;
}
 
ndt_t *
mk_record(enum ndt_variadic_flag flag, ndt_record_field_seq_t *seq, ndt_context_t *ctx)
{
    ndt_t *t;

    seq = ndt_record_field_seq_finalize(seq);
    t = ndt_record(flag, seq->ptr, seq->len, ctx);

    ndt_free(seq);
    return t;
}
 
ndt_t *
mk_function(ndt_t *ret,
            enum ndt_variadic_flag tflag, ndt_tuple_field_seq_t *tseq,
            enum ndt_variadic_flag rflag, ndt_record_field_seq_t *rseq,
            ndt_context_t *ctx)
{
    ndt_t *pos = NULL;
    ndt_t *kwds = NULL;

    if (tseq) {
        pos = mk_tuple(tflag, tseq, ctx);
        if (pos == NULL) {
            ndt_del(ret);
            ndt_record_field_seq_del(rseq);
            return NULL;
        }
    }
    else if (tflag == Variadic) {
        pos = ndt_tuple(Variadic, NULL, 0, ctx);
        if (pos == NULL) {
            ndt_del(ret);
            ndt_record_field_seq_del(rseq);
            return NULL;
        }
    }

    if (rseq) {
        kwds = mk_record(rflag, rseq, ctx);
        if (kwds == NULL) {
            ndt_del(ret);
            ndt_del(pos);
            return NULL;
        }
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
