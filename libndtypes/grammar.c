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
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, plures
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
#include "lexer.h"


#undef yyerror
#undef yylex

void
yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta,
        ndt_context_t *ctx, const char *msg)
{
    (void)scanner;
    (void)ast;
    (void)meta;

    ndt_err_format(ctx, NDT_ParseError, "%d:%d: %s\n", loc->first_line,
                   loc->first_column, msg);
}

int
yylex(YYSTYPE *val, YYLTYPE *loc, yyscan_t scanner, ndt_context_t *ctx)
{
    return lexfunc(val, loc, scanner, ctx);
}

#line 126 "grammar.c" /* yacc.c:339  */

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
#line 61 "grammar.y" /* yacc.c:355  */

  #include "ndtypes.h"
  #include "seq.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;

#line 164 "grammar.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ENDMARKER = 0,
    ANY_KIND = 258,
    SCALAR_KIND = 259,
    VOID = 260,
    BOOL = 261,
    SIGNED_KIND = 262,
    INT8 = 263,
    INT16 = 264,
    INT32 = 265,
    INT64 = 266,
    UNSIGNED_KIND = 267,
    UINT8 = 268,
    UINT16 = 269,
    UINT32 = 270,
    UINT64 = 271,
    FLOAT_KIND = 272,
    FLOAT16 = 273,
    FLOAT32 = 274,
    FLOAT64 = 275,
    COMPLEX_KIND = 276,
    COMPLEX32 = 277,
    COMPLEX64 = 278,
    COMPLEX128 = 279,
    CATEGORICAL = 280,
    NA = 281,
    INTPTR = 282,
    UINTPTR = 283,
    SIZE = 284,
    CHAR = 285,
    STRING = 286,
    FIXED_STRING_KIND = 287,
    FIXED_STRING = 288,
    BYTES = 289,
    FIXED_BYTES_KIND = 290,
    FIXED_BYTES = 291,
    REF = 292,
    FIXED = 293,
    VAR = 294,
    COMMA = 295,
    DOT = 296,
    COLON = 297,
    LPAREN = 298,
    RPAREN = 299,
    LBRACE = 300,
    RBRACE = 301,
    LBRACK = 302,
    RBRACK = 303,
    STAR = 304,
    ELLIPSIS = 305,
    RARROW = 306,
    EQUAL = 307,
    QUESTIONMARK = 308,
    AMPERSAND = 309,
    BAR = 310,
    ERRTOKEN = 311,
    INTEGER = 312,
    FLOATNUMBER = 313,
    STRINGLIT = 314,
    NAME_LOWER = 315,
    NAME_UPPER = 316,
    NAME_OTHER = 317,
    NAMED_ELLIPSIS = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 89 "grammar.y" /* yacc.c:355  */

    ndt_t *ndt;
    ndt_field_t *field;
    ndt_field_seq_t *field_seq;
    ndt_value_t *typed_value;
    ndt_value_seq_t *typed_value_seq;
    ndt_attr_t *attribute;
    ndt_attr_seq_t *attribute_seq;
    enum ndt_variadic variadic_flag;
    enum ndt_encoding encoding;
    char *string;
    ndt_string_seq_t *string_seq;

#line 255 "grammar.c" /* yacc.c:355  */
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



int yyparse (yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx);
/* "%code provides" blocks.  */
#line 69 "grammar.y" /* yacc.c:355  */

  #define YY_DECL extern int lexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int lexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx, const char *msg);

#line 285 "grammar.c" /* yacc.c:355  */

#endif /* !YY_YY_GRAMMAR_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 291 "grammar.c" /* yacc.c:358  */

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
#define YYFINAL  122
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   599

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  235

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
       0,   192,   192,   196,   197,   201,   202,   204,   207,   208,
     211,   212,   213,   214,   215,   216,   219,   220,   221,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   235,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   260,
     261,   262,   263,   266,   267,   268,   269,   272,   273,   274,
     277,   278,   279,   283,   284,   285,   288,   289,   292,   295,
     296,   299,   302,   305,   308,   309,   312,   315,   316,   319,
     320,   321,   322,   325,   326,   329,   330,   331,   334,   335,
     336,   339,   340,   343,   344,   347,   348,   349,   352,   353,
     356,   357,   360,   361,   362,   365,   366,   369,   370,   373,
     374,   377,   378,   381,   382,   383,   384,   387,   389,   391,
     393,   395
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "ANY_KIND", "SCALAR_KIND",
  "VOID", "BOOL", "SIGNED_KIND", "INT8", "INT16", "INT32", "INT64",
  "UNSIGNED_KIND", "UINT8", "UINT16", "UINT32", "UINT64", "FLOAT_KIND",
  "FLOAT16", "FLOAT32", "FLOAT64", "COMPLEX_KIND", "COMPLEX32",
  "COMPLEX64", "COMPLEX128", "CATEGORICAL", "NA", "INTPTR", "UINTPTR",
  "SIZE", "CHAR", "STRING", "FIXED_STRING_KIND", "FIXED_STRING", "BYTES",
  "FIXED_BYTES_KIND", "FIXED_BYTES", "REF", "FIXED", "VAR", "COMMA", "DOT",
  "COLON", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACK", "RBRACK",
  "STAR", "ELLIPSIS", "RARROW", "EQUAL", "QUESTIONMARK", "AMPERSAND",
  "BAR", "ERRTOKEN", "INTEGER", "FLOATNUMBER", "STRINGLIT", "NAME_LOWER",
  "NAME_UPPER", "NAME_OTHER", "NAMED_ELLIPSIS", "$accept", "input",
  "datashape_or_module", "datashape", "dimensions", "dimensions_nooption",
  "dimensions_tail", "dtype", "scalar", "signed", "unsigned", "ieee_float",
  "ieee_complex", "alias", "character", "string", "fixed_string",
  "encoding", "bytes", "fixed_bytes", "ref", "categorical",
  "typed_value_seq", "typed_value", "variadic_flag", "comma_variadic_flag",
  "tuple_type", "tuple_field_seq", "tuple_field", "record_type",
  "record_field_seq", "record_field", "record_field_name", "arguments_opt",
  "attribute_seq", "attribute", "untyped_value_seq", "untyped_value",
  "function_type", YY_NULLPTR
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

