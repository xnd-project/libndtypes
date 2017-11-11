#ifndef PYNDTYPES_H
#define PYNDTYPES_H


#include "ndtypes.h"

int pyndt_check_exact(PyObject *);
int pyndt_check(PyObject *);
ndt_t *pyndt_get_ndt(PyObject *);


#endif /* PYNDTYPES_H */
