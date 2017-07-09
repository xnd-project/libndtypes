#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include "ndtypes.h"


/******************************************************************************/
/*                          Pointer-based array layout                        */
/******************************************************************************/


#define ND_FIXED_DATA(ptr) (((ndt_fixed_dim_t *)ptr)->data)
#define ND_VAR_SHAPE(ptr) (((ndt_var_dim_t *)ptr)->shape)
#define ND_VAR_DATA(ptr) (((ndt_var_dim_t *)ptr)->data)
#define ND_POINTER_DATA(ptr) (((ndt_pointer_t *)ptr)->data)


typedef struct {
    ndt_t *type;
    char *ptr;
} nd_array_t;

int nd_init(char *ptr, const ndt_t *t, int64_t nth_shape, ndt_context_t *ctx);
void nd_clear(char *ptr, const ndt_t *t);
static int ndarray_init(char *ptr, int64_t *shape, int64_t *strides, int ndim, ndt_t *dtype, ndt_context_t *ctx);
static void ndarray_clear(char *ptr, int64_t *shape, int64_t *strides, int ndim, ndt_t *dtype);
static char *ndarray_get_item(char *ptr, ndt_t *t, const int64_t *indices, ndt_context_t *ctx);



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
        if (!ndt_is_contiguous(t)) {
            ndt_err_format(ctx, NDT_ValueError,
                           "cannot initialize from non-contiguous type");
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
                return -1;
            }
        }

        return 0;

    case VarDim:
        if (!ndt_is_contiguous(t)) {
            ndt_err_format(ctx, NDT_ValueError,
                           "cannot initialize from non-contiguous type");
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

    case Ndarray:
        if (!ndt_is_contiguous(t)) {
            ndt_err_format(ctx, NDT_ValueError,
                           "cannot initialize from non-contiguous type");
            return -1;
        }

        return ndarray_init(ptr, t->Ndarray.shape, t->Ndarray.strides, t->ndim,
                            t->Ndarray.dtype, ctx);

    /* Pointer represents a pointer to an explicit type. */
    case Pointer:
        ND_POINTER_DATA(ptr) = ndt_calloc(1, t->Pointer.type->size);
        if (ND_POINTER_DATA(ptr) == NULL) {
            ndt_err_format(ctx, NDT_MemoryError, "out of memory");
            return -1;
        }

        if (nd_init(ND_POINTER_DATA(ptr), t->Pointer.type, 0, ctx) < 0) {
            nd_clear(ptr, t);
            return -1;
        }
        return 0;

    /* Constr is a named explicit type. */
    case Constr:
        if (nd_init(ptr, t->Constr.type, 0, ctx) < 0) {
            nd_clear(ptr, t);
            return -1;
        }

    /* Option is represented as a tuple: (type, byte).  'byte' is the flag
     * that indicates whether an element is present and is initialized to
     * 0 (NA). XXX
     */
    case Option:
        ndt_err_format(ctx, NDT_NotImplementedError, "option semantics");
        return -1;

    /* Function represents a typed function pointer, initialized to NULL. */
    case Function:
        return 0;

    /* Nominal represents a pointer to an opaque type, initialized to NULL. */
    case Nominal:
        return 0;

    /* Categorical is already initialized by calloc(). */
    case Categorical:
        return 0;

    /* Primitive types are already initialized by calloc(). */
    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex64: case Complex128:
    case FixedString: case FixedBytes:
    case Char: case String: case Bytes:
        return 0;

    case AnyKind: case SymbolicDim: case EllipsisDim: case Typevar:
    case ScalarKind: case SignedKind: case UnsignedKind: case FloatKind:
    case ComplexKind: case FixedStringKind: case FixedBytesKind: case Field:
    case Void:
        ndt_err_format(ctx, NDT_RuntimeError,
                       "nd_init() called on abstract type");
        return -1;
   }

   ndt_err_format(ctx, NDT_RuntimeError, "invalid type tag in nd_init()");
   return -1;
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

static int
ndarray_init_rec(char *ptr, int64_t *shape, int64_t *strides, int ndim,
                 ndt_t *dtype, ndt_context_t *ctx)
{
    char *item;
    int i;

    if (ndim == 1) {
        for (i = 0; i < shape[0]; i++) {
            item = ptr + i * strides[i];
            if (nd_init(item, dtype, 0, ctx) < 0) {
                return -1;
            }
        }
    }

    for (i = 0; i < shape[i]; i++) {
        if (ndarray_init_rec(ptr, shape+1, strides+1, ndim-1, dtype, ctx) < 0) {
            return -1;
        }
    }

    return 0;
}

static void
ndarray_clear(char *ptr, int64_t *shape, int64_t *strides, int ndim, ndt_t *dtype)
{
    char *item;
    int i;

    if (ptr == NULL) {
        return;
    }

    if (ndim == 1) {
        for (i = 0; i < shape[0]; i++) {
            item = ptr + i * strides[i];
            nd_clear(item, dtype);
        }
    }

    for (i = 0; i < shape[i]; i++) {
        ndarray_clear(ptr, shape+1, strides+1, ndim-1, dtype);
    }
}

