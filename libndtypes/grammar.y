%{
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017-2018, plures
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "grammar.h"


void
yyerror(YYLTYPE *loc, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx,
        const char *msg)
{
    (void)scanner;
    (void)ast;

    ndt_err_format(ctx, NDT_ParseError, "%d:%d: %s", loc->first_line,
                   loc->first_column, msg);
}

int
yylex(YYSTYPE *val, YYLTYPE *loc, yyscan_t scanner, ndt_context_t *ctx)
{
    return ndt_yylexfunc(val, loc, scanner, ctx);
}
%}

%code requires {
  #include "ndtypes.h"
  #include "seq.h"
  #include "attr.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;
}

%code provides {
  #define YY_DECL extern int ndt_yylexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int ndt_yylexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx, const char *msg);
}

%pure-parser
%define parse.error verbose

%locations
%initial-action {
   @$.first_line = 1;
   @$.first_column = 1;
   @$.last_line = 1;
   @$.last_column = 1;
}

%lex-param   {yyscan_t scanner} {ndt_context_t *ctx}
%parse-param {yyscan_t scanner} {const ndt_t **ast} {ndt_context_t *ctx}

%union {
    const ndt_t *ndt;
    enum ndt tag;
    enum ndt_alias alias;
    ndt_field_t *field;
    ndt_field_seq_t *field_seq;
    ndt_value_t *typed_value;
    ndt_value_seq_t *typed_value_seq;
    ndt_attr_t *attribute;
    ndt_attr_seq_t *attribute_seq;
    enum ndt_variadic variadic_flag;
    enum ndt_encoding encoding;
    uint32_t uint32;
    char *string;
    ndt_string_seq_t *string_seq;
    ndt_type_seq_t *type_seq;
}

%start input
%type <ndt> input
%type <ndt> datashape
%type <ndt> datashape_or_module
%type <ndt> datashape_with_ellipsis
%type <ndt> fixed_ellipsis
%type <ndt> dimensions
%type <ndt> dimensions_nooption
%type <ndt> dimensions_tail
%type <ndt> dtype
%type <ndt> scalar

%type <tag> signed
%type <tag> unsigned
%type <tag> ieee_float
%type <tag> ieee_complex
%type <alias> alias

%type <ndt> character
%type <ndt> string
%type <ndt> fixed_string
%type <ndt> bytes
%type <ndt> fixed_bytes
%type <ndt> array_dimensions
%type <ndt> array
%type <ndt> ref

%type <ndt> tuple_type
%type <field> tuple_field
%type <field_seq> tuple_field_seq

%type <ndt> record_type
%type <field> record_field
%type <field_seq> record_field_seq

%type <ndt> union_type
%type <field> union_member
%type <field_seq> union_member_seq

%type <string> field_name_or_tag

%type <ndt> categorical
%type <typed_value> typed_value
%type <typed_value_seq> typed_value_seq

%type <string> untyped_value
%type <string_seq> untyped_value_seq

%type <type_seq> type_seq
%type <type_seq> type_seq_or_void

%type <attribute> attribute
%type <attribute_seq> attribute_seq

%type <attribute_seq> arguments_opt

%type <ndt> function_type

%type <uint32> flags_opt
%type <uint32> option_opt
%type <uint32> endian_opt
%type <encoding> encoding
%type <variadic_flag> variadic_flag
%type <variadic_flag> comma_variadic_flag

%token
 ANY_KIND
 SCALAR_KIND
   VOID
   BOOL
   SIGNED_KIND INT8 INT16 INT32 INT64
   UNSIGNED_KIND UINT8 UINT16 UINT32 UINT64
   FLOAT_KIND BFLOAT16 FLOAT16 FLOAT32 FLOAT64
   COMPLEX_KIND BCOMPLEX32 COMPLEX32 COMPLEX64 COMPLEX128
   CATEGORICAL NA
   INTPTR UINTPTR SIZE
   CHAR
   STRING FIXED_STRING_KIND FIXED_STRING
   BYTES FIXED_BYTES_KIND FIXED_BYTES
   REF

