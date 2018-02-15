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


#ifndef SEQ_H
#define SEQ_H


#include "ndtypes.h"


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
        return ndt_memory_error(ctx);                          \
    }                                                          \
                                                               \
    ptr = ndt_alloc(2, sizeof *ptr);                           \
    if (ptr == NULL) {                                         \
        ndt_free(seq);                                         \
        elem##_del(elt);                                       \
        return ndt_memory_error(ctx);                          \
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
static int                                                         \
elem##_seq_grow(elem##_seq_t *seq, ndt_context_t *ctx)             \
{                                                                  \
    elem##_t *ptr;                                                 \
                                                                   \
    ptr = ndt_realloc(seq->ptr, seq->reserved, 2 * (sizeof *ptr)); \
    if (ptr == NULL) {                                             \
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");     \
        return -1;                                                 \
    }                                                              \
                                                                   \
    seq->ptr = ptr;                                                \
    seq->reserved = 2 * seq->reserved;                             \
                                                                   \
    return 0;                                                      \
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
    if (seq == NULL) {                                  \
        return NULL;                                    \
    }                                                   \
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

typedef struct {
    int64_t len;
    int64_t reserved;
    ndt_field_t *ptr;
} ndt_field_seq_t;

typedef struct {
    int64_t len;
    int64_t reserved;
    ndt_value_t *ptr;
} ndt_value_seq_t;

typedef struct {
    int64_t len;
    int64_t reserved;
    char **ptr;
} ndt_string_seq_t;

typedef struct {
    int64_t len;
    int64_t reserved;
    ndt_t **ptr;
} ndt_type_seq_t;


ndt_field_seq_t *ndt_field_seq_new(ndt_field_t *, ndt_context_t *ctx);
void ndt_field_seq_del(ndt_field_seq_t *);
ndt_field_seq_t *ndt_field_seq_append(ndt_field_seq_t *, ndt_field_t *, ndt_context_t *ctx);
ndt_field_seq_t *ndt_field_seq_finalize(ndt_field_seq_t *);

ndt_value_seq_t *ndt_value_seq_new(ndt_value_t *, ndt_context_t *ctx);
void ndt_value_seq_del(ndt_value_seq_t *);
ndt_value_seq_t *ndt_value_seq_append(ndt_value_seq_t *, ndt_value_t *, ndt_context_t *ctx);
ndt_value_seq_t *ndt_value_seq_finalize(ndt_value_seq_t *);

ndt_string_seq_t *ndt_string_seq_new(char *, ndt_context_t *ctx);
void ndt_string_seq_del(ndt_string_seq_t *);
ndt_string_seq_t *ndt_string_seq_append(ndt_string_seq_t *, char *, ndt_context_t *ctx);
ndt_string_seq_t *ndt_string_seq_finalize(ndt_string_seq_t *);

ndt_type_seq_t *ndt_type_seq_empty(ndt_context_t *ctx);
ndt_type_seq_t *ndt_type_seq_new(ndt_t *, ndt_context_t *ctx);
void ndt_type_seq_del(ndt_type_seq_t *);
ndt_type_seq_t *ndt_type_seq_append(ndt_type_seq_t *, ndt_t *, ndt_context_t *ctx);
ndt_type_seq_t *ndt_type_seq_finalize(ndt_type_seq_t *);


#endif /* SEQ_H */
