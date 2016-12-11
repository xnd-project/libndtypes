/*
 * Copyright (c) 2016, Continuum Analytics, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * 
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "ndtypes.h"
#include "symtable.h"


static int match_datashape(const ndt_t *, const ndt_t *, symtable_t *, ndt_context_t *);


static int
symtable_entry_equal(symtable_entry_t *v, symtable_entry_t *w)
{
    switch (v->tag) {
    case SizeEntry:
        return w->tag == SizeEntry && v->SizeEntry == w->SizeEntry;
    case SymbolEntry:
        return w->tag == SymbolEntry && strcmp(v->SymbolEntry, w->SymbolEntry) == 0;
    case TypeEntry:
        return w->tag == TypeEntry && ndt_equal(v->TypeEntry, w->TypeEntry);
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

    v = symtable_find(tbl, key);
    if (v.tag == Unbound) {
        if (symtable_add(tbl, key, w, ctx) < 0) {
            return -1;
        }
        return 1;
    }

    return symtable_entry_equal(&v, &w);
}

static int
match_dimensions(ndt_dim_t *p, size_t pshape,
                 ndt_dim_t *c, size_t cshape,
                 symtable_t *tbl, ndt_context_t *ctx)
{
    symtable_entry_t v;
    int stride = 1;
    size_t i, k, tmp;
    int n;

    for (i=0, k=0; i!=pshape && k!=cshape; i+=stride, k+=stride) {
        switch (p[i].tag) {
        case EllipsisDim:
            if (i == pshape-stride) {
                return 1;
            }
            tmp = pshape; pshape = i; i = tmp-1;
            tmp = cshape; cshape = k; k = tmp-1;
            stride = -1;
            break;
        case FixedDimKind:
            if (c[k].tag == FixedDimKind || c[k].tag == FixedDim)
                break;
            return 0;
        case FixedDim:
            if (c[k].tag == FixedDim && p[i].FixedDim.shape == c[k].FixedDim.shape)
                break;
            return 0;
        case SymbolicDim:
            switch (c[k].tag) {
            case FixedDim:
                v.tag = SizeEntry;
                v.SizeEntry = c[k].FixedDim.shape;
                break;
            case SymbolicDim:
                v.tag = SymbolEntry;
                v.SymbolEntry = c[k].SymbolicDim.name; /* borrowed */
                break;
            default:
                return 0;
            }
            n = resolve_sym(p[i].SymbolicDim.name, v, tbl, ctx);
            if (n == 1)
                break;
            return n;
        case VarDim:
            if (c[k].tag == VarDim)
                break;
            return 0;
        default: /* NOT REACHED */
            abort();
        }
    }

    return i == pshape && k == cshape;
}

static int
match_tuple_fields(ndt_tuple_field_t *p, size_t pshape,
                   ndt_tuple_field_t *c, size_t cshape,
                   symtable_t *tbl, ndt_context_t *ctx)
{
    size_t i;
    int n;

    if (!!p != !!c || pshape != cshape) {
        return 0;
    }
    if (p == NULL) {
        return 1;
    }

    for (i = 0; i < pshape; i++) {
        n = match_datashape(p[i].type, c[i].type, tbl, ctx);
        if (n <= 0) return n;
    }

    return 1;
}

static int
match_record_fields(ndt_record_field_t *p, size_t pshape,
                    ndt_record_field_t *c, size_t cshape,
                    symtable_t *tbl, ndt_context_t *ctx)
{
    size_t i;
    int n;

    if (!!p != !!c || pshape != cshape) {
        return 0;
    }
    if (p == NULL) {
        return 1;
    }

    for (i = 0; i < pshape; i++) {
        n = strcmp(p[i].name, c[i].name);
        if (n != 0) return 0;

        n = match_datashape(p[i].type, c[i].type, tbl, ctx);
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

    /* XXX: needs to be sorted according to a total order */
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
    int n;

    switch (p->tag) {
    case AnyKind:
        return 1;
    case Void: case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex64: case Complex128:
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
    case RealKind:
        return c->tag == RealKind || ndt_is_real(c);
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
        if (c->tag != Tuple || p->Tuple.flag != c->Tuple.flag) return 0;
        return match_tuple_fields(p->Tuple.fields, p->Tuple.shape,
                                  c->Tuple.fields, c->Tuple.shape,
                                  tbl, ctx);
    case Record:
        if (c->tag != Record || p->Record.flag != c->Record.flag) return 0;
        return match_record_fields(p->Record.fields, p->Record.shape,
                                   c->Record.fields, c->Record.shape,
                                   tbl, ctx);
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
    case Nominal:
        /* Assume that the type has been created through ndt_nominal(), in
           which case the name is guaranteed to be unique and present in the
           typedef table. */
        return c->tag == Nominal && strcmp(p->Nominal.name, c->Nominal.name) == 0;
    case Constr:
        return c->tag == Constr && strcmp(p->Constr.name, c->Constr.name) == 0 &&
               ndt_equal(p->Constr.type, c->Constr.type);
    case Array:
        if (c->tag != Array) return 0;
        n = match_dimensions(p->Array.dim, p->Array.ndim,
                             c->Array.dim, c->Array.ndim,
                             tbl, ctx);
        if (n <= 0) return n;
        return match_datashape(p->Array.dtype, c->Array.dtype, tbl, ctx);
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


