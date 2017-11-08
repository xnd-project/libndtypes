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


#ifndef NDTYPES_H
#define NDTYPES_H


#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <complex.h>


#if SIZE_MAX > ULLONG_MAX
  #error "need SIZE_MAX <= ULLONG_MAX"
#endif

#ifdef _MSC_VER
  #if defined (EXPORT)
    #define NDTYPES_API __declspec(dllexport)
  #elif defined(IMPORT)
    #define NDTYPES_API __declspec(dllimport)
  #else
    #define NDTYPES_API
  #endif

  typedef _Dcomplex ndt_complex128_t;
  typedef _Fcomplex ndt_complex64_t;
  #define alignof __alignof
  #define alignas(n) __declspec(align(n))
  #define MAX_ALIGN 8
#else
  #define NDTYPES_API

  #if !defined(__APPLE__) && !defined(__STDC_IEC_559__)
    #error "ndtypes requires IEEE floating point arithmetic"
  #endif
  #include <stdalign.h>
  typedef double complex ndt_complex128_t;
  typedef float complex ndt_complex64_t;
  #define MAX_ALIGN (alignof(max_align_t))
#endif


/*****************************************************************************/
/*                                 Datashape                                 */
/*****************************************************************************/

#define NDT_MAX_DIM 128

#define NDT_ELLIPSIS     0x00000001U
#define NDT_DIM_OPTION   0x00000002U
#define NDT_C_CONTIGUOUS 0x00000004U
#define NDT_F_CONTIGUOUS 0x00000008U
#define NDT_CONTIGUOUS   (NDT_C_CONTIGUOUS|NDT_F_CONTIGUOUS)


/* Types: ndt_t */
typedef struct _ndt ndt_t;

/* Internal option type */
enum ndt_option {
  None,
  Some
};

typedef struct {
  enum ndt_option tag;
  char Some;
} char_opt_t;

typedef struct {
  enum ndt_option tag;
  int64_t Some;
} int64_opt_t;

typedef struct {
  enum ndt_option tag;
  uint16_t Some;
} uint16_opt_t;


enum ndt_attr {
  AttrBool,
  AttrChar,
  AttrInt8,
  AttrInt16,
  AttrInt32,
  AttrInt64,
  AttrUint8,
  AttrUint16,
  AttrUint32,
  AttrUint64,
  AttrSize,
  AttrFloat32,
  AttrFloat64,
  AttrString,
  AttrInt32List,
  AttrCharOpt,
  AttrInt64Opt,
  AttrUint16Opt
};

enum ndt_attr_tag {
  AttrValue,
  AttrList
};

/* Attribute: name=value or name=[value, value, ...]. */
typedef struct {
    enum ndt_attr_tag tag;
    char *name;
    union {
        char *AttrValue;
        struct {
            size_t len;
            char **items;
        } AttrList;
    };
} ndt_attr_t;


/* Flag for variadic tuples and records */
enum ndt_variadic {
  Nonvariadic,
  Variadic
};


/* Ownership flag (experimental) for var dim offsets */
enum ndt_offsets {
  OwnOffsets,
  ExternalOffsets,
  NoOffsets,
};


/* Encoding for characters and strings */
enum ndt_encoding {
  Ascii,
  Utf8,
  Utf16,
  Utf32,
  Ucs2,
  ErrorEncoding
};


/* Datashape kinds */
enum ndt {
  /* Name space */
  Module,

  /* Any */
  AnyKind,
    FixedDim,
    SymbolicDim,
    VarDim,
    EllipsisDim,

    Option,
    OptionItem,
    Nominal,
    Constr,

      /* Dtype */
      Tuple,
      Record,
      Function,
      Typevar,

      /* Scalar */
      ScalarKind,
        Void,
        Bool,

        SignedKind,
          Int8,
          Int16,
          Int32,
          Int64,

        UnsignedKind,
          Uint8,
          Uint16,
          Uint32,
          Uint64,

        FloatKind,
          Float16,
          Float32,
          Float64,

        ComplexKind,
          Complex32,
          Complex64,
          Complex128,

        FixedStringKind,
          FixedString,

        FixedBytesKind,
          FixedBytes,

        Char,
        String,
        Bytes,

        Categorical,
        Pointer,

        Field /* used internally */

        /* User1, User2, ... tags for a limited number of user-defined types */
};

enum ndt_alias {
  Size,
  Intptr,
  Uintptr
};

/* Protect access to concrete type fields. */
enum ndt_access {
  Abstract,
  Concrete
};

