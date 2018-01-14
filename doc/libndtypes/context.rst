.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Context
=======

The context is used to facilitate error handling. The context struct itself
should not be considered public and is subject to change.


Constants
---------

The *err* field of the context is set to one of the following enum values:

.. code-block:: c

   #include <ndtypes.h>

   enum ndt_error {
     NDT_Success,
     NDT_ValueError,
     NDT_TypeError,
     NDT_InvalidArgumentError,
     NDT_NotImplementedError,
     NDT_LexError,
     NDT_ParseError,
     NDT_OSError,
     NDT_RuntimeError,
     NDT_MemoryError
   };


Static contexts
---------------

.. code-block:: c

    NDT_STATIC_CONTEXT(ctx);

This creates a static context, usually a local variable in a function.
Error messages may be dynamically allocated, so :func:`ndt_context_del`
must be called on static contexts, too.


Functions
---------

.. topic:: ndt_context_new
.. topic:: ndt_context_del

.. code-block:: c

   ndt_context_t *ndt_context_new(void);
   void ndt_context_del(ndt_context_t *ctx);


Create an initialized context or delete a context.  It is safe to call
:func:`ndt_context_del` on both dynamic and static contexts.


.. topic:: ndt_err_format

.. code-block:: c

   void ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...);

Set a context's error constant and error message.  *fmt* may contain the same
format specifiers as :func:`printf`.


.. topic:: ndt_err_occurred

.. code-block:: c

   int ndt_err_occurred(const ndt_context_t *ctx);

Check if an error has occurred.


.. topic:: ndt_err_clear

.. code-block:: c

    void ndt_err_clear(ndt_context_t *ctx);

Clear an error.


.. topic:: ndt_memory_error

.. code-block:: c

   void *ndt_memory_error(ndt_context_t *ctx);

Convenience function.  Set :c:macro:`NDT_MemoryError` and return :c:macro:`NULL`;


.. topic:: ndt_err_as_string

.. code-block:: c

   const char *ndt_err_as_string(enum ndt_error err);

Get the string representation of an error constant.


.. topic:: ndt_context_msg

.. code-block:: c

   const char *ndt_context_msg(ndt_context_t *ctx);

Get the current error string.  It is safe to call this function if no
error has occurred, in which case the string is :c:macro:`Success`.


.. topic:: ndt_err_fprint

.. code-block:: c

   ndt_err_fprint(FILE *fp, ndt_context_t *ctx);

Print an error to *fp*.  Mostly useful for debugging.



