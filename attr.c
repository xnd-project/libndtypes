#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ndtypes.h"
#include "seq.h"


#define MAX_ATTR 8

typedef struct {
   const size_t min;
   const size_t max;
   const enum ndt tags[MAX_ATTR];
   const char *names[MAX_ATTR];
} attr_spec;

/* Container attributes */
static const attr_spec array_attr = {0, 1, {Char}, {"order"}};
static const attr_spec tuple_record_attr = {0, 2, {Uint8, Uint8}, {"align", "pack"}};
static const attr_spec field_attr = {0, 2, {Uint8, Uint8}, {"align", "pack"}};

/* Type constructor attributes */
static const attr_spec prim_attr = {0, 1, {Char}, {"endian"}};
static const attr_spec char_attr = {0, 1, {String}, {"encoding"}};
static const attr_spec bytes_attr = {0, 1, {Uint8}, {"align"}};
static const attr_spec fixed_bytes_attr = {1, 2, {Size, Uint8}, {"size", "align"}};


const attr_spec *
ndt_get_attr_spec(enum ndt tag, ndt_context_t *ctx)
{
    switch(tag) {
    case Array:
        return &array_attr;
    case Tuple: case Record:
        return &tuple_record_attr;
    case Field:
        return &field_attr;
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float32: case Float64: case Complex64: case Complex128:
        return &prim_attr;
    case Char:
        return &char_attr;
    case Bytes:
        return &bytes_attr;
    case FixedBytes:
        return &fixed_bytes_attr;
    default:
        ndt_err_format(ctx, NDT_RuntimeError, "'%s' does not take attributes",
                       ndt_tag_as_string(tag));
        return NULL;
    }
}

int
ndt_parse_attr(enum ndt tag, ndt_context_t *ctx, ndt_attr_seq_t *seq, ...)
{
    va_list ap;
    const attr_spec *spec;
    char *v[MAX_ATTR] = {NULL};
    int found;
    size_t i, k;

    spec = ndt_get_attr_spec(tag, ctx);
    if (spec == NULL) {
        return -1;
    }

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
                v[k] = seq->ptr[i].value;
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
        case Bool: *(bool *)ptr = ndt_strtobool(v[i], ctx); break;
        case Char: *(char *)ptr = ndt_strtochar(v[i], ctx); break; /* Char here means C-char */
        case Int8: *(int8_t *)ptr = (int8_t)ndt_strtol(v[i], INT8_MIN, INT8_MAX, ctx); break;
        case Int16: *(int16_t *)ptr = (int16_t)ndt_strtol(v[i], INT16_MIN, INT16_MAX, ctx); break;
        case Int32: *(int32_t *)ptr = (int32_t)ndt_strtol(v[i], INT32_MIN, INT32_MAX, ctx); break;
        case Int64: *(int64_t *)ptr = (int64_t)ndt_strtoll(v[i], INT64_MIN, INT64_MAX, ctx); break;
        case Uint8: *(uint8_t *)ptr = (uint8_t)ndt_strtoul(v[i], UINT8_MAX, ctx); break;
        case Uint16: *(uint16_t *)ptr = (uint16_t)ndt_strtoul(v[i], UINT16_MAX, ctx); break;
        case Uint32: *(uint32_t *)ptr = (uint32_t)ndt_strtoul(v[i], UINT32_MAX, ctx); break;
        case Uint64: *(uint64_t *)ptr = (uint64_t)ndt_strtoull(v[i], UINT64_MAX, ctx); break;
        case Size: *(size_t *)ptr = (size_t)ndt_strtoull(v[i], SIZE_MAX, ctx); break;
        case Float32: *(float *)ptr = ndt_strtof(v[i], ctx); break;
        case Float64: *(double *)ptr = ndt_strtod(v[i], ctx); break;
        case String: *(char **)ptr = v[i]; break;
        default:
            ndt_err_format(ctx, NDT_RuntimeError,
                           "invalid attribute type", v[i]);
            break;
        }
    }
    va_end(ap);

    return ctx->err == NDT_Success ? 0 : -1;
}
