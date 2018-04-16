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
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"
#include "symtable.h"
#include "substitute.h"


static int match_datashape(const ndt_t *, const ndt_t *, symtable_t *, ndt_context_t *);
static int match_dimensions(const ndt_t *p[], int pshape, const ndt_t *c[], int cshape, symtable_t *tbl, ndt_context_t *ctx);
static int match_dimensions_rev(const ndt_t *p[], int pshape, const ndt_t *c[], int cshape, symtable_t *tbl, ndt_context_t *ctx);
static int match_single(const ndt_t *p, const ndt_t *c, bool ellipsis, symtable_t *tbl, ndt_context_t *ctx);


static int
is_array(const ndt_t *t)
{
    switch (t->tag) {
    case FixedDim: case SymbolicDim: case VarDim: case EllipsisDim:
        return 1;
    default:
        return 0;
    }
}

static int
match_concrete_var_dim(const ndt_t *t, const ndt_t *u, ndt_context_t *ctx)
{
    const int32_t noffsets = t->Concrete.VarDim.noffsets;
    const int64_t itemsize = t->Concrete.VarDim.itemsize;
    int64_t tshape, ushape;
    int64_t start, step;

    if (itemsize != u->Concrete.VarDim.itemsize ||
        noffsets != u->Concrete.VarDim.noffsets) {
        return 0;
    }

    for (int64_t i = 0; i < noffsets-1; i++) {
        tshape = ndt_var_indices(&start, &step, t, i, ctx);
        if (tshape < 0) {
            return -1;
        }

        ushape = ndt_var_indices(&start, &step, u, i, ctx);
        if (ushape < 0) {
            return -1;
        }

        if (tshape != ushape) {
            return 0;
        }
    }

    return 1;
}

static int
symtable_entry_equal(symtable_entry_t *v, symtable_entry_t *w, symtable_t *tbl,
                     ndt_context_t *ctx)
{
    switch (v->tag) {
    case ShapeEntry:
        return w->tag == ShapeEntry && v->ShapeEntry == w->ShapeEntry;
    case SymbolEntry:
        return w->tag == SymbolEntry && strcmp(v->SymbolEntry, w->SymbolEntry) == 0;
    case VarDimEntry:
        if (w->tag != VarDimEntry)
            return 0;

        return match_concrete_var_dim(v->VarDimEntry.type,
                                      w->VarDimEntry.type,
                                      ctx);
    case TypeEntry:
        return w->tag == TypeEntry && ndt_equal(v->TypeEntry, w->TypeEntry);
    case DimListEntry:
        if (w->tag != DimListEntry)
            return 0;

        return match_dimensions(v->DimListEntry.dims, v->DimListEntry.size,
                                w->DimListEntry.dims, w->DimListEntry.size,
                                tbl, ctx);
    case BroadcastEntry:
        /* NOT REACHED: function is not used for broadcasting. */
        return 0;
    case Unbound:
        /* NOT REACHED: 'v' is always bound. */
        return 0;
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid symtable entry");
}

static int
resolve_sym(const char *key, symtable_entry_t w,
            symtable_t *tbl,
            ndt_context_t *ctx)
{
    symtable_entry_t v;
    int n;

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            symtable_free_entry(w);
            return -1;
        }
        return 1;
    }

    n = symtable_entry_equal(&v, &w, tbl, ctx);
    symtable_free_entry(w);
    return n;
}

static int
resolve_var_dim(int ndim, symtable_entry_t w, symtable_t *tbl,
                ndt_context_t *ctx)
{
    char key[10];
    int n;

    n = snprintf(key, 10, "00%dVAR", ndim);
    if (n < 0 || n >= 10) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "write failed or buffer too small %s", key);
        return -1;
    }

    return resolve_sym(key, w, tbl, ctx);
}

static int
resolve_sym_broadcast(symtable_entry_t w, symtable_t *tbl, ndt_context_t *ctx)
{
    const char *key = "00_ELLIPSIS";
    symtable_entry_t *v;
    int64_t n, m;
    int vsize, wsize;
    int i, k;

    v = symtable_find_ptr(tbl, key);
    if (v == NULL) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            symtable_free_entry(w);
            return -1;
        }
        return 1;
    }

    vsize = v->BroadcastEntry.size;
    wsize = w.BroadcastEntry.size;

    for (i=vsize-1, k=wsize-1; i>=0 && k>=0; i--, k--) {
        n = v->BroadcastEntry.dims[i];
        m = w.BroadcastEntry.dims[k];
        if (m != n) {
            if (n == 1) {
                n = m;
            }
            else if (m == 0) {
                n = 0;
            }
            else if (m != 1) {
                return 0;
            }
        }
        v->BroadcastEntry.dims[i<k ? k : i] = n;
    }
    for (; k >= 0; k--) {
        v->BroadcastEntry.dims[k] = w.BroadcastEntry.dims[k];
    }

    v->BroadcastEntry.size = vsize >= wsize ? vsize : wsize;

    return 1;
}

