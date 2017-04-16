#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include "ndtypes.h"



#define ND_FIXED_DATA(ptr) (((ndt_fixed_dim_t *)ptr)->data)
#define ND_VAR_SHAPE(ptr) (((ndt_var_dim_t *)ptr)->shape)
#define ND_VAR_DATA(ptr) (((ndt_var_dim_t *)ptr)->data)

typedef struct {
    ndt_t *type;
    char *ptr;
} nd_array_t;

int nd_init(char *ptr, const ndt_t *t, int64_t nth_shape, ndt_context_t *ctx);
void nd_clear(char *ptr, const ndt_t *t);


/*****************************************************************************/
/*                         Create and delete arrays                          */
/*****************************************************************************/

/* Return a new array structure with type 't'. */
char *
nd_new(const ndt_t *t, ndt_context_t *ctx)
{
    char *ptr;

    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_ValueError, "value has abstract type");
        return NULL;
    }

    ptr = ndt_alloc(1, t->size);
    if (ptr == NULL) {
        return ndt_memory_error(ctx);
    }

    if (nd_init(ptr, t, 0, ctx) < 0) {
        ndt_free(ptr);
        return NULL;
    }

    return ptr;
}

/* Initialize a new array structure with type 't'. */
int
nd_init(char *ptr, const ndt_t *t, int64_t nth_shape, ndt_context_t *ctx)
{
    char *item;
    int64_t i;

    switch (t->tag) {
    case FixedDim:
        if (t->FixedDim.offset != 0 || t->FixedDim.stride != t->FixedDim.itemsize) {
            ndt_err_format(ctx, NDT_ValueError,
                           "cannot initialize from type with offsets or strides");
            return -1;
        }

        ND_FIXED_DATA(ptr) = ndt_calloc(t->FixedDim.shape, t->FixedDim.itemsize);
        if (ND_FIXED_DATA(ptr) == NULL) {
            ndt_err_format(ctx, NDT_MemoryError, "out of memory");
            return -1;
        }

        for (i = 0; i < t->FixedDim.shape; i++) {
            item = ND_FIXED_DATA(ptr) + i * t->FixedDim.itemsize;
            if (nd_init(item, t->FixedDim.type, i, ctx) < 0) {
                nd_clear(ptr, t);
                return 0;
            }
        }

        return 0;

    case VarDim:
        if (t->VarDim.offset != 0 || t->VarDim.stride != t->VarDim.itemsize) {
            ndt_err_format(ctx, NDT_ValueError,
                           "cannot initialize from type with offsets or strides");
            return -1;
        }

        if (t->VarDim.shapes == NULL || nth_shape >= t->VarDim.nshapes) {
             ndt_err_format(ctx, NDT_ValueError,
                            "missing or invalid number of var dim shapes");
             return -1;
        }

        ND_VAR_SHAPE(ptr) = t->VarDim.shapes[nth_shape];
        ND_VAR_DATA(ptr) = ndt_calloc(ND_VAR_SHAPE(ptr), t->VarDim.itemsize);
        if (ND_VAR_DATA(ptr) == NULL) {
            ndt_err_format(ctx, NDT_MemoryError, "out of memory");
            return -1;
        }

        for (i = 0; i < ND_VAR_SHAPE(ptr); i++) {
            item = ND_VAR_DATA(ptr) + i * t->VarDim.itemsize;
            if (nd_init(item, t->VarDim.type, i, ctx) < 0) {
                nd_clear(ptr, t);
                return -1;
            }
       }

       return 0;

   case Tuple:
        for (i = 0; i < t->Tuple.shape; i++) {
            item = ptr + t->Tuple.fields[i].offset;
            if (nd_init(item, t->Tuple.fields[i].type, 0, ctx) < 0) {
                nd_clear(ptr, t);
                return -1;
            }
        }

        return 0;

   case Record:
        for (i = 0; i < t->Record.shape; i++) {
            item = ptr + t->Record.fields[i].offset;
            if (nd_init(item, t->Record.fields[i].type, 0, ctx) < 0) {
                nd_clear(ptr, t);
                return -1;
            }
        }

        return 0;

    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex64: case Complex128:
    case FixedString: case FixedBytes:
    case Char: case String: case Bytes:
        return 0;

    case Ndarray: case Function: case Option: case Nominal:
    case Constr: case Categorical: case Pointer:
        ndt_err_format(ctx, NDT_NotImplementedError,
                       "initialization not implemented for this type");
        return -1;

    case AnyKind: case SymbolicDim: case EllipsisDim: case Typevar:
    case ScalarKind: case SignedKind: case UnsignedKind: case RealKind:
    case ComplexKind: case FixedStringKind: case FixedBytesKind: case Field:
    case Void:
        abort(); /* NOT REACHED (abstract) */
   }

   abort(); /* NOT REACHED */
}

