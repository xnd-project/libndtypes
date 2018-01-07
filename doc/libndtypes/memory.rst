

.. meta::
   :robots: index,follow
   :description: libndtypes documentation


Memory handling
===============

Type allocation and deallocation
--------------------------------

.. code-block:: c

   ndt_t *ndt_new(enum ndt tag, ndt_context_t *ctx);

Allocate a new type according to *tag* with the common fields initialized to
the default values.

Most types need additional initialization, so this function is rarely used
on its own.


.. code-block:: c

   ndt_t *ndt_tuple_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx);
   ndt_t *ndt_record_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx);

Allocate a new tuple or record type. Because of their internal complexity
these types have dedicated allocation functions.

As above, the functions are never used outside of wrapper functions.


.. code-block:: c

   void ndt_del(ndt_t *t);

Deallocate a type.  *t* may be :macro:`NULL`.  This function is meant to
be used by applications directly.


Custom allocators
-----------------

.. code-block:: c

   extern void *(* ndt_mallocfunc)(size_t size);
   extern void *(* ndt_callocfunc)(size_t nmemb, size_t size);
   extern void *(* ndt_reallocfunc)(void *ptr, size_t size);
   extern void (* ndt_freefunc)(void *ptr);

libndtypes allows applications to set custom allocators at program start.
By default these global variables are set to the usual libc allocators.


