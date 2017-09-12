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
#include <stdlib.h>
#include "ndtypes.h"


#ifdef _MSC_VER
  #ifndef UNUSED
    #define UNUSED
  #endif
#else
  #if defined(__GNUC__) && !defined(__INTEL_COMPILER)
    #define UNUSED __attribute__((unused))
  #else
    #define UNUSED
  #endif
#endif


typedef struct {
    PyObject_HEAD
    ndt_t *ndt;
} NdtObject;

#undef NDT
static PyTypeObject Ndt_Type;
#define Ndt_CheckExact(v) (Py_TYPE(v) == &Ndt_Type)
#define Ndt_Check(v) PyObject_TypeCheck(v, &Ndt_Type)
#define NDT(v) (((NdtObject *)v)->ndt)


static PyTypeObject *ApplySpec = NULL;

static PyObject *
seterr(ndt_context_t *ctx)
{
    PyObject *exc = PyExc_RuntimeError;

    switch (ctx->err) {
    case NDT_Success: /* should never be set on error */
        exc = PyExc_RuntimeError;
        break;
    case NDT_ValueError:
        exc = PyExc_ValueError;
        break;
    case NDT_TypeError:
        exc = PyExc_TypeError;
        break;
    case NDT_InvalidArgumentError:
        exc = PyExc_ValueError;
        break;
    case NDT_NotImplementedError:
        exc = PyExc_NotImplementedError;
        break;
    case NDT_LexError: case NDT_ParseError:
        exc = PyExc_ValueError;
        break;
    case NDT_OSError:
        exc = PyExc_OSError;
        break;
    case NDT_RuntimeError:
        exc = PyExc_RuntimeError;
        break;
    case NDT_MemoryError:
        exc = PyExc_MemoryError;
        break;
    }

    PyErr_SetString(exc, ndt_context_msg(ctx));
    ndt_context_del(ctx);

    return NULL;
};

static PyObject *
ndtype_alloc(PyTypeObject *type)
{
    NdtObject *t;

    if (type == &Ndt_Type) {
        t = PyObject_New(NdtObject, &Ndt_Type);
    }
    else {
        t = (NdtObject *)type->tp_alloc(type, 0);
    }
    if (t == NULL) {
        return NULL;
    }
 
    NDT(t) = NULL;
    return (PyObject *)t;
}

static void
ndtype_dealloc(PyObject *t)
{
    ndt_del(NDT(t));
    Py_TYPE(t)->tp_free(t);
}

static PyObject *
ndtype_new(PyTypeObject *type, PyObject *args, PyObject *kwds UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *t;
    PyObject *v;
    const char *cp;

    if (!PyArg_ParseTuple(args, "O", &v)) {
        return NULL;
    }

    cp = PyUnicode_AsUTF8(v);
    if (cp == NULL) {
        return NULL;
    }

    t = ndtype_alloc(type);
    if (t == NULL) {
        return NULL;
    }

    NDT(t) = ndt_from_string(cp, &ctx);
    if (NDT(t) == NULL) {
        Py_DECREF(t);
        return seterr(&ctx);
    }

    return t;
}

