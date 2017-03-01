/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "grammar.y" /* yacc.c:339  */

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

#line 118 "grammar.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "grammar.h".  */
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
#line 53 "grammar.y" /* yacc.c:355  */

  #include "ndtypes.h"
  #include "seq.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;

#line 156 "grammar.c" /* yacc.c:355  */

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
    UNSIGNED_KIND = 268,
    UINT8 = 269,
    UINT16 = 270,
    UINT32 = 271,
    UINT64 = 272,
    REAL_KIND = 273,
    FLOAT16 = 274,
    FLOAT32 = 275,
    FLOAT64 = 276,
    COMPLEX_KIND = 277,
    COMPLEX64 = 278,
    COMPLEX128 = 279,
    CATEGORICAL = 280,
    REAL = 281,
    COMPLEX = 282,
    INT = 283,
    INTPTR = 284,
    UINTPTR = 285,
    SIZE = 286,
    CHAR = 287,
    STRING = 288,
    FIXED_STRING_KIND = 289,
    FIXED_STRING = 290,
    BYTES = 291,
    FIXED_BYTES_KIND = 292,
    FIXED_BYTES = 293,
    POINTER = 294,
    FIXED_DIM_KIND = 295,
    FIXED = 296,
    VAR = 297,
    COMMA = 298,
    COLON = 299,
    LPAREN = 300,
    RPAREN = 301,
    LBRACE = 302,
    RBRACE = 303,
    LBRACK = 304,
    RBRACK = 305,
    STAR = 306,
    ELLIPSIS = 307,
    RARROW = 308,
    EQUAL = 309,
    QUESTIONMARK = 310,
    ALIGN = 311,
    ERRTOKEN = 312,
    INTEGER = 313,
    FLOATNUMBER = 314,
    STRINGLIT = 315,
    NAME_LOWER = 316,
    NAME_UPPER = 317,
    NAME_OTHER = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 81 "grammar.y" /* yacc.c:355  */

    ndt_t *ndt;
    ndt_dim_t *dim;
    ndt_dim_seq_t *dim_seq;
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

#line 250 "grammar.c" /* yacc.c:355  */
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
#line 61 "grammar.y" /* yacc.c:355  */

  #define YY_DECL extern int lexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int lexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx, const char *msg);

#line 280 "grammar.c" /* yacc.c:355  */

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 286 "grammar.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  105
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   697

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  117
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  217

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   185,   185,   189,   190,   193,   194,   195,   198,   199,
     202,   203,   206,   207,   208,   209,   210,   211,   214,   215,
     216,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     230,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   255,   256,   257,   258,   261,   262,   263,   264,   267,
     268,   269,   272,   273,   274,   275,   276,   280,   281,   282,
     284,   285,   286,   289,   290,   293,   296,   297,   300,   303,
     306,   309,   312,   315,   316,   319,   320,   321,   324,   325,
     328,   329,   330,   333,   334,   337,   338,   341,   344,   345,
     348,   349,   352,   355,   356,   357,   360,   361,   364,   365,
     368,   369,   370,   373,   375,   377,   379,   381
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "ANY_KIND", "OPTION",
  "SCALAR_KIND", "VOID", "BOOL", "SIGNED_KIND", "INT8", "INT16", "INT32",
  "INT64", "UNSIGNED_KIND", "UINT8", "UINT16", "UINT32", "UINT64",
  "REAL_KIND", "FLOAT16", "FLOAT32", "FLOAT64", "COMPLEX_KIND",
  "COMPLEX64", "COMPLEX128", "CATEGORICAL", "REAL", "COMPLEX", "INT",
  "INTPTR", "UINTPTR", "SIZE", "CHAR", "STRING", "FIXED_STRING_KIND",
  "FIXED_STRING", "BYTES", "FIXED_BYTES_KIND", "FIXED_BYTES", "POINTER",
  "FIXED_DIM_KIND", "FIXED", "VAR", "COMMA", "COLON", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACK", "RBRACK", "STAR", "ELLIPSIS", "RARROW",
  "EQUAL", "QUESTIONMARK", "ALIGN", "ERRTOKEN", "INTEGER", "FLOATNUMBER",
  "STRINGLIT", "NAME_LOWER", "NAME_UPPER", "NAME_OTHER", "$accept",
  "input", "datashape", "array", "array_nooption", "dimension_seq",
  "dimension", "dtype", "dtype_nooption", "scalar", "signed", "unsigned",
  "ieee_float", "ieee_complex", "alias", "character", "string",
  "fixed_string", "encoding", "bytes", "fixed_bytes", "pointer",
  "categorical", "typed_value_seq", "typed_value", "variadic_flag",
  "comma_variadic_flag", "tuple_type", "tuple_field_seq", "tuple_field",
  "record_type", "record_field_seq", "record_field", "record_field_name",
  "attribute_seq_opt", "attribute_seq", "attribute", "function_type", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318
};
# endif

