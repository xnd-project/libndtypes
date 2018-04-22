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
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "ndtypes.h"
#include "test.h"
#include "alloc_fail.h"


static int
init_tests(void)
{
    ndt_context_t *ctx;
    ndt_t *t = NULL;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    if (ndt_init(ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return -1;
    }

    t = ndt_from_string("{a: size_t, b: ref(string)}", ctx);
    if (t == NULL) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return -1;
    }
    if (ndt_typedef("defined_t", t, NULL, ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return -1;
    }

    t = ndt_from_string("(10 * 2 * defined_t)", ctx);
    if (t == NULL) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return -1;
    }
    if (ndt_typedef("foo_t", t, NULL, ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        ndt_context_del(ctx);
        return -1;
    }

    ndt_context_del(ctx);
    return 0;
}

static int
test_parse(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    char *s;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            t = ndt_from_string(*c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_parse: parse: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_parse: parse: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (t == NULL) {
            fprintf(stderr, "test_parse: parse: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_parse: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            s = ndt_as_string(t, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (s != NULL) {
                ndt_free(s);
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_parse: convert: FAIL: s != NULL after MemoryError\n");
                fprintf(stderr, "test_parse: parse: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_parse: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_parse: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(s);
        ndt_del(t);
        count++;
    }
    fprintf(stderr, "test_parse (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_parse_roundtrip(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    char *s;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_roundtrip_tests; *c != NULL; c++) {
        t = ndt_from_string(*c, ctx);
        if (t == NULL) {
            fprintf(stderr, "test_parse_roundtrip: parse: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_parse_roundtrip: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        s = ndt_as_string(t, ctx);
        if (s == NULL) {
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        if (strcmp(s, *c) != 0) {
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: input:     \"%s\"\n", *c);
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: roundtrip: \"%s\"\n", s);
            ndt_free(s);
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(s);
        ndt_del(t);
        count++;
    }
    fprintf(stderr, "test_parse_roundtrip (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_parse_error(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_error_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            t = ndt_from_string(*c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_parse_error: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_parse_error: FAIL: input: %s\n", *c);
                return -1;
            }
        }
        if (t != NULL) {
            fprintf(stderr, "test_parse_error: FAIL: unexpected success: \"%s\"\n", *c);
            fprintf(stderr, "test_parse_error: FAIL: t != NULL after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }
        count++;
    }
    fprintf(stderr, "test_parse_error (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_indent(void)
{
    const indent_testcase_t *tc;
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    char *s;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_tests; *c != NULL; c++) {
        t = ndt_from_string(*c, ctx);
        if (t == NULL) {
            fprintf(stderr, "test_indent: parse: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_indent: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            s = ndt_indent(t, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (s != NULL) {
                ndt_free(s);
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_indent: convert: FAIL: s != NULL after MemoryError\n");
                fprintf(stderr, "test_indent: convert: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_indent: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_indent: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(s);
        ndt_del(t);
        count++;
    }

    for (tc = indent_tests; tc->input != NULL; tc++) {
        t = ndt_from_string(tc->input, ctx);
        if (t == NULL) {
            fprintf(stderr, "test_indent: parse: FAIL: expected success: \"%s\"\n", tc->input);
            fprintf(stderr, "test_indent: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            s = ndt_indent(t, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (s != NULL) {
                ndt_free(s);
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_indent: convert: FAIL: s != NULL after MemoryError\n");
                fprintf(stderr, "test_indent: convert: FAIL: %s\n", tc->input);
                return -1;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_indent: convert: FAIL: expected success: \"%s\"\n", tc->input);
            fprintf(stderr, "test_indent: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        if (strcmp(s, tc->indented) != 0) {
            fprintf(stderr, "test_indent: convert: FAIL: expected success: \"%s\"\n", tc->input);
            fprintf(stderr, "test_indent: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(s);
        ndt_del(t);
        count++;
    }

    fprintf(stderr, "test_indent (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_typedef(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = typedef_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            t = ndt_from_string("10 * 20 * {a : int64, b : ref(float64)}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*c, t, NULL, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*c, ctx) != NULL) {
                fprintf(stderr, "test_typedef: FAIL: key in map after MemoryError\n");
                fprintf(stderr, "test_typedef: FAIL: input: %s\n", *c);
                ndt_context_del(ctx);
                return -1;
            }
        }

        if (ndt_typedef_find(*c, ctx) == NULL) {
            fprintf(stderr, "test_typedef: FAIL: key not found: \"%s\"\n", *c);
            fprintf(stderr, "test_typedef: FAIL: lookup failed after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        count++;
    }

    fprintf(stderr, "test_typedef (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_typedef_duplicates(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = typedef_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            t = ndt_from_string("10 * 20 * {a : int64, b : ref(float64)}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*c, t, NULL, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*c, ctx) == NULL) {
                fprintf(stderr, "test_typedef: FAIL: key should be in map\n");
                fprintf(stderr, "test_typedef: FAIL: input: %s\n", *c);
                ndt_context_del(ctx);
                return -1;
            }
        }

        if (ctx->err != NDT_ValueError) {
            fprintf(stderr, "test_typedef: FAIL: no value error after duplicate key\n");
            fprintf(stderr, "test_typedef: FAIL: input: %s\n", *c);
            ndt_context_del(ctx);
            return -1;
        }

        count++;
    }

    fprintf(stderr, "test_typedef_duplicates (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_typedef_error(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = typedef_error_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            t = ndt_from_string("10 * 20 * {a : int64, b : ref(float64)}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*c, t, NULL, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*c, ctx) != NULL) {
                fprintf(stderr, "test_typedef_error: FAIL: key in map after MemoryError\n");
                fprintf(stderr, "test_typedef: FAIL: input: %s\n", *c);
                ndt_context_del(ctx);
                return -1;
            }
        }

        if (ndt_typedef_find(*c, ctx) != NULL) {
            fprintf(stderr, "test_typedef_error: FAIL: unexpected success: \"%s\"\n", *c);
            fprintf(stderr, "test_typedef_error: FAIL: key in map after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        count++;
    }

    fprintf(stderr, "test_typedef_error (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_equal(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t, *u;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_roundtrip_tests; *c && *(c+1); c++) {
        ndt_err_clear(ctx);

        t = ndt_from_string(*c, ctx);
        if (t == NULL) {
            ndt_context_del(ctx);
            fprintf(stderr, "test_equal: FAIL: could not parse \"%s\"\n", *c);
            return -1;
        }

        u = ndt_from_string(*(c+1), ctx);
        if (u == NULL) {
            ndt_del(t);
            ndt_context_del(ctx);
            fprintf(stderr, "test_equal: FAIL: could not parse \"%s\"\n", *(c+1));
            return -1;
        }

        if (!ndt_equal(t, t)) {
            ndt_del(t);
            ndt_del(u);
            ndt_context_del(ctx);
            fprintf(stderr, "test_equal: FAIL: \"%s\" != \"%s\"\n", *c, *c);
            return -1;
        }

        if (ndt_equal(t, u)) {
            ndt_del(t);
            ndt_del(u);
            fprintf(stderr, "test_equal: FAIL: \"%s\" == \"%s\"\n", *c, *(c+1));
            return -1;
        }

        ndt_del(t);
        ndt_del(u);
        count++;
    }

    fprintf(stderr, "test_equal (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_match(void)
{
    const match_testcase_t *t;
    ndt_context_t *ctx;
    ndt_t *p;
    ndt_t *c;
    int ret, count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (t = match_tests; t->pattern != NULL; t++) {
        p = ndt_from_string(t->pattern, ctx);
        if (p == NULL) {
            fprintf(stderr, "test_match: FAIL: could not parse \"%s\"\n", t->pattern);
            ndt_context_del(ctx);
            return -1;
        }

        c = ndt_from_string(t->candidate, ctx);
        if (c == NULL) {
            ndt_del(p);
            ndt_context_del(ctx);
            fprintf(stderr, "test_match: FAIL: could not parse \"%s\"\n", t->candidate);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            ret = ndt_match(p, c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ret != -1) {
                ndt_del(p);
                ndt_del(c);
                ndt_context_del(ctx);
                fprintf(stderr, "test_match: FAIL: expect ret == -1 after MemoryError\n");
                fprintf(stderr, "test_match: FAIL: \"%s\"\n", t->pattern);
                return -1;
            }
        }

        if (ret != t->expected) {
            ndt_del(p);
            ndt_del(c);
            ndt_context_del(ctx);
            fprintf(stderr, "test_match: FAIL: expected %s\n", t->expected ? "true" : "false");
            fprintf(stderr, "test_match: FAIL: pattern: \"%s\"\n", t->pattern);
            fprintf(stderr, "test_match: FAIL: candidate: \"%s\"\n", t->candidate);
            return -1;
        }

        ndt_del(p);
        ndt_del(c);
        count++;
    }
    fprintf(stderr, "test_match (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

typedef struct {
    int64_t size;
    ndt_t *types[NDT_MAX_ARGS];
} type_array_t;

static type_array_t
types_from_string(const char * const *s, ndt_context_t *ctx)
{
    type_array_t t;
    int i;

    t.size = -1;

    for (i = 0; s[i] != NULL; i++) {
        t.types[i] = ndt_from_string(s[i], ctx);
        if (t.types[i] == NULL) {
            ndt_type_array_clear(t.types, i);
            return t;
        }
    }

    t.size = i;
    return t;
}

static int
validate_typecheck_test(const typecheck_testcase_t *test,
                        const ndt_t *sig,
                        ndt_apply_spec_t *spec,
                        int ret,
                        ndt_context_t *ctx)
{
    type_array_t out;
    type_array_t broadcast;
    int64_t i;

    if (!test->success) {
        if (ret == -1) {
            return 0;
        }
        ndt_err_format(ctx, NDT_RuntimeError,
            "test_typecheck: unexpected success: %s  %s  %s",
             test->signature, test->in[0], test->in[1]);
        return -1;
    }

    out = types_from_string(test->out, ctx);
    if (out.size < 0) {
        return -1;
    }

    broadcast = types_from_string(test->broadcast, ctx);
    if (broadcast.size < 0) {
        ndt_type_array_clear(broadcast.types, broadcast.size);
        return -1;
    }

    if (spec->nout != sig->Function.nout ||
        spec->nout != out.size) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "test_typecheck: expected nout==test->nout==%d, got nout==%d and "
            "test->nout==%d\n", sig->Function.nout, spec->nout, out.size);
        return -1;
    }

    if (spec->nbroadcast != broadcast.size) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "test_typecheck: expected nbroadcast==%d, got %d\n",
            broadcast.size, spec->nbroadcast);
        return -1;
    }

    if (spec->outer_dims != test->outer_dims) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "test_typecheck: expected outer_dims==%d, got %d\n",
            test->outer_dims, spec->outer_dims);
        return -1;
    }

    for (i = 0; i < spec->nout; i++) {
        if (!ndt_equal(spec->out[i], out.types[i])) {
            ndt_err_format(ctx, NDT_RuntimeError,
                "test_typecheck: out value %d: expected %s, got %s",
                i, test->out[i], ndt_ast_repr(spec->out[i], ctx));
            ndt_type_array_clear(out.types, out.size);
            ndt_type_array_clear(broadcast.types, broadcast.size);
            return -1;
        }
    }

    for (i = 0; i < spec->nbroadcast; i++) {
        if (!ndt_equal(spec->broadcast[i], broadcast.types[i])) {
            ndt_err_format(ctx, NDT_RuntimeError,
                "test_typecheck: broadcast value %d: expected %s, got %s",
                i, test->broadcast[i], ndt_ast_repr(spec->broadcast[i], ctx));
            ndt_type_array_clear(out.types, out.size);
            ndt_type_array_clear(broadcast.types, broadcast.size);
            return -1;
        }
    }

    ndt_type_array_clear(out.types, out.size);
    ndt_type_array_clear(broadcast.types, broadcast.size);

    return 0;
}

static int
test_typecheck(void)
{
    NDT_STATIC_CONTEXT(ctx);
    ndt_apply_spec_t spec = ndt_apply_spec_empty;
    const typecheck_testcase_t *test;
    const ndt_t *sig = NULL;
    type_array_t in;
    int count = 0;
    int ret = -1;

    for (test = typecheck_tests; test->signature != NULL; test++) {
        sig = ndt_from_string(test->signature, &ctx);
        if (sig == NULL) {
            ndt_err_format(&ctx, NDT_RuntimeError,
                "test_typecheck: could not parse \"%s\"\n", test->signature);
            goto error;
        }

        in = types_from_string(test->in, &ctx);
        if (in.size < 0) {
            goto error;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(&ctx);

            ndt_set_alloc_fail();
            ret = ndt_typecheck(&spec, sig, (const ndt_t **)in.types, in.size, NULL, NULL, &ctx);
            ndt_set_alloc();

            if (ctx.err != NDT_MemoryError) {
                break;
            }

            ndt_apply_spec_clear(&spec);
            if (ret != -1) {
                ndt_err_format(&ctx, NDT_RuntimeError,
                    "test_typecheck: expect nout == -1 after MemoryError\n"
                    "test_typecheck: \"%s\"\n", test->signature);
                goto error;
            }
        }

        ndt_err_clear(&ctx);
        ret = validate_typecheck_test(test, sig, &spec, ret, &ctx);
        ndt_type_array_clear(in.types, in.size);
        ndt_apply_spec_clear(&spec);
        ndt_del((ndt_t *)sig);

        if (ret < 0) {
            goto error;
        }

        count++;
    }

    ret = 0;
    fprintf(stderr, "test_typecheck (%d test cases)\n", count);


out:
    ndt_context_del(&ctx);
    return ret;

error:
   ret = -1;
   ndt_err_fprint(stderr, &ctx);
   goto out;
}

static int
test_numba(void)
{
    NDT_STATIC_CONTEXT(ctx);
    const numba_testcase_t *test;
    ndt_t *t;
    char *sig = NULL;
    char *core = NULL;
    int count = 0;
    int ret = -1;

    for (test = numba_tests; test->signature != NULL; test++) {
        t = ndt_from_string(test->signature, &ctx);
        if (t == NULL) {
            ndt_err_format(&ctx, NDT_RuntimeError,
                "test_numba: could not parse \"%s\"\n", test->signature);
            goto error;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(&ctx);

            ndt_set_alloc_fail();
            ret = ndt_to_nbformat(&sig, &core, t, &ctx);
            ndt_set_alloc();

            if (ctx.err != NDT_MemoryError) {
                break;
            }

            if (ret != -1) {
                ndt_del(t);
                ndt_err_format(&ctx, NDT_RuntimeError,
                    "test_numba: expect ret == -1 after MemoryError\n"
                    "test_numba: \"%s\"\n", test->signature);
                goto error;
            }
        }

        ndt_err_clear(&ctx);
        ndt_del(t);

        if (sig == NULL || strcmp(sig, test->sig) != 0) {
            ndt_err_format(&ctx, NDT_RuntimeError,
                "test_numba: input: \"%s\" output: \"%s\"\n", test->sig, sig);
            goto error;
        }

        if (core == NULL || strcmp(core, test->core) != 0) {
            ndt_err_format(&ctx, NDT_RuntimeError,
                "test_numba: input: \"%s\" output: \"%s\"\n", test->core, core);
            goto error;
        }

        ndt_free(sig);
        ndt_free(core);
        sig = core = NULL;

        count++;
    }

    ret = 0;
    fprintf(stderr, "test_numba (%d test cases)\n", count);


out:
    ndt_context_del(&ctx);
    return ret;

error:
   ret = -1;
   ndt_err_fprint(stderr, &ctx);
   goto out;
}

static int
test_static_context(void)
{
    const char **c;
    NDT_STATIC_CONTEXT(ctx);
    ndt_t *t;
    char *s;
    int count = 0;

    for (c = parse_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(&ctx);

            ndt_set_alloc_fail();
            t = ndt_from_string(*c, &ctx);
            ndt_set_alloc();

            if (ctx.err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                fprintf(stderr, "test_static_context: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_static_context: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (t == NULL) {
            fprintf(stderr, "test_static_context: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_static_context: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx.err),
                    ndt_context_msg(&ctx));
            ndt_context_del(&ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(&ctx);

            ndt_set_alloc_fail();
            s = ndt_as_string(t, &ctx);
            ndt_set_alloc();

            if (ctx.err != NDT_MemoryError) {
                break;
            }

            if (s != NULL) {
                ndt_free(s);
                ndt_del(t);
                fprintf(stderr, "test_static_context: FAIL: s != NULL after MemoryError\n");
                fprintf(stderr, "test_static_context: FAIL: %s\n", *c);
                ndt_context_del(&ctx);
                return -1;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_static_context: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_static_context: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx.err),
                    ndt_context_msg(&ctx));
            ndt_del(t);
            ndt_context_del(&ctx);
            return -1;
        }

        ndt_free(s);
        ndt_del(t);
        count++;
    }

    s = "2 * 1000000000000000000000000000 * complex128";
    t = ndt_from_string(s, &ctx);
    if (s == NULL) {
        fprintf(stderr, "test_static_context: FAIL: expected failure: \"%s\"\n", s);
        ndt_del(t);
        ndt_context_del(&ctx);
        return -1;
    }
    count++;

    ndt_context_del(&ctx);
    fprintf(stderr, "test_static_context (%d test cases)\n", count);

    return 0;
}

typedef struct {
    const char *str;
    ndt_ssize_t hash;
} hash_testcase_t;

static int
cmp_hash_testcase(const void *x, const void *y)
{
    const hash_testcase_t *p = (const hash_testcase_t *)x;
    const hash_testcase_t *q = (const hash_testcase_t *)y;

    if (p->hash < q->hash) {
        return -1;
    }
    else if (p->hash == q->hash) {
        return 0;
    }

    return 1;
}

static int
test_hash(void)
{
    NDT_STATIC_CONTEXT(ctx);
    hash_testcase_t buf[1000];
    ptrdiff_t n = 1;
    const char **c;
    ndt_t *t;
    hash_testcase_t x;
    int i;

    for (c = parse_roundtrip_tests; *c != NULL; c++) {
        n = c - parse_roundtrip_tests;
        if (n >= 1000) {
            break;
        }

        ndt_err_clear(&ctx);

        t = ndt_from_string(*c, &ctx);
        if (t == NULL) {
            fprintf(stderr, "test_hash: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_hash: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx.err),
                    ndt_context_msg(&ctx));
            ndt_context_del(&ctx);
            return -1;
        }

        x.hash = ndt_hash(t, &ctx);
        x.str = *c;
        ndt_del(t);

        if (x.hash == -1) {
            fprintf(stderr, "test_hash: FAIL: hash==-1\n\n");
            ndt_context_del(&ctx);
            return -1;
        }

        buf[n] = x;
    }

    qsort(buf, n, sizeof *buf, cmp_hash_testcase);
    for (i = 0; i < n-1; i++) {
        if (buf[i].hash == buf[i+1].hash) {
            fprintf(stderr,
                "test_hash: duplicate hash for %s: %" PRI_ndt_ssize "\n\n",
                buf[i].str, buf[i].hash);
        }
    }

    t = ndt_from_string("var * {a: float64, b: string}", &ctx);
    if (t == NULL) {
        fprintf(stderr, "test_hash: FAIL: expected success\n\n");
        ndt_context_del(&ctx);
        return -1;
    }

    alloc_fail = 1;
    ndt_set_alloc_fail();
    x.hash = ndt_hash(t, &ctx);
    ndt_set_alloc();

    ndt_del(t);

    if (x.hash != -1 || ctx.err != NDT_MemoryError) {
        fprintf(stderr, "test_hash: FAIL: expected failure, got %" PRI_ndt_ssize "\n\n", x.hash);
        ndt_context_del(&ctx);
        return -1;
    }

    ndt_context_del(&ctx);
    fprintf(stderr, "test_hash (%d test cases)\n", (int)n);

    return 0;
}

static int
test_copy(void)
{
    NDT_STATIC_CONTEXT(ctx);
    const char **c;
    ndt_t *t, *u;
    int count = 0;

    for (c = parse_tests; *c != NULL; c++) {
        t = ndt_from_string(*c, &ctx);
        if (t == NULL) {
            fprintf(stderr, "test_copy: FAIL: from_string: \"%s\"\n", *c);
            fprintf(stderr, "test_copy: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx.err),
                    ndt_context_msg(&ctx));
            ndt_context_del(&ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(&ctx);

            ndt_set_alloc_fail();
            u = ndt_copy(t, &ctx);
            ndt_set_alloc();

            if (ctx.err != NDT_MemoryError) {
                break;
            }

            if (u != NULL) {
                fprintf(stderr, "test_copy: FAIL: unexpected success: \"%s\"\n", *c);
                ndt_del(u);
                ndt_del(t);
                ndt_context_del(&ctx);
                return -1;
            }
        }
        if (u == NULL) {
            fprintf(stderr, "test_copy: FAIL: copying failed: \"%s\"\n", *c);
            fprintf(stderr, "test_copy: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx.err),
                    ndt_context_msg(&ctx));
            ndt_del(t);
            ndt_context_del(&ctx);
            return -1;
        }

        if (!ndt_equal(t, u)) {
            fprintf(stderr, "test_copy: FAIL: copy: not equal\n\n");
            ndt_del(t);
            ndt_del(u);
            ndt_context_del(&ctx);
            return -1;
        }

        ndt_del(u);
        ndt_del(t);
        count++;
    }

    ndt_context_del(&ctx);
    fprintf(stderr, "test_copy (%d test cases)\n", count);

    return 0;
}

static int
test_buffer(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = buffer_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            t = ndt_from_bpformat(*c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_buffer: convert: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_buffer: convert: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (t == NULL) {
            fprintf(stderr, "test_buffer: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_buffer: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        ndt_del(t);
        count++;
    }
    fprintf(stderr, "test_buffer (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_buffer_roundtrip(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    char *s;
    int count = 0;
    int ret;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = buffer_roundtrip_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            t = ndt_from_bpformat(*c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_buffer: convert: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_buffer: convert: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (t == NULL) {
            fprintf(stderr, "test_buffer: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_buffer: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_context_del(ctx);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            s = ndt_to_bpformat(t, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (s != NULL) {
                ndt_free(s);
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_buffer_roundtrip: convert: FAIL: s != NULL after MemoryError\n");
                fprintf(stderr, "test_buffer_roundtrip: convert: FAIL: %s\n", *c);
                return -1;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_buffer_roundtrip: convert: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_buffer_roundtrip: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }

        ret = strcmp(s, *c);
        ndt_del(t);

        if (ret != 0) {
            fprintf(stderr, "test_buffer_roundtrip: convert: FAIL: input: \"%s\" output: \"%s\"\n", *c, s);
            ndt_free(s);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(s);
        count++;
    }
    fprintf(stderr, "test_buffer_roundtrip (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_buffer_error(void)
{
    const char **c;
    ndt_context_t *ctx;
    ndt_t *t;
    int count = 0;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = buffer_error_tests; *c != NULL; c++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            t = ndt_from_bpformat(*c, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (t != NULL) {
                ndt_del(t);
                ndt_context_del(ctx);
                fprintf(stderr, "test_buffer_error: FAIL: t != NULL after MemoryError\n");
                fprintf(stderr, "test_buffer_error: FAIL: input: %s\n", *c);
                return -1;
            }
        }
        if (t != NULL) {
            fprintf(stderr, "test_buffer_error: FAIL: unexpected success: \"%s\"\n", *c);
            fprintf(stderr, "test_buffer_error: FAIL: t != NULL after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_context_del(ctx);
            return -1;
        }
        count++;
    }
    fprintf(stderr, "test_buffer_error (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

static int
test_serialize(void)
{
    const char **c;
    ndt_meta_t *m;
    ndt_context_t *ctx;
    ndt_t *t, *u;
    int count = 0;
    char *bytes;
    int64_t len;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        return -1;
    }

    for (c = parse_tests; *c != NULL; c++) {
        m = ndt_meta_new(ctx);
        if (m == NULL) {
            ndt_context_del(ctx);
            fprintf(stderr,
                "test_serialize: FAIL: unexpected failure in meta_new");
            return -1;
        }

        t = ndt_from_string(*c, ctx);
        if (t == NULL) {
            ndt_meta_del(m);
            ndt_context_del(ctx);
            fprintf(stderr,
                "test_serialize: FAIL: unexpected failure in from_string");
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            bytes = NULL;

            ndt_set_alloc_fail();
            len = ndt_serialize(&bytes, t, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (len >= 0 || bytes != NULL) {
                ndt_del(t);
                ndt_free(bytes);
                ndt_meta_del(m);
                ndt_context_del(ctx);
                fprintf(stderr, "test_serialize: FAIL: invalid len or bytes after MemoryError\n");
                fprintf(stderr, "test_serialize: FAIL: %s\n", *c);
                return -1;
            }
        }

        if (len < 0 || bytes == NULL) {
            ndt_del(t);
            ndt_free(bytes);
            ndt_meta_del(m);
            ndt_context_del(ctx);
            fprintf(stderr, "test_serialize: FAIL: invalid len or bytes (expected success)\n");
            fprintf(stderr, "test_serialize: FAIL: %s\n", *c);
            return -1;
        }

        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);

            ndt_set_alloc_fail();
            u = ndt_deserialize(m, bytes, len, ctx);
            ndt_set_alloc();

            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (u != NULL) {
                ndt_del(t);
                ndt_del(u);
                ndt_free(bytes);
                ndt_meta_del(m);
                ndt_context_del(ctx);
                fprintf(stderr, "test_serialize: FAIL: u != NULL after MemoryError\n");
                fprintf(stderr, "test_serialize: FAIL: %s\n", *c);
                return -1;
            }
        }

        if (u == NULL) {
            fprintf(stderr, "test_serialize: FAIL: expected success: \"%s\"\n", *c);
            fprintf(stderr, "test_serialize: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            ndt_free(bytes);
            ndt_meta_del(m);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_free(bytes);

        if (!ndt_equal(u, t)) {
            fprintf(stderr, "test_serialize: FAIL: u != v in %s\n", *c);
            ndt_del(t);
            ndt_del(u);
            ndt_meta_del(m);
            ndt_context_del(ctx);
            return -1;
        }

        ndt_del(t);
        ndt_del(u);

        ndt_meta_del(m);
        count++;
    }
    fprintf(stderr, "test_serialize (%d test cases)\n", count);

    ndt_context_del(ctx);
    return 0;
}

#if defined(__linux__)
static int
test_serialize_fuzz(void)
{
    ndt_context_t *ctx;
    ndt_meta_t *m;
    char *buf;
    ssize_t ret, n;
    int src;
    int64_t i;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "\nmalloc error in fuzz tests\n");
        return -1;
    }

    src = open("/dev/urandom", O_RDONLY);
    if (src < 0) {
        ndt_context_del(ctx);
        fprintf(stderr, "\ncould not open /dev/urandom\n");
        return -1;
    }

    for (i = 0; i < 10000; i++) {
        n = rand() % 1000;
        buf = ndt_alloc(n, 1);
        if (buf == NULL) {
            close(src);
            ndt_context_del(ctx);
            fprintf(stderr, "malloc error in fuzz tests\n");
            return -1;
        }

        ret = read(src, buf, n);
        if (ret < 0) {
            ndt_free(buf);
            close(src);
            ndt_context_del(ctx);
            fprintf(stderr, "\nread error in fuzz tests\n");
            return -1;
        }

        m = ndt_meta_new(ctx);
        if (m == NULL) {
            ndt_free(buf);
            close(src);
            ndt_context_del(ctx);
            fprintf(stderr, "\nmalloc error in fuzz tests\n");
            return -1;
        }

        ndt_err_clear(ctx);
        ndt_t *t = ndt_deserialize(m, buf, n, ctx);
        if (t != NULL) {
            ndt_del(t);
        }

        ndt_free(buf);
        ndt_meta_del(m);
    }
    fprintf(stderr, "test_serialize_fuzz (%" PRIi64 " test cases)\n", i);

    close(src);
    ndt_context_del(ctx);
    return 0;
}
#endif

static int (*tests[])(void) = {
  test_parse,
  test_parse_roundtrip,
  test_parse_error,
  test_indent,
  test_typedef,
  test_typedef_duplicates,
  test_typedef_error,
  test_equal,
  test_match,
  test_typecheck,
  test_numba,
  test_static_context,
  test_hash,
  test_copy,
  test_buffer,
  test_buffer_roundtrip,
  test_buffer_error,
  test_serialize,
#ifdef __linux__
  test_serialize_fuzz,
#endif
#ifdef __GNUC__
  test_struct_align_pack,
  test_array,
#endif
  NULL
};

int
main(void)
{
    int (**f)(void);
    int success = 0;
    int fail = 0;

    if (init_tests() < 0) {
        return 1;
    }

    for (f = tests; *f != NULL; f++) {
        if ((*f)() < 0)
            fail++;
        else
            success++;
    }

    if (fail) {
        fprintf(stderr, "\nFAIL (failures=%d)\n", fail);
    }
    else {
        fprintf(stderr, "\n%d tests OK.\n", success);
    }

    ndt_finalize();
    return fail ? 1 : 0;
}
