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
#include <limits.h>
#include <stddef.h>
#include "ndtypes.h"


/*****************************************************************************/
/*                                 Charmap                                   */
/*****************************************************************************/

#define ALPHABET_LEN 63

static int code[UCHAR_MAX+1];
static unsigned char alpha[ALPHABET_LEN+1] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";

static void
init_charmap(void)
{
    int i;

    for (i = 0; i < UCHAR_MAX+1; i++) {
        code[i] = UCHAR_MAX;
    }

    for (i = 0; i < ALPHABET_LEN; i++) {
        code[alpha[i]] = i;
    }
}


/*****************************************************************************/
/*                                  Trie                                     */
/*****************************************************************************/

typedef struct trie trie_t;
struct trie {
    const ndt_t *value;
    trie_t *next[];
};

static trie_t *map = NULL;

static trie_t *
trie_new(ndt_context_t *ctx)
{
    trie_t *t;
    int i;

    t = ndt_alloc(1, offsetof(trie_t, next) + ALPHABET_LEN * (sizeof t));
    if (t == NULL) {
        ndt_err_format(ctx, NDT_MemoryError, "out of memory");
        return NULL;
    }

    t->value = NULL;

    for (i = 0; i < ALPHABET_LEN; i++) {
        t->next[i] = NULL;
    }

    return t;
}

static void
trie_del(trie_t *t)
{
    int i;

    if (t == NULL) {
        return;
    }

    ndt_del((ndt_t *)t->value);

    for (i = 0; i < ALPHABET_LEN; i++) {
        trie_del(t->next[i]);
    }

    ndt_free(t);
}


int
ndt_init(ndt_context_t *ctx)
{
    init_charmap();

    map = trie_new(ctx);
    if (map == NULL) {
        return -1;
    }

    return 0;
}

void
ndt_finalize(void)
{
    trie_del(map);
    map = NULL;
}

int
ndt_nominal_map_insert(const char *key, const ndt_t *value, ndt_context_t *ctx)
{
    trie_t *t = map;
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            ndt_err_format(ctx, NDT_ValueError,
                           "invalid character in typedef: '%c'", *cp);
            return -1;
        }

        if (t->next[i] == NULL) {
            trie_t *u = trie_new(ctx);
            if (u == NULL) {
                return -1;
            }
            t->next[i] = u;
            t = u;
        }
        else {
            t = t->next[i];
        }
    }

    if (t->value) {
        ndt_err_format(ctx, NDT_ValueError, "duplicate typedef '%s'", key);
        return -1;
    }

    t->value = value;
    return 0;
}

const ndt_t *
ndt_nominal_map_lookup(const char *key, ndt_context_t *ctx)
{
    trie_t *t = map;
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            ndt_err_format(ctx, NDT_ValueError,
                           "invalid character in typedef: '%c'", *cp);
            return NULL;
        }

        if (t->next[i] == NULL) {
            ndt_err_format(ctx, NDT_ValueError,
                           "missing typedef for key '%s'", key);
            return NULL;
        }
        t = t->next[i];
    }

    if (t->value == NULL) {
        ndt_err_format(ctx, NDT_RuntimeError,
                       "missing typedef for key '%s'", key);
        return NULL;
    }

    return t->value;
}
