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

#include <Python.h>
#include <ndtypes.h>

static NDT_STATIC_CONTEXT(ctx);

/*
 * Type methods and attributes
 */

typedef struct
{
  PyObject_HEAD
  ndt_t *impl;
} type;

static PyObject *raise_error(ndt_context_t *ctx)
{
  char const *err = ndt_err_as_string(ctx->err);
  char buffer[128];
  snprintf(buffer, 128, "%s: %s\n", err, ndt_context_msg(ctx));
  PyErr_SetString(PyExc_RuntimeError, buffer);
  return NULL;
}


static void type_dealloc(type *self)
{
  ndt_del(self->impl);
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *type_new(PyTypeObject *t, PyObject *args, PyObject *kwds)
{
  type *self = (type *)t->tp_alloc(t, 0);
  self->impl = 0;
  /* if (self != NULL) */
  /* { */
  /* } */
  return (PyObject *)self;
}

static int type_init(type *self, PyObject *args, PyObject *kwds)
{
  int tag;

  static char *kwlist[] = {"tag", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "i:__init__", kwlist, &tag))
    return -1;
  self->impl = ndt_new(tag, &ctx);
  return 0;
}

static PyObject *type_str(type *self)
{
  char *str = ndt_as_string(self->impl, &ctx);
#if PY_MAJOR_VERSION >= 3
  PyObject *result = PyUnicode_FromString(str);
#else
  PyObject *result = PyString_FromString(str);
#endif
  return result;
}

static PyObject *type_richcompare(PyObject *self, PyObject *other, int op);
static PyObject *type_match(PyObject *self, PyObject *args);
static PyObject *type_mul(PyObject *self, PyObject *other);
static PyObject *type_next_dim(PyObject *self, PyObject *args);
static PyObject *_fixed_dim(long shape, type *base);

static PyNumberMethods type_number_methods[] =
{
  0, /* nb_add */
  0, /* nd_subtract */
  type_mul,
  0, /* nb_divide */
};


static PyMethodDef type_methods[] =
{
  {"match", type_match, METH_VARARGS, "match"},
  {"next_dim", type_next_dim, METH_VARARGS, "next dim"},
  {NULL}  /* Sentinel */
};

#define define_type_is(what)						\
static PyObject *type_is_ ## what(PyObject *self, void *closure)	\
{									\
  return PyBool_FromLong(ndt_is_ ## what(((type*)self)->impl));		\
}

define_type_is(abstract)
define_type_is(concrete)
define_type_is(signed)
define_type_is(unsigned)
define_type_is(float)
define_type_is(complex)
define_type_is(scalar)
define_type_is(array)
define_type_is(column_major)
define_type_is(contiguous)
define_type_is(c_contiguous)
define_type_is(f_contiguous)
define_type_is(optional)


static PyGetSetDef type_properties[] =
{
  {"is_abstract", type_is_abstract},
  {"is_concrete", type_is_concrete},
  {"is_signed", type_is_signed},
  {"is_unsigned", type_is_unsigned},
  {"is_float", type_is_float},
  {"is_complex", type_is_complex},
  {"is_scalar", type_is_scalar},
  {"is_array", type_is_array},
  {"is_colum_major", type_is_column_major},
  {"is_contiguous", type_is_contiguous},
  {"is_c_contiguous", type_is_c_contiguous},
  {"is_f_contiguous", type_is_f_contiguous},
  {"is_optional", type_is_optional},
  {NULL}
};

static PyTypeObject typeType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "_ndtypes.type",          /* tp_name */
  sizeof(type),             /* tp_basicsize */
  0,                        /* tp_itemsize */
  (destructor)type_dealloc, /* tp_dealloc */
  0,                        /* tp_print */
  0,                        /* tp_getattr */
  0,                        /* tp_setattr */
  0,                        /* tp_compare */
  0,                        /* tp_repr */
  type_number_methods,      /* tp_as_number */
  0,                        /* tp_as_sequence */
  0,                        /* tp_as_mapping */
  0,                        /* tp_hash */
  0,                        /* tp_call */
  (reprfunc)type_str,       /* tp_str */
  0,                        /* tp_getattro */
  0,                        /* tp_setattro */
  0,                        /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT |
#if PY_MAJOR_VERSION < 3
  Py_TPFLAGS_CHECKTYPES |
#endif
  Py_TPFLAGS_BASETYPE,      /* tp_flags */
  "type objects",           /* tp_doc */
  0,                        /* tp_traverse */
  0,                        /* tp_clear */
  (richcmpfunc)type_richcompare, /* tp_richcompare */
  0,                        /* tp_weaklistoffset */
  0,                        /* tp_iter */
  0,                        /* tp_iternext */
  type_methods,             /* tp_methods */
  0,                        /* tp_members */
  type_properties,          /* tp_getset */
  0,                        /* tp_base */
  0,                        /* tp_dict */
  0,                        /* tp_descr_get */
  0,                        /* tp_descr_set */
  0,                        /* tp_dictoffset */
  (initproc)type_init,      /* tp_init */
  0,                        /* tp_alloc */
  type_new,                 /* tp_new */
};

static PyObject *type_richcompare(PyObject *self, PyObject *other, int op)
{
  type *t = (type*)self;
  type *o;
  if (!PyObject_TypeCheck(other, &typeType))
    return NULL;
  o = (type*)other;
  switch (op)
  {
    case Py_EQ:
      return PyBool_FromLong(ndt_equal(t->impl, o->impl));
    case Py_NE:
      return PyBool_FromLong(!ndt_equal(t->impl, o->impl));
    default:
      Py_RETURN_FALSE;
  }
}

static PyObject *type_match(PyObject *self, PyObject *args)
{
  type *t = (type*)self;
  PyObject *other;
  type *o;
  if (!PyArg_ParseTuple(args, "O:match", args, &other) ||
      !PyObject_TypeCheck(other, &typeType))
    return NULL;
  o = (type*)other;
  return PyBool_FromLong(ndt_match(t->impl, o->impl, &ctx));
}

static PyObject *type_mul(PyObject *self, PyObject *other)
{
  if(!PyObject_TypeCheck(self, &typeType))
  {
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }
  return _fixed_dim(PyLong_AsLong(other), (type*)self);
}

static PyObject *type_next_dim(PyObject *self, PyObject *args)
{
  type *t = (type*)self;
  type *inst = PyObject_New(type, &typeType);
  inst->impl = ndt_next_dim(ndt_copy(t->impl, &ctx));
  return (PyObject *)inst; 
}

/*
 * Module methods and attributes
 */

static PyObject *init(PyObject *self, PyObject *args)
{
  ndt_init(&ctx);
  Py_RETURN_NONE;
}

static PyObject *finish(PyObject *self, PyObject *args)
{
  ndt_finalize();
  Py_RETURN_NONE;
}

static PyObject *any(PyObject *self)
{
  type *inst = PyObject_New(type, &typeType);
  inst->impl = ndt_any_kind(&ctx);
  return (PyObject *)inst;
}

static PyObject *_fixed_dim(long shape, type *base)
{
  type *inst = PyObject_New(type, &typeType);
  inst->impl = ndt_fixed_dim(shape, ndt_copy(base->impl, &ctx), &ctx);
  return (PyObject *)inst;
}

static PyObject *fixed_dim(PyObject *self, PyObject *args)
{
  long shape;
  PyObject *pytype;
  if (!PyArg_ParseTuple(args, "lO:fixed_dim", &shape, &pytype) ||
      !PyObject_TypeCheck(pytype, &typeType))
    return NULL;
  return _fixed_dim(shape, (type*)pytype);
}

#define declare_kind(k)				\
static PyObject *k ##_kind(PyObject *self)	\
{						\
  type *inst = PyObject_New(type, &typeType);	\
  inst->impl = ndt_##k##_kind(&ctx);		\
  return (PyObject *)inst;			\
}

declare_kind(scalar)
declare_kind(signed)
declare_kind(unsigned)
declare_kind(float)
declare_kind(complex)
declare_kind(fixed_string)
declare_kind(fixed_bytes)

static PyObject *primitive(PyObject *self, PyObject *args)
{
  int tag;
  char endian;
  type *inst;
  if (!PyArg_ParseTuple(args, "lc:primitive", &tag, &endian))
    return NULL;
  inst = PyObject_New(type, &typeType);
  inst->impl = ndt_primitive(tag, endian, &ctx);
  return (PyObject *)inst;
}

static PyObject *_signed(PyObject *self, PyObject *args)
{
  int tag;
  char endian;
  type *inst;
  if (!PyArg_ParseTuple(args, "lc:signed", &tag, &endian))
    return NULL;
  inst = PyObject_New(type, &typeType);
  // TODO: validate endian
  inst->impl = ndt_signed(tag, endian, &ctx);
  if (ndt_err_isset(&ctx))
    return raise_error(&ctx);
  return (PyObject *)inst;
}

static PyObject *_unsigned(PyObject *self, PyObject *args)
{
  int tag;
  char endian;
  type *inst;
  if (!PyArg_ParseTuple(args, "lc:unsigned", &tag, &endian))
    return NULL;
  inst = PyObject_New(type, &typeType);
  inst->impl = ndt_unsigned(tag, endian, &ctx);
  return (PyObject *)inst;
}

