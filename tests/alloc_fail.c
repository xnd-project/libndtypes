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


#include <stdlib.h>
#include "ndtypes.h"
#include "alloc_fail.h"


/* Test allocation failures */
int alloc_fail;
int alloc_idx;


void *
ndt_alloc_fail(size_t size)
{
#ifdef TEST_ALLOC
	if (++alloc_idx >= alloc_fail) {
		return NULL;
	}
#endif
	return malloc(size);
}

void *
ndt_realloc_fail(void *ptr, size_t size)
{
#ifdef TEST_ALLOC
	if (++alloc_idx >= alloc_fail) {
		return NULL;
	}
#endif
	return realloc(ptr, size);
}

void
ndt_set_alloc_fail(void)
{
	ndt_mallocfunc = ndt_alloc_fail;
	ndt_reallocfunc = ndt_realloc_fail;
	alloc_idx = 0;
}

void
ndt_set_alloc(void)
{
	ndt_mallocfunc = malloc;
	ndt_reallocfunc = realloc;
}



