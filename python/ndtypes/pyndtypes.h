#ifndef PYNDTYPES_H
#define PYNDTYPES_H
#ifdef __cplusplus
extern "C" {
#endif


#include "ndtypes.h"


#define Ndt_CheckExact_INDEX 0
#define Ndt_CheckExact_RETURN int
#define Ndt_CheckExact_ARGS (const PyObject *)

#define Ndt_Check_INDEX 1
#define Ndt_Check_RETURN int
#define Ndt_Check_ARGS (const PyObject *)

#define CONST_NDT_INDEX 2
#define CONST_NDT_RETURN const ndt_t *
#define CONST_NDT_ARGS (const PyObject *)

#define NDTYPES_MAX_API 3


#ifdef NDTYPES_MODULE
static Ndt_CheckExact_RETURN Ndt_CheckExact Ndt_CheckExact_ARGS;
static Ndt_Check_RETURN Ndt_Check Ndt_Check_ARGS;
static CONST_NDT_RETURN CONST_NDT CONST_NDT_ARGS;
#else
static void **_ndtypes_API;

#define Ndt_CheckExact \
    (*(Ndt_CheckExact_RETURN (*)Ndt_CheckExact_ARGS) _ndtypes_API[Ndt_CheckExact_INDEX])

#define Ndt_Check \
    (*(Ndt_Check_RETURN (*)Ndt_Check_ARGS) _ndtypes_API[Ndt_Check_INDEX])

#define CONST_NDT \
    (*(CONST_NDT_RETURN (*)CONST_NDT_ARGS) _ndtypes_API[CONST_NDT_INDEX])

static int
import_ndtypes(void)
{
    _ndtypes_API = (void **)PyCapsule_Import("ndtypes._ndtypes._API", 0);
    if (_ndtypes_API == NULL) {
        return -1;
    }

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* PYNDTYPES_H */