#define YYPACT_NINF -102

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-102)))

#define YYTABLE_NINF -107

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     395,  -102,   -30,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,   -10,  -102,    -6,  -102,  -102,  -102,  -102,
      -5,  -102,  -102,    11,    25,  -102,    29,    30,  -102,    31,
      28,   213,   -34,  -102,   515,    28,  -102,   -20,    78,    80,
    -102,  -102,    32,  -102,  -102,  -102,  -102,  -102,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,   -42,
    -102,  -102,   515,     9,    33,    21,    26,    34,    35,   395,
      36,    37,  -102,    12,    42,   -28,  -102,    28,    41,    46,
    -102,    49,  -102,    51,  -102,  -102,  -102,    48,    49,  -102,
    -102,  -102,   575,  -102,    52,  -102,  -102,   455,   395,    53,
      54,    58,    60,    61,    14,  -102,    63,    64,    65,  -102,
      66,    18,    43,    70,    68,    69,    67,   -24,  -102,    10,
    -102,  -102,   274,    71,   -11,    72,   395,  -102,    59,    74,
     635,    67,    75,    76,    19,   455,    -8,  -102,  -102,  -102,
    -102,  -102,   395,   395,   395,     9,  -102,  -102,  -102,  -102,
    -102,    21,  -102,    73,    50,  -102,  -102,   335,    37,  -102,
      49,    20,  -102,    49,  -102,  -102,  -102,    77,    28,  -102,
     635,  -102,  -102,  -102,  -102,  -102,  -102,  -102,    79,    81,
      83,    -3,  -102,  -102,  -102,    82,    10,    86,   395,  -102,
    -102,  -102,    84,    85,    49,    90,  -102,    87,   395,    88,
     395,  -102,  -102,    91,  -102,   395,  -102
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    21,     0,    22,    31,    32,    33,    51,    52,    53,
      54,    35,    55,    56,    57,    58,    37,    59,    60,    61,
      39,    62,    63,     0,    68,    69,    67,    70,    71,    72,
      73,    75,    44,     0,     0,    47,     0,     0,    12,     0,
     106,    88,    88,    17,     0,   106,    27,    30,     0,     0,
       3,     5,     0,    10,     4,    18,    23,    34,    36,    38,
      40,    41,    42,    43,    45,    46,    48,    50,    49,    24,
      25,    26,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,    17,    27,    30,   105,   106,     0,    90,
      95,    90,   100,     0,    89,   103,   104,     0,    90,     6,
      19,    13,     0,    15,     0,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,     0,    78,
       0,     0,     0,     0,     0,     0,     0,     0,   108,     0,
      97,    93,    91,     0,    91,     0,     0,    98,     0,     0,
       0,    27,    30,     0,     0,     0,    30,    11,     9,   113,
       7,    20,     0,     0,     0,     0,    82,    64,    65,    66,
      74,     0,    76,     0,     0,    81,    14,     0,     0,   107,
      90,    17,    96,    90,    94,    92,   101,     0,   106,    99,
       0,    28,    29,     8,    85,    86,    87,    84,     0,     0,
       0,   110,   111,   112,   109,     0,     0,     0,     0,   102,
      77,    79,     0,     0,    90,     0,   114,     0,     0,     0,
       0,    80,   115,     0,   116,     0,   117
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -102,  -102,     0,  -102,   -36,   -40,  -101,   -98,   -39,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,  -102,  -102,   -37,  -102,
    -102,  -102,  -102,  -102,   -32,    93,   -88,  -102,  -102,     7,
    -102,   -41,    13,  -102,   -38,    24,   -23,  -102
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    48,    87,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,   120,    65,
      66,    67,    68,   114,   115,    88,   133,    69,    89,    90,
      70,    91,    92,    93,   101,   127,   128,    71
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      49,    98,    82,   135,   143,   100,   147,   104,    99,   148,
     138,   108,    38,    39,    40,    72,  -104,   102,    94,   168,
      38,    39,    40,   -15,    43,   102,   169,    95,    96,    86,
      45,   -15,    43,   110,   103,    73,   109,   102,    45,    74,
      75,   175,   103,   -15,   147,   104,    81,   183,  -106,   130,
      95,    96,    86,   116,   117,   129,    76,   155,   -89,   118,
     156,   161,   168,   196,   162,   182,   -92,   111,   112,   113,
      77,    95,    96,    86,    78,    79,    80,    81,   105,   124,
     106,   119,   195,   107,   121,   197,  -103,   131,   170,   132,
     122,   173,   134,   123,   125,   136,   137,   163,   126,   150,
     151,   100,   152,   145,   153,   154,   190,   179,   149,   157,
     158,   159,   160,   164,   165,   166,   209,   174,   177,   180,
     102,   167,   181,   187,   188,   200,   144,   201,   203,     0,
     198,   189,   205,   211,   213,    97,   178,   202,   208,   172,
     199,   110,   207,   210,   215,   194,     0,   176,     0,     0,
       0,     0,   184,   185,   186,   204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   193,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,     0,
     214,     0,     0,     0,     0,   216,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,     0,    41,     0,
      42,     0,     0,     0,     0,    83,     0,     0,    44,     0,
       0,    45,     0,     0,    84,    85,    86,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     0,     0,    41,
       0,    42,     0,     0,     0,     0,   171,     0,     0,    44,
       0,     0,    45,     0,     0,    84,    85,    86,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,     0,
      41,     0,    42,     0,     0,     0,     0,    43,     0,     0,
      44,     0,     0,   191,     0,   192,    46,    47,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,     0,
      41,     0,    42,     0,     0,     0,     0,    43,     0,     0,
      44,     0,     0,    45,     0,     0,    46,    47,     1,   139,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,     0,
      41,     0,    42,     0,     0,     0,     0,    43,     0,     0,
     140,     0,     0,    45,     0,     0,    46,   146,     1,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,     0,
      41,     0,    42,     0,     0,     0,     0,    43,     0,     0,
       0,     0,     0,    45,     0,     0,    46,    47,     1,   139,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,     0,     0,     0,     0,
      41,     0,    42,     0,     0,     0,     0,     0,     0,     0,
     140,     0,     0,     0,     0,     0,   141,   142,     1,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,     0,     0,     0,     0,     0,
      41,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    46,   142
};

