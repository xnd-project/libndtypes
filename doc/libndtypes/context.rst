.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Context
=======

.. c:type:: struct ndt_context_t

   The context is used to facilitate error handling. The context struct itself
   should not be considered public and is subject to change.


Constants
---------

.. c:member:: enum ndt_error ndt_context_t.err

   The *err* field of the context is set to one of the following enum values:

.. c:type:: enum ndt_error

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

.. c:macro:: NDT_STATIC_CONTEXT(ctx)

   This creates a static context, usually a local variable in a function.
   Error messages may be dynamically allocated, so :c:func:`ndt_context_del`
   must be called on static contexts, too.


Functions
---------

.. c:function:: ndt_context_t *ndt_context_new(void)

   Create an initialized contex

.. c:function:: void ndt_context_del(ndt_context_t *ctx)

   Delete a context. Safe to call on both dynamic and static contexts.

.. c:function:: void ndt_err_format(ndt_context_t *ctx, enum ndt_error err, const char *fmt, ...)

   Set a context's error constant and error message.  *fmt* may contain the same
   format specifiers as :func:`printf`.

.. c:function:: int ndt_err_occurred(const ndt_context_t *ctx)

   Check if an error has occurred.

.. c:function:: void ndt_err_clear(ndt_context_t *ctx)

   Clear an error.

.. c:function:: void *ndt_memory_error(ndt_context_t *ctx)

   Convenience function.
   Set :c:macro:`NDT_MemoryError` and return :c:macro:`NULL`;

.. c:function:: const char *ndt_err_as_string(enum ndt_error err)

   Get the string representation of an error constant.

.. c:function:: const char *ndt_context_msg(ndt_context_t *ctx)

   Get the current error string.  It is safe to call this function if no
   error has occurred, in which case the string is :c:macro:`Success`.

.. c:function:: void ndt_err_fprint(FILE *fp, ndt_context_t *ctx)

   Print an error to *fp*.  Mostly useful for debugging.