FIXED VAR ARRAY OF

COMMA COLON LPAREN RPAREN LBRACE RBRACE LBRACK RBRACK STAR ELLIPSIS
RARROW EQUAL LESS GREATER QUESTIONMARK BANG AMPERSAND BAR
ERRTOKEN

%token <string>
  INTEGER FLOATNUMBER STRINGLIT
  NAME_LOWER NAME_UPPER NAME_OTHER

%token ENDMARKER 0 "end of file"

%destructor { ndt_decref($$); } <ndt>
%destructor { ndt_field_del($$); } <field>
%destructor { ndt_field_seq_del($$); } <field_seq>
%destructor { ndt_value_del($$); } <typed_value>
%destructor { ndt_value_seq_del($$); } <typed_value_seq>
%destructor { ndt_attr_del($$); } <attribute>
%destructor { ndt_attr_seq_del($$); } <attribute_seq>
%destructor { ndt_free($$); } <string>
%destructor { ndt_string_seq_del($$); } <string_seq>
%destructor { ndt_type_seq_del($$); } <type_seq>

%token BELOW_BAR
%precedence BELOW_BAR
%precedence BAR

%%

input:
  datashape_or_module ENDMARKER { $$ = $1;  *ast = $$; YYACCEPT; }

/* types (optionally with module qualifier) */
datashape_or_module:
  datashape_with_ellipsis                        { $$ = $1; }
| function_type                                  { $$ = $1; }
| NAME_UPPER COLON COLON datashape_with_ellipsis { $$ = mk_module($1, $4, ctx); if ($$ == NULL) YYABORT; }

/* types */
datashape_with_ellipsis:
  datashape                                { $$ = $1; }
| fixed_ellipsis                           { $$ = $1; }
| NAME_UPPER LBRACK fixed_ellipsis RBRACK  { $$ = mk_contig($1, (ndt_t *)$3, ctx); if ($$ == NULL) YYABORT; }
| VAR ELLIPSIS STAR dtype                  { $$ = mk_var_ellipsis($4, ctx); if ($$ == NULL) YYABORT; }
| array_dimensions                         { $$ = $1; }

