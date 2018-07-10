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


#include <Python.h>
#include <stdlib.h>
#include "ndtypes.h"
#include "docstrings.h"

#define NDTYPES_MODULE
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

static PyObject *seterr(ndt_context_t *ctx);

typedef struct {
    PyObject_HEAD
    ndt_meta_t *m;
} ResourceBufferObject;

static PyTypeObject ResourceBuffer_Type;

static PyObject *
rbuf_alloc(void)
{
    NDT_STATIC_CONTEXT(ctx);
    ResourceBufferObject *self;

    self = (ResourceBufferObject *)
        PyObject_GC_New(ResourceBufferObject, &ResourceBuffer_Type);
    if (self == NULL) {
        return NULL;
    }
 
    self->m = ndt_meta_new(&ctx);
    if (self->m == NULL) {
        return seterr(&ctx);
    }

    PyObject_GC_Track(self);
    return (PyObject *)self;
}

static int
rbuf_traverse(ResourceBufferObject *self UNUSED, visitproc visit UNUSED,
              void *arg UNUSED)
{
    return 0;
}

static void
rbuf_dealloc(ResourceBufferObject *self)
{
    ndt_meta_del(self->m);
    self->m = NULL;
    PyObject_GC_UnTrack(self);
    PyObject_GC_Del(self);
}

static int
rbuf_init_from_offset_list(ResourceBufferObject *rbuf, PyObject *list)
{
    ndt_meta_t * const m = rbuf->m;
    PyObject *lst;

    if (!PyList_Check(list)) {
        PyErr_SetString(PyExc_TypeError, "expected a list of offset lists");
        return -1;
    }

    const int64_t n = PyList_GET_SIZE(list);
    if (n < 1 || n > NDT_MAX_DIM) {
        PyErr_Format(PyExc_ValueError,
            "number of offset lists must be in [1, %d]", NDT_MAX_DIM);
        return -1;
    }

    m->ndims = 0;
    for (int64_t i = n-1; i >= 0; i--) {
        lst = PyList_GET_ITEM(list, i);
        if (!PyList_Check(lst)) {
            PyErr_SetString(PyExc_TypeError,
                "expected a list of offset lists");
            return -1;
        }

        const int64_t noffsets = PyList_GET_SIZE(lst);
        if (noffsets < 2 || noffsets > INT32_MAX) {
            PyErr_SetString(PyExc_ValueError,
                "length of a single offset list must be in [2, INT32_MAX]");
            return -1;
        }

        int32_t * const offsets = ndt_alloc(noffsets, sizeof(int32_t));
        if (offsets == NULL) {
            PyErr_NoMemory();
            return -1;
        }

        for (int32_t k = 0; k < noffsets; k++) {
            long long x = PyLong_AsLongLong(PyList_GET_ITEM(lst, k));
            if (x == -1 && PyErr_Occurred()) {
                ndt_free(offsets);
                return -1;
            }

            if (x < 0 || x > INT32_MAX) {
                ndt_free(offsets);
                PyErr_SetString(PyExc_ValueError,
                    "offset must be in [0, INT32_MAX]");
                return -1;
            }

            offsets[k] = (int32_t)x;
        }

        m->noffsets[m->ndims] = (int32_t)noffsets;
        m->offsets[m->ndims] = offsets;
        m->ndims++;
    }

    return 0;
}

static PyObject *
rbuf_from_offset_lists(PyObject *list)
{
    PyObject *rbuf;

    rbuf = rbuf_alloc();
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
    .tp_name = "_ndtypes.resource",
    .tp_basicsize = sizeof(ResourceBufferObject),
    .tp_dealloc = (destructor)rbuf_dealloc,
    .tp_getattro = PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,
    .tp_traverse = (traverseproc)rbuf_traverse
};


/****************************************************************************/
/*                               Cached objects                             */
/****************************************************************************/

static PyObject *Deserialize = NULL;
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
    case NDT_IndexError:
        exc = PyExc_IndexError;
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

static PyTypeObject Ndt_Type;

