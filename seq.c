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
#include <assert.h>
#include "ndtypes.h"
#include "seq.h"


/*****************************************************************************/
/*                         Strongly typed sequences                          */
/*****************************************************************************/

/* Create a new sequence with one initial element.  Steal the element. */
#define NDT_SEQ_NEW(elem) \
elem##_seq_t *                                                 \
elem##_seq_new(elem##_t *elt, ndt_context_t *ctx)              \
{                                                              \
    elem##_seq_t *seq;                                         \
    elem##_t *ptr;                                             \
                                                               \
    seq = ndt_alloc(1, sizeof *seq);                           \
    if (seq == NULL) {                                         \
        elem##_del(elt);                                       \
        ndt_err_format(ctx, NDT_MemoryError, "out of memory"); \
        return NULL;                                           \
    }                                                          \
                                                               \
    ptr = ndt_alloc(2, sizeof *ptr);                           \
    if (ptr == NULL) {                                         \
        ndt_free(seq);                                         \
        elem##_del(elt);                                       \
        ndt_err_format(ctx, NDT_MemoryError, "out of memory"); \
        return NULL;                                           \
    }                                                          \
                                                               \
    ptr[0] = *elt;                                             \
    seq->len = 1;                                              \
    seq->reserved = 2;                                         \
    seq->ptr = ptr;                                            \
                                                               \
    ndt_free(elt);                                             \
    return seq;                                                \
}

/* Delete a sequence */
#define NDT_SEQ_DEL(elem) \
void                                          \
elem##_seq_del(elem##_seq_t *seq)             \
{                                             \
    if (seq != NULL) {                        \
        elem##_array_del(seq->ptr, seq->len); \
        ndt_free(seq);                        \
    }                                         \
}

/* Grow a sequence */
#define NDT_SEQ_GROW(elem) \
static int                                                       \
elem##_seq_grow(elem##_seq_t *seq, ndt_context_t *ctx)           \
{                                                                \
    elem##_t *ptr;                                               \
                                                                 \
    if (seq->reserved > SIZE_MAX / 2) {                          \
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");   \
        return -1;                                               \
    }                                                            \
                                                                 \
    ptr = ndt_realloc(seq->ptr, 2 * seq->reserved, sizeof *ptr); \
    if (ptr == NULL) {                                           \
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");   \
        return -1;                                               \
    }                                                            \
                                                                 \
    seq->ptr = ptr;                                              \
    seq->reserved = 2 * seq->reserved;                           \
                                                                 \
    return 0;                                                    \
}

/* Append an element to a sequence */
#define NDT_SEQ_APPEND(elem) \
elem##_seq_t *                                                          \
elem##_seq_append(elem##_seq_t *seq, elem##_t *elt, ndt_context_t *ctx) \
{                                                                       \
    assert(seq->len <= seq->reserved);                                  \
                                                                        \
    if (seq->len == seq->reserved) {                                    \
        if (elem##_seq_grow(seq, ctx) < 0) {                            \
            elem##_seq_del(seq);                                        \
            elem##_del(elt);                                            \
            return NULL;                                                \
        }                                                               \
    }                                                                   \
                                                                        \
    seq->ptr[seq->len] = *elt;                                          \
    seq->len++;                                                         \
                                                                        \
    ndt_free(elt);                                                      \
    return seq;                                                         \
}

#define NDT_SEQ_FINALIZE(elem) \
elem##_seq_t *                                          \
elem##_seq_finalize(elem##_seq_t *seq)                  \
{                                                       \
    elem##_t *ptr;                                      \
                                                        \
    assert(seq->len <= seq->reserved);                  \
                                                        \
    ptr = ndt_realloc(seq->ptr, seq->len, sizeof *ptr); \
    if (ptr == NULL) {                                  \
        return seq; /* seq is still valid */            \
    }                                                   \
                                                        \
    seq->ptr = ptr;                                     \
    seq->reserved = seq->len;                           \
                                                        \
    return seq;                                         \
}


NDT_SEQ_NEW(ndt_dim)
NDT_SEQ_DEL(ndt_dim)
NDT_SEQ_GROW(ndt_dim)
NDT_SEQ_APPEND(ndt_dim)
NDT_SEQ_FINALIZE(ndt_dim)

NDT_SEQ_NEW(ndt_tuple_field)
NDT_SEQ_DEL(ndt_tuple_field)
NDT_SEQ_GROW(ndt_tuple_field)
NDT_SEQ_APPEND(ndt_tuple_field)
NDT_SEQ_FINALIZE(ndt_tuple_field)

NDT_SEQ_NEW(ndt_record_field)
NDT_SEQ_DEL(ndt_record_field)
NDT_SEQ_GROW(ndt_record_field)
NDT_SEQ_APPEND(ndt_record_field)
NDT_SEQ_FINALIZE(ndt_record_field)

NDT_SEQ_NEW(ndt_memory)
NDT_SEQ_DEL(ndt_memory)
NDT_SEQ_GROW(ndt_memory)
NDT_SEQ_APPEND(ndt_memory)
NDT_SEQ_FINALIZE(ndt_memory)


