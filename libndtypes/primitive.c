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


#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <complex.h>
#include <assert.h>
#include "ndtypes.h"


/******************************************************************************/
/*                               Primitive types                              */
/******************************************************************************/

typedef bool bool_t;
#undef bool

#define NDT_PRIMITIVE(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name = {                                 \
  .tag = _tag,                                                    \
  .access = _access,                                              \
  .flags = _flags,                                                \
  .ndim = 0,                                                      \
  .datasize = _size,                                              \
  .align = _align,                                                \
  .refcnt = 1                                                     \
};

#define NDT_PRIMITIVE_LE(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name##_le = {                               \
  .tag = _tag,                                                       \
  .access = _access,                                                 \
  .flags = _flags|NDT_LITTLE_ENDIAN,                                 \
  .ndim = 0,                                                         \
  .datasize = _size,                                                 \
  .align = _align,                                                   \
  .refcnt = 1                                                        \
};

#define NDT_PRIMITIVE_BE(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name##_be = {                               \
  .tag = _tag,                                                       \
  .access = _access,                                                 \
  .flags = _flags|NDT_BIG_ENDIAN,                                    \
  .ndim = 0,                                                         \
  .datasize = _size,                                                 \
  .align = _align,                                                   \
  .refcnt = 1                                                        \
};

#define NDT_PRIMITIVE_OPT(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name##_opt = {                               \
  .tag = _tag,                                                        \
  .access = _access,                                                  \
  .flags = _flags|NDT_OPTION,                                         \
  .ndim = 0,                                                          \
  .datasize = _size,                                                  \
  .align = _align,                                                    \
  .refcnt = 1                                                         \
};

#define NDT_PRIMITIVE_OPT_LE(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name##_opt_le = {                               \
  .tag = _tag,                                                           \
  .access = _access,                                                     \
  .flags = _flags|NDT_OPTION|NDT_LITTLE_ENDIAN,                          \
  .ndim = 0,                                                             \
  .datasize = _size,                                                     \
  .align = _align,                                                       \
  .refcnt = 1                                                            \
};

#define NDT_PRIMITIVE_OPT_BE(name, _tag, _access, _flags, _size, _align) \
static const ndt_t ndt_##name##_opt_be = {                       \
  .tag = _tag,                                                   \
  .access = _access,                                             \
  .flags = _flags|NDT_OPTION|NDT_BIG_ENDIAN,                     \
  .ndim = 0,                                                     \
  .datasize = _size,                                             \
  .align = _align,                                               \
  .refcnt = 1                                                    \
};

#define NDT_PRIMITIVE_ALL(name, _tag, _size, _align) \
   NDT_PRIMITIVE(name, _tag, Concrete, 0, _size, _align)        \
   NDT_PRIMITIVE_LE(name, _tag, Concrete, 0, _size, _align)     \
   NDT_PRIMITIVE_BE(name, _tag, Concrete, 0, _size, _align)     \
   NDT_PRIMITIVE_OPT(name, _tag, Concrete, 0, _size, _align)    \
   NDT_PRIMITIVE_OPT_LE(name, _tag, Concrete, 0, _size, _align) \
   NDT_PRIMITIVE_OPT_BE(name, _tag, Concrete, 0, _size, _align)

#define NDT_PRIMITIVE_KIND_ALL(name, _tag) \
   NDT_PRIMITIVE(name, _tag, Abstract, 0, 0, UINT16_MAX)        \
   NDT_PRIMITIVE_LE(name, _tag, Abstract, 0, 0, UINT16_MAX)     \
   NDT_PRIMITIVE_BE(name, _tag, Abstract, 0, 0, UINT16_MAX)     \
   NDT_PRIMITIVE_OPT(name, _tag, Abstract, 0, 0, UINT16_MAX)    \
   NDT_PRIMITIVE_OPT_LE(name, _tag, Abstract, 0, 0, UINT16_MAX) \
   NDT_PRIMITIVE_OPT_BE(name, _tag, Abstract, 0, 0, UINT16_MAX)


NDT_PRIMITIVE_ALL(bool, Bool, sizeof(bool_t), alignof(bool_t))

NDT_PRIMITIVE_KIND_ALL(signed_set, SignedKind)
NDT_PRIMITIVE_ALL(int8, Int8, sizeof(int8_t), alignof(int8_t))
NDT_PRIMITIVE_ALL(int16, Int16, sizeof(int16_t), alignof(int16_t))
NDT_PRIMITIVE_ALL(int32, Int32, sizeof(int32_t), alignof(int32_t))
NDT_PRIMITIVE_ALL(int64, Int64, sizeof(int64_t), alignof(int64_t))

NDT_PRIMITIVE_KIND_ALL(unsigned_set, UnsignedKind)
NDT_PRIMITIVE_ALL(uint8, Uint8, sizeof(uint8_t), alignof(uint8_t))
NDT_PRIMITIVE_ALL(uint16, Uint16, sizeof(uint16_t), alignof(uint16_t))
NDT_PRIMITIVE_ALL(uint32, Uint32, sizeof(uint32_t), alignof(uint32_t))
NDT_PRIMITIVE_ALL(uint64, Uint64, sizeof(uint64_t), alignof(uint64_t))

