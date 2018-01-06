

.. meta::
   :robots: index,follow
   :description: libndtypes documentation


Types
=====

Types are implemented as a tagged union.  For the defined type enum values
it is best to refer to :macro:`ndtypes.h` directly or to search the constructor
functions below.


Abstract and concrete types
---------------------------

.. topic:: abstract_concrete

.. code-block:: c

   /* Protect access to concrete type fields. */
   enum ndt_access {
     Abstract,
     Concrete
   };

An important concept in libndtypes are abstract and concrete types.

Abstract types can have symbolic values like dimension or type variables
and are used for type checking.

Concrete types additionally have full memory layout information like
alignment and data size.

In order to protect against accidental access to undefined concrete fields, types
have the *ndt_access* field that is set to *Abstract* or *Concrete*.


Flags
-----

.. code-block:: c

   /* flags */
   #define NDT_LITTLE_ENDIAN  0x00000001U
   #define NDT_BIG_ENDIAN     0x00000002U
   #define NDT_OPTION         0x00000004U
   #define NDT_SUBTREE_OPTION 0x00000008U
   #define NDT_ELLIPSIS       0x00000010U

The endian flags are set if a type has explicit endianness. If native order
is used, they are unset.

:macro:`NDT_OPTION` is set if a type itself is optional.

:macro:`NDT_SUBTREE_OPTION` is set if any subtree of a type is optional.

:macro:`NDT_ELLIPSIS` is set if the tail of a dimension sequence contains
an ellipsis dimension.  The flag is not propagated to an outer array with
a dtype that contains an inner array with an ellipsis.


Common fields
-------------

.. topic:: common_fields

.. code-block:: c

   struct _ndt {
       /* Always defined */
       enum ndt tag;
       enum ndt_access access;
       uint32_t flags;
       int ndim;
       /* Undefined if the type is abstract */
       int64_t datasize;
       uint16_t align;
       ...
   };

*tag*, *access* and *flags* are explained above.  Every type has an *ndim* field
even when it is not an array, in which case *ndim* is zero.

The *datasize* and *align* fields are defined for concrete types.


Abstract fields
---------------

.. topic:: abstract_fields

.. code-block:: c

    union {
        ...

        struct {
            int64_t shape;
            ndt_t *type;
        } FixedDim;

        ...

    };

These fields are always defined for both abstract and concrete types.
:macro:`FixedDim` is just an example field.  Refer to :macro:`ndtypes.h`
directly for the complete set of fields.


Concrete fields
---------------

.. topic:: concrete_fields

.. code-block:: c

   struct {
        union {
            struct {
                int64_t itemsize;
                int64_t step;
            } FixedDim;

        ...

        };
   } Concrete;

These fields are only defined for concrete types.  For internal reasons
(facilitating copying etc.) they are initialized to zero for abstract
types.


Type constructor functions
--------------------------

All functions in this section steal their arguments.  On success, heap
allocated memory like *type* and *name* arguments belong to the return
value.

On error, all arguments are deallocated within the respective functions.


Special types
--------------

The types in this section all have some property that makes them different
from the regular types.

.. topic:: ndt_option

.. code-block:: c

   ndt_t *ndt_option(ndt_t *type);

This constructor is unique in that it does *not* create a new type with an
:macro:`Option` tag, but sets the :macro:`NDT_OPTION` flag of its argument.

The reason is that having a separate :macro:`Option` tag complicates the
type traversal when using libndtypes.

The function returns its argument and cannot fail.


.. topic:: ndt_module

.. code-block:: c

   ndt_t *ndt_module(char *name, ndt_t *type, ndt_context_t *ctx);

The module type is for implementing type name spaces and is always abstract.
Used in type checking.


.. topic:: ndt_function

.. code-block:: c

   ndt_t *ndt_function(ndt_t *ret, ndt_t *pos, ndt_t *kwds, ndt_context_t *ctx);

The function type is used for declaring function signatures.
Used in type checking.


.. topic:: ndt_void

.. code-block:: c

   ndt_t *ndt_void(ndt_context_t *ctx)

Currently only used as the empty return value in function signatures.


Regular types
-------------

.. topic:: ndt_fixed_dim

.. code-block:: c

   ndt_t *ndt_fixed_dim(ndt_t *type, int64_t shape, int64_t step, ndt_context_t *ctx);

*type* is either a dtype or the rest of the dimensions.

*shape* is the dimension size and must be a natural number.

 *step* is the amount to add to the linear index in order to move one
dimension element dimension to the next. *step* may be negative.


If *step* is :macro:`INT64_MAX`, the steps are computed from the dimensions
shapes and the resulting array is C-contiguous. This is the regular case.

If *step* is given, it is used without further checks. This is mostly useful
for slicing. The computed *datasize* is the minimum datasize such that all
index combinations are within the bounds of the allocated memory.

