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
#include <limits.h>
#include <assert.h>
#include "ndtypes.h"
#include "overflow.h"


#if defined(_MSC_VER)
  #pragma warning(disable : 4232)
#else
  #include "config.h"
  #ifdef WITH_VALGRIND
    #include <valgrind/memcheck.h>
  #endif
#endif

#if SIZE_MAX > UINTPTR_MAX
  #error "need SIZE_MAX <= UINTPTR_MAX"
#endif


/* Custom allocation and free functions */
void *(* ndt_mallocfunc)(size_t size) = malloc;
void *(* ndt_callocfunc)(size_t nmemb, size_t size) = calloc;
void *(* ndt_reallocfunc)(void *ptr, size_t size) = realloc;
void (* ndt_freefunc)(void *ptr) = free;


static inline int
ispower2(uint16_t n)
{
    return n != 0 && (n & (n-1)) == 0;
}

static inline size_t
convert_req(int64_t nmemb, int64_t size, bool *overflow)
{
    int64_t req;

    assert(nmemb >= 0 && size >= 0);
    if (nmemb == 0 || size == 0) {
        return 1;
    }

    req = MULi64(nmemb, size, overflow);

#if SIZE_MAX < INT64_MAX
    if (req > INT32_MAX) {
        *overflow = 1;
        return SIZE_MAX;
    }
#endif

    return (size_t)req;
}

/* malloc with overflow checking */
void *
ndt_alloc(int64_t nmemb, int64_t size)
{
    bool overflow = 0;
    size_t req;

    req = convert_req(nmemb, size, &overflow);
    if (overflow) {
        return NULL;
    }

    return ndt_mallocfunc(req);
}

void *
ndt_alloc_size(size_t size)
{
    return ndt_mallocfunc(size);
}

/* calloc */
void *
ndt_calloc(int64_t nmemb, int64_t size)
{
    assert(nmemb >= 0 && size >= 0);
    nmemb = nmemb == 0 ? 1 : nmemb;
    size = size == 0 ? 1 : size;

#if SIZE_MAX < INT64_MAX
    if (nmemb > INT32_MAX || size > INT32_MAX) {
        return NULL;
    }
#endif

    return ndt_callocfunc((size_t)nmemb, (size_t)size);
}

/* realloc with overflow checking */
void *
ndt_realloc(void *ptr, int64_t nmemb, int64_t size)
{
    bool overflow = 0;
    size_t req;

    req = convert_req(nmemb, size, &overflow);
    if (overflow) {
        return NULL;
    }

    return ndt_reallocfunc(ptr, req);
}

/* free */
void
ndt_free(void *ptr)
{
    ndt_freefunc(ptr);
}

/* aligned calloc */
void *
ndt_aligned_calloc(uint16_t alignment, int64_t size)
{
    bool overflow = 0;
    uintptr_t uintptr, aligned;
    int64_t req;
    uint32_t extra;
    void *ptr;

    if (!ispower2(alignment)) {
        return NULL;
    }

    if (alignment < MAX_ALIGN) {
        alignment = MAX_ALIGN;
    }

    if (size == 0) {
        size = 1;
    }

    extra = alignment - 1;
    extra = extra + sizeof(uintptr_t);

    req = ADDi64(size, extra, &overflow);
    if (overflow) {
        return NULL;
    }

#if SIZE_MAX < INT64_MAX
    if (req > INT32_MAX) {
        return NULL;
    }
#endif

    ptr = ndt_callocfunc((size_t)req, 1);
    if (ptr == NULL) {
        return NULL;
    }

    uintptr = (uintptr_t)ptr;
    aligned = uintptr + extra;
    if (aligned < uintptr) {
        free(ptr);
        return NULL;
    }

    aligned = aligned - (aligned % alignment);
    *((uintptr_t *)aligned - 1) = uintptr;

#ifdef WITH_VALGRIND
{
    uintptr_t leading_noaccess = aligned - uintptr;
    uintptr_t trailing_noaccess = req - (size + leading_noaccess);
    VALGRIND_MAKE_MEM_NOACCESS(uintptr, leading_noaccess);
    VALGRIND_MAKE_MEM_NOACCESS(aligned+size, trailing_noaccess);
}
#endif

    return (void *)aligned;
}

void
ndt_aligned_free(void *aligned)
{
    uintptr_t uintptr;

    if (aligned == NULL) {
        return;
    }

#ifdef WITH_VALGRIND
    VALGRIND_MAKE_MEM_DEFINED((uintptr_t *)aligned - 1, sizeof(uintptr_t));
#endif

    uintptr = *((uintptr_t *)aligned - 1);
    ndt_freefunc((void *)uintptr);
}