NDT_PRIMITIVE_KIND_ALL(float_set, FloatKind)
NDT_PRIMITIVE_ALL(bfloat16, BFloat16, 2, 2)
NDT_PRIMITIVE_ALL(float16, Float16, 2, 2)
NDT_PRIMITIVE_ALL(float32, Float32, sizeof(float), alignof(float))
NDT_PRIMITIVE_ALL(float64, Float64, sizeof(double), alignof(double))

NDT_PRIMITIVE_KIND_ALL(complex_set, ComplexKind)
NDT_PRIMITIVE_ALL(bcomplex32, BComplex32, 4, 2)
NDT_PRIMITIVE_ALL(complex32, Complex32, 4, 2)
NDT_PRIMITIVE_ALL(complex64, Complex64, sizeof(ndt_complex64_t), alignof(ndt_complex64_t))
NDT_PRIMITIVE_ALL(complex128, Complex128, sizeof(ndt_complex128_t), alignof(ndt_complex128_t))

NDT_PRIMITIVE(str, String, Concrete, NDT_POINTER, sizeof(char *), alignof(char *))
NDT_PRIMITIVE_OPT(str, String, Concrete, NDT_POINTER, sizeof(char *), alignof(char *))


const ndt_t *
ndt_string(bool_t opt, ndt_context_t *ctx)
{
    (void)ctx;

    return opt ? &ndt_str_opt : &ndt_str;
}

const ndt_t *
ndt_signed_kind(uint32_t flags, ndt_context_t *ctx)
{
    return ndt_primitive(SignedKind, flags, ctx);
}

const ndt_t *
ndt_unsigned_kind(uint32_t flags, ndt_context_t *ctx)
{
    return ndt_primitive(UnsignedKind, flags, ctx);
}

const ndt_t *
ndt_float_kind(uint32_t flags, ndt_context_t *ctx)
{
    return ndt_primitive(FloatKind, flags, ctx);
}

const ndt_t *
ndt_complex_kind(uint32_t flags, ndt_context_t *ctx)
{
    return ndt_primitive(ComplexKind, flags, ctx);
}