/* Assumption: type is contiguous. */
static int
ndarray_init(char *ptr, int64_t *shape, int64_t *strides, int ndim, ndt_t *dtype,
             ndt_context_t *ctx)
{
    if (ndim < 1) {
        ndt_err_format(ctx, NDT_RuntimeError, "ndim < 1 in init_ndarray()");
        return -1;
    }

    if (ndarray_init_rec(ptr, shape, strides, ndim, dtype, ctx) < 0) {
        ndarray_clear(ptr, shape, strides, ndim, dtype);
        return -1;
    }

    return 0;
}


/*****************************************************************************/
/*                        Return a typed subarray                            */
/*****************************************************************************/

static nd_array_t
nd_subarray(nd_array_t a, const int64_t *indices, int len, ndt_context_t *ctx)
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

    if (len == 0) {
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
        if (len < t->ndim) {
            ndt_err_format(ctx, NDT_ValueError,
                "subarrays not supported for ndarray");
            a.ptr = NULL;
            return a;
        }
        a.type = t->Ndarray.dtype;
        a.ptr = ndarray_get_item(a.ptr, t, indices, ctx);
        if (a.ptr == NULL) {
            return a;
        }
        return nd_subarray(a, indices+t->ndim, len-t->ndim, ctx);
    default:
        ndt_err_format(ctx, NDT_ValueError, "type not indexable");
        a.ptr = NULL;
        return a;
    }

    if (i < 0 || i >= shape) {
        ndt_err_format(ctx, NDT_ValueError, "index out of bounds");
        a.ptr = NULL;
        return a;
    }

    return nd_subarray(a, indices+1, len-1, ctx);
}

static char *
ndarray_get_item(char *ptr, ndt_t *t, const int64_t *indices, ndt_context_t *ctx)
{
    int i, k;

    if (t->tag != Ndarray) {
        ndt_err_format(ctx, NDT_RuntimeError, "expect ndarray");
        return NULL;
    }

    for (i = 0; i < t->ndim; i++) {
        k = indices[i];
        if (k < 0 || k >= t->Ndarray.shape[i]) {
            ndt_err_format(ctx, NDT_ValueError, "index out of bounds");
            return NULL;
        }
        ptr += k * t->Ndarray.strides[i];
    }

    return ptr;
}

/*
 * Variable array example for:
 *   [[0,1,2,3,4,5,6,7,8,9],
 *    [10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29]]
 */
int
var_example(void)
{
    ndt_context_t *ctx;
    nd_array_t a, b;
    int64_t indices[2];
    char *ptr;
    int ret = 0;
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


out:
    nd_del(a.ptr, a.type);
    ndt_del(a.type);
    ndt_context_del(ctx);
    return ret;

error:
    ret = -1;
    ndt_err_fprint(stderr, ctx);
    goto out;
}

/*
 * Ndarray example for:
 *   [[{0, 1.0}, {2, 3.0}, {4, 5.0}],
 *    [{6, 7.0}, {8, 9.0}, {10, 11.0}]]
 */
int
ndarray_example(void)
{
    ndt_context_t *ctx;
    nd_array_t a, b;
    const ndt_t *t, *dtype;
    int64_t indices[3];
    char *ptr, *item;
    int ret = 0, count = 0;
    int i, k;

    ctx = ndt_context_new();
    if (ctx == NULL) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    a.type = ndt_from_string("[2 * 3 * {a: int32, b: float64}, style='ndarray']", ctx);
    a.ptr = NULL;
    if (a.type == NULL) {
        goto error;
    }

    a.ptr = nd_new(a.type, ctx);
    if (a.ptr == NULL) {
        goto error;
    }

    /*** efficient initialization ***/
    t =  a.type;
    dtype = t->Ndarray.dtype;
    for (i = 0; i < t->Ndarray.shape[0]; i++) {
        for (k = 0; k < t->Ndarray.shape[1]; k++) {
            ptr = a.ptr + i * t->Ndarray.strides[0] + k * t->Ndarray.strides[1];

            item = ptr + dtype->Record.fields[0].offset;
            *(int64_t *)item = count++;

            item = ptr + dtype->Record.fields[1].offset;
            *(double *)item = count++;
        }
    }

    /*** print each element individually (for testing, inefficient) ***/
    printf("[");
    for (i = 0; i < t->Ndarray.shape[0]; i++) {
        indices[0] = i;
        printf("%s[", i > 0 ? " " : "");
        for (k = 0; k < t->Ndarray.shape[1]; k++) {
            indices[1] = k;
            printf("{");

            indices[2] = 0;
            b = nd_subarray(a, indices, 3, ctx);
            printf("%" PRIi64 "%s", *(int64_t *)b.ptr, ", ");

            indices[2] = 1;
            b = nd_subarray(a, indices, 3, ctx);
            printf("%2.1f%s", *(double *)b.ptr, k < t->Ndarray.shape[1]-1 ? "}, " : "}");
        }
        printf("]%s", i < t->Ndarray.shape[0]-1 ? ",\n" : "");
    }
    printf("]\n");

    nd_del(a.ptr, a.type);
    ndt_del(a.type);
    ndt_context_del(ctx);

out:
    return ret;

error:
    ndt_err_fprint(stderr, ctx);
    goto out;
}


int
main(void)
{
    if (var_example() < 0) {
        return 1;
    }
    if (ndarray_example() < 0) {
        return 1;
    }

    return 0;
}
