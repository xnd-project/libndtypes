/* Functions useful for interfacing shared rbuf objects with the Ruby GC. */
/* Author: Sameer Deshmukh (@v0dro) */
#include "ruby_ndtypes_internal.h"

#define GC_GUARD_TABLE_NAME "@__gc_guard_table"

static ID id_gc_guard_table;

/* Unregister an NDT object-rbuf pair from the GC guard. */
void
rb_ndtypes_gc_guard_unregister(NdtObject *ndt)
{
  VALUE table = rb_ivar_get(mNDTypes_GCGuard, id_gc_guard_table);
  rb_hash_delete(table, PTR2NUM(ndt));
}

/* Register a NDT-rbuf pair in the GC guard.  */
void
rb_ndtypes_gc_guard_register(NdtObject *ndt, VALUE rbuf)
{
  VALUE table = rb_ivar_get(mNDTypes_GCGuard, id_gc_guard_table);
  if (table == Qnil) {
    rb_raise(rb_eLoadError, "GC guard not initialized.");
  }
  
  rb_hash_aset(table, PTR2NUM(ndt), rbuf);
}

/* Initialize the global GC guard table. klass is a VALUE reprensenting NDTypes class. */
void
rb_ndtypes_init_gc_guard(void)
{
  id_gc_guard_table = rb_intern(GC_GUARD_TABLE_NAME);
  rb_ivar_set(mNDTypes_GCGuard, id_gc_guard_table, rb_hash_new());
}

