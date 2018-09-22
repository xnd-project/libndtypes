/* Header file containing various functions for GC guard table. */

#ifndef GC_GUARD_H
#define GC_GUARD_H

#include "ruby_ndtypes_internal.h"

void rb_ndtypes_gc_guard_unregister(NdtObject *ndt);
void rb_ndtypesgc_guard_register(NdtObject *ndt, VALUE rbuf);
void rb_ndtypes_init_gc_guard(VALUE cNDTypes);

#endif  /* GC_GUARD_H */
