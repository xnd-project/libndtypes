

.. meta::
   :robots: index,follow
   :description: libndtypes documentation


Utilities
=========

This section contains utility functions that are meant to be used by other
applications.  Some of these functions are not yet in the stable API and
are subject to change.


Stable API
----------

.. topic:: ndt_strdup

.. code-block:: c

   char *ndt_strdup(const char *s, ndt_context_t *ctx);

Same as :func:`strdup`, but uses libndtypes's custom allocators.  On failure,
set an error in the context and return :macro:`NULL`.  The result must be
deallocated using :func:`ndt_free`.


.. topic:: ndt_asprintf

.. code-block:: c

   char *ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...);

Print to a string allocated by libndtypes's custom allocators.  On failure,
set an error in the context and return :macro:`NULL`.  The result must be 
deallocated using :func:`ndt_free`.


Unstable API
------------

.. topic:: ndt_dtype

.. code-block:: c

   const ndt_t *ndt_dtype(const ndt_t *t);

Return the dtype (element type) of an array.  If the argument is not an array,
return *t* itself.  The function cannot fail.


.. topic:: ndt_dims_dtype

.. code-block:: c

   int ndt_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *t);

Extract constant pointers to the dimensions and the dtype of an array and return 
the number of dimensions.  The function cannot fail.


.. topic:: ndt_as_ndarray

.. code-block:: c

   int ndt_as_ndarray(ndt_ndarray_t *a, const ndt_t *t, ndt_context_t *ctx);

Convert *t* to its ndarray representation *a*.  On success, return 0.
If *t* is abstract or not representable as an ndarray, set an error in
the context and return -1.


.. topic:: ndt_hash

.. code-block:: c

   ndt_ssize_t ndt_hash(ndt_t *t, ndt_context_t *ctx);

Hash a type.  This is currently implemented by converting the type to its
string representation and hashing the string.



