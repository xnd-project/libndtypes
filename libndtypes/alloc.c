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
#include "ndtypes.h"


#if defined(_MSC_VER)
  #pragma warning(disable : 4232)
#endif


/* Custom allocation and free functions */
void *(* ndt_mallocfunc)(size_t size) = malloc;
void *(* ndt_alignedallocfunc)(size_t alignment, size_t size) = aligned_alloc;
void *(* ndt_callocfunc)(size_t nmemb, size_t size) = calloc;
void *(* ndt_reallocfunc)(void *ptr, size_t size) = realloc;
void (* ndt_freefunc)(void *ptr) = free;


void
ndt_free(void *ptr)
{
    ndt_freefunc(ptr);
}

/* malloc with overflow checking */
void *
ndt_alloc(size_t nmemb, size_t size)
{
    if (size > SIZE_MAX / nmemb) {
        return NULL;
    }

    return ndt_mallocfunc(nmemb * size);
}

/* aligned malloc with overflow checking */
void *
ndt_aligned_alloc(size_t alignment, size_t nmemb, size_t size)
{
    if (size > SIZE_MAX / nmemb) {
        return NULL;
    }

    return ndt_alignedallocfunc(alignment, nmemb * size);
}

/* calloc */
void *
ndt_calloc(size_t nmemb, size_t size)
{
    return ndt_callocfunc(nmemb, size);
}

/* realloc with overflow checking */
void *
ndt_realloc(void *ptr, size_t nmemb, size_t size)
{
    if (size > SIZE_MAX / nmemb) {
        return NULL;
    }

    return ndt_reallocfunc(ptr, nmemb * size);
}
