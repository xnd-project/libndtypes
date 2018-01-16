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
/*                 struct packing and alignment                      */
/*********************************************************************/

#ifdef __GNUC__
typedef struct {
    uint8_t u1;
    int64_t i1 __attribute__((packed, aligned(1))); // align >= 1
    uint8_t u2;
    int64_t i2 __attribute__((packed, aligned(1))); // align >= 1
    uint8_t u3;
} packed_no_trailing_padding_t;

typedef struct __attribute__((aligned(2))) {
    char u1;
    int64_t i1 __attribute__((packed, aligned(1))); // align >= 1
    char u2;
    int64_t i2 __attribute__((packed, aligned(1))); // align >= 1
    char u3;
} packed_with_trailing_padding_t;

typedef struct {
    char u1 __attribute__((aligned(16)));   // align >= 16
    int64_t i1 __attribute__((aligned(8))); // align >= max(1, alignof(int64_t))
    char u2;
} aligned_t;


int
test_struct_align_pack(void)
{
    static const char *s1 =
       "{u1: uint8, i1: int64 |pack=1|, u2: uint8, i2: int64 |pack=1|, u3: uint8}";
    static const char *s1_abbrev =
       "{u1: uint8, i1: int64, u2: uint8, i2: int64, u3: uint8, pack=1}";
    static const char *s2 =
       "{u1: uint8, i1: int64 |pack=1|, u2: uint8, i2: int64 |pack=1|, u3: uint8, align=2}";
    static const char *s3 =
       "{u1: uint8 |align=16|, i1: int64 |align=8|, u2 : uint8}";
    packed_no_trailing_padding_t packed_no_trail;
    packed_with_trailing_padding_t packed_with_trail;
    aligned_t aligned;
    ndt_context_t *ctx;
    ndt_t *t;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    t = ndt_from_string(s1, ctx);
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

    if (t->datasize != sizeof(packed_no_trail) ||
        t->align != alignof(packed_no_trail) ||
        t->Concrete.Record.offset[0] != offsetof(packed_no_trailing_padding_t, u1) ||
        t->Concrete.Record.align[0] !=  alignof(packed_no_trail.u1) ||
        t->Concrete.Record.offset[1] != offsetof(packed_no_trailing_padding_t, i1) ||
        t->Concrete.Record.align[1] !=  alignof(packed_no_trail.i1) ||
        t->Concrete.Record.offset[2] != offsetof(packed_no_trailing_padding_t, u2) ||
        t->Concrete.Record.align[2] !=  alignof(packed_no_trail.u2) ||
        t->Concrete.Record.offset[3] != offsetof(packed_no_trailing_padding_t, i2) ||
        t->Concrete.Record.align[3] !=  alignof(packed_no_trail.i2) ||
        t->Concrete.Record.offset[4] != offsetof(packed_no_trailing_padding_t, u3) ||
        t->Concrete.Record.align[4] !=  alignof(packed_no_trail.u3)) {
        fprintf(stderr, "test_struct_align_pack: packed_no_trail: FAIL");
        ndt_context_del(ctx);
    }
    ndt_del(t);


    t = ndt_from_string(s1_abbrev, ctx);
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

    if (t->datasize != sizeof(packed_no_trail) ||
        t->align != alignof(packed_no_trail) ||
        t->Concrete.Record.offset[0] != offsetof(packed_no_trailing_padding_t, u1) ||
        t->Concrete.Record.align[0] !=  alignof(packed_no_trail.u1) ||
        t->Concrete.Record.offset[1] != offsetof(packed_no_trailing_padding_t, i1) ||
        t->Concrete.Record.align[1] !=  alignof(packed_no_trail.i1) ||
        t->Concrete.Record.offset[2] != offsetof(packed_no_trailing_padding_t, u2) ||
        t->Concrete.Record.align[2] !=  alignof(packed_no_trail.u2) ||
        t->Concrete.Record.offset[3] != offsetof(packed_no_trailing_padding_t, i2) ||
        t->Concrete.Record.align[3] !=  alignof(packed_no_trail.i2) ||
        t->Concrete.Record.offset[4] != offsetof(packed_no_trailing_padding_t, u3) ||
        t->Concrete.Record.align[4] !=  alignof(packed_no_trail.u3)) {
        fprintf(stderr, "test_struct_align_pack: packed_no_trail_abbrev: FAIL\n");
        ndt_context_del(ctx);
    }
    ndt_del(t);


    t = ndt_from_string(s2, ctx);
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

    if (t->datasize != sizeof(packed_with_trail) ||
        t->align != alignof(packed_with_trail) ||
        t->Concrete.Record.offset[0] != offsetof(packed_with_trailing_padding_t, u1) ||
        t->Concrete.Record.align[0] !=  alignof(packed_with_trail.u1) ||
        t->Concrete.Record.offset[1] != offsetof(packed_with_trailing_padding_t, i1) ||
        t->Concrete.Record.align[1] !=  alignof(packed_with_trail.u1) ||
        t->Concrete.Record.offset[2] != offsetof(packed_with_trailing_padding_t, u2) ||
        t->Concrete.Record.align[2] !=  alignof(packed_with_trail.u2) ||
        t->Concrete.Record.offset[3] != offsetof(packed_with_trailing_padding_t, i2) ||
        t->Concrete.Record.align[3] !=  alignof(packed_with_trail.i2) ||
        t->Concrete.Record.offset[4] != offsetof(packed_with_trailing_padding_t, u3) ||
        t->Concrete.Record.align[4] !=  alignof(packed_with_trail.u3)) {
        fprintf(stderr, "test_struct_align_pack: packed_with_trail: FAIL");
        ndt_context_del(ctx);
    }
    ndt_del(t);


    t = ndt_from_string(s3, ctx);
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

    if (t->datasize != sizeof(aligned) ||
        t->align != alignof(aligned) ||
        t->Concrete.Record.offset[0] != offsetof(aligned_t, u1) ||
        t->Concrete.Record.align[0] !=  alignof(aligned.u1) ||
        t->Concrete.Record.offset[1] != offsetof(aligned_t, i1) ||
        t->Concrete.Record.align[1] !=  alignof(aligned.i1) ||
        t->Concrete.Record.offset[2] != offsetof(aligned_t, u2) ||
        t->Concrete.Record.align[2] != alignof(aligned.u2)) {
        fprintf(stderr, "test_struct_align_pack: aligned: FAIL");
        ndt_context_del(ctx);
    }
    ndt_del(t);

    fprintf(stderr, "test_struct_align_pack (4 test cases)\n");

    ndt_context_del(ctx);
    return 0;
}
#endif


