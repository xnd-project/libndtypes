.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Initialization and tables
=========================

libndtypes has global tables that need to be initialized and finalized.


.. c:function:: int ndt_init(ndt_context_t *ctx)

   Initialize the global tables.  This function must be called once at program
   start before using any other libndtypes functions.

   Return *0* on success and *-1* otherwise.

.. c:function:: void ndt_finalize(void)

   Deallocate the global tables.  This function may be called once at program
   end for the benefit of memory debuggers.

.. c:function:: int ndt_typedef(const char *name, ndt_t *type, ndt_constraint_t f, ndt_context_t *ctx)

   Add a nominal type alias for *type* to the typedef table. *name* must be globally
   unique. The function steals the *type* argument. *f* is an optional constraint function.

   On error, deallocate *type* and return *-1*.  Return *0* otherwise.

.. c:function:: int ndt_typedef_from_string(const char *name, const char *type, ndt_constraint_t f, ndt_context_t *ctx)

   Add a type alias for *type* to the typedef table. Like :c:func:`ndt_typedef` but reads
   the *type* as a string. 


.. c:function:: const ndt_typedef_t *ndt_typedef_find(const char *name, ndt_context_t *ctx)

   Try to find the type associated with *name* in the typedef table.  On success,
   return a const pointer to the typedef, :c:macro:`NULL` otherwise.

.. c:type:: struct ndt_typedef_t

   .. c:member:: const ndt_t * type
   .. c:member:: ndt_constraint_t constraint

.. c:type:: bool (* ndt_constraint_t)(void *, ndt_context_t *)

   Limits the permissible values to a subset of the type, by returning
   whether the initialized value is valid.
