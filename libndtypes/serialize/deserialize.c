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


#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include "ndtypes.h"
#include "overflow.h"


static ndt_t *read_type(ndt_meta_t *m, const char * const ptr, int64_t offset,
                        const int64_t len, ndt_context_t *ctx);



typedef double float64_t;
typedef bool bool_t;

typedef enum ndt ndt_tag;
typedef enum ndt_access ndt_access;
typedef enum ndt_variadic ndt_variadic;
typedef enum ndt_encoding ndt_encoding;
typedef enum ndt_value ndt_value;
typedef enum ndt_contig ndt_contig;

typedef struct {
    enum ndt tag;
    enum ndt_access access;
    uint32_t flags;
    int ndim;
    int64_t datasize;
    uint16_t align;
} common_t;


/*****************************************************************************/
/*                     Read values from the bytes buffer                     */
/*****************************************************************************/

static inline int64_t
next_offset(const int64_t offset, const size_t size, const int64_t len,
            ndt_context_t *ctx)
{
    bool overflow = 0;
    int64_t next;

    next = ADDi64(offset, (int64_t)size, &overflow);
    if (overflow || next > len) {
        ndt_err_format(ctx, NDT_ValueError,
            "buffer overflow in type deserialization");
        return -1;
    }

    return next;
}

static inline size_t
array_size(const int64_t nmemb, const size_t size, ndt_context_t *ctx)
{
    bool overflow = 0;
    size_t array_size;

    array_size = MULi64_size(nmemb, (int64_t)size, &overflow);
    if (nmemb < 0 || overflow) {
        ndt_err_format(ctx, NDT_ValueError,
            "corrupted data or buffer overflow in type deserialization");
        return SIZE_MAX;
    }

    return array_size;
}

static inline int64_t
string_size(const char * const ptr, const int64_t offset, const int64_t len,
            ndt_context_t *ctx)
{
    const int64_t n = len-offset;
    const char *end = memchr(ptr+offset, '\0', (size_t)n);

    if (end == NULL) {
        ndt_err_format(ctx, NDT_ValueError,
            "buffer overflow in type deserialization");
        return -1;
    }

    return end-(ptr+offset)+1;
}

#define READ(type) \
static inline int64_t                                         \
read_##type(type##_t * const value, const char * const ptr,   \
            const int64_t offset, const int64_t len,          \
            ndt_context_t *ctx )                              \
{                                                             \
    const size_t size = sizeof(type##_t);                     \
    const int64_t next = next_offset(offset, size, len, ctx); \
    if (next < 0) {                                           \
        return -1;                                            \
    }                                                         \
                                                              \
    memcpy(value, ptr+offset, size);                          \
    return next;                                              \
}

#define READ_POS(type) \
static inline int64_t                                                        \
read_pos_##type(type##_t * const value, const char * const ptr,              \
                const int64_t offset, const int64_t len,                     \
                ndt_context_t *ctx)                                          \
{                                                                            \
    const size_t size = sizeof(type##_t);                                    \
    const int64_t next = next_offset(offset, size, len, ctx);                \
    if (next < 0) {                                                          \
        return -1;                                                           \
    }                                                                        \
                                                                             \
    memcpy(value, ptr+offset, size);                                         \
    if (*value < 0) {                                                        \
        ndt_err_format(ctx, NDT_ValueError,                                  \
            "unexpected negative value in deserialization (corrupt data?)"); \
        return -1;                                                           \
    }                                                                        \
    return next;                                                             \
}

#define READ_CAST(dtype, stype) \
static inline int64_t                                             \
dtype##_from_##stype(dtype * const value, const char * const ptr, \
                     const int64_t offset, const int64_t len,     \
                     ndt_context_t *ctx)                          \
{                                                                 \
    stype##_t x;                                                  \
    const size_t size = sizeof x;                                 \
    const int64_t next = next_offset(offset, size, len, ctx);     \
    if (next < 0) {                                               \
        return -1;                                                \
    }                                                             \
                                                                  \
    memcpy(&x, ptr+offset, size);                                 \
    *value = (dtype)x;                                            \
    return next;                                                  \
}

