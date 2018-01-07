.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Typedef
=======

libndtypes has a global lookup table for type aliases.  These aliases are
treated as nominal types in pattern matching.


.. code-block:: c

   int ndt_init(ndt_context_t *ctx);

This function must be called at program start to initialize the typedef
table.


.. code-block:: c

   int ndt_typedef(const char *name, ndt_t *type, ndt_context_t *ctx);

Create a nominal type alias for *type*.  The function steals the *type*
argument.



