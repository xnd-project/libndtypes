

.. meta::
   :robots: index,follow
   :description: libndtypes documentation


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
in the :macro:`ndt_meta_t` struct.

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


Range-checked input of primitive types
--------------------------------------

.. topic:: ndt_strtobool

.. code-block:: c

   bool ndt_strtobool(const char *v, ndt_context_t *ctx);

Convert string *v* to a bool. *v* must be "true" or "false". Return *0*
and set :macro:`NDT_InvalidArgumentError` if the conversion fails.


.. topic:: ndt_strtochar

.. code-block:: c

   char ndt_strtochar(const char *v, ndt_context_t *ctx);

Convert string *v* to a char. *v* must have length *1*.  Return *0* and
set :macro:`NDT_InvalidArgumentError` if the conversion fails.



.. topic:: ndt_strtol

.. code-block:: c

   char ndt_strtol(const char *v, ndt_context_t *ctx);

Convert string *v* to a long. In case of an error, use the return value
from :func:`strtol`.

If *v* is not an integer, set :macro:`NDT_InvalidArgumentError`.

If *v* is out of range, set :macro:`NDT_ValueError`.


.. code-block:: c

   long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx);

Convert string *v* to a long long.

If *v* is not an integer, set :macro:`NDT_InvalidArgumentError`.

If *v* is not in the range [*min*, *max*] , set :macro:`NDT_ValueError`.



.. code-block:: c

   unsigned long long ndt_strtoll(const char *v, long long min, long long max, ndt_context_t *ctx);

Convert string *v* to an unsigned long long.

If *v* is not an integer, set :macro:`NDT_InvalidArgumentError`.

If *v* is not in the range [*min*, *max*] , set :macro:`NDT_ValueError`.


.. code-block:: c

   float ndt_strtof(const char *v, ndt_context_t *ctx);

Convert string *v* to a float.

If *v* is not an integer, set :macro:`NDT_InvalidArgumentError`.

If *v* is out of range, set :macro:`NDT_ValueError`.


.. code-block:: c

   double ndt_strtod(const char *v, ndt_context_t *ctx);

Convert string *v* to a double.

If *v* is not an integer, set :macro:`NDT_InvalidArgumentError`.

If *v* is out of range, set :macro:`NDT_ValueError`.
