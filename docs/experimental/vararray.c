#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ndtypes.h"


/*
 * Small example how to access an element main() has an example for var
 * dimensions.  XXX This is horrible code, just a quick example.
 */

static char *
ndt_get_pointer(char *buf, ndt_t *t, int64_t *indices, int len, ndt_context_t *ctx)
{
    char *ptr = buf;
    int64_t shape;
    int i;

    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_ValueError, "array has abstract type");
        return NULL;
    }

    for (i = 0; i < len; i++) {
        switch (t->tag) {
        case FixedDim:
            if (indices[i] < 0 || indices[i] >= t->FixedDim.shape) {
                goto invalid_index;
            }
            ptr += t->FixedDim.offset + t->FixedDim.stride * indices[i];
            t = t->FixedDim.type;
            break;
        case VarDim:
            shape = ((ndt_var_dim_t *)ptr)->shape;
            ptr = ((ndt_var_dim_t *)ptr)->ptr;
            if (indices[i] < 0 || indices[i] >= shape) {
                goto invalid_index;
            }
            ptr += t->VarDim.offset + t->VarDim.stride * indices[i];
            t = t->VarDim.type;
            break;
        case Tuple:
            if (indices[i] < 0 || indices[i] >= t->Tuple.shape) {
                goto invalid_index;
            }
            ptr += t->Tuple.fields[i].offset;
            t = t->Tuple.fields[i].type;
            break;
        case Record:
            if (indices[i] < 0 || indices[i] >= t->Record.shape) {
                goto invalid_index;
            }
            ptr += t->Record.fields[i].offset;
            t = t->Record.fields[i].type;
            break;
        case Ndarray:
            ndt_err_format(ctx, NDT_NotImplementedError, "ndarray");
            return NULL;
        default:
            ndt_err_format(ctx, NDT_ValueError, "type not indexable");
            return NULL;
        }
    }

    return ptr;

invalid_index:
    ndt_err_format(ctx, NDT_ValueError, "index %d out of range", i);
    return NULL;
}

main(void)
{
    ndt_var_dim_t *a = malloc(2 * sizeof(ndt_var_dim_t));
    char *var_dim0 = malloc(3 * sizeof(int64_t));
    char *var_dim1 = malloc(4 * sizeof(int64_t));
    ndt_context_t *ctx = ndt_context_new();
    ndt_t *t = ndt_from_string("2 * var * int64", ctx);
    char *ptr;
    int i, k;
 
    for (i = 0; i < 3; i++) {
        ((int64_t *)var_dim0)[i] = i;
    }

    for (i = 0; i < 4; i++) {
        ((int64_t *)var_dim1)[i] = i+3;
    }

    a[0].shape = 3;
    a[0].ptr = var_dim0;

    a[1].shape = 4;
    a[1].ptr = var_dim1;

    for (k = 0; k < 3; k++) {
        int64_t indices[2] = {0, k};
        ptr = ndt_get_pointer(a, t, indices, 2, ctx);
        if (ptr == NULL) {
            ndt_err_fprint(stdout, ctx);
            return 1;
        }
        fprintf(stderr, "%" PRIi64 "\n", *(int64_t *)ptr);
    }  

    for (k = 0; k < 4; k++) {
        int64_t indices[2] = {1, k};
        ptr = ndt_get_pointer(a, t, indices, 2, ctx);
        if (ptr == NULL) {
            ndt_err_fprint(stdout, ctx);
            return 1;
        }
        fprintf(stderr, "%" PRIi64 "\n", *(int64_t *)ptr);
    }  

    return 0;
}



