.. meta::
   :robots: index,follow
   :description: ndtypes datashape specification
   :keywords: ndtypes, datashape, specification

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


.. _types:

#####
Types
#####

The set of all types comprises :ref:`dtypes <dtypes>` and :ref:`arrays <arrays>`.

The rest of this document assumes that the :py:mod:`ndtypes` module has been
imported:

.. testcode::

   from ndtypes import ndt

.. _dtypes:

******
Dtypes
******

An important notion in datashape is the ``dtype``, which roughly translates to
the element type of an array.  In datashape, the ``dtype`` can be of arbitrary
complexity and can contain e.g. tuples, records and functions.


.. _scalars:

=======
Scalars
=======

Scalars are the primitive C/C++ types. Most scalars are fixed-size and platform
independent.


Fixed size
----------

Datashape offers a number of fixed-size scalars. Here's how to construct a simple
:c:type:`int64_t` type:

.. doctest::

   >>> ndt('int64')
   ndt("int64")


All fixed-size scalars:

   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |   void    |     boolean     | signed int | unsigned int |  float [#f2]_ |        complex        |
   +===========+=================+============+==============+===============+=======================+
   | ``void``  | ``bool`` [#f1]_ |   ``int8`` |   ``uint8``  |  ``float16``  | ``complex32``         |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int16`` |  ``uint16``  |  ``float32``  | ``complex64`` [#f3]_  |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int32`` |  ``uint32``  |  ``float64``  | ``complex128`` [#f4]_ |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int64`` |  ``uint64``  |               |                       |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+

   .. [#f1] implemented as :c:type:`char`
   .. [#f2] IEEE 754-2008 binary floating point types
   .. [#f3] implemented as :c:type:`complex\<float32\>`
   .. [#f4] implemented as :c:type:`complex\<float64\>`


Aliases
-------

Datashape has a number of aliases for scalars, which are internally mapped
to their corresponding platform specific fixed-size types. This is how to
construct an :c:type:`intptr_t`:

.. doctest::

   >>> ndt('intptr')
   ndt("int64")

Machine dependent aliases:

   +-----------------+----------+------------------+
   | ``intptr``      | :c:type:`intptr_t`          |
   +-----------------+----------+------------------+
   | ``uintptr``     | :c:type:`uintptr_t`         |
   +-----------------+-----------------------------+


=====================
Chars, strings, bytes
=====================

Encodings
---------

Datashape defines the following encodings for strings and characters. Each encoding
has several aliases:

   +-----------------+----------------------+
   | canonical form  |        aliases       |
   +=================+=========+============+
   |     'ascii'     |    'A'  | 'us-ascii' |
   +-----------------+---------+------------+
   |     'utf8'      |   'U8'  |   'utf-8'  |
   +-----------------+---------+------------+
   |     'utf16'     |  'U16'  |  'utf-16'  |
   +-----------------+---------+------------+
   |     'utf32'     |  'U32'  |  'utf-32'  |
   +-----------------+---------+------------+
   |     'ucs2'      | 'ucs_2' |  'ucs2'    |
   +-----------------+---------+------------+


As seen in the table, encodings must be given in string form:

.. doctest::

   >>> ndt("char('utf16')")
   ndt("char('utf16')")


Chars
-----

The ``char`` constructor accepts ``'ascii'``, ``'ucs2'`` and ``'utf32'`` encoding
arguments.  ``char`` without arguments is equivalent to ``char(utf32)``.

.. doctest::

   >>> ndt("char('ascii')")
   ndt("char('ascii')")

   >>> ndt("char('utf32')")
   ndt("char('utf32')")

   >>> ndt("char")
   ndt("char('utf32')")


UTF-8 strings
-------------

The ``string`` type is a variable length NUL-terminated UTF-8 string:

.. doctest::

   >>> ndt("string")
   ndt("string")


.. _fixed-string:

Fixed size strings
------------------

The ``fixed_string`` type takes a length and an optional encoding argument:

.. doctest::

   >>> ndt("fixed_string(1729)")
   ndt("fixed_string(1729)")

   >>> ndt("fixed_string(1729, 'utf16')")
   ndt("fixed_string(1729, 'utf16')")


Bytes
-----

The `bytes` type is variable length and takes an optional alignment argument.
Valid values are powers of two in the range ``[1, 16]``.

.. doctest::

   >>> ndt("bytes")
   ndt("bytes")

   >>> ndt("bytes(align=2)")
   ndt("bytes(align=2)")


.. _fixed-bytes:

Fixed size bytes
----------------

The ``fixed_bytes`` type takes a length and an optional alignment argument.
The latter is a keyword-only argument in order to prevent accidental swapping of
the two integer arguments:

.. doctest::

   >>> ndt("fixed_bytes(size=32)")
   ndt("fixed_bytes(size=32)")

   >>> ndt("fixed_bytes(size=128, align=8)")
   ndt("fixed_bytes(size=128, align=8)")


==========
References
==========

Datashape references are fully general and can point to types of arbitrary
complexity:

.. doctest::

   >>> ndt("ref(int64)")
   ndt("ref(int64)")

   >>> ndt("ref(10 * {a: int64, b: 10 * float64})")
   ndt("ref(10 * {a : int64, b : 10 * float64})")


================
Categorical type
================

The categorical type allows to specify subsets of types. This is implemented
as a set of typed values. Types are inferred and interpreted as int64, float64
or strings. The *NA* keyword creates a category for missing values.

.. doctest::

   >>> ndt("categorical(1, 10)")
   ndt("categorical(1, 10)")

   >>> ndt("categorical(1.2, 100.0)")
   ndt("categorical(1.2, 100)")

   >>> ndt("categorical('January', 'August')")
   ndt("categorical('January', 'August')")

   >>> ndt("categorical('January', 'August', NA)")
   ndt("categorical('January', 'August', NA)")


===========
Option type
===========

The option type provides safe handling of values that may or may not be present.
The concept is well-known from languages like ML or SQL.

.. doctest::

   >>> ndt("?complex64")
   ndt("?complex64")


.. _dtype-variables:

===============
Dtype variables
===============

Dtype variables are used in quantifier free type schemes and pattern matching.
The range of a variable extends over the entire type term.

.. doctest::

   >>> ndt("T")
   ndt("T")

   >>> ndt("10 * 16 * T")
   ndt("10 * 16 * T")


.. _symbolic-constructors:

=====================
Symbolic constructors
=====================

Symbolic constructors stand for any constructor that takes the given datashape
argument. Used in pattern matching.

.. doctest::

   >>> ndt("Coulomb(float64)")
   ndt("Coulomb(float64)")


.. _type-kinds:

==========
Type kinds
==========

Type kinds denote specific subsets of :ref:`dtypes <dtypes>`, :ref:`types <types>`
or :ref:`dimension types <arrays>`. Type kinds are in the dtype section because
of the way the grammar is organized. Currently available are:

   +---------------------+-------------------------------+-------------------------------+
   |   type kind         |              set              |       specific subset         |
   +=====================+===============================+===============================+
   | ``Any``             | ``datashape``                 | ``datashape``                 |
   +---------------------+-------------------------------+-------------------------------+
   | ``Scalar``          | ``dtypes``                    | ``scalars``                   |
   +---------------------+-------------------------------+-------------------------------+
   | ``Categorical``     | ``dtypes``                    | ``categoricals``              |
   +---------------------+-------------------------------+-------------------------------+
   | ``FixedString``     | ``dtypes``                    | ``fixed_strings``             |
   +---------------------+-------------------------------+-------------------------------+
   | ``FixedBytes``      | ``dtypes``                    | ``fixed_bytes``               |
   +---------------------+-------------------------------+-------------------------------+
   | ``Fixed``           | ``dimension kind instances``  | ``fixed dimensions``          |
   +---------------------+-------------------------------+-------------------------------+


Type kinds are used in :ref:`pattern matching <type-kinds-matching>`.


===============
Composite types
===============

Datashape has container and function :ref:`dtypes <dtypes>`.

Tuples
------

As usual, the tuple type is the product type of a fixed number of types:

.. doctest::

   >>> ndt("(int64, float32, string)")
   ndt("(int64, float32, string)")


Tuples can be nested:

.. doctest::

   >>> ndt("(bytes, (int8, fixed_string(10)))")
   ndt("(bytes, (int8, fixed_string(10)))")


Records
-------

Records are equivalent to tuples with named fields:

.. doctest::

   >>> ndt("{a: float32, b: float64}")
   ndt("{a : float32, b : float64}")


Functions
---------

In datashape, function types can have positional and keyword arguments.
Internally, positional arguments are represented by a tuple and keyword
arguments by a record.  Both kinds of arguments can be variadic.


Positional-only
~~~~~~~~~~~~~~~

This is a function type with a single positional ``int32`` argument, returning
an ``int32``:

.. doctest::

  >>> ndt("(int32) -> int32")
  ndt("(int32) -> int32")


This is a function type with three positional arguments:

.. doctest::

   >>> ndt("(int32, complex128, string) -> float64")
   ndt("(int32, complex128, string) -> float64")


Positional-variadic
~~~~~~~~~~~~~~~~~~~

This is a function type with a single required positional argument,
followed by any number of additional positional arguments:

.. doctest::

   >>> ndt("(int32, ...) -> int32")
   ndt("(int32, ...) -> int32")



.. _arrays:

******
Arrays
******

In datashape dimension kinds [#f6]_ are part of array type declarations. Datashape
supports the following dimension kinds:


.. _fixed-dimension:

===============
Fixed Dimension
===============

A fixed dimension denotes an array type with a fixed number of elements of
a specific type.  The type can be written in two ways:

.. doctest::

   >>> ndt("fixed(shape=10) * uint64")
   ndt("10 * uint64")

   >>> ndt("10 * uint64")
   ndt("10 * uint64")

Formally, ``fixed(shape=10)`` is a dimension constructor, not a type constructor.
The ``*`` is the array type constructor in infix notation, taking as arguments
a dimension and an element type.

The second form is equivalent to the first one.  For users of other languages,
it may be helpful to view this type as ``array[10] of uint64``.


Multidimensional arrays are constructed in the same manner, the ``*`` is
right associative:

.. doctest::

   >>> ndt("10 * 25 * float64")
   ndt("10 * 25 * float64")


Again, it may help to view this type as ``array[10] of (array[25] of float64)``.

In this case, ``float64`` is the :ref:`dtype <dtypes>` of the multidimensional
array.

Dtypes can be arbitrarily complex. Here is an array with a dtype of a record that
contains another array:

.. doctest::

   >>> ndt("120 * {size: int32, items: 10 * int8}")
   ndt("120 * {size : int32, items : 10 * int8}")


.. _variable-dimension:

==================
Variable Dimension
==================

The variable dimension kind describes an array type with a variable number
of elements of a specific type:

.. doctest::

   >>> ndt("var * float32")
   ndt("var * float32")

In this case, ``var`` is the dimension constructor and the ``*`` fulfils the
same role as above. Many managed languages have variable sized arrays, so this
type could be viewed as ``array of float32``. In a sense, fixed size arrays
are just a special case of variable sized arrays.


.. _symbolic-dim:

==================
Symbolic Dimension
==================

Datashape supports symbolic dimensions, which are used in pattern matching. A
symbolic dimension is an uppercase variable that stands for a fixed dimension.

In this manner entire sets of array types can be specified.  The following type
describes the set of all ``M * N`` matrices with a ``float32`` dtype: 

.. doctest::

   >>> ndt("M * N * float32")
   ndt("M * N * float32")


The next type describes a function that performs matrix multiplication on any
permissible pair of input matrices with dtype ``T``:

.. doctest::

   >>> ndt("(M * N * T, N * P * T) -> M * P * T")
   ndt("(M * N * T, N * P * T) -> M * P * T")

In this case, we have used both symbolic dimensions and the type variable ``T``.


Symbolic dimensions can be mixed fixed dimensions:

.. doctest::

   >>> ndt("10 * N * float64")
   ndt("10 * N * float64")


.. _ellipsis-dim:

==================
Ellipsis Dimension
==================

The ellipsis, used in pattern matching, stands for any number of dimensions.
Datashape supports both named and unnamed ellipses:

.. doctest::

   >>> ndt("... * float32")
   ndt("... * float32")


Named form:

.. doctest::

   >>> ndt("Dim... * float32")
   ndt("Dim... * float32")

Ellipsis dimensions play an important role in broadcasting, more on the topic
in the section on pattern matching.

|
|

.. [#f6] In the whole text *dimension kind* and *dimension* are synonymous.
