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

static int
_resolve_broadcast(int64_t vshape[NDT_MAX_DIM], int vsize,
                   const int64_t wshape[NDT_MAX_DIM], int wsize)
{
    int64_t n, m;
    int i, k;

    for (i=vsize-1, k=wsize-1; i>=0 && k>=0; i--, k--) {
        n = vshape[i];
        m = wshape[k];
        if (m != n) {
            if (n == 1) {
                n = m;
            }
            else if (m == 0) {
                n = 0;
            }
            else if (m != 1) {
                return -1;
            }
        }
        vshape[i<k ? k : i] = n;
    }
    for (; k >= 0; k--) {
        vshape[k] = wshape[k];
    }

    return vsize >= wsize ? vsize : wsize;
}

static int
resolve_broadcast(symtable_entry_t w, symtable_t *tbl, ndt_context_t *ctx)
{
    const char *key = "00_ELLIPSIS";
    symtable_entry_t *v;
    int vsize;

    v = symtable_find_ptr(tbl, key);
    if (v == NULL) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    vsize = _resolve_broadcast(v->BroadcastSeq.dims, v->BroadcastSeq.size,
                               w.BroadcastSeq.dims, w.BroadcastSeq.size);
    if (vsize < 0) {
        ndt_err_format(ctx, NDT_TypeError, "broadcast error");
        return -1;
    }
    v->BroadcastSeq.size = vsize;

    return 1;
}

static int
check_contig(ndt_t *ptypes[], ndt_t *ctypes[], int64_t nargs)
{
    for (int i = 0; i < nargs; i++) {
        const ndt_t *p = ptypes[i];
        const ndt_t *c = ctypes[i];

        if (p->tag == EllipsisDim) {
            switch (p->EllipsisDim.tag) {
            case RequireNA:
                break;
            case RequireC:
                if (!ndt_is_c_contiguous(c)) {
                    return 0;
                }
                break;
            case RequireF:
                if (!ndt_is_f_contiguous(c)) {
                    return 0;
                }
                break;
            }
        }
    }

    return 1;
}

static ndt_t *
to_fortran(const ndt_t *p, ndt_t *c, ndt_context_t *ctx)
{
    if (p->tag == EllipsisDim && p->EllipsisDim.tag == RequireF) {
        ndt_t *t = ndt_to_fortran(c, ctx);
        return t;
    }
    else {
        return c;
    }
}

static int
resolve_fixed(const char *key, symtable_entry_t w,
              symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    if (w.FixedSeq.size != v.FixedSeq.size) {
        return 0;
    }

    for (int i = 0; i < v.FixedSeq.size; i++) {
        const ndt_t *t = v.FixedSeq.dims[i];
        const ndt_t *u = w.FixedSeq.dims[i];
        if (u->FixedDim.shape != t->FixedDim.shape) {
            return 0;
        }
    }

    return 1;
}