static const yytype_int16 yycheck[] =
{
       0,    42,    40,    91,   102,    44,   107,    47,    44,   107,
      98,    53,    40,    41,    42,    45,    44,    45,    52,    43,
      40,    41,    42,    51,    52,    45,    50,    61,    62,    63,
      58,    51,    52,    72,    62,    45,    72,    45,    58,    45,
      45,    52,    62,    51,   145,    85,    49,   145,    51,    87,
      61,    62,    63,    20,    21,    43,    45,    43,    46,    26,
      46,    43,    43,    43,    46,    46,    46,    58,    59,    60,
      45,    61,    62,    63,    45,    45,    45,    49,     0,    79,
       0,    60,   170,    51,    58,   173,    44,    46,   129,    43,
      56,   132,    43,    58,    58,    44,    48,    54,    61,    46,
      46,   140,    44,    51,    44,    44,    56,    48,   108,    46,
      46,    46,    46,    43,    46,    46,   204,    46,    46,    45,
      45,    54,    46,   155,   161,    46,   102,    46,    46,    -1,
      53,    58,    46,    46,    46,    42,   136,    54,    53,   132,
     178,   180,    58,    53,    53,   168,    -1,   134,    -1,    -1,
      -1,    -1,   152,   153,   154,   196,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
     210,    -1,    -1,    -1,    -1,   215,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    -1,    -1,    45,    -1,
      47,    -1,    -1,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    -1,    -1,    61,    62,    63,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    45,
      -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    -1,    -1,    61,    62,    63,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    -1,    60,    61,    62,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    -1,    -1,    61,    62,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      55,    -1,    -1,    58,    -1,    -1,    61,    62,     3,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    61,    62,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    61,    62,     3,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    61,    62
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    45,    47,    52,    55,    58,    61,    62,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    83,    84,    85,    86,    91,
      94,   101,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    49,    98,    52,    61,    62,    63,    66,    89,    92,
      93,    95,    96,    97,    52,    61,    62,    89,    95,    68,
      72,    98,    45,    62,    69,     0,     0,    51,    53,    68,
      72,    58,    59,    60,    87,    88,    20,    21,    26,    60,
      82,    58,    56,    58,    66,    58,    61,    99,   100,    43,
      98,    46,    43,    90,    43,    90,    44,    48,    90,     4,
      55,    61,    62,    71,    99,    51,    62,    70,    71,    66,
      46,    46,    44,    44,    44,    43,    46,    46,    46,    46,
      46,    43,    46,    54,    43,    46,    46,    54,    43,    50,
      95,    52,    93,    95,    46,    52,    96,    46,    66,    48,
      45,    46,    46,    71,    66,    66,    66,    88,    82,    58,
      56,    58,    60,    66,   100,    90,    43,    90,    53,    98,
      46,    46,    54,    46,    95,    46,    66,    58,    53,    90,
      53,    46,    66,    46,    66,    53,    66
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    66,    66,    67,    67,    67,    68,    68,
      69,    69,    70,    70,    70,    70,    70,    70,    71,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    74,    74,    74,    75,    75,    75,    75,    76,
      76,    76,    77,    77,    77,    77,    77,    78,    78,    78,
      78,    78,    78,    79,    79,    80,    81,    81,    82,    83,
      84,    85,    86,    87,    87,    88,    88,    88,    89,    89,
      90,    90,    90,    91,    91,    92,    92,    93,    94,    94,
      95,    95,    96,    97,    97,    97,    98,    98,    99,    99,
     100,   100,   100,   101,   101,   101,   101,   101
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     2,     4,     4,     3,
       1,     3,     1,     2,     4,     1,     2,     1,     1,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     4,     6,     1,     6,
       8,     4,     4,     1,     3,     3,     3,     3,     0,     1,
       0,     1,     2,     3,     4,     1,     3,     2,     3,     4,
       1,     3,     4,     1,     1,     1,     0,     3,     1,     3,
       3,     3,     3,     3,     6,     8,     8,    10
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, scanner, ast, ctx, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner, ast, ctx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (ast);
  YYUSE (ctx);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, ast, ctx);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, ast, ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, ast, ctx); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (ast);
  YYUSE (ctx);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 58: /* INTEGER  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1444 "grammar.c" /* yacc.c:1257  */
        break;

    case 59: /* FLOATNUMBER  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1450 "grammar.c" /* yacc.c:1257  */
        break;

    case 60: /* STRINGLIT  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1456 "grammar.c" /* yacc.c:1257  */
        break;

    case 61: /* NAME_LOWER  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1462 "grammar.c" /* yacc.c:1257  */
        break;

    case 62: /* NAME_UPPER  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1468 "grammar.c" /* yacc.c:1257  */
        break;

    case 63: /* NAME_OTHER  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1474 "grammar.c" /* yacc.c:1257  */
        break;

    case 65: /* input  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1480 "grammar.c" /* yacc.c:1257  */
        break;

    case 66: /* datashape  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1486 "grammar.c" /* yacc.c:1257  */
        break;

    case 67: /* array  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1492 "grammar.c" /* yacc.c:1257  */
        break;

    case 68: /* array_nooption  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1498 "grammar.c" /* yacc.c:1257  */
        break;

    case 69: /* dimension_seq  */
