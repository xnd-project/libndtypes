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


static int64_t write_type(char * const ptr, int64_t offset, const ndt_t * const t, bool *overflow);

typedef double float64_t;
typedef bool bool_t;


/*****************************************************************************/
/*                       Write values to the bytes buffer                    */
/*****************************************************************************/

#define WRITE(type) \
static inline int64_t                                                \
write_##type(char * const ptr, int64_t offset, const type##_t value, \
             bool *overflow)                                         \
{                                                                    \
    const size_t size = sizeof(type##_t);                            \
                                                                     \
    if (ptr != NULL) {                                               \
        memcpy(ptr+offset, &value, size);                            \
    }                                                                \
                                                                     \
    return ADDi64(offset, size, overflow);                           \
}

#define WRITE_ARRAY(type) \
static inline int64_t                                                     \
write_##type##_array(char * const ptr, int64_t offset, const type##_t *v, \
                     const int64_t nmemb, bool *overflow)                 \
{                                                                         \
    size_t size = MULi64_size(nmemb, sizeof(type##_t), overflow);         \
                                                                          \
    if (ptr != NULL) {                                                    \
        memcpy(ptr+offset, v, size);                                      \
    }                                                                     \
                                                                          \
    return ADDi64(offset, size, overflow);                                \
}

WRITE(bool)
WRITE(uint8)
WRITE(uint16)
WRITE(uint32)
WRITE(int32)
WRITE(int64)
WRITE(float64)
WRITE_ARRAY(uint16)
WRITE_ARRAY(int32)
WRITE_ARRAY(int64)
WRITE_ARRAY(ndt_slice)


static inline int64_t
alloc_int64_array(const int64_t offset, const int64_t shape, bool *overflow)
{
    int64_t size = MULi64(shape, sizeof(int64_t), overflow);
    return ADDi64(offset, size, overflow);
}

static inline int64_t
write_string(char * const ptr, int64_t offset, char *src, bool *overflow)
{
    size_t size;

    if (ptr != NULL) {
        strcpy(ptr+offset, src);
    }

    size = strlen(src) + 1;
    return ADDi64(offset, (int64_t)size, overflow);
}

static inline int64_t
write_string_array(char * const ptr, int64_t offset, char *s[],
                   const int64_t shape, bool *overflow)
{
    for (int64_t i = 0; i < shape; i++) {
        offset = write_string(ptr, offset, s[i], overflow);
    }

    return offset;
}

static inline int64_t
write_ndt_value_array(char * const ptr, int64_t offset, const ndt_value_t *v,
                      const int64_t nmemb, bool *overflow)
{
    for (int64_t i = 0; i < nmemb; i++) {
        offset = write_uint8(ptr, offset, (uint8_t)v[i].tag, overflow);

        switch (v[i].tag) {
        case ValNA:
            break;
        case ValBool:
            offset = write_bool(ptr, offset, v[i].ValBool, overflow);
            break;
        case ValInt64:
            offset = write_int64(ptr, offset, v[i].ValInt64, overflow);
            break;
        case ValFloat64:
            offset = write_float64(ptr, offset, v[i].ValFloat64, overflow);
            break;
        case ValString:
            offset = write_string(ptr, offset, v[i].ValString, overflow);
            break;
        }
    }

    return offset;
}


/******************************************************************************/
/*                             Serialize types                                */ 
/******************************************************************************/

static int64_t
write_common_fields(char *ptr, int64_t offset, const ndt_t * const t,
                    bool *overflow)
{
    offset = write_uint8(ptr, offset, (uint8_t)t->tag, overflow);
    offset = write_uint8(ptr, offset, (uint8_t)t->access, overflow);
    offset = write_uint32(ptr, offset, t->flags, overflow);
    offset = write_int32(ptr, offset, t->ndim, overflow);
    offset = write_int64(ptr, offset, t->datasize, overflow);
    return write_uint16(ptr, offset, t->align, overflow);
}

static int64_t
write_module(char * const ptr, int64_t offset, const ndt_t * const t,
             bool *overflow)
{
    offset = write_string(ptr, offset, t->Module.name, overflow);
    return write_type(ptr, offset, t->Module.type, overflow);
}

static int64_t
write_function(char * const ptr, int64_t offset, const ndt_t * const t,
               bool *overflow)
{
    const int64_t nargs = t->Function.nargs;
    int64_t metaoffset;

    offset = write_int64(ptr, offset, t->Function.nin, overflow);
    offset = write_int64(ptr, offset, t->Function.nout, overflow);
    offset = write_int64(ptr, offset, nargs, overflow);

    metaoffset = offset;
    offset = alloc_int64_array(offset, nargs, overflow);

    for (int64_t i = 0; i < nargs; i++) {
        metaoffset = write_int64(ptr, metaoffset, offset, overflow); 
        offset = write_type(ptr, offset, t->Function.types[i], overflow);
    }

    return offset;
}

static int64_t
write_fixed_dim(char * const ptr, int64_t offset, const ndt_t * const t,
                bool *overflow)
{
    offset = write_uint8(ptr, offset, (uint8_t)t->FixedDim.tag, overflow);
    offset = write_int64(ptr, offset, t->FixedDim.shape, overflow);
    offset = write_int64(ptr, offset, t->Concrete.FixedDim.step, overflow);
    offset = write_int64(ptr, offset, t->Concrete.FixedDim.itemsize, overflow);
    return write_type(ptr, offset, t->FixedDim.type, overflow);
}

static int64_t
write_symbolic_dim(char * const ptr, int64_t offset, const ndt_t * const t,
                   bool *overflow)
{
    offset = write_uint8(ptr, offset, (uint8_t)t->SymbolicDim.tag, overflow);
    offset = write_string(ptr, offset, t->SymbolicDim.name, overflow);
    return write_type(ptr, offset, t->SymbolicDim.type, overflow);
}

static int64_t
write_ellipsis_dim(char * const ptr, int64_t offset, const ndt_t * const t,
                   bool *overflow)
{
    char *cp = t->EllipsisDim.name ? t->EllipsisDim.name : "";
    offset = write_uint8(ptr, offset, (uint8_t)t->EllipsisDim.tag, overflow);
    offset = write_string(ptr, offset, cp, overflow);
    return write_type(ptr, offset, t->EllipsisDim.type, overflow);
}

static int64_t
write_var_dim(char * const ptr, int64_t offset, const ndt_t * const t,
              bool *overflow)
{
    const int32_t noffsets = t->Concrete.VarDim.noffsets;
    const int32_t nslices = t->Concrete.VarDim.nslices;

    offset = write_int64(ptr, offset, t->Concrete.VarDim.itemsize, overflow);
    offset = write_int32(ptr, offset, noffsets, overflow);
    offset = write_int32(ptr, offset, t->Concrete.VarDim.nslices, overflow);
    offset = write_int32_array(ptr, offset, t->Concrete.VarDim.offsets, noffsets, overflow);
    offset = write_ndt_slice_array(ptr, offset, t->Concrete.VarDim.slices, nslices, overflow);
    return write_type(ptr, offset, t->VarDim.type, overflow);
}

static int64_t
write_tuple(char * const ptr, int64_t offset, const ndt_t * const t,
            bool *overflow)
{
    const int64_t shape = t->Tuple.shape;
    int64_t metaoffset;

    offset = write_uint8(ptr, offset, (uint8_t)t->Tuple.flag, overflow);
    offset = write_int64(ptr, offset, shape, overflow);
    offset = write_int64_array(ptr, offset, t->Concrete.Tuple.offset, shape, overflow);
    offset = write_uint16_array(ptr, offset, t->Concrete.Tuple.align, shape, overflow);
    offset = write_uint16_array(ptr, offset, t->Concrete.Tuple.pad, shape, overflow);

    metaoffset = offset;
    offset = alloc_int64_array(offset, shape, overflow);

    for (int64_t i = 0; i < shape; i++) {
        metaoffset = write_int64(ptr, metaoffset, offset, overflow); 
        offset = write_type(ptr, offset, t->Tuple.types[i], overflow);
    }

    return offset;
}

static int64_t
write_record(char * const ptr, int64_t offset, const ndt_t * const t,
             bool *overflow)
{
    const int64_t shape = t->Record.shape;
    int64_t metaoffset;

    offset = write_uint8(ptr, offset, (uint8_t)t->Record.flag, overflow);
    offset = write_int64(ptr, offset, shape, overflow);
    offset = write_int64_array(ptr, offset, t->Concrete.Record.offset, shape, overflow);
    offset = write_uint16_array(ptr, offset, t->Concrete.Record.align, shape, overflow);
    offset = write_uint16_array(ptr, offset, t->Concrete.Tuple.pad, shape, overflow);
    offset = write_string_array(ptr, offset, t->Record.names, shape, overflow);

    metaoffset = offset;
    offset = alloc_int64_array(offset, shape, overflow);

    for (int64_t i = 0; i < shape; i++) {
        metaoffset = write_int64(ptr, metaoffset, offset, overflow); 
        offset = write_type(ptr, offset, t->Record.types[i], overflow);
    }

    return offset;
}

static int64_t
write_ref(char * const ptr, int64_t offset, const ndt_t * const t,
          bool *overflow)
{
    return write_type(ptr, offset, t->Ref.type, overflow);
}

static int64_t
write_constr(char * const ptr, int64_t offset, const ndt_t * const t,
             bool *overflow)
{
    offset = write_string(ptr, offset, t->Constr.name, overflow);
    return write_type(ptr, offset, t->Constr.type, overflow);
}

static int64_t
write_nominal(char * const ptr, int64_t offset, const ndt_t * const t,
              bool *overflow)
{
    offset = write_string(ptr, offset, t->Nominal.name, overflow);
    return write_type(ptr, offset, t->Nominal.type, overflow);
    /* The constraint function pointer is deliberately omitted. It must be
     * looked up and restored during deserialization. */
}

static int64_t
write_categorical(char * const ptr, int64_t offset, const ndt_t * const t,
                  bool *overflow)
{
    const int64_t ntypes = t->Categorical.ntypes;

    offset = write_int64(ptr, offset, ntypes, overflow);
    return write_ndt_value_array(ptr, offset, t->Categorical.types, ntypes, overflow);
}

static int64_t
write_fixed_string(char * const ptr, int64_t offset, const ndt_t * const t,
                   bool *overflow)
{
    offset = write_int64(ptr, offset, t->FixedString.size, overflow);
    return write_uint8(ptr, offset, (uint8_t)t->FixedString.encoding, overflow);
}

static int64_t
write_fixed_bytes(char * const ptr, int64_t offset, const ndt_t * const t,
                  bool *overflow)
{
    offset = write_int64(ptr, offset, t->FixedBytes.size, overflow);
    return write_uint16(ptr, offset, t->FixedBytes.align, overflow);
}

static int64_t
write_bytes(char * const ptr, int64_t offset, const ndt_t * const t,
            bool *overflow)
{
    return write_uint16(ptr, offset, t->Bytes.target_align, overflow);
}

static int64_t
write_char(char * const ptr, int64_t offset, const ndt_t * const t,
           bool *overflow)
{
    return write_uint8(ptr, offset, (uint8_t)t->Char.encoding, overflow);
}

static int64_t
write_typevar(char * const ptr, int64_t offset, const ndt_t * const t,
              bool *overflow)
{
    return write_string(ptr, offset, t->Typevar.name, overflow);
}

static int64_t
write_type(char * const ptr, int64_t offset, const ndt_t * const t,
           bool *overflow)
{
    offset = write_common_fields(ptr, offset, t, overflow);

    switch (t->tag) {
    case Module: return write_module(ptr, offset, t, overflow);
    case Function: return write_function(ptr, offset, t, overflow);
    case FixedDim: return write_fixed_dim(ptr, offset, t, overflow);
    case SymbolicDim: return write_symbolic_dim(ptr, offset, t, overflow);
    case EllipsisDim: return write_ellipsis_dim(ptr, offset, t, overflow);
    case VarDim: return write_var_dim(ptr, offset, t, overflow);
    case Tuple: return write_tuple(ptr, offset, t, overflow);
    case Record: return write_record(ptr, offset, t, overflow);
    case Ref: return write_ref(ptr, offset, t, overflow);
    case Constr: return write_constr(ptr, offset, t, overflow);
    case Nominal: return write_nominal(ptr, offset, t, overflow);
    case Categorical: return write_categorical(ptr, offset, t, overflow);
    case FixedString: return write_fixed_string(ptr, offset, t, overflow);
    case FixedBytes: return write_fixed_bytes(ptr, offset, t, overflow);
    case Bytes: return write_bytes(ptr, offset, t, overflow);
    case Char: return write_char(ptr, offset, t, overflow);
    case Typevar: return write_typevar(ptr, offset, t, overflow);

    case Bool:
    case Int8: case Int16: case Int32: case Int64:
    case Uint8: case Uint16: case Uint32: case Uint64:
    case Float16: case Float32: case Float64:
    case Complex32: case Complex64: case Complex128:
    case String:

    case AnyKind:
    case ScalarKind: case SignedKind: case UnsignedKind: case FloatKind:
    case ComplexKind: case FixedStringKind: case FixedBytesKind:
        return offset;
    }

   /* NOT REACHED: tags should be exhaustive. */
    ndt_internal_error("invalid tag");
}

int64_t
ndt_serialize(char **dest, const ndt_t * const t, ndt_context_t *ctx)
{
    bool overflow = 0;
    int64_t len;
    char *bytes;

    *dest = NULL;

    len = write_type(NULL, 0, t, &overflow);
    if (overflow) {
        ndt_err_format(ctx, NDT_ValueError,
            "overflow during type serialization");
        return -1;
    }

    bytes = ndt_alloc(len, 1);
    if (bytes == NULL) {
        (void)ndt_memory_error(ctx);
        return -1;
    }

    overflow = 0;
    int64_t n = write_type(bytes, 0, t, &overflow);
    if (overflow || n != len) {
        ndt_err_format(ctx, NDT_RuntimeError,
            "unexpected overflow or different length in second pass "
            "of serialization");
        ndt_free(bytes);
        return -1;
    }

    *dest = bytes;
    return len;
}
