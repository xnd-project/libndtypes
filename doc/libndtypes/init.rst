.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Initialization and tables
=========================

libndtypes has global tables that need to be initialized and finalized.


.. code-block:: c

   int ndt_init(ndt_context_t *ctx);


Initialize the global tables.  This function must be called once at program
start before using any other libndtypes functions.

Return *0* on success and *-1* otherwise.


.. code-block:: c

   void ndt_finalize(void);

Deallocate the global tables.  This function may be called once at program
end for the benefit of memory debuggers.


.. code-block:: c

   int ndt_typedef_add(const char *name, const ndt_t *type, ndt_context_t *ctx);

Add a type alias for *type* to the typedef table. *name* must be globally
unique. The function steals the *type* argument.

On error, deallocate *type* and return *-1*.  Return *0* otherwise.


.. code-block:: c

   const ndt_t *ndt_typedef_find(const char *name, ndt_context_t *ctx);

Try to find the type associated with  *name* in the typedef table.  On success,
return a const pointer to the type, :c:macro:`NULL` otherwise.