static PyObject *
ndtype_alloc(PyTypeObject *type)
{
    NdtObject *self;

    self = (NdtObject *)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }
 
    RBUF(self) = NULL;
    NDT(self) = NULL;

    return (PyObject *)self;
}

static int
ndtype_traverse(NdtObject *self, visitproc visit, void *arg)
{
    Py_VISIT(self->rbuf);
    return 0;
}

static void
ndtype_dealloc(NdtObject *self)
{
    PyObject_GC_UnTrack(self);
    ndt_del(NDT(self));
    Py_CLEAR(self->rbuf);
    Py_TYPE(self)->tp_free(self);
}

static PyObject *
ndtype_from_object(PyTypeObject *tp, PyObject *type)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *self;
    const char *cp;

    if (Ndt_Check(type)) {
        Py_INCREF(type);
        return type;
    }

    cp = PyUnicode_AsUTF8(type);
    if (cp == NULL) {
        return NULL;
    }

    self = ndtype_alloc(tp);
    if (self == NULL) {
        return NULL;
    }

    RBUF(self) = rbuf_alloc();
    if (RBUF(self) == NULL) {
        Py_DECREF(self);
        return NULL;
    }

    NDT(self) = ndt_from_string_fill_meta(RBUF_NDT_META(self), cp, &ctx);
    if (NDT(self) == NULL) {
        Py_DECREF(self);
        return seterr(&ctx);
    }

    return self;
}

static PyObject *
ndtype_from_format(PyTypeObject *tp, PyObject *format)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *self;
    const char *cp;

    cp = PyUnicode_AsUTF8(format);
    if (cp == NULL) {
        return NULL;
    }

    self = ndtype_alloc(tp);
    if (self == NULL) {
        return NULL;
    }

    NDT(self) = ndt_from_bpformat(cp, &ctx);
    if (NDT(self) == NULL) {
        Py_DECREF(self);
        return seterr(&ctx);
    }

    return self;
}

static PyObject *
ndtype_to_format(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *ret;
    char *cp;

    cp = ndt_to_bpformat(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    ret = PyUnicode_FromString(cp);
    ndt_free(cp);

    return ret;
}

static PyObject *
ndtype_from_offsets_and_dtype(PyTypeObject *tp, PyObject *offsets, PyObject *dtype)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *self;
    const char *cp;

    cp = PyUnicode_AsUTF8(dtype);
    if (cp == NULL) {
        return NULL;
    }

    self = ndtype_alloc(tp);
    if (self == NULL) {
        return NULL;
    }

    RBUF(self) = rbuf_from_offset_lists(offsets);
    if (RBUF(self) == NULL) {
        Py_DECREF(self);
        return NULL;
    }

    NDT(self) = ndt_from_metadata_and_dtype(RBUF_NDT_META(self), cp, &ctx);

    if (NDT(self) == NULL) {
        Py_DECREF(self);
        return seterr(&ctx);
    }

    return self;
}

static PyObject *
ndtype_deserialize(PyTypeObject *tp, PyObject *bytes)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *self;

    if (!PyBytes_Check(bytes)) {
        PyErr_SetString(PyExc_TypeError, "expected bytes object");
        return NULL;
    }

    self = ndtype_alloc(tp);
    if (self == NULL) {
        return NULL;
    }

    RBUF(self) = rbuf_alloc();
    if (RBUF(self) == NULL) {
        Py_DECREF(self);
        return NULL;
    }

    NDT(self) = ndt_deserialize(RBUF_NDT_META(self), PyBytes_AS_STRING(bytes),
                                PyBytes_GET_SIZE(bytes), &ctx);
    if (NDT(self) == NULL) {
        Py_DECREF(self);
        return seterr(&ctx);
    }

    return self;
}