/* Tuple or record field, used in the parser. */
typedef struct {
  enum ndt_access access;
  char *name;
  ndt_t *type;
  struct {
      uint16_t data_align;
      bool explicit_align;
  } Concrete;
} ndt_field_t;

/* Selected values for the categorical type. */
enum ndt_value {
  ValBool,
  ValInt64,
  ValFloat64,
  ValString,
  ValNA,
};

typedef struct {
    enum ndt_value tag;
    union {
        bool ValBool;
        int64_t ValInt64;
        double ValFloat64;
        char *ValString;
    };
} ndt_value_t;

/* Datashape type */
struct _ndt {
    /* Always defined */
    enum ndt tag;
    enum ndt_access access;
    int ndim;
    int64_t hash;
    /* Undefined if the type is abstract */
    int64_t data_size;
    uint16_t data_align;

    /* Abstract */
    union {
        struct {
            char *name;
            ndt_t *type;
        } Module;

        struct {
            uint32_t flags;
            int64_t shape;
            ndt_t *type;
        } FixedDim;

        struct {
            uint32_t flags;
            char *name;
            ndt_t *type;
        } SymbolicDim;

        struct {
            uint32_t flags;
            ndt_t *type;
        } VarDim;

        struct {
            uint32_t flags;
            char *name;
            ndt_t *type;
        } EllipsisDim;

        struct {
            ndt_t *type;
        } Option;

        struct {
            ndt_t *type;
        } OptionItem;

        struct {
            char *name;
        } Nominal;

        struct {
            char *name;
            ndt_t *type;
        } Constr;

        struct {
            enum ndt_variadic flag;
            int64_t shape;
            ndt_t **types;
        } Tuple;

        struct {
            enum ndt_variadic flag;
            int64_t shape;
            char **names;
            ndt_t **types;
        } Record;

        struct {
            ndt_t *ret;
            ndt_t *pos;
            ndt_t *kwds;
        } Function;

        struct {
            char *name;
        } Typevar;

        struct {
            enum ndt_encoding encoding;
        } Char;

        struct {
            uint16_t target_align;
        } Bytes;

        struct {
            size_t size;
            enum ndt_encoding encoding;
        } FixedString;

        struct {
            size_t size;
            uint16_t align;
        } FixedBytes;

        struct {
            size_t ntypes;
            ndt_value_t *types;
        } Categorical;

        struct {
            ndt_t *type;
        } Pointer;
    };

    /* Concrete */
    struct {
        union {
            struct {
                int64_t itemsize;
                int64_t stride;
            } FixedDim;

            struct {
                enum ndt_offsets flag;
                int64_t itemsize;
                int32_t start;
                int32_t stop;
                int32_t step;
                int32_t noffsets;
                const int32_t *offsets;
            } VarDim;

            struct {
                int64_t *offset;
                uint16_t *align;
                uint16_t *pad;
            } Tuple;

            struct {
                int64_t *offset;
                uint16_t *align;
                uint16_t *pad;
            } Record;
        };
    } Concrete;

    alignas(MAX_ALIGN) char extra[];
};


/*****************************************************************************/
/*                        Context and  error handling                        */
/*****************************************************************************/

#define NDT_Dynamic 0x00000001U

#define NDT_STATIC_CONTEXT(name) \
    ndt_context_t name = { .flags=0, .err=NDT_Success, .msg=ConstMsg, .ConstMsg="Success" }


enum ndt_error {
  NDT_Success,
  NDT_ValueError,
  NDT_TypeError,
  NDT_InvalidArgumentError,
  NDT_NotImplementedError,
  NDT_LexError,
  NDT_ParseError,
  NDT_OSError,
  NDT_RuntimeError,
  NDT_MemoryError
};

enum ndt_msg {
  ConstMsg,
  DynamicMsg
};

typedef struct {
    uint32_t flags;
    enum ndt_error err;
    enum ndt_msg msg;
    union {
        const char *ConstMsg;
        char *DynamicMsg;
    };
} ndt_context_t;

NDTYPES_API ndt_context_t *ndt_context_new(void);
NDTYPES_API void ndt_context_del(ndt_context_t *ctx);

NDTYPES_API void ndt_err_clear(ndt_context_t *ctx);
NDTYPES_API const char *ndt_context_msg(ndt_context_t *ctx);
NDTYPES_API const char *ndt_err_as_string(enum ndt_error err);
NDTYPES_API void ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...);
NDTYPES_API void ndt_err_fprint(FILE *fp, ndt_context_t *ctx);


/*****************************************************************************/
/*                                Functions                                  */
/*****************************************************************************/