const ndt_t *
ndt_primitive(enum ndt tag, uint32_t flags, ndt_context_t *ctx)
{
    switch(flags) {
    case 0: {
        switch(tag) {
        case Bool: return &ndt_bool;

        case SignedKind: return &ndt_signed_set;
        case Int8: return &ndt_int8;
        case Int16: return &ndt_int16;
        case Int32: return &ndt_int32;
        case Int64: return &ndt_int64;

        case UnsignedKind: return &ndt_unsigned_set;
        case Uint8: return &ndt_uint8;
        case Uint16: return &ndt_uint16;
        case Uint32: return &ndt_uint32;
        case Uint64: return &ndt_uint64;

        case FloatKind: return &ndt_float_set;
        case BFloat16: return &ndt_bfloat16;
        case Float16: return &ndt_float16;
        case Float32: return &ndt_float32;
        case Float64: return &ndt_float64;

        case ComplexKind: return &ndt_complex_set;
        case BComplex32: return &ndt_bcomplex32;
        case Complex32: return &ndt_complex32;
        case Complex64: return &ndt_complex64;
        case Complex128: return &ndt_complex128;

        default: goto value_error_tag;
        }
    }

    case NDT_LITTLE_ENDIAN: {
        switch(tag) {
        case Bool: return &ndt_bool_le;

        case SignedKind: return &ndt_signed_set_le;
        case Int8: return &ndt_int8_le;
        case Int16: return &ndt_int16_le;
        case Int32: return &ndt_int32_le;
        case Int64: return &ndt_int64_le;

        case UnsignedKind: return &ndt_unsigned_set_le;
        case Uint8: return &ndt_uint8_le;
        case Uint16: return &ndt_uint16_le;
        case Uint32: return &ndt_uint32_le;
        case Uint64: return &ndt_uint64_le;

        case FloatKind: return &ndt_float_set_le;
        case BFloat16: return &ndt_bfloat16_le;
        case Float16: return &ndt_float16_le;
        case Float32: return &ndt_float32_le;
        case Float64: return &ndt_float64_le;

        case ComplexKind: return &ndt_complex_set_le;
        case BComplex32: return &ndt_bcomplex32_le;
        case Complex32: return &ndt_complex32_le;
        case Complex64: return &ndt_complex64_le;
        case Complex128: return &ndt_complex128_le;

        default: goto value_error_tag;
        }
    }

    case NDT_BIG_ENDIAN: {
       switch(tag) {
        case Bool: return &ndt_bool_be;

        case SignedKind: return &ndt_signed_set_be;
        case Int8: return &ndt_int8_be;
        case Int16: return &ndt_int16_be;
        case Int32: return &ndt_int32_be;
        case Int64: return &ndt_int64_be;

        case UnsignedKind: return &ndt_unsigned_set_be;
        case Uint8: return &ndt_uint8_be;
        case Uint16: return &ndt_uint16_be;
        case Uint32: return &ndt_uint32_be;
        case Uint64: return &ndt_uint64_be;

        case FloatKind: return &ndt_float_set_be;
        case BFloat16: return &ndt_bfloat16_be;
        case Float16: return &ndt_float16_be;
        case Float32: return &ndt_float32_be;
        case Float64: return &ndt_float64_be;

        case ComplexKind: return &ndt_complex_set_be;
        case BComplex32: return &ndt_bcomplex32_be;
        case Complex32: return &ndt_complex32_be;
        case Complex64: return &ndt_complex64_be;
        case Complex128: return &ndt_complex128_be;

        default: goto value_error_tag;
        }
    }

    case NDT_OPTION: {
        switch(tag) {
        case Bool: return &ndt_bool_opt;

        case SignedKind: return &ndt_signed_set_opt;
        case Int8: return &ndt_int8_opt;
        case Int16: return &ndt_int16_opt;
        case Int32: return &ndt_int32_opt;
        case Int64: return &ndt_int64_opt;

        case UnsignedKind: return &ndt_unsigned_set_opt;
        case Uint8: return &ndt_uint8_opt;
        case Uint16: return &ndt_uint16_opt;
        case Uint32: return &ndt_uint32_opt;
        case Uint64: return &ndt_uint64_opt;

        case FloatKind: return &ndt_float_set_opt;
        case BFloat16: return &ndt_bfloat16_opt;
        case Float16: return &ndt_float16_opt;
        case Float32: return &ndt_float32_opt;
        case Float64: return &ndt_float64_opt;

        case ComplexKind: return &ndt_complex_set_opt;
        case BComplex32: return &ndt_bcomplex32_opt;
        case Complex32: return &ndt_complex32_opt;
        case Complex64: return &ndt_complex64_opt;
        case Complex128: return &ndt_complex128_opt;

        default: goto value_error_tag;
        }
    }

    case NDT_OPTION|NDT_LITTLE_ENDIAN: {
        switch(tag) {
        case Bool: return &ndt_bool_opt_le;

        case SignedKind: return &ndt_signed_set_opt_le;
        case Int8: return &ndt_int8_opt_le;
        case Int16: return &ndt_int16_opt_le;
        case Int32: return &ndt_int32_opt_le;
        case Int64: return &ndt_int64_opt_le;

        case UnsignedKind: return &ndt_unsigned_set_opt_le;
        case Uint8: return &ndt_uint8_opt_le;
        case Uint16: return &ndt_uint16_opt_le;
        case Uint32: return &ndt_uint32_opt_le;
        case Uint64: return &ndt_uint64_opt_le;

        case FloatKind: return &ndt_float_set_opt_le;
        case BFloat16: return &ndt_bfloat16_opt_le;
        case Float16: return &ndt_float16_opt_le;
        case Float32: return &ndt_float32_opt_le;
        case Float64: return &ndt_float64_opt_le;

        case ComplexKind: return &ndt_complex_set_opt_le;
        case BComplex32: return &ndt_bcomplex32_opt_le;
        case Complex32: return &ndt_complex32_opt_le;
        case Complex64: return &ndt_complex64_opt_le;
        case Complex128: return &ndt_complex128_opt_le;
        default: goto value_error_tag;
        }
    }

    case NDT_OPTION|NDT_BIG_ENDIAN: {
        switch(tag) {
        case Bool: return &ndt_bool_opt_be;

        case SignedKind: return &ndt_signed_set_opt_be;
        case Int8: return &ndt_int8_opt_be;
        case Int16: return &ndt_int16_opt_be;
        case Int32: return &ndt_int32_opt_be;
        case Int64: return &ndt_int64_opt_be;

        case UnsignedKind: return &ndt_unsigned_set_opt_be;
        case Uint8: return &ndt_uint8_opt_be;
        case Uint16: return &ndt_uint16_opt_be;
        case Uint32: return &ndt_uint32_opt_be;
        case Uint64: return &ndt_uint64_opt_be;

        case FloatKind: return &ndt_float_set_opt_be;
        case BFloat16: return &ndt_bfloat16_opt_be;
        case Float16: return &ndt_float16_opt_be;
        case Float32: return &ndt_float32_opt_be;
        case Float64: return &ndt_float64_opt_be;

        case ComplexKind: return &ndt_complex_set_opt_be;
        case BComplex32: return &ndt_bcomplex32_opt_be;
        case Complex32: return &ndt_complex32_opt_be;
        case Complex64: return &ndt_complex64_opt_be;
        case Complex128: return &ndt_complex128_opt_be;

        default: goto value_error_tag;
        }
    }
 
    case NDT_POINTER: {
        switch(tag) {
        case String: return &ndt_str;
        default: goto value_error_tag;
        }
    }
 
    case NDT_POINTER|NDT_OPTION: {
        switch(tag) {
        case String: return &ndt_str_opt;
        default: goto value_error_tag;
        }
    }

    default:
        goto value_error_flags;
    }

value_error_tag:
    ndt_err_format(ctx, NDT_ValueError, "not a primitive type");
    return NULL;

value_error_flags:
    ndt_err_format(ctx, NDT_ValueError, "invalid type flags");
    return NULL;
}
