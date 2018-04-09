.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Memory handling
===============

Type allocation and deallocation
--------------------------------

.. c:function::  ndt_t *ndt_new(enum ndt tag, ndt_context_t *ctx)

   Allocate a new type according to *tag* with the common fields initialized to
   the default values.

   Most types need additional initialization, so this function is rarely used
   on its own.


.. c:function:: ndt_t *ndt_tuple_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx)

   Allocate a new tuple type. Because of its internal complexity
   this types has a dedicated allocation function.

   As above, this function is  never used outside of wrapper functions.

.. c:function:: ndt_t *ndt_record_new(enum ndt_variadic flag, int64_t shape, ndt_context_t *ctx)

   Allocate a new tuple or record type. Because of its internal complexity
   this types has a dedicated allocation function.

   As above, this function is never used outside of wrapper functions.


.. c:function:: void ndt_del(ndt_t *t)

   Deallocate a type.  *t* may be :c:macro:`NULL`.  This function is meant to
   be used by applications directly.


Custom allocators
-----------------

libndtypes allows applications to set custom allocators at program start.
By default these global variables are set to the usual libc allocators.

.. c:var:: void *(* ndt_mallocfunc)(size_t size)
.. c:var:: void *(* ndt_callocfunc)(size_t nmemb, size_t size)
.. c:var:: void *(* ndt_reallocfunc)(void *ptr, size_t size)
.. c:var:: void (* ndt_freefunc)(void *ptr)


Allocation/deallocation
-----------------------

.. c:function:: void *ndt_alloc(int64_t nmemb, int64_t size)

   Allocate *nmemb \* size* bytes, using the function set in the custom allocator.

   Overflow in the multiplication is checked.  Return :c:macro:`NULL` on overflow
   or if the allocation fails.


.. c:function:: void *ndt_alloc_size(size_t size)

   Allocate *size* bytes, using the function set in the custom allocator.

   Return :c:macro:`NULL` on overflow or if the allocation fails.


.. c:function:: void *ndt_calloc(int64_t nmemb, int64_t size)

   Allocate *nmemb \* size* zero-initialized bytes, using the function set in the custom
   allocator.

   Return :c:macro:`NULL` if the allocation fails.


.. c:function:: void *ndt_realloc(void *ptr, int64_t nmemb, int64_t size)

   Reallocate *ptr* to use *nmemb \* size* bytes.

   Return :c:macro:`NULL` on overflow or if the allocation fails.  As usual, *ptr*
   is still valid after failure.


.. c:function:: void ndt_free(void *ptr)

   Free a pointer allocated by one of the above functions.  *ptr* may be
   :c:macro:`NULL` if the custom allocator allows this -- the C Standard
   requires :func:`free` to accept :c:macro:`NULL`.


Aligned allocation/deallocation
-------------------------------

.. c:function:: void *ndt_aligned_calloc(uint16_t alignment, int64_t size)

   Allocate *size* bytes with a guaranteed *alignment*.


.. c:function:: void ndt_aligned_free(void *ptr)

   Free a pointer that was allocated by :c:func:`ndt_aligned_calloc`.  *ptr*
   may be :c:macro:`NULL`.
