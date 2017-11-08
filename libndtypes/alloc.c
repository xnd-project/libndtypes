/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, plures
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
#include "ndtypes.h"


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
ispower2(size_t n)
{
    return n != 0 && (n & (n-1)) == 0;
}

static inline size_t
mul_size_t_overflow(size_t nmemb, size_t size, int *overflow)
{
    *overflow = 0;

    if (nmemb == 0 || size == 0) {
        return 1;
    }

    if (nmemb > SIZE_MAX / size) {
        *overflow = 1;
        return SIZE_MAX;
    }

    return nmemb * size;
}

/* malloc with overflow checking */
void *
ndt_alloc(size_t nmemb, size_t size)
{
    size_t req;
    int overflow;

    req = mul_size_t_overflow(nmemb, size, &overflow);
    if (overflow) {
        return NULL;
    }

    return ndt_mallocfunc(req);
}

/* calloc */
void *
ndt_calloc(size_t nmemb, size_t size)
{
    nmemb = nmemb == 0 ? 1 : nmemb;
    size = size == 0 ? 1 : size;

    return ndt_callocfunc(nmemb, size);
}

/* realloc with overflow checking */
void *
ndt_realloc(void *ptr, size_t nmemb, size_t size)
{
    size_t req;
    int overflow;

    req = mul_size_t_overflow(nmemb, size, &overflow);
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
ndt_aligned_calloc(size_t alignment, size_t size)
{
    uintptr_t uintptr, aligned;
    size_t req, extra;
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
    if (extra < sizeof(uintptr_t)) {
        return NULL;
    }

    req = size + extra;
    if (req < size) {
        return NULL;
    }

    ptr = ndt_callocfunc(req, 1);
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

#ifdef HAVE_VALGRIND
    VALGRIND_MAKE_MEM_DEFINED((uintptr_t *)aligned - 1, sizeof(uintptr_t));
#endif

    uintptr = *((uintptr_t *)aligned - 1);
    ndt_freefunc((void *)uintptr);
}
