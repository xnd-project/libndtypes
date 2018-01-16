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
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"
#include "seq.h"
#include "attr.h"


/*****************************************************************************/
/*                    Type attributes used in the parser                     */
/*****************************************************************************/

void
ndt_attr_del(ndt_attr_t *attr)
{
    int64_t i;

    if (attr == NULL) {
        return;
    }

    ndt_free(attr->name);

    switch (attr->tag) {
    case AttrValue:
        ndt_free(attr->AttrValue);
        ndt_free(attr);
        return;
    case AttrList:
        for (i = 0; i < attr->AttrList.len; i++) {
            ndt_free(attr->AttrList.items[i]);
        }
        ndt_free(attr->AttrList.items);
        ndt_free(attr);
        return;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid attribute");
}

void
ndt_attr_array_del(ndt_attr_t *attr, int64_t nattr)
{
    int64_t i, k;

    if (attr == NULL) {
        return;
    }

    for (i = 0; i < nattr; i++) {
        ndt_free(attr[i].name);

        switch (attr[i].tag) {
        case AttrValue:
            ndt_free(attr[i].AttrValue);
            continue;
        case AttrList:
            for (k = 0; k < attr[i].AttrList.len; k++) {
                ndt_free(attr[i].AttrList.items[k]);
            }
            ndt_free(attr[i].AttrList.items);
            continue;
        }

        /* NOT REACHED: tags should be exhaustive. */
        ndt_internal_error("invalid attribute");
    }

    ndt_free(attr);
}

/* Attribute sequences */
NDT_SEQ_NEW(ndt_attr)
NDT_SEQ_DEL(ndt_attr)
NDT_SEQ_GROW(ndt_attr)
NDT_SEQ_APPEND(ndt_attr)
NDT_SEQ_FINALIZE(ndt_attr)


int
ndt_parse_attr(const attr_spec *spec, ndt_context_t *ctx,
               const ndt_attr_seq_t *seq, ...)
{
    va_list ap;
    ndt_attr_t const *v[MAX_ATTR] = {NULL};
    int found;
    int64_t i, k;

    if (seq->len < spec->min || seq->len > spec->max) {
        ndt_err_format(ctx, NDT_InvalidArgumentError,
                       "too %s arguments", seq->len < spec->min ? "few" : "many");
        return -1;
    }

    /* Quadratic time algorithm, we have a small number of args. */
    for (i = 0; i < seq->len; i++) {
        found = 0;
        for (k = 0; k < spec->max; k++) {
            if (strcmp(seq->ptr[i].name, spec->names[k]) == 0) {
                if (v[k]) {
                    ndt_err_format(ctx, NDT_InvalidArgumentError,
                        "duplicate argument: %s", spec->names[k]);
                    return -1;
                }
                found = 1;
                v[k] = &seq->ptr[i];
            }
        }
        if (!found) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                "invalid argument: %s", seq->ptr[i].name);
            return -1;
        }
    }

    for (i = 0; i < spec->min; i++) {
        if (v[i] == NULL) {
            ndt_err_format(ctx, NDT_InvalidArgumentError,
                           "missing required keyword: '%s'", spec->names[i]);
            return -1;
        }
    }

    va_start(ap, seq);
    for (i = 0; i < spec->max; i++) {
        void *ptr = va_arg(ap, void *);
        if (v[i] == NULL) {
            continue;
        }

        switch(spec->tags[i]) {
        case AttrBool:
            *(bool *)ptr = ndt_strtobool(v[i]->AttrValue, ctx);
            goto endloop;
        case AttrChar:
            *(char *)ptr = ndt_strtochar(v[i]->AttrValue, ctx);
            goto endloop;
        case AttrInt8:
            *(int8_t *)ptr = (int8_t)ndt_strtol(v[i]->AttrValue, INT8_MIN, INT8_MAX, ctx);
            goto endloop;
        case AttrInt16:
            *(int16_t *)ptr = (int16_t)ndt_strtol(v[i]->AttrValue, INT16_MIN, INT16_MAX, ctx);
            goto endloop;
        case AttrInt32:
            *(int32_t *)ptr = (int32_t)ndt_strtol(v[i]->AttrValue, INT32_MIN, INT32_MAX, ctx);
            goto endloop;
        case AttrInt64:
            *(int64_t *)ptr = (int64_t)ndt_strtoll(v[i]->AttrValue, INT64_MIN, INT64_MAX, ctx);
            goto endloop;
        case AttrUint8:
            *(uint8_t *)ptr = (uint8_t)ndt_strtoul(v[i]->AttrValue, UINT8_MAX, ctx);
            goto endloop;
        case AttrUint16:
            *(uint16_t *)ptr = (uint16_t)ndt_strtoul(v[i]->AttrValue, UINT16_MAX, ctx);
            goto endloop;
        case AttrUint32:
            *(uint32_t *)ptr = (uint32_t)ndt_strtoul(v[i]->AttrValue, UINT32_MAX, ctx);
            goto endloop;
        case AttrUint64:
            *(uint64_t *)ptr = (uint64_t)ndt_strtoull(v[i]->AttrValue, UINT64_MAX, ctx);
            goto endloop;
        case AttrSize:
            *(size_t *)ptr = (size_t)ndt_strtoull(v[i]->AttrValue, SIZE_MAX, ctx);
            goto endloop;
        case AttrFloat32:
            *(float *)ptr = ndt_strtof(v[i]->AttrValue, ctx);
            goto endloop;
        case AttrFloat64:
            *(double *)ptr = ndt_strtod(v[i]->AttrValue, ctx);
            goto endloop;

        case AttrCharOpt:
            ((char_opt_t *)ptr)->tag = Some;
            ((char_opt_t *)ptr)->Some = ndt_strtochar(v[i]->AttrValue, ctx);
            goto endloop;

        case AttrInt64Opt:
            ((int64_opt_t *)ptr)->tag = Some;
            ((int64_opt_t *)ptr)->Some = (int64_t)ndt_strtoll(v[i]->AttrValue, INT64_MIN, INT64_MAX, ctx);
            goto endloop;

        case AttrUint16Opt:
            ((uint16_opt_t *)ptr)->tag = Some;
            ((uint16_opt_t *)ptr)->Some = (uint16_t)ndt_strtoul(v[i]->AttrValue, UINT16_MAX, ctx);
            goto endloop;

        case AttrString: {
            char *value = ndt_strdup(v[i]->AttrValue, ctx);
            if (value == NULL) {
                return -1;
            }
            *(char **)ptr = value;
            goto endloop;
        }

        case AttrInt32List: {
            int32_t *values = ndt_alloc(v[i]->AttrList.len, sizeof(int32_t));

            if (values == NULL) {
                ndt_err_format(ctx, NDT_MemoryError, "out of memory");
                return -1;
            }

            for (k = 0; k < v[i]->AttrList.len; k++) {
                values[k] = (int32_t)ndt_strtol(v[i]->AttrList.items[k], 0, INT32_MAX, ctx);
                if (ndt_err_occurred(ctx)) {
                    ndt_free(values);
                    return -1;
                }
            }

            *(int32_t **)ptr = values;

            ptr = va_arg(ap, void *);
            *(int64_t *)ptr = v[i]->AttrList.len;
            i++;
            goto endloop;
          }
        }

        /* NOT REACHED: tags should be exhaustive */
        ndt_err_format(ctx, NDT_RuntimeError, "invalid attribute tag", v[i]);

     endloop:
        if (ndt_err_occurred(ctx)) {
            return -1;
        }
    }
    va_end(ap);

    return 0;
}
