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

#define Ndt_SetError_INDEX 3
#define Ndt_SetError_RETURN PyObject *
#define Ndt_SetError_ARGS (ndt_context_t *)

#define Ndt_CopySubtree_INDEX 4
#define Ndt_CopySubtree_RETURN PyObject *
#define Ndt_CopySubtree_ARGS (const PyObject *, const ndt_t *)

#define Ndt_MoveSubtree_INDEX 5
#define Ndt_MoveSubtree_RETURN PyObject *
#define Ndt_MoveSubtree_ARGS (const PyObject *, ndt_t *)

#define Ndt_FromType_INDEX 6
#define Ndt_FromType_RETURN PyObject *
#define Ndt_FromType_ARGS (ndt_t *)

#define NDTYPES_MAX_API 7


#ifdef NDTYPES_MODULE
static Ndt_CheckExact_RETURN Ndt_CheckExact Ndt_CheckExact_ARGS;
static Ndt_Check_RETURN Ndt_Check Ndt_Check_ARGS;
static CONST_NDT_RETURN CONST_NDT CONST_NDT_ARGS;
static Ndt_SetError_RETURN Ndt_SetError Ndt_SetError_ARGS;
static Ndt_CopySubtree_RETURN Ndt_CopySubtree Ndt_CopySubtree_ARGS;
static Ndt_MoveSubtree_RETURN Ndt_MoveSubtree Ndt_MoveSubtree_ARGS;
static Ndt_FromType_RETURN Ndt_FromType Ndt_FromType_ARGS;
#else
static void **_ndtypes_api;

#define Ndt_CheckExact \
    (*(Ndt_CheckExact_RETURN (*)Ndt_CheckExact_ARGS) _ndtypes_api[Ndt_CheckExact_INDEX])

#define Ndt_Check \
    (*(Ndt_Check_RETURN (*)Ndt_Check_ARGS) _ndtypes_api[Ndt_Check_INDEX])

#define CONST_NDT \
    (*(CONST_NDT_RETURN (*)CONST_NDT_ARGS) _ndtypes_api[CONST_NDT_INDEX])

#define Ndt_SetError \
    (*(Ndt_SetError_RETURN (*)Ndt_SetError_ARGS) _ndtypes_api[Ndt_SetError_INDEX])

#define Ndt_CopySubtree \
    (*(Ndt_CopySubtree_RETURN (*)Ndt_CopySubtree_ARGS) _ndtypes_api[Ndt_CopySubtree_INDEX])

#define Ndt_MoveSubtree \
    (*(Ndt_MoveSubtree_RETURN (*)Ndt_MoveSubtree_ARGS) _ndtypes_api[Ndt_MoveSubtree_INDEX])

#define Ndt_FromType \
    (*(Ndt_FromType_RETURN (*)Ndt_FromType_ARGS) _ndtypes_api[Ndt_FromType_INDEX])

static int
import_ndtypes(void)
{
    _ndtypes_api = (void **)PyCapsule_Import("ndtypes._ndtypes._API", 0);
    if (_ndtypes_api == NULL) {
        return -1;
    }

    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* PYNDTYPES_H */
