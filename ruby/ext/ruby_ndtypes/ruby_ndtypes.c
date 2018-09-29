/* Main file for ndtypes ruby wrapper.
 *
 * Author: Sameer Deshmukh (@v0dro)
 */

#include "ruby_ndtypes_internal.h"

/* ---------- Interal declarations ---------- */
/* data_type_t variables. */
static const rb_data_type_t NdtObject_type;
static const rb_data_type_t ResourceBufferObject_type;

/* Class declarations. */
VALUE cNDTypes;
VALUE mNDTypes_GCGuard;
static VALUE cNDTypes_RBuf;

static VALUE rb_eValueError;

/* ------------------------------------------ */
/****************************************************************************/
/*                               Error handling                             */
/****************************************************************************/

/* Raise an error stored in $!. Clears it before raising. */
static void
raise_error(void)
{
  VALUE exeception = rb_errinfo();

  rb_set_errinfo(Qnil);
  rb_exc_raise(exeception);
}

static void
set_error_info(VALUE err, const char * msg)
{
  rb_set_errinfo(rb_exc_new2(err, msg));
}

static VALUE
seterr(ndt_context_t *ctx)
{
  VALUE exc = rb_eRuntimeError;

  switch(ctx->err) {
  case NDT_Success: /* should never be set on error */
    exc = rb_eRuntimeError;
    break;
  case NDT_ValueError:
    exc = rb_eValueError;
    break;
  case NDT_TypeError:
    exc = rb_eTypeError;
    break;
  case NDT_InvalidArgumentError:
    exc = rb_eValueError;
    break;
  case NDT_NotImplementedError:
    exc = rb_eNotImpError;
    break;
  case NDT_IndexError:
    exc = rb_eIndexError;
    break;
  case NDT_LexError: case NDT_ParseError:
    exc = rb_eValueError;
    break;
  case NDT_OSError:
    exc = rb_eSysStackError;
    break;
  case NDT_RuntimeError:
    exc = rb_eRuntimeError;
    break;
  case NDT_MemoryError:
    exc = rb_eNoMemError;
    break;
  }

  set_error_info(exc, ndt_context_msg(ctx));
  ndt_context_del(ctx);
  
  return exc;
}

/******************************************************************************/
/************************* Resource Buffer Object *****************************/

/* A ResourceBufferObject is passed around various NDT objects for storing 
 * internal state. It might or might not be duplicated across other NDT
 * objects.
 */
typedef struct ResourceBufferObject {
  ndt_meta_t *m;
} ResourceBufferObject;

#define GET_RBUF(obj, rbuf_p) do {                              \
    TypedData_Get_Struct((obj), ResourceBufferObject,           \
                         &ResourceBufferObject_type, (rbuf_p)); \
} while (0)
#define RBUF_NDT_M(rbuf_p) ((ndt_meta_t*)(rbuf_p->m))
#define MAKE_RBUF(self, rbuf_p) TypedData_Make_Struct(self, ResourceBufferObject, \
                                                      &ResourceBufferObject_type, rbuf_p)
#define WRAP_RBUF(self, rbuf_p) TypedData_Wrap_Struct(self,             \
                                                      &ResourceBufferObject_type, rbuf_p)

/* GC free the ResourceBufferObject struct. */
static void
ResourceBufferObject_dfree(void * self)
{
  ResourceBufferObject * rbf = (ResourceBufferObject*)self;

  ndt_meta_del(rbf->m);
  rbf->m = NULL;
  xfree(rbf);
}

/* Calculate the size of the object. */
static size_t
ResourceBufferObject_dsize(const void *self)
{
  return sizeof(ResourceBufferObject);           
}

