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
#include <stdint.h>
#include <inttypes.h>
#include <stdalign.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "test.h"


#ifdef __clang__
  #pragma clang diagnostic ignored "-Wgnu-alignof-expression"
#endif


/*********************************************************************/
/*                           nested arrays                           */
/*********************************************************************/

#ifdef __GNUC__
typedef struct {
    uint8_t data[2][3][10];
} nested_1;

typedef struct {
    ndt_complex64_t data[3][7];
} nested_2;

typedef struct {
    nested_1 a;
    nested_2 b;
} struct_1;

static struct_1 array_1[10][3];

static const char *type_array_1 =
    "10 * 3 * { a: 2 * 3 * 10 * ?uint8, b: 3 * 7 * ?complex64 }";

int
test_array(void)
{
    ndt_context_t *ctx;
    ndt_t *t;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    t = ndt_from_string(type_array_1, ctx);
    if (t == NULL) {
        fprintf(stderr, "test_struct_align_pack: parse: FAIL: %s\n",
                ndt_context_msg(ctx));
        ndt_context_del(ctx);
        return -1;
    }
    if (ndt_is_abstract(t)) {
        fprintf(stderr,
            "test_struct_align_pack: parse: FAIL: unexpected abstract type\n");
        ndt_context_del(ctx);
        return -1;
    }

    (void)array_1;
    if (t->datasize != sizeof(array_1)) {
        fprintf(stderr, "test_array: array_1: FAIL: %" PRIi64 " %zu", t->datasize, sizeof(array_1));
    }
    else {
        fprintf(stderr, "test_array (1 test case)\n");
    }

    ndt_del(t);
    ndt_context_del(ctx);

    return 0;
}
#endif


