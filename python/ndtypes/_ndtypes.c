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
#include "pyndtypes.h"


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


/****************************************************************************/
/*                           Resource Buffer Object                         */
/****************************************************************************/

/* This object handles resources like offsets that are shared by several
   ndt objects. It is never exposed to the Python level.

   At a later stage, the object will need to communicate with Arrow
   or other formats to store external resources. */

typedef struct {
    PyObject_HEAD
    uint32_t flags;
    int32_t num_offset_arrays;           /* number of offset arrays */
    int32_t num_offsets[NDT_MAX_DIM];    /* lengths of the offset arrays */
    int32_t *offset_arrays[NDT_MAX_DIM]; /* offset arrays (NULL or valid pointer) */
} ResourceBufferObject;

static PyTypeObject ResourceBuffer_Type;

#define RBUF_OWN_OFFSETS      0x00000001U
#define RBUF_EXTERNAL_OFFSETS 0x00000002U

static PyObject *
rbuf_alloc(uint32_t flags)
{
    ResourceBufferObject *self;
    int i;

    self = PyObject_New(ResourceBufferObject, &ResourceBuffer_Type);
    if (self == NULL) {
        return NULL;
    }
 
    self->flags = flags;
    self->num_offset_arrays = 0;

    for (i = 0; i < NDT_MAX_DIM; i++) {
        self->num_offsets[i] = 0;
        self->offset_arrays[i] = NULL;
    }

    return (PyObject *)self;
}

static void
rbuf_dealloc(ResourceBufferObject *self)
{
    int i;

    if (self->flags & RBUF_OWN_OFFSETS) {
        for (i = 0; i < NDT_MAX_DIM; i++) {
            ndt_free(self->offset_arrays[i]);
            self->offset_arrays[i] = NULL;
        }
    }

    PyObject_Del(self);
}

static int
rbuf_init_from_offset_list(ResourceBufferObject *rbuf, PyObject *list)
{
    PyObject *lst;
    Py_ssize_t i, k;

    if (!PyList_Check(list)) {
        PyErr_SetString(PyExc_TypeError, "expected a list of offset lists");
        return -1;
    }

    rbuf->num_offset_arrays = PyList_GET_SIZE(list);
    if (rbuf->num_offset_arrays < 1 || rbuf->num_offset_arrays > NDT_MAX_DIM) {
        PyErr_Format(PyExc_ValueError,
            "number of offset lists must be in [1, %d]", NDT_MAX_DIM);
        return -1;
    }

    for (i = 0; i < rbuf->num_offset_arrays; i++) {
        lst = PyList_GET_ITEM(list, i);
        if (!PyList_Check(lst)) {
            PyErr_SetString(PyExc_TypeError,
                "expected a list of offset lists");
            return -1;
        }

        rbuf->num_offsets[i] = PyList_GET_SIZE(lst);
        if (rbuf->num_offsets[i] < 2 || rbuf->num_offsets[i] > INT32_MAX) {
            PyErr_SetString(PyExc_ValueError,
                "length of a single offset list must be in [2, INT32_MAX]");
            return -1;
        }

        rbuf->offset_arrays[i] = ndt_alloc(rbuf->num_offsets[i], sizeof(int32_t));
        if (rbuf->offset_arrays[i] == NULL) {
            PyErr_NoMemory();
            return -1;
        }

        for (k = 0; k < rbuf->num_offsets[i]; k++) {
            long x = PyLong_AsLong(PyList_GET_ITEM(lst, k));
            if (x == -1 && PyErr_Occurred()) {
                return -1;
            }

            if (x < 0 || x > INT32_MAX) {
                PyErr_SetString(PyExc_ValueError,
                    "offset must be in [0, INT32_MAX]");
                return -1;
            }

            rbuf->offset_arrays[i][k] = x;
        }
    }

    return 0;
}

static PyObject *
rbuf_from_offset_lists(PyObject *list)
{
    PyObject *rbuf;

    rbuf = rbuf_alloc(RBUF_OWN_OFFSETS);
    if (rbuf == NULL) {
        return NULL;
    }

    if (rbuf_init_from_offset_list((ResourceBufferObject *)rbuf, list) < 0) {
        Py_DECREF(rbuf);
        return NULL;
    }

    return rbuf;
}

static PyTypeObject ResourceBuffer_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "ndtypes.resource",
    sizeof(ResourceBufferObject),
    0,
    (destructor)rbuf_dealloc,     /* tp_dealloc */
    0,                            /* tp_print */
    0,                            /* tp_getattr */
    0,                            /* tp_setattr */
    0,                            /* tp_reserved */
    0,                            /* tp_repr */
    0,                            /* tp_as_number */
    0,                            /* tp_as_sequence */
    0,                            /* tp_as_mapping */
    0,                            /* tp_hash */
    0,                            /* tp_call */
    0,                            /* tp_str */
    PyObject_GenericGetAttr,      /* tp_getattro */
    0,                            /* tp_setattro */
    0,                            /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,           /* tp_flags */
};


