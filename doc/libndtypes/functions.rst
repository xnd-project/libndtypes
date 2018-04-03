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

.. c:function:: ndt_t *ndt_copy(const ndt_t *t, ndt_context_t *ctx)

   Create a copy of the argument. This is an important function, since types
   should be immutable.


Equality
--------

.. c:function:: int ndt_equal(const ndt_t *t, const ndt_t *u)

   Return 1 if *t* and *u* are structurally equal, *0* otherwise.


Pattern matching
----------------

.. c:function:: int ndt_match(const ndt_t *p, const ndt_t *c, ndt_context_t *ctx)

   Match concrete candidate *c* against the (possibly abstract) pattern *p*.

   This is the main function used in type checking.



Type checking
-------------

.. c:function:: int ndt_typecheck(ndt_apply_spec_t *spec, const ndt_t *sig, const ndt_t *in[], \
              const int nin, ndt_context_t *ctx)

   Take a function type *sig*, check if it can accept the concrete arguments *in*.
   *args* must be a tuple type that contains the individual arguments.

   On success, infer and store the concrete return types and the (possibly broadcasted) 'in' types
   in *spec*.