void
nd_del(char *ptr, const ndt_t *t)
{
    nd_clear(ptr, t);
    ndt_free(ptr);
}

void
nd_clear(char *ptr, const ndt_t *t)
{
    char *item;
    int64_t i;

    if (ptr == NULL) {
        return;
    }

    switch (t->tag) {
    case FixedDim:
        for (i = 0; i < t->FixedDim.shape; i++) {
            item = ND_FIXED_DATA(ptr) + i * t->FixedDim.itemsize;
            nd_clear(item, t->FixedDim.type);
        }
        ndt_free(ND_FIXED_DATA(ptr));
        break;
    case VarDim:
        for (i = 0; i < ND_VAR_SHAPE(ptr); i++) {
            item = ND_VAR_DATA(ptr) + i * t->VarDim.itemsize;
            nd_clear(item, t->VarDim.type);
        }
        ndt_free(ND_VAR_DATA(ptr));
        break;
    case Tuple:
        for (i = 0; i < t->Tuple.shape; i++) {
            item = ptr + t->Tuple.fields[i].offset;
            nd_clear(item, t->Tuple.fields[i].type);
        }
        break;
    case Record:
        for (i = 0; i < t->Record.shape; i++) {
            item = ptr + t->Record.fields[i].offset;
            nd_clear(item, t->Record.fields[i].type);
        }
        break;
    default:
        return;
    }
}

/*****************************************************************************/
/*                        Return a typed subarray                            */
/*****************************************************************************/

static nd_array_t
nd_subarray(nd_array_t a, const int64_t *indices, int ndim, ndt_context_t *ctx)
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
        shape = t->FixedDim.shape;
        a.type = t->FixedDim.type;
        a.ptr = ND_FIXED_DATA(ptr) + t->FixedDim.offset + i * t->FixedDim.stride;
        break;
    case VarDim:
        shape = ND_VAR_SHAPE(ptr);
        a.type = t->VarDim.type;
        a.ptr = ND_VAR_DATA(ptr) + t->VarDim.offset + i * t->VarDim.stride;
        break;
    case Tuple:
        shape = t->Tuple.shape;
        a.type = t->Tuple.fields[i].type;
        a.ptr += t->Tuple.fields[i].offset;
        break;
    case Record:
        shape = t->Record.shape;
        a.type = t->Record.fields[i].type;
        a.ptr += t->Record.fields[i].offset;
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

    return nd_subarray(a, indices+1, ndim-1, ctx);
}

int
main(void)
{
    ndt_context_t *ctx;
    nd_array_t a, b;
    int64_t indices[2];
    char *ptr;
    int ret = 1;
    int i;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    a.type = ndt_from_string("2 * var(10,20) * int64", ctx);
    a.ptr = NULL;
    if (a.type == NULL) {
        goto error;
    }

    a.ptr = nd_new(a.type, ctx);
    if (a.ptr == NULL) {
        goto error;
    }

    /*** efficient initialization ***/

    /* b = a[0] */
    indices[0] = 0;
    b = nd_subarray(a, indices, 1, ctx);

    /* init a[0] */
    assert(b.type->tag == VarDim);
    assert(b.type->VarDim.type->tag == Int64);
    for (i = 0; i < 10; i++) {
        ptr = ND_VAR_DATA(b.ptr) + i * sizeof(int64_t);
        *(int64_t *)ptr = i;
    }

    /* b = a[1] */
    indices[0] = 1;
    b = nd_subarray(a, indices, 1, ctx);
    assert(b.type->tag == VarDim);
    assert(b.type->VarDim.type->tag == Int64);

    /* init a[1] */
    for (i = 0; i < 20; i++) {
        ptr = ND_VAR_DATA(b.ptr) + i * sizeof(int64_t);
        *(int64_t *)ptr = i+10;
    }

    /*** print each element individually (for testing, inefficient) ***/
    printf("[");

    indices[0] = 0;
    printf("[");
    for (i = 0; i < 10; i++) {
        indices[1] = i;
        b = nd_subarray(a, indices, 2, ctx);
        assert(b.type->tag == Int64);
        printf("%" PRIi64 "%s", *(int64_t *)b.ptr, i < 9 ? ", " : "],\n");
    }

    indices[0] = 1;
    printf(" [");
    for (i = 0; i < 20; i++) {
        indices[1] = i;
        b = nd_subarray(a, indices, 2, ctx);
        assert(b.type->tag == Int64);
        printf("%" PRIi64 "%s", *(int64_t *)b.ptr, i < 19 ? ", " : "]");
    }

    printf("]\n");

    nd_del(a.ptr, a.type);
    ndt_del(a.type);
    ndt_context_del(ctx);
    ret = 0;

out:
    return ret;

error:
    ndt_err_fprint(stderr, ctx);
    goto out;
}



