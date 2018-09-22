/* File containing headers for Ruby ndtypes wrapper. 
 *
 * Author: Sameer Deshmukh (@v0dro)
*/

#ifndef RUBY_NDTYPES_H
#define RUBY_NDTYPES_H

#if defined(__cplusplus)
extern "C" {
} /* satisfy cc-mode */
#endif

#include "ruby.h"
#include "ndtypes.h"

/* Public interface for ndtypes. */
typedef struct NdtObject NdtObject;
extern VALUE cNDTypes;

int rb_ndtypes_check_type(VALUE obj);
NdtObject * rb_ndtypes_get_ndt_object(VALUE obj);
VALUE rb_ndtypes_make_ndt_object(NdtObject *ndt_p);
VALUE rb_ndtypes_wrap_ndt_object(void);
const ndt_t * rb_ndtypes_const_ndt(VALUE ndt);
VALUE rb_ndtypes_move_subtree(VALUE src, ndt_t *t);
VALUE rb_ndtypes_from_object(VALUE type);
VALUE rb_ndtypes_set_error(ndt_context_t *ctx);
VALUE rb_ndtypes_from_type(ndt_t *type);

#define INT2BOOL(t) (t ? Qtrue : Qfalse)

#if defined(__cplusplus)
} /* extern "C" { */
#endif

#endif  /* RUBY_NDTYPES_H */
