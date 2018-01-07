.. meta::
   :robots: index,follow
   :description: ndtypes pattern matching
   :keywords: ndtypes, pattern matching

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>



################
Pattern matching
################

The libndtypes implementation of datashape is dynamically typed with strict type
checking. Static type checking of datashape would be far more complex, since
datashape allows dependent types [#f1]_, i.e. types depending on values.

Dynamic pattern matching is used for checking function arguments, return
values, broadcasting and general array functions.

Again, we will be using the :py:mod:`ndtypes` module included in
`ndtypes <https://github.com/plures/ndtypes>`_ to demonstrate
datashape pattern matching. The rest of this document assumes that the
:py:mod:`ndtypes` module has been imported:

.. code-block:: py

   from ndtypes import ndt


*************
General notes
*************

:py:class:`ndt` instances have a :py:meth:`match` method for determining
whether the argument type is compatible with the instance type. The match
succeeds if and only if the set of types described by the right hand side
is a subset of the set of types described by the left hand side.


Simple example
--------------

.. code-block:: py

   >>> p = ndt("Any")
   >>> c = ndt("int32")
   >>> p.match(c)
   True


Non-commutativity
-----------------

From the above definition it follows that pattern matching is not commutative:

.. code-block:: py

   >>> p = ndt("int32")
   >>> c = ndt("Any")
   >>> p.match(c)
   False


Concrete matching
-----------------

Much like members of the alphabet in regular expressions, concrete types
match themselves:

.. code-block:: py

   >>> p = ndt("int32")
   >>> c = ndt("int32")
   >>> p.match(c)
   True

   >>> p = ndt("var * float32")
   >>> c = ndt("var * float32")
   >>> p.match(c)
   True

   >>> p = ndt("10 * float64")
   >>> c = ndt("10 * float32")
   >>> p.match(c)
   False


.. _type-kinds-matching:

**********
Type kinds
**********

:ref:`Type kinds <type-kinds>` are named subsets of :ref:`types <types>`.

Unlike :ref:`dtype variables <dtype-vars-matching>`, matching type kinds
does not require that a well defined substitution exists. Two instances
of a type kind can match different types:

.. code-block:: py

   >>> p = ndt("(Any) -> Any")
   >>> c = ndt("(float64) -> int32")
   >>> p.match(c)
   True


Any
---

The *Any* type kind is the most general and describes the set of all
:ref:`types <types>`.

Here's how to match a dtype against the set of all types:

.. code-block:: py

   >>> p = ndt("Any")
   >>> c = ndt("int32")
   >>> p.match(c)
   True


This matches an array type against the set of all types:

.. code-block:: py

   >>> p = ndt("Any")
   >>> c = ndt("10 * 5 * { v: float64, t: float64 }")
   >>> p.match(c)
   True


Scalar
------

The *Scalar* type kind stands for the set of all :ref:`scalars <scalars>`.

:c:type:`int32` is a member of the set of all scalars:

.. code-block:: py

   >>> p = ndt("Scalar")
   >>> c = ndt("int32")
   >>> p.match(c)
   True


A pattern for a function that takes any type and returns a scalar:

.. code-block:: py

   >>> p = ndt("(Any) -> Scalar")
   >>> c = ndt("(10 * complex128) -> float64")
   >>> p.match(c)
   True

   >>> p = ndt("(Any) -> Scalar")
   >>> c = ndt("(?{a: 10 * uint8}) -> uint8")
   >>> p.match(c)
   True

   >>> p = ndt("(Any) -> Scalar")
   >>> c = ndt("(?{a: 10 * uint8}) -> 10 * uint8")
   >>> p.match(c)
   False


Unlike with type variables, different types match a type kind:

.. code-block:: py

   >>> p = ndt("(Scalar, Scalar)")
   >>> c = ndt("(uint8, float64)")
   >>> p.match(c)
   True


FixedString
-----------

The set of all :ref:`fixed string <fixed-string>` types.

.. code-block:: py

   >>> p = ndt("FixedString")
   >>> c = ndt("fixed_string(100)")
   >>> p.match(c)
   True

   >>> p = ndt("FixedString")
   >>> c = ndt("fixed_string(100, 'utf16')")
   >>> p.match(c)
   True

   >>> p = ndt("FixedString")
   >>> c = ndt("string")
   >>> p.match(c)
   False


FixedBytes
----------

The set of all :ref:`fixed bytes <fixed-bytes>` types.

.. code-block:: py

   >>> p = ndt("FixedBytes")
   >>> c = ndt("fixed_bytes(size=100)")
   >>> p.match(c)
   True

   >>> p = ndt("FixedBytes")
   >>> c = ndt("fixed_bytes(size=100, align=2)")
   >>> p.match(c)
   True

   >>> p = ndt("FixedBytes")
   >>> c = ndt("bytes(align=2)")
   >>> p.match(c)
   False


***************
Dimension kinds
***************

:ref:`Dimension kinds <arrays>` stand for the set of all instances of
the respective kind.

Fixed
-----

The set of all instances of the :ref:`fixed dimension <arrays>` kind.

.. code-block:: py

   >>> p = ndt("Fixed * 20 * bool")
   >>> c = ndt("10 * 20 * bool")
   >>> p.match(c)
   True

   >>> p = ndt("Fixed * Fixed * bool")
   >>> c = ndt("var * var * bool")
   >>> p.match(c)
   False


.. _dtype-vars-matching:

***************
Dtype variables
***************

:ref:`dtype variables <dtype-variables>` are placeholders for dtypes. It is important
to note that they are *not* general type variables.  For example, they do not match
:ref:`array types <arrays>`, a concept which is used in general array functions [#f2]_,
whose base cases may operate on a dtype.


This matches a record against a single :ref:`dtype <dtypes>` variable:

.. code-block:: py

   >>> p = ndt("T")
   >>> c = ndt("{v: float64, t: float64}")
   >>> p.match(c)
   True


Match against several dtype variables in a tuple type:

.. code-block:: py

   >>> p = ndt("T")
   >>> c = ndt("(int32, int32, bool)")
   >>> p.match(c)
   True

   >>> p = ndt("(T, T, S)")
   >>> c = ndt("(int32, int64, bool)")
   >>> p.match(c)
   False


.. _symbolic-dim-matching:

*******************
Symbolic dimensions
*******************

Recall that :ref:`array <arrays>` types include the dimension kind, which
can be symbolic.


Simple symbolic match
---------------------

This matches a concrete fixed size array against the set of all one-dimensional
fixed size arrays:

.. code-block:: py

   >>> p = ndt("N * float64")
   >>> c = ndt("100 * float64")
   >>> p.match(c)
   True


Symbolic-symbolic match
-----------------------

Symbolic dimensions also match against other symbolic dimensions:

.. code-block:: py

   >>> p = ndt("N * float64")
   >>> c = ndt("M * float64")
   >>> p.match(c)
   True


Symbolic+Dtypevar
-----------------

Symbolic dimensions can be used in conjunction with dtype variables:

.. code-block:: py

   >>> p = ndt("N * T")
   >>> c = ndt("10 * float32")
   >>> p.match(c)
   True


Ellipsis match
--------------

Finally, all dimension kinds (including multiple dimensions) match against
ellipsis dimensions (named or unnamed):

.. code-block:: py

   >>> p = ndt("... * float64")
   >>> c = ndt("N * float64")
   >>> p.match(c)
   True

   >>> p = ndt("... * float64")
   >>> c = ndt("10 * N * float64")
   >>> p.match(c)
   True

   >>> p = ndt("Dim... * float64")
   >>> c = ndt("10 * 20 * float64")
   >>> p.match(c)
   True


This is used in broadcasting [#f2]_.



|
|

.. [#f1] An argument is often made that the term *dependent types* should
         be reserved for static type systems.  We use it here while explicitly
         acknowledging that the datashape implementation is dynamically typed.

.. [#f2] Additional section needed.