static PyObject *string(PyObject *self)
{
  type *inst = PyObject_New(type, &typeType);
  inst->impl = ndt_string(&ctx);
  return (PyObject *)inst;
}

static PyObject *from_file(PyObject *self, PyObject *args)
{
  char *filename;
  type *inst;
  if (!PyArg_ParseTuple(args, "s:from_file", &filename))
    return NULL;
  inst = PyObject_New(type, &typeType);
  inst->impl = ndt_from_file(filename, &ctx);
  return (PyObject *)inst;
}

static PyObject *from_string(PyObject *self, PyObject *args)
{
  char *string;
  type *inst;
  if (!PyArg_ParseTuple(args, "s:from_string", &string))
    return NULL;
  inst = PyObject_New(type, &typeType);
  inst->impl = ndt_from_string(string, &ctx);
  return (PyObject *)inst;
}


static PyMethodDef methods[] =
{
  {"init", init, METH_VARARGS, "initialization"},
  {"finish", finish, METH_VARARGS, "finalization"},
  {"any", (PyCFunction)any, METH_NOARGS},
  {"fixed_dim", fixed_dim, METH_VARARGS},
  {"scalar_kind", (PyCFunction)scalar_kind, METH_NOARGS},
  {"signed_kind", (PyCFunction)signed_kind, METH_NOARGS},
  {"unsigned_kind", (PyCFunction)unsigned_kind, METH_NOARGS},
  {"float_kind", (PyCFunction)float_kind, METH_NOARGS},
  {"complex_kind", (PyCFunction)complex_kind, METH_NOARGS},
  {"fixed_string_kind", (PyCFunction)fixed_string_kind, METH_NOARGS},
  {"fixed_bytes_kind", (PyCFunction)fixed_bytes_kind, METH_NOARGS},
  {"primitive", primitive, METH_VARARGS},
  {"signed", _signed, METH_VARARGS},
  {"unsigned", _unsigned, METH_VARARGS},
  {"string", (PyCFunction)string, METH_NOARGS},
  {"from_file", from_file, METH_VARARGS},
  {"from_string", from_string, METH_VARARGS},
  {NULL, NULL, 0, NULL}
};

#define define_constant(N, V) if (PyModule_AddIntConstant(module, N, V)) return -1

int define_constants(PyObject *module)
{
  /* encoding */
  define_constant("_ascii", Ascii);
  define_constant("_utf8", Utf8);
  define_constant("_utf16", Utf16);
  define_constant("_utf32", Utf32);
  define_constant("_ucs2", Ucs2);
  define_constant("_error_encoding", ErrorEncoding);
  /* dimension data types */
  define_constant("_dim_none", DimNone);
  define_constant("_dim_uint8", DimUint8);
  define_constant("_dim_uint16", DimUint16);
  define_constant("_dim_uint32", DimUint32);
  define_constant("_dim_int32", DimInt32);
  define_constant("_dim_int64", DimInt64);
  /* datashape kinds */
  define_constant("_any_kind", AnyKind);
  define_constant("_fixed_dim", FixedDim);
  define_constant("_symbolic_dim", SymbolicDim);
  define_constant("_var_dim", VarDim);
  define_constant("_ellipsis_dim", EllipsisDim);
  define_constant("_array", Array);
  define_constant("_option", Option);
  define_constant("_option_item", OptionItem);
  define_constant("_nominal", Nominal);
  define_constant("_constr", Constr);

  return 0;
}


#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef moduledef =
{
  PyModuleDef_HEAD_INIT,
  "_ndtypes",
  0, /* doc      */
  -1, //sizeof(struct module_state),
  methods,
  0, /* reload   */
  0, /* traverse */
  0, /* clear    */
  0  /* free     */
};

PyMODINIT_FUNC PyInit__ndtypes()
#else
void init_ndtypes()
#endif
{
  PyObject *module;
#if PY_MAJOR_VERSION >= 3
  if (PyType_Ready(&typeType) < 0)
    return NULL;
  module = PyModule_Create(&moduledef);
  if (define_constants(module) == -1)
  {
    Py_XDECREF(module);
    module = NULL;
    return NULL;
  }
  Py_INCREF(&typeType);
  PyModule_AddObject(module, "type", (PyObject *)&typeType);
  return module;
#else
  if (PyType_Ready(&typeType) < 0)
    return;
  module = Py_InitModule("_ndtypes", methods);
  if (define_constants(module) == -1)
  {
    Py_XDECREF(module);
    module = NULL;
    return;
  }
  Py_INCREF(&typeType);
  PyModule_AddObject(module, "type", (PyObject *)&typeType);
#endif
}
