#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ndtypes.h"


/*
 * Small (quick and dirty) example how to access an element.
 */

typedef struct {
    ndt_t *type;
    char *ptr;
} nd_array_t;

static nd_array_t
nd_get_pointer(nd_array_t a, const int64_t *indices, int ndim, ndt_context_t *ctx)
{
    ndt_t *t = a.type;
    char *ptr = a.ptr;
    int64_t shape;
    int i;

    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_ValueError, "array has abstract type");
        a.ptr = NULL;
        return a;
    }

    if (ndim == 0) {
        return a;
    }

    i = indices[0];

    switch (t->tag) {
    case FixedDim:
        a.ptr = ((ndt_fixed_dim_t *)ptr)->ptr;
        a.ptr += t->FixedDim.offset + i * t->FixedDim.stride;
        a.type = t->FixedDim.type;
        shape = t->FixedDim.shape;
        break;
    case VarDim:
        a.ptr = ((ndt_var_dim_t *)ptr)->ptr;
        a.ptr += t->VarDim.offset + i * t->VarDim.stride;
        a.type = t->VarDim.type;
        shape = ((ndt_var_dim_t *)ptr)->shape;
        break;
    case Tuple:
        a.type = t->Tuple.fields[i].type;
        a.ptr += t->Tuple.fields[i].offset;
        shape = t->Tuple.shape;
        break;
    case Record:
        a.type = t->Record.fields[i].type;
        a.ptr += t->Record.fields[i].offset;
        shape = t->Record.shape;
        break;
    case Ndarray:
        ndt_err_format(ctx, NDT_NotImplementedError, "ndarray");
        a.ptr = NULL;
        return a;
    default:
        ndt_err_format(ctx, NDT_ValueError, "type not indexable");
        a.ptr = NULL;
        return a;
    }

    if (i < 0 || i > shape) {
        ndt_err_format(ctx, NDT_ValueError, "index out of bounds");
        a.ptr = NULL;
        return a;
    }

    return nd_get_pointer(a, indices+1, ndim-1, ctx);
}


char *
ndt_memory_error(ndt_context_t *ctx)
{
    ndt_err_format(ctx, NDT_MemoryError, "out of memory");
    return NULL;
}

/*
 * Allocate the memory (tree structure) required for a type.
 *
 * XXX shapes[] is a hack to explore the concept of specifying shapes
 * for 'var' dimensions in the type.  It must contain all shapes
 * (including fixed dimensions) as a flat array in depth first (!) order.
 */
static char *
nd_array_new(ndt_t *t, int64_t shapes[], int len, ndt_context_t *ctx)
{
    int64_t shape;
    size_t itemsize;
    int i;

    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_ValueError, "array has abstract type");
        return NULL;
    }

    switch (t->tag) {
    case FixedDim: {
        ndt_fixed_dim_t *f;

        if (len <= 0 || shapes[0] != t->FixedDim.shape) {
            ndt_err_format(ctx, NDT_ValueError, "incorrect or missing fixed dim");
            return NULL;
        }

        shape = t->FixedDim.shape;
        itemsize = t->FixedDim.itemsize;

        f = ndt_alloc(1, sizeof *f);
        if (f == NULL) {
            return ndt_memory_error(ctx);
        }

        f->ptr = ndt_alloc(shape, itemsize);
        if (f->ptr == NULL) {
            ndt_free(f);
            return ndt_memory_error(ctx);
        }

        if (t->ndim > 1) {
            for (i = 0; i < t->FixedDim.shape; i++) {
                shapes++; len--;
                char *elem = nd_array_new(t->FixedDim.type, shapes, len, ctx);
                if (elem == NULL) {
                    return NULL;
                }

                memcpy(f->ptr + i * itemsize, elem, itemsize);
                ndt_free(elem);
            }
        }

        return (char *)f;
    }

    case VarDim: {
        ndt_var_dim_t *v;

        if (len <= 0) {
            ndt_err_format(ctx, NDT_ValueError, "missing var dim");
            return NULL;
        }

        itemsize = t->VarDim.itemsize;

        v = ndt_alloc(1, sizeof *v);
        if (v == NULL) {
            return ndt_memory_error(ctx);
        }

        v->shape = shape = shapes[0];
        v->ptr = ndt_alloc(shape, itemsize);
        if (v->ptr == NULL) {
            return ndt_memory_error(ctx);
        }

        if (t->ndim > 1) {
            for (i = 0; i < shape; i++) {
                shapes++; len--;
                char *elem = nd_array_new(t->VarDim.type, shapes, len, ctx);
                if (elem == NULL) return NULL;

                memcpy(v->ptr + i * itemsize, elem, itemsize);
                ndt_free(elem);
            }
        }

        return (char *)v;
    }

    default:
        ndt_err_format(ctx, NDT_NotImplementedError, "not implemented");
        return NULL;
    }
}

int
main(void)
{
    ndt_context_t *ctx;
    nd_array_t a, b;
    int64_t shapes[3] = {2, 2, 3};
    int64_t indices[2];
    int j;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    a.type = ndt_from_string("2 * var * int64", ctx);
    if (a.type == NULL) {
        ndt_err_fprint(stderr, ctx);
        return 1;
    }

    a.ptr = nd_array_new(a.type, shapes, 3, ctx);
    if (a.ptr == NULL) {
        ndt_err_fprint(stderr, ctx);
        return 1;
    }

    indices[0] = 0;
    for (j = 0; j < 2; j++) {
        indices[1] = j;
        b = nd_get_pointer(a, indices, 2, ctx);
        if (b.ptr == NULL) {
            ndt_err_fprint(stderr, ctx);
            return 1;
        }
        if (b.type->tag != Int64) {
            fprintf(stderr, "unexpected data type\n");
            return 1;
        }
        *(int64_t *)b.ptr = j;
    }

    indices[0] = 1;
    for (j = 0; j < 3; j++) {
        indices[1] = j;
        b = nd_get_pointer(a, indices, 2, ctx);
        if (b.ptr == NULL) {
            ndt_err_fprint(stderr, ctx);
            return 1;
        }
        if (b.type->tag != Int64) {
            fprintf(stderr, "unexpected data type\n");
            return 1;
        }
        *(int64_t *)b.ptr = j+2;
    }

    return 0;
}