#line 174 "grammar.y" /* yacc.c:1257  */
      { ndt_dim_seq_del(((*yyvaluep).dim_seq)); }
#line 1504 "grammar.c" /* yacc.c:1257  */
        break;

    case 71: /* dtype  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1510 "grammar.c" /* yacc.c:1257  */
        break;

    case 72: /* dtype_nooption  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1516 "grammar.c" /* yacc.c:1257  */
        break;

    case 73: /* scalar  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1522 "grammar.c" /* yacc.c:1257  */
        break;

    case 74: /* signed  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1528 "grammar.c" /* yacc.c:1257  */
        break;

    case 75: /* unsigned  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1534 "grammar.c" /* yacc.c:1257  */
        break;

    case 76: /* ieee_float  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1540 "grammar.c" /* yacc.c:1257  */
        break;

    case 77: /* ieee_complex  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1546 "grammar.c" /* yacc.c:1257  */
        break;

    case 78: /* alias  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1552 "grammar.c" /* yacc.c:1257  */
        break;

    case 79: /* character  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1558 "grammar.c" /* yacc.c:1257  */
        break;

    case 80: /* string  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1564 "grammar.c" /* yacc.c:1257  */
        break;

    case 81: /* fixed_string  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1570 "grammar.c" /* yacc.c:1257  */
        break;

    case 83: /* bytes  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1576 "grammar.c" /* yacc.c:1257  */
        break;

    case 84: /* fixed_bytes  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1582 "grammar.c" /* yacc.c:1257  */
        break;

    case 85: /* pointer  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1588 "grammar.c" /* yacc.c:1257  */
        break;

    case 86: /* categorical  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1594 "grammar.c" /* yacc.c:1257  */
        break;

    case 87: /* typed_value_seq  */
#line 178 "grammar.y" /* yacc.c:1257  */
      { ndt_memory_seq_del(((*yyvaluep).typed_value_seq)); }