#define YYPACT_NINF -190

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-190)))

#define YYTABLE_NINF -104

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     292,  -190,  -190,   -31,   -31,  -190,   -31,   -31,   -31,   -31,
    -190,   -31,   -31,   -31,   -31,  -190,   -31,   -31,   -31,  -190,
     -31,   -31,   -31,    13,   -31,   -31,   -31,    14,  -190,  -190,
      16,   -31,  -190,    56,    57,    62,   -31,   169,   -29,    26,
     536,   353,    40,  -190,    19,    48,    95,   106,  -190,  -190,
    -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,
    -190,  -190,  -190,  -190,  -190,    61,  -190,  -190,   -47,  -190,
    -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,
    -190,  -190,  -190,  -190,  -190,   -15,  -190,  -190,  -190,    58,
      63,  -190,   -47,   353,   -47,    64,   -11,    73,    27,  -190,
      66,    74,    88,  -190,    90,  -190,    93,  -190,  -190,  -190,
      94,   104,   414,    24,  -190,  -190,  -190,   414,   109,   475,
     414,   414,  -190,  -190,   353,   101,   -17,  -190,  -190,  -190,
    -190,  -190,    37,  -190,  -190,   110,    44,    46,   111,    47,
     414,    65,   -47,  -190,   231,   114,   -10,   115,   353,  -190,
       4,   116,   536,  -190,  -190,  -190,  -190,   353,   101,   119,
      52,  -190,  -190,  -190,   -12,   -47,  -190,   -15,  -190,  -190,
      58,  -190,  -190,  -190,   118,  -190,    90,   -33,     9,   -16,
    -190,    90,    54,  -190,  -190,  -190,   113,   105,   101,    32,
    -190,  -190,   121,  -190,  -190,    51,  -190,  -190,  -190,  -190,
    -190,  -190,  -190,   124,   414,   127,  -190,    65,   151,  -190,
     353,   -47,  -190,  -190,    23,  -190,  -190,  -190,   158,    90,
     160,  -190,   -30,    51,  -190,   353,   171,   353,  -190,  -190,
    -190,   162,  -190,   353,  -190
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    19,    20,   105,   105,    31,   105,   105,   105,   105,
      33,   105,   105,   105,   105,    35,   105,   105,   105,    37,
     105,   105,   105,     0,   105,   105,   105,    66,    68,    42,
       0,   105,    45,     0,     0,     0,   105,    83,    83,     0,
       0,     0,     0,    25,    28,     0,     0,     0,     3,     5,
       8,     6,    21,    32,    34,    36,    38,    39,    40,    41,
      43,    44,    46,    48,    47,    22,    23,    24,     0,    29,
      30,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,     0,    63,    64,    65,     0,
       0,    72,     0,     0,     0,     0,    84,    25,    28,   104,
      93,     0,    85,    91,    85,    98,     0,    84,   102,   103,
       0,    85,     0,    28,     9,     7,    75,     0,     0,     0,
       0,     0,     1,     2,     0,     0,     0,   107,    82,    79,
      80,    81,     0,    77,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    88,    86,     0,    86,     0,     0,    95,
      86,     0,     0,    18,    14,    16,    10,     0,    25,     0,
       0,    12,    15,   117,     0,     0,   106,     0,    76,    67,
       0,    69,    73,    74,     0,    13,    85,     0,    87,    25,
      92,    85,     0,    89,    87,    99,     0,   100,   102,     0,
      96,    17,     0,    26,    27,     0,   114,   115,   116,   113,
     109,   108,    78,     0,     0,     0,    94,     0,     0,    90,
       0,     0,    97,     4,     0,   111,    70,    11,     0,    85,
       0,   118,     0,     0,   110,     0,     0,     0,   101,   112,
     119,     0,   120,     0,   121
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -190,  -190,  -190,     0,  -103,   -36,  -101,   -38,  -190,  -190,
    -190,  -190,  -190,  -190,  -190,  -190,  -190,    -1,  -190,  -190,
    -190,  -190,  -190,    49,   179,   -96,  -190,  -190,    76,  -190,
     -37,  -122,  -190,   125,   -89,    53,  -190,  -189,  -190
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    46,    47,   100,    49,    50,   154,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   135,    61,    62,
      63,    64,   132,   133,   101,   145,    65,   102,   103,    66,
     104,   105,   106,    69,   126,   127,   214,   200,    67
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   111,   115,   137,   114,   139,   215,   165,   147,   153,
     165,   128,    68,   125,   153,   151,   156,   153,   153,   161,
     162,   107,   206,   165,   185,   228,  -102,   166,   185,   141,
     160,   108,   109,    99,   229,   195,   164,   153,   112,   175,
     184,   116,   129,   130,   131,   196,   197,   198,   199,   207,
     108,   109,    99,   177,   184,   182,    85,    89,   112,    90,
     118,   189,   119,   223,   188,   109,    99,   119,   120,  -103,
     119,   224,   165,   120,   155,   112,   120,   167,   212,   155,
     205,   168,   155,   155,   170,   208,   165,   165,   171,   117,
     172,   174,   165,   138,   165,   122,   194,   121,   209,    92,
      93,   153,   155,   217,   176,    94,   123,   181,   196,   197,
     198,   199,   124,   140,   191,  -102,   114,   134,   143,   159,
     136,   142,   222,   226,   163,   108,   109,    99,   144,    70,
     146,    71,    72,    73,    74,   148,    75,    76,    77,    78,
     149,    79,    80,    81,   150,    82,    83,    84,   187,    86,
      87,    88,   157,   164,   169,   173,    91,   192,   183,   186,
     211,    95,   190,   193,   210,   213,   155,   204,   216,   203,
     219,   218,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,   220,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,   225,
     221,   227,    37,   233,    38,   231,   202,   110,   201,    96,
     180,     0,    40,    41,     0,   230,    42,   232,     0,    97,
      98,    99,    45,   234,     1,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,     0,     0,     0,    37,     0,    38,     0,     0,     0,
       0,   178,     0,     0,    40,    41,     0,     0,    42,     0,
       0,   179,    98,    99,    45,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     0,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,     0,     0,     0,    37,     0,    38,     0,     0,
       0,     0,    39,     0,     0,    40,    41,     0,     0,    42,
       0,     0,    43,    44,     0,    45,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,     0,     0,     0,    37,     0,    38,     0,
       0,     0,     0,    39,     0,     0,    40,    41,     0,     0,
      42,     0,     0,    43,   113,     0,    45,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,     0,     0,     0,    37,     0,    38,
       0,     0,     0,     0,    39,     0,     0,   152,    41,     0,
       0,    42,     0,     0,    43,   113,     0,    45,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,     0,     0,     0,    37,     0,
      38,     0,     0,     0,     0,    39,     0,     0,    40,    41,
       0,     0,    42,     0,     0,   158,   113,     0,    45,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,     0,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,     0,     0,     0,    37,
       0,    38,     0,     0,     0,     0,    39,     0,     0,     0,
      41,     0,     0,    42,     0,     0,    43,   113,     0,    45
};