static int
resolve_dimension_list(const char *name, const ndt_t *c[], int size,
                       symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    int i;

    v.tag = DimListEntry;
    v.DimListEntry.size = size;
    v.DimListEntry.dims = ndt_alloc(size, sizeof *v.DimListEntry.dims);

    if (v.DimListEntry.dims == NULL) {
        (void)ndt_memory_error(ctx);
        return -1;
    }

    for (i = 0; i < size; i++) {
        switch(c[i]->tag) {
        case FixedDim: case VarDim:
            v.DimListEntry.dims[i] = c[i];
            break;
        default:
            ndt_free((void *)v.DimListEntry.dims);
            return 0;
        }
    }

    return resolve_sym(name, v, tbl, ctx);
}

static int
resolve_broadcast(const ndt_t *c[], int size, symtable_t *tbl,
                  ndt_context_t *ctx)
{
    symtable_entry_t v;
    int i;

    v.tag = BroadcastEntry;
    v.BroadcastEntry.size = size;

    for (i = 0; i < size; i++) {
        switch(c[i]->tag) {
        case FixedDim:
            v.BroadcastEntry.dims[i] = c[i]->FixedDim.shape;
            break;
        default:
            return 0;
        }
    }

    return resolve_sym_broadcast(v, tbl, ctx);
}

static int
match_single(const ndt_t *p, const ndt_t *c, bool ellipsis, symtable_t *tbl,
             ndt_context_t *ctx)
{
    switch (p->tag) {
    case FixedDim: {
        return c->tag == FixedDim && p->FixedDim.shape == c->FixedDim.shape;
    }

    case VarDim: {
        if (ndt_is_abstract(p)) {
            symtable_entry_t v;

            if (c->tag != VarDim || c->ndim != p->ndim) {
                return 0;
            }

            if (!ellipsis) {
                return 1;
            }

            v.tag = VarDimEntry;
            v.VarDimEntry.type = c;

            return resolve_var_dim(c->ndim, v, tbl, ctx);
        }
        else {
            return match_concrete_var_dim(p, c, ctx);
        }
    }

    case SymbolicDim: {
        symtable_entry_t v;

        switch (c->tag) {
        case FixedDim:
            v.tag = ShapeEntry;
            v.ShapeEntry = c->FixedDim.shape;
            break;

        case SymbolicDim:
            v.tag = SymbolEntry;
            v.SymbolEntry = c->SymbolicDim.name; /* borrowed */
            break;

        default:
            return 0;
        }

        return resolve_sym(p->SymbolicDim.name, v, tbl, ctx);
    }

    case EllipsisDim: {
        ndt_err_format(ctx, NDT_RuntimeError, "unexpected ellipsis");
        return -1;
    }

    default: /* NOT REACHED */
        ndt_internal_error("not a dimension");
    }
}

static int
resolve_ellipsis(const char *name, const ndt_t *c[], int cshape,
                 symtable_t *tbl, ndt_context_t *ctx)
{
    if (name == NULL) {
        return resolve_broadcast(c, cshape, tbl, ctx);
    }
    else {
        return resolve_dimension_list(name, c, cshape, tbl, ctx);
    }
}

static int
match_dimensions_rev(const ndt_t *p[], int pshape,
                     const ndt_t *c[], int cshape,
                     symtable_t *tbl, ndt_context_t *ctx)
{
    int n, i, k;

    assert(p[0]->tag == EllipsisDim);

    for (i=pshape-1, k=cshape-1; i>=0 && k>=0; i--, k--) {
        if (i == 0) {
            return resolve_ellipsis(p[0]->EllipsisDim.name, c, k+1, tbl, ctx);
        }
        n = match_single(p[i], c[k], true, tbl, ctx);
        if (n <= 0) return n;
    }

    if (i == 0 && k == -1) {
        return resolve_ellipsis(p[0]->EllipsisDim.name, c, 0, tbl, ctx);
    }

    return i <= 0 && k == -1;
}

