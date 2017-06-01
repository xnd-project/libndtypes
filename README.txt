
Experimental Datashape C library.

Datashape is a specification language to describe any data-structure relevant for analytic computing.   See http://datashape.readthedocs.io/en/latest/

Datashape is a generalization of NumPy's dtype and Python's struct-syntax for describing buffers.

This library is a C-implementation of a parser for the Datashape specification to enable any language to easily parse data-shape 
and use it to describe function dispatch as well as data-interchange.

Sometimes one speaks of High-level Datashape to describe machine-independent general "schemas".  

Low-level data-shape must be sufficiently specific to describe exactly the bits that 
are laid out in a storage substrate (memory or disk). 

To accomplish the latter, datashape has the concept of a datshape "template" and potentially arbitrary "meta-data" that is necessary to complete the low-level specification.  For example, in an N-d array like NumPy, the meta-data would be the specific strides and possibly the shape.

An important use-case of this library is to support function dispatch to implementations for analytic computation (see libgufunc).