static const yytype_int16 yycheck[] =
{
       0,    38,    40,    92,    40,    94,   195,    40,   104,   112,
      40,    26,    43,    60,   117,   111,   117,   120,   121,   120,
     121,    50,    55,    40,   146,    55,    42,    44,   150,    40,
     119,    60,    61,    62,   223,    47,    52,   140,    49,   140,
      50,    41,    57,    58,    59,    57,    58,    59,    60,    40,
      60,    61,    62,   142,    50,   144,    43,    43,    49,    43,
      41,   150,    43,    40,    60,    61,    62,    43,    49,    42,
      43,    48,    40,    49,   112,    49,    49,    40,    46,   117,
     176,    44,   120,   121,    40,   181,    40,    40,    44,    49,
      44,    44,    40,    93,    40,     0,    44,    49,    44,    43,
      43,   204,   140,   204,   141,    43,     0,   144,    57,    58,
      59,    60,    51,    49,   152,    42,   152,    59,    44,   119,
      57,    55,   211,   219,   124,    60,    61,    62,    40,     4,
      40,     6,     7,     8,     9,    42,    11,    12,    13,    14,
      46,    16,    17,    18,    40,    20,    21,    22,   148,    24,
      25,    26,    43,    52,    44,    44,    31,   157,    44,    44,
      55,    36,    46,    44,    51,    44,   204,    49,    44,   170,
     207,    44,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    44,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    51,
     210,    51,    43,    51,    45,    44,   167,    38,   165,    50,
     144,    -1,    53,    54,    -1,   225,    57,   227,    -1,    60,
      61,    62,    63,   233,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    53,    54,    -1,    -1,    57,    -1,
      -1,    60,    61,    62,    63,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    -1,    -1,    43,    -1,    45,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    53,    54,    -1,    -1,    57,
      -1,    -1,    60,    61,    -1,    63,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    -1,    -1,    -1,    43,    -1,    45,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    53,    54,    -1,    -1,
      57,    -1,    -1,    60,    61,    -1,    63,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    -1,    43,    -1,    45,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    54,    -1,
      -1,    57,    -1,    -1,    60,    61,    -1,    63,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    43,    -1,
      45,    -1,    -1,    -1,    -1,    50,    -1,    -1,    53,    54,
      -1,    -1,    57,    -1,    -1,    60,    61,    -1,    63,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    43,
      -1,    45,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      54,    -1,    -1,    57,    -1,    -1,    60,    61,    -1,    63
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    43,    45,    50,
      53,    54,    57,    60,    61,    63,    65,    66,    67,    68,
      69,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    82,    83,    84,    85,    90,    93,   102,    43,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    43,    97,    97,    97,    43,
      43,    97,    43,    43,    43,    97,    50,    60,    61,    62,
      67,    88,    91,    92,    94,    95,    96,    50,    60,    61,
      88,    94,    49,    61,    69,    71,    67,    49,    41,    43,
      49,    49,     0,     0,    51,    60,    98,    99,    26,    57,
      58,    59,    86,    87,    59,    81,    57,    98,    67,    98,
      49,    40,    55,    44,    40,    89,    40,    89,    42,    46,
      40,    89,    53,    68,    70,    71,    70,    43,    60,    67,
      98,    70,    70,    67,    52,    40,    44,    40,    44,    44,
      40,    44,    44,    44,    44,    70,    94,    98,    50,    60,
      92,    94,    98,    44,    50,    95,    44,    67,    60,    98,
      46,    71,    67,    44,    44,    47,    57,    58,    59,    60,
     101,    99,    87,    81,    49,    89,    55,    40,    89,    44,
      51,    55,    46,    44,   100,   101,    44,    70,    44,    94,
      44,    67,    98,    40,    48,    51,    89,    51,    55,   101,
      67,    44,    67,    51,    67
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    64,    65,    66,    66,    67,    67,    67,    68,    68,
      69,    69,    69,    69,    69,    69,    70,    70,    70,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    73,
      73,    73,    73,    74,    74,    74,    74,    75,    75,    75,
      76,    76,    76,    77,    77,    77,    78,    78,    79,    80,
      80,    81,    82,    83,    84,    84,    85,    86,    86,    87,
      87,    87,    87,    88,    88,    89,    89,    89,    90,    90,
      90,    91,    91,    92,    92,    93,    93,    93,    94,    94,
      95,    95,    96,    96,    96,    97,    97,    98,    98,    99,
      99,   100,   100,   101,   101,   101,   101,   102,   102,   102,
     102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     5,     1,     1,     2,     1,     2,
       3,     6,     3,     4,     3,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     4,     1,     4,
       6,     1,     2,     4,     4,     2,     4,     1,     3,     1,
       1,     1,     1,     0,     1,     0,     1,     2,     3,     4,
       5,     1,     3,     1,     4,     3,     4,     5,     1,     3,
       3,     6,     1,     1,     1,     0,     3,     1,     3,     3,
       5,     1,     3,     1,     1,     1,     1,     3,     6,     8,
       8,    10
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
      yyerror (&yylloc, scanner, ast, meta, ctx, YY_("syntax error: cannot back up")); \
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
                  Type, Value, Location, scanner, ast, meta, ctx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (ast);
  YYUSE (meta);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, scanner, ast, meta, ctx);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, ast, meta, ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, ast, meta, ctx); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (ast);
  YYUSE (meta);
  YYUSE (ctx);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 57: /* INTEGER  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1441 "grammar.c" /* yacc.c:1257  */
        break;

    case 58: /* FLOATNUMBER  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1447 "grammar.c" /* yacc.c:1257  */
        break;

    case 59: /* STRINGLIT  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1453 "grammar.c" /* yacc.c:1257  */
        break;

    case 60: /* NAME_LOWER  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1459 "grammar.c" /* yacc.c:1257  */
        break;

    case 61: /* NAME_UPPER  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1465 "grammar.c" /* yacc.c:1257  */
        break;

    case 62: /* NAME_OTHER  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1471 "grammar.c" /* yacc.c:1257  */
        break;

    case 63: /* NAMED_ELLIPSIS  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1477 "grammar.c" /* yacc.c:1257  */
        break;

    case 65: /* input  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1483 "grammar.c" /* yacc.c:1257  */
        break;

    case 66: /* datashape_or_module  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1489 "grammar.c" /* yacc.c:1257  */
        break;

    case 67: /* datashape  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1495 "grammar.c" /* yacc.c:1257  */
        break;

    case 68: /* dimensions  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1501 "grammar.c" /* yacc.c:1257  */
        break;

    case 69: /* dimensions_nooption  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1507 "grammar.c" /* yacc.c:1257  */
        break;

    case 70: /* dimensions_tail  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1513 "grammar.c" /* yacc.c:1257  */
        break;

    case 71: /* dtype  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1519 "grammar.c" /* yacc.c:1257  */
        break;

    case 72: /* scalar  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1525 "grammar.c" /* yacc.c:1257  */
        break;

    case 73: /* signed  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1531 "grammar.c" /* yacc.c:1257  */
        break;

    case 74: /* unsigned  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1537 "grammar.c" /* yacc.c:1257  */
        break;

    case 75: /* ieee_float  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1543 "grammar.c" /* yacc.c:1257  */
        break;

    case 76: /* ieee_complex  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1549 "grammar.c" /* yacc.c:1257  */
        break;

    case 77: /* alias  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1555 "grammar.c" /* yacc.c:1257  */
        break;

    case 78: /* character  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1561 "grammar.c" /* yacc.c:1257  */
        break;

    case 79: /* string  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1567 "grammar.c" /* yacc.c:1257  */
        break;

    case 80: /* fixed_string  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1573 "grammar.c" /* yacc.c:1257  */
        break;

    case 82: /* bytes  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1579 "grammar.c" /* yacc.c:1257  */
        break;

    case 83: /* fixed_bytes  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1585 "grammar.c" /* yacc.c:1257  */
        break;

    case 84: /* ref  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1591 "grammar.c" /* yacc.c:1257  */
        break;

    case 85: /* categorical  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1597 "grammar.c" /* yacc.c:1257  */
        break;

    case 86: /* typed_value_seq  */