fixed_ellipsis:
  ELLIPSIS STAR dimensions_tail            { $$ = mk_ellipsis_dim(NULL, $3, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER ELLIPSIS STAR dimensions_tail { $$ = mk_ellipsis_dim($1, $4, ctx); if ($$ == NULL) YYABORT; }

datashape:
  dimensions         { $$ = $1; }
| dtype              { $$ = $1; }

dimensions:
  dimensions_nooption                 { $$ = $1; }
| NAME_UPPER LBRACK dimensions RBRACK { $$ = mk_contig($1, (ndt_t *)$3, ctx); if ($$ == NULL) YYABORT; }
| BANG dimensions                     { $$ = mk_fortran($2, ctx); if ($$ == NULL) YYABORT; }

dimensions_nooption:
  INTEGER STAR dimensions_tail                           { $$ = mk_fixed_dim_from_shape($1, $3, ctx); if ($$ == NULL) YYABORT; }
| FIXED LPAREN attribute_seq RPAREN STAR dimensions_tail { $$ = mk_fixed_dim_from_attrs($3, $6, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER STAR dimensions_tail                        { $$ = mk_symbolic_dim($1, $3, ctx); if ($$ == NULL) YYABORT; }
| VAR arguments_opt STAR dimensions_tail                 { $$ = mk_var_dim($2, $4, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK VAR arguments_opt STAR dimensions_tail    { $$ = mk_var_dim($3, $5, true, ctx); if ($$ == NULL) YYABORT; }


dimensions_tail:
  dtype              { $$ = $1; }
| dimensions         { $$ = $1; }

dtype:
  option_opt ANY_KIND                             { $$ = ndt_any_kind($1, ctx); if ($$ == NULL) YYABORT; }
| option_opt SCALAR_KIND                          { $$ = ndt_scalar_kind($1, ctx); if ($$ == NULL) YYABORT; }
| scalar                                          { $$ = $1; }
| tuple_type                                      { $$ = $1; }
| record_type                                     { $$ = $1; }
| union_type                                      { $$ = $1; }
| NAME_LOWER                                      { $$ = ndt_nominal($1, NULL, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK NAME_LOWER                         { $$ = ndt_nominal($2, NULL, true, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER LPAREN datashape RPAREN              { $$ = mk_constr($1, $3, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK NAME_UPPER LPAREN datashape RPAREN { $$ = mk_constr($2, $4, true, ctx); if ($$ == NULL) YYABORT; }
| NAME_UPPER                                      { $$ = ndt_typevar($1, ctx); if ($$ == NULL) YYABORT; }

scalar:
  flags_opt BOOL               { $$ = ndt_primitive(Bool, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt SIGNED_KIND        { $$ = ndt_signed_kind($1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt signed             { $$ = ndt_primitive($2, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt UNSIGNED_KIND      { $$ = ndt_unsigned_kind($1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt unsigned           { $$ = ndt_primitive($2, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt FLOAT_KIND         { $$ = ndt_float_kind($1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt BFLOAT16           { $$ = ndt_primitive(BFloat16, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt ieee_float         { $$ = ndt_primitive($2, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt COMPLEX_KIND       { $$ = ndt_complex_kind($1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt BCOMPLEX32         { $$ = ndt_primitive(BComplex32, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt ieee_complex       { $$ = ndt_primitive($2, $1, ctx); if ($$ == NULL) YYABORT; }
| flags_opt alias              { $$ = ndt_from_alias($2, $1, ctx); if ($$ == NULL) YYABORT; }
| character                    { $$ = $1; }
| string                       { $$ = $1; }
| option_opt FIXED_STRING_KIND { $$ = ndt_fixed_string_kind($1, ctx); if ($$ == NULL) YYABORT; }
| fixed_string                 { $$ = $1; }
| bytes                        { $$ = $1; }
| option_opt FIXED_BYTES_KIND  { $$ = ndt_fixed_bytes_kind($1, ctx); if ($$ == NULL) YYABORT; }
| fixed_bytes                  { $$ = $1; }
| categorical                  { $$ = $1; }
| ref                          { $$ = $1; }

signed:
  INT8  { $$ = Int8; }
| INT16 { $$ = Int16; }
| INT32 { $$ = Int32; }
| INT64 { $$ = Int64; }

unsigned:
  UINT8  { $$ = Uint8; }
| UINT16 { $$ = Uint16; }
| UINT32 { $$ = Uint32; }
| UINT64 { $$ = Uint64; }

ieee_float:
  FLOAT16 { $$ = Float16; }
| FLOAT32 { $$ = Float32; }
| FLOAT64 { $$ = Float64; }

ieee_complex:
  COMPLEX32  { $$ = Complex32; }
| COMPLEX64  { $$ = Complex64; }
| COMPLEX128 { $$ = Complex128; }

alias:
  /* machine dependent */
  INTPTR  { $$ = Intptr; }
| UINTPTR { $$ = Uintptr; }
| SIZE    { $$ = Size; }

character:
  option_opt CHAR                        { $$ = ndt_char(Utf32, $1, ctx); if ($$ == NULL) YYABORT; }
| option_opt CHAR LPAREN encoding RPAREN { $$ = ndt_char($4, $1, ctx); if ($$ == NULL) YYABORT; }

string:
  option_opt STRING { $$ = ndt_string($1, ctx); if ($$ == NULL) YYABORT; }

fixed_string:
  option_opt FIXED_STRING LPAREN INTEGER RPAREN                { $$ = mk_fixed_string($4, Utf8, $1, ctx); if ($$ == NULL) YYABORT; }
| option_opt FIXED_STRING LPAREN INTEGER COMMA encoding RPAREN { $$ = mk_fixed_string($4, $6, $1, ctx); if ($$ == NULL) YYABORT; }

flags_opt:
  option_opt endian_opt { $$ = $1 | $2; }

option_opt:
  %empty       { $$ = 0; }
| QUESTIONMARK { $$ = NDT_OPTION; }

endian_opt:
  %empty  { $$ = 0; }
| EQUAL   { $$ = NDT_SYS_BIG_ENDIAN ? NDT_BIG_ENDIAN : NDT_LITTLE_ENDIAN; }
| LESS    { $$ = NDT_LITTLE_ENDIAN; }
| GREATER { $$ = NDT_BIG_ENDIAN; }
| BAR     { $$ = 0; }

encoding:
  STRINGLIT { $$ = encoding_from_string($1, ctx); if (ndt_err_occurred(ctx)) YYABORT; }

bytes:
  option_opt BYTES arguments_opt { $$ = mk_bytes($3, $1, ctx); if ($$ == NULL) YYABORT; }

array_dimensions:
  array                                  { $$ = $1; }
| option_opt ARRAY ELLIPSIS OF datashape { $$ = mk_array_ellipsis($5, ctx); if ($$ == NULL) YYABORT; }
| option_opt ARRAY ELLIPSIS OF array     { $$ = mk_array_ellipsis($5, ctx); if ($$ == NULL) YYABORT; }

array:
  option_opt ARRAY OF datashape { $$ = mk_array($4, $1, ctx); if ($$ == NULL) YYABORT; }
| option_opt ARRAY OF array     { $$ = mk_array($4, $1, ctx); if ($$ == NULL) YYABORT; }

fixed_bytes:
  option_opt FIXED_BYTES LPAREN attribute_seq RPAREN { $$ = mk_fixed_bytes($4, $1, ctx); if ($$ == NULL) YYABORT; }

ref:
  option_opt REF LPAREN datashape RPAREN { $$ = mk_ref($4, $1, ctx); if ($$ == NULL) YYABORT; }
| option_opt AMPERSAND datashape         { $$ = mk_ref($3, $1, ctx); if ($$ == NULL) YYABORT; }

categorical:
  option_opt CATEGORICAL LPAREN typed_value_seq RPAREN { $$ = mk_categorical($4, $1, ctx); if ($$ == NULL) YYABORT; }

typed_value_seq:
  typed_value                       { $$ = ndt_value_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| typed_value_seq COMMA typed_value { $$ = ndt_value_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

typed_value:
  INTEGER     { $$ = ndt_value_from_number(ValInt64, $1, ctx); if ($$ == NULL) YYABORT; }
| FLOATNUMBER { $$ = ndt_value_from_number(ValFloat64, $1, ctx); if ($$ == NULL) YYABORT; }
| STRINGLIT   { $$ = ndt_value_from_string($1, ctx); if ($$ == NULL) YYABORT; }
| NA          { $$ = ndt_value_na(ctx); if ($$ == NULL) YYABORT; }

variadic_flag:
  %empty   { $$ = Nonvariadic; }
| ELLIPSIS { $$ = Variadic; }

comma_variadic_flag:
  %empty         { $$ = Nonvariadic; }
| COMMA          { $$ = Nonvariadic; }
| COMMA ELLIPSIS { $$ = Variadic; }

tuple_type:
  LPAREN variadic_flag RPAREN                                    { $$ = mk_tuple($2, NULL, NULL, false, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq comma_variadic_flag RPAREN              { $$ = mk_tuple($3, $2, NULL, false, ctx); if ($$ == NULL) YYABORT; }
| LPAREN tuple_field_seq COMMA attribute_seq RPAREN              { $$ = mk_tuple(Nonvariadic, $2, $4, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LPAREN variadic_flag RPAREN                       { $$ = mk_tuple($3, NULL, NULL, true, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LPAREN tuple_field_seq comma_variadic_flag RPAREN { $$ = mk_tuple($4, $3, NULL, true, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LPAREN tuple_field_seq COMMA attribute_seq RPAREN { $$ = mk_tuple(Nonvariadic, $3, $5, true, ctx); if ($$ == NULL) YYABORT; }

tuple_field_seq:
  tuple_field                       { $$ = ndt_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| tuple_field_seq COMMA tuple_field { $$ = ndt_field_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

tuple_field:
  datashape                       { $$ = mk_field(NULL, $1, NULL, ctx); if ($$ == NULL) YYABORT; }
| datashape BAR attribute_seq BAR { $$ = mk_field(NULL, $1, $3, ctx); if ($$ == NULL) YYABORT; }

record_type:
  LBRACE variadic_flag RBRACE                                     { $$ = mk_record($2, NULL, NULL, false, ctx); if ($$ == NULL) YYABORT; }
| LBRACE record_field_seq comma_variadic_flag RBRACE              { $$ = mk_record($3, $2, NULL, false, ctx); if ($$ == NULL) YYABORT; }
| LBRACE record_field_seq COMMA attribute_seq RBRACE              { $$ = mk_record(Nonvariadic, $2, $4, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LBRACE variadic_flag RBRACE                        { $$ = mk_record($3, NULL, NULL, true, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LBRACE record_field_seq comma_variadic_flag RBRACE { $$ = mk_record($4, $3, NULL, true, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LBRACE record_field_seq COMMA attribute_seq RBRACE { $$ = mk_record(Nonvariadic, $3, $5, true, ctx); if ($$ == NULL) YYABORT; }

record_field_seq:
  record_field                         { $$ = ndt_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| record_field_seq COMMA record_field  { $$ = ndt_field_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

record_field:
  field_name_or_tag COLON datashape                       { $$ = mk_field($1, $3, NULL, ctx); if ($$ == NULL) YYABORT; }
| field_name_or_tag COLON datashape BAR attribute_seq BAR { $$ = mk_field($1, $3, $5, ctx); if ($$ == NULL) YYABORT; }

field_name_or_tag:
  NAME_LOWER { $$ = $1; if ($$ == NULL) YYABORT; }
| NAME_UPPER { $$ = $1; if ($$ == NULL) YYABORT; }
| NAME_OTHER { $$ = $1; if ($$ == NULL) YYABORT; }

union_type:
  union_member_seq %prec BELOW_BAR              { $$ = mk_union($1, false, ctx); if ($$ == NULL) YYABORT; }
| LBRACK union_member_seq RBRACK                { $$ = mk_union($2, false, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK union_member_seq %prec BELOW_BAR { $$ = mk_union($2, true, ctx); if ($$ == NULL) YYABORT; }
| QUESTIONMARK LBRACK union_member_seq RBRACK   { $$ = mk_union($3, true, ctx); if ($$ == NULL) YYABORT; }

union_member_seq:
  union_member                      { $$ = ndt_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| union_member_seq BAR union_member { $$ = ndt_field_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

union_member:
  field_name_or_tag OF datashape    { $$ = mk_field($1, $3, NULL, ctx); if ($$ == NULL) YYABORT; }

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
  INTEGER     { $$ = $1; if ($$ == NULL) YYABORT; }
| FLOATNUMBER { $$ = $1; if ($$ == NULL) YYABORT; }
| STRINGLIT   { $$ = $1; if ($$ == NULL) YYABORT; }

function_type:
  type_seq_or_void RARROW type_seq_or_void { $$ = mk_function($1, $3, ctx); if ($$ == NULL) YYABORT; }

type_seq_or_void:
  type_seq { $$ = $1; }
| VOID     { $$ = ndt_type_seq_empty(ctx); if ($$ == NULL) YYABORT; }

type_seq:
  datashape_with_ellipsis                { $$ = ndt_type_seq_new((ndt_t *)$1, ctx); if ($$ == NULL) YYABORT; }
| type_seq COMMA datashape_with_ellipsis { $$ = ndt_type_seq_append($1, (ndt_t *)$3, ctx); if ($$ == NULL) YYABORT; }
