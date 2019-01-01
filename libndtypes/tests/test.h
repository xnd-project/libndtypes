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


#ifndef TEST_H
#define TEST_H


#include "ndtypes.h"



#define XSTRINGIZE(v) #v
#define STRINGIZE(v) XSTRINGIZE(v)

#define loc() __FILE__ ":" STRINGIZE(__LINE__)


typedef struct {
    const char *input;
    const char *indented;
} indent_testcase_t;

typedef struct {
    const char *pattern;
    const char *candidate;
    int expected;
} match_testcase_t;

typedef struct {
    const char *t1;
    const char *t2;
    const char *expected;
} unify_testcase_t;

typedef struct {
    const char *loc;
    bool success;

    const char *signature;
    const char *args[NDT_MAX_ARGS];
    const char *kwargs[NDT_MAX_ARGS];

    int outer_dims;
    int nin;
    int nout;
    int nargs;
    const char *types[NDT_MAX_ARGS];
} typecheck_testcase_t;

typedef struct {
    const char *signature;
    const char *sig;
    const char *core;
} numba_testcase_t;

extern const char *parse_tests[];
extern const char *parse_tests[];
extern const char *parse_roundtrip_tests[];
extern const char *parse_error_tests[];
extern const indent_testcase_t indent_tests[];
extern const char *typedef_tests[];
extern const char *typedef_error_tests[];
extern const match_testcase_t match_tests[];
extern const unify_testcase_t unify_tests[];
extern const typecheck_testcase_t typecheck_tests[];
extern const numba_testcase_t numba_tests[];
extern const char *buffer_tests[];
extern const char *buffer_roundtrip_tests[];
extern const char *buffer_error_tests[];

int test_struct_align_pack(void);
int test_array(void);


#endif /* TEST_H */
