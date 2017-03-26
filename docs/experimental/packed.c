#include <stdio.h>
#include <stdint.h>
#include <stdalign.h>
#include <stddef.h>


typedef struct __attribute__((aligned(1))) {
    char c1;
    int64_t i1 __attribute__((packed, aligned(1))); // align >= 1
    char c2;
    int64_t i2 __attribute__((packed, aligned(2))); // align >= 2
    char c3;
} packed_and_aligned_t;

typedef struct {
    char c1 __attribute__((aligned(16)));   // align >= 16
    int64_t i1 __attribute__((aligned(1))); // align >= max(1, alignof(int64_t))
    char c2;
    int64_t i2 __attribute__((aligned(2)));
    char c3;
} aligned_t;

int
main(void)
{
    packed_and_aligned_t packed;
    aligned_t aligned;

    printf("struct packed_and_aligned: size: %zu  align: %zu\n", sizeof(packed), alignof(packed));
    printf("c1: offset: %zu  align: %zu\n", offsetof(packed_and_aligned_t, c1), alignof(packed.c1));
    printf("i1: offset: %zu  align: %zu\n", offsetof(packed_and_aligned_t, i1), alignof(packed.i1));
    printf("c2: offset: %zu  align: %zu\n", offsetof(packed_and_aligned_t, c2), alignof(packed.c2));
    printf("i2: offset: %zu  align: %zu\n", offsetof(packed_and_aligned_t, i2), alignof(packed.i2));
    printf("c3: offset: %zu  align: %zu\n\n", offsetof(packed_and_aligned_t, c3), alignof(packed.c3));

    printf("struct aligned_only: size: %zu  align: %zu\n", sizeof(aligned), alignof(aligned));
    printf("c1: offset: %zu  align: %zu\n", offsetof(aligned_t, c1), alignof(aligned.c1));
    printf("i1: offset: %zu  align: %zu\n", offsetof(aligned_t, i1), alignof(aligned.i1));
    printf("c2: offset: %zu  align: %zu\n", offsetof(aligned_t, c2), alignof(aligned.c2));
    printf("i2: offset: %zu  align: %zu\n", offsetof(aligned_t, i2), alignof(aligned.i2));
    printf("c3: offset: %zu  align: %zu\n\n", offsetof(aligned_t, c3), alignof(aligned.c3));

    return 0;
}


