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

#ifndef YY_BP_BPGRAMMAR_H_INCLUDED
# define YY_BP_BPGRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int bpdebug;
#endif
/* "%code requires" blocks.  */
#line 276 "bpgrammar.y" /* yacc.c:1909  */

  #include <ctype.h>
  #include <assert.h>
  #include "ndtypes.h"
  #include "seq.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;

#line 53 "bpgrammar.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ENDMARKER = 0,
    PAD = 258,
    BOOL = 259,
    CHAR = 260,
    SCHAR = 261,
    UCHAR = 262,
    SHORT = 263,
    INT = 264,
    LONG = 265,
    LONGLONG = 266,
    SSIZE = 267,
    USHORT = 268,
    UINT = 269,
    ULONG = 270,
    ULONGLONG = 271,
    SIZE = 272,
    FLOAT = 273,
    DOUBLE = 274,
    BYTES = 275,
    COMPLEX = 276,
    RECORD = 277,
    AT = 278,
    EQUAL = 279,
    LESS = 280,
    GREATER = 281,
    BANG = 282,
    COMMA = 283,
    COLON = 284,
    LPAREN = 285,
    RPAREN = 286,
    LBRACE = 287,
    RBRACE = 288,
    ERRTOKEN = 289,
    DTYPE = 290,
    INTEGER = 291,
    NAME = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 307 "bpgrammar.y" /* yacc.c:1909  */

    ndt_t *ndt;
    ndt_field_t *field;
    ndt_field_seq_t *field_seq;
    ndt_string_seq_t *string_seq;
    char *string;
    unsigned char uchar;
    uint16_t uint16;

#line 114 "bpgrammar.h" /* yacc.c:1909  */
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



int bpparse (yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx);
/* "%code provides" blocks.  */
#line 285 "bpgrammar.y" /* yacc.c:1909  */

  #define YY_DECL extern int bplexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int bplexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx, const char *msg);

#line 144 "bpgrammar.h" /* yacc.c:1909  */

#endif /* !YY_BP_BPGRAMMAR_H_INCLUDED  */
