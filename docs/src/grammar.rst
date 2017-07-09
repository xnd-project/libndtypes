.. meta::
   :robots: index,follow
   :description: libndtypes datashape documentation

.. sectionauthor:: Stefan Krah <skrah at bytereef.org>


Grammar
=======

Lexing
------

The following table contains lexemes mapped to their corresponding tokens.


Lexeme/Token Table
------------------

.. code-block:: ocaml

   newline        [\n\r]
   space          [ \t\f]
   non_newline    [^\n\r]
   comment        #{non_newline}*

   escapeseq        \\.
   single_strchar [^\\\n']
   double_strchar [^\\\n"]
   single_str     '({single_strchar}|{escapeseq})*'
   double_str     \"({double_strchar}|{escapeseq})*\"
   stringlit      {single_str}|{double_str}

   octdigit       [0-7]
   octinteger     0[oO]{octdigit}+
   nonzerodigit   [1-9]
   digit          [0-9]
   decimalinteger {nonzerodigit}{digit}*|0+
   hexdigit       {digit}|[a-f]|[A-F]
   hexinteger     0[xX]{hexdigit}+
   integer        -?({decimalinteger}|{octinteger}|{hexinteger})

   intpart        {digit}+
   fraction       \.{digit}+
   exponent       [eE][+-]?{digit}+
   pointfloat     {intpart}?{fraction}|{intpart}\.
   exponentfloat  ({intpart}|{pointfloat}){exponent}
   floatnumber    -?({pointfloat}|{exponentfloat})

   name_lower     [a-z][a-zA-Z0-9_]*
   name_upper     [A-Z][a-zA-Z0-9_]*
   name_other     _[a-zA-Z0-9_]*

   "Any" -> ANY_KIND
   "Scalar" -> SCALAR_KIND

   "void" -> VOID
   "bool" -> BOOL

   "Signed" -> SIGNED_KIND
   "int8" -> INT8
   "int16" -> INT16
   "int32" -> INT32
   "int64" -> INT64
   "int128" -> INT128

   "Unsigned" -> UNSIGNED_KIND
   "uint8" -> UINT8
   "uint16" -> UINT16
   "uint32" -> UINT32
   "uint64" -> UINT64
   "uint128" -> UINT128

   "Float" -> FLOAT_KIND
   "float16" -> FLOAT16
   "float32" -> FLOAT32
   "float64" -> FLOAT64
   "float128" -> FLOAT128

   "Complex" -> COMPLEX_KIND
   "complex64" -> COMPLEX64
   "complex128" -> COMPLEX128

   "intptr" -> INTPTR
   "uintptr" -> UINTPTR
   "size" -> SIZE
   "char" -> CHAR
   "string" -> STRING
   "bytes" -> BYTES

   "FixedString" -> FIXED_STRING_KIND
   "fixed_string" -> FIXED_STRING

   "FixedBytes" -> FIXED_BYTES_KIND
   "fixed_bytes" -> FIXED_BYTES

   "categorical" -> CATEGORICAL

   "nominal" -> NOMINAL

   "pointer" -> POINTER

   "option" -> OPTION

   "Fixed" -> FIXED_DIM_KIND
   "fixed" -> FIXED
   "var" -> VAR

   "align" -> ALIGN

   "..." -> ELLIPSIS; }
   "**" -> DOUBLESTAR; }
   "->" -> RARROW
   "," -> COMMA
   ":" -> COLON
   "(" -> LPAREN
   ")" -> RPAREN
   "{" -> LBRACE
   "}" -> RBRACE
   "[" -> LBRACK
   "]" -> RBRACK
   "*" -> STAR
   "=" -> EQUAL
   "?" -> QUESTIONMARK


Encodings
---------

:c:type:`fixed_string` and :c:type:`char` take encoding arguments, which
must be given as string literals. The preferred spelling uses single quotes
(here double quotes are used for better syntax highlighting):

.. code-block:: ocaml

   "A"    | "ascii" | "us-ascii" -> Ascii
   "U8"   | "utf8"  | "utf-8"    -> Utf8
   "U16"  | "utf16" | "utf-16"   -> Utf16
   "U32"  | "utf32" | "utf-32"   -> Utf32
   "ucs2" | "ucs-2" | "ucs_2"    -> Ucs2