static PyObject *
ndtype_new(PyTypeObject *tp, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"type", "offsets", NULL};
    PyObject *offsets = Py_None;
    PyObject *type;

    if (!PyArg_ParseTupleAndKeywords(
             args, kwds, "O|O", kwlist, &type, &offsets)) {
        return NULL;
    }

    if (offsets == Py_None) {
        return ndtype_from_object(tp, type);
    }

    return ndtype_from_offsets_and_dtype(tp, offsets, type);
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
ndtype_pformat(PyObject *self, PyObject *args UNUSED)
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
ndtype_pprint(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    char *cp;

    cp = ndt_indent(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    printf("%s\n", cp);
    fflush(stdout);

    ndt_free(cp);
    Py_RETURN_NONE;
}

static PyObject *
ndtype_ast_repr(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *cp;

    cp = ndt_ast_repr(NDT(self), &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    res = PyUnicode_FromString(cp);
    ndt_free(cp);
    return res;
}

static PyObject *
ndtype_copy(PyObject *self, PyObject *args UNUSED)
{
    Py_INCREF(self);
    return self;
}

static PyObject *
ndtype_serialize(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *res;
    char *bytes;
    int64_t size;

    size = ndt_serialize(&bytes, NDT(self), &ctx);
    if (size < 0) {
        return seterr(&ctx);
    }

    res = PyBytes_FromStringAndSize(bytes, (Py_ssize_t)size);
    ndt_free(bytes);
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

Ndtype_BoolFunc(ndt_is_abstract)
Ndtype_BoolFunc(ndt_is_concrete)

Ndtype_BoolFunc(ndt_is_optional)
Ndtype_BoolFunc(ndt_is_scalar)
Ndtype_BoolFunc(ndt_is_signed)
Ndtype_BoolFunc(ndt_is_unsigned)
Ndtype_BoolFunc(ndt_is_float)
Ndtype_BoolFunc(ndt_is_complex)

Ndtype_BoolFunc(ndt_is_c_contiguous)
Ndtype_BoolFunc(ndt_is_f_contiguous)


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
ndtype_apply(PyObject *self, PyObject *args)
{
    NDT_STATIC_CONTEXT(ctx);
    ndt_t *sig = NDT(self);
    const ndt_t *in[NDT_MAX_ARGS];
    ndt_apply_spec_t spec;
    PyObject *flags = NULL, *out = NULL, *broadcast = NULL, *outer_dims = NULL;
    PyObject *res = NULL;
    Py_ssize_t nin;
    Py_ssize_t i;

    if (!PyTuple_Check(args) && !PyList_Check(args)) {
        PyErr_SetString(PyExc_TypeError, "arguments must be a tuple or a list");
        return NULL;
    }

    nin = PySequence_Fast_GET_SIZE(args);
    if (nin > NDT_MAX_ARGS) {
        PyErr_Format(PyExc_ValueError,
            "maximum number of arguments is %d", NDT_MAX_ARGS);
        return NULL;
    }

    for (i = 0; i < nin; i++) {
        PyObject *tmp = PySequence_Fast_GET_ITEM(args, i);
        if (!Ndt_Check(tmp)) {
            PyErr_Format(PyExc_TypeError, "argument types must be ndt");
            return NULL;
        }
        in[i] = CONST_NDT(tmp);
    }

    spec = ndt_apply_spec_empty;
    if (ndt_typecheck(&spec, sig, in, (int)nin, NULL, NULL, &ctx) < 0) {
        return seterr(&ctx);
    }

    flags = PyUnicode_FromString(ndt_apply_flags_as_string(&spec));
    if (flags == NULL) {
        return NULL;
    }

    out = PyList_New(spec.nout);
    if (out == NULL) {
        ndt_apply_spec_clear(&spec);
        goto finish;
    }

    for (i=spec.nout-1; i >= 0; i--) {
        PyObject *x = ndtype_alloc(&Ndt_Type);
        if (x == NULL) {
            ndt_apply_spec_clear(&spec);
            goto finish;
        }
        NDT(x) = spec.out[i];
        spec.out[i] = NULL; spec.nout--;
        PyList_SET_ITEM(out, i, x);
    }

    broadcast = PyList_New(spec.nbroadcast);
    if (broadcast == NULL) {
        ndt_apply_spec_clear(&spec);
        goto finish;
    }

    for (i=spec.nbroadcast-1; i >= 0; i--) {
        PyObject *x = ndtype_alloc(&Ndt_Type);
        if (x == NULL) {
            ndt_apply_spec_clear(&spec);
            goto finish;
        }
        NDT(x) = spec.broadcast[i];
        spec.broadcast[i] = NULL; spec.nbroadcast--;
        PyList_SET_ITEM(broadcast, i, x);
    }

    outer_dims = PyLong_FromLong(spec.outer_dims);
    if (outer_dims == NULL) {
        goto finish;
    }

    res = PyObject_CallFunctionObjArgs((PyObject *)ApplySpec, flags, self,
                                       args, broadcast, out, outer_dims, NULL);

finish:
    Py_XDECREF(flags);
    Py_XDECREF(out);
    Py_XDECREF(broadcast);
    Py_XDECREF(outer_dims);
    return res;
}

static PyObject *
tuple_from_int64(int64_t x[NDT_MAX_DIM], int ndim)
{
    PyObject *tuple;
    int i;

    tuple = PyTuple_New(ndim);
    if (tuple == NULL) {
        return NULL;
    }

    for (i = 0; i < ndim; i++) {
        PyObject *v = PyLong_FromLongLong(x[i]);
        if (v == NULL) {
            Py_DECREF(tuple);
            return NULL;
        }
        PyTuple_SET_ITEM(tuple, i, v);
   }

   return tuple;
}

static PyObject *
ndtype_ndim(PyObject *self, PyObject *args UNUSED)
{
    const ndt_t *t = NDT(self);

    if (ndt_is_abstract(t)) {
        PyErr_SetString(PyExc_TypeError,
            "abstract type has no ndim");
        return NULL;
    }

    return PyLong_FromLong(t->ndim);
}

static PyObject *
ndtype_datasize(PyObject *self, PyObject *args UNUSED)
{
    const ndt_t *t = NDT(self);

    if (ndt_is_abstract(t)) {
        PyErr_SetString(PyExc_TypeError,
            "abstract type has no datasize");
        return NULL;
    }

    return PyLong_FromLongLong(t->datasize);
}

static PyObject *
ndtype_itemsize(PyObject *self, PyObject *args UNUSED)
{
    const ndt_t *t = NDT(self);
    int64_t size;

    if (ndt_is_abstract(t)) {
        PyErr_SetString(PyExc_TypeError,
            "abstract type has no itemsize");
        return NULL;
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

    return PyLong_FromLongLong(size);
}

static PyObject *
ndtype_shape(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    ndt_ndarray_t x;

    if (ndt_as_ndarray(&x, NDT(self), &ctx) < 0) {
        return seterr(&ctx);
    }

    return tuple_from_int64(x.shape, x.ndim);
}

static PyObject *
ndtype_strides(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    ndt_ndarray_t x;

    if (ndt_as_ndarray(&x, NDT(self), &ctx) < 0) {
        return seterr(&ctx);
    }

    return tuple_from_int64(x.strides, x.ndim);
}

static PyObject *
ndtype_hidden_dtype(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    const ndt_t *t = NDT(self);
    const ndt_t *dtype;
    ndt_t *u;

    dtype = ndt_hidden_dtype(t);

    u = ndt_copy(dtype, &ctx); 
    if (u == NULL) {
        return seterr(&ctx);
    }

    return Ndt_FromType(u);
}

static PyObject *
ndtype_align(PyObject *self, PyObject *args UNUSED)
{
    if (ndt_is_abstract(NDT(self))) {
        PyErr_SetString(PyExc_TypeError,
            "abstract type has no alignment");
        return NULL;
    }

    return PyLong_FromLong(NDT(self)->align);
}

static PyObject *
ndtype_reduce(PyObject *self, PyObject *args UNUSED)
{
    PyObject *bytes;
    PyObject *res;

    bytes = ndtype_serialize(self, NULL);
    if (bytes == NULL) {
        return NULL;
    }

    res = Py_BuildValue("O(O)", Deserialize, bytes);
    Py_DECREF(bytes);

    return res;
}

static PyObject *
ndtype_to_nbformat(PyObject *self, PyObject *args UNUSED)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *sig;
    PyObject *core;
    PyObject *ret;
    char *s;
    char *c;

    if (ndt_to_nbformat(&s, &c, NDT(self), &ctx) < 0) {
        return seterr(&ctx);
    }

    sig = PyUnicode_FromString(s);
    ndt_free(s);
    if (s == NULL) {
        ndt_free(c);
        return NULL;
    }

    core = PyUnicode_FromString(c);
    ndt_free(c);
    if (s == NULL) {
        Py_DECREF(sig);
        return NULL;
    }

    ret = PyTuple_New(2);
    if (ret == NULL) {
        Py_DECREF(sig);
        Py_DECREF(core);
        return NULL;
    }

    PyTuple_SET_ITEM(ret, 0, sig);
    PyTuple_SET_ITEM(ret, 1, core);

    return ret;
}


static PyGetSetDef ndtype_getsets [] =
{
  { "align", (getter)ndtype_align, NULL, doc_align, NULL},
  { "datasize", (getter)ndtype_datasize, NULL, doc_datasize, NULL},
  { "itemsize", (getter)ndtype_itemsize, NULL, doc_itemsize, NULL},
  { "ndim", (getter)ndtype_ndim, NULL, doc_ndim, NULL},
  { "shape", (getter)ndtype_shape, NULL, doc_shape, NULL},
  { "strides", (getter)ndtype_strides, NULL, doc_strides, NULL},
  { "hidden_dtype", (getter)ndtype_hidden_dtype, NULL, doc_strides, NULL},
  {NULL}
};

static PyMethodDef ndtype_methods [] =
{
  /* Boolean functions */
  { "isoptional", (PyCFunction)ndtype_ndt_is_optional, METH_NOARGS, doc_isoptional },
  { "isabstract", (PyCFunction)ndtype_ndt_is_abstract, METH_NOARGS, doc_isabstract },
  { "isconcrete", (PyCFunction)ndtype_ndt_is_concrete, METH_NOARGS, doc_isconcrete },
  { "isscalar", (PyCFunction)ndtype_ndt_is_scalar, METH_NOARGS, doc_isscalar },
  { "issigned", (PyCFunction)ndtype_ndt_is_signed, METH_NOARGS, doc_issigned },
  { "isunsigned", (PyCFunction)ndtype_ndt_is_unsigned, METH_NOARGS, doc_isunsigned },
  { "isfloat", (PyCFunction)ndtype_ndt_is_float, METH_NOARGS, doc_isfloat },
  { "iscomplex", (PyCFunction)ndtype_ndt_is_complex, METH_NOARGS, doc_iscomplex },

  /* Boolean functions on array types */
  { "is_c_contiguous", (PyCFunction)ndtype_ndt_is_c_contiguous, METH_NOARGS, doc_is_c_contiguous },
  { "is_f_contiguous", (PyCFunction)ndtype_ndt_is_f_contiguous, METH_NOARGS, doc_is_f_contiguous },

  /* Binary functions */
  { "match", (PyCFunction)ndtype_match, METH_O, doc_match },
  { "apply", (PyCFunction)ndtype_apply, METH_O, "method likely to change" },

  /* Other functions */
  { "to_format", (PyCFunction)ndtype_to_format, METH_NOARGS, NULL },
  { "to_nbformat", (PyCFunction)ndtype_to_nbformat, METH_NOARGS, NULL },
  { "pformat", (PyCFunction)ndtype_pformat, METH_NOARGS, doc_pformat },
  { "pprint", (PyCFunction)ndtype_pprint, METH_NOARGS, doc_pprint },
  { "ast_repr", (PyCFunction)ndtype_ast_repr, METH_NOARGS, doc_ast_repr },
  { "serialize", (PyCFunction)ndtype_serialize, METH_NOARGS, doc_serialize },

  /* Class methods */
  { "from_format", (PyCFunction)ndtype_from_format, METH_O|METH_CLASS, doc_from_format },
  { "deserialize", (PyCFunction)ndtype_deserialize, METH_O|METH_CLASS, doc_deserialize },

  /* Special methods */
  { "__copy__", ndtype_copy, METH_NOARGS, NULL },
  { "__deepcopy__", ndtype_copy, METH_O, NULL },
  { "__reduce__", ndtype_reduce, METH_NOARGS, NULL },

  { NULL, NULL, 1 }
};

static PyTypeObject Ndt_Type =
{
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "ndtypes.ndt",
    .tp_basicsize = sizeof(NdtObject),
    .tp_dealloc = (destructor) ndtype_dealloc,
    .tp_repr = (reprfunc) ndtype_repr,
    .tp_hash = ndtype_hash,
    .tp_str = (reprfunc) ndtype_str,
    .tp_getattro = (getattrofunc) PyObject_GenericGetAttr,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_traverse = (traverseproc)ndtype_traverse,
    .tp_richcompare = ndtype_richcompare,
    .tp_methods = ndtype_methods,
    .tp_getset = ndtype_getsets,
    .tp_alloc = PyType_GenericAlloc,
    .tp_new = ndtype_new,
    .tp_free = PyObject_GC_Del
};


/****************************************************************************/
/*                                   C-API                                  */
/****************************************************************************/

static void **ndtypes_api[NDTYPES_MAX_API];

static int
Ndt_CheckExact(const PyObject *v)
{
    return Py_TYPE(v) == &Ndt_Type;
}

static int
Ndt_Check(const PyObject *v)
{
    return PyObject_TypeCheck(v, &Ndt_Type);
}

static const ndt_t *
CONST_NDT(const PyObject *v)
{
    assert(Ndt_Check(v));
    return ((NdtObject *)v)->ndt;
}

static PyObject *
Ndt_SetError(ndt_context_t *ctx)
{
    return seterr(ctx);
}

static PyObject *
Ndt_CopySubtree(const PyObject *src, const ndt_t *t)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *dest;

    if (!Ndt_Check(src)) {
        PyErr_SetString(PyExc_TypeError, "expected ndt object");
        return NULL;
    }

    dest = ndtype_alloc(Py_TYPE(src));
    if (dest == NULL) {
        return NULL;
    }

    NDT(dest) = ndt_copy(t, &ctx);
    if (NDT(dest) == NULL) {
        return seterr(&ctx);
    }

    RBUF(dest) = RBUF(src);
    Py_XINCREF(RBUF(dest));

    return dest;
}

static PyObject *
Ndt_MoveSubtree(const PyObject *src, ndt_t *t)
{
    PyObject *dest;

    if (!Ndt_Check(src)) {
        PyErr_SetString(PyExc_TypeError, "expected ndt object");
        return NULL;
    }

    dest = ndtype_alloc(Py_TYPE(src));
    if (dest == NULL) {
        ndt_del(t);
        return NULL;
    }

    NDT(dest) = t;
    RBUF(dest) = RBUF(src);
    Py_XINCREF(RBUF(dest));

    return dest;
}

static PyObject *
Ndt_FromType(ndt_t *type)
{
    PyObject *self;

    self = ndtype_alloc(&Ndt_Type);
    if (self == NULL) {
        ndt_del(type);
        return NULL;
    }

    NDT(self) = type;
    return self;
}

static PyObject *
Ndt_FromObject(PyObject *obj)
{
    return ndtype_from_object(&Ndt_Type, obj);
}

static PyObject *
init_api(void)
{
    ndtypes_api[Ndt_CheckExact_INDEX] = (void *)Ndt_CheckExact;
    ndtypes_api[Ndt_Check_INDEX] = (void *)Ndt_Check;
    ndtypes_api[CONST_NDT_INDEX] = (void *)CONST_NDT;
    ndtypes_api[Ndt_SetError_INDEX] = (void *)Ndt_SetError;
    ndtypes_api[Ndt_CopySubtree_INDEX] = (void *)Ndt_CopySubtree;
    ndtypes_api[Ndt_MoveSubtree_INDEX] = (void *)Ndt_MoveSubtree;
    ndtypes_api[Ndt_FromType_INDEX] = (void *)Ndt_FromType;
    ndtypes_api[Ndt_FromObject_INDEX] = (void *)Ndt_FromObject;

    return PyCapsule_New(ndtypes_api, "ndtypes._ndtypes._API", NULL);
}


/****************************************************************************/
/*                                  Module                                  */
/****************************************************************************/

static PyObject *
ndtype_typedef(PyObject *mod UNUSED, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"name", "type", NULL};
    NDT_STATIC_CONTEXT(ctx);
    PyObject *name, *type;
    const char *cname, *ctype;
    ndt_t *t;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &name, &type)) {
        return NULL;
    }

    cname = PyUnicode_AsUTF8(name);
    if (cname == NULL) {
        return NULL;
    }

    ctype = PyUnicode_AsUTF8(type);
    if (ctype == NULL) {
        return NULL;
    }

    t = ndt_from_string(ctype, &ctx);
    if (t == NULL) {
        return seterr(&ctx);
    }

    if (ndt_typedef(cname, t, NULL, &ctx) < 0) {
        return seterr(&ctx);
    }

    Py_RETURN_NONE;
}