#line 1600 "grammar.c" /* yacc.c:1257  */
        break;

    case 88: /* typed_value  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_memory_del(((*yyvaluep).typed_value)); }
#line 1606 "grammar.c" /* yacc.c:1257  */
        break;

    case 91: /* tuple_type  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1612 "grammar.c" /* yacc.c:1257  */
        break;

    case 92: /* tuple_field_seq  */
#line 175 "grammar.y" /* yacc.c:1257  */
      { ndt_tuple_field_seq_del(((*yyvaluep).tuple_field_seq)); }
#line 1618 "grammar.c" /* yacc.c:1257  */
        break;

    case 94: /* record_type  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1624 "grammar.c" /* yacc.c:1257  */
        break;

    case 95: /* record_field_seq  */
#line 176 "grammar.y" /* yacc.c:1257  */
      { ndt_record_field_seq_del(((*yyvaluep).record_field_seq)); }
#line 1630 "grammar.c" /* yacc.c:1257  */
        break;

    case 96: /* record_field  */
#line 177 "grammar.y" /* yacc.c:1257  */
      { ndt_record_field_del(((*yyvaluep).record_field)); }
#line 1636 "grammar.c" /* yacc.c:1257  */
        break;

    case 97: /* record_field_name  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1642 "grammar.c" /* yacc.c:1257  */
        break;

    case 101: /* function_type  */
#line 173 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1648 "grammar.c" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 71 "grammar.y" /* yacc.c:1429  */
{
   yylloc.first_line = 1;
   yylloc.first_column = 1;
   yylloc.last_line = 1;
   yylloc.last_column = 1;
}

#line 1764 "grammar.c" /* yacc.c:1429  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner, ctx);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 185 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[-1].ndt);  *ast = (yyval.ndt); YYACCEPT; }
#line 1953 "grammar.c" /* yacc.c:1646  */
    break;

  case 3:
#line 189 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1959 "grammar.c" /* yacc.c:1646  */
    break;

  case 4:
#line 190 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1965 "grammar.c" /* yacc.c:1646  */
    break;

  case 5:
#line 193 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1971 "grammar.c" /* yacc.c:1646  */
    break;

  case 6:
#line 194 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_option((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 1977 "grammar.c" /* yacc.c:1646  */
    break;

  case 7:
#line 195 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_option((yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 1983 "grammar.c" /* yacc.c:1646  */
    break;

  case 8:
#line 198 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_array((yyvsp[-3].string), (yyvsp[-2].dim_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 1989 "grammar.c" /* yacc.c:1646  */
    break;

  case 9:
#line 199 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_array(NULL, (yyvsp[-2].dim_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 1995 "grammar.c" /* yacc.c:1646  */
    break;

  case 10:
#line 202 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim_seq) = ndt_dim_seq_new((yyvsp[0].dim), ctx); if ((yyval.dim_seq) == NULL) YYABORT; }
#line 2001 "grammar.c" /* yacc.c:1646  */
    break;

  case 11:
#line 203 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim_seq) = ndt_dim_seq_append((yyvsp[-2].dim_seq), (yyvsp[0].dim), ctx); if ((yyval.dim_seq) == NULL) YYABORT; }
#line 2007 "grammar.c" /* yacc.c:1646  */
    break;

  case 12:
#line 206 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = ndt_fixed_dim_kind(ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2013 "grammar.c" /* yacc.c:1646  */
    break;

  case 13:
#line 207 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = mk_fixed_dim((yyvsp[-1].string), (yyvsp[0].attribute_seq), ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2019 "grammar.c" /* yacc.c:1646  */
    break;

  case 14:
#line 208 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = mk_fixed_dim((yyvsp[-1].string), NULL, ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2025 "grammar.c" /* yacc.c:1646  */
    break;

  case 15:
#line 209 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = ndt_symbolic_dim((yyvsp[0].string), ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2031 "grammar.c" /* yacc.c:1646  */
    break;

  case 16:
#line 210 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = mk_var_dim((yyvsp[0].attribute_seq), ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2037 "grammar.c" /* yacc.c:1646  */
    break;

  case 17:
#line 211 "grammar.y" /* yacc.c:1646  */
    { (yyval.dim) = ndt_ellipsis_dim(ctx); if ((yyval.dim) == NULL) YYABORT; }
#line 2043 "grammar.c" /* yacc.c:1646  */
    break;

  case 18:
#line 214 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2049 "grammar.c" /* yacc.c:1646  */
    break;

  case 19:
#line 215 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_option((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2055 "grammar.c" /* yacc.c:1646  */
    break;

  case 20:
#line 216 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_option((yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2061 "grammar.c" /* yacc.c:1646  */
    break;

  case 21:
#line 219 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_any_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2067 "grammar.c" /* yacc.c:1646  */
    break;

  case 22:
#line 220 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_scalar_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2073 "grammar.c" /* yacc.c:1646  */
    break;

  case 23:
#line 221 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2079 "grammar.c" /* yacc.c:1646  */
    break;

  case 24:
#line 222 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2085 "grammar.c" /* yacc.c:1646  */
    break;

  case 25:
#line 223 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2091 "grammar.c" /* yacc.c:1646  */
    break;

  case 26:
#line 224 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2097 "grammar.c" /* yacc.c:1646  */
    break;

  case 27:
#line 225 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2103 "grammar.c" /* yacc.c:1646  */
    break;

  case 28:
#line 226 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_constr((yyvsp[-3].string), (yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2109 "grammar.c" /* yacc.c:1646  */
    break;

  case 29:
#line 227 "grammar.y" /* yacc.c:1646  */
    { (void)(yyvsp[-3].string); (void)(yyvsp[-1].attribute_seq); (yyval.ndt) = NULL;
                                            ndt_err_format(ctx, NDT_NotImplementedError, "general attributes are not implemented");
                                            YYABORT; }
#line 2117 "grammar.c" /* yacc.c:1646  */
    break;

  case 30:
#line 230 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_typevar((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2123 "grammar.c" /* yacc.c:1646  */
    break;

  case 31:
#line 233 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Void, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2129 "grammar.c" /* yacc.c:1646  */
    break;

  case 32:
#line 234 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Bool, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2135 "grammar.c" /* yacc.c:1646  */
    break;

  case 33:
#line 235 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_signed_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2141 "grammar.c" /* yacc.c:1646  */
    break;

  case 34:
#line 236 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2147 "grammar.c" /* yacc.c:1646  */
    break;

  case 35:
#line 237 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_unsigned_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2153 "grammar.c" /* yacc.c:1646  */
    break;

  case 36:
#line 238 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2159 "grammar.c" /* yacc.c:1646  */
    break;

  case 37:
#line 239 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_real_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2165 "grammar.c" /* yacc.c:1646  */
    break;

  case 38:
#line 240 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2171 "grammar.c" /* yacc.c:1646  */
    break;

  case 39:
#line 241 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_complex_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2177 "grammar.c" /* yacc.c:1646  */
    break;

  case 40:
#line 242 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2183 "grammar.c" /* yacc.c:1646  */
    break;

  case 41:
#line 243 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2189 "grammar.c" /* yacc.c:1646  */
    break;

  case 42:
#line 244 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2195 "grammar.c" /* yacc.c:1646  */
    break;

  case 43:
#line 245 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2201 "grammar.c" /* yacc.c:1646  */
    break;

  case 44:
#line 246 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_fixed_string_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2207 "grammar.c" /* yacc.c:1646  */
    break;

  case 45:
#line 247 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2213 "grammar.c" /* yacc.c:1646  */
    break;

  case 46:
#line 248 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2219 "grammar.c" /* yacc.c:1646  */
    break;

  case 47:
#line 249 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_fixed_bytes_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2225 "grammar.c" /* yacc.c:1646  */
    break;

  case 48:
#line 250 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2231 "grammar.c" /* yacc.c:1646  */
    break;

  case 49:
#line 251 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2237 "grammar.c" /* yacc.c:1646  */
    break;

  case 50:
#line 252 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2243 "grammar.c" /* yacc.c:1646  */
    break;

  case 51:
#line 255 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Int8, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2249 "grammar.c" /* yacc.c:1646  */
    break;

  case 52:
#line 256 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Int16, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2255 "grammar.c" /* yacc.c:1646  */
    break;

  case 53:
#line 257 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Int32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2261 "grammar.c" /* yacc.c:1646  */
    break;

  case 54:
#line 258 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Int64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2267 "grammar.c" /* yacc.c:1646  */
    break;

  case 55:
#line 261 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Uint8, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2273 "grammar.c" /* yacc.c:1646  */
    break;

  case 56:
#line 262 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Uint16, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2279 "grammar.c" /* yacc.c:1646  */
    break;

  case 57:
#line 263 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Uint32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2285 "grammar.c" /* yacc.c:1646  */
    break;

  case 58:
#line 264 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Uint64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2291 "grammar.c" /* yacc.c:1646  */
    break;

  case 59:
#line 267 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Float16, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2297 "grammar.c" /* yacc.c:1646  */
    break;

  case 60:
#line 268 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Float32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2303 "grammar.c" /* yacc.c:1646  */
    break;

  case 61:
