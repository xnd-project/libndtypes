.. meta::
   :robots: index,follow
   :description: libndtypes2 datashape specification
   :keywords: libndtypes2, datashape, specification

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


.. _datashape:

#########
Datashape
#########

.. warning::
   This section uses Python bindings that still need to be written.

In this section we will be using the :py:mod:`ndt` module (See:
`ndtypes-python <https://github.com/blaze/ndtypes-python>`_) to demonstrate the
capabilities of datashape. The rest of this document assumes that the :py:mod:`ndt`
module has been imported:

.. code-block:: py

   from ndtypes import ndt


.. _types:

*****
Types
*****

The set of all datashape types comprises :ref:`dtypes <dtypes>` and
:ref:`arrays <arrays>`.


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

.. code-block:: py

   >>> ndt.type('int64')
   ndt.type('int64')


All fixed-size scalars:

   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |   void    |     boolean     | signed int | unsigned int |  float [#f2]_ |        complex        |
   +===========+=================+============+==============+===============+=======================+
   | ``void``  | ``bool`` [#f1]_ |   ``int8`` |   ``uint8``  |  ``float16``  | ``complex64`` [#f3]_  |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int16`` |  ``uint16``  |  ``float32``  | ``complex128`` [#f4]_ |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int32`` |  ``uint32``  |  ``float64``  |                       |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 |  ``int64`` |  ``uint64``  | ``float128``  |                       |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+
   |           |                 | ``int128`` | ``uint128``  |               |                       |
   +-----------+-----------------+------------+--------------+---------------+-----------------------+

   .. [#f1] implemented as :c:type:`char`
   .. [#f2] IEEE 754-2008 binary floating point types
   .. [#f3] implemented as :c:type:`complex\<float32\>`
   .. [#f4] implemented as :c:type:`complex\<float64\>`


Aliases
-------

Datashape has a number of aliases for scalars, which are internally mapped
to their corresponding platform specific fixed-size types. This is how to
construct a :c:type:`size_t`:

.. code-block:: py

   >>> ndt.type('size')
   ndt.type('uint64')


Machine dependent aliases:

   +-----------------+----------+------------------+
   | ``intptr``      | :c:type:`intptr_t`          |
   +-----------------+----------+------------------+
   | ``size``        | :c:type:`size_t`            |
   +-----------------+-----------------------------+
   | ``uintptr``     | :c:type:`uintptr_t`         |
   +-----------------+-----------------------------+

Machine independent aliases:

   +--------------+------------------------------+
   | ``int``      | :c:type:`int32_t`            |
   +--------------+------------------------------+
   | ``real``     | :c:type:`float64`            |
   +--------------+------------------------------+
   | ``complex``  | :c:type:`complex\<float64\>` |
   +--------------+------------------------------+

Since IEEE 754-2008 is required, ``sizeof(double) == 8`` and ``double`` maps
to ``float64``.


Complex
-------

Even though complex numbers are scalars, datashape provides a type constructor
syntax for them:

.. code-block:: py

   >>> ndt.type("complex[float32]")
   ndt.type('complex[float32]')

   >>> ndt.type("complex64")
   ndt.type('complex[float32]')

   >>> ndt.type("complex[real]")
   ndt.type('complex[float64]')

   >>> ndt.type("complex[float64]")
   ndt.type('complex[float64]')


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

.. code-block:: py

   >>> ndt.type("char['ucs2']")
   ndt.type("char['ucs2']")


Chars
-----

The ``char`` constructor accepts ``'ascii'``, ``'ucs2'`` and ``'utf32'`` encoding
arguments.  ``char`` without arguments is equivalent to ``char[utf32]``.

.. code-block:: py

   >>> ndt.type("char['ascii']")
   ndt.type("char['ascii']")

   >>> ndt.type("char['utf32']")
   ndt.type('char')

   >>> ndt.type("char")
   ndt.type('char')


UTF-8 strings
-------------

The ``string`` type is a variable length UTF-8 string:

.. code-block:: py

   >>> t = ndt.type("string")
   >>> t.encoding
   'utf8'


.. _fixed-string:

Fixed size strings
------------------

The ``fixed_string`` type takes a length and an optional encoding argument:

.. code-block:: py

   >>> t = ndt.type("fixed_string[1729]")
   >>> t.data_size     
   1729
   >>> t.encoding 
   'utf8'

   >>> t = ndt.type("fixed_string[1729, 'utf16']")
   >>> t.data_size
   3458
   >>> t.encoding
   'utf16'


Bytes
-----

The `bytes` type is variable length and takes an optional alignment argument.
Valid values are powers of two in the range ``[1, 16]``.

.. code-block:: py

   >>> t = ndt.type("bytes")
   >>> t.data_alignment
   8
   >>> t.target_alignment
   1

   >>> t = ndt.type("bytes[align=2]")
   >>> t.data_alignment
   8
   >>> t.target_alignment
   2


.. _fixed-bytes:

Fixed size bytes
----------------

The ``fixed_bytes`` type takes a length and an optional alignment argument.
The latter is a keyword-only argument in order to prevent accidental swapping of
the two integer arguments:

.. code-block:: py

   >>> t = ndt.type("fixed_bytes[32]")
   >>> t.data_size
   32
   >>> t.data_alignment
   1

   >>> t = ndt.type("fixed_bytes[128, align=8]")
   >>> t.data_size
   128
   >>> t.data_alignment
   8


========
Pointers
========

Datashape pointers are fully general and can point to types of arbitrary
complexity:

.. code-block:: py

   >>> ndt.type("pointer[int64]")
   ndt.type('pointer[int64]')

   >>> ndt.type("pointer[10 * {a: int, b: 10 * float64}]")
   ndt.type('pointer[10 * {a: int32, b: 10 * float64}]')


================
Categorical type
================

The categorical type allows to specify subsets of types. This is implemented
as a set of typed values. Currently all signed and unsigned, float, complex
and the string type are supported:

.. code-block:: py

   >>> ndt.type("categorical[1 : int64, 10 : int64]")
   ndt.type("categorical[1 : int64, 10 : int64]")

   >>> ndt.type("categorical[1 : int64, "this" : string]")
   ndt.type("categorical[1 : int64, 10 : int64]")


===========
Option type
===========

The option type provides safe handling of values that may or may not be present.
The concept is well-known from languages like ML or SQL.

Two equivalent notations exist:

.. code-block:: py

   >>> ndt.type("option[complex]")
   ndt.type('?complex[float64]')

   >>> ndt.type('?complex[float64]')
   ndt.type('?complex[float64]')


.. _dtype-variables:

===============
Dtype variables
===============

Dtype variables are used in quantifier free type schemes and pattern matching.
The range of a variable extends over the entire type term.

.. code-block:: py

   >>> ndt.type("T")
   ndt.type('T')

   >>> ndt.type("10 * 16 * T")
   ndt.type('10 * 16 * T')


.. _symbolic-constructors:

=====================
Symbolic constructors
=====================

Symbolic constructors stand for any constructor that takes the given datashape
argument. Used in pattern matching.

.. code-block:: py

   >>> ndt.type("T[int32]")
   ndt.type('T[int32]')


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

.. code-block:: py

   >>> ndt.type("(int64, float32, string)")
   ndt.type('(int64, float32, string)')


Tuples can be nested:

.. code-block:: py

   >>> ndt.type("(bytes, (int8, fixed_string[10]))")
   ndt.type('(bytes, (int8, fixed_string[10]))')


Records
-------

Records are equivalent to tuples with named fields:

.. code-block:: py

   >>> ndt.type("{a: float32, b: float64}")
   ndt.type('{a: float32, b: float64}')


Functions
---------

In datashape, function types can have positional and keyword arguments.
Internally, positional arguments are represented by a tuple and keyword
arguments by a record.  Both kinds of arguments can be variadic.


Positional-only
~~~~~~~~~~~~~~~

This is a function type with a single positional ``int32`` argument, returning
an ``int32``:

.. code-block:: py

  >>> ndt.type("(int32) -> int32")
  ndt.type('(int32) -> int32')


This is a function type with three positional arguments:

.. code-block:: py

   >>> ndt.type("(int32, complex128, string) -> float64")
   ndt.type('(int32, complex[float64], string) -> float64')


Positional-variadic
~~~~~~~~~~~~~~~~~~~

This is a function type with a single required positional argument,
followed by any number of additional positional arguments:

.. code-block:: py

   >>> ndt.type("(int32, ...) -> int32")
   ndt.type('(int32, ...) -> int32')


Keyword-only
~~~~~~~~~~~~

Keywords are specified inline:

.. code-block:: py

   >>> ndt.type("(distance: float32, velocity: float32) -> float32")
   ndt.type('(distance: float32, velocity: float32) -> float32')


Keyword-variadic
~~~~~~~~~~~~~~~~

This is a function type with a single required keyword argument,
followed by any number of additional keyword arguments:

.. code-block:: py

   >>> ndt.type("(sum: float64, ...) -> float64")
   ndt.type('(sum: float64, ...) -> float64')

 
Mixed
~~~~~

Function types can have both positional and keyword arguments, the former
must precede the latter:

.. code-block:: py

   >>> ndt.type("(uint32, uint32, product: float64) -> float64")
   ndt.type('(uint32, uint32, product: float64) -> float64')

 
Mixed-variadic
~~~~~~~~~~~~~~

Any combination of positional-variadic and keyword-variadic is permitted.

This function has positional-variadic arguments, followed by keyword
arguments:

.. code-block:: py

   >>> ndt.type("(uint64, ..., scale: uint8) -> uint64")
   ndt.type('(uint64, ..., scale: uint8) -> uint64')

Positional arguments, followed by keyword-variadic arguments:

.. code-block:: py

   >>> ndt.type("(uint64, scale: uint8, ...) -> uint64")
   ndt.type('(uint64, scale: uint8, ...) -> uint64')

Positional-variadic and keyword-variadic:

.. code-block:: py

   >>> ndt.type("(..., color: uint32, ...) -> uint64")
   ndt.type('(..., color: uint32, ...) -> uint64')


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

.. code-block:: py

   >>> ndt.type("fixed[10] * uint64")
   ndt.type('10 * uint64')

   >>> ndt.type("10 * uint64")
   ndt.type('10 * uint64')

Formally, ``fixed[10]`` is a dimension constructor, not a type constructor.
The ``*`` is the array type constructor in infix notation, taking as arguments
a dimension and an element type.

The second form is equivalent to the first one.  For users of other languages,
it may be helpful to view this type as ``array[10] of uint64``.


Multidimensional arrays are constructed in the same manner, the ``*`` is
right associative:

.. code-block:: py

   >>> ndt.type("10 * 25 * float64")
   ndt.type('10 * 25 * float64')


Again, it may help to view this type as ``array[10] of (array[25] of float64)``.

In this case, ``float64`` is the :ref:`dtype <dtypes>` of the multidimensional
array.

Dtypes can be arbitrarily complex. Here is an array with a dtype of a record that
contains another array:

.. code-block:: py

   >>> ndt.type("120 * {size: int32, items: 10 * int8}")
   ndt.type('120 * {size: int32, items: 10 * int8}')


.. _variable-dimension:

==================
Variable Dimension
==================

The variable dimension kind describes an array type with a variable number
of elements of a specific type:

.. code-block:: py

   >>> ndt.type("var * float32")
   ndt.type('var * float32')

In this case, ``var`` is the dimension constructor and the ``*`` fulfils the
same role as above. Many managed languages have variable sized arrays, so this
type could be viewed as ``array of float32``. In a sense, fixed size arrays
are just a special case of variable sized arrays.


Dimension kinds can be mixed freely:

.. code-block:: py

   >>> ndt.type("10 * var * char")
   ndt.type('10 * var * char')

Which corresponds to ``array[10] of (array of char)``.


.. _symbolic-dim:

==================
Symbolic Dimension
==================

Datashape supports symbolic dimensions, which are used in pattern matching. A
symbolic dimension is an uppercase variable that stands for a fixed dimension [#f7]_.

In this manner entire sets of array types can be specified.  The following type
describes the set of all ``M * N`` matrices with a ``float32`` dtype: 

.. code-block:: py

   >>> ndt.type("M * N * float32")
   ndt.type('M * N * float32')


The next type describes a function that performs matrix multiplication on any
permissible pair of input matrices with dtype ``T``:

.. code-block:: py

   >>> ndt.type("(M * N * T, N * P * T) -> M * P * T")
   ndt.type('(M * N * T, N * P * T) -> M * P * T')

In this case, we have used both symbolic dimensions and the type variable ``T``.


Symbolic dimensions can be mixed with other dimension kinds:

.. code-block:: py

   >>> ndt.type("10 * N * var * real")
   ndt.type('10 * N * var * float64')


.. _ellipsis-dim:

==================
Ellipsis Dimension
==================

The ellipsis, used in pattern matching, stands for any number of dimensions.
Datashape supports both named and unnamed ellipses:

.. code-block:: py

   >>> ndt.type("... * float32")
   ndt.type('M * N * float32')


Named form:

.. code-block:: py

   >>> ndt.type("Dim... * float32")
   ndt.type('Dim... * float32')

Ellipsis dimensions play an important role in broadcasting, more on the topic
in the section on pattern matching.


.. _power-dim:

===============
Power Dimension
===============

Power dimensions are syntactic sugar for repeated dimension kinds:

.. code-block:: py

   >>> ndt.type("128**2 * float32")
   ndt.type('128 * 128 * float32')

   >>> ndt.type("var**3 * (complex, complex)")
   ndt.type('var * var * var * (complex[float64], complex[float64])')

   >>> ndt.type("N**3 * {a: int32, b: int64}")
   ndt.type('N * N * N * {a: int32, b: int64}')

Ellipsis dimensions cannot be repeated.



|
|

.. [#f6] In the whole text *dimension kind* and *dimension* are synonymous.

.. [#f7] It is currently under debate if symbolic dimensions should be
         restricted to match fixed dimensions only.