static int
match_dimensions(const ndt_t *p[], int pshape,
                 const ndt_t *c[], int cshape,
                 symtable_t *tbl, ndt_context_t *ctx)
{
    int n, i;

    if (pshape > 0 && p[0]->tag == EllipsisDim) {
        return match_dimensions_rev(p, pshape, c, cshape, tbl, ctx);
    }

    for (i = 0; i < pshape && i < cshape; i++) {
        n = match_single(p[i], c[i], false, tbl, ctx);
        if (n <= 0) return n;
    }

    return i == pshape && i == cshape;
}

static int
match_tuple_fields(const ndt_t *p, const ndt_t *c, symtable_t *tbl,
                   ndt_context_t *ctx)
{
    int64_t i;
    int n;

    assert(p->tag == Tuple && c->tag == Tuple);

    if (p->Tuple.shape != c->Tuple.shape) {
        return 0;
    }

    for (i = 0; i < p->Tuple.shape; i++) {
        n = match_datashape(p->Tuple.types[i], c->Tuple.types[i], tbl, ctx);
        if (n <= 0) return n;
    }

    return 1;
}

static int
match_record_fields(const ndt_t *p, const ndt_t *c, symtable_t *tbl,
                    ndt_context_t *ctx)
{
    int64_t i;
    int n;

    assert(p->tag == Record && c->tag == Record);

    if (p->Record.shape != c->Record.shape) {
        return 0;
    }

    for (i = 0; i < p->Record.shape; i++) {
        n = strcmp(p->Record.names[i], c->Record.names[i]);
        if (n != 0) return 0;

        n = match_datashape(p->Record.types[i], c->Record.types[i], tbl, ctx);
        if (n <= 0) return n;
    }

    return 1;
}

static int
match_categorical(ndt_value_t *p, int64_t plen,
                  ndt_value_t *c, int64_t clen)
{
    int64_t i;

    if (plen != clen) {
        return 0;
    }

    for (i = 0; i < plen; i++) {
        if (!ndt_value_equal(&p[i], &c[i])) {
            return 0;
        }
    }

    return 1;
}

static int
match_datashape(const ndt_t *p, const ndt_t *c,
                symtable_t *tbl,
                ndt_context_t *ctx)
{
    const ndt_t *pdims[NDT_MAX_DIM];
    const ndt_t *cdims[NDT_MAX_DIM];
    const ndt_t *pdtype;
    const ndt_t *cdtype;
    int pn, cn;
    int n;

    if (ndt_is_optional(c) != ndt_is_optional(p)) return 0;

    switch (p->tag) {
    case AnyKind:
        return 1;
    case FixedDim: case SymbolicDim: case VarDim:
        if (!is_array(c)) return 0; /* fall through */
    case EllipsisDim:
        pn = ndt_dims_dtype(pdims, &pdtype, p);
        cn = ndt_dims_dtype(cdims, &cdtype, c);

        n = match_dimensions(pdims, pn, cdims, cn, tbl, ctx);
        if (n <= 0) return n;

        return match_datashape(pdtype, cdtype, tbl, ctx);
    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case String:
        return p->tag == c->tag;
    case FixedString:
        return c->tag == FixedString &&
               p->FixedString.size == c->FixedString.size &&
               p->FixedString.encoding == c->FixedString.encoding;
    case FixedBytes:
        return c->tag == FixedBytes &&
               p->FixedBytes.size == c->FixedBytes.size &&
               p->FixedBytes.align == c->FixedBytes.align;
    case SignedKind:
        return c->tag == SignedKind || ndt_is_signed(c);
    case UnsignedKind:
        return c->tag == UnsignedKind || ndt_is_unsigned(c);
    case FloatKind:
        return c->tag == FloatKind || ndt_is_float(c);
    case ComplexKind:
        return c->tag == ComplexKind || ndt_is_complex(c);
    case FixedStringKind:
        return c->tag == FixedStringKind || c->tag == FixedString;
    case FixedBytesKind:
        return c->tag == FixedBytesKind || c->tag == FixedBytes;
    case ScalarKind:
        return c->tag == ScalarKind || ndt_is_scalar(c);
    case Char:
        return c->tag == Char && c->Char.encoding == p->Char.encoding;
    case Bytes:
        return c->tag == Bytes && p->Bytes.target_align == c->Bytes.target_align;
    case Categorical:
        return c->tag == Categorical &&
               match_categorical(p->Categorical.types, p->Categorical.ntypes,
                                 c->Categorical.types, c->Categorical.ntypes);
    case Ref:
        if (c->tag != Ref) return 0;
        return match_datashape(p->Ref.type, c->Ref.type, tbl, ctx);
    case Tuple:
        if (p->Tuple.flag == Variadic) return 0;
        if (c->tag != Tuple) return 0;
        return match_tuple_fields(p, c, tbl, ctx);
    case Record:
        if (p->Tuple.flag == Variadic) return 0;
        if (c->tag != Record) return 0;
        return match_record_fields(p, c, tbl, ctx);
    case Function: {
        if (c->tag != Function ||
            c->Function.nin != p->Function.nin ||
            c->Function.nout != p->Function.nout ||
            c->Function.nargs != p->Function.nargs) {
            return 0;
        }

        for (int32_t i = 0; i < p->Function.nargs; i++) {
            n = match_datashape(p->Function.types[i], c->Function.types[i], tbl, ctx);
            if (n <= 0) return n;
        }

        return 1;
    }
    case Typevar:
        if (c->tag == Typevar) {
            symtable_entry_t entry = { .tag = SymbolEntry,
                                       .SymbolEntry = c->Typevar.name };
            return resolve_sym(p->Typevar.name, entry, tbl, ctx);
        }
        else {
            symtable_entry_t entry = { .tag = TypeEntry,
                                       .TypeEntry = c };
            return resolve_sym(p->Typevar.name, entry, tbl, ctx);
        }
    case Nominal:
        /* Assume that the type has been created through ndt_nominal(), in
           which case the name is guaranteed to be unique and present in the
           typedef table. */
        return c->tag == Nominal && strcmp(p->Nominal.name, c->Nominal.name) == 0;
    case Module:
        return c->tag == Module && strcmp(p->Module.name, c->Module.name) == 0 &&
               ndt_equal(p->Module.type, c->Module.type);
    case Constr:
        return c->tag == Constr && strcmp(p->Constr.name, c->Constr.name) == 0 &&
               ndt_equal(p->Constr.type, c->Constr.type);
    }

    /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid type");
}

