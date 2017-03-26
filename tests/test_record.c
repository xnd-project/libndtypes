#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "test.h"


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
    int64_t i1 __attribute__((aligned(1))); // align >= max(1, alignof(int64_t))
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
       "{u1: uint8 |align=16|, i1: int64 |align=1|, u2 : uint8}";
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

    if (t->size != sizeof(packed_no_trail) ||
        t->align != alignof(packed_no_trail) ||
        t->Record.fields[0].offset != offsetof(packed_no_trailing_padding_t, u1) ||
        t->Record.fields[0].align !=  alignof(packed_no_trail.u1) ||
        t->Record.fields[1].offset != offsetof(packed_no_trailing_padding_t, i1) ||
        t->Record.fields[1].align !=  alignof(packed_no_trail.i1) ||
        t->Record.fields[2].offset != offsetof(packed_no_trailing_padding_t, u2) ||
        t->Record.fields[2].align !=  alignof(packed_no_trail.u2) ||
        t->Record.fields[3].offset != offsetof(packed_no_trailing_padding_t, i2) ||
        t->Record.fields[3].align !=  alignof(packed_no_trail.i2) ||
        t->Record.fields[4].offset != offsetof(packed_no_trailing_padding_t, u3) ||
        t->Record.fields[4].align !=  alignof(packed_no_trail.u3)) {
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

    if (t->size != sizeof(packed_no_trail) ||
        t->align != alignof(packed_no_trail) ||
        t->Record.fields[0].offset != offsetof(packed_no_trailing_padding_t, u1) ||
        t->Record.fields[0].align !=  alignof(packed_no_trail.u1) ||
        t->Record.fields[1].offset != offsetof(packed_no_trailing_padding_t, i1) ||
        t->Record.fields[1].align !=  alignof(packed_no_trail.i1) ||
        t->Record.fields[2].offset != offsetof(packed_no_trailing_padding_t, u2) ||
        t->Record.fields[2].align !=  alignof(packed_no_trail.u2) ||
        t->Record.fields[3].offset != offsetof(packed_no_trailing_padding_t, i2) ||
        t->Record.fields[3].align !=  alignof(packed_no_trail.i2) ||
        t->Record.fields[4].offset != offsetof(packed_no_trailing_padding_t, u3) ||
        t->Record.fields[4].align !=  alignof(packed_no_trail.u3)) {
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

    if (t->size != sizeof(packed_with_trail) ||
        t->align != alignof(packed_with_trail) ||
        t->Record.fields[0].offset != offsetof(packed_with_trailing_padding_t, u1) ||
        t->Record.fields[0].align !=  alignof(packed_with_trail.u1) ||
        t->Record.fields[1].offset != offsetof(packed_with_trailing_padding_t, i1) ||
        t->Record.fields[1].align !=  alignof(packed_with_trail.u1) ||
        t->Record.fields[2].offset != offsetof(packed_with_trailing_padding_t, u2) ||
        t->Record.fields[2].align !=  alignof(packed_with_trail.u2) ||
        t->Record.fields[3].offset != offsetof(packed_with_trailing_padding_t, i2) ||
        t->Record.fields[3].align !=  alignof(packed_with_trail.i2) ||
        t->Record.fields[4].offset != offsetof(packed_with_trailing_padding_t, u3) ||
        t->Record.fields[4].align !=  alignof(packed_with_trail.u3)) {
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

    if (t->size != sizeof(aligned) ||
        t->align != alignof(aligned) ||
        t->Record.fields[0].offset != offsetof(aligned_t, u1) ||
        t->Record.fields[0].align !=  alignof(aligned.u1) ||
        t->Record.fields[1].offset != offsetof(aligned_t, i1) ||
        t->Record.fields[1].align !=  alignof(aligned.i1) ||
        t->Record.fields[2].offset != offsetof(aligned_t, u2) ||
        t->Record.fields[2].align != alignof(aligned.u2)) {
        fprintf(stderr, "test_struct_align_pack: aligned: FAIL");
        ndt_context_del(ctx);
    }
    ndt_del(t);

    fprintf(stderr, "test_struct_align_pack: (4 test cases)\n");

    ndt_context_del(ctx);
    return 0;
}
#endif