#define READ_ARRAY(type) \
static inline int64_t                                               \
read_##type##_array(type##_t * const v, const int64_t nmemb,        \
                    const char * const ptr, const int64_t offset,   \
                    const int64_t len, ndt_context_t *ctx)          \
{                                                                   \
    const size_t size = array_size(nmemb, sizeof(type##_t), ctx);   \
    if (size == SIZE_MAX) {                                         \
        return -1;                                                  \
    }                                                               \
                                                                    \
    const int64_t next = next_offset(offset, size, len, ctx);       \
    if (next < 0) {                                                 \
        return -1;                                                  \
    }                                                               \
                                                                    \
    memcpy(v, ptr+offset, size);                                    \
    return next;                                                    \
}

READ_CAST(ndt_tag, uint8)
READ_CAST(ndt_access, uint8)
READ_CAST(ndt_variadic, uint8)
READ_CAST(ndt_encoding, uint8)
READ_CAST(ndt_value, uint8)
READ_CAST(bool, uint8)
READ_CAST(ndt_contig, uint8)

READ_POS(int64)
READ_POS(int32)

READ(uint16)
READ(uint32)
READ(int64)
READ(float64)
READ_ARRAY(uint16)
READ_ARRAY(int32)
READ_ARRAY(int64)
READ_ARRAY(ndt_slice)

static inline int64_t
next_metaoffset(int64_t *offset, const char * const ptr,
                int64_t metaoffset, const int64_t len,
                ndt_context_t *ctx)
{
    metaoffset = read_pos_int64(offset, ptr, metaoffset, len, ctx);
    if (metaoffset < 0 || *offset >= len) {
        ndt_err_format(ctx, NDT_ValueError,
            "corrupt data or buffer overflow in type deserialization");
        return -1;
    }

    return metaoffset;
}


/*****************************************************************************/
/*                              Deserialize types                            */
/*****************************************************************************/

static void
copy_common(ndt_t *t, const common_t *f)
{
    t->tag = f->tag;
    t->access = f->access;
    t->flags = f->flags;
    t->ndim = f->ndim;
    t->datasize = f->datasize;
    t->align = f->align;
}

static ndt_t *
new_copy_common(const common_t *f, ndt_context_t *ctx)
{
    ndt_t *t = ndt_new(f->tag, ctx);
    if (t == NULL) {
        return NULL;
    }

    copy_common(t, f);
    return t;
}

static int64_t
read_string(char **dest, const char * const ptr, const int64_t offset,
            const int64_t len, ndt_context_t *ctx)
{
    const int64_t size = string_size(ptr, offset, len, ctx);
    if (size < 0) {
        return -1;
    }

    *dest = ndt_strdup(ptr+offset, ctx);
    if (*dest == NULL) {
        return -1;
    }

    return offset + size;
}

static int64_t
read_string_array(char *s[], int64_t shape, const char * const ptr,
                  int64_t offset, const int64_t len,
                  ndt_context_t *ctx)
{
    for (int64_t i = 0; i < shape; i++) {
        offset = read_string(&s[i], ptr, offset, len, ctx);
        if (offset < 0) {
            for (int64_t k = 0; k < i; k++) {
                ndt_free(s[k]);
                s[k] = NULL;
            }
            return -1;
        }
    }

    return offset;
}

static inline int64_t
read_ndt_value_array(ndt_value_t *v, const int64_t nmemb,
                     const char * const ptr,
                     int64_t offset, const int64_t len,
                     ndt_context_t *ctx)
{
    for (int64_t i = 0; i < nmemb; i++) {
        offset = ndt_value_from_uint8(&v[i].tag, ptr, offset, len, ctx);
        if (offset < 0) return -1;

        switch (v[i].tag) {
        case ValNA:
            break;
        case ValBool:
            offset = bool_from_uint8(&v[i].ValBool, ptr, offset, len, ctx);
            if (offset < 0) return -1;
            break;
        case ValInt64:
            offset = read_int64(&v[i].ValInt64, ptr, offset, len, ctx);
            if (offset < 0) return -1;
            break;
        case ValFloat64:
            offset = read_float64(&v[i].ValFloat64, ptr, offset, len, ctx);
            if (offset < 0) return -1;
            break;
        case ValString:
            offset = read_string(&v[i].ValString, ptr, offset, len, ctx);
            if (offset < 0) return -1;
            break;
        }
    }

    return offset;
}

static int64_t
read_common_fields(common_t *fields, const char * const ptr,
                   int64_t offset, const int64_t len,
                   ndt_context_t *ctx)
{
    offset = ndt_tag_from_uint8(&fields->tag, ptr, offset, len, ctx);
    if (offset < 0) return -1;

    offset = ndt_access_from_uint8(&fields->access, ptr, offset, len, ctx);
    if (offset < 0) return -1;

    offset = read_uint32(&fields->flags, ptr, offset, len, ctx);
    if (offset < 0) return -1;

    offset = read_pos_int32(&fields->ndim, ptr, offset, len, ctx);
    if (offset < 0) return -1;

    offset = read_pos_int64(&fields->datasize, ptr, offset, len, ctx);
    if (offset < 0) return -1;

    return read_uint16(&fields->align, ptr, offset, len, ctx);
}

static ndt_t *
read_module(ndt_meta_t *m, const common_t *fields, const char * const ptr,
            int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    char *name;
    ndt_t *type;
    ndt_t *t;

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) {
        return NULL;
    }

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        ndt_free(name);
        return NULL;
    }
    t->Module.name = name;
    t->Module.type = type;

    return t;
}

