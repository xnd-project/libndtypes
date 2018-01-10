
ndtypes
=======

ndtypes is a package for typing raw memory blocks using a close variant of
the datashape type language.


libndtypes
----------

libndtypes implements the type part of a compiler frontend. It can describe
C types needed for array computing and additionally includes symbolic types
for the purposes of dynamic type checking.

libndtypes has the concept of abstract and concrete types. Concrete types
contain the exact data layout and all sizes that are required to access
subtypes or individual elements in memory.

Abstract types are for type checking and include functions, symbolic dimensions
and type variables. Module support is planned at a later stage.

Concrete types with rich layout information make it possible to write
relatively small container libraries that can traverse memory without
type erasure.


http://ndtypes.readthedocs.io/en/latest/


ndtypes (Python module)
-----------------------

The ndtypes Python module implements bindings for libndtypes.  Its purpose
is to support the type part required for the xnd container module.

http://ndtypes.readthedocs.io/en/latest/

http://xnd.readthedocs.io/en/latest/
