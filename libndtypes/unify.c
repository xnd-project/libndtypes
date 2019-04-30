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
#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "parsefuncs.h"
#include "seq.h"


/*****************************************************************************/
/*                              Unify two types                              */
/*****************************************************************************/

/*
 * Unify two types and return a type that can hold all values of both input
 * types.  Because of slicing, steps, etc. the returned type cannot be used
 * in general to describe an existing memory block.
 */

static const ndt_t *unify(const ndt_t *t, const ndt_t *u, bool replace_any, ndt_context_t *ctx);

static enum ndt
max_tag(enum ndt x, enum ndt y)
{
    return x >= y ? x : y;
}

static enum ndt_encoding
max_encoding(enum ndt_encoding x, enum ndt_encoding y)
{
    return x >= y ? x : y;
}

static int64_t
max_i64(int64_t x, int64_t y)
{
    return x >= y ? x : y;
}

static uint16_t
max_u16(uint16_t x, uint16_t y)
{
    return x >= y ? x : y;
}


static ndt_t *
unification_error(const char *msg, ndt_context_t *ctx)
{
    ndt_err_format(ctx, NDT_ValueError, "unification failed: %s", msg);
    return NULL;
}

static const ndt_t *
unify_common(ndt_t *w, const ndt_t *t, const ndt_t *u, ndt_context_t *ctx)
{
    if ((t->flags & ~(NDT_OPTION|NDT_SUBTREE_OPTION|NDT_POINTER)) !=
        (u->flags & ~(NDT_OPTION|NDT_SUBTREE_OPTION|NDT_POINTER))) {
        ndt_decref(w);
        return unification_error("flags differ", ctx);
    }

    if (ndt_is_static(w)) {
        return ndt_primitive(w->tag, t->flags | u->flags, ctx);
    }

    w->flags = t->flags | u->flags;
    return w;
}

static const ndt_t *
unify_tuple(const ndt_t *t, const ndt_t *u, bool replace_any, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t) || ndt_is_optional(u);
    ndt_field_seq_t *seq;
    ndt_field_t *field;
    const ndt_t *tmp, *w;
    int64_t shape;
    int64_t i;

    shape = t->Tuple.shape;
    if (shape == 0) {
        return mk_tuple(t->Tuple.flag, NULL, NULL, opt, ctx);
    }

    tmp = unify(t->Tuple.types[0], u->Tuple.types[0], replace_any, ctx);
    if (tmp == NULL) {
        return NULL;
    }

    field = mk_field(NULL, tmp, NULL, ctx); 
    if (field == NULL) {
        return NULL;
    }

    seq = ndt_field_seq_new(field, ctx);
    if (seq == NULL) {
        return NULL;
    }

    for (i = 1; i < t->Tuple.shape; i++) {
        tmp = unify(t->Tuple.types[i], u->Tuple.types[i], replace_any, ctx);
        if (tmp == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        field = mk_field(NULL, tmp, NULL, ctx); 
        if (field == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        seq = ndt_field_seq_append(seq, field, ctx);
        if (seq == NULL) {
            return NULL;
        }
    }

    w = mk_tuple(t->Tuple.flag, seq, NULL, opt, ctx);
    if (w == NULL) {
        return NULL;
    }

    return unify_common((ndt_t *)w, t, u, ctx);
}

static const ndt_t *
unify_record(const ndt_t *t, const ndt_t *u, bool replace_any, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t) || ndt_is_optional(u);
    ndt_field_seq_t *seq;
    ndt_field_t *field;
    const ndt_t *tmp, *w;
    char *name;
    int64_t shape;
    int64_t i;

    shape = t->Record.shape;
    for (i = 0; i < shape; i++) {
        if (strcmp(t->Record.names[i], u->Record.names[i]) != 0) {
            return unification_error("field name mismatch", ctx);
        }
    }

    if (shape == 0) {
        return mk_record(t->Record.flag, NULL, NULL, opt, ctx);
    }

    name = ndt_strdup(t->Record.names[0], ctx);
    if (name == NULL) {
        return NULL;
    }

    tmp = unify(t->Record.types[0], u->Record.types[0], replace_any, ctx);
    if (tmp == NULL) {
        ndt_free(name);
        return NULL;
    }

    field = mk_field(name, tmp, NULL, ctx); 
    if (field == NULL) {
        return NULL;
    }

    seq = ndt_field_seq_new(field, ctx);
    if (seq == NULL) {
        return NULL;
    }

    for (i = 1; i < shape; i++) {
        name = ndt_strdup(t->Record.names[i], ctx);
        if (name == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        tmp = unify(t->Record.types[i], u->Record.types[i], replace_any, ctx);
        if (tmp == NULL) {
            ndt_free(name);
            ndt_field_seq_del(seq);
            return NULL;
        }

        field = mk_field(name, tmp, NULL, ctx); 
        if (field == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        seq = ndt_field_seq_append(seq, field, ctx);
        if (seq == NULL) {
            return NULL;
        }
    }

    w = mk_record(t->Record.flag, seq, NULL, opt, ctx);
    if (w == NULL) {
        return NULL;
    }

    return unify_common((ndt_t *)w, t, u, ctx);
}

static const ndt_t *
unify_union(const ndt_t *t, const ndt_t *u, bool replace_any, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t) || ndt_is_optional(u);
    ndt_field_seq_t *seq;
    ndt_field_t *field;
    const ndt_t *tmp, *w;
    char *name;
    int64_t ntags;
    int64_t i;

    ntags = t->Union.ntags;
    assert(ntags > 0);

    for (i = 0; i < ntags; i++) {
        if (strcmp(t->Union.tags[i], u->Union.tags[i]) != 0) {
            return unification_error("tag mismatch", ctx);
        }
    }

    name = ndt_strdup(t->Union.tags[0], ctx);
    if (name == NULL) {
        return NULL;
    }

    tmp = unify(t->Union.types[0], u->Union.types[0], replace_any, ctx);
    if (tmp == NULL) {
        ndt_free(name);
        return NULL;
    }

    field = mk_field(name, tmp, NULL, ctx); 
    if (field == NULL) {
        return NULL;
    }

    seq = ndt_field_seq_new(field, ctx);
    if (seq == NULL) {
        return NULL;
    }

    for (i = 1; i < ntags; i++) {
        name = ndt_strdup(t->Union.tags[i], ctx);
        if (name == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        tmp = unify(t->Union.types[i], u->Union.types[i], replace_any, ctx);
        if (tmp == NULL) {
            ndt_free(name);
            ndt_field_seq_del(seq);
            return NULL;
        }

        field = mk_field(name, tmp, NULL, ctx); 
        if (field == NULL) {
            ndt_field_seq_del(seq);
            return NULL;
        }

        seq = ndt_field_seq_append(seq, field, ctx);
        if (seq == NULL) {
            return NULL;
        }
    }

    w = mk_union(seq, opt, ctx);
    if (w == NULL) {
        return NULL;
    }

    return unify_common((ndt_t *)w, t, u, ctx);
}

static const ndt_t *
unify_primitive(const ndt_t *t, const ndt_t *u, ndt_context_t *ctx)
{
    enum ndt tag = max_tag(t->tag, u->tag);
    const ndt_t *w;

    w = ndt_primitive(tag, 0, ctx);
    if (w == NULL) {
        return NULL;
    }

    return unify_common((ndt_t *)w, t, u, ctx);
}

static const ndt_t *
unify(const ndt_t *t, const ndt_t *u, bool replace_any, ndt_context_t *ctx)
{
    bool opt = ndt_is_optional(t) || ndt_is_optional(u);
    const ndt_t *type, *w;

    if (replace_any && t->tag == AnyKind && u->tag == AnyKind) {
        return ndt_primitive(Float64, opt, ctx);
    }

    if (u->tag == AnyKind) {
        w = ndt_copy(t, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    if (t->tag == AnyKind) {
        w = ndt_copy(u, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    switch (t->tag) {
    case FixedDim: {
        if (u->tag != FixedDim) {
            return unification_error("different types", ctx);
        }

        if (t->FixedDim.shape != u->FixedDim.shape) {
            return unification_error("mismatch in fixed dimension", ctx);
        }

        type = unify(t->FixedDim.type, u->FixedDim.type, replace_any, ctx);
        if (type == NULL) {
            return NULL;
        }

        w = ndt_fixed_dim(type, t->FixedDim.shape, t->Concrete.FixedDim.step, ctx);
        ndt_decref(type);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case VarDim: {

        if (u->tag != VarDim) {
            return unification_error("different types", ctx);
        }

        if (ndt_is_abstract(t) || ndt_is_abstract(u)) {
            return unification_error("cannot unify abstract var dimension", ctx);
        }

        if (t->Concrete.VarDim.nslices != 0 || u->Concrete.VarDim.nslices != 0) {
            return unification_error("cannot unify sliced var dimension", ctx);
        }

        int32_t noffsets = t->Concrete.VarDim.offsets->n;
        if (u->Concrete.VarDim.offsets->n != noffsets) {
            return unification_error("offset mismatch in var dimension", ctx);
        }

        if (memcmp(t->Concrete.VarDim.offsets->v, u->Concrete.VarDim.offsets->v,
                   noffsets * (sizeof *t->Concrete.VarDim.offsets->v))) {
            return unification_error("shape mismatch in var dimension", ctx);
        }

        type = unify(t->VarDim.type, u->VarDim.type, replace_any, ctx);
        if (type == NULL) {
            return NULL;
        }

        w = ndt_var_dim(type, t->Concrete.VarDim.offsets, 0, NULL, opt, ctx);
        ndt_decref(type);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case VarDimElem: {
        return unification_error("cannot unify VarDimElem", ctx);
    }

    case Tuple: {
        if (u->tag != Tuple) {
            return unification_error("different types", ctx);
        }

        if (t->Tuple.flag != u->Tuple.flag ||
            t->Tuple.shape != u->Tuple.shape) {
            return 0;
        }

        return unify_tuple(t, u, replace_any, ctx);
    }

    case Record: {
        if (u->tag != Record) {
            return unification_error("different types", ctx);
        }

        if (t->Record.flag != u->Record.flag ||
            t->Record.shape != u->Record.shape) {
            return 0;
        }

        return unify_record(t, u, replace_any, ctx);
    }

    case Union: {
        if (u->tag != Union) {
            return unification_error("different types", ctx);
        }

        if (t->Union.ntags != u->Union.ntags) {
            return 0;
        }

        return unify_union(t, u, replace_any, ctx);
    }

    case Ref: {
        if (u->tag != Ref) {
            return unification_error("different types", ctx);
        }

        type = unify(t->Ref.type, u->Ref.type, replace_any, ctx);
        if (type == NULL) {
            return NULL;
        }

        w = ndt_ref(type, opt, ctx);
        ndt_decref(type);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Constr: {
        char *name;

        if (u->tag != Constr) {
            return unification_error("different types", ctx);
        }

        if (strcmp(t->Constr.name, u->Constr.name) != 0) {
            return unification_error("different constructor names", ctx);
        }

        name = ndt_strdup(t->Constr.name, ctx);
        if (name == NULL) {
            return NULL;
        }

        type = unify(t->Constr.type, u->Constr.type, replace_any, ctx);
        if (type == NULL) {
            ndt_free(name);
            return NULL;
        }

        w = ndt_constr(name, type, opt, ctx);
        ndt_decref(type);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Nominal: {
        if (!ndt_equal(t, u)) {
            return unification_error("nominal types must be identical", ctx);
        }

        w = ndt_copy(t, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Categorical: {
        if (!ndt_equal(t, u)) {
            return unification_error("categorical types must be identical", ctx);
        }

        w = ndt_copy(t, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case FixedString: {
        int64_t size;
        enum ndt_encoding encoding;

        if (u->tag != FixedString) {
            return unification_error("different types", ctx);
        }

        size = max_i64(t->FixedString.size, u->FixedString.size);
        encoding = max_encoding(t->FixedString.encoding, u->FixedString.encoding);

        w = ndt_fixed_string(size, encoding, opt, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case FixedBytes: {
        int64_t size;
        uint16_opt_t align = {Some, 1024};

        if (u->tag != FixedBytes) {
            return unification_error("different types", ctx);
        }

        size = max_i64(t->FixedBytes.size, u->FixedBytes.size);
        align.Some = max_u16(t->FixedBytes.align, u->FixedBytes.align);

        w = ndt_fixed_bytes(size, align, opt, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Bytes: {
        uint16_opt_t align = {Some, 1024};

        if (u->tag != Bytes) {
            return unification_error("different types", ctx);
        }

        align.Some = max_u16(t->Bytes.target_align, u->Bytes.target_align);

        w = ndt_bytes(align, opt, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Char: {
        enum ndt_encoding encoding;

        if (u->tag != Char) {
            return unification_error("different types", ctx);
        }

        encoding = max_encoding(t->Char.encoding, u->Char.encoding);

        w = ndt_char(encoding, opt, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case Bool: {
        if (u->tag != Bool) {
            return unification_error("different types", ctx);
        }

        return unify_primitive(t, u, ctx);
    }

    case Int8: case Int16: case Int32: case Int64: {
        if (!ndt_is_signed(u)) {
            return unification_error("different types", ctx);
        }

        return unify_primitive(t, u, ctx);
    }

    case Uint8: case Uint16: case Uint32: case Uint64: {
        if (!ndt_is_unsigned(u)) {
            return unification_error("different types", ctx);
        }

        return unify_primitive(t, u, ctx);
    }

    case BFloat16: case Float16: case Float32: case Float64: {
        if (!ndt_is_float(u)) {
            return unification_error("different types", ctx);
        }

        return unify_primitive(t, u, ctx);
    }

    case BComplex32: case Complex32: case Complex64: case Complex128: {
        if (!ndt_is_complex(u)) {
            return unification_error("different types", ctx);
        }

        return unify_primitive(t, u, ctx);
    }

    case String: {
        if (u->tag != String) {
            return unification_error("different types", ctx);
        }

        w = ndt_string(opt, ctx);
        if (w == NULL) {
            return NULL;
        }

        return unify_common((ndt_t *)w, t, u, ctx);
    }

    case AnyKind:
    case Module: case Function:
    case SymbolicDim: case EllipsisDim:
    case ScalarKind:
    case SignedKind: case UnsignedKind:
    case FloatKind: case ComplexKind:
    case FixedStringKind: case FixedBytesKind:
    case Typevar:
        return unification_error("abstract type", ctx);
    }

    ndt_internal_error("invalid type tag");
}

const ndt_t *
ndt_unify(const ndt_t *t, const ndt_t *u, ndt_context_t *ctx)
{
    return unify(t, u, false, ctx);
}

const ndt_t *
ndt_unify_replace_any(const ndt_t *t, const ndt_t *u, ndt_context_t *ctx)
{
    return unify(t, u, true, ctx);
}
