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


#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"


/*****************************************************************************/
/*                     String  and formatting utilities                      */
/*****************************************************************************/

char *
ndt_strdup(const char *s, ndt_context_t *ctx)
{
    size_t len = strlen(s);
    char *cp;

    cp = ndt_alloc_size(len+1);
    if (cp == NULL) {
        return ndt_memory_error(ctx);
    }

    memcpy(cp, s, len);
    cp[len] = '\0';
    return cp;
}

char *
ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...)
{
    va_list ap, aq;
    char *s;
    int n;

    va_start(ap, fmt);
    va_copy(aq, ap);

    n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0 || n == INT_MAX) {
        ndt_err_format(ctx, NDT_RuntimeError, "vsprintf failed");
        return NULL;
    }

    s = ndt_alloc_size(n+1);
    if (s == NULL) {
        va_end(aq);
        return ndt_memory_error(ctx);
    }

    n = vsnprintf(s, n+1, fmt, aq);
    va_end(aq);
    if (n < 0) {
        ndt_free(s);
        return NULL;
    }

    return s;
}


/*****************************************************************************/
/*                       Type functions (unstable API)                       */
/*****************************************************************************/

/* Return the next type in a dimension chain.  Undefined for non-dimensions. */
static const ndt_t *
next_dim(const ndt_t *t)
{
    assert(t->ndim > 0);

    switch (t->tag) {
    case FixedDim:
        return t->FixedDim.type;
    case VarDim:
        return t->VarDim.type;
    case VarDimElem:
        return t->VarDimElem.type;
    case SymbolicDim:
        return t->SymbolicDim.type;
    case EllipsisDim:
        return t->EllipsisDim.type;
    default:
        /* NOT REACHED: tags should be exhaustive. */
        ndt_internal_error("invalid value");
    }
}

const ndt_t *
ndt_dtype(const ndt_t *t)
{
    while (t->ndim > 0) {
        t = next_dim(t);
    }

    return t;
}

int
ndt_logical_ndim(const ndt_t *t)
{
    int ndim = t->ndim;

    while (t->tag == VarDim || t->tag == VarDimElem) {
        if (t->tag == VarDimElem) {
            --ndim;
        }
        t = t->VarDim.type;
    }

    return ndim;
}

const ndt_t *
ndt_hidden_dtype(const ndt_t *t)
{
    switch (t->tag) {
    case Ref:
        return ndt_hidden_dtype(t->Ref.type);
    case Constr:
        return ndt_hidden_dtype(t->Constr.type);
    case Nominal:
        return ndt_hidden_dtype(t->Nominal.type);
    default:
        return ndt_dtype(t);
    }
}

int
ndt_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *t)
{
    int n = 0;

    assert(t->ndim <= NDT_MAX_DIM);

    while (t->ndim > 0) {
        dims[n++] = t;
        t = next_dim(t);
    }

    *dtype = t;

    return n;
}

static const ndt_t *
compress(const ndt_t *t)
{
    while (t->tag == VarDimElem) {
        t = t->VarDimElem.type;
    }

    return t;
}

/*
 * Return the next logical type in a dimension chain.  Undefined for
 * non-dimensions.
 */
static const ndt_t *
next_logical_dim(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim:
        return t->FixedDim.type;
    case VarDim:
        return compress(t->VarDim.type);
    case VarDimElem:
        return next_logical_dim(compress(t));
    case SymbolicDim:
        return t->SymbolicDim.type;
    case EllipsisDim:
        return t->EllipsisDim.type;
    default:
        return t;
    }
}

const ndt_t *
ndt_logical_dim_at(const ndt_t *t, int n)
{
    for (int i = 0; i < n; i++) {
        t = next_logical_dim(t);
    }

    return t;
}