#line 183 "grammar.y" /* yacc.c:1257  */
      { ndt_value_seq_del(((*yyvaluep).typed_value_seq)); }
#line 1603 "grammar.c" /* yacc.c:1257  */
        break;

    case 87: /* typed_value  */
#line 182 "grammar.y" /* yacc.c:1257  */
      { ndt_value_del(((*yyvaluep).typed_value)); }
#line 1609 "grammar.c" /* yacc.c:1257  */
        break;

    case 90: /* tuple_type  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1615 "grammar.c" /* yacc.c:1257  */
        break;

    case 91: /* tuple_field_seq  */
#line 181 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1621 "grammar.c" /* yacc.c:1257  */
        break;

    case 92: /* tuple_field  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1627 "grammar.c" /* yacc.c:1257  */
        break;

    case 93: /* record_type  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1633 "grammar.c" /* yacc.c:1257  */
        break;

    case 94: /* record_field_seq  */
#line 181 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1639 "grammar.c" /* yacc.c:1257  */
        break;

    case 95: /* record_field  */
#line 180 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1645 "grammar.c" /* yacc.c:1257  */
        break;

    case 96: /* record_field_name  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1651 "grammar.c" /* yacc.c:1257  */
        break;

    case 97: /* arguments_opt  */
#line 185 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1657 "grammar.c" /* yacc.c:1257  */
        break;

    case 98: /* attribute_seq  */
