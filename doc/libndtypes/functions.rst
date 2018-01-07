.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Functions
=========

Most library functions are for creating types. The functions in this section
operate *on* types.


Copying
-------

.. topic:: ndt_copy

.. code-block:: c

   ndt_t *ndt_copy(const ndt_t *t, ndt_context_t *ctx);

Create a copy of the argument. This is an important function, since types
should be immutable.


Equality
--------

.. topic:: ndt_equal

.. code-block:: c

   int ndt_equal(const ndt_t *t, const ndt_t *u);

Return 1 if *t* and *u* are structurally equal, *0* otherwise.


Pattern matching
----------------

.. topic:: ndt_match

.. code-block:: c

   int ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx);

Match concrete candidate *c* against the (possibly abstract) pattern *p*.

This is the main function used in type checking.



Type checking
-------------

.. topic:: ndt_typecheck

.. code-block:: c

   ndt_t *ndt_typecheck(const ndt_t *f, const ndt_t *args, int *outer_dims, ndt_context_t *ctx);

Take a function type *f*, check if it can accept the concrete type *args*.
*args* must be a tuple type that contains the individual arguments.

The return value is the inferred return type.

Store the number of outer dimensions that need to be traversed before applying
the function kernel.


