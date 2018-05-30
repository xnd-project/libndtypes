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
#include <limits.h>
#include <stddef.h>
#include "ndtypes.h"
#include "symtable.h"


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
/*                            Global typedef map                             */
/*****************************************************************************/

typedef struct typedef_trie {
    ndt_typedef_t def;
    struct typedef_trie *next[];
} typedef_trie_t;

static typedef_trie_t *typedef_map = NULL;

static typedef_trie_t *
typedef_trie_new(ndt_context_t *ctx)
{
    typedef_trie_t *t;
    int i;

    t = ndt_alloc_size(offsetof(typedef_trie_t, next) + ALPHABET_LEN * (sizeof t));
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->def.type = NULL;
    t->def.meth.init = NULL;
    t->def.meth.constraint = NULL;
    t->def.meth.repr = NULL;

    for (i = 0; i < ALPHABET_LEN; i++) {
        t->next[i] = NULL;
    }

    return t;
}

static void
typedef_trie_del(typedef_trie_t *t)
{
    int i;

    if (t == NULL) {
        return;
    }

    ndt_del((ndt_t *)t->def.type);

    for (i = 0; i < ALPHABET_LEN; i++) {
        typedef_trie_del(t->next[i]);
    }

    ndt_free(t);
}

int
ndt_typedef_add(const char *key, ndt_t *type, const ndt_methods_t *m, ndt_context_t *ctx)
{
    typedef_trie_t *t = typedef_map;
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            ndt_err_format(ctx, NDT_ValueError,
                           "invalid character in typedef: '%c'", *cp);
            ndt_del(type);
            return -1;
        }

        if (t->next[i] == NULL) {
            typedef_trie_t *u = typedef_trie_new(ctx);
            if (u == NULL) {
                ndt_del(type);
                return -1;
            }
            t->next[i] = u;
            t = u;
        }
        else {
            t = t->next[i];
        }
    }

    if (t->def.type) {
        ndt_err_format(ctx, NDT_ValueError, "duplicate typedef '%s'", key);
        ndt_del(type);
        return -1;
    }

    t->def.type = type;

    if (m != NULL) {
        t->def.meth = *m;
    }

    return 0;
}

const ndt_typedef_t *
ndt_typedef_find(const char *key, ndt_context_t *ctx)
{
    typedef_trie_t *t = typedef_map;
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

    if (t->def.type == NULL) {
        ndt_err_format(ctx, NDT_RuntimeError,
                       "missing typedef for key '%s'", key);
        return NULL;
    }

    return &t->def;
}


/*****************************************************************************/
/*                     Initialize/finalize global values                     */
/*****************************************************************************/

int
ndt_init(ndt_context_t *ctx)
{
    init_charmap();

    typedef_map = typedef_trie_new(ctx);
    if (typedef_map == NULL) {
        return -1;
    }

    return 0;
}

void
ndt_finalize(void)
{
    typedef_trie_del(typedef_map);
    typedef_map = NULL;
}


/*****************************************************************************/
/*                        Symbol tables for matching                         */
/*****************************************************************************/

symtable_t *
symtable_new(ndt_context_t *ctx)
{
    symtable_t *t;
    int i;

    t = ndt_alloc_size(offsetof(symtable_t, next) + ALPHABET_LEN * (sizeof t));
    if (t == NULL) {
        return ndt_memory_error(ctx);
    }

    t->entry.tag = Unbound;

    for (i = 0; i < ALPHABET_LEN; i++) {
        t->next[i] = NULL;
    }

    return t;
}

void
symtable_del(symtable_t *t)
{
    int i;

    if (t == NULL) {
        return;
    }

    for (i = 0; i < ALPHABET_LEN; i++) {
        symtable_del(t->next[i]);
    }

    ndt_free(t);
}

int
symtable_add(symtable_t *t, const char *key, const symtable_entry_t entry,
             ndt_context_t *ctx)
{
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            ndt_err_format(ctx, NDT_ValueError,
                           "invalid character in symbol: '%c'", *cp);
            return -1;
        }

        if (t->next[i] == NULL) {
            symtable_t *u = symtable_new(ctx);
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

    if (t->entry.tag != Unbound) {
        ndt_err_format(ctx, NDT_ValueError, "duplicate binding for '%s'", key);
        return -1;
    }

    t->entry = entry;
    return 0;
}

symtable_entry_t
symtable_find(const symtable_t *t, const char *key)
{
    symtable_entry_t unbound = { .tag=Unbound };
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            return unbound; /* NOT REACHED */
        }

        if (t->next[i] == NULL) {
            return unbound;
        }
        t = t->next[i];
    }

    return t->entry;
}

symtable_entry_t *
symtable_find_ptr(symtable_t *t, const char *key)
{
    const unsigned char *cp;
    int i;

    for (cp = (const unsigned char *)key; *cp != '\0'; cp++) {
        i = code[*cp];
        if (i == UCHAR_MAX) {
            return NULL; /* NOT REACHED */
        }

        if (t->next[i] == NULL) {
            return NULL;
        }
        t = t->next[i];
    }

    return &t->entry;
}

int64_t
symtable_find_shape(const symtable_t *tbl, const char *key, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    switch (v.tag) {
    case Shape:
        return v.Shape;
    default:
        ndt_err_format(ctx, NDT_ValueError,
            "variable not found or has incorrect type");
        return -1;
    }
}

const ndt_t *
symtable_find_typevar(const symtable_t *tbl, const char *key, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    switch (v.tag) {
    case Type:
        return v.Type;
    default:
        ndt_err_format(ctx, NDT_ValueError,
            "variable not found or has incorrect type");
        return NULL;
    }
}
