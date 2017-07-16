/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, plures
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
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "ndtypes.h"
#include "symtable.h"


static int match_datashape(const ndt_t *, const ndt_t *, symtable_t *, ndt_context_t *);
static int match_dimensions(const ndt_t *p[], int pshape, const ndt_t *c[], int cshape, symtable_t *tbl, ndt_context_t *ctx);


static int
symtable_entry_equal(symtable_entry_t *v, symtable_entry_t *w, symtable_t *tbl,
                     ndt_context_t *ctx)
{
    switch (v->tag) {
    case SizeEntry:
        return w->tag == SizeEntry && v->SizeEntry == w->SizeEntry;
    case SymbolEntry:
        return w->tag == SymbolEntry && strcmp(v->SymbolEntry, w->SymbolEntry) == 0;
    case TypeEntry:
        return w->tag == TypeEntry && ndt_equal(v->TypeEntry, w->TypeEntry);
    case DimListEntry:
        if (w->tag != DimListEntry)
            return 0;

        return match_dimensions(v->DimListEntry.dims, v->DimListEntry.size,
                                w->DimListEntry.dims, w->DimListEntry.size,
                                tbl, ctx);
    default:
        return 0;
    }
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
match_dimensions(const ndt_t *p[], int pshape,
                 const ndt_t *c[], int cshape,
                 symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    int stride = 1;
    int i, j, k, tmp;
    int n;

    for (i=0, k=0; i!=pshape && k!=cshape; i+=stride, k+=stride) {
        switch (p[i]->tag) {
        case EllipsisDim:
            if (i == pshape-stride) {
                if (p[i]->EllipsisDim.name != NULL) {
                    int size = abs(cshape-k);

                    if (size == 1 && c[k]->tag == EllipsisDim) {
                        if (c[k]->EllipsisDim.name != NULL) {
                            v.tag = SymbolEntry;
                            v.SymbolEntry = c[k]->EllipsisDim.name; /* borrowed */
                            goto resolve;
                        }
                        return 0;
                    }

                    v.tag = DimListEntry;
                    v.DimListEntry.size = size;
                    v.DimListEntry.dims = ndt_alloc(size, sizeof *v.DimListEntry.dims);

                    if (v.DimListEntry.dims == NULL) {
                        (void)ndt_memory_error(ctx);
                        return -1;
                    }

                    for (j = k; j != cshape; j+=stride) {
                        switch(c[k]->tag) {
                        case FixedDim: case VarDim:
                            v.DimListEntry.dims[abs(k-j)] = c[j];
                            break;
                        default:
                            ndt_free(v.DimListEntry.dims);
                            return 0;
                        }
                    }

                resolve:
                    return resolve_sym(p[i]->EllipsisDim.name, v, tbl, ctx);
                }

                return 1;
            }

            tmp = pshape; pshape = i-1; i = tmp;
            tmp = cshape; cshape = k-1; k = tmp;
            stride = -1;
            break;

        case FixedDim:
            if (c[k]->tag == FixedDim && p[i]->FixedDim.shape == c[k]->FixedDim.shape)
                break;
            return 0;
        case SymbolicDim:
            switch (c[k]->tag) {
            case FixedDim:
                v.tag = SizeEntry;
                v.SizeEntry = c[k]->FixedDim.shape;
                break;
            case SymbolicDim:
                v.tag = SymbolEntry;
                v.SymbolEntry = c[k]->SymbolicDim.name; /* borrowed */
                break;
            default:
                return 0;
            }
            n = resolve_sym(p[i]->SymbolicDim.name, v, tbl, ctx);
            if (n == 1)
                break;
            return n;
        case VarDim:
            if (c[k]->tag == VarDim)
                break;
            return 0;
        default: /* NOT REACHED */
            abort();
        }
    }

    if (k == cshape && pshape != i && p[i]->tag == EllipsisDim) {
        if (p[i]->EllipsisDim.name != NULL) {
            v.tag = DimListEntry;
            v.DimListEntry.size = 0;
            v.DimListEntry.dims = NULL;
            return resolve_sym(p[i]->EllipsisDim.name, v, tbl, ctx);
        }
       return 1;
    }

    return i == pshape && k == cshape;
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
match_categorical(ndt_memory_t *p, size_t plen,
                  ndt_memory_t *c, size_t clen)
{
    size_t i;

    if (plen != clen) {
        return 0;
    }

    for (i = 0; i < plen; i++) {
        if (!ndt_memory_equal(&p[i], &c[i])) {
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
    size_t pn, cn;
    int n;

    switch (p->tag) {
    case AnyKind:
        return 1;
    case FixedDim: case SymbolicDim: case VarDim: case EllipsisDim:
        if (!ndt_is_array(c)) return 0;
        if (ndt_is_optional(c) != ndt_is_optional(p)) return 0;
        if (ndt_is_column_major(c) != ndt_is_column_major(p)) return 0;

        pn = ndt_const_dims_dtype(pdims, &pdtype, p);
        cn = ndt_const_dims_dtype(cdims, &cdtype, c);

        n = match_dimensions(pdims, pn, cdims, cn, tbl, ctx);
        if (n <= 0) return n;

        return match_datashape(pdtype, cdtype, tbl, ctx);
    case Array:
        if (c->tag == Array) {
            return match_datashape(p->Array.type, c->Array.type, tbl, ctx);
        }
        return 0;
    case Void: case Bool:
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
    case Pointer:
        if (c->tag != Pointer) return 0;
        return match_datashape(p->Pointer.type, c->Pointer.type, tbl, ctx);
    case Tuple:
        if (c->tag != Tuple) return 0;
        return match_tuple_fields(p, c, tbl, ctx);
    case Record:
        if (c->tag != Record) return 0;
        return match_record_fields(p, c, tbl, ctx);
    case Function:
        if (c->tag != Function) return 0;
        n = match_datashape(p->Function.ret, c->Function.ret, tbl, ctx);
        if (n <= 0) return n;

        n = match_datashape(p->Function.pos, c->Function.pos, tbl, ctx);
        if (n <= 0) return n;

        return match_datashape(p->Function.kwds, c->Function.kwds, tbl, ctx);
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
    case Option:
        if (c->tag != Option) return 0;
        return match_datashape(p->Option.type, c->Option.type, tbl, ctx);
    case OptionItem:
        if (c->tag != OptionItem) return 0;
        return match_datashape(p->OptionItem.type, c->OptionItem.type, tbl, ctx);
    case Nominal:
        /* Assume that the type has been created through ndt_nominal(), in
           which case the name is guaranteed to be unique and present in the
           typedef table. */
        return c->tag == Nominal && strcmp(p->Nominal.name, c->Nominal.name) == 0;
    case Constr:
        return c->tag == Constr && strcmp(p->Constr.name, c->Constr.name) == 0 &&
               ndt_equal(p->Constr.type, c->Constr.type);
    default: /* NOT REACHED */
        abort();
    }
}

int
ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx)
{
    symtable_t *tbl;
    int ret;

    tbl = symtable_new(ctx);
    if (tbl == NULL) {
        return -1;
    }

    ret = match_datashape(p, c, tbl, ctx);
    symtable_del(tbl);
    return ret;
}


/**********************************************************************/
/*                        Experimental section                        */
/**********************************************************************/

static char_opt_t none_char = {None, '\0'};
static int64_opt_t none_int64 = {None, 0};

/* For demonstration: only handles fixed, symbolic, int64 */
static ndt_t *
ndt_substitute(const ndt_t *t, const symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    const ndt_t *w;
    ndt_t *u;
    int i;

    switch (t->tag) {
    case Array:
        u = ndt_substitute(t->Array.type, tbl, ctx);
        if (u == NULL) {
            return NULL;
        }

        return ndt_array(u, NULL, none_int64, none_int64, none_char, ctx);

    case FixedDim:
        u = ndt_substitute(t->FixedDim.type, tbl, ctx);
        if (u == NULL) {
            return NULL;
        }

        return ndt_fixed_dim(t->FixedDim.shape, u, ctx);

    case SymbolicDim:
        v = symtable_find(tbl, t->SymbolicDim.name);

        switch (v.tag) {
        case SizeEntry:
            u = ndt_substitute(t->SymbolicDim.type, tbl, ctx);
            if (u == NULL) {
                return NULL;
            }
            return ndt_fixed_dim(v.SizeEntry, u, ctx);

        default:
            ndt_err_format(ctx, NDT_ValueError,
                "variable is not found or has incorrect type");
            return NULL;
        }

    case EllipsisDim:
        if (t->EllipsisDim.name == NULL) {
            ndt_err_format(ctx, NDT_ValueError,
                "cannot substitute unnamed ellipsis dimension");
            return NULL;
        }

        u = ndt_substitute(t->EllipsisDim.type, tbl, ctx);
        if (u == NULL) {
            return NULL;
        }

        v = symtable_find(tbl, t->EllipsisDim.name);

        switch (v.tag) {
        case DimListEntry:
            for (i = v.DimListEntry.size-1; i >= 0; i--) {
                w = v.DimListEntry.dims[i];
                switch (w->tag) {
                case FixedDim:
                    u = ndt_fixed_dim(w->FixedDim.shape, u, ctx);
                    if (u == NULL) {
                        return NULL;
                    }
                    break;
               default:
                   ndt_err_format(ctx, NDT_NotImplementedError,
                       "substitution not implemented for this type");
                   ndt_del(u);
                   return NULL;
                }
            }

            return u;

        default:
            ndt_err_format(ctx, NDT_ValueError,
                "variable is not found or has incorrect type");
            return NULL;
        }

    case Typevar:
        v = symtable_find(tbl, t->Typevar.name);
        switch (v.tag) {
        case TypeEntry:
            return ndt_substitute(v.TypeEntry, tbl, ctx);

        default:
            ndt_err_format(ctx, NDT_ValueError,
                "variable is not found or has incorrect type");
            return NULL;
        }

    case Int64: case Float32: case Float64:
        return ndt_primitive(t->tag, 'L', ctx);

    default:
        ndt_err_format(ctx, NDT_NotImplementedError,
            "substitution not implemented for this type");
        return NULL;
    }
}

/*
 * Check the concrete function arguments 'args' against the function
 * signature 'f'.  On success, infer and return the concrete return
 * type.
 */
ndt_t *
ndt_typecheck(const ndt_t *f, const ndt_t *args, ndt_context_t *ctx)
{
    symtable_t *tbl;
    ndt_t *return_type;
    int ret;

    if (f->tag != Function) {
        ndt_err_format(ctx, NDT_RuntimeError, "expected function type");
        return NULL;
    }

    if (f->Function.kwds->Record.shape != 0) {
        ndt_err_format(ctx, NDT_NotImplementedError, "kwargs not implemented");
        return NULL;
    }

    if (!ndt_is_concrete(args)) {
        ndt_err_format(ctx, NDT_RuntimeError, "expected concrete argument types");
        return NULL;
    }

    tbl = symtable_new(ctx);
    if (tbl == NULL) {
        return NULL;
    }

    ret = match_datashape(f->Function.pos, args, tbl, ctx);
    if (ret <= 0) {
        symtable_del(tbl);
        return NULL;
    }

    return_type = ndt_substitute(f->Function.ret, tbl, ctx);
    symtable_del(tbl);

    return return_type;
}
