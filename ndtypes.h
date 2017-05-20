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
  typedef _Dcomplex ndt_complex128_t;
  typedef _Fcomplex ndt_complex64_t;
  #define alignof __alignof
  #define alignas(n) __declspec(align(n))
  #define MAX_ALIGN 8
#else
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

/*** Dimension flags ***/

/* Flags that are propagated through all dimensions */
#define NDT_Dim_uint8        0x00000001U
#define NDT_Dim_uint16       0x00000002U
#define NDT_Dim_uint32       0x00000004U
#define NDT_Dim_int64        0x00000008U
#define NDT_Dim_ellipsis     0x00000010U

/* Flags for individual dimensions */
#define NDT_Dim_option       0x00000020U

/* Array flags */
#define NDT_C_contiguous     0x00000040U
#define NDT_F_contiguous     0x00000080U
#define NDT_Ndarray          0x00000100U


#define NDT_Dim_size (NDT_Dim_uint8  \
                     |NDT_Dim_uint16 \
                     |NDT_Dim_uint32 \
                     |NDT_Dim_int64)

#define NDT_Contiguous (NDT_C_contiguous|NDT_F_contiguous)


/* Types: ndt_t */
typedef struct _ndt ndt_t;

/* Values: ndt_value_t */
typedef union {
  bool Bool;
  char Char;
  int8_t Int8;
  int16_t Int16;
  int32_t Int32;
  int64_t Int64;
  uint8_t Uint8;
  uint16_t Uint16;
  uint32_t Uint32;
  uint64_t Uint64;
  float Float32;
  double Float64;
  char *String;
} ndt_value_t;


/* Typed memory (could be extended to a memoryview) */
typedef struct {
    ndt_t *t;
    ndt_value_t v;
} ndt_memory_t;

/* Internal option type */
enum ndt_option {
  None,
  Some
};

typedef struct {
  enum ndt_option tag;
  uint16_t Some;
} uint16_opt_t;

typedef struct {
  enum ndt_option tag;
  char Some;
} char_opt_t;

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
  AttrInt64List,
  AttrCharOpt,
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


/* Encoding for characters and strings */
enum ndt_encoding {
  Ascii,
  Utf8,
  Utf16,
  Utf32,
  Ucs2,
  ErrorEncoding
};

/* Dimension data types */
enum ndt_dim {
  DimNone,
  DimUint8,
  DimUint16,
  DimUint32,
  DimInt32,
  DimInt64
};

/* Datashape kinds */
enum ndt {
  /* Any */
  AnyKind,
    FixedDim,
    SymbolicDim,
    VarDim,
    EllipsisDim,

    Array,

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

        RealKind,
          Float16,
          Float32,
          Float64,

        ComplexKind,
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
      uint16_t align;
      bool explicit_align;
  } Concrete;
} ndt_field_t;

/* Datashape type */
struct _ndt {
    enum ndt tag;
    enum ndt_access access;
    int ndim;

    /* Abstract */
    union {
        struct {
            uint32_t flags;
            ndt_t *type;
        } Array;

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
            ndt_memory_t *types;
        } Categorical;

        struct {
            ndt_t *type;
        } Pointer;
    };

    /* Concrete */
    struct {
        union {
            struct {
                enum ndt_dim dim_type; /* data type of the var-dim representation */
                int data_start;        /* start index into the 'data' member array */
                int64_t *data;         /* offsets of actual array data and var-dim data */
                int64_t *bitmaps;      /* offsets of validity bitmaps */
            } Array;

            struct {
                int64_t offset;
                int64_t itemsize;
                int64_t stride;
            } FixedDim;

            struct {
                int64_t offset;
                int64_t itemsize;
                int64_t stride;
                int64_t nshapes; /* default: 0 */
                int64_t *shapes; /* default: NULL */
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

        size_t size;
        uint16_t align;

    } Concrete;

    alignas(MAX_ALIGN) char extra[];
};


/*****************************************************************************/
/*                        Context and  error handling                        */
/*****************************************************************************/

enum ndt_error {
  NDT_Success,
  NDT_MemoryError,
  NDT_ValueError,
  NDT_TypeError,
  NDT_InvalidArgumentError,
  NDT_RuntimeError,
  NDT_NotImplementedError,
  NDT_LexError,
  NDT_ParseError,
  NDT_OSError
};

enum ndt_msg {
  ConstMsg,
  DynamicMsg
};

typedef struct {
    enum ndt_error err;
    enum ndt_msg msg;
    union {
        const char *ConstMsg;
        char *DynamicMsg;
    };
} ndt_context_t;

ndt_context_t *ndt_context_new(void);
void ndt_context_del(ndt_context_t *ctx);

void ndt_err_clear(ndt_context_t *ctx);
const char *ndt_context_msg(ndt_context_t *ctx);
const char *ndt_err_as_string(enum ndt_error err);
void ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...);
void ndt_err_fprint(FILE *fp, ndt_context_t *ctx);