#line 269 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Float64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2309 "grammar.c" /* yacc.c:1646  */
    break;

  case 62:
#line 272 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2315 "grammar.c" /* yacc.c:1646  */
    break;

  case 63:
#line 273 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex128, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2321 "grammar.c" /* yacc.c:1646  */
    break;

  case 64:
#line 274 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2327 "grammar.c" /* yacc.c:1646  */
    break;

  case 65:
#line 275 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex128, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2333 "grammar.c" /* yacc.c:1646  */
    break;

  case 66:
#line 276 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex128, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2339 "grammar.c" /* yacc.c:1646  */
    break;

  case 67:
#line 280 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Int32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2345 "grammar.c" /* yacc.c:1646  */
    break;

  case 68:
#line 281 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Float64, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2351 "grammar.c" /* yacc.c:1646  */
    break;

  case 69:
#line 282 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_primitive(Complex128, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2357 "grammar.c" /* yacc.c:1646  */
    break;

  case 70:
#line 284 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_from_alias(Intptr, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2363 "grammar.c" /* yacc.c:1646  */
    break;

  case 71:
#line 285 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_from_alias(Uintptr, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2369 "grammar.c" /* yacc.c:1646  */
    break;

  case 72:
#line 286 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_from_alias(Size, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2375 "grammar.c" /* yacc.c:1646  */
    break;

  case 73:
#line 289 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_char(Utf32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2381 "grammar.c" /* yacc.c:1646  */
    break;

  case 74:
#line 290 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_char((yyvsp[-1].encoding), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2387 "grammar.c" /* yacc.c:1646  */
    break;

  case 75:
#line 293 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_string(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2393 "grammar.c" /* yacc.c:1646  */
    break;

  case 76:
#line 296 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-1].string), Utf8, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2399 "grammar.c" /* yacc.c:1646  */
    break;

  case 77:
#line 297 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-3].string), (yyvsp[-1].encoding), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2405 "grammar.c" /* yacc.c:1646  */
    break;

  case 78:
#line 300 "grammar.y" /* yacc.c:1646  */
    { (yyval.encoding) = ndt_encoding_from_string((yyvsp[0].string), ctx); if ((yyval.encoding) == ErrorEncoding) YYABORT; }
#line 2411 "grammar.c" /* yacc.c:1646  */
    break;

  case 79:
#line 303 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_bytes((yyvsp[-1].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2417 "grammar.c" /* yacc.c:1646  */
    break;

  case 80:
#line 306 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_bytes((yyvsp[-5].string), (yyvsp[-1].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2423 "grammar.c" /* yacc.c:1646  */
    break;

  case 81:
#line 309 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_pointer((yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2429 "grammar.c" /* yacc.c:1646  */
    break;

  case 82:
#line 312 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_categorical((yyvsp[-1].typed_value_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2435 "grammar.c" /* yacc.c:1646  */
    break;

  case 83:
#line 315 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value_seq) = ndt_memory_seq_new((yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2441 "grammar.c" /* yacc.c:1646  */
    break;

  case 84:
#line 316 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value_seq) = ndt_memory_seq_append((yyvsp[-2].typed_value_seq), (yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2447 "grammar.c" /* yacc.c:1646  */
    break;

  case 85:
#line 319 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_memory_from_number((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2453 "grammar.c" /* yacc.c:1646  */
    break;

  case 86:
#line 320 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_memory_from_number((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2459 "grammar.c" /* yacc.c:1646  */
    break;

  case 87:
#line 321 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_memory_from_string((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2465 "grammar.c" /* yacc.c:1646  */
    break;

  case 88:
#line 324 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2471 "grammar.c" /* yacc.c:1646  */
    break;

  case 89:
#line 325 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Variadic; }
#line 2477 "grammar.c" /* yacc.c:1646  */
    break;

  case 90:
#line 328 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2483 "grammar.c" /* yacc.c:1646  */
    break;

  case 91:
#line 329 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2489 "grammar.c" /* yacc.c:1646  */
    break;

  case 92:
#line 330 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Variadic; }
#line 2495 "grammar.c" /* yacc.c:1646  */
    break;

  case 93:
#line 333 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2501 "grammar.c" /* yacc.c:1646  */
    break;

  case 94:
#line 334 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].tuple_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2507 "grammar.c" /* yacc.c:1646  */
    break;

  case 95:
#line 337 "grammar.y" /* yacc.c:1646  */
    { (yyval.tuple_field_seq) = ndt_tuple_field_seq_new((yyvsp[0].tuple_field), ctx); if ((yyval.tuple_field_seq) == NULL) YYABORT; }
#line 2513 "grammar.c" /* yacc.c:1646  */
    break;

  case 96:
#line 338 "grammar.y" /* yacc.c:1646  */
    { (yyval.tuple_field_seq) = ndt_tuple_field_seq_append((yyvsp[-2].tuple_field_seq), (yyvsp[0].tuple_field), ctx); if ((yyval.tuple_field_seq) == NULL) YYABORT; }
#line 2519 "grammar.c" /* yacc.c:1646  */
    break;

  case 97:
#line 341 "grammar.y" /* yacc.c:1646  */
    { (yyval.tuple_field) = mk_tuple_field((yyvsp[-1].ndt), (yyvsp[0].attribute_seq), ctx); if ((yyval.tuple_field) == NULL) YYABORT; }
#line 2525 "grammar.c" /* yacc.c:1646  */
    break;

  case 98:
#line 344 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2531 "grammar.c" /* yacc.c:1646  */
    break;

  case 99:
#line 345 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].record_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2537 "grammar.c" /* yacc.c:1646  */
    break;

  case 100:
#line 348 "grammar.y" /* yacc.c:1646  */
    { (yyval.record_field_seq) = ndt_record_field_seq_new((yyvsp[0].record_field), ctx); if ((yyval.record_field_seq) == NULL) YYABORT; }
#line 2543 "grammar.c" /* yacc.c:1646  */
    break;

  case 101:
#line 349 "grammar.y" /* yacc.c:1646  */
    { (yyval.record_field_seq) = ndt_record_field_seq_append((yyvsp[-2].record_field_seq), (yyvsp[0].record_field), ctx); if ((yyval.record_field_seq) == NULL) YYABORT; }
#line 2549 "grammar.c" /* yacc.c:1646  */
    break;

  case 102:
#line 352 "grammar.y" /* yacc.c:1646  */
    { (yyval.record_field) = ndt_record_field((yyvsp[-3].string), (yyvsp[-1].ndt), ctx); if ((yyval.record_field) == NULL) YYABORT; /* XXX: use attributes */ }
#line 2555 "grammar.c" /* yacc.c:1646  */
    break;

  case 103:
#line 355 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2561 "grammar.c" /* yacc.c:1646  */
    break;

  case 104:
#line 356 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2567 "grammar.c" /* yacc.c:1646  */
    break;

  case 105:
#line 357 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2573 "grammar.c" /* yacc.c:1646  */
    break;

  case 106:
#line 360 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = NULL; }
#line 2579 "grammar.c" /* yacc.c:1646  */
    break;

  case 107:
#line 361 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = (yyvsp[-1].attribute_seq); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2585 "grammar.c" /* yacc.c:1646  */
    break;

  case 108:
#line 364 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = ndt_attr_seq_new((yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2591 "grammar.c" /* yacc.c:1646  */
    break;

  case 109:
#line 365 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = ndt_attr_seq_append((yyvsp[-2].attribute_seq), (yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2597 "grammar.c" /* yacc.c:1646  */
    break;

  case 110:
#line 368 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute) = ndt_attr_from_number((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2603 "grammar.c" /* yacc.c:1646  */
    break;

  case 111:
#line 369 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute) = ndt_attr_from_string((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2609 "grammar.c" /* yacc.c:1646  */
    break;

  case 112:
#line 370 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute) = ndt_attr_from_type((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2615 "grammar.c" /* yacc.c:1646  */
    break;

  case 113:
#line 374 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function_from_tuple((yyvsp[0].ndt), (yyvsp[-2].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2621 "grammar.c" /* yacc.c:1646  */
    break;

  case 114:
#line 376 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Nonvariadic, NULL, (yyvsp[-3].variadic_flag), (yyvsp[-4].record_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2627 "grammar.c" /* yacc.c:1646  */
    break;

  case 115:
#line 378 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Variadic, NULL, (yyvsp[-3].variadic_flag), (yyvsp[-4].record_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2633 "grammar.c" /* yacc.c:1646  */
    break;

  case 116:
#line 380 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Nonvariadic, (yyvsp[-6].tuple_field_seq), (yyvsp[-3].variadic_flag), (yyvsp[-4].record_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2639 "grammar.c" /* yacc.c:1646  */
    break;

  case 117:
#line 382 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Variadic, (yyvsp[-8].tuple_field_seq), (yyvsp[-3].variadic_flag), (yyvsp[-4].record_field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2645 "grammar.c" /* yacc.c:1646  */
    break;


#line 2649 "grammar.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, scanner, ast, ctx, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, ast, ctx, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner, ast, ctx);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, scanner, ast, ctx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, ast, ctx, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, ast, ctx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, ast, ctx);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