const ndt_t *
ndt_copy_contiguous_at(const ndt_t *t, int n, const ndt_t *dtype, ndt_context_t *ctx)
{
    const ndt_t *u;

    if (!ndt_is_ndarray(t)) {
        ndt_err_format(ctx, NDT_NotImplementedError,
            "partial copies are currently restricted to fixed dimensions");
        return NULL;
    }

    if (n < 0 || n > t->ndim) {
        ndt_err_format(ctx, NDT_ValueError, "n out of bounds");
        return NULL;
    }

    u = ndt_logical_dim_at(t, n);

    if (dtype != NULL) {
        return ndt_copy_contiguous_dtype(u, dtype, 0, ctx);
    }
    else {
        return ndt_copy_contiguous(u, 0, ctx);
    }
}

int
ndt_as_ndarray(ndt_ndarray_t *a, const ndt_t *t, ndt_context_t *ctx)
{
    int n;

    assert(t->ndim <= NDT_MAX_DIM);

    if (ndt_is_abstract(t)) {
        ndt_err_format(ctx, NDT_TypeError, "type is not an ndarray");
        return -1;
    }

    if (!ndt_is_ndarray(t)) {
        ndt_err_format(ctx, NDT_TypeError, "type is not an ndarray");
        return -1;
    }

    if (t->ndim == 0) {
        a->ndim = 0;
        a->itemsize = t->datasize;
        return 0;
    }

    a->ndim = t->ndim;
    a->itemsize = t->Concrete.FixedDim.itemsize;

    for (n=0; t->ndim > 0; n++, t=t->FixedDim.type) {
        a->shape[n] = t->FixedDim.shape;
        a->strides[n] = t->Concrete.FixedDim.step * a->itemsize;
        a->steps[n] = t->Concrete.FixedDim.step;
    }

    return n;
}

static const ndt_t *
_ndt_transpose(const ndt_ndarray_t *a, const int p[], const ndt_t *dtype,
               ndt_context_t *ctx)
{
    const ndt_t *t;

    t = dtype;
    ndt_incref(t);

    for (int i = a->ndim-1; i >= 0; i--) {
        const ndt_t *u = ndt_fixed_dim(t, a->shape[p[i]], a->steps[p[i]], ctx);
        ndt_decref(t);
        if (u == NULL) {
            return NULL;
        }
        t = u;
    }

    return t;
}

const ndt_t *
ndt_transpose(const ndt_t *t, const int *p, int ndim, ndt_context_t *ctx)
{
    ndt_ndarray_t a;
    int permute[NDT_MAX_DIM];

    if (ndt_as_ndarray(&a, t, ctx) < 0) {
        return NULL;
    }

    if (p == NULL) {
        if (ndim != 0) {
            ndt_err_format(ctx, NDT_RuntimeError,
                "internal error: ndim != 0 but no permutations given");
            return NULL;
        }

        ndim = a.ndim;
        if (ndim <= 1) {
            ndt_incref(t);
            return t;
        }

        for (int i = 0; i < ndim; i++) {
            permute[i] = ndim-i-1;
        }
        p = permute;
    }
    else {
        if (ndim != a.ndim) {
            ndt_err_format(ctx, NDT_ValueError,
                "number of permutations != ndim");
            return NULL;
        }

        for (int i = 0; i < ndim; i++) {
            permute[i] = 0;
        }

        for (int i = 0; i < ndim; i++) {
            if (p[i] < 0 || p[i] >= ndim) {
                ndt_err_format(ctx, NDT_ValueError,
                    "permutation with value %d out of bounds", p[i]);
                return NULL;
            }

            if (permute[p[i]] != 0) {
                ndt_err_format(ctx, NDT_ValueError,
                    "duplicate permutation index=%d", p[i]);
                return NULL;
            }

            permute[p[i]] = 1;
        }
    }

    return _ndt_transpose(&a, p, ndt_dtype(t), ctx);
}

/* Unoptimized hash function for experimenting. */
ndt_ssize_t
ndt_hash(const ndt_t *t, ndt_context_t *ctx)
{
    unsigned char *s, *cp;
    size_t len;
    ndt_ssize_t x;

    cp = s = (unsigned char *)ndt_as_string(t, ctx);
    if (s == NULL) {
        return -1;
    }

    len = strlen((char *)s);

    x = *cp << 7;
    while (*cp != '\0') {
        x = (1000003 * x) ^ *cp++;
    }
    x ^= len;

    if (x == -1) {
        x = -2;
    }

    ndt_free(s);

    return x;
}


