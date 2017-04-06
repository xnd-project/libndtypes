%{
/*
 * Copyright (c) 2016, Continuum Analytics, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * 
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "grammar.h"
#include "lexer.h"

#undef yyerror
#undef yylex

void
yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx, const char *msg)
{
    (void)scanner;
    (void)ast;

    ndt_err_format(ctx, NDT_ParseError, "%d:%d: %s\n", loc->first_line,
                   loc->first_column, msg);
}

int
yylex(YYSTYPE *val, YYLTYPE *loc, yyscan_t scanner, ndt_context_t *ctx)
{
    return lexfunc(val, loc, scanner, ctx);
}
%}

%code requires {
  #include "ndtypes.h"
  #include "seq.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;
}

%code provides {
  #define YY_DECL extern int lexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int lexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx, const char *msg);
}

%pure-parser
%error-verbose

%locations
%initial-action {
   @$.first_line = 1;
   @$.first_column = 1;
   @$.last_line = 1;
   @$.last_column = 1;
}

%lex-param   {yyscan_t scanner} {ndt_context_t *ctx}
%parse-param {yyscan_t scanner} {ndt_t **ast} {ndt_context_t *ctx}

%union {
    ndt_t *ndt;
    ndt_tuple_field_t *tuple_field;
    ndt_tuple_field_seq_t *tuple_field_seq;
    ndt_record_field_t *record_field;
    ndt_record_field_seq_t *record_field_seq;
    ndt_memory_t *typed_value;
    ndt_memory_seq_t *typed_value_seq;
    ndt_attr_t *attribute;
    ndt_attr_seq_t *attribute_seq;
    enum ndt_variadic_flag variadic_flag;
    enum ndt_encoding encoding;
    char *string;
    ndt_string_seq_t *string_seq;
}

%start input
%type <ndt> input
%type <ndt> datashape
%type <ndt> array
%type <ndt> array_nooption
%type <ndt> flexarray
%type <ndt> flexarray_tail
%type <ndt> dtype
%type <ndt> dtype_nooption
%type <ndt> scalar
%type <ndt> signed
%type <ndt> unsigned
%type <ndt> ieee_float
%type <ndt> ieee_complex
%type <ndt> alias
%type <ndt> character
%type <ndt> string
%type <ndt> fixed_string
%type <ndt> bytes
%type <ndt> fixed_bytes
%type <ndt> pointer

%type <ndt> tuple_type
%type <tuple_field> tuple_field
%type <tuple_field_seq> tuple_field_seq

%type <ndt> record_type
%type <record_field> record_field
%type <record_field_seq> record_field_seq
%type <string> record_field_name

%type <ndt> categorical
%type <typed_value> typed_value
%type <typed_value_seq> typed_value_seq

%type <string> untyped_value
%type <string_seq> untyped_value_seq

%type <attribute> attribute
%type <attribute_seq> attribute_seq

%type <attribute_seq> arguments_opt

%type <ndt> function_type

%type <encoding> encoding
%type <variadic_flag> variadic_flag
%type <variadic_flag> comma_variadic_flag

%token
 ANY_KIND
 OPTION
 SCALAR_KIND
   VOID
   BOOL
   SIGNED_KIND INT8 INT16 INT32 INT64
   UNSIGNED_KIND UINT8 UINT16 UINT32 UINT64
   REAL_KIND FLOAT16 FLOAT32 FLOAT64
   COMPLEX_KIND COMPLEX64 COMPLEX128
   CATEGORICAL
   REAL COMPLEX INT
   INTPTR UINTPTR SIZE
   CHAR
   STRING FIXED_STRING_KIND FIXED_STRING
   BYTES FIXED_BYTES_KIND FIXED_BYTES
   POINTER

FIXED_DIM_KIND VAR

COMMA COLON LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK STAR ELLIPSIS
RARROW EQUAL QUESTIONMARK BAR
ERRTOKEN

%token <string>
  INTEGER FLOATNUMBER STRINGLIT
  NAME_LOWER NAME_UPPER NAME_OTHER

%token ENDMARKER 0 "end of file"

%destructor { ndt_del($$); } <ndt>
%destructor { ndt_tuple_field_del($$); } <tuple_field>
%destructor { ndt_tuple_field_seq_del($$); } <tuple_field_seq>
%destructor { ndt_record_field_del($$); } <record_field>
%destructor { ndt_record_field_seq_del($$); } <record_field_seq>
%destructor { ndt_memory_del($$); } <typed_value>
%destructor { ndt_memory_seq_del($$); } <typed_value_seq>
%destructor { ndt_attr_del($$); } <attribute>
%destructor { ndt_attr_seq_del($$); } <attribute_seq>
%destructor { ndt_free($$); } <string>
%destructor { ndt_string_seq_del($$); } <string_seq>

%%

input:
  datashape ENDMARKER { $$ = $1;  *ast = $$; YYACCEPT; }

/* types */
datashape:
  array { $$ = $1; }
| dtype { $$ = $1; }

array:
  array_nooption                      { $$ = $1; }
| QUESTIONMARK array_nooption         { $$ = ndt_option($2, ctx); if ($$ == NULL) YYABORT; }
| OPTION LPAREN array_nooption RPAREN { $$ = ndt_option($3, ctx); if ($$ == NULL) YYABORT; }

array_nooption:
  flexarray                                   { $$ = $1; }
| LBRACK flexarray COMMA attribute_seq RBRACK { $$ = mk_array($2, $4, ctx); if ($$ == NULL) YYABORT; }

flexarray:
  FIXED_DIM_KIND STAR flexarray_tail { $$ = ndt_fixed_dim_kind($3, ctx); if ($$ == NULL) YYABORT; }
| INTEGER STAR flexarray_tail        { $$ = mk_fixed_dim($1, $3, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER STAR flexarray_tail     { $$ = ndt_symbolic_dim($1, $3, ctx); if ($$ == NULL) YYABORT; }
| VAR STAR flexarray_tail            { $$ = ndt_var_dim($3, ctx); if ($$ == NULL) YYABORT; }
| ELLIPSIS STAR flexarray_tail       { $$ = ndt_ellipsis_dim($3, ctx); if ($$ == NULL) YYABORT; }

flexarray_tail:
  dtype     { $$ = $1; }
| flexarray { $$ = $1; }

dtype:
  dtype_nooption                      { $$ = $1; }
| QUESTIONMARK dtype_nooption         { $$ = ndt_option($2, ctx); if ($$ == NULL) YYABORT; }
| OPTION LPAREN dtype_nooption RPAREN { $$ = ndt_option($3, ctx); if ($$ == NULL) YYABORT; }

dtype_nooption:
  ANY_KIND                               { $$ = ndt_any_kind(ctx); if ($$ == NULL) YYABORT; }
| SCALAR_KIND                            { $$ = ndt_scalar_kind(ctx); if ($$ == NULL) YYABORT; }
| scalar                                 { $$ = $1; }
| tuple_type                             { $$ = $1; }
| record_type                            { $$ = $1; }
| function_type                          { $$ = $1; }
| NAME_LOWER                             { $$ = ndt_nominal($1, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER LPAREN dtype RPAREN         { $$ = ndt_constr($1, $3, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER LPAREN attribute_seq RPAREN { (void)$1; (void)$3; ndt_free($1); ndt_attr_seq_del($3); $$ = NULL;
                                            ndt_err_format(ctx, NDT_NotImplementedError, "general attributes are not implemented");
                                            YYABORT; }
| NAME_UPPER                             { $$ = ndt_typevar($1, ctx); if ($$ == NULL) YYABORT; }

scalar:
  VOID arguments_opt { $$ = mk_primitive(Void, $2, ctx); if ($$ == NULL) YYABORT; }
| BOOL arguments_opt { $$ = mk_primitive(Bool, $2, ctx); if ($$ == NULL) YYABORT; }
| SIGNED_KIND        { $$ = ndt_signed_kind(ctx); if ($$ == NULL) YYABORT; }
| signed             { $$ = $1; }
| UNSIGNED_KIND      { $$ = ndt_unsigned_kind(ctx); if ($$ == NULL) YYABORT; }
| unsigned           { $$ = $1; }
| REAL_KIND          { $$ = ndt_real_kind(ctx); if ($$ == NULL) YYABORT; }
| ieee_float         { $$ = $1; }
| COMPLEX_KIND       { $$ = ndt_complex_kind(ctx); if ($$ == NULL) YYABORT; }
| ieee_complex       { $$ = $1; }
| alias              { $$ = $1; }
| character          { $$ = $1; }
| string             { $$ = $1; }
| FIXED_STRING_KIND  { $$ = ndt_fixed_string_kind(ctx); if ($$ == NULL) YYABORT; }
| fixed_string       { $$ = $1; }
| bytes              { $$ = $1; }
| FIXED_BYTES_KIND   { $$ = ndt_fixed_bytes_kind(ctx); if ($$ == NULL) YYABORT; }
| fixed_bytes        { $$ = $1; }
| categorical        { $$ = $1; }
| pointer            { $$ = $1; }

signed:
  INT8 arguments_opt  { $$ = mk_primitive(Int8, $2, ctx); if ($$ == NULL) YYABORT; }
| INT16 arguments_opt { $$ = mk_primitive(Int16, $2, ctx); if ($$ == NULL) YYABORT; }
| INT32 arguments_opt { $$ = mk_primitive(Int32, $2, ctx); if ($$ == NULL) YYABORT; }
| INT64 arguments_opt { $$ = mk_primitive(Int64, $2, ctx); if ($$ == NULL) YYABORT; }

unsigned:
  UINT8 arguments_opt  { $$ = mk_primitive(Uint8, $2, ctx); if ($$ == NULL) YYABORT; }
| UINT16 arguments_opt { $$ = mk_primitive(Uint16, $2, ctx); if ($$ == NULL) YYABORT; }
| UINT32 arguments_opt { $$ = mk_primitive(Uint32, $2, ctx); if ($$ == NULL) YYABORT; }
| UINT64 arguments_opt { $$ = mk_primitive(Uint64, $2, ctx); if ($$ == NULL) YYABORT; }

ieee_float:
  FLOAT16 arguments_opt { $$ = mk_primitive(Float16, $2, ctx); if ($$ == NULL) YYABORT; }
| FLOAT32 arguments_opt { $$ = mk_primitive(Float32, $2, ctx); if ($$ == NULL) YYABORT; }
| FLOAT64 arguments_opt { $$ = mk_primitive(Float64, $2, ctx); if ($$ == NULL) YYABORT; }

ieee_complex:
  COMPLEX64 arguments_opt  { $$ = mk_primitive(Complex64, $2, ctx); if ($$ == NULL) YYABORT; }
| COMPLEX128 arguments_opt { $$ = mk_primitive(Complex128, $2, ctx); if ($$ == NULL) YYABORT; }

alias:
  /* machine independent */
  INT arguments_opt     { $$ = mk_primitive(Int32, $2, ctx); if ($$ == NULL) YYABORT; }
| REAL arguments_opt    { $$ = mk_primitive(Float64, $2, ctx); if ($$ == NULL) YYABORT; }
| COMPLEX arguments_opt { $$ = mk_primitive(Complex128, $2, ctx); if ($$ == NULL) YYABORT; }
  /* machine dependent */
| INTPTR arguments_opt  { $$ = mk_alias(Intptr, $2, ctx); if ($$ == NULL) YYABORT; }
| UINTPTR arguments_opt { $$ = mk_alias(Uintptr, $2, ctx); if ($$ == NULL) YYABORT; }
| SIZE arguments_opt    { $$ = mk_alias(Size, $2, ctx); if ($$ == NULL) YYABORT; }

character:
  CHAR                        { $$ = ndt_char(Utf32, ctx); if ($$ == NULL) YYABORT; }
| CHAR LPAREN encoding RPAREN { $$ = ndt_char($3, ctx); if ($$ == NULL) YYABORT; }

string:
  STRING { $$ = ndt_string(ctx); if ($$ == NULL) YYABORT; }

fixed_string:
  FIXED_STRING LPAREN INTEGER RPAREN                { $$ = mk_fixed_string($3, Utf8, ctx); if ($$ == NULL) YYABORT; }
| FIXED_STRING LPAREN INTEGER COMMA encoding RPAREN { $$ = mk_fixed_string($3, $5, ctx); if ($$ == NULL) YYABORT; }

encoding:
  STRINGLIT { $$ = ndt_encoding_from_string($1, ctx); if ($$ == ErrorEncoding) YYABORT; }

bytes:
  BYTES arguments_opt { $$ = mk_bytes($2, ctx); if ($$ == NULL) YYABORT; }

fixed_bytes:
  FIXED_BYTES LPAREN attribute_seq RPAREN { $$ = mk_fixed_bytes($3, ctx); if ($$ == NULL) YYABORT; }

pointer:
  POINTER LPAREN datashape RPAREN { $$ = ndt_pointer($3, ctx); if ($$ == NULL) YYABORT; }

categorical:
  CATEGORICAL LPAREN typed_value_seq RPAREN { $$ = mk_categorical($3, ctx); if ($$ == NULL) YYABORT; }

typed_value_seq:
  typed_value                       { $$ = ndt_memory_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| typed_value_seq COMMA typed_value { $$ = ndt_memory_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

typed_value:
  INTEGER COLON datashape     { $$ = ndt_memory_from_number($1, $3, ctx); if ($$ == NULL) YYABORT; }
| FLOATNUMBER COLON datashape { $$ = ndt_memory_from_number($1, $3, ctx); if ($$ == NULL) YYABORT; }
| STRINGLIT COLON datashape   { $$ = ndt_memory_from_string($1, $3, ctx); if ($$ == NULL) YYABORT; }

variadic_flag:
  %empty   { $$ = Nonvariadic; }
| ELLIPSIS { $$ = Variadic; }

comma_variadic_flag:
  %empty         { $$ = Nonvariadic; }
| COMMA          { $$ = Nonvariadic; }
| COMMA ELLIPSIS { $$ = Variadic; }

tuple_type:
  LPAREN variadic_flag RPAREN                       { $$ = mk_tuple($2, NULL, NULL, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq comma_variadic_flag RPAREN { $$ = mk_tuple($3, $2, NULL, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq COMMA attribute_seq RPAREN { $$ = mk_tuple(Nonvariadic, $2, $4, ctx); if ($$ == NULL) YYABORT; }

tuple_field_seq:
  tuple_field                       { $$ = ndt_tuple_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| tuple_field_seq COMMA tuple_field { $$ = ndt_tuple_field_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

tuple_field:
  datashape                       { $$ = mk_tuple_field($1, NULL, ctx); if ($$ == NULL) YYABORT; }
| datashape BAR attribute_seq BAR { $$ = mk_tuple_field($1, $3, ctx); if ($$ == NULL) YYABORT; }

record_type:
  LBRACE variadic_flag RBRACE                        { $$ = mk_record($2, NULL, NULL, ctx); if ($$ == NULL) YYABORT; }
| LBRACE record_field_seq comma_variadic_flag RBRACE { $$ = mk_record($3, $2, NULL, ctx); if ($$ == NULL) YYABORT; }
| LBRACE record_field_seq COMMA attribute_seq RBRACE { $$ = mk_record(Nonvariadic, $2, $4, ctx); if ($$ == NULL) YYABORT; }

record_field_seq:
  record_field                         { $$ = ndt_record_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| record_field_seq COMMA record_field  { $$ = ndt_record_field_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

record_field:
  record_field_name COLON datashape                       { $$ = mk_record_field($1, $3, NULL, ctx); if ($$ == NULL) YYABORT; }
| record_field_name COLON datashape BAR attribute_seq BAR { $$ = mk_record_field($1, $3, $5, ctx); if ($$ == NULL) YYABORT; }

record_field_name:
  NAME_LOWER { $$ = $1; if ($$ == NULL) YYABORT; }
| NAME_UPPER { $$ = $1; if ($$ == NULL) YYABORT; }
| NAME_OTHER { $$ = $1; if ($$ == NULL) YYABORT; }

arguments_opt:
  %empty                      { $$ = NULL; }
| LPAREN attribute_seq RPAREN { $$ = $2; if ($$ == NULL) YYABORT; }

attribute_seq:
  attribute                     { $$ = ndt_attr_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| attribute_seq COMMA attribute { $$ = ndt_attr_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

attribute:
  NAME_LOWER EQUAL untyped_value                   { $$ = mk_attr($1, $3, ctx); if ($$ == NULL) YYABORT; }
| NAME_LOWER EQUAL LBRACK untyped_value_seq RBRACK { $$ = mk_attr_from_seq($1, $4, ctx); if ($$ == NULL) YYABORT; }

untyped_value_seq:
  untyped_value                         { $$ = ndt_string_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| untyped_value_seq COMMA untyped_value { $$ = ndt_string_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

untyped_value:
  NAME_LOWER  { $$ = $1; if ($$ == NULL) YYABORT; }
| INTEGER     { $$ = $1; if ($$ == NULL) YYABORT; }
| FLOATNUMBER { $$ = $1; if ($$ == NULL) YYABORT; }
| STRINGLIT   { $$ = $1; if ($$ == NULL) YYABORT; }

function_type:
  tuple_type RARROW datashape
    { $$ = mk_function_from_tuple($3, $1, ctx); if ($$ == NULL) YYABORT; }
| LPAREN record_field_seq comma_variadic_flag RPAREN RARROW datashape
    { $$ = mk_function($6, Nonvariadic, NULL, $3, $2, ctx); if ($$ == NULL) YYABORT; }
| LPAREN ELLIPSIS COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
    { $$ = mk_function($8, Variadic, NULL, $5, $4, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
    { $$ = mk_function($8, Nonvariadic, $2, $5, $4, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq COMMA ELLIPSIS COMMA record_field_seq comma_variadic_flag RPAREN RARROW datashape
    { $$ = mk_function($10, Variadic, $2, $7, $6, ctx); if ($$ == NULL) YYABORT; }
