

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

These fields are only defined for concrete types. For internal reasons
(facilitating copying etc.) they are initialized to zero for abstract
types.
