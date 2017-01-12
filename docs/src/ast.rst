.. meta::
   :robots: index,follow
   :description: libndtypes abstract syntax tree
   :keywords: libndtypes, datashape, ast

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


====================
Abstract Syntax Tree
====================

An abstract syntax tree provides an excellent insight into the datashape
tree structure. Internally, `libndtypes <https://github.com/blaze/libndtypes>`_
uses an object tree that closely mirrors this presentation.


AST
---

Following CPython's tradition of specifying the AST in
`ASDL <https://github.com/python/cpython/blob/master/Parser/Python.asdl>`_
form, here we use a very similar OCaml data type:

.. code-block:: ocaml

   type size = int
   type alignment = int

   type variadic_flag
     = Nonvariadic
     | Variadic

   type encoding
     = Ascii
     | Utf8
     | Utf16
     | Utf32
     | Ucs2

   type datashape =
     (* dtypes *)
     | Void
     | Bool

     | Int8
     | Int16
     | Int32
     | Int64
     | Int128

     | Uint8
     | Uint16
     | Uint32
     | Uint64
     | Uint128

     | Float16
     | Float32
     | Float64
     | Float128

     | Complex of datashape (* argument restricted to IEEE 754-2008 floats *)

     | Char of encoding

     | String (* utf8 string *)
     | FixedString of size * encoding option

     | Bytes of alignment
     | FixedBytes of size * alignment

     | Pointer of datashape
     | Option of datashape

     | Constr of string * datashape (* general type constructor *)
   
     (* symbolic dtypes *)
     | Dtypevar of string
   
     (* dtype kinds (denoting specific subsets of dtypes) *)
     | ScalarKind
     | CategoricalKind
     | FixedBytesKind
     | FixedStringKind
   
     (* compound types *)
     | Tuple of variadic_flag * datashape list
     | Record of variadic_flag * field list
     | Function of parameters
   
     (* array types *)
     | FixedDim of size * datashape      (* equivalent to "array[size] of type" *)
     | VarDim of datashape               (* equivalent to "array of type" *)
     | SymbolicDim of string * datashape (* equivalent to "array[N] of type" *)
     | EllipsisDim of string * datashape (* any number of dimensions (... or Dim...) *)

     (* dimension kinds *)
     | FixedDimKind of datashape         (* set of all array[N] of type *)

     (* type kinds *)
     | AnyKind                           (* set of all types *)
   
   and field = (string * datashape)
   
   and parameters =
     { fun_ret: datashape;   (* any type *)
       fun_pos: datashape;   (* always a tuple *)
       fun_kwds: datashape } (* always a record *)
   