Grammar
-------

This is the actual grammar in BNF form:

.. code-block:: ocaml

   input:
     datashape ENDMARKER
   
   datashape:
     array
   | dtype
   
   array:
     array_nooption
   | QUESTIONMARK array_nooption
   | OPTION LBRACK array_nooption RBRACK
   
   array_nooption:
     dimension_seq STAR dtype
   
   dimension_seq:
     dimension
   | dimension_seq STAR dimension
   
   dimension:
     FIXED_DIM_KIND
   | INTEGER
   | FIXED LBRACK INTEGER RBRACK
   | NAME_UPPER
   | VAR
   | ELLIPSIS
   
   dtype:
     dtype_nooption
   | QUESTIONMARK dtype_nooption
   | OPTION LBRACK dtype_nooption RBRACK
   
   dtype_nooption:
     ANY_KIND
   | SCALAR_KIND
   | scalar
   | tuple_type
   | record_type
   | function_type
   | NAME_LOWER
   | NAME_UPPER LBRACK dtype RBRACK
   | NAME_UPPER
   
   scalar:
     VOID
   | BOOL
   | SIGNED_KIND
   | signed
   | UNSIGNED_KIND
   | unsigned
   | FLOAT_KIND
   | ieee_float
   | COMPLEX_KIND
   | ieee_complex
   | alias
   | character
   | string
   | FIXED_STRING_KIND
   | fixed_string
   | bytes
   | FIXED_BYTES_KIND
   | fixed_bytes
   | categorical
   | pointer
   
   signed:
     INT8
   | INT16
   | INT32
   | INT64
   
   unsigned:
     UINT8
   | UINT16
   | UINT32
   | UINT64
   
   ieee_float:
     FLOAT16
   | FLOAT32
   | FLOAT64
   
   ieee_complex:
     COMPLEX64
   | COMPLEX128
   | COMPLEX LBRACK FLOAT32 RBRACK
   | COMPLEX LBRACK FLOAT64 RBRACK
   
   alias:
     INTPTR
   | UINTPTR
   | SIZE
   
   character:
     CHAR
   | CHAR LBRACK encoding RBRACK
   
   string:
     STRING
   
   fixed_string:
     FIXED_STRING LBRACK INTEGER RBRACK
   | FIXED_STRING LBRACK INTEGER COMMA encoding RBRACK
   
   encoding:
     STRINGLIT
   
   bytes:
     BYTES LBRACK ALIGN EQUAL INTEGER RBRACK
   
   fixed_bytes:
     FIXED_BYTES LBRACK INTEGER COMMA ALIGN EQUAL INTEGER RBRACK
   
   pointer:
     POINTER LBRACK datashape RBRACK 
   
   categorical:
     CATEGORICAL LBRACK typed_value_seq RBRACK
   
   typed_value_seq:
     typed_value
   | typed_value_seq COMMA typed_value
   
   typed_value:
     INTEGER COLON datashape
   | FLOATNUMBER COLON datashape
   | STRINGLIT COLON datashape
   
   variadic_flag:
     %empty
   | ELLIPSIS
   
   comma_variadic_flag:
     %empty
   | COMMA
   | COMMA ELLIPSIS
   
   tuple_type:
     LPAREN variadic_flag RPAREN
   | LPAREN tuple_field_seq comma_variadic_flag RPAREN
   
   tuple_field_seq:
     tuple_field
   | tuple_field_seq COMMA tuple_field
   
   tuple_field:
     datashape
   
   record_type:
     LBRACE variadic_flag RBRACE
   | LBRACE record_field_seq comma_variadic_flag RBRACE
   
   record_field_seq:
     record_field
   | record_field_seq COMMA record_field 
   
   record_field:
     record_field_name COLON datashape
   
   record_field_name:
     NAME_LOWER
   | NAME_UPPER
   | NAME_OTHER
   
   function_type:
     tuple_type RARROW datashape
   | LPAREN record_field_seq comma_variadic_flag RPAREN RARROW datashape
   | LPAREN ELLIPSIS COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
   | LPAREN tuple_field_seq COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
   | LPAREN tuple_field_seq COMMA ELLIPSIS COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
