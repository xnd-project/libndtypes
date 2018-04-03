.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Input/output
============

Functions for creating and displaying types.


Input
-----

.. c:function:: ndt_t *ndt_from_file(const char *name, ndt_context_t *ctx)

  Create a type from a file that contains the datashape representation.


.. c:function:: ndt_t *ndt_from_string(const char *input, ndt_context_t *ctx);

  Create a type from a string in datashape syntax. This is the primary function
  for creating types.



.. c:type:: struct ndt_meta_t

  .. c:member:: int num_offset_arrays

     number of offset arrays
  
  .. c:member:: int32_t num_offsets[NDT_MAX_DIM]

    lengths of the offset arrays

  .. c:member:: int32_t *offset_arrays[NDT_MAX_DIM]

    offset arrays

.. c:function:: ndt_t *ndt_from_metadata_and_dtype(const ndt_meta_t *m, const char *dtype, ndt_context_t *ctx)

  Create a concrete var dimension using the external offset arrays given
  in the :c:type:`ndt_meta_t` struct.

  The application is responsible for keeping the offset arrays alive while the
  type *and all copies of the type* exist.

  This is not as difficult as it sounds.  One approach that utilizes a resource
  manager object is implemented in the Python ndtypes module.


.. c:function:: ndt_t *ndt_from_bpformat(const char *input, ndt_context_t *ctx)

  Create a type from a buffer protocol format string (PEP-3118 syntax). This
  is useful for translating dtypes in a `Py_buffer` struct.

  The outer dimensions specified by the `Py_buffer` shape member need to
  be created separately.


Output
------

.. c:function:: char *ndt_as_string(const ndt_t *t, ndt_context_t *ctx)

  Convert *t* to its string representation.  This currently omits some layout
  details like alignment, packing or Fortran layout.


.. c:function:: char *ndt_indent(const ndt_t *t, ndt_context_t *ctx)

  Same as :c:func:`ndt_as_string`, but indent the result.


.. c:function:: char *ndt_ast_repr(const ndt_t *t, ndt_context_t *ctx)

  Return the representation of the abstract syntax tree of the input type.
  This representation includes all low level details.