static int
resolve_shape(const char *key, int64_t shape, symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        v.tag = Shape;
        v.Shape = shape;
        if (symtable_add(tbl, key, v, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    if (v.tag != Shape) {
        return 0;
    }

    return shape == v.Shape;
}

static int
resolve_typevar(const char *key, symtable_entry_t w, symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    if (v.tag == Symbol && w.tag == Symbol) {
        return strcmp(v.Symbol, w.Symbol) == 0;
    }
    else if (v.tag == Type && w.tag == Type) {
        return ndt_equal(v.Type, w.Type);
    }
    else {
        return 0;
    }
}

static int
match_concrete_var_dim(const ndt_t *t, int64_t tindex,
                       const ndt_t *u, int64_t uindex,
                       const int outer_dims, ndt_context_t *ctx)
{
    int64_t tshape, tstart, tstep;
    int64_t ushape, ustart, ustep;

    if (outer_dims == 0) {
        return 1;
    }
    if (t->Concrete.VarDim.itemsize != u->Concrete.VarDim.itemsize) {
        return 0;
    }

    tshape = ndt_var_indices(&tstart, &tstep, t, tindex, ctx);
    if (tshape < 0) {
        return -1;
    }

    ushape = ndt_var_indices(&ustart, &ustep, u, uindex, ctx);
    if (ushape < 0) {
        return -1;
    }

    if (ushape != tshape) {
        return 0;
    }

    for (int64_t i = 0; i < tshape; i++) {
        int64_t tnext = tstart + i * tstep;
        int64_t unext = ustart + i * ustep;
        int ret = match_concrete_var_dim(t->VarDim.type, tnext,
                                         u->VarDim.type, unext,
                                         outer_dims-1, ctx);
        if (ret <= 0) {
            return ret;
        }
    }

    return 1;
}

static int
resolve_var(symtable_entry_t w, symtable_t *tbl, ndt_context_t *ctx)
{
    const char *key = "var";
    symtable_entry_t v;

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    if (w.VarSeq.size != v.VarSeq.size) {
        return 0;
    }
    if (v.VarSeq.size == 0) {
        return 1;
    }

    return match_concrete_var_dim(w.VarSeq.dims[0], 0,
                                  v.VarSeq.dims[0], 0,
                                  v.VarSeq.size, ctx);
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

static const ndt_t *
outer_inner(symtable_entry_t *v, int i, const ndt_t *t, int ndim)
{
    assert(ndt_is_concrete(t));

    if (t->ndim < ndim) {
        return NULL;
    }
    if (t->ndim == ndim) {
        return t;
    }

    switch (t->tag) {
    case FixedDim: {
        switch (v->tag) {
        case FixedSeq:
            v->FixedSeq.size = i+1;
            v->FixedSeq.dims[i] = t;
            break;
        case BroadcastSeq:
            v->BroadcastSeq.size = i+1;
            v->BroadcastSeq.dims[i] = t->FixedDim.shape;
            break;
        default:
            return NULL;
        }
        return outer_inner(v, i+1, t->FixedDim.type, ndim);
    }
    case VarDim: {
        switch (v->tag) {
        case VarSeq:
            v->VarSeq.size = i+1;
            v->VarSeq.dims[i] = t;
            break;
        default:
            return NULL;
        }
        return outer_inner(v, i+1, t->VarDim.type, ndim);
    }
    default:
        return NULL;
    }
}

static int
match_datashape(const ndt_t *p, const ndt_t *c, symtable_t *tbl,
                ndt_context_t *ctx)
{
    int n;

    if (ndt_is_optional(c) != ndt_is_optional(p)) return 0;

    switch (p->tag) {
    case AnyKind: {
        return 1;
    }

    case FixedDim: {
        if (c->tag != FixedDim || p->FixedDim.shape != c->FixedDim.shape) {
            return 0;
        }
        if (p->FixedDim.tag == RequireC && !ndt_is_c_contiguous(c)) {
            return 0;
        }
        if (p->FixedDim.tag == RequireF && !ndt_is_f_contiguous(c)) {
            return 0;
        }

        return match_datashape(p->FixedDim.type, c->FixedDim.type, tbl, ctx);
    }

    case VarDim: {
        if (c->tag != VarDim) {
            return 0;
        }
        return match_datashape(p->VarDim.type, c->VarDim.type, tbl, ctx);
    }

    case SymbolicDim: {
        if (c->tag != FixedDim) return 0;

        if (p->SymbolicDim.tag == RequireC && !ndt_is_c_contiguous(c)) {
            return 0;
        }
        if (p->SymbolicDim.tag == RequireF && !ndt_is_f_contiguous(c)) {
            return 0;
        }

        n = resolve_shape(p->SymbolicDim.name, c->FixedDim.shape, tbl, ctx);
        if (n <= 0) {
            return n;
        }
        return match_datashape(p->SymbolicDim.type, c->FixedDim.type, tbl, ctx);
    }

    case EllipsisDim: {
        symtable_entry_t outer;
        const ndt_t *inner;

        if (p->EllipsisDim.tag == RequireC && !ndt_is_c_contiguous(c)) {
            return 0;
        }
        if (p->EllipsisDim.tag == RequireF && !ndt_is_f_contiguous(c)) {
            return 0;
        }

        if (p->EllipsisDim.name == NULL) {
            outer.tag = BroadcastSeq;
            outer.BroadcastSeq.size = 0;
        }
        else if (strcmp(p->EllipsisDim.name, "var") == 0) {
            outer.tag = VarSeq;
            outer.VarSeq.size = 0;
        }
        else {
            outer.tag = FixedSeq;
            outer.FixedSeq.size = 0;
        }

        inner = outer_inner(&outer, 0, c, p->EllipsisDim.type->ndim);
        if (inner == NULL) {
            return 0;
        }

        n = match_datashape(p->EllipsisDim.type, inner, tbl, ctx);
        if (n <= 0) {
            return n;
        }

        switch (outer.tag) {
        case BroadcastSeq:
            return resolve_broadcast(outer, tbl, ctx);
        case FixedSeq:
            return resolve_fixed(p->EllipsisDim.name, outer, tbl, ctx);
        case VarSeq:
            return resolve_var(outer, tbl, ctx);
        default: /* NOT REACHED */
            ndt_internal_error("invalid tag");
        }
    }

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
        int64_t i;
        if (c->tag != Function ||
            c->Function.nin != p->Function.nin ||
            c->Function.nout != p->Function.nout ||
            c->Function.nargs != p->Function.nargs) {
            return 0;
        }

        for (i = 0; i < p->Function.nargs; i++) {
            n = match_datashape(p->Function.types[i], c->Function.types[i], tbl, ctx);
            if (n <= 0) return n;
        }

        return check_contig(p->Function.types, c->Function.types, p->Function.nargs);
    }
    case Typevar: {
        if (c->tag == Typevar) {
            symtable_entry_t entry = { .tag=Symbol, .Symbol=c->Typevar.name };
            return resolve_typevar(p->Typevar.name, entry, tbl, ctx);
        }
        else {
            symtable_entry_t entry = { .tag=Type, .Type=c };
            return resolve_typevar(p->Typevar.name, entry, tbl, ctx);
        }
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
          int outer_dims, int inner_dims,
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

int
ndt_broadcast_all(ndt_apply_spec_t *spec, const ndt_t *sig,
                  const ndt_t *in[], const int nin,
                  const int64_t *shape, int outer_dims,
                  ndt_context_t *ctx)
{
    ndt_t *u;
    int inner_dims;
    int i;

    for (i = 0; i < nin; i++) {
        inner_dims = sig->Function.types[i]->ndim-1;
        spec->broadcast[i] = broadcast(in[i], shape,
                                       outer_dims, inner_dims, false, ctx);
        if (spec->broadcast[i] == NULL) {
            return -1;
        }
        spec->nbroadcast++;
    }

    for (i = 0; i < spec->nout; i++) {
        inner_dims = sig->Function.types[nin+i]->ndim-1;
        u = broadcast(spec->out[i], shape,
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
broadcast_all(ndt_apply_spec_t *spec, const ndt_t *sig,
              const ndt_t *in[], const int nin,
              const symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;

    v = symtable_find(tbl, "00_ELLIPSIS");
    if (v.tag != BroadcastSeq) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "unexpected missing unnamed ellipsis entry");
        return -1;
    }

    return ndt_broadcast_all(spec, sig, in, nin,
                             v.BroadcastSeq.dims, v.BroadcastSeq.size,
                             ctx);
}

static int
resolve_constraint(const ndt_constraint_t *c, const void *args, symtable_t *tbl,
                   ndt_context_t *ctx)
{
    int64_t shapes[NDT_MAX_SYMBOLS];
    symtable_entry_t v;

    for (int i = 0; i < c->nin; i++) {
        v = symtable_find(tbl, c->symbols[i]);
        if (v.tag != Shape) {
            ndt_err_format(ctx, NDT_ValueError, "expected dimension variable");
            return -1;
        }
        shapes[i] = v.Shape;
    }

    if (c->f(shapes, args, ctx) < 0) {
        return -1;
    }

    for (int i = 0; i < c->nout; i++) {
        if (resolve_shape(c->symbols[c->nin+i], shapes[c->nin+i], tbl, ctx) < 0) {
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
              const ndt_t *in[], const int nin,
              const ndt_constraint_t *c, const void *args,
              ndt_context_t *ctx)
{
    symtable_t *tbl;
    ndt_t *t;
    const char *name;
    int ret;
    int64_t i;

    assert(spec->flags == 0);
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

    for (i = 0; i < nin; i++) {
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

    for (i = 0; i < nin; i++) {
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

    for (i = 0; i < sig->Function.nout; i++) {
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
            switch (v.tag) {
            case FixedSeq:
                spec->outer_dims = v.FixedSeq.size;
                break;
            case VarSeq:
                spec->outer_dims = v.VarSeq.size;
                break;
            default:
                ndt_err_format(ctx, NDT_RuntimeError,
                    "unexpected missing dimension list entry");
                ndt_apply_spec_clear(spec);
                symtable_del(tbl);
                return -1;
            }
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

    for (i = 0; i < sig->Function.nout; i++) {
        ndt_t *_p = sig->Function.types[nin+i];
        ndt_t *_c = spec->out[i];
        ndt_t *_t = to_fortran(_p, _c, ctx);
        if (_t == NULL) {
            ndt_apply_spec_clear(spec);
            return -1;
        }
        if (_t != _c) {
            ndt_del(_c);
        }
        spec->out[i] = _t;
    }

    if (!check_contig(sig->Function.types, (ndt_t **)in, nin)) {
        ndt_err_format(ctx, NDT_TypeError, "argument types do not match");
        return -1;
    }
    if (!check_contig(sig->Function.types+nin, spec->out, spec->nout)) {
        ndt_err_format(ctx, NDT_TypeError, "argument types do not match");
        return -1;
    }

    ndt_select_kernel_strategy(spec, sig, in, nin);

    return 0;
}


/*****************************************************************************/
/*                  Optimized binary typecheck for fixed input               */
/*****************************************************************************/

static ndt_t *
binary_broadcast_1D(const ndt_ndarray_t *t, const ndt_t *dtype,
                    const int64_t *shape, int size, ndt_context_t *ctx)
{
    ndt_t *v;
    int64_t step;
    int i, k;

    v = ndt_copy(dtype, ctx);
    if (v == NULL) {
        return NULL;
    }

    for (i=t->ndim-1, k=size-1; i>=0 && k>=0; i--, k--) {
        step = t->shape[i]<=1 ? 0 : t->steps[i];
        v = ndt_fixed_dim(v, shape[k], step, ctx);
        if (v == NULL) {
            return NULL;
        }
    }

    for (; k>=0; k--) {
        v = ndt_fixed_dim(v, shape[k], 0, ctx);
        if (v == NULL) {
            return NULL;
        }
    }

    return v;
}

static ndt_t *
fixed_dim_from_shape(const int64_t shape[], int len, ndt_t *dtype,
                     ndt_context_t *ctx)
{
    ndt_t *t;
    int i;

    for (i=len-1, t=dtype; i >= 0; i--) {
        t = ndt_fixed_dim(t, shape[i], INT64_MAX, ctx);
        if (t == NULL) {
            return NULL;
        }
    }

    return t;
}

static bool
shape_equal(const ndt_ndarray_t *a, const ndt_ndarray_t *b)
{
    if (b->ndim != a->ndim) {
        return false;
    }

    for (int i = 0; i < a->ndim; i++) {
        if (b->shape[i] != a->shape[i]) {
            return false;
        }
    }

    return true;
}

static int
_ndt_binary_broadcast(ndt_apply_spec_t *spec, const ndt_t *sig,
                      const ndt_ndarray_t *x, const ndt_ndarray_t *y,
                      const ndt_t *in[], const int nin, ndt_t *dtype,
                      int inner, ndt_context_t *ctx)
{
    int64_t shape[NDT_MAX_DIM];
    int size;

    if (shape_equal(x, y)) {
        spec->nout = 1;
        spec->nbroadcast = 0;
        spec->outer_dims = x->ndim-inner;
        spec->out[0] = fixed_dim_from_shape(x->shape, x->ndim, dtype, ctx);
        if (spec->out[0] == NULL) {
            return -1;
        }
    }
    else {
        for (int i = 0; i < x->ndim; i++) {
            shape[i] = x->shape[i];
        }

        size = _resolve_broadcast(shape, x->ndim, y->shape, y->ndim);
        if (size < 0) {
            ndt_err_format(ctx, NDT_TypeError, "broadcast error");
            ndt_del(dtype);
            return -1;
        }

        spec->nout = 1;
        spec->nbroadcast = 2;
        spec->outer_dims = size-inner;

        spec->out[0] = fixed_dim_from_shape(shape, size, dtype, ctx);
        if (spec->out[0] == NULL) {
            return -1;
        }

        spec->broadcast[0] = binary_broadcast_1D(x, ndt_dtype(in[0]), shape, size, ctx);
        if (spec->broadcast[0] == NULL) {
            ndt_del(spec->out[0]);
            return -1;
        }

        spec->broadcast[1] = binary_broadcast_1D(y, ndt_dtype(in[1]), shape, size, ctx);
        if (spec->broadcast[1] == NULL) {
            ndt_del(spec->out[0]);
            ndt_del(spec->broadcast[0]);
            return -1;
        }
    }

    ndt_select_kernel_strategy(spec, sig, in, nin);

    return 0;
}

static bool
all_ellipses(const ndt_t *t0, const ndt_t *t1, const ndt_t *t2,
             ndt_context_t *ctx)
{
    if ((t0->tag != EllipsisDim || t0->EllipsisDim.name != NULL) ||
        (t1->tag != EllipsisDim || t1->EllipsisDim.name != NULL) ||
        (t2->tag != EllipsisDim || t2->EllipsisDim.name != NULL)) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "fast binary typecheck expects leading ellipsis dimensions");
        return false;
    }

    return true;
}

static bool
all_same_symbol(const ndt_t *t0, const ndt_t *t1, const ndt_t *t2)
{
    if (t0->tag != SymbolicDim || t1->tag != SymbolicDim ||
        t2->tag != SymbolicDim) {
        return false;
    }

    return strcmp(t0->SymbolicDim.name, t1->SymbolicDim.name) == 0 &&
           strcmp(t0->SymbolicDim.name, t2->SymbolicDim.name) == 0;
}

static bool
all_ndim0(const ndt_t *t0, const ndt_t *t1, const ndt_t *t2)
{
    return t0->ndim == 0 && t1->ndim == 0 && t2->ndim == 0;
}

/*
 * Optimized type checking for very specific signatures. The caller must
 * have identified the kernel location, signature and the dtype.  For
 * performance reasons, no substitution is performed on the dtype, so
 * the dtype must be concrete.
 *
 * Supported signatures:
 *   1) ... * N * T0, ... * N * T1 -> N * T2
 *   2) ... * T0, ... * T1 -> ... * T2
 */
int
ndt_fast_binary_fixed_typecheck(ndt_apply_spec_t *spec, const ndt_t *sig,
                                const ndt_t *in[], const int nin, ndt_t *dtype,
                                ndt_context_t *ctx)
{
    ndt_t *p0, *p1, *p2;
    ndt_ndarray_t x, y;

    assert(spec->flags == 0);
    assert(spec->nout == 0);
    assert(spec->nbroadcast == 0);
    assert(spec->outer_dims == 0);

    if (sig->tag != Function ||
        sig->Function.nin != 2 ||
        sig->Function.nout != 1) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "fast binary typecheck expects a signature with two inputs and "
            "one output");
        return -1;
    }

    if (nin != 2) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "fast binary typecheck expects two input arguments");
        return -1;
    }

    if (ndt_is_abstract(dtype)) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "fast binary typecheck expects a concrete dtype");
        return -1;
    }

    p0 = sig->Function.types[0];
    p1 = sig->Function.types[1];
    p2 = sig->Function.types[2];

    if (!all_ellipses(p0, p1, p2, ctx)) {
        return -1;
    }

    if (ndt_as_ndarray(&x, in[0], ctx) < 0) {
        ndt_del(dtype);
        return -1;
    }

    if (ndt_as_ndarray(&y, in[1], ctx) < 0) {
        ndt_del(dtype);
        return -1;
    }

    p0 = p0->EllipsisDim.type;
    p1 = p1->EllipsisDim.type;
    p2 = p2->EllipsisDim.type;

    if (all_same_symbol(p0, p1, p2)) {
        if (x.ndim > 0 && y.ndim > 0) {
            const int64_t xshape = x.shape[x.ndim-1];
            const int64_t yshape = y.shape[y.ndim-1];
            if (xshape != 1 && yshape != 1 && xshape != yshape) {
                ndt_err_format(ctx, NDT_TypeError, "mismatch in inner dimensions");
                ndt_del(dtype);
                return -1;
            }
        }
        return _ndt_binary_broadcast(spec, sig, &x, &y, in, nin, dtype, 1, ctx);
    }
    else if (all_ndim0(p0, p1, p2)) {
        return _ndt_binary_broadcast(spec, sig, &x, &y, in, nin, dtype, 0, ctx);
    }
    else {
        ndt_err_format(ctx, NDT_RuntimeError,
            "unsupported signature in fast binary typecheck");
        return -1;
    }
}