/*****************************************************************************/
/*                                Functions                                  */
/*****************************************************************************/

/*** Various ***/
void *ndt_memory_error(ndt_context_t *ctx);
char *ndt_strdup(const char *s, ndt_context_t *ctx);
char *ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...);
const char *ndt_tag_as_string(enum ndt tag);
enum ndt_dim ndt_dim_type(const ndt_t *t);
const char *ndt_dim_type_as_string(enum ndt_dim tag);
enum ndt_encoding ndt_encoding_from_string(char *s, ndt_context_t *ctx);
const char *ndt_encoding_as_string(enum ndt_encoding encoding);
uint32_t ndt_dim_flags(const ndt_t *t);

int ndt_is_abstract(const ndt_t *t);
int ndt_is_concrete(const ndt_t *t);
int ndt_is_signed(const ndt_t *t);
int ndt_is_unsigned(const ndt_t *t);
int ndt_is_real(const ndt_t *t);
int ndt_is_complex(const ndt_t *t);
int ndt_is_abstract(const ndt_t *t);
int ndt_is_scalar(const ndt_t *t);
int ndt_is_array(const ndt_t *t);
int ndt_is_column_major(const ndt_t *t);
int ndt_is_contiguous(const ndt_t *t);
int ndt_is_c_contiguous(const ndt_t *t);
int ndt_is_f_contiguous(const ndt_t *t);
int ndt_is_scalar(const ndt_t *t);
int ndt_is_optional(const ndt_t *t);
int ndt_equal(const ndt_t *p, const ndt_t *c);
int ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx);

ndt_t *ndt_next_dim(ndt_t *a);
void ndt_set_next_type(ndt_t *a, ndt_t *type);
int ndt_dims_dtype(ndt_t *dims[NDT_MAX_DIM], ndt_t **dtype, ndt_t *array);
int ndt_const_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *array);

/*** String conversion ***/
bool ndt_strtobool(const char *v, ndt_context_t *ctx);
char ndt_strtochar(const char *v, ndt_context_t *ctx);
long ndt_strtol(const char *v, long min, long max, ndt_context_t *ctx);
long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx);
unsigned long ndt_strtoul(const char *v, unsigned long max, ndt_context_t *ctx);
unsigned long long ndt_strtoull(const char *v, unsigned long long max, ndt_context_t *ctx);
float ndt_strtof(const char *v, ndt_context_t *ctx);
double ndt_strtod(const char *v, ndt_context_t *ctx);


/*** Sequence elements ***/
void ndt_memory_del(ndt_memory_t *mem);
void ndt_memory_array_del(ndt_memory_t *types, size_t ntypes);

void ndt_attr_del(ndt_attr_t *attr);
void ndt_attr_array_del(ndt_attr_t *attr, size_t nattr);