/****************************************************************************/
/*                               Cached objects                             */
/****************************************************************************/

static PyTypeObject *ApplySpec = NULL;


/****************************************************************************/
/*                               Error handling                             */
/****************************************************************************/

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


/****************************************************************************/
/*                                 ndt object                               */
/****************************************************************************/

typedef struct {
    PyObject_HEAD
    PyObject *rbuf; /* resource buffer */
    ndt_t *ndt;     /* type */
} NdtObject;

static PyTypeObject Ndt_Type;

#define Ndt_CheckExact(v) (Py_TYPE(v) == &Ndt_Type)
#define Ndt_Check(v) PyObject_TypeCheck(v, &Ndt_Type)
#define NDT(v) (((NdtObject *)v)->ndt)

#define RBUF(v) (((NdtObject *)v)->rbuf)
#define RBUF_NUM_OFFSET_ARRAYS(v) (((ResourceBufferObject *)(((NdtObject *)v)->rbuf))->num_offset_arrays)
#define RBUF_NUM_OFFSETS(v) (((ResourceBufferObject *)(((NdtObject *)v)->rbuf))->num_offsets)
#define RBUF_OFFSET_ARRAYS(v) (((ResourceBufferObject *)(((NdtObject *)v)->rbuf))->offset_arrays)


static PyObject *
ndtype_alloc(PyTypeObject *type)
{
    NdtObject *self;

    if (type == &Ndt_Type) {
        self = PyObject_New(NdtObject, &Ndt_Type);
    }
    else {
        self = (NdtObject *)type->tp_alloc(type, 0);
    }
    if (self == NULL) {
        return NULL;
    }
 
    RBUF(self) = NULL;
    NDT(self) = NULL;
    return (PyObject *)self;
}

static void
ndtype_dealloc(PyObject *self)
{
    Py_CLEAR(RBUF(self));
    ndt_del(NDT(self));
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
ndtype_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"type", "offsets", NULL};
    NDT_STATIC_CONTEXT(ctx);
    PyObject *offsets = Py_None;
    PyObject *self, *s;
    const char *cp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O", kwlist, &s, &offsets)) {
        return NULL;
    }

    cp = PyUnicode_AsUTF8(s);
    if (cp == NULL) {
        return NULL;
    }

    self = ndtype_alloc(type);
    if (self == NULL) {
        return NULL;
    }

    if (offsets == Py_None) {
        NDT(self) = ndt_from_string(cp, &ctx);
        if (NDT(self) == NULL) {
            Py_DECREF(self);
            return seterr(&ctx);
        }
        return self;
    }

    RBUF(self) = rbuf_from_offset_lists(offsets);
    if (RBUF(self) == NULL) {
        Py_DECREF(self);
        return NULL;
    }

    NDT(self) = ndt_from_offsets_dtype(
                    ExternalOffsets,
                    RBUF_NUM_OFFSET_ARRAYS(self),
                    RBUF_NUM_OFFSETS(self),
                    RBUF_OFFSET_ARRAYS(self),
                    cp,
                    &ctx);

    if (NDT(self) == NULL) {
        Py_DECREF(self);
        return seterr(&ctx);
    }

    return self;
}

static PyObject *
ndtype_str(PyObject *self)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_repr(PyObject *self)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromFormat("ndt(\"%s\")", cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_indent(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_indent(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_ast_repr(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_as_string_with_meta(NDT(self), &ctx);
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
ndtype_richcompare(PyObject *self, PyObject *other, int op)
{
    int r = 0;

    assert(Ndt_Check(self));

    if (Ndt_Check(other)) {
        r = ndt_equal(NDT(self), NDT(other));
    }

    switch (op) {
    case Py_EQ:
        return PyBool_FromLong(r);
    case Py_NE:
        return PyBool_FromLong(!r);
    default:
        Py_RETURN_NOTIMPLEMENTED;
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


/****************************************************************************/
/*                                   C-API                                  */
/****************************************************************************/

int
pyndt_check_exact(PyObject *v)
{
    return Ndt_CheckExact(v);
}

int
pyndt_check(PyObject *v)
{
    return Ndt_Check(v);
}

ndt_t *
pyndt_get_ndt(PyObject *v)
{
    assert(Ndt_Check(v));
    return NDT(v);
}


/****************************************************************************/
/*                                  Module                                  */
/****************************************************************************/

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

    if (PyModule_AddIntConstant(m, "MAX_DIM", NDT_MAX_DIM) < 0) {
        goto error;
    }

    num_modules++;
    return m;

error:
    ndt_finalize();
    Py_CLEAR(m);
    return NULL;
}


