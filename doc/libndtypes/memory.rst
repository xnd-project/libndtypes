.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


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

Deallocate a type.  *t* may be :c:macro:`NULL`.  This function is meant to
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


Allocation/deallocation
-----------------------

.. code-block:: c

   void *ndt_alloc(int64_t nmemb, int64_t size);

Allocate *nmemb \* size* bytes, using the function set in the custom allocator.

Overflow in the multiplication is checked.  Return :c:macro:`NULL` on overflow
or if the allocation fails.


.. code-block:: c

   void *ndt_alloc_size(size_t size);

Allocate *size* bytes, using the function set in the custom allocator.

Return :c:macro:`NULL` on overflow or if the allocation fails.


.. code-block:: c

   void *ndt_calloc(int64_t nmemb, int64_t size);

Allocate *nmemb \* size* zero-initialized bytes, using the function set in the custom
allocator.

Return :c:macro:`NULL` if the allocation fails.


.. code-block:: c

   void *ndt_realloc(void *ptr, int64_t nmemb, int64_t size);

Reallocate *ptr* to use *nmemb \* size* bytes.

Return :c:macro:`NULL` on overflow or if the allocation fails.  As usual, *ptr*
is still valid after failure.


.. code-block:: c

   void ndt_free(void *ptr);

Free a pointer allocated by one of the above functions.  *ptr* may be
:c:macro:`NULL` if the custom allocator allows this -- the C Standard
requires :func:`free` to accept :c:macro:`NULL`.


Aligned allocation/deallocation
-------------------------------

.. code-block:: c

   void *ndt_aligned_calloc(uint16_t alignment, int64_t size);

Allocate *size* bytes with a guaranteed *alignment*.


.. code-block:: c

   void ndt_aligned_free(void *ptr);

Free a pointer that was allocated by :func:`ndt_aligned_calloc`.  *ptr*
may be :c:macro:`NULL`.
