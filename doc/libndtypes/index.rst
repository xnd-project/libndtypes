.. meta::
   :robots: index, follow
   :description: libndtypes documentation
   :keywords: datashape, libndtypes, C, Python, array computing

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


libndtypes - C library
----------------------

libndtypes implements the type part of a compiler frontend.  It can describe
C types needed for array computing and additionally includes symbolic types
for the purposes of pattern matching.


libndtypes has the concept of abstract and concrete types.  Concrete types
contain the exact data layout and all sizes that are required to access
subtypes or individual elements in memory.

Abstract types are for pattern matching and include functions, symbolic
dimensions and type variables.  Module support is planned at a later stage.


Concrete types with rich layout information make it possible to write relatively
small container libraries that can traverse memory without type erasure.


.. toctree::

   context.rst
   encodings.rst
   fields-values.rst
   predicates.rst
   typedef.rst
   types.rst
   util.rst


