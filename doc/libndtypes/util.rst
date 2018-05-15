.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Utilities
=========

This section contains utility functions that are meant to be used by other
applications.  Some of these functions are not yet in the stable API and
are subject to change.


Stable API
----------

.. c:function:: char *ndt_strdup(const char *s, ndt_context_t *ctx)

   Same as :func:`strdup`, but uses libndtypes's custom allocators.  On failure,
   set an error in the context and return :c:macro:`NULL`.  The result must be
   deallocated using :func:`ndt_free`.


.. c:function:: char *ndt_asprintf(ndt_context_t *ctx, const char *fmt, ...)

   Print to a string allocated by libndtypes's custom allocators.  On failure,
   set an error in the context and return :c:macro:`NULL`.  The result must be 
   deallocated using :func:`ndt_free`.


.. c:function:: bool ndt_strtobool(const char *v, ndt_context_t *ctx)

   Convert string *v* to a bool. *v* must be "true" or "false". Return *0*
   and set :c:macro:`NDT_InvalidArgumentError` if the conversion fails.


.. c:function:: char ndt_strtochar(const char *v, ndt_context_t *ctx)

   Convert string *v* to a char. *v* must have length *1*.  Return *0* and
   set :c:macro:`NDT_InvalidArgumentError` if the conversion fails.


.. c:function:: char ndt_strtol(const char *v, ndt_context_t *ctx)

   Convert string *v* to a long. In case of an error, use the return value
   from :c:func:`strtol`.

   If *v* is not an integer, set :c:data:`NDT_InvalidArgumentError`.

   If *v* is out of range, set :c:data:`NDT_ValueError`.


.. c:function:: long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx)

   Convert string *v* to a long long.

   If *v* is not an integer, set :c:data:`NDT_InvalidArgumentError`.

   If *v* is not in the range [*min*, *max*] , set :c:data:`NDT_ValueError`.




.. c:function:: unsigned long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx)

   Convert string *v* to an unsigned long long.

   If *v* is not an integer, set :c:data:`NDT_InvalidArgumentError`.

   If *v* is not in the range [*min*, *max*] , set :c:data:`NDT_ValueError`.


.. c:function:: float ndt_strtof(const char *v, ndt_context_t *ctx)

   Convert string *v* to a float.

   If *v* is not an integer, set :c:data:`NDT_InvalidArgumentError`.

   If *v* is out of range, set :c:data:`NDT_ValueError`.


.. c:function:: double ndt_strtod(const char *v, ndt_context_t *ctx)

   Convert string *v* to a double.

   If *v* is not an integer, set :c:data:`NDT_InvalidArgumentError`.


   If *v* is out of range, set :c:data:`NDT_ValueError`.


Unstable API
------------

.. c:function:: const ndt_t *ndt_dtype(const ndt_t *t)

   Return the dtype (element type) of an array.  If the argument is not an array,
   return *t* itself.  The function cannot fail.


.. c:function:: int ndt_dims_dtype(const ndt_t *dims[NDT_MAX_DIM], const ndt_t **dtype, const ndt_t *t)

   Extract constant pointers to the dimensions and the dtype of an array and return 
   the number of dimensions.  The function cannot fail.


.. c:function:: int ndt_as_ndarray(ndt_ndarray_t *a, const ndt_t *t, ndt_context_t *ctx)

   Convert *t* to its ndarray representation *a*.  On success, return 0.
   If *t* is abstract or not representable as an ndarray, set an error in
   the context and return -1.


.. c:function:: ndt_ssize_t ndt_hash(ndt_t *t, ndt_context_t *ctx)

   Hash a type.  This is currently implemented by converting the type to its
   string representation and hashing the string.



