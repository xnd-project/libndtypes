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


#ifndef PYNDTYPES_H
#define PYNDTYPES_H
#ifdef __cplusplus
extern "C" {
#endif


#include <Python.h>
#include "ndtypes.h"


/****************************************************************************/
/*                                 ndt object                               */
/****************************************************************************/

/* Exposed here for the benefit of Numba. The API should not be regarded
   stable across versions. */

typedef struct {
    PyObject_HEAD
    PyObject *rbuf; /* resource buffer */
    ndt_t *ndt;     /* type */
} NdtObject;

#define NDT(v) (((NdtObject *)v)->ndt)
#define RBUF(v) (((NdtObject *)v)->rbuf)
#define RBUF_NDT_META(v) (((ResourceBufferObject *)(((NdtObject *)v)->rbuf))->m)


/****************************************************************************/
/*                                Capsule API                               */
/****************************************************************************/

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

#define Ndt_FromObject_INDEX 7
#define Ndt_FromObject_RETURN PyObject *
#define Ndt_FromObject_ARGS (PyObject *)

#define NDTYPES_MAX_API 8


#ifdef NDTYPES_MODULE
static Ndt_CheckExact_RETURN Ndt_CheckExact Ndt_CheckExact_ARGS;
static Ndt_Check_RETURN Ndt_Check Ndt_Check_ARGS;
static CONST_NDT_RETURN CONST_NDT CONST_NDT_ARGS;
static Ndt_SetError_RETURN Ndt_SetError Ndt_SetError_ARGS;
static Ndt_CopySubtree_RETURN Ndt_CopySubtree Ndt_CopySubtree_ARGS;
static Ndt_MoveSubtree_RETURN Ndt_MoveSubtree Ndt_MoveSubtree_ARGS;
static Ndt_FromType_RETURN Ndt_FromType Ndt_FromType_ARGS;
static Ndt_FromObject_RETURN Ndt_FromObject Ndt_FromObject_ARGS;
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

#define Ndt_FromObject \
    (*(Ndt_FromObject_RETURN (*)Ndt_FromObject_ARGS) _ndtypes_api[Ndt_FromObject_INDEX])

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