int
ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx)
{
    symtable_t *tbl;
    int ret;

    if (ndt_is_abstract(c)) {
        return 0;
    }

    tbl = symtable_new(ctx);
    if (tbl == NULL) {
        return -1;
    }

    ret = match_datashape(p, c, tbl, ctx);
    symtable_del(tbl);
    return ret;
}

static ndt_t *
broadcast(const ndt_t *t, const int64_t *shape,
          int32_t outer_dims, int32_t inner_dims,
          bool use_max, ndt_context_t *ctx)
{
    ndt_ndarray_t u;
    const ndt_t *dtype;
    ndt_t *v;
    int64_t step;
    int ndim;
    int i, k;

    ndim = ndt_as_ndarray(&u, t, ctx);
    if (ndim < 0) {
        return NULL;
    }

    dtype = ndt_dtype(t);
    v = ndt_copy(dtype, ctx);
    if (v == NULL) {
        return NULL;
    }

    for (i=ndim-1; i>=ndim-inner_dims; i--) {
        v = ndt_fixed_dim(v, u.shape[i], u.steps[i], ctx);
        if (v == NULL) {
            return NULL;
        }
    }

    for (k=outer_dims-1; i>=0 && k>=0; i--, k--) {
        step = u.shape[i]<=1 ? 0 : u.steps[i];
        v = ndt_fixed_dim(v, shape[k], step, ctx);
        if (v == NULL) {
            return NULL;
        }
    }

    for (; k>=0; k--) {
        if (use_max) {
            v = ndt_fixed_dim(v, shape[k], INT64_MAX, ctx);
        }
        else {
            v = ndt_fixed_dim(v, shape[k], 0, ctx);
        }
        if (v == NULL) {
            return NULL;
        }
    }

    return v;
}

static int
broadcast_all(ndt_apply_spec_t *spec, const ndt_t *sig,
              const ndt_t *in[], const int32_t nin,
              const symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    ndt_t *u;
    int outer_dims;
    int inner_dims;
    int32_t i;

    v = symtable_find(tbl, "00_ELLIPSIS");
    if (v.tag != BroadcastEntry) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "unexpected missing unnamed ellipsis entry");
        return -1;
    }

    outer_dims = v.BroadcastEntry.size;

    for (i = 0; i < nin; i++) {
        inner_dims = sig->Function.types[i]->ndim-1;
        spec->broadcast[i] = broadcast(in[i], v.BroadcastEntry.dims,
                                       outer_dims, inner_dims, false, ctx);
        if (spec->broadcast[i] == NULL) {
            return -1;
        }
        spec->nbroadcast++;
    }

    for (i = 0; i < spec->nout; i++) {
        inner_dims = sig->Function.types[nin+i]->ndim-1;
        u = broadcast(spec->out[i], v.BroadcastEntry.dims,
                      outer_dims, inner_dims, true, ctx);
        if (u == NULL) {
            return -1;
        }
        ndt_del(spec->out[i]);
        spec->out[i] = u;
    }

    spec->outer_dims = outer_dims;

    return 0;
}