static ndt_t *
read_function(ndt_meta_t *m, common_t *fields, const char * const ptr,
              int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    int64_t metaoffset;
    int64_t nin;
    int64_t nout;
    int64_t nargs;
    ndt_t *t;

    offset = read_pos_int64(&nin, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&nout, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&nargs, ptr, offset, len, ctx);
    if (offset < 0) return NULL;


    t = ndt_function_new(nargs, ctx);
    if (t == NULL) {
        return NULL;
    }
    copy_common(t, fields);

    metaoffset = offset;
    for (int64_t i = 0; i < nargs; i++) {
        metaoffset = next_metaoffset(&offset, ptr, metaoffset, len, ctx);
        if (metaoffset < 0) return NULL;

        t->Function.types[i] = read_type(m, ptr, offset, len, ctx);
        if (t->Function.types[i] == NULL) {
            ndt_del(t);
            return NULL;
        }
    }

    t->Function.nin = nin;
    t->Function.nout = nout;

    return t;
}

static ndt_t *
read_fixed_dim(ndt_meta_t *m, const common_t *fields, const char * const ptr,
               int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    ndt_contig tag;
    int64_t shape;
    int64_t step;
    int64_t itemsize;
    ndt_t *type;
    ndt_t *t;

    offset = ndt_contig_from_uint8(&tag, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&shape, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_int64(&step, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&itemsize, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->FixedDim.tag = tag;
    t->FixedDim.shape = shape;
    t->FixedDim.type = type;
    t->Concrete.FixedDim.step = step;
    t->Concrete.FixedDim.itemsize = itemsize;

    return t;
}

static ndt_t *
read_symbolic_dim(ndt_meta_t *m, const common_t *fields, const char * const ptr,
                  int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    ndt_contig tag;
    char *name;
    ndt_t *type;
    ndt_t *t;

    offset = ndt_contig_from_uint8(&tag, ptr, offset, len, ctx);
    if (offset < 0) {
        return NULL;
    }

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) {
        return NULL;
    }

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        ndt_free(name);
        return NULL;
    }
    t->SymbolicDim.tag = tag;
    t->SymbolicDim.name = name;
    t->SymbolicDim.type = type;

    return t;
}

static ndt_t *
read_ellipsis_dim(ndt_meta_t *m, const common_t *fields, const char * const ptr,
                  int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    ndt_contig tag;
    char *name;
    ndt_t *type;
    ndt_t *t;

    offset = ndt_contig_from_uint8(&tag, ptr, offset, len, ctx);
    if (offset < 0) {
        return NULL;
    }

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) {
        return NULL;
    }
    if (name[0] == '\0') {
        ndt_free(name);
        name = NULL;
    }

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        ndt_free(name);
        return NULL;
    }
    t->EllipsisDim.tag = tag;
    t->EllipsisDim.name = name;
    t->EllipsisDim.type = type;

    return t;
}

