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


#include <stdarg.h>
#include "ndtypes.h"


/******************************************************************************/
/*                                  Context                                   */
/******************************************************************************/

ndt_context_t *
ndt_context_new(void)
{
    ndt_context_t *ctx;

    ctx = ndt_alloc_size(sizeof *ctx);
    if (ctx == NULL) {
        return NULL;
    }

    ctx->flags = NDT_Dynamic;
    ctx->err = NDT_Success;
    ctx->msg = ConstMsg;
    ctx->ConstMsg = "Success";

    return ctx;
}

void
ndt_context_del(ndt_context_t *ctx)
{
    if (ctx) {
        if (ctx->msg == DynamicMsg) {
            ndt_free(ctx->DynamicMsg);
        }
        if (ctx->flags & NDT_Dynamic) {
            ndt_free(ctx);
        }
    }
}


/******************************************************************************/
/*                              Error handling                                */
/******************************************************************************/

/* Set an error with a message. */
void
ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...)
{
    va_list ap, aq;
    char *s;
    int n;

    ndt_err_clear(ctx);
    ctx->err = err;

    va_start(ap, fmt);
    va_copy(aq, ap);

    n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0 || n == INT_MAX) {
        ctx->msg = ConstMsg;
        ctx->ConstMsg = \
           "internal error during the handling of the original error";
        return;
    }

    s = ndt_alloc(1, n+1);
    if (s == NULL) {
        va_end(aq);
        ctx->err = NDT_MemoryError;
        ctx->msg = ConstMsg;
        ctx->ConstMsg = "out of memory";
        return;
    }

    n = vsnprintf(s, n+1, fmt, aq);
    va_end(aq);
    if (n < 0) {
        ndt_free(s);
        ctx->msg = ConstMsg;
        ctx->ConstMsg = \
            "internal error during the handling of the original error";
        return;
    }

    ctx->msg = DynamicMsg;
    ctx->DynamicMsg = s;
}

/* Append 'msg' to an existing error message. If no error is set, do nothing. */
void
ndt_err_append(ndt_context_t *ctx, const char *msg)
{
    NDT_STATIC_CONTEXT(localctx);
    enum ndt_error err = ctx->err;
    char *s;

    if (!ndt_err_occurred(ctx)) {
        return;
    }

    s = ndt_strdup(ndt_context_msg(ctx), &localctx);
    if (s == NULL) {
        ndt_context_del(&localctx);
        return;
    }

    ndt_err_clear(ctx);
    ndt_err_format(ctx, err, "%s: \"%s\"", s, msg);
    ndt_free(s);
}

/* Check if an error has occurred. */
int
ndt_err_occurred(const ndt_context_t *ctx)
{
    return ctx->err != NDT_Success;
}

/* Clear an error. */
void
ndt_err_clear(ndt_context_t *ctx)
{
    ctx->err = NDT_Success;
    if (ctx->msg == DynamicMsg) {
        ndt_free(ctx->DynamicMsg);
        ctx->msg = ConstMsg;
        ctx->ConstMsg = "Success";
    }
}

/* Set a malloc error. */
void *
ndt_memory_error(ndt_context_t *ctx)
{
    ndt_err_format(ctx, NDT_MemoryError, "out of memory");
    return NULL;
}


/******************************************************************************/
/*                               Display errors                               */
/******************************************************************************/

const char *
ndt_err_as_string(enum ndt_error err)
{
    switch (err) {
    case NDT_Success:
        return "Success";
    case NDT_MemoryError:
        return "MemoryError";
    case NDT_ValueError:
        return "ValueError";
    case NDT_TypeError:
        return "TypeError";
    case NDT_InvalidArgumentError:
        return "InvalidArgumentError";
    case NDT_NotImplementedError:
        return "NotImplementedError";
    case NDT_IndexError:
        return "IndexError";
    case NDT_RuntimeError:
        return "RuntimeError";
    case NDT_LexError:
        return "LexError";
    case NDT_ParseError:
        return "ParseError";
    case NDT_OSError:
        return "OSError";
    }

    /* NOT REACHED: tags should be exhaustive. */
    return "UnknownError";
}

/* Get the current error message. */
const char *
ndt_context_msg(ndt_context_t *ctx)
{
    if (ctx->msg == ConstMsg) {
        return ctx->ConstMsg;
    }
    return ctx->DynamicMsg;
}

/* Print an error.  Mostly useful for debugging. */
void
ndt_err_fprint(FILE *fp, ndt_context_t *ctx)
{
    const char *err = ndt_err_as_string(ctx->err);

    fprintf(fp, "%s: %s\n", err, ndt_context_msg(ctx));
}