static PyObject *
ndtype_instantiate(PyObject *mod UNUSED, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"name", "type", NULL};
    NDT_STATIC_CONTEXT(ctx);
    PyObject *name, *type;
    const char *cname;
    char *cp;
    ndt_t *t, *tp;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO", kwlist, &name, &type)) {
        return NULL;
    }

    cname = PyUnicode_AsUTF8(name);
    if (cname == NULL) {
        return NULL;
    }

    if (!Ndt_Check(type)) {
        PyErr_SetString(PyExc_TypeError, "type argument must be ndt");
        return NULL;
    }

    cp = ndt_strdup(cname, &ctx);
    if (cp == NULL) {
        return seterr(&ctx);
    }

    tp = ndt_copy(NDT(type), &ctx);
    if (tp == NULL) {
        ndt_free(cp);
        return seterr(&ctx);
    }

    t = ndt_nominal(cp, tp, &ctx);
    if (t == NULL) {
        return seterr(&ctx);
    }

    return Ndt_MoveSubtree(type, t);
}

static PyMethodDef _ndtypes_methods [] =
{
  { "typedef", (PyCFunction)ndtype_typedef, METH_VARARGS|METH_KEYWORDS, NULL},
  { "instantiate", (PyCFunction)ndtype_instantiate, METH_VARARGS|METH_KEYWORDS, NULL},
  { NULL, NULL, 1, NULL }
};


