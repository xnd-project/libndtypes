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


#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "seq.h"


/*****************************************************************************/
/*                    Values for the categorical type                        */
/*****************************************************************************/

void
ndt_value_del(ndt_value_t *mem)
{
    if (mem == NULL) {
        return;
    }

    if (mem->tag == ValString) {
        ndt_free(mem->ValString);
    }

    ndt_free(mem);
}

void
ndt_value_array_del(ndt_value_t *mem, int64_t ntypes)
{
    int64_t i;

    if (mem == NULL) {
        return;
    }

    for (i = 0; i < ntypes; i++) {
        if (mem[i].tag == ValString) {
            ndt_free(mem[i].ValString);
        }
    }

    ndt_free(mem);
}

/* Value sequences */
NDT_SEQ_NEW(ndt_value)
NDT_SEQ_DEL(ndt_value)
NDT_SEQ_GROW(ndt_value)
NDT_SEQ_APPEND(ndt_value)
NDT_SEQ_FINALIZE(ndt_value)


/* Return a new ndt memory buffer. Input types are restricted. */
ndt_value_t *
ndt_value_from_number(enum ndt_value tag, char *v, ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc_size(sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        return ndt_memory_error(ctx);
    }
    mem->tag = tag;

    switch (tag) {
    case ValBool:
        mem->ValBool = ndt_strtobool(v, ctx);
        break;
    case ValInt64:
        mem->ValInt64 = (int64_t)ndt_strtoll(v, INT64_MIN, INT64_MAX, ctx);
        break;
    case ValFloat64:
        mem->ValFloat64 = ndt_strtod(v, ctx);
        break;
    case ValString: case ValNA:
        ndt_err_format(ctx, NDT_InvalidArgumentError, "expected number tag");
        break;
    }

    ndt_free(v);
    if (ndt_err_occurred(ctx)) {
        ndt_free(mem);
        return NULL;
    }

    return mem;
}

/* Return a new ndt memory buffer. The input type is 'string'. */
ndt_value_t *
ndt_value_from_string(char *v, ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc_size(sizeof *mem);
    if (mem == NULL) {
        ndt_free(v);
        return ndt_memory_error(ctx);
    }

    /* XXX: check utf8 */
    mem->tag = ValString;
    mem->ValString = v;

    return mem;
}

/* Return a new ndt memory buffer. The input type is 'NA'. */
ndt_value_t *
ndt_value_na(ndt_context_t *ctx)
{
    ndt_value_t *mem;

    mem = ndt_alloc_size(sizeof *mem);
    if (mem == NULL) {
        return ndt_memory_error(ctx);
    }
    mem->tag = ValNA;

    return mem;
}

/* Equality: NA does not compare equal to anything. */
int
ndt_value_equal(const ndt_value_t *x, const ndt_value_t *y)
{
    if (x->tag != y->tag) {
        return 0;
    }

    switch(x->tag) {
    case ValBool:
        return x->ValBool == y->ValBool;
    case ValInt64:
        return x->ValInt64 == y->ValInt64;
    case ValFloat64:
        return x->ValFloat64 == y->ValFloat64;
    case ValString:
        return strcmp(x->ValString, y->ValString) == 0;
    case ValNA:
        return 0;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid value");
}

/* Structural equality: NA compares equal to itself. */
int
ndt_value_mem_equal(const ndt_value_t *x, const ndt_value_t *y)
{
    if (x->tag != y->tag) {
        return 0;
    }

    switch(x->tag) {
    case ValBool:
        return x->ValBool == y->ValBool;
    case ValInt64:
        return x->ValInt64 == y->ValInt64;
    case ValFloat64:
        return x->ValFloat64 == y->ValFloat64;
    case ValString:
        return strcmp(x->ValString, y->ValString) == 0;
    case ValNA:
        return 1;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid value");
}

/* Sort order: NA compares equal to itself. */
int
ndt_value_compare(const ndt_value_t *x, const ndt_value_t *y)
{
    if (x->tag != y->tag) {
        return x->tag - y->tag;
    }

    switch(x->tag) {
    case ValBool:
        return x->ValBool < y->ValBool ? -1 : x->ValBool != y->ValBool;
    case ValInt64:
        return x->ValInt64 < y->ValInt64 ? -1 : x->ValInt64 != y->ValInt64;
    case ValFloat64:
        return x->ValFloat64 < y->ValFloat64 ? -1 : x->ValFloat64 != y->ValFloat64;
    case ValString:
        return strcmp(x->ValString, y->ValString);
    case ValNA:
        return 0;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid value");
}