static ndt_t *
read_var_dim(ndt_meta_t *m, const common_t *fields, const char * const ptr,
             int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    int64_t itemsize;
    int32_t noffsets;
    int32_t *offsets;
    int32_t nslices = 0;
    ndt_slice_t *slices = NULL;
    ndt_t *type;
    ndt_t *t;

    offset = read_pos_int64(&itemsize, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int32(&noffsets, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int32(&nslices, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offsets = ndt_alloc(noffsets, sizeof *offsets);
    if (offsets == NULL) {
        return ndt_memory_error(ctx);
    }

    offset = read_int32_array(offsets, noffsets, ptr, offset, len, ctx);
    if (offset < 0) {
        ndt_free(offsets);
        return NULL;
    }

    slices = ndt_alloc(nslices, sizeof *slices);
    if (slices == NULL) {
        ndt_free(offsets);
        return ndt_memory_error(ctx);
    }

    offset = read_ndt_slice_array(slices, nslices, ptr, offset, len, ctx);
    if (offset < 0) {
        ndt_free(offsets);
        ndt_free(slices);
        return NULL;
    }

    if (nslices == 0) {
        ndt_free(slices);
        slices = NULL;
    }

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(offsets);
        ndt_free(slices);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_free(offsets);
        ndt_free(slices);
        ndt_del(type);
        return NULL;
    }
    t->VarDim.type = type;
    t->Concrete.VarDim.flag = ExternalOffsets;
    t->Concrete.VarDim.itemsize = itemsize;
    t->Concrete.VarDim.noffsets = noffsets;
    t->Concrete.VarDim.offsets = offsets;
    t->Concrete.VarDim.nslices = nslices;
    t->Concrete.VarDim.slices = slices;

    m->noffsets[m->ndims] = noffsets;
    m->offsets[m->ndims] = offsets;
    m->ndims++;

    return t;
}

static ndt_t *
read_tuple(ndt_meta_t *m, const common_t *fields, const char * const ptr,
           int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    int64_t metaoffset;
    enum ndt_variadic flag;
    int64_t shape;
    ndt_t *t;

    offset = ndt_variadic_from_uint8(&flag, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&shape, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = ndt_tuple_new(flag, shape, ctx);
    if (t == NULL) {
        return NULL;
    }
    copy_common(t, fields);

    offset = read_int64_array(t->Concrete.Tuple.offset, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    offset = read_uint16_array(t->Concrete.Tuple.align, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    offset = read_uint16_array(t->Concrete.Tuple.pad, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    metaoffset = offset;
    for (int64_t i = 0; i < shape; i++) {
        metaoffset = next_metaoffset(&offset, ptr, metaoffset, len, ctx);
        if (metaoffset < 0) goto error;

        t->Tuple.types[i] = read_type(m, ptr, offset, len, ctx);
        if (t->Tuple.types[i] == NULL) {
            goto error;
        }
    }

    return t;

error:
    ndt_del(t);
    return NULL;
}

static ndt_t *
read_record(ndt_meta_t *m, const common_t *fields, const char * const ptr,
            int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    int64_t metaoffset;
    enum ndt_variadic flag;
    int64_t shape;
    ndt_t *t;

    offset = ndt_variadic_from_uint8(&flag, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_pos_int64(&shape, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = ndt_record_new(flag, shape, ctx);
    if (t == NULL) {
        return NULL;
    }
    copy_common(t, fields);

    offset = read_int64_array(t->Concrete.Record.offset, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    offset = read_uint16_array(t->Concrete.Record.align, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    offset = read_uint16_array(t->Concrete.Tuple.pad, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    offset = read_string_array(t->Record.names, shape, ptr, offset, len, ctx);
    if (offset < 0) goto error;

    metaoffset = offset;
    for (int64_t i = 0; i < shape; i++) {
        metaoffset = next_metaoffset(&offset, ptr, metaoffset, len, ctx);
        if (metaoffset < 0) goto error;

        t->Record.types[i] = read_type(m, ptr, offset, len, ctx);
        if (t->Record.types[i] == NULL) {
            goto error;
        }
    }

    return t;

error:
    ndt_del(t);
    return NULL;
}

static ndt_t *
read_ref(ndt_meta_t *m, const common_t *fields, const char * const ptr,
         int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    ndt_t *type;
    ndt_t *t;

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        return NULL;
    }
    t->Ref.type = type;

    return t;
}

static ndt_t *
read_constr(ndt_meta_t *m, const common_t *fields, const char * const ptr,
            int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    char *name;
    ndt_t *type;
    ndt_t *t;

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_del(type);
        ndt_free(name);
        return NULL;
    }
    t->Constr.name = name;
    t->Constr.type = type;

    return t;
}

static ndt_t *
read_nominal(ndt_meta_t *m, const common_t *fields, const char * const ptr,
             int64_t offset, const int64_t len, ndt_context_t *ctx)
{
    char *name;
    ndt_t *type;
    const ndt_typedef_t *d;
    ndt_t *t;

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    type = read_type(m, ptr, offset, len, ctx);
    if (type == NULL) {
        ndt_free(name);
        return NULL;
    }

    d = ndt_typedef_find(name, ctx);
    if (d == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_free(name);
        ndt_del(type);
        return NULL;
    }
    t->Nominal.name = name;
    t->Nominal.type = type;
    t->Nominal.meth = &d->meth;

    return t;
}

static ndt_t *
read_categorical(const common_t *fields, const char * const ptr, int64_t offset,
                 const int64_t len, ndt_context_t *ctx)
{
    int64_t ntypes;
    ndt_value_t *types;
    ndt_t *t;

    offset = read_pos_int64(&ntypes, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    types = ndt_calloc(ntypes, sizeof *types);
    if (types == NULL) {
        return ndt_memory_error(ctx);
    }

    offset = read_ndt_value_array(types, ntypes, ptr, offset, len, ctx);
    if (offset < 0) {
        ndt_value_array_del(types, ntypes);
        return NULL;
    }

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_value_array_del(types, ntypes);
        return NULL;
    }
    t->Categorical.ntypes = ntypes;
    t->Categorical.types = types;

    return t;
}

static ndt_t *
read_fixed_string(const common_t *fields, const char * const ptr, int64_t offset,
                  const int64_t len, ndt_context_t *ctx)
{
    int64_t size;
    enum ndt_encoding encoding;
    ndt_t *t;

    offset = read_pos_int64(&size, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = ndt_encoding_from_uint8(&encoding, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedString.size = size;
    t->FixedString.encoding = encoding;

    return t;
}

static ndt_t *
read_fixed_bytes(const common_t *fields, const char * const ptr, int64_t offset,
                 const int64_t len, ndt_context_t *ctx)
{
    int64_t size;
    uint16_t align;
    ndt_t *t;

    offset = read_pos_int64(&size, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    offset = read_uint16(&align, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->FixedBytes.size = size;
    t->FixedBytes.align = align;

    return t;
}

static ndt_t *
read_bytes(const common_t *fields, const char * const ptr, int64_t offset,
           const int64_t len, ndt_context_t *ctx)
{
    uint16_t align;
    ndt_t *t;

    offset = read_uint16(&align, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Bytes.target_align = align;

    return t;
}

static ndt_t *
read_char(const common_t *fields, const char * const ptr, int64_t offset,
          const int64_t len, ndt_context_t *ctx)
{
    enum ndt_encoding encoding;
    ndt_t *t;

    offset = ndt_encoding_from_uint8(&encoding, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        return NULL;
    }
    t->Char.encoding = encoding;

    return t;
}

static ndt_t *
read_typevar(const common_t *fields, const char * const ptr, int64_t offset,
             const int64_t len, ndt_context_t *ctx)
{
    char *name;
    ndt_t *t;

    offset = read_string(&name, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    t = new_copy_common(fields, ctx);
    if (t == NULL) {
        ndt_free(name);
        return NULL;
    }
    t->Typevar.name = name;

    return t;
}

static ndt_t *
read_type(ndt_meta_t *m, const char * const ptr, int64_t offset, const int64_t len,
          ndt_context_t *ctx)
{
    common_t fields;

    offset = read_common_fields(&fields, ptr, offset, len, ctx);
    if (offset < 0) return NULL;

    switch (fields.tag) {
    case Module: return read_module(m, &fields, ptr, offset, len, ctx);
    case Function: return read_function(m, &fields, ptr, offset, len, ctx);
    case FixedDim: return read_fixed_dim(m, &fields, ptr, offset, len, ctx);
    case SymbolicDim: return read_symbolic_dim(m, &fields, ptr, offset, len, ctx);
    case EllipsisDim: return read_ellipsis_dim(m, &fields, ptr, offset, len, ctx);
    case VarDim: return read_var_dim(m, &fields, ptr, offset, len, ctx);
    case Tuple: return read_tuple(m, &fields, ptr, offset, len, ctx);
    case Record: return read_record(m, &fields, ptr, offset, len, ctx);
    case Ref: return read_ref(m, &fields, ptr, offset, len, ctx);
    case Constr: return read_constr(m, &fields, ptr, offset, len, ctx);
    case Nominal: return read_nominal(m, &fields, ptr, offset, len, ctx);
    case Categorical: return read_categorical(&fields, ptr, offset, len, ctx);
    case FixedString: return read_fixed_string(&fields, ptr, offset, len, ctx);
    case FixedBytes: return read_fixed_bytes(&fields, ptr, offset, len, ctx);
    case Bytes: return read_bytes(&fields, ptr, offset, len, ctx);
    case Char: return read_char(&fields, ptr, offset, len, ctx);
    case Typevar: return read_typevar(&fields, ptr, offset, len, ctx);

    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case String:

    case AnyKind:
    case ScalarKind: case SignedKind: case UnsignedKind: case FloatKind:
    case ComplexKind: case FixedStringKind: case FixedBytesKind:
        return new_copy_common(&fields, ctx);
    }

    ndt_err_format(ctx, NDT_RuntimeError,
        "found invalid type tag in deserialization (corrupted data?)");
    return NULL;
}

ndt_t *
ndt_deserialize(ndt_meta_t *m, const char * const ptr, int64_t len,
                ndt_context_t *ctx)
{
    return read_type(m, ptr, 0, len, ctx);
}