static struct PyModuleDef ndtypes_module = {
    PyModuleDef_HEAD_INIT,        /* m_base */
    "_ndtypes",                   /* m_name */
    doc_module,                   /* m_doc */
    -1,                           /* m_size */
    _ndtypes_methods,             /* m_methods */
    NULL,                         /* m_slots */
    NULL,                         /* m_traverse */
    NULL,                         /* m_clear */
    NULL                          /* m_free */
};


PyMODINIT_FUNC
PyInit__ndtypes(void)
{
    NDT_STATIC_CONTEXT(ctx);
    PyObject *m = NULL;
    PyObject *collections = NULL;
    PyObject *obj = NULL;
    static PyObject *capsule = NULL;
    static int initialized = 0;

    if (!initialized) {
        capsule = init_api();
        if (capsule == NULL) {
            return NULL;
        }
        if (ndt_init(&ctx) < 0) {
            return seterr(&ctx);
        }
        initialized = 1;
    }

    if (PyType_Ready(&ResourceBuffer_Type) < 0) {
        goto error;
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
                                    "flags sig in_types in_broadcast out_types outer_dims");
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

    Deserialize = PyObject_GetAttrString((PyObject *)&Ndt_Type, "deserialize");
    if (Deserialize == NULL) {
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

    Py_INCREF(capsule);
    if (PyModule_AddObject(m, "_API", capsule) < 0) {
        goto error;
    }


    return m;


error:
    Py_CLEAR(m);
    return NULL;
}


