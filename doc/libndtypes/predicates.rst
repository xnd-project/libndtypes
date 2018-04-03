.. meta::
   :robots: index,follow
   :description: libndtypes documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Predicates
==========

libndtypes has a number of type predicates.


.. c:function:: int ndt_is_abstract(const ndt_t *t)

   Determine whether a type is abstract. These functions need to be called to check whether the concrete type fields are defined.

.. c:function:: int ndt_is_concrete(const ndt_t *t)

   Determine whether a type is concrete. These functions need to be called to check whether the concrete type fields are defined.


.. c:function:: int ndt_is_optional(const ndt_t *t)

   Check if a type is optional.


.. c:function:: int ndt_subtree_is_optional(const ndt_t *t)

   Check if a subtree of a type is optional.  This is useful for deciding if
   bitmaps need to be allocated for subtrees.


.. c:function:: int ndt_is_ndarray(const ndt_t *t)

   Check if a type describes an n-dimensional (n > 0) array of fixed dimensions.


.. c:function:: int ndt_is_c_contiguous(const ndt_t *t)

   Check if a type is an n-dimensional (n > 0) contiguous C
   array.  Currently this returns 0 for scalars.


.. c:function:: int ndt_is_f_contiguous(const ndt_t *t)

   Check if a type is an n-dimensional (n > 0) contiguous Fortran
   array.  Currently this returns 0 for scalars.

.. c:function:: int ndt_is_scalar(const ndt_t *t)

   Check if a type is a scalar.


.. c:function:: int ndt_is_signed(const ndt_t *t)

   Check if a type is signed.


.. c:function:: int ndt_is_unsigned(const ndt_t *t)

   Check if a type is unsigned.

.. c:function:: int ndt_is_float(const ndt_t *t)

   Check if a type is float.

.. c:function:: int ndt_is_complex(const ndt_t *t)

   Check if a type is complex.

.. c:function:: int ndt_endian_is_set(const ndt_t *t)

   Check whether the endianness of a type is explicitly set.


.. c:function:: int ndt_is_little_endian(const ndt_t *t)

   Check whether a type is little endian.  Use the native order
   if no endian flag is set.

.. c:function:: int ndt_is_big_endian(const ndt_t *t)

   Check whether a type is big endian.  Use the native order
   if no endian flag is set.
