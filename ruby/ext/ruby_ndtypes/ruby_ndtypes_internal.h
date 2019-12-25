/* Various internal macros and function definitions. Not exposed to other Ruby C extensions. */
/* Author: Sameer Deshmukh (@v0dro) */

#ifndef RUBY_NDTYPES_INTERNAL_H
#define RUBY_NDTYPES_INTERNAL_H

#include "ruby.h"
#include "ndtypes.h"
#include "ruby_ndtypes.h"

extern VALUE mNDTypes_GCGuard;

/* typedefs */
typedef struct NdtObject NdtObject;
typedef struct ResourceBufferObject ResourceBufferObject;

/* macros */
#if SIZEOF_LONG == SIZEOF_VOIDP
# define PTR2NUM(x)   (LONG2NUM((long)(x)))
# define NUM2PTR(x)   ((void*)(NUM2ULONG(x)))
#elif SIZEOF_LONG_LONG == SIZEOF_VOIDP
# define PTR2NUM(x)   (LL2NUM((LONG_LONG)(x)))
# define NUM2PTR(x)   ((void*)(NUM2ULL(x)))
#else
# error ---->> ruby requires sizeof(void*) == sizeof(long) or sizeof(LONG_LONG) to be compiled. <<----
#endif

#endif  /* RUBY_NDTYPES_INTERNAL_H */
