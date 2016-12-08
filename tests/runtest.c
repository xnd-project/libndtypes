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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "alloc_fail.h"


extern const char *ndt_test_parse[];
extern const char *ndt_test_parse_roundtrip[];
extern const char *ndt_test_parse_error[];
extern const char *ndt_test_typedef[];
extern const char *ndt_test_typedef_error[];

int
main(void)
{
    ndt_context_t *ctx;
    const char **input;
    int test_parse = 0;
    int test_parse_roundtrip = 0;
    int test_parse_error = 0;
    int test_typedef = 0;
    int test_typedef_duplicates = 0;
    int test_typedef_error = 0;
    int test_equality = 0;
    char *s;
    ndt_t *t = NULL;
    ndt_t *u = NULL;
    int ret = 1;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "error: out of memory");
        exit(1);
    }

    if (ndt_init(ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        goto out;
    }

    t = ndt_from_string("(10 * 2 * int64) -> {a: size, b: pointer[string]}", ctx);
    if (t == NULL) {
        ndt_err_fprint(stderr, ctx);
        goto out;
    }
    if (ndt_typedef("defined_t", t, ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        goto out;
    }

    t = ndt_from_string("(10 * 2 * defined_t) -> defined_t", ctx);
    if (t == NULL) {
        ndt_err_fprint(stderr, ctx);
        goto out;
    }
    if (ndt_typedef("d", t, ctx) < 0) {
        ndt_err_fprint(stderr, ctx);
        goto out;
    }

    /******************************************************************/
    /*                             Parser                             */
    /******************************************************************/

    /* Test valid input */
    for (input = ndt_test_parse; *input != NULL; input++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            ndt_set_alloc_fail();
            t = ndt_from_string(*input, ctx);
            ndt_set_alloc();
            if (ctx->err != NDT_MemoryError) {
                break;
            }
            if (t != NULL) {
                ndt_del(t);
                fprintf(stderr, "test_parse: parse: FAIL: t != NULL after MemoryError\n");
                goto out;
            }
        }
        if (t == NULL) {
            fprintf(stderr, "test_parse: parse: FAIL: expected success: \"%s\"\n", *input);
            fprintf(stderr, "test_parse: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            goto out;
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
                fprintf(stderr, "test_parse: convert: FAIL: s != NULL after MemoryError\n");
                goto out;
            }
        }
        if (s == NULL) {
            fprintf(stderr, "test_parse: convert: FAIL: expected success: \"%s\"\n", *input);
            fprintf(stderr, "test_parse: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            goto out;
        }

        ndt_free(s);
        ndt_del(t);
        test_parse++;
    }
    fprintf(stderr, "test_parse: PASS (%d test cases)\n", test_parse);

    /* Test roundtrip (valid input) */
    for (input = ndt_test_parse_roundtrip; *input != NULL; input++) {
        t = ndt_from_string(*input, ctx);
        if (t == NULL) {
            fprintf(stderr, "test_parse_roundtrip: parse: FAIL: expected success: \"%s\"\n", *input);
            fprintf(stderr, "test_parse_roundtrip: parse: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            goto out;
        }

        s = ndt_as_string(t, ctx);
        if (s == NULL) {
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: expected success: \"%s\"\n", *input);
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: got: %s: %s\n\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            ndt_del(t);
            goto out;
        }

        if (strcmp(s, *input) != 0) {
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: input:     \"%s\"\n", *input);
            fprintf(stderr, "test_parse_roundtrip: convert: FAIL: roundtrip: \"%s\"\n", s);
            ndt_free(s);
            ndt_del(t);
            goto out;
        }

        ndt_free(s);
        ndt_del(t);
        test_parse_roundtrip++;
    }
    fprintf(stderr, "test_parse_roundtrip: PASS (%d test cases)\n", test_parse_roundtrip);

    /* Test invalid input */
    for (input = ndt_test_parse_error; *input != NULL; input++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            ndt_set_alloc_fail();
            t = ndt_from_string(*input, ctx);
            ndt_set_alloc();
            if (ctx->err != NDT_MemoryError) {
                break;
            }
            if (t != NULL) {
                ndt_del(t);
                fprintf(stderr, "test_parse_error: FAIL: t != NULL after MemoryError\n");
                goto out;
            }
        }
        if (t != NULL) {
            ndt_del(t);
            fprintf(stderr, "test_parse_error: FAIL: unexpected success: \"%s\"\n", *input);
            fprintf(stderr, "test_parse_error: FAIL: t != NULL after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            goto out;
        }
        test_parse_error++;
    }
    fprintf(stderr, "test_parse_error: PASS (%d test cases)\n", test_parse_error);


    /******************************************************************/
    /*                          Typedef table                         */
    /******************************************************************/

    /* Test valid names */
    for (input = ndt_test_typedef; *input != NULL; input++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            t = ndt_from_string("10 * 20 * {a : int64, b : pointer[float64]}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*input, t, ctx);
            ndt_set_alloc();
            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*input, ctx) != NULL) {
                fprintf(stderr, "test_typedef: FAIL: key in map after MemoryError\n");
                goto out;
            }
        }
        if (ndt_typedef_find(*input, ctx) == NULL) {
            fprintf(stderr, "test_typedef: FAIL: key not found: \"%s\"\n", *input);
            fprintf(stderr, "test_typedef: FAIL: lookup failed after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            goto out;
        }
        test_typedef++;
    }
    fprintf(stderr, "test_typedef: PASS (%d test cases)\n", test_typedef);

    /* Test duplicate names */
    for (input = ndt_test_typedef; *input != NULL; input++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            t = ndt_from_string("10 * 20 * {a : int64, b : pointer[float64]}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*input, t, ctx);
            ndt_set_alloc();
            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*input, ctx) == NULL) {
                fprintf(stderr, "test_typedef: FAIL: key should be in map\n");
                goto out;
            }
        }
        if (ctx->err != NDT_ValueError) {
            fprintf(stderr, "test_typedef: FAIL: no value error after duplicate key: \"%s\"\n", *input);
            goto out;
        }
        test_typedef_duplicates++;
    }
    fprintf(stderr, "test_typedef_duplicates: PASS (%d test cases)\n", test_typedef_duplicates);

    /* Test invalid names */
    for (input = ndt_test_typedef_error; *input != NULL; input++) {
        for (alloc_fail = 1; alloc_fail < INT_MAX; alloc_fail++) {
            ndt_err_clear(ctx);
            t = ndt_from_string("10 * 20 * {a : int64, b : pointer[float64]}", ctx);

            ndt_set_alloc_fail();
            (void)ndt_typedef(*input, t, ctx);
            ndt_set_alloc();
            if (ctx->err != NDT_MemoryError) {
                break;
            }

            if (ndt_typedef_find(*input, ctx) != NULL) {
                fprintf(stderr, "test_typedef_error: FAIL: key in map after MemoryError\n");
                goto out;
            }
        }
        if (ndt_typedef_find(*input, ctx) != NULL) {
            fprintf(stderr, "test_typedef_error: FAIL: unexpected success: \"%s\"\n", *input);
            fprintf(stderr, "test_typedef_error: FAIL: key in map after %s: %s\n",
                    ndt_err_as_string(ctx->err),
                    ndt_context_msg(ctx));
            goto out;
        }
        test_typedef_error++;
    }
    fprintf(stderr, "test_typedef_error: PASS (%d test cases)\n", test_typedef_error);


    /******************************************************************/
    /*                          Type equality                         */
    /******************************************************************/

    /* Test equality */
    for (input = ndt_test_parse_roundtrip; *input && *(input+1); input++) {
        ndt_err_clear(ctx);
        t = ndt_from_string(*input, ctx);
        if (t == NULL) {
            fprintf(stderr, "test_equality: FAIL: could not parse \"%s\"\n", *input);
            goto out;
        }

        u = ndt_from_string(*(input+1), ctx);
        if (u == NULL) {
            ndt_del(t);
            fprintf(stderr, "test_equality: FAIL: could not parse \"%s\"\n", *(input+1));
            goto out;
        }

        if (!ndt_equal(t, t)) {
            ndt_del(t);
            ndt_del(u);
            fprintf(stderr, "test_equality: FAIL: \"%s\" != \"%s\"\n", *input, *input);
            goto out;
        }

        if (ndt_equal(t, u)) {
            ndt_del(t);
            ndt_del(u);
            fprintf(stderr, "test_equality: FAIL: \"%s\" == \"%s\"\n", *input, *(input+1));
            goto out;
        }

        ndt_del(t);
        ndt_del(u);
        test_equality++;
    }
    fprintf(stderr, "test_equality: PASS (%d test cases)\n", test_equality);


    ret = 0;

out:
    ndt_context_del(ctx);
    ndt_finalize();

    return ret;
}