static const rb_data_type_t ResourceBufferObject_type = {
  .wrap_struct_name = "ResourceBufferObject",
  .function = {
    .dmark = 0,
    .dfree = ResourceBufferObject_dfree,
    .dsize = ResourceBufferObject_dsize,
    .reserved = {0,0},
  },
  .parent = 0,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

/* FIXME: change this to rbuf_alloc to reflect that its not called by Ruby alloc. */
static VALUE
rbuf_allocate(void)
{
  NDT_STATIC_CONTEXT(ctx);
  ResourceBufferObject *self;

  self = ALLOC(ResourceBufferObject);

  self->m = ndt_meta_new(&ctx);
  if (self->m == NULL) {
    rb_raise(rb_eNoMemError, "cannot allocate rbuf object.");
  }
  
  return WRAP_RBUF(cNDTypes_RBuf, self);
}

static int
rbuf_init_from_offset_list(ResourceBufferObject *rbuf, VALUE list)
{
  ndt_meta_t * const m = rbuf->m;
  VALUE lst;

  Check_Type(list, T_ARRAY);

  const int64_t n = RARRAY_LEN(list);
  if (n < 1 || n > NDT_MAX_DIM) {
    rb_raise(rb_eValueError, "number of offset lists must be in [1, %d].",
             NDT_MAX_DIM);
  }

  m->ndims = 0;
  for (int64_t i = n-1; i >= 0; i--) {
    lst = rb_ary_entry(list, i);
    if (!RB_TYPE_P(lst, T_ARRAY)) {
      rb_raise(rb_eTypeError, "expected a list of offset lists.");
    }

    const int64_t noffsets = RARRAY_LEN(lst);
    if (noffsets < 2 || noffsets > INT32_MAX) {
      rb_raise(rb_eValueError, "length of a single offset must be in [2, INT32_MAX].");
    }

    int32_t * const offsets = ndt_alloc(noffsets, sizeof(int32_t));
    if (offsets == NULL) {
      rb_raise(rb_eNoMemError, "could not allocate offsets.");
    }

    for (int32_t k = 0; k < noffsets; k++) {
      long long x = NUM2LL(rb_ary_entry(lst, k));
      if (x == -1 || x < 0 || x > INT32_MAX) {
        ndt_free(offsets);
        rb_raise(rb_eValueError, "offset must be in [0, INT32_MAX].");
      }

      offsets[k] = (int32_t)x;
    }

    m->noffsets[m->ndims] = (int32_t)noffsets;
    m->offsets[m->ndims] = offsets;
    m->ndims++;
  }

  return 0;
}

static VALUE
rbuf_from_offset_lists(VALUE list)
{
  VALUE rbuf;
  ResourceBufferObject * rbuf_p;

  rbuf = rbuf_allocate();
  GET_RBUF(rbuf, rbuf_p);
  rbuf_init_from_offset_list(rbuf_p, list);

  return rbuf;
}

/******************************************************************************/

/******************************************************************************/
/************************* NDT struct object **********************************/

typedef struct NdtObject {
  VALUE rbuf;                  /* resource buffer */
  ndt_t *ndt;                   /* type */
} NdtObject;

#define NDT(v) (((NdtObject *)v)->ndt)
#define RBUF(v) (((NdtObject *)v)->rbuf)
#define GET_NDT(obj, ndt_p) do {                        \
    TypedData_Get_Struct((obj), NdtObject,              \
                         &NdtObject_type, (ndt_p));     \
  } while (0)
#define MAKE_NDT(self, ndt_p) TypedData_Make_Struct(self, NdtObject,    \
                                                    &NdtObject_type, ndt_p)
#define WRAP_NDT(self, ndt_p) TypedData_Wrap_Struct(self, &NdtObject_type, ndt_p)
#define NDT_CHECK_TYPE(obj) (CLASS_OF(obj) == cNDTypes)

/* Get the metatdata of the ResourceBufferObject within this NDT Ruby object. */
static ndt_meta_t *
rbuf_ndt_meta(VALUE ndt)
{
  NdtObject *ndt_p;
  ResourceBufferObject *rbuf_p;
  
  GET_NDT(ndt, ndt_p);
  GET_RBUF(ndt_p->rbuf, rbuf_p);

  return rbuf_p->m;
}

/* Allocate an NdtObject, initialize members and return wrapped as a Ruby object. */
static VALUE
NdtObject_alloc(void)
{
  NdtObject *ndt_p;

  ndt_p = ZALLOC(NdtObject);

  ndt_p->rbuf = 0;
  ndt_p->ndt = NULL;

  return WRAP_NDT(cNDTypes, ndt_p);
}

/* GC mark the NdtObject struct. */
/* TODO: verify that there are no more object allocations happening inside ndt. */
static void
NdtObject_dmark(void * self)
{
  NdtObject * ndt = (NdtObject*)self;
  
  rb_gc_mark(ndt->rbuf);
}