static PyObject *
ndtype_str(PyObject *t)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string(NDT(t), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_repr(PyObject *t)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string(NDT(t), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromFormat("ndt(\"%s\")", cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_indent(PyObject *t, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_indent(NDT(t), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_ast_repr(PyObject *t, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string_with_meta(NDT(t), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

/******************************************************************************/
/*                                 Ndt methods                                */
/******************************************************************************/

#define Ndtype_BoolFunc(NDTFUNC) \
static PyObject *                                       \
ndtype_##NDTFUNC(PyObject *self, PyObject *args UNUSED) \
{                                                       \
    if (NDTFUNC(NDT(self))) {                           \
        Py_RETURN_TRUE;                                 \
    }                                                   \
    Py_RETURN_FALSE;                                    \
}

#define Ndtype_ArrayBoolFunc(NDTFUNC) \
static PyObject *                                       \
ndtype_##NDTFUNC(PyObject *self, PyObject *args UNUSED) \
{                                                       \
    if (!ndt_is_array(NDT(self))) {                     \
        PyErr_SetString(PyExc_ValueError,               \
            "argument must be an array type");          \
            return NULL;                                \
    }                                                   \
                                                        \
    if (NDTFUNC(NDT(self))) {                           \
        Py_RETURN_TRUE;                                 \
    }                                                   \
    Py_RETURN_FALSE;                                    \
}

Ndtype_BoolFunc(ndt_is_abstract)
Ndtype_BoolFunc(ndt_is_concrete)
Ndtype_BoolFunc(ndt_is_signed)
Ndtype_BoolFunc(ndt_is_unsigned)
Ndtype_BoolFunc(ndt_is_float)
Ndtype_BoolFunc(ndt_is_complex)
Ndtype_BoolFunc(ndt_is_scalar)
Ndtype_BoolFunc(ndt_is_array)
Ndtype_BoolFunc(ndt_is_optional)

Ndtype_ArrayBoolFunc(ndt_is_contiguous)
Ndtype_ArrayBoolFunc(ndt_is_c_contiguous)
Ndtype_ArrayBoolFunc(ndt_is_f_contiguous)

static PyObject *
ndtype_richcompare(PyObject *v, PyObject *w, int op)
{
    int r = ndt_equal(NDT(v), NDT(w));

    switch (op) {
    case Py_EQ:
        return PyBool_FromLong(r);
    case Py_NE:
        return PyBool_FromLong(!r);
    default:
        PyErr_Format(PyExc_TypeError,
            "unorderable type: '%.200s'", Py_TYPE(v)->tp_name);
        return NULL;
    }
}

static Py_hash_t
ndtype_hash(PyObject *self)
{
    NDT_STATIC_CONTEXT(ctx);
    Py_hash_t res;

    res = ndt_hash(NDT(self), &ctx);
    if (res == -1) {
        (void)seterr(&ctx);
    }

    return res;
}

static PyObject *
ndtype_match(PyObject *self, PyObject *other)
{
    NDT_STATIC_CONTEXT(ctx);
    int res;

    if (!Ndt_Check(other)) {
        PyErr_SetString(PyExc_TypeError, "argument must be 'ndt'");
        return NULL;
    }

    res = ndt_match(NDT(self), NDT(other), &ctx);
    if (res == -1) {
        return seterr(&ctx);
    }

    return PyBool_FromLong(res);
}

static PyObject *
ndtype_apply(PyObject *self, PyObject *other)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *return_type, *outer_dims;
    ndt_t *t;
    int k = -1;

    if (!Ndt_Check(other)) {
        PyErr_SetString(PyExc_TypeError, "argument must be 'ndt'");
        return NULL;
    }

    t = ndt_typecheck(NDT(self), NDT(other), &k, &ctx);
    if (t == NULL) {
        return seterr(&ctx);
    }

    return_type = ndtype_alloc(&Ndt_Type);
    if (return_type == NULL) {
        ndt_del(t);
        return NULL;
    }
    NDT(return_type) = t;

    outer_dims = PyLong_FromLong(k);
    if (outer_dims == NULL) {
        Py_DECREF(return_type);
        return NULL;
    }

    return PyObject_CallFunctionObjArgs((PyObject *)ApplySpec, self, other,
                                        return_type, outer_dims, NULL);
}

static PyMethodDef ndtype_methods [] =
{
  /* Boolean functions */
  { "is_abstract", (PyCFunction)ndtype_ndt_is_abstract, METH_NOARGS, NULL },
  { "is_concrete", (PyCFunction)ndtype_ndt_is_concrete, METH_NOARGS, NULL },
  { "is_signed", (PyCFunction)ndtype_ndt_is_signed, METH_NOARGS, NULL },
  { "is_unsigned", (PyCFunction)ndtype_ndt_is_unsigned, METH_NOARGS, NULL },
  { "is_float", (PyCFunction)ndtype_ndt_is_float, METH_NOARGS, NULL },
  { "is_complex", (PyCFunction)ndtype_ndt_is_complex, METH_NOARGS, NULL },
  { "is_scalar", (PyCFunction)ndtype_ndt_is_scalar, METH_NOARGS, NULL },
  { "is_array", (PyCFunction)ndtype_ndt_is_array, METH_NOARGS, NULL },
  { "is_optional", (PyCFunction)ndtype_ndt_is_optional, METH_NOARGS, NULL },

  /* Boolean functions on array types */
  { "is_contiguous", (PyCFunction)ndtype_ndt_is_contiguous, METH_NOARGS, NULL },
  { "is_c_contiguous", (PyCFunction)ndtype_ndt_is_c_contiguous, METH_NOARGS, NULL },
  { "is_f_contiguous", (PyCFunction)ndtype_ndt_is_f_contiguous, METH_NOARGS, NULL },

  /* Binary functions */
  { "match", (PyCFunction)ndtype_match, METH_O, NULL },
  { "apply", (PyCFunction)ndtype_apply, METH_O, NULL },

  /* Other functions */
  { "pretty", (PyCFunction)ndtype_indent, METH_NOARGS, NULL },
  { "ast_repr", (PyCFunction)ndtype_ast_repr, METH_NOARGS, NULL },

  { NULL, NULL, 1 }

};

static PyTypeObject Ndt_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    "ndtypes.ndt",                          /* tp_name */
    sizeof(NdtObject),                      /* tp_basicsize */
    0,                                      /* tp_itemsize */
    (destructor) ndtype_dealloc,            /* tp_dealloc */
    0,                                      /* tp_print */
    (getattrfunc) 0,                        /* tp_getattr */
    (setattrfunc) 0,                        /* tp_setattr */
    0,                                      /* tp_reserved */
    (reprfunc) ndtype_repr,                 /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    ndtype_hash,                            /* tp_hash */
    0,                                      /* tp_call */
    (reprfunc) ndtype_str,                  /* tp_str */
    (getattrofunc) PyObject_GenericGetAttr, /* tp_getattro */
    (setattrofunc) 0,                       /* tp_setattro */
    (PyBufferProcs *) 0,                    /* tp_as_buffer */
    (Py_TPFLAGS_DEFAULT|
     Py_TPFLAGS_BASETYPE),                  /* tp_flags */
    0, // ndtypes_doc,                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    ndtype_richcompare,                     /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    ndtype_methods,                         /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0,                                      /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    ndtype_new,                             /* tp_new */
    PyObject_Del,                           /* tp_free */
};

static int num_modules = 0;

static void
ndtypes_module_free(PyObject *m UNUSED)
{
    if (--num_modules == 0) {
        ndt_finalize();
    }
}

static struct PyModuleDef ndtypes_module = {
    PyModuleDef_HEAD_INIT,        /* m_base */
    "_ndtypes",                   /* m_name */
    NULL,                         /* m_doc */
    -1,                           /* m_size */
    NULL,                         /* m_methods */
    NULL,                         /* m_slots */
    NULL,                         /* m_traverse */
    NULL,                         /* m_clear */
    (freefunc)ndtypes_module_free /* m_free */
};


PyMODINIT_FUNC
PyInit__ndtypes(void)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *m = NULL;
    PyObject *collections = NULL;
    PyObject *obj = NULL;

    if (num_modules == 0) {
        if (ndt_init(&ctx) < 0) {
            return seterr(&ctx);
        }
    }

    Ndt_Type.tp_base = &PyBaseObject_Type;
    if (PyType_Ready(&Ndt_Type) < 0) {
        goto error;
    }

    /* ApplySpec */
    collections = PyImport_ImportModule("collections");
    if (collections == NULL) {
        goto error;
    }

    ApplySpec = (PyTypeObject *)PyObject_CallMethod(collections,
                                    "namedtuple", "(ss)", "ApplySpec",
                                    "func args ret outer_dims");
    if (ApplySpec == NULL) {
        goto error;
    }

    obj = PyUnicode_FromString("ndtypes");
    if (obj == NULL) {
        goto error;
    }

    if (PyDict_SetItemString(ApplySpec->tp_dict, "__module__", obj) < 0) {
        goto error;
    }
    Py_CLEAR(obj);


    m = PyModule_Create(&ndtypes_module);
    if (m == NULL) {
        goto error;
    }

    Py_INCREF(&Ndt_Type);
    if (PyModule_AddObject(m, "ndt", (PyObject *)&Ndt_Type) < 0) {
        goto error;
    }

    Py_INCREF(ApplySpec);
    if (PyModule_AddObject(m, "ApplySpec", (PyObject *)ApplySpec) < 0) {
        goto error;
    }


    num_modules++;
    return m;

error:
    ndt_finalize();
    Py_CLEAR(m);
    return NULL;
}