ndt_field_t *ndt_field(char *name, ndt_t *type, uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
void ndt_field_del(ndt_field_t *field);
void ndt_field_array_del(ndt_field_t *fields, size_t shape);


/*** Datashape ***/
ndt_t *ndt_new(enum ndt tag, ndt_context_t *ctx);
void ndt_del(ndt_t *t);

/* Typedef for nominal types */
int ndt_typedef(const char *name, ndt_t *type, ndt_context_t *ctx);

/* Any */
ndt_t *ndt_any_kind(ndt_context_t *ctx);
ndt_t *ndt_fixed_dim(int64_t shape, ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_symbolic_dim(char *name, ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_var_dim(int64_t *shapes, int64_t nshapes, ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_ellipsis_dim(ndt_t *type, ndt_context_t *ctx);

ndt_t *ndt_array(ndt_t *array, int64_t *strides, int64_t *offsets, char_opt_t order, ndt_context_t *ctx);
ndt_t *ndt_option(ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_dim_option(ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_item_option(ndt_t *type, ndt_context_t *ctx);
ndt_t *ndt_nominal(char *name, ndt_context_t *ctx);
ndt_t *ndt_constr(char *name, ndt_t *type, ndt_context_t *ctx);


/* Dtypes */
ndt_t *ndt_tuple(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
                 uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
ndt_t *ndt_record(enum ndt_variadic flag, ndt_field_t *fields, int64_t shape,
                  uint16_opt_t align, uint16_opt_t pack, ndt_context_t *ctx);
ndt_t *ndt_function(ndt_t *ret, ndt_t *pos, ndt_t *kwds, ndt_context_t *ctx);
ndt_t *ndt_typevar(char *name, ndt_context_t *ctx);


/* Scalar Kinds */
ndt_t *ndt_scalar_kind(ndt_context_t *ctx);
ndt_t *ndt_signed_kind(ndt_context_t *ctx);
ndt_t *ndt_unsigned_kind(ndt_context_t *ctx);
ndt_t *ndt_real_kind(ndt_context_t *ctx);
ndt_t *ndt_complex_kind(ndt_context_t *ctx);
ndt_t *ndt_fixed_string_kind(ndt_context_t *ctx);
ndt_t *ndt_fixed_bytes_kind(ndt_context_t *ctx);


/* Primitive Scalars */
ndt_t *ndt_primitive(enum ndt tag, char endian, ndt_context_t *ctx);
ndt_t *ndt_signed(int size, char endian, ndt_context_t *ctx);
ndt_t *ndt_unsigned(int size, char endian, ndt_context_t *ctx);
ndt_t *ndt_from_alias(enum ndt_alias tag, char endian, ndt_context_t *ctx);
ndt_t *ndt_char(enum ndt_encoding encoding, ndt_context_t *ctx);


/* Scalars */
ndt_t *ndt_string(ndt_context_t *ctx);
ndt_t *ndt_fixed_string(size_t size, enum ndt_encoding encoding, ndt_context_t *ctx);
ndt_t *ndt_bytes(uint16_opt_t target_align, ndt_context_t *ctx);
ndt_t *ndt_fixed_bytes(size_t size, uint16_opt_t align, ndt_context_t *ctx);
ndt_t *ndt_categorical(ndt_memory_t *types, size_t ntypes, ndt_context_t *ctx);
ndt_t *ndt_pointer(ndt_t *type, ndt_context_t *ctx);


/* Typed values */
ndt_memory_t *ndt_memory_from_number(char *v, ndt_t *t, ndt_context_t *ctx);
ndt_memory_t *ndt_memory_from_string(char *v, ndt_t *t, ndt_context_t *ctx);
int ndt_memory_equal(const ndt_memory_t *x, const ndt_memory_t *y);
int ndt_memory_compare(const ndt_memory_t *x, const ndt_memory_t *y);


/******************************************************************************/
/*                                  Parsing                                   */
/******************************************************************************/

ndt_t *ndt_from_file(const char *name, ndt_context_t *ctx);
ndt_t *ndt_from_string(const char *input, ndt_context_t *ctx);


/******************************************************************************/
/*                       Initialization and tables                            */
/******************************************************************************/

int ndt_init(ndt_context_t *ctx);
void ndt_finalize(void);
int ndt_typedef_add(const char *name, const ndt_t *type, ndt_context_t *ctx);
const ndt_t *ndt_typedef_find(const char *name, ndt_context_t *ctx);


/******************************************************************************/
/*                                 Printing                                   */
/******************************************************************************/

char *ndt_as_string(ndt_t *t, ndt_context_t *ctx);
char *ndt_as_string_with_meta(ndt_t *t, ndt_context_t *ctx);
char *ndt_indent(ndt_t *t, ndt_context_t *ctx);


/******************************************************************************/
/*                            Memory handling                                 */
/******************************************************************************/

extern void *(* ndt_mallocfunc)(size_t size);
extern void *(* ndt_callocfunc)(size_t nmemb, size_t size);
extern void *(* ndt_reallocfunc)(void *ptr, size_t size);
extern void (* ndt_free)(void *ptr);

void *ndt_alloc(size_t nmemb, size_t size);
void *ndt_calloc(size_t nmemb, size_t size);
void *ndt_realloc(void *ptr, size_t nmemb, size_t size);


/******************************************************************************/
/*                            Low level details                               */
/******************************************************************************/

typedef struct {
    char *data;
} ndt_fixed_dim_t;

typedef struct {
    int64_t shape;
    char *data;
} ndt_var_dim_t;

typedef struct {
    size_t size;
    char *data;
} ndt_bytes_t;

typedef struct {
    size_t size;
    char *data;
} ndt_sized_string_t;

typedef struct {
    char *data;
} ndt_pointer_t;


#endif /* NDTYPES_H */
