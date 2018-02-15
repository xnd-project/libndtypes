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


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ndtypes.h"
#include "seq.h"


/*****************************************************************************/
/*                              Field sequences                              */
/*****************************************************************************/

NDT_SEQ_NEW(ndt_field)
NDT_SEQ_DEL(ndt_field)
NDT_SEQ_GROW(ndt_field)
NDT_SEQ_APPEND(ndt_field)
NDT_SEQ_FINALIZE(ndt_field)


/*****************************************************************************/
/*                               String sequences                            */
/*****************************************************************************/

ndt_string_seq_t *
ndt_string_seq_new(char *elt, ndt_context_t *ctx)
{
    ndt_string_seq_t *seq;
    char **ptr;

    seq = ndt_alloc_size(sizeof *seq);
    if (seq == NULL) {
        ndt_free(elt);
        return ndt_memory_error(ctx);
    }

    ptr = ndt_alloc_size(2 * (sizeof *ptr));
    if (ptr == NULL) {
        ndt_free(seq);
        ndt_free(elt);
        return ndt_memory_error(ctx);
    }

    ptr[0] = elt;
    seq->len = 1;
    seq->reserved = 2;
    seq->ptr = ptr;

    return seq;
}

void
ndt_string_seq_del(ndt_string_seq_t *seq)
{
    int64_t i;

    if (seq != NULL) {
        for (i = 0; i < seq->len; i++) {
            ndt_free(seq->ptr[i]);
        }
        ndt_free(seq->ptr);
        ndt_free(seq);
    }
}

static int
ndt_string_seq_grow(ndt_string_seq_t *seq, ndt_context_t *ctx)
{
    char **ptr;

    ptr = ndt_realloc(seq->ptr, seq->reserved, 2 * (sizeof *ptr));
    if (ptr == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return -1;
    }

    seq->ptr = ptr;
    seq->reserved = 2 * seq->reserved;

    return 0;
}

ndt_string_seq_t *
ndt_string_seq_append(ndt_string_seq_t *seq, char *elt, ndt_context_t *ctx)
{
    assert(seq->len <= seq->reserved);

    if (seq->len == seq->reserved) {
        if (ndt_string_seq_grow(seq, ctx) < 0) {
            ndt_string_seq_del(seq);
            ndt_free(elt);
            return NULL;
        }
    }

    seq->ptr[seq->len] = elt;
    seq->len++;

    return seq;
}

ndt_string_seq_t *
ndt_string_seq_finalize(ndt_string_seq_t *seq)
{
    char **ptr;

    if (seq == NULL) {
        return NULL;
    }

    assert(seq->len <= seq->reserved);

    ptr = ndt_realloc(seq->ptr, seq->len, sizeof *ptr);
    if (ptr == NULL) {
        return seq; /* seq is still valid */
    }

    seq->ptr = ptr;
    seq->reserved = seq->len;

    return seq;
}


/*****************************************************************************/
/*                                Type sequences                             */
/*****************************************************************************/

ndt_type_seq_t *
ndt_type_seq_empty(ndt_context_t *ctx)
{
    ndt_type_seq_t *seq;

    seq = ndt_alloc_size(sizeof *seq);
    if (seq == NULL) {
        return ndt_memory_error(ctx);
    }

    seq->len = 0;
    seq->reserved = 0;
    seq->ptr = NULL;

    return seq;
}

ndt_type_seq_t *
ndt_type_seq_new(ndt_t *elt, ndt_context_t *ctx)
{
    ndt_type_seq_t *seq;
    ndt_t **ptr;

    seq = ndt_alloc_size(sizeof *seq);
    if (seq == NULL) {
        ndt_del(elt);
        return ndt_memory_error(ctx);
    }

    ptr = ndt_alloc_size(2 * (sizeof *ptr));
    if (ptr == NULL) {
        ndt_free(seq);
        ndt_del(elt);
        return ndt_memory_error(ctx);
    }

    ptr[0] = elt;
    seq->len = 1;
    seq->reserved = 2;
    seq->ptr = ptr;

    return seq;
}

void
ndt_type_seq_del(ndt_type_seq_t *seq)
{
    if (seq != NULL) {
        ndt_type_array_del(seq->ptr, seq->len);
        ndt_free(seq);
    }
}

static int
ndt_type_seq_grow(ndt_type_seq_t *seq, ndt_context_t *ctx)
{
    ndt_t **ptr;

    ptr = ndt_realloc(seq->ptr, seq->reserved, 2 * (sizeof *ptr));
    if (ptr == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return -1;
    }

    seq->ptr = ptr;
    seq->reserved = 2 * seq->reserved;

    return 0;
}

ndt_type_seq_t *
ndt_type_seq_append(ndt_type_seq_t *seq, ndt_t *elt, ndt_context_t *ctx)
{
    assert(seq->len <= seq->reserved);

    if (seq->len == seq->reserved) {
        if (ndt_type_seq_grow(seq, ctx) < 0) {
            ndt_type_seq_del(seq);
            ndt_free(elt);
            return NULL;
        }
    }

    seq->ptr[seq->len] = elt;
    seq->len++;

    return seq;
}

ndt_type_seq_t *
ndt_type_seq_finalize(ndt_type_seq_t *seq)
{
    ndt_t **ptr;

    if (seq == NULL) {
        return NULL;
    }

    assert(seq->len <= seq->reserved);

    ptr = ndt_realloc(seq->ptr, seq->len, sizeof *ptr);
    if (ptr == NULL) {
        return seq; /* seq is still valid */
    }

    seq->ptr = ptr;
    seq->reserved = seq->len;

    return seq;
}
