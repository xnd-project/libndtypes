.. meta::
   :robots: index,follow
   :description: libndtypes datashape pattern matching
   :keywords: libndtypes, datashape, pattern matching

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

:py:class:`ndt.type` instances have a :py:meth:`match` method for determining
whether the argument type is compatible with the instance type. The match
succeeds if and only if the set of types described by the right hand side
is a subset of the set of types described by the left hand side.


Simple example
--------------

.. code-block:: py

   >>> ndt.type("Any").match("int32")
   True


Non-commutativity
-----------------

From the above definition it follows that pattern matching is not commutative:

.. code-block:: py

   >>> ndt.type("int32").match("Any")
   False


Concrete matching
-----------------

Much like members of the alphabet in regular expressions, concrete types
match themselves:

.. code-block:: py

   >>> ndt.type("int32").match("int32")
   True

   >>> ndt.type("10 * var * float32").match("10 * var * float32")
   True

   >>> ndt.type("10 * var * float64").match("10 * var * float32")
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

   >>> ndt.type("(Any) -> Any").match("(float64) -> int32")
   True


Any
---

The *Any* type kind is the most general and describes the set of all
:ref:`types <types>`.

Here's how to match a dtype against the set of all types:

.. code-block:: py

   >>> ndt.type("Any").match("int32")
   True


This matches an array type against the set of all types:

.. code-block:: py

   >>> ndt.type("Any").match("10 * 5 * { v: float64, t: float64 }")
   True


Scalar
------

The *Scalar* type kind stands for the set of all :ref:`scalars <scalars>`.

:c:type:`int32` is a member of the set of all scalars:

.. code-block:: py

   >>> ndt.type("Scalar").match("int32")
   True


A pattern for a function that takes any type and returns a scalar:

.. code-block:: py

   >>> ndt.type("(Any) -> Scalar").match("(10 * complex128) -> float64")
   True

   >>> ndt.type("(Any) -> Scalar").match("(?{a: 10 * uint8}) -> uint8")
   True

   >>> ndt.type("(Any) -> Scalar").match("(?{a: 10 * uint8}) -> 10 * uint8")
   False


Only the type kind is important, not the concrete type:

.. code-block:: py

   >>> ndt.type("(Scalar, Scalar)").match("(uint8, float64)")
   True


Categorical
-----------

The set of all categorical types. Categorical types are currently not implemented.



FixedString
-----------

The set of all :ref:`fixed string <fixed-string>` types.

.. code-block:: py

   >>> ndt.type("FixedString").match("fixed_string[100]")
   True

   >>> ndt.type("FixedString").match("fixed_string[100, 'utf16']")
   True

   >>> ndt.type("FixedString").match("string")
   False


FixedBytes
----------

The set of all :ref:`fixed bytes <fixed-bytes>` types.

.. code-block:: py

   >>> ndt.type("FixedBytes").match("fixed_bytes[100]")
   True

   >>> ndt.type("FixedBytes").match("fixed_bytes[100, align=2]")
   True

   >>> ndt.type("FixedBytes").match("bytes[align=2]")
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

   >>> ndt.type("Fixed * var * bool").match("10 * var * bool")
   True

   >>> ndt.type("Fixed * var * bool").match("var * var * bool")
   False

   >>> ndt.type("Fixed * var * bool").match("N * var * bool")
   False


.. _dtype-vars-matching:

***************
Dtype variables
***************

:ref:`dtype variables <dtype-variables>` are placeholders for dtypes. It is important
to note that they are *not* general type variables.  For example, they do not match
:ref:`array types <arrays>`, a concept which is used in general array functions [#f3]_,
whose base cases may operate on a dtype.


This matches a record against a single :ref:`dtype <dtypes>` variable:

.. code-block:: py

   >>> ndt.type("T").match("{v: float64, t: float64}")
   True


An :ref:`array <arrays>` is not a :ref:`dtype <dtypes>`, so this match fails:

.. code-block:: py

   >>> ndt.type("T").match("10 * 5 * {v: float64, t: float64}")
   False


Match against several dtype variables in a tuple type:

.. code-block:: py

   >>> ndt.type("(T, T, S)").match("(int32, int32, bool)")
   True

   >>> ndt.type("(T, T, S)").match("(int32, int64, bool)")
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
fixed size [#f2]_ arrays:

.. code-block:: py

   >>> ndt.type("N * float64").match(ndt.type("100 * float64"))
   True


Symbolic-symbolic match
-----------------------

Symbolic dimensions also match against other symbolic dimensions:

.. code-block:: py

   >>> ndt.type("N * float64").match(ndt.type("M * float64"))
   True


Symbolic+Dtypevar
-----------------

Symbolic dimensions can be used in conjunction with dtype variables:

.. code-block:: py

   >>> ndt.type("N * T").match(ndt.type("10 * float32"))
   True


While it is prudent to use standard variable naming conventions like above,
symbolic variables and dtype variables can have the same name:

.. code-block:: py

   >>> ndt.type("N * N").match(ndt.type("10 * float32"))
   True

Obviously this form is strongly discouraged.


Ellipsis match
--------------

Finally, all dimension kinds (including multiple dimensions) match against
ellipsis dimensions (named or unnamed):

.. code-block:: py

   >>> ndt.type("... * float64").match(ndt.type("N * float64"))
   True

   >>> ndt.type("... * float64").match(ndt.type("10 * N * float64"))
   True

   >>> ndt.type("Dim... * float64").match(ndt.type("10 * 20 * float64"))
   True


This is used in broadcasting [#f3]_.



|
|

.. [#f1] An argument is often made that the term *dependent types* should
         be reserved for static type systems.  We use it here while explicitly
         acknowledging that the datashape implementation is dynamically typed.

.. [#f2] It is currently under debate whether symbolic dimensions should only
         match fixed size dimensions or also include variable size dimensions.

.. [#f3] Additional section needed.