/*****************************************************************************/
/*                           Apply spec (unstable API)                       */
/*****************************************************************************/

const ndt_apply_spec_t ndt_apply_spec_empty = {
  .flags = 0U,
  .outer_dims = 0,
  .nin = 0,
  .nout = 0,
  .nargs = 0,
  .types = {NULL}
};

ndt_apply_spec_t *
ndt_apply_spec_new(ndt_context_t *ctx)
{
    ndt_apply_spec_t *spec;

    spec = ndt_calloc(1, sizeof *spec);
    if (spec == NULL) {
        return ndt_memory_error(ctx);
    }
    spec->flags = 0U;
    spec->outer_dims = 0;
    spec->nin = 0;
    spec->nout = 0;
    spec->nargs = 0;

    return spec;
}

void
ndt_apply_spec_clear(ndt_apply_spec_t *spec)
{
    if (spec == NULL) {
        return;
    }

    for (int i = 0; i < spec->nin+spec->nout; i++) {
        ndt_decref(spec->types[i]);
        spec->types[i] = NULL;
    }

    spec->flags = 0U;
    spec->outer_dims = 0;
    spec->nin = 0;
    spec->nout = 0;
    spec->nargs = 0;
}

void
ndt_apply_spec_del(ndt_apply_spec_t *spec)
{
    if (spec == NULL) {
        return;
    }

    ndt_apply_spec_clear(spec);
    ndt_free(spec);
}

/* This function is used in places where it is _really_ convenient not to
   have to deal with deallocating an error message. */
const char *
ndt_apply_flags_as_string(const ndt_apply_spec_t *spec)
{
    switch (spec->flags) {
    case 0: return "None";
    case NDT_C: return "C";
    case NDT_FORTRAN: return "Fortran";
    case NDT_FORTRAN|NDT_C: return "C|Fortran";
    case NDT_STRIDED: return "Strided";
    case NDT_STRIDED|NDT_C: return "C|Strided";
    case NDT_STRIDED|NDT_FORTRAN: return "Fortran|Strided";
    case NDT_STRIDED|NDT_FORTRAN|NDT_C: return "C|Fortran|Strided";
    case NDT_XND: return "Xnd";
    case NDT_XND|NDT_C: return "C|Xnd";
    case NDT_XND|NDT_FORTRAN: return "Fortran|Xnd";
    case NDT_XND|NDT_FORTRAN|NDT_C: return "C|Fortran|Xnd";
    case NDT_XND|NDT_STRIDED: return "Strided|Xnd";
    case NDT_XND|NDT_STRIDED|NDT_C: return "C|Strided|Xnd";
    case NDT_XND|NDT_STRIDED|NDT_FORTRAN: return "Fortran|Strided|Xnd";
    case NDT_XND|NDT_STRIDED|NDT_FORTRAN|NDT_C: return "C|Fortran|Strided|Xnd";
    case NDT_ELEMWISE_1D: return "Elemwise1D";
    case NDT_ELEMWISE_1D|NDT_C: return "Elemwise1D|C";
    case NDT_ELEMWISE_1D|NDT_FORTRAN: return "Elemwise1D|Fortran";
    case NDT_ELEMWISE_1D|NDT_FORTRAN|NDT_C: return "Elemwise1D|C|Fortran";
    case NDT_ELEMWISE_1D|NDT_STRIDED: return "Elemwise1D|Strided";
    case NDT_ELEMWISE_1D|NDT_STRIDED|NDT_C: return "Elemwise1D|C|Strided";
    case NDT_ELEMWISE_1D|NDT_STRIDED|NDT_FORTRAN: return "Elemwise1D|Fortran|Strided";
    case NDT_ELEMWISE_1D|NDT_STRIDED|NDT_FORTRAN|NDT_C: return "Elemwise1D|C|Fortran|Strided";
    case NDT_ELEMWISE_1D|NDT_XND: return "Elemwise1D|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_C: return "Elemwise1D|C|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_FORTRAN: return "Elemwise1D|Fortran|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_FORTRAN|NDT_C: return "Elemwise1D|C|Fortran|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_STRIDED: return "Elemwise1D|Strided|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_STRIDED|NDT_C: return "Elemwise1D|C|Strided|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_STRIDED|NDT_FORTRAN: return "Elemwise1D|Fortran|Strided|Xnd";
    case NDT_ELEMWISE_1D|NDT_XND|NDT_STRIDED|NDT_FORTRAN|NDT_C: return "Elemwise1D|C|Fortran|Strided|Xnd";
    default: return "error: invalid combination of spec->flags";
    }
}


