/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_GRAMMAR_H_INCLUDED
# define YY_YY_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 53 "grammar.y" /* yacc.c:1909  */

  #include "ndtypes.h"
  #include "seq.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;

#line 52 "grammar.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ENDMARKER = 0,
    ANY_KIND = 258,
    OPTION = 259,
    SCALAR_KIND = 260,
    VOID = 261,
    BOOL = 262,
    SIGNED_KIND = 263,
    INT8 = 264,
    INT16 = 265,
    INT32 = 266,
    INT64 = 267,
    INT128 = 268,
    UNSIGNED_KIND = 269,
    UINT8 = 270,
    UINT16 = 271,
    UINT32 = 272,
    UINT64 = 273,
    UINT128 = 274,
    REAL_KIND = 275,
    FLOAT16 = 276,
    FLOAT32 = 277,
    FLOAT64 = 278,
    FLOAT128 = 279,
    COMPLEX_KIND = 280,
    COMPLEX64 = 281,
    COMPLEX128 = 282,
    CATEGORICAL = 283,
    REAL = 284,
    COMPLEX = 285,
    INT = 286,
    INTPTR = 287,
    UINTPTR = 288,
    SIZE = 289,
    CHAR = 290,
    STRING = 291,
    FIXED_STRING_KIND = 292,
    FIXED_STRING = 293,
    BYTES = 294,
    FIXED_BYTES_KIND = 295,
    FIXED_BYTES = 296,
    NOMINAL = 297,
    POINTER = 298,
    FIXED_DIM_KIND = 299,
    FIXED = 300,
    VAR = 301,
    COMMA = 302,
    COLON = 303,
    LPAREN = 304,
    RPAREN = 305,
    LBRACE = 306,
    RBRACE = 307,
    LBRACK = 308,
    RBRACK = 309,
    STAR = 310,
    DOUBLESTAR = 311,
    ELLIPSIS = 312,
    RARROW = 313,
    EQUAL = 314,
    QUESTIONMARK = 315,
    ALIGN = 316,
    ERRTOKEN = 317,
    INTEGER = 318,
    FLOATNUMBER = 319,
    STRINGLIT = 320,
    NAME_LOWER = 321,
    NAME_UPPER = 322,
    NAME_OTHER = 323
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 81 "grammar.y" /* yacc.c:1909  */

    ndt_t *ndt;
    ndt_dim_t *dim;
    ndt_dim_seq_t *dim_seq;
    ndt_tuple_field_t *tuple_field;
    ndt_tuple_field_seq_t *tuple_field_seq;
    ndt_record_field_t *record_field;
    ndt_record_field_seq_t *record_field_seq;
    ndt_memory_t *typed_value;
    ndt_memory_seq_t *typed_value_seq;
    enum ndt_variadic_flag variadic_flag;
    enum ndt_encoding encoding;
    char *string;

#line 149 "grammar.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx);
/* "%code provides" blocks.  */
#line 61 "grammar.y" /* yacc.c:1909  */

  #define YY_DECL extern int lexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int lexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx, const char *msg);

#line 179 "grammar.h" /* yacc.c:1909  */

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */
