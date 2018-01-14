.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Input/output
============

Functions for creating and displaying types.


Input
-----

.. topic:: ndt_from_file

.. code-block:: c

   ndt_t *ndt_from_file(const char *name, ndt_context_t *ctx);

Create a type from a file that contains the datashape representation.


.. topic:: ndt_from_string

.. code-block:: c

   ndt_t *ndt_from_string(const char *input, ndt_context_t *ctx);

Create a type from a string in datashape syntax. This is the primary function
for creating types.


.. topic:: ndt_from_metadata_and_dtype

.. code-block:: c

   typedef struct {
     int num_offset_arrays;               /* number of offset arrays */
     int32_t num_offsets[NDT_MAX_DIM];    /* lengths of the offset arrays */
     int32_t *offset_arrays[NDT_MAX_DIM]; /* offset arrays */
   } ndt_meta_t;

   ndt_t *ndt_from_metadata_and_dtype(const ndt_meta_t *m, const char *dtype, ndt_context_t *ctx);

Create a concrete var dimension using the external offset arrays given
in the :c:macro:`ndt_meta_t` struct.

The application is responsible for keeping the offset arrays alive while the
type *and all copies of the type* exist.

This is not as difficult as it sounds.  One approach that utilizes a resource
manager object is implemented in the Python ndtypes module.


.. topic:: ndt_from_bpformat

.. code-block:: c

   ndt_t *ndt_from_bpformat(const char *input, ndt_context_t *ctx);

Create a type from a buffer protocol format string (PEP-3118 syntax). This
is useful for translating dtypes in a `Py_buffer` struct.

The outer dimensions specified by the `Py_buffer` shape member need to
be created separately.


Output
------

.. topic:: ndt_as_string

.. code-block:: c

   char *ndt_as_string(const ndt_t *t, ndt_context_t *ctx);

Convert *t* to its string representation.  This currently omits some layout
details like alignment, packing or Fortran layout.


.. topic:: ndt_indent

.. code-block:: c

   char *ndt_indent(const ndt_t *t, ndt_context_t *ctx);

Same as :func:`ndt_as_string`, but indent the result.


.. topic:: ndt_ast_repr

.. code-block:: c

   char *ndt_ast_repr(const ndt_t *t, ndt_context_t *ctx);

Return the representation of the abstract syntax tree of the input type.
This representation includes all low level details.