static int
resolve_constraint(const ndt_constraint_t *c, const void *args, symtable_t *tbl,
                   ndt_context_t *ctx)
{
    int64_t shapes[NDT_MAX_SYMBOLS];
    symtable_entry_t v;

    for (int i = 0; i < c->nin; i++) {
        v = symtable_find(tbl, c->symbols[i]);
        if (v.tag != ShapeEntry) {
            ndt_err_format(ctx, NDT_ValueError, "expected dimension variable");
            return -1;
        }
        shapes[i] = v.ShapeEntry;
    }

    if (c->f(shapes, args, ctx) < 0) {
        return -1;
    }

    for (int i = 0; i < c->nout; i++) {
        v.tag = ShapeEntry;
        v.ShapeEntry = shapes[c->nin+i];
        if (resolve_sym(c->symbols[c->nin+i], v, tbl, ctx) < 0) {
            return -1;
        }
    }

    return 0;
}

/*
 * Check the concrete function arguments 'in' against the function
 * signature 'sig'.  On success, infer and return the concrete return
 * types and the (possibly broadcasted) 'in' types.
 */
int
ndt_typecheck(ndt_apply_spec_t *spec, const ndt_t *sig,
              const ndt_t *in[], const int32_t nin,
              const ndt_constraint_t *c, const void *args,
              ndt_context_t *ctx)
{
    symtable_t *tbl;
    ndt_t *t;
    const char *name;
    int ret;

    assert(spec->tag == Xnd);
    assert(spec->nout == 0);
    assert(spec->nbroadcast == 0);
    assert(spec->outer_dims == 0);

    if (sig->tag != Function) {
        ndt_err_format(ctx, NDT_ValueError,
            "signature must be a function type");
        return -1;
    }

    if (nin != sig->Function.nin) {
        ndt_err_format(ctx, NDT_ValueError,
            "expected %" PRIi64 " arguments, got %d", sig->Function.nin, nin);
        return -1;
    }

    for (int32_t i = 0; i < nin; i++) {
        if (ndt_is_abstract(in[i])) {
            ndt_err_format(ctx, NDT_ValueError,
                "type checking requires concrete argument types");
            return -1;
        }
    }

    tbl = symtable_new(ctx);
    if (tbl == NULL) {
        return -1;
    }

    for (int32_t i = 0; i < nin; i++) {
        ret = match_datashape(sig->Function.types[i], in[i], tbl, ctx);
        if (ret <= 0) {
            symtable_del(tbl);

            if (ret == 0) {
                ndt_err_format(ctx, NDT_TypeError,
                    "argument types do not match");
            }

            return -1;
        }
    }

    if (c != NULL && resolve_constraint(c, args, tbl, ctx) < 0) {
        symtable_del(tbl);
        return -1;
    }

    for (int32_t i = 0; i < sig->Function.nout; i++) {
        spec->out[i] = ndt_substitute(sig->Function.types[nin+i], tbl, false, ctx);
        if (spec->out[i] == NULL) {
            ndt_apply_spec_clear(spec);
            symtable_del(tbl);
            return -1;
        }
        spec->nout++;
    }

    if (sig->flags & NDT_ELLIPSIS) {
        if (sig->Function.nargs == 0 || sig->Function.types[0]->tag != EllipsisDim) {
            ndt_err_format(ctx, NDT_RuntimeError,
               "unexpected configuration of ellipsis flag and function types");
            ndt_apply_spec_clear(spec);
            symtable_del(tbl);
            return -1;
        }

        t = sig->Function.types[0];
        name = t->EllipsisDim.name;

        if (name != NULL) {
            symtable_entry_t v = symtable_find(tbl, name);
            if (v.tag != DimListEntry) {
                ndt_err_format(ctx, NDT_RuntimeError,
                    "unexpected missing dimension list entry");
                ndt_apply_spec_clear(spec);
                symtable_del(tbl);
                return -1;
            }
            spec->outer_dims = v.DimListEntry.size;
        }
        else {
            if (broadcast_all(spec, sig, in, nin, tbl, ctx) < 0) {
                ndt_apply_spec_clear(spec);
                symtable_del(tbl);
                return -1;
            }
        }
    }

    symtable_del(tbl);

    if (ndt_select_kernel_strategy(spec, sig, in, nin, ctx) < 0) {
        ndt_apply_spec_clear(spec);
        return -1;
    }

    return 0;
}