/* GC free the NdtObject struct. */
static void
NdtObject_dfree(void * self)
{
  NdtObject * ndt = (NdtObject*)self;
  
  rb_ndtypes_gc_guard_unregister(ndt);
  xfree(ndt);
}

/* Calculate the size of the object. */
static size_t
NdtObject_dsize(const void *self)
{
  return sizeof(NdtObject);
}

static const rb_data_type_t NdtObject_type = {
  .wrap_struct_name = "NdtObject",
  .function = {
    .dmark = NdtObject_dmark,
    .dfree = NdtObject_dfree,
    .dsize = NdtObject_dsize,
    .reserved = {0,0},
  },
  .parent = 0,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

/* Allocate an NDT object and return a Ruby object. Used for Ruby class initialization. */
static VALUE
NDTypes_allocate(VALUE self)
{
  NdtObject *ndt;
  
  return MAKE_NDT(self, ndt);
}
/******************************************************************************/

#define NDTYPES_BOOL_FUNC(NDTFUNC)                                      \
  static VALUE                                                          \
  NDTypes_##NDTFUNC(VALUE self)                                         \
  {                                                                     \
    NdtObject *ndt_p;                                                   \
    GET_NDT(self, ndt_p);                                               \
    if (NDTFUNC(NDT(ndt_p))) {                                          \
      return Qtrue;                                                     \
    }                                                                   \
    return Qfalse;                                                      \
  }

NDTYPES_BOOL_FUNC(ndt_is_abstract)
NDTYPES_BOOL_FUNC(ndt_is_concrete)

NDTYPES_BOOL_FUNC(ndt_is_optional)
NDTYPES_BOOL_FUNC(ndt_is_scalar)
NDTYPES_BOOL_FUNC(ndt_is_signed)
NDTYPES_BOOL_FUNC(ndt_is_unsigned)
NDTYPES_BOOL_FUNC(ndt_is_float)
NDTYPES_BOOL_FUNC(ndt_is_complex)

NDTYPES_BOOL_FUNC(ndt_is_c_contiguous)
NDTYPES_BOOL_FUNC(ndt_is_f_contiguous)

static VALUE
array_from_int64(int64_t x[NDT_MAX_DIM], int ndim)
{
  VALUE array;
  int i;

  array = rb_ary_new2(ndim);

  for (i = 0; i < ndim; ++i) {
    VALUE v = LL2NUM(x[i]);
    rb_ary_store(array, i, v);
  }

  return array;
}

static VALUE
NDTypes_from_object(VALUE self, VALUE type)
{
  NDT_STATIC_CONTEXT(ctx);
  const char *cp;
  NdtObject *ndt_p;

  if (NDT_CHECK_TYPE(type)) {
    return rb_funcall(type, rb_intern("dup"), 0, NULL);
  }

  cp = StringValuePtr(type);

  GET_NDT(self, ndt_p);
  RBUF(ndt_p) = rbuf_allocate();
  if (RBUF(ndt_p) == NULL) {
    rb_raise(rb_eNoMemError, "problem in allocating RBUF object.");
  }
  
  rb_ndtypes_gc_guard_register(ndt_p, RBUF(ndt_p));

  NDT(ndt_p) = ndt_from_string_fill_meta(rbuf_ndt_meta(self), cp, &ctx);
  if (NDT(ndt_p) == NULL) {
    seterr(&ctx);
    raise_error();
  }

  return self; 
}

static VALUE
NDTypes_from_offsets_and_dtype(VALUE self, VALUE offsets, VALUE type)
{
  NDT_STATIC_CONTEXT(ctx);
  NdtObject *self_p;
  ResourceBufferObject *rbuf_p;
  const char *cp;

  Check_Type(type, T_STRING);
  Check_Type(offsets, T_ARRAY);

  cp = StringValuePtr(type);

  GET_NDT(self, self_p);
  RBUF(self_p) = rbuf_from_offset_lists(offsets);
  GET_RBUF(RBUF(self_p), rbuf_p);
  
  NDT(self_p) = ndt_from_metadata_and_dtype(rbuf_p->m, cp, &ctx);
  if(NDT(self_p) == NULL) {
    seterr(&ctx);
    raise_error();
  }

  rb_ndtypes_gc_guard_register(self_p, RBUF(self_p));

  return self;
}

/* Initialize an instance of an NDTypes object. */
static VALUE
NDTypes_initialize(int argc, VALUE *argv, VALUE self)
{
  NdtObject *ndt_p;
  VALUE offsets = Qnil, type;
  
  if (argc < 1) {
    rb_raise(rb_eArgError, "expected atleast type. offset optional. Number of args: %d.",
             argc);
  }

  type = argv[0];
  if (argc == 2) {
    offsets = argv[1];
  }

  if (offsets == Qnil) {
    return NDTypes_from_object(self, type);
  }

  return NDTypes_from_offsets_and_dtype(self, offsets, type);
}

/* String representation of the type. */
static VALUE
NDTypes_to_s(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  char *cp;
  NdtObject *ndt;
  VALUE str;

  GET_NDT(self, ndt);
  cp = ndt_as_string(NDT(ndt), &ctx);
  if (cp == NULL) {
    seterr(&ctx);
    raise_error();
  }

  str = rb_str_new_cstr(cp);
  ndt_free(cp);

  return str;
}

/* Serialize the NDTypes object into a byte string. */
static VALUE
NDTypes_serialize(VALUE self)
{
  NdtObject *self_p;
  char *bytes;
  int64_t size;
  VALUE str;
  
  NDT_STATIC_CONTEXT(ctx);
  GET_NDT(self, self_p);

  size = ndt_serialize(&bytes, NDT(self_p), &ctx);
  if (size < 0) {
    seterr(&ctx);
    raise_error();
  }

  str = rb_usascii_str_new(bytes, size);
  ndt_free(bytes);

  return str;
}

/* Implement #ndim */
static VALUE
NDTypes_ndim(VALUE self)
{
  NdtObject *ndt_p;

  GET_NDT(self, ndt_p);
  
  const ndt_t *t = NDT(ndt_p);

  if (ndt_is_abstract(t)) {
    rb_raise(rb_eNoMethodError, "abstract type has no ndim.");
  }

  return LL2NUM(t->ndim);
}

/* #datasize */
static VALUE
NDTypes_datasize(VALUE self)
{
  NdtObject *ndt_p;

  GET_NDT(self, ndt_p);
  
  const ndt_t *t = NDT(ndt_p);

  if (ndt_is_abstract(t)) {
    rb_raise(rb_eTypeError, "abstract type has no datasize.");
  }

  return LL2NUM(t->datasize);  
}

/* #itemsize */
static VALUE
NDTypes_itemsize(VALUE self)
{
  NdtObject *ndt_p;
  int64_t size;

  GET_NDT(self, ndt_p);
  
  const ndt_t *t = NDT(ndt_p);

  if (ndt_is_abstract(t)) {
    rb_raise(rb_eNoMethodError, "abstract type has no datasize.");
  }

  switch (t->tag) {
  case FixedDim:
    size = t->Concrete.FixedDim.itemsize;
    break;
  case VarDim:
    size = t->Concrete.VarDim.itemsize;
    break;
  default:
    size = t->datasize;
    break;
  }

  return LL2NUM(size);
}

/* Implement #align */
static VALUE
NDTypes_align(VALUE self)
{
  NdtObject *ndt_p;
 
  GET_NDT(self, ndt_p);
  
  const ndt_t *t = NDT(ndt_p);

  if (ndt_is_abstract(t)) {
    rb_raise(rb_eNoMethodError, "abstract type has no datasize.");
  }

  return LL2NUM(t->align);
}

static int
NDTypes_compare(VALUE left, VALUE right)
{
  NdtObject *left_p = NULL, *right_p = NULL;

  GET_NDT(left, left_p);
  GET_NDT(right, right_p);

  return ndt_equal(NDT(left_p), NDT(right_p));  
}

/* Implement #== operator */
static VALUE
NDTypes_eqeq(VALUE self, VALUE other)
{
  if (!NDT_CHECK_TYPE(other)) {
    return Qfalse;
  }
  
  int r = NDTypes_compare(self, other);
  
  if (r == 0) {                  /* not equal */
    return Qfalse;
  }
  else {                        /* equal */
    return Qtrue;
  }
}

/* Implemented #!= operator */
static VALUE
NDTypes_neq(VALUE self, VALUE other)
{
  if (!NDT_CHECK_TYPE(other)) {
    return Qfalse;
  }
  
  int r = NDTypes_compare(self, other);
  
  if (r == 0) {                 /* not equal */
    return Qtrue;
  }
  else {                        /* equal */
    return Qfalse;
  }
}

/* Implement NDT#hidden_dtype */
static VALUE
NDTypes_hidden_dtype(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  NdtObject *self_p;

  GET_NDT(self, self_p);
  
  const ndt_t *t = NDT(self_p);
  const ndt_t *dtype;
  ndt_t *u;

  dtype = ndt_hidden_dtype(t);

  u = ndt_copy(dtype, &ctx);
  if (u == NULL) {
    seterr(&ctx);
    raise_error();
  }

  return rb_ndtypes_from_type(u);
}

static VALUE
NDTypes_match(VALUE self, VALUE other)
{
  NDT_STATIC_CONTEXT(ctx);
  int res;
  NdtObject *self_p, *other_p;

  if (!NDT_CHECK_TYPE(other)) {
    rb_raise(rb_eTypeError, "argument must be of type NDT.");
  }

  GET_NDT(self, self_p);
  GET_NDT(other, other_p);

  res = ndt_match(NDT(self_p), NDT(other_p), &ctx);
  if (res == -1) {
    seterr(&ctx);
    raise_error();
  }

  return INT2BOOL(res);
}

static VALUE
NDTypes_ast(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  VALUE result;
  char *cp;
  NdtObject *self_p;

  GET_NDT(self, self_p);

  cp = ndt_ast_repr(NDT(self_p), &ctx);
  if (cp == NULL) {
    seterr(&ctx);
    raise_error();
  }

  result = rb_str_new2(cp);
  ndt_free(cp);

  return result; 
}

static VALUE
NDTypes_pretty(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  char *cp;
  NdtObject *self_p;
  VALUE result;

  GET_NDT(self, self_p);

  cp = ndt_indent(NDT(self_p), &ctx);
  if (cp == NULL) {
    seterr(&ctx);
    raise_error();
  }

  result = rb_str_new2(cp);
  ndt_free(cp);

  return result;
}

/* NDT#shape */
static VALUE
NDTypes_shape(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  ndt_ndarray_t x;
  NdtObject *self_p;

  GET_NDT(self, self_p);

  if (ndt_as_ndarray(&x, NDT(self_p), &ctx) < 0) {
    seterr(&ctx);
    raise_error();
  }

  return array_from_int64(x.shape, x.ndim);
}

static VALUE
NDTypes_strides(VALUE self)
{
  NDT_STATIC_CONTEXT(ctx);
  ndt_ndarray_t x;
  NdtObject *self_p;

  GET_NDT(self, self_p);

  if (ndt_as_ndarray(&x, NDT(self_p), &ctx) < 0) {
    seterr(&ctx);
    raise_error();
  }

  return array_from_int64(x.strides, x.ndim);  
}

static VALUE
NDTypes_apply(VALUE self, VALUE types)
{
  NDT_STATIC_CONTEXT(ctx);
  NdtObject *self_p, *x_p;
  ndt_t *sig;
  const ndt_t *in[NDT_MAX_ARGS];
  ndt_apply_spec_t spec;
  VALUE flags, out, broadcast, outer_dims;
  size_t nin;
  int i;

  Check_Type(types, T_ARRAY);

  GET_NDT(self, self_p);
  sig = NDT(self_p);

  nin = RARRAY_LEN(types);
  if (nin > NDT_MAX_ARGS) {
    rb_raise(rb_eArgError, "maximum number of arguments is %d", NDT_MAX_ARGS);
  }

  for (i = 0; i < nin; ++i) {
    VALUE temp = rb_ary_entry(types, i);
    if (!NDT_CHECK_TYPE(temp)) {
      rb_raise(rb_eTypeError, "argument types must be NDT.");
    }
    in[i] = rb_ndtypes_const_ndt(temp);
  }

  spec = ndt_apply_spec_empty;
  if (ndt_typecheck(&spec, sig, in, (int)nin, NULL, NULL, &ctx) < 0) {
    seterr(&ctx);
    raise_error();
  }

  flags = rb_str_new2(ndt_apply_flags_as_string(&spec));
  out = rb_ary_new2(spec.nout);

  for (i = spec.nout - 1; i >= 0 ; i--) {
    VALUE x = NdtObject_alloc();
    GET_NDT(x, x_p);
    NDT(x_p) = spec.out[i];
    spec.out[i]= NULL; spec.nout--;
    rb_ary_store(out, i, x);
  }

  broadcast = rb_ary_new2(spec.nbroadcast);

  for (i = spec.nbroadcast-1; i >= 0; i--) {
    VALUE x = NdtObject_alloc();
    GET_NDT(x, x_p);
    NDT(x_p) = spec.broadcast[i];
    spec.broadcast[i] = NULL; spec.nbroadcast--;
    rb_ary_store(broadcast, i, x);
  }

  outer_dims = LL2NUM(spec.outer_dims);

  return rb_funcall(rb_const_get(cNDTypes, rb_intern("ApplySpec")), rb_intern("new"),
                    6, flags, self, types, out, broadcast, outer_dims);
}

/****************************************************************************/
/*                                  Class methods                           */
/****************************************************************************/

/* Deserialize a byte string into an NDTypes object. */
static VALUE
NDTypes_s_deserialize(VALUE klass, VALUE str)
{
  NdtObject *ndt_p;
  ResourceBufferObject *rbuf_p;
  VALUE ndt, rbuf;
  char *cp;
  int64_t len;
  NDT_STATIC_CONTEXT(ctx);

  Check_Type(str, T_STRING);

  cp = StringValuePtr(str);
  len = RSTRING_LEN(str);

  rbuf_p = ALLOC(ResourceBufferObject);
  rbuf_p->m = ndt_meta_new(&ctx);
  if (rbuf_p->m == NULL) {
    seterr(&ctx);
    raise_error();
  }

  ndt_p = ALLOC(NdtObject);
  NDT(ndt_p) = ndt_deserialize(RBUF_NDT_M(rbuf_p), cp, len, &ctx);
  if (NDT(ndt_p) == NULL) {
    seterr(&ctx);
    raise_error();
  }

  rbuf = WRAP_RBUF(cNDTypes_RBuf, rbuf_p);
  RBUF(ndt_p) = rbuf;
  rb_ndtypes_gc_guard_register(ndt_p, rbuf);
  ndt = WRAP_NDT(cNDTypes, ndt_p);
  
  return ndt;
}

/* Create a typedef */
static VALUE
NDTypes_s_typedef(VALUE klass, VALUE new_type, VALUE old_type)
{
  NDT_STATIC_CONTEXT(ctx);
  const char *cname, *ctype;
  ndt_t *t;

  Check_Type(new_type, T_STRING);
  Check_Type(old_type, T_STRING);

  cname = StringValueCStr(new_type);
  ctype = StringValueCStr(old_type);

  t = ndt_from_string(ctype, &ctx);
  if (t == NULL) {
    seterr(&ctx);
    raise_error();
  }

  if (ndt_typedef(cname, t, NULL, &ctx) < 0) {
    seterr(&ctx);
    raise_error();    
  }

  return Qnil;
}

/* Instatiate ndtypes object using typedef'd type and another NDTypes object. */
static VALUE
NDTypes_s_instantiate(VALUE klass, VALUE name, VALUE type)
{
  const char *cname;
  char *cp;
  ndt_t *t, *tp;
  NdtObject *type_p;
  NDT_STATIC_CONTEXT(ctx);

  Check_Type(name, T_STRING);

  cname = StringValuePtr(name);

  if (!NDT_CHECK_TYPE(type)) {
    rb_raise(rb_eTypeError, "type argument must be ndt.");
  }

  cp = ndt_strdup(cname, &ctx);
  if (cp == NULL) {
    seterr(&ctx);
    raise_error();
  }

  GET_NDT(type, type_p);

  tp = ndt_copy(NDT(type_p), &ctx);
  if (tp == NULL) {
    ndt_free(cp);
    seterr(&ctx);
    raise_error();
  }

  t = ndt_nominal(cp, tp, &ctx);
  if (t == NULL) {
    seterr(&ctx);
    raise_error();
  }

  return rb_ndtypes_move_subtree(type, t);
}

static VALUE
NDTypes_s_from_format(VALUE klass, VALUE format)
{
  NDT_STATIC_CONTEXT(ctx);
  VALUE self;
  NdtObject *self_p;
  const char *cp;

  Check_Type(format, T_STRING);

  cp = StringValueCStr(format);
  self = NdtObject_alloc();
  
  GET_NDT(self, self_p);
  
  NDT(self_p) = ndt_from_bpformat(cp, &ctx);
  if (NDT(self_p) == NULL) {
    seterr(&ctx);
    raise_error();
  }

  return self;
}

/****************************************************************************/
/*                                 Public C API                               */
/****************************************************************************/

/* Create NDT object from ndt_t type struct. */
VALUE
rb_ndtypes_from_type(ndt_t *type)
{
  VALUE self;
  NdtObject *self_p;

  self = NdtObject_alloc();
  GET_NDT(self, self_p);

  NDT(self_p) = type;

  return self;
}

/* Return 1 if obj is of type NDTypes. 0 otherwise. */
int
rb_ndtypes_check_type(VALUE obj)
{
  return NDT_CHECK_TYPE(obj);
}

/* Get a pointer to the NdtObject struct that is contained within obj. */
NdtObject *
rb_ndtypes_get_ndt_object(VALUE obj)
{
  NdtObject *ndt_p;

  if (!NDT_CHECK_TYPE(obj)) {
    /* raise error */
  }
  
  GET_NDT(obj, ndt_p);

  return ndt_p;
}

/* Get an allocated Ruby object of type NDTypes. ndt_p should have been allocated already. */
VALUE
rb_ndtypes_make_ndt_object(NdtObject *ndt_p)
{
  return MAKE_NDT(cNDTypes, ndt_p);
}

/* Perform allocation and get a Ruby object of type NDTypes. */
VALUE
rb_ndtypes_wrap_ndt_object(void)
{
  NdtObject *ndt_p = NULL;

  return WRAP_NDT(cNDTypes, ndt_p);
}

/* Get pointer to the internal ndt_t object from the NDTypes Ruby object ndt. */
const ndt_t *
rb_ndtypes_const_ndt(VALUE ndt)
{
  NdtObject *ndt_p;
  
  if(!NDT_CHECK_TYPE(ndt)) {
    rb_raise(rb_eArgError, "must be NDT");
  }

  GET_NDT(ndt, ndt_p);

  return ndt_p->ndt;
}

/* Function for taking a source type and moving it accross the subtree.

   @param src NDTypes Ruby object of the source XND object.
   @param t Pointer to type of the view of XND object.
*/
VALUE
rb_ndtypes_move_subtree(VALUE src, ndt_t *t)
{
  NDT_STATIC_CONTEXT(ctx);
  VALUE dest;
  NdtObject *dest_p, *src_p;

  if (!NDT_CHECK_TYPE(src)) {
    rb_raise(rb_eArgError, "expected NDT object from view src.");
  }

  dest = NdtObject_alloc();
  
  GET_NDT(dest, dest_p);
  NDT(dest_p) = ndt_copy(t, &ctx);
  if (NDT(dest_p) == NULL) {
    rb_raise(rb_eNoMemError, "could not allocate memory for ndt_copy().");
  }

  GET_NDT(src, src_p);
  RBUF(dest_p) = RBUF(src_p);

  rb_ndtypes_gc_guard_register(dest_p, RBUF(dest_p));

  return dest;
}

/* Create NDT object from String. Returns the same object if type is NDT. 
   
   @param type String object containing description of type.
   @return New NDT object.
*/
VALUE
rb_ndtypes_from_object(VALUE type)
{
  NDT_STATIC_CONTEXT(ctx);
  VALUE copy;
  NdtObject *copy_p;
  const char *cp;
  
  if (NDT_CHECK_TYPE(type)) {
    return type;
  }

  Check_Type(type, T_STRING);

  cp = StringValuePtr(type);
  if (cp == NULL) {
    rb_raise(rb_eNoMemError,
             "error is getting C string from type in rb_ndtypes_from_object.");
  }

  copy = NdtObject_alloc();
  GET_NDT(copy, copy_p);

  RBUF(copy_p) = rbuf_allocate();
  NDT(copy_p) = ndt_from_string_fill_meta(
                                          rbuf_ndt_meta(copy),
                                          cp, &ctx);
  if (NDT(copy_p) == NULL) {
    seterr(&ctx);
    raise_error();
  }
  rb_ndtypes_gc_guard_register(copy_p, RBUF(copy_p));

  return copy;
}

VALUE
rb_ndtypes_set_error(ndt_context_t *ctx)
{
  return seterr(ctx);
}

void Init_ruby_ndtypes(void)
{
  NDT_STATIC_CONTEXT(ctx);

  /* initialize NDT internals */
  ndt_init(&ctx);

  /* define classes */
  cNDTypes = rb_define_class("NDTypes", rb_cObject);
  cNDTypes_RBuf = rb_define_class_under(cNDTypes, "RBuf", rb_cObject);
  mNDTypes_GCGuard = rb_define_module_under(cNDTypes, "GCGuard");

  /* errors */
  rb_eValueError = rb_define_class("ValueError", rb_eRuntimeError);

  /* Initializers */
  rb_define_alloc_func(cNDTypes, NDTypes_allocate);
  rb_define_method(cNDTypes, "initialize", NDTypes_initialize, -1);

  /* Instance methods */
  rb_define_method(cNDTypes, "serialize", NDTypes_serialize, 0);
  rb_define_method(cNDTypes, "ndim", NDTypes_ndim, 0);
  rb_define_method(cNDTypes, "itemsize", NDTypes_itemsize, 0);
  rb_define_method(cNDTypes, "datasize", NDTypes_datasize, 0);
  rb_define_method(cNDTypes, "align", NDTypes_align, 0);
  rb_define_method(cNDTypes, "to_s", NDTypes_to_s, 0);
  rb_define_method(cNDTypes, "hidden_dtype", NDTypes_hidden_dtype, 0);
  rb_define_method(cNDTypes, "match", NDTypes_match, 1);
  rb_define_method(cNDTypes, "ast", NDTypes_ast, 0);
  rb_define_method(cNDTypes, "pretty", NDTypes_pretty, 0);
  rb_define_method(cNDTypes, "shape", NDTypes_shape, 0);
  rb_define_method(cNDTypes, "strides", NDTypes_strides, 0);
  rb_define_method(cNDTypes, "apply", NDTypes_apply, 1);

  /* Boolean functions */
  rb_define_method(cNDTypes, "concrete?", NDTypes_ndt_is_concrete, 0);
  rb_define_method(cNDTypes, "abstract?", NDTypes_ndt_is_abstract, 0);
  rb_define_method(cNDTypes, "optional?", NDTypes_ndt_is_optional, 0);
  rb_define_method(cNDTypes, "scalar?", NDTypes_ndt_is_scalar, 0);
  rb_define_method(cNDTypes, "signed?", NDTypes_ndt_is_signed, 0);
  rb_define_method(cNDTypes, "unsigned?", NDTypes_ndt_is_unsigned, 0);
  rb_define_method(cNDTypes, "float?", NDTypes_ndt_is_float, 0);
  rb_define_method(cNDTypes, "complex?", NDTypes_ndt_is_complex, 0);
  rb_define_method(cNDTypes, "c_contiguous?", NDTypes_ndt_is_c_contiguous, 0);
  rb_define_method(cNDTypes, "f_contiguous?", NDTypes_ndt_is_f_contiguous, 0);
  rb_define_method(cNDTypes, "==", NDTypes_eqeq, 1);
  rb_define_method(cNDTypes, "!=", NDTypes_neq, 1);

  /* Class methods */
  rb_define_singleton_method(cNDTypes, "deserialize", NDTypes_s_deserialize, 1);
  rb_define_singleton_method(cNDTypes, "typedef", NDTypes_s_typedef, 2);
  rb_define_singleton_method(cNDTypes, "instantiate", NDTypes_s_instantiate, 2);
  rb_define_singleton_method(cNDTypes, "from_format", NDTypes_s_from_format, 1);

  /* Constants */
  rb_define_const(cNDTypes, "MAX_DIM", INT2NUM(NDT_MAX_DIM));

  /* GC guard init */
  rb_ndtypes_init_gc_guard();
}