/*****************************************************************************/
/*                            Metadata (unstable API)                        */
/*****************************************************************************/

ndt_meta_t *
ndt_meta_new(ndt_context_t *ctx)
{
    ndt_meta_t *m;

    m = ndt_calloc(1, sizeof *m);
    if (m == NULL) {
        ndt_memory_error(ctx);
    }

    m->ndims = 0;
    return m;
}

void
ndt_meta_clear(ndt_meta_t *m)
{
    if (m == NULL) {
        return;
    }

    for (int i = 0; i < m->ndims; i++) {
        ndt_decref_offsets(m->offsets[i]);
    }
}

void
ndt_meta_del(ndt_meta_t *m)
{
    ndt_meta_clear(m);
    ndt_free(m);
}


/*****************************************************************************/
/*                     Optimized kernel strategy (unstable API)              */
/*****************************************************************************/

static bool
all_inner_1D_contiguous(const ndt_t *types[], int n)
{
    for (int i = 0; i < n; i++) {
        int ldim = ndt_logical_ndim(types[i]);

        if (ldim == 0) {
            return false;
        }
        if (!ndt_is_c_contiguous(ndt_logical_dim_at(types[i], ldim-1))) {
            return false;
        }
    }

    return true;
}

static bool
all_inner_c_contiguous(const ndt_t *types[], int n, int outer)
{
    for (int i = 0; i < n; i++) {
        if (!ndt_is_c_contiguous(ndt_logical_dim_at(types[i], outer))) {
            return false;
        }
    }

    return true;
}

static bool
all_inner_f_contiguous(const ndt_t *types[], int n, int outer)
{
    for (int i = 0; i < n; i++) {
        if (!ndt_is_f_contiguous(ndt_logical_dim_at(types[i], outer))) {
            return false;
        }
    }

    return true;
}

static bool
all_ndarray(const ndt_t *types[], int n)
{
    for (int i = 0; i < n; i++) {
        if (!ndt_is_ndarray(types[i])) {
            return false;
        }
        if (ndt_subtree_is_optional(types[i])) {
            return false;
        }
    }

    return true;
}

void
ndt_select_kernel_strategy(ndt_apply_spec_t *spec, const ndt_t *sig)
{
    const int outer_dims = spec->outer_dims;
    const int nin = spec->nin;
    const int nout  = spec->nout;
    bool in_inner_c, in_inner_f, out_inner_c;

    assert(sig->tag == Function);
    assert(spec->flags == 0);

    in_inner_c = all_inner_c_contiguous(spec->types, nin, outer_dims);
    in_inner_f = all_inner_f_contiguous(spec->types, nin, outer_dims);
    out_inner_c = all_inner_c_contiguous(spec->types+nin, nout, outer_dims);

    spec->flags = NDT_XND;

    if (sig->Function.elemwise) {
        if (all_inner_1D_contiguous(spec->types, nin) &&
            all_inner_1D_contiguous(spec->types+nin, nout)) {
            spec->flags |= NDT_ELEMWISE_1D;
        }
    }
    if (in_inner_c && out_inner_c) {
        spec->flags |= NDT_C;
    }
    if (in_inner_f && out_inner_c) {
        spec->flags |= NDT_FORTRAN;
    }

    if (all_ndarray(spec->types, nin) && all_ndarray(spec->types+nin, nout)) {
        spec->flags |= NDT_STRIDED;
    }
}
