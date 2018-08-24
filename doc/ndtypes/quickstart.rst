.. meta::
   :robots: index,follow
   :description: ndtypes quickstart
   :keywords: ndtypes examples

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Quick Start
===========

Install
-------

Prerequisites
~~~~~~~~~~~~~

Python2 is not supported. If not already present, install the Python3
development packages:

.. code-block:: sh

   # Debian, Ubuntu:
   sudo apt-get install gcc make
   sudo apt-get install python3-dev

   # Fedora, RedHat:
   sudo yum install gcc make
   sudo yum install python3-devel

   # openSUSE:
   sudo zypper install gcc make
   sudo zypper install python3-devel

   # BSD:
   # You know what to do.

   # Mac OS X:
   # Install Xcode and Python 3 headers.


Install
~~~~~~~

If `pip <http://pypi.python.org/pypi/pip>`_ is present on the system, installation
should be as easy as:

.. code-block:: sh

   pip install ndtypes


Otherwise:

.. code-block:: sh

   tar xvzf ndtypes-0.2.0dev3.tar.gz
   cd ndtypes-0.2.0dev3
   python3 setup.py install


Windows
~~~~~~~

Refer to the instructions in the *vcbuild* directory in the source distribution.


Examples
--------

The libndtypes Python bindings are mostly useful in conjunction with other
modules like the xnd module. While the underlying libndtypes does most of
the heavy-lifting for libraries like libxnd, virtually all of this happens
on the C level.


Nevertheless, some selected examples should give a good understanding of
what libndtypes and ndtypes actually do:


Create types
~~~~~~~~~~~~

The most fundamental operation is to create a type:

.. doctest::

   >>> from ndtypes import *
   >>> t = ndt("2 * 3 * int64")
   >>> t
   ndt("2 * 3 * int64")


This type describes a 2 by 3 array with an int64 data type. Types have
common and individual properties.


Type properties
~~~~~~~~~~~~~~~

All types have the following properties (continuing the example above):

.. doctest::

   >>> t.ndim
   2
   >>> t.datasize
   48
   >>> t.itemsize
   8
   >>> t.align
   8

Array types have these individual properties:

.. doctest::

   >>> t.shape
   (2, 3)

   >>> t.strides
   (24, 8)

For NumPy compatibility ndtypes displays *strides* (amount of bytes to skip).
Internally, libndtypes uses steps (amount of indices to skip).


Internals
~~~~~~~~~

This is how to display the internal type AST:

.. doctest::

   >>> print(t.ast_repr())
   FixedDim(
     FixedDim(
       Int64(access=Concrete, ndim=0, datasize=8, align=8, flags=[]),
       tag=None, shape=3, itemsize=8, step=1,
       access=Concrete, ndim=1, datasize=24, align=8, flags=[]
     ),
     tag=None, shape=2, itemsize=8, step=3,
     access=Concrete, ndim=2, datasize=48, align=8, flags=[]
   )
