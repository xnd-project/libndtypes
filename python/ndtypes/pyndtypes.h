#ifndef PYNDTYPES_H
#define PYNDTYPES_H

typedef struct {
    PyObject_HEAD
    PyObject *rbuf; /* resource buffer */
    ndt_t *ndt;     /* type */
} NdtObject;

PyTypeObject Ndt_Type;

#define Ndt_CheckExact(v) (Py_TYPE(v) == &Ndt_Type)
#define Ndt_Check(v) PyObject_TypeCheck(v, &Ndt_Type)
#define NDT(v) (((NdtObject *)v)->ndt)

#endif