/*** Various ***/
NDTYPES_API void *ndt_memory_error(ndt_context_t *ctx);
NDTYPES_API char *ndt_strdup(const char *s, ndt_context_t *ctx);
NDTYPES_API char *ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...);
NDTYPES_API const char *ndt_tag_as_string(enum ndt tag);
NDTYPES_API enum ndt_encoding ndt_encoding_from_string(char *s, ndt_context_t *ctx);
NDTYPES_API const char *ndt_encoding_as_string(enum ndt_encoding encoding);
NDTYPES_API uint32_t ndt_dim_flags(const ndt_t *t);
NDTYPES_API char ndt_order(const ndt_t *t);

NDTYPES_API int ndt_is_abstract(const ndt_t *t);
NDTYPES_API int ndt_is_concrete(const ndt_t *t);
NDTYPES_API int ndt_is_signed(const ndt_t *t);
NDTYPES_API int ndt_is_unsigned(const ndt_t *t);
NDTYPES_API int ndt_is_float(const ndt_t *t);
NDTYPES_API int ndt_is_complex(const ndt_t *t);
NDTYPES_API int ndt_is_scalar(const ndt_t *t);
NDTYPES_API int ndt_is_array(const ndt_t *t);
NDTYPES_API int ndt_is_column_major(const ndt_t *t);
NDTYPES_API int ndt_is_contiguous(const ndt_t *t);
NDTYPES_API int ndt_is_c_contiguous(const ndt_t *t);
NDTYPES_API int ndt_is_f_contiguous(const ndt_t *t);
NDTYPES_API int ndt_is_optional(const ndt_t *t);
NDTYPES_API int ndt_equal(const ndt_t *p, const ndt_t *c);
NDTYPES_API int ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_typecheck(const ndt_t *f, const ndt_t *args, int *outer_dims, ndt_context_t *ctx);

NDTYPES_API ndt_t *ndt_next_dim(ndt_t *a);
NDTYPES_API void ndt_set_next_type(ndt_t *a, ndt_t *type);
NDTYPES_API int ndt_dims_dtype(ndt_t *dims[NDT_MAX_DIM], ndt_t **dtype, ndt_t *array);
NDTYPES_API int ndt_const_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *array);

/*** String conversion ***/
NDTYPES_API bool ndt_strtobool(const char *v, ndt_context_t *ctx);
NDTYPES_API char ndt_strtochar(const char *v, ndt_context_t *ctx);
NDTYPES_API long ndt_strtol(const char *v, long min, long max, ndt_context_t *ctx);
NDTYPES_API long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx);
NDTYPES_API unsigned long ndt_strtoul(const char *v, unsigned long max, ndt_context_t *ctx);
NDTYPES_API unsigned long long ndt_strtoull(const char *v, unsigned long long max, ndt_context_t *ctx);
NDTYPES_API float ndt_strtof(const char *v, ndt_context_t *ctx);
NDTYPES_API double ndt_strtod(const char *v, ndt_context_t *ctx);


/*** Sequence elements ***/
NDTYPES_API void ndt_value_del(ndt_value_t *mem);
NDTYPES_API void ndt_value_array_del(ndt_value_t *types, size_t ntypes);

NDTYPES_API void ndt_attr_del(ndt_attr_t *attr);
NDTYPES_API void ndt_attr_array_del(ndt_attr_t *attr, size_t nattr);