#line 185 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1663 "grammar.c" /* yacc.c:1257  */
        break;

    case 99: /* attribute  */
#line 184 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_del(((*yyvaluep).attribute)); }
#line 1669 "grammar.c" /* yacc.c:1257  */
        break;

    case 100: /* untyped_value_seq  */
#line 187 "grammar.y" /* yacc.c:1257  */
      { ndt_string_seq_del(((*yyvaluep).string_seq)); }
#line 1675 "grammar.c" /* yacc.c:1257  */
        break;

    case 101: /* untyped_value  */
#line 186 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1681 "grammar.c" /* yacc.c:1257  */
        break;

    case 102: /* function_type  */
#line 179 "grammar.y" /* yacc.c:1257  */
      { ndt_del(((*yyvaluep).ndt)); }
#line 1687 "grammar.c" /* yacc.c:1257  */
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
yyparse (yyscan_t scanner, ndt_t **ast, ndt_meta_t *meta, ndt_context_t *ctx)
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
#line 79 "grammar.y" /* yacc.c:1429  */
{
   yylloc.first_line = 1;
   yylloc.first_column = 1;
   yylloc.last_line = 1;
   yylloc.last_column = 1;
}

#line 1803 "grammar.c" /* yacc.c:1429  */
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
#line 192 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[-1].ndt);  *ast = (yyval.ndt); YYACCEPT; }
#line 1992 "grammar.c" /* yacc.c:1646  */
    break;

  case 3:
#line 196 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1998 "grammar.c" /* yacc.c:1646  */
    break;

  case 4:
#line 197 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_module((yyvsp[-4].string), (yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2004 "grammar.c" /* yacc.c:1646  */
    break;

  case 5:
#line 201 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2010 "grammar.c" /* yacc.c:1646  */
    break;

  case 6:
#line 202 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2016 "grammar.c" /* yacc.c:1646  */
    break;

  case 7:
#line 204 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_option((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2022 "grammar.c" /* yacc.c:1646  */
    break;

  case 8:
#line 207 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2028 "grammar.c" /* yacc.c:1646  */
    break;

  case 9:
#line 208 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_dim_option((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2034 "grammar.c" /* yacc.c:1646  */
    break;

  case 10:
#line 211 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_dim_from_shape((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2040 "grammar.c" /* yacc.c:1646  */
    break;

  case 11:
#line 212 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_dim_from_attrs((yyvsp[-3].attribute_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2046 "grammar.c" /* yacc.c:1646  */
    break;

  case 12:
#line 213 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_symbolic_dim((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2052 "grammar.c" /* yacc.c:1646  */
    break;

  case 13:
#line 214 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_var_dim(meta, (yyvsp[-2].attribute_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2058 "grammar.c" /* yacc.c:1646  */
    break;

  case 14:
#line 215 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_ellipsis_dim(NULL, (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2064 "grammar.c" /* yacc.c:1646  */
    break;

  case 15:
#line 216 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_ellipsis_dim((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2070 "grammar.c" /* yacc.c:1646  */
    break;

  case 16:
#line 219 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2076 "grammar.c" /* yacc.c:1646  */
    break;

  case 17:
#line 220 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_item_option((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2082 "grammar.c" /* yacc.c:1646  */
    break;

  case 18:
#line 221 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2088 "grammar.c" /* yacc.c:1646  */
    break;

  case 19:
#line 224 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_any_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2094 "grammar.c" /* yacc.c:1646  */
    break;

  case 20:
#line 225 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_scalar_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2100 "grammar.c" /* yacc.c:1646  */
    break;

  case 21:
#line 226 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2106 "grammar.c" /* yacc.c:1646  */
    break;

  case 22:
#line 227 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2112 "grammar.c" /* yacc.c:1646  */
    break;

  case 23:
#line 228 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2118 "grammar.c" /* yacc.c:1646  */
    break;

  case 24:
#line 229 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2124 "grammar.c" /* yacc.c:1646  */
    break;

  case 25:
#line 230 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2130 "grammar.c" /* yacc.c:1646  */
    break;

  case 26:
#line 231 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_constr((yyvsp[-3].string), (yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2136 "grammar.c" /* yacc.c:1646  */
    break;

  case 27:
#line 232 "grammar.y" /* yacc.c:1646  */
    { (void)(yyvsp[-3].string); (void)(yyvsp[-1].attribute_seq); ndt_free((yyvsp[-3].string)); ndt_attr_seq_del((yyvsp[-1].attribute_seq)); (yyval.ndt) = NULL;
                                            ndt_err_format(ctx, NDT_NotImplementedError, "general attributes are not implemented");
                                            YYABORT; }
#line 2144 "grammar.c" /* yacc.c:1646  */
    break;

  case 28:
#line 235 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_typevar((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2150 "grammar.c" /* yacc.c:1646  */
    break;

  case 29:
#line 238 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Void, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2156 "grammar.c" /* yacc.c:1646  */
    break;

  case 30:
#line 239 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Bool, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2162 "grammar.c" /* yacc.c:1646  */
    break;

  case 31:
#line 240 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_signed_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2168 "grammar.c" /* yacc.c:1646  */
    break;

  case 32:
#line 241 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2174 "grammar.c" /* yacc.c:1646  */
    break;

  case 33:
#line 242 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_unsigned_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2180 "grammar.c" /* yacc.c:1646  */
    break;

  case 34:
#line 243 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2186 "grammar.c" /* yacc.c:1646  */
    break;

  case 35:
#line 244 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_float_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2192 "grammar.c" /* yacc.c:1646  */
    break;

  case 36:
#line 245 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2198 "grammar.c" /* yacc.c:1646  */
    break;

  case 37:
#line 246 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_complex_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2204 "grammar.c" /* yacc.c:1646  */
    break;

  case 38:
#line 247 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2210 "grammar.c" /* yacc.c:1646  */
    break;

  case 39:
#line 248 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2216 "grammar.c" /* yacc.c:1646  */
    break;

  case 40:
#line 249 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2222 "grammar.c" /* yacc.c:1646  */
    break;

  case 41:
#line 250 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2228 "grammar.c" /* yacc.c:1646  */
    break;

  case 42:
#line 251 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_fixed_string_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2234 "grammar.c" /* yacc.c:1646  */
    break;

  case 43:
#line 252 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2240 "grammar.c" /* yacc.c:1646  */
    break;

  case 44:
#line 253 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2246 "grammar.c" /* yacc.c:1646  */
    break;

  case 45:
#line 254 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_fixed_bytes_kind(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2252 "grammar.c" /* yacc.c:1646  */
    break;

  case 46:
#line 255 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2258 "grammar.c" /* yacc.c:1646  */
    break;

  case 47:
#line 256 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2264 "grammar.c" /* yacc.c:1646  */
    break;

  case 48:
#line 257 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2270 "grammar.c" /* yacc.c:1646  */
    break;

  case 49:
#line 260 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Int8, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2276 "grammar.c" /* yacc.c:1646  */
    break;

  case 50:
#line 261 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Int16, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2282 "grammar.c" /* yacc.c:1646  */
    break;

  case 51:
#line 262 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Int32, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2288 "grammar.c" /* yacc.c:1646  */
    break;

  case 52:
#line 263 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Int64, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2294 "grammar.c" /* yacc.c:1646  */
    break;

  case 53:
#line 266 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Uint8, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2300 "grammar.c" /* yacc.c:1646  */
    break;

  case 54:
#line 267 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Uint16, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2306 "grammar.c" /* yacc.c:1646  */
    break;

  case 55:
#line 268 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Uint32, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2312 "grammar.c" /* yacc.c:1646  */
    break;

  case 56:
#line 269 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Uint64, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2318 "grammar.c" /* yacc.c:1646  */
    break;

  case 57:
#line 272 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Float16, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2324 "grammar.c" /* yacc.c:1646  */
    break;

  case 58:
#line 273 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Float32, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2330 "grammar.c" /* yacc.c:1646  */
    break;

  case 59:
#line 274 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Float64, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2336 "grammar.c" /* yacc.c:1646  */
    break;

  case 60:
#line 277 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Complex32, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2342 "grammar.c" /* yacc.c:1646  */
    break;

  case 61:
#line 278 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Complex64, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2348 "grammar.c" /* yacc.c:1646  */
    break;

  case 62:
#line 279 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_primitive(Complex128, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2354 "grammar.c" /* yacc.c:1646  */
    break;

  case 63:
#line 283 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_alias(Intptr, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2360 "grammar.c" /* yacc.c:1646  */
    break;

  case 64:
#line 284 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_alias(Uintptr, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2366 "grammar.c" /* yacc.c:1646  */
    break;

  case 65:
#line 285 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_alias(Size, (yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2372 "grammar.c" /* yacc.c:1646  */
    break;

  case 66:
#line 288 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_char(Utf32, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2378 "grammar.c" /* yacc.c:1646  */
    break;

  case 67:
#line 289 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_char((yyvsp[-1].encoding), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2384 "grammar.c" /* yacc.c:1646  */
    break;

  case 68:
#line 292 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_string(ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2390 "grammar.c" /* yacc.c:1646  */
    break;

  case 69:
#line 295 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-1].string), Utf8, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2396 "grammar.c" /* yacc.c:1646  */
    break;

  case 70:
#line 296 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-3].string), (yyvsp[-1].encoding), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2402 "grammar.c" /* yacc.c:1646  */
    break;

  case 71:
#line 299 "grammar.y" /* yacc.c:1646  */
    { (yyval.encoding) = ndt_encoding_from_string((yyvsp[0].string), ctx); if ((yyval.encoding) == ErrorEncoding) YYABORT; }
#line 2408 "grammar.c" /* yacc.c:1646  */
    break;

  case 72:
#line 302 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_bytes((yyvsp[0].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2414 "grammar.c" /* yacc.c:1646  */
    break;

  case 73:
#line 305 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_fixed_bytes((yyvsp[-1].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2420 "grammar.c" /* yacc.c:1646  */
    break;

  case 74:
#line 308 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_ref((yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2426 "grammar.c" /* yacc.c:1646  */
    break;

  case 75:
#line 309 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = ndt_ref((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2432 "grammar.c" /* yacc.c:1646  */
    break;

  case 76:
#line 312 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_categorical((yyvsp[-1].typed_value_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2438 "grammar.c" /* yacc.c:1646  */
    break;

  case 77:
#line 315 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value_seq) = ndt_value_seq_new((yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2444 "grammar.c" /* yacc.c:1646  */
    break;

  case 78:
#line 316 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value_seq) = ndt_value_seq_append((yyvsp[-2].typed_value_seq), (yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2450 "grammar.c" /* yacc.c:1646  */
    break;

  case 79:
#line 319 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_value_from_number(ValInt64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2456 "grammar.c" /* yacc.c:1646  */
    break;

  case 80:
#line 320 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_value_from_number(ValFloat64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2462 "grammar.c" /* yacc.c:1646  */
    break;

  case 81:
#line 321 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_value_from_string((yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2468 "grammar.c" /* yacc.c:1646  */
    break;

  case 82:
#line 322 "grammar.y" /* yacc.c:1646  */
    { (yyval.typed_value) = ndt_value_na(ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2474 "grammar.c" /* yacc.c:1646  */
    break;

  case 83:
#line 325 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2480 "grammar.c" /* yacc.c:1646  */
    break;

  case 84:
#line 326 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Variadic; }
#line 2486 "grammar.c" /* yacc.c:1646  */
    break;

  case 85:
#line 329 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2492 "grammar.c" /* yacc.c:1646  */
    break;

  case 86:
#line 330 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2498 "grammar.c" /* yacc.c:1646  */
    break;

  case 87:
#line 331 "grammar.y" /* yacc.c:1646  */
    { (yyval.variadic_flag) = Variadic; }
#line 2504 "grammar.c" /* yacc.c:1646  */
    break;

  case 88:
#line 334 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2510 "grammar.c" /* yacc.c:1646  */
    break;

  case 89:
#line 335 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2516 "grammar.c" /* yacc.c:1646  */
    break;

  case 90:
#line 336 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2522 "grammar.c" /* yacc.c:1646  */
    break;

  case 91:
#line 339 "grammar.y" /* yacc.c:1646  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2528 "grammar.c" /* yacc.c:1646  */
    break;

  case 92:
#line 340 "grammar.y" /* yacc.c:1646  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2534 "grammar.c" /* yacc.c:1646  */
    break;

  case 93:
#line 343 "grammar.y" /* yacc.c:1646  */
    { (yyval.field) = mk_field(NULL, (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2540 "grammar.c" /* yacc.c:1646  */
    break;

  case 94:
#line 344 "grammar.y" /* yacc.c:1646  */
    { (yyval.field) = mk_field(NULL, (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2546 "grammar.c" /* yacc.c:1646  */
    break;

  case 95:
#line 347 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2552 "grammar.c" /* yacc.c:1646  */
    break;

  case 96:
#line 348 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2558 "grammar.c" /* yacc.c:1646  */
    break;

  case 97:
#line 349 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2564 "grammar.c" /* yacc.c:1646  */
    break;

  case 98:
#line 352 "grammar.y" /* yacc.c:1646  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2570 "grammar.c" /* yacc.c:1646  */
    break;

  case 99:
#line 353 "grammar.y" /* yacc.c:1646  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2576 "grammar.c" /* yacc.c:1646  */
    break;

  case 100:
#line 356 "grammar.y" /* yacc.c:1646  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2582 "grammar.c" /* yacc.c:1646  */
    break;

  case 101:
#line 357 "grammar.y" /* yacc.c:1646  */
    { (yyval.field) = mk_field((yyvsp[-5].string), (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2588 "grammar.c" /* yacc.c:1646  */
    break;

  case 102:
#line 360 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2594 "grammar.c" /* yacc.c:1646  */
    break;

  case 103:
#line 361 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2600 "grammar.c" /* yacc.c:1646  */
    break;

  case 104:
#line 362 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2606 "grammar.c" /* yacc.c:1646  */
    break;

  case 105:
#line 365 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = NULL; }
#line 2612 "grammar.c" /* yacc.c:1646  */
    break;

  case 106:
#line 366 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = (yyvsp[-1].attribute_seq); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2618 "grammar.c" /* yacc.c:1646  */
    break;

  case 107:
#line 369 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = ndt_attr_seq_new((yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2624 "grammar.c" /* yacc.c:1646  */
    break;

  case 108:
#line 370 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute_seq) = ndt_attr_seq_append((yyvsp[-2].attribute_seq), (yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2630 "grammar.c" /* yacc.c:1646  */
    break;

  case 109:
#line 373 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute) = mk_attr((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2636 "grammar.c" /* yacc.c:1646  */
    break;

  case 110:
#line 374 "grammar.y" /* yacc.c:1646  */
    { (yyval.attribute) = mk_attr_from_seq((yyvsp[-4].string), (yyvsp[-1].string_seq), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2642 "grammar.c" /* yacc.c:1646  */
    break;

  case 111:
#line 377 "grammar.y" /* yacc.c:1646  */
    { (yyval.string_seq) = ndt_string_seq_new((yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2648 "grammar.c" /* yacc.c:1646  */
    break;

  case 112:
#line 378 "grammar.y" /* yacc.c:1646  */
    { (yyval.string_seq) = ndt_string_seq_append((yyvsp[-2].string_seq), (yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2654 "grammar.c" /* yacc.c:1646  */
    break;

  case 113:
#line 381 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2660 "grammar.c" /* yacc.c:1646  */
    break;

  case 114:
#line 382 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2666 "grammar.c" /* yacc.c:1646  */
    break;

  case 115:
#line 383 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2672 "grammar.c" /* yacc.c:1646  */
    break;

  case 116:
#line 384 "grammar.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2678 "grammar.c" /* yacc.c:1646  */
    break;

  case 117:
#line 388 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function_from_tuple((yyvsp[0].ndt), (yyvsp[-2].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2684 "grammar.c" /* yacc.c:1646  */
    break;

  case 118:
#line 390 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Nonvariadic, NULL, (yyvsp[-3].variadic_flag), (yyvsp[-4].field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2690 "grammar.c" /* yacc.c:1646  */
    break;

  case 119:
#line 392 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Variadic, NULL, (yyvsp[-3].variadic_flag), (yyvsp[-4].field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2696 "grammar.c" /* yacc.c:1646  */
    break;

  case 120:
#line 394 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Nonvariadic, (yyvsp[-6].field_seq), (yyvsp[-3].variadic_flag), (yyvsp[-4].field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2702 "grammar.c" /* yacc.c:1646  */
    break;

  case 121:
#line 396 "grammar.y" /* yacc.c:1646  */
    { (yyval.ndt) = mk_function((yyvsp[0].ndt), Variadic, (yyvsp[-8].field_seq), (yyvsp[-3].variadic_flag), (yyvsp[-4].field_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2708 "grammar.c" /* yacc.c:1646  */
    break;


#line 2712 "grammar.c" /* yacc.c:1646  */
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
      yyerror (&yylloc, scanner, ast, meta, ctx, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, ast, meta, ctx, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, ast, meta, ctx);
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
                  yystos[yystate], yyvsp, yylsp, scanner, ast, meta, ctx);
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
  yyerror (&yylloc, scanner, ast, meta, ctx, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, ast, meta, ctx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, scanner, ast, meta, ctx);
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
