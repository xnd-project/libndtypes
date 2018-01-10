.. meta::
   :robots: index, follow
   :description: libndtypes documentation
   :keywords: libndtypes, C, array computing

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


libndtypes
----------

libndtypes implements the type part of a compiler frontend.  It can describe
C types needed for array computing and additionally includes symbolic types
for dynamic type checking.


libndtypes has the concept of abstract and concrete types.  Concrete types
contain the exact data layout and all sizes that are required to access
subtypes or individual elements in memory.

Abstract types are for type checking and include functions, symbolic
dimensions and type variables.  Module support is planned at a later stage.


Concrete types with rich layout information make it possible to write relatively
small container libraries that can traverse memory without type erasure.


.. toctree::

   init.rst
   context.rst
   types.rst
   predicates.rst
   functions.rst
   typedef.rst
   io.rst
   encodings.rst
   fields-values.rst
   memory.rst
   util.rst