NDTYPES_API ndt_field_t *ndt_field(char *name, ndt_t *type, uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
NDTYPES_API void ndt_field_del(ndt_field_t *field);
NDTYPES_API void ndt_field_array_del(ndt_field_t *fields, size_t shape);


/*** Datashape ***/
NDTYPES_API ndt_t *ndt_new(enum ndt tag, ndt_context_t *ctx);
NDTYPES_API void ndt_del(ndt_t *t);
NDTYPES_API int64_t ndt_hash(ndt_t *t, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_copy(const ndt_t *t, ndt_context_t *ctx);

/* Typedef for nominal types */
NDTYPES_API int ndt_typedef(const char *name, ndt_t *type, ndt_context_t *ctx);

/* Module */
NDTYPES_API ndt_t *ndt_module(char *name, ndt_t *type, ndt_context_t *ctx);

/* Any */
NDTYPES_API ndt_t *ndt_any_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_fixed_dim(int64_t shape, ndt_t *type, char order, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_symbolic_dim(char *name, ndt_t *type, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_var_dim(ndt_t *type, enum ndt_offsets flag, int32_t noffsets, const int32_t *offsets,
                               int32_t start, int32_t stop, int32_t step, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_ellipsis_dim(char *name, ndt_t *type, ndt_context_t *ctx);

NDTYPES_API ndt_t *ndt_option(ndt_t *type, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_dim_option(ndt_t *type, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_item_option(ndt_t *type, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_nominal(char *name, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_constr(char *name, ndt_t *type, ndt_context_t *ctx);


/* Dtypes */
NDTYPES_API ndt_t *ndt_tuple(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
                 uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_record(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
                  uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_function(ndt_t *ret, ndt_t *pos, ndt_t *kwds, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_typevar(char *name, ndt_context_t *ctx);


/* Scalar Kinds */
NDTYPES_API ndt_t *ndt_scalar_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_signed_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_unsigned_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_float_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_complex_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_fixed_string_kind(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_fixed_bytes_kind(ndt_context_t *ctx);


/* Primitive Scalars */
NDTYPES_API ndt_t *ndt_primitive(enum ndt tag, char endian, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_signed(int size, char endian, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_unsigned(int size, char endian, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_from_alias(enum ndt_alias tag, char endian, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_char(enum ndt_encoding encoding, ndt_context_t *ctx);


/* Scalars */
NDTYPES_API ndt_t *ndt_string(ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_fixed_string(size_t size, enum ndt_encoding encoding, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_bytes(uint16_opt_t target_align, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_fixed_bytes(size_t size, uint16_opt_t align, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_categorical(ndt_value_t *types, size_t ntypes, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_pointer(ndt_t *type, ndt_context_t *ctx);


/* Typed values */
NDTYPES_API ndt_value_t *ndt_value_from_number(enum ndt_value tag, char *v, ndt_context_t *ctx);
NDTYPES_API ndt_value_t *ndt_value_from_string(char *v, ndt_context_t *ctx);
NDTYPES_API ndt_value_t *ndt_value_na(ndt_context_t *ctx);
NDTYPES_API int ndt_value_equal(const ndt_value_t *x, const ndt_value_t *y);
NDTYPES_API int ndt_value_compare(const ndt_value_t *x, const ndt_value_t *y);


/******************************************************************************/
/*                                  Parsing                                   */
/******************************************************************************/

NDTYPES_API ndt_t *ndt_from_file(const char *name, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_from_string(const char *input, ndt_context_t *ctx);
NDTYPES_API ndt_t *ndt_from_offsets_dtype(enum ndt_offsets flag, int ndim, int32_t noffsets[], int32_t *offsets[], const char *dtype, ndt_context_t *ctx);


/******************************************************************************/
/*                       Initialization and tables                            */
/******************************************************************************/

NDTYPES_API int ndt_init(ndt_context_t *ctx);
NDTYPES_API void ndt_finalize(void);
NDTYPES_API int ndt_typedef_add(const char *name, const ndt_t *type, ndt_context_t *ctx);
NDTYPES_API const ndt_t *ndt_typedef_find(const char *name, ndt_context_t *ctx);


/******************************************************************************/
/*                                 Printing                                   */
/******************************************************************************/

NDTYPES_API char *ndt_as_string(ndt_t *t, ndt_context_t *ctx);
NDTYPES_API char *ndt_as_string_with_meta(ndt_t *t, ndt_context_t *ctx);
NDTYPES_API char *ndt_indent(ndt_t *t, ndt_context_t *ctx);


/******************************************************************************/
/*                            Memory handling                                 */
/******************************************************************************/

NDTYPES_API extern void *(* ndt_mallocfunc)(size_t size);
NDTYPES_API extern void *(* ndt_callocfunc)(size_t nmemb, size_t size);
NDTYPES_API extern void *(* ndt_reallocfunc)(void *ptr, size_t size);
NDTYPES_API extern void (* ndt_freefunc)(void *ptr);

NDTYPES_API void *ndt_alloc(size_t nmemb, size_t size);
NDTYPES_API void *ndt_calloc(size_t nmemb, size_t size);
NDTYPES_API void *ndt_realloc(void *ptr, size_t nmemb, size_t size);
NDTYPES_API void ndt_free(void *ptr);

NDTYPES_API void * ndt_aligned_calloc(size_t alignment, size_t size);
NDTYPES_API void ndt_aligned_free(void *ptr);


/******************************************************************************/
/*                            Low level details                               */
/******************************************************************************/

typedef struct {
    size_t size;
    uint8_t *data;
} ndt_bytes_t;

typedef size_t ndt_categorical_t;


#endif /* NDTYPES_H */
