/* A Bison parser, made by GNU Bison 3.3.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         ndt_yyparse
#define yylex           ndt_yylex
#define yyerror         ndt_yyerror
#define yydebug         ndt_yydebug
#define yynerrs         ndt_yynerrs


/* First part of user prologue.  */
#line 1 "grammar.y" /* yacc.c:337  */

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

#line 131 "grammar.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
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
#ifndef YY_NDT_YY_GRAMMAR_H_INCLUDED
# define YY_NDT_YY_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int ndt_yydebug;
#endif
/* "%code requires" blocks.  */
#line 56 "grammar.y" /* yacc.c:352  */

  #include "ndtypes.h"
  #include "seq.h"
  #include "attr.h"
  #include "parsefuncs.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;

#line 173 "grammar.c" /* yacc.c:352  */

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
    BFLOAT16 = 273,
    FLOAT16 = 274,
    FLOAT32 = 275,
    FLOAT64 = 276,
    COMPLEX_KIND = 277,
    BCOMPLEX32 = 278,
    COMPLEX32 = 279,
    COMPLEX64 = 280,
    COMPLEX128 = 281,
    CATEGORICAL = 282,
    NA = 283,
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
    REF = 294,
    FIXED = 295,
    VAR = 296,
    ARRAY = 297,
    OF = 298,
    COMMA = 299,
    COLON = 300,
    LPAREN = 301,
    RPAREN = 302,
    LBRACE = 303,
    RBRACE = 304,
    LBRACK = 305,
    RBRACK = 306,
    STAR = 307,
    ELLIPSIS = 308,
    RARROW = 309,
    EQUAL = 310,
    LESS = 311,
    GREATER = 312,
    QUESTIONMARK = 313,
    BANG = 314,
    AMPERSAND = 315,
    BAR = 316,
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

union YYSTYPE
{
#line 85 "grammar.y" /* yacc.c:352  */

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

#line 273 "grammar.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
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



int ndt_yyparse (yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx);
/* "%code provides" blocks.  */
#line 65 "grammar.y" /* yacc.c:352  */

  #define YY_DECL extern int ndt_yylexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int ndt_yylexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
  void yyerror(YYLTYPE *loc, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx, const char *msg);

#line 305 "grammar.c" /* yacc.c:352  */

#endif /* !YY_NDT_YY_GRAMMAR_H_INCLUDED  */



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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   357

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  148
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  258

#define YYUNDEFTOK  2
#define YYMAXUTOK   323

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   209,   209,   213,   214,   215,   219,   220,   221,   222,
     225,   226,   229,   230,   233,   234,   235,   238,   239,   240,
     241,   242,   246,   247,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   287,   288,   289,
     290,   293,   294,   295,   296,   299,   300,   301,   304,   305,
     306,   310,   311,   312,   315,   316,   319,   322,   323,   326,
     329,   330,   333,   334,   335,   336,   337,   340,   343,   346,
     349,   352,   353,   356,   359,   360,   363,   364,   365,   366,
     369,   370,   373,   374,   375,   378,   379,   380,   381,   382,
     383,   386,   387,   390,   391,   394,   395,   396,   397,   398,
     399,   402,   403,   406,   407,   410,   411,   412,   415,   416,
     419,   420,   423,   426,   427,   430,   431,   434,   435,   438,
     439,   442,   443,   444,   447,   450,   451,   454,   455
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
  "BFLOAT16", "FLOAT16", "FLOAT32", "FLOAT64", "COMPLEX_KIND",
  "BCOMPLEX32", "COMPLEX32", "COMPLEX64", "COMPLEX128", "CATEGORICAL",
  "NA", "INTPTR", "UINTPTR", "SIZE", "CHAR", "STRING", "FIXED_STRING_KIND",
  "FIXED_STRING", "BYTES", "FIXED_BYTES_KIND", "FIXED_BYTES", "REF",
  "FIXED", "VAR", "ARRAY", "OF", "COMMA", "COLON", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACK", "RBRACK", "STAR", "ELLIPSIS", "RARROW",
  "EQUAL", "LESS", "GREATER", "QUESTIONMARK", "BANG", "AMPERSAND", "BAR",
  "ERRTOKEN", "INTEGER", "FLOATNUMBER", "STRINGLIT", "NAME_LOWER",
  "NAME_UPPER", "NAME_OTHER", "$accept", "input", "datashape_or_module",
  "datashape_with_ellipsis", "fixed_ellipsis", "datashape", "dimensions",
  "dimensions_nooption", "dimensions_tail", "dtype", "scalar", "signed",
  "unsigned", "ieee_float", "ieee_complex", "alias", "character", "string",
  "fixed_string", "flags_opt", "option_opt", "endian_opt", "encoding",
  "bytes", "array", "fixed_bytes", "ref", "categorical", "typed_value_seq",
  "typed_value", "variadic_flag", "comma_variadic_flag", "tuple_type",
  "tuple_field_seq", "tuple_field", "record_type", "record_field_seq",
  "record_field", "field_name_or_tag", "union_type", "union_member_seq",
  "union_member", "arguments_opt", "attribute_seq", "attribute",
  "untyped_value_seq", "untyped_value", "function_type",
  "type_seq_or_void", "type_seq", YY_NULLPTR
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
     315,   316,   317,   318,   319,   320,   321,   322,   323
};
# endif

#define YYPACT_NINF -210

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-210)))

#define YYTABLE_NINF -104

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     139,  -210,     5,   -17,   -25,   -14,   116,    10,   239,    84,
      14,  -210,    31,    43,    78,    82,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,   301,    53,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,    39,    59,    45,    45,
      60,    64,    71,  -210,   124,    65,    80,    90,  -210,  -210,
    -210,  -210,    91,   117,  -210,   150,   119,   -21,  -210,   225,
      71,   -25,   -14,   116,  -210,   153,   155,   106,  -210,   225,
     156,   225,   114,   225,   158,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,   161,   162,
    -210,  -210,   166,    71,  -210,   168,   169,   174,  -210,  -210,
    -210,   225,  -210,  -210,   163,   211,   164,   -12,  -210,    89,
     287,   225,    84,    45,  -210,   187,   173,  -210,    62,   182,
     225,   225,  -210,   116,  -210,  -210,  -210,   180,   189,   194,
     190,   197,     4,   225,  -210,   211,   195,   -26,   192,   193,
    -210,   225,   -18,   183,   184,  -210,    45,   225,   225,  -210,
     107,  -210,  -210,  -210,    56,    45,   203,  -210,   290,   210,
    -210,  -210,   -24,  -210,   164,  -210,   101,  -210,   164,  -210,
      50,  -210,   188,  -210,  -210,   225,  -210,   187,   213,  -210,
      62,   209,  -210,   215,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,   102,  -210,  -210,   216,   131,   144,   220,
    -210,   160,  -210,  -210,  -210,  -210,  -210,   225,  -210,  -210,
    -210,    45,  -210,   146,  -210,    52,  -210,  -210,   -18,  -210,
    -210,   183,  -210,  -210,  -210,    28,  -210,  -210,   -13,  -210,
    -210,  -210,   221,   160,  -210,  -210,  -210,  -210
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      80,   146,     0,   133,    80,   100,     0,     0,    81,     0,
       0,    30,    34,     0,     0,   147,     7,     6,    12,    14,
      13,    26,    47,    48,    50,     0,    82,    51,    55,    53,
      56,    54,    27,    28,    29,     4,     0,   145,     0,     0,
       0,     0,   133,   101,    34,   113,     0,   102,   111,   125,
     126,   127,     0,   102,   121,     0,     0,     0,   130,    80,
     133,    80,   100,     0,    31,     0,     0,     0,    16,    80,
       0,    80,     0,    80,     0,     1,     2,    35,    36,    57,
      58,    59,    60,    38,    61,    62,    63,    64,    40,    41,
      65,    66,    67,    43,    44,    68,    69,    70,    71,    72,
      73,    37,    39,    42,    45,    46,    24,    25,     0,    74,
      76,    49,     0,   133,    52,     0,     0,     0,    83,    84,
      85,    80,    86,    79,    80,    80,     0,     0,   135,     0,
      80,    80,     0,     0,   105,    80,     0,   115,   103,     0,
      80,    80,   128,     0,    23,    10,    22,     0,     0,   102,
       0,   102,     0,    80,    17,    80,     0,     0,     0,     0,
      19,    80,     0,     0,     0,    88,     0,    80,    80,    92,
      34,   147,   144,   148,     0,     0,     0,   134,    81,    34,
       9,    20,     0,   104,    30,   112,     0,   106,   125,   122,
       0,   116,   123,   132,   131,    80,   108,    80,     0,   118,
     103,     0,   129,     0,     5,    32,     8,    15,    11,    99,
      96,    97,    98,     0,    94,    87,     0,     0,     0,     0,
      89,     0,   141,   142,   143,   137,   136,    80,   114,   107,
     117,     0,    21,     0,   109,     0,   119,    33,     0,    93,
      75,     0,    77,    90,    91,     0,   139,    18,     0,   110,
     120,    95,     0,     0,   138,   124,    78,   140
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -210,  -210,  -210,  -106,   200,    -3,    -9,  -210,   -64,   -56,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,    35,  -210,  -210,  -210,  -210,  -210,  -210,    41,
       9,   -51,  -210,   229,  -128,  -210,   219,  -127,    -2,  -210,
     223,   151,   -54,   -31,   118,  -210,  -209,  -210,   171,  -210
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   145,    20,
      21,   101,   102,   103,   104,   105,    22,    23,    24,    25,
      26,   123,   216,    27,    28,    29,    30,    31,   213,   214,
      46,   136,    32,    47,    48,    33,    53,    54,    55,    34,
      57,    58,    41,   127,   128,   245,   225,    35,    36,    37
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    45,   139,   146,    56,   154,   147,   185,   129,   160,
     209,   189,   246,   146,    52,     2,    42,   146,   171,   173,
     175,     4,  -100,     5,   132,     6,    73,    74,    43,    39,
     142,   175,   175,     8,     9,   176,    40,   228,    10,    43,
     143,    11,    44,    75,   257,   210,   211,   212,   255,   204,
     144,    38,    49,    50,    51,   202,   106,   107,    45,   165,
     144,    56,    59,   159,   144,   143,    69,   181,   156,   185,
     148,   150,   253,   189,   180,   146,    70,    71,    76,   254,
     108,    72,    -3,    73,    74,   109,   110,   111,   112,   113,
     114,   115,   116,   124,   175,   117,   175,   208,   198,   230,
     201,   250,   182,   125,   186,   146,   221,   190,   118,   119,
     120,   126,   130,   121,   122,   183,   131,    39,   169,   222,
     223,   224,   144,   159,     2,    42,   133,   134,   188,    50,
      51,   232,    45,   175,   135,   218,   177,   192,   193,   146,
     137,    56,    66,     9,     1,   175,   238,    10,   229,   239,
     203,    67,   144,    71,     2,    42,   132,    72,    73,    73,
      74,   138,   141,   247,   219,   220,   233,     7,     1,   235,
      71,   146,    66,     9,   132,   241,    73,    10,   242,     2,
       3,   157,    49,    50,    51,     4,   144,     5,   175,     6,
     175,   243,     7,   249,    45,   140,    60,     8,     9,   153,
     248,   155,    10,     2,     3,    11,    12,   162,   163,     4,
     161,     5,   164,     6,   166,   167,     7,   168,   144,   174,
     187,     8,     9,   222,   223,   224,    10,     2,    42,    11,
     170,   191,   195,     4,  -103,     5,   196,     6,   197,   199,
     183,   200,   205,   206,   207,     8,     9,   217,   215,   231,
      10,     2,     3,   184,    44,   227,    71,     4,   236,     5,
     234,     6,   237,   240,     7,     2,    42,   244,   256,     8,
       9,     4,   158,     5,    10,     6,   252,    11,   170,   251,
      60,   151,     0,     8,     9,    61,   152,    62,    10,    63,
     149,    11,    44,   226,   194,   172,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,     0,     0,
      98,    99,   100,     4,     0,     5,    61,     6,    62,     0,
      63,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,     0,    11,   179,     0,    64,    65
};

static const yytype_int16 yycheck[] =
{
       9,     4,    53,    59,     6,    69,    60,   135,    39,    73,
      28,   138,   221,    69,     5,    40,    41,    73,   124,   125,
      44,    46,    47,    48,    50,    50,    52,    53,    53,    46,
      51,    44,    44,    58,    59,    47,    53,    61,    63,    53,
      61,    66,    67,     0,   253,    63,    64,    65,    61,   155,
      59,    46,    66,    67,    68,    51,     3,     4,    61,   113,
      69,    63,    52,    72,    73,    61,    52,   131,    71,   197,
      61,    62,    44,   200,   130,   131,    45,    46,     0,    51,
      27,    50,     0,    52,    53,    32,    33,    34,    35,    36,
      37,    38,    39,    54,    44,    42,    44,   161,   149,    49,
     151,    49,   133,    44,   135,   161,    50,   138,    55,    56,
      57,    66,    52,    60,    61,    53,    52,    46,   121,    63,
      64,    65,   131,   132,    40,    41,    61,    47,    66,    67,
      68,   195,   135,    44,    44,   166,    47,   140,   141,   195,
      49,   143,    58,    59,     5,    44,    44,    63,    47,    47,
     153,    67,   161,    46,    40,    41,    50,    50,    52,    52,
      53,    44,    43,   227,   167,   168,   197,    53,     5,   200,
      46,   227,    58,    59,    50,    44,    52,    63,    47,    40,
      41,    67,    66,    67,    68,    46,   195,    48,    44,    50,
      44,    47,    53,    47,   197,    45,    41,    58,    59,    46,
     231,    45,    63,    40,    41,    66,    67,    46,    46,    46,
      52,    48,    46,    50,    46,    46,    53,    43,   227,    55,
      47,    58,    59,    63,    64,    65,    63,    40,    41,    66,
      67,    49,    52,    46,    47,    48,    47,    50,    44,    49,
      53,    44,    47,    51,    51,    58,    59,    63,    65,    61,
      63,    40,    41,    66,    67,    52,    46,    46,    49,    48,
      47,    50,    47,    47,    53,    40,    41,    47,    47,    58,
      59,    46,    72,    48,    63,    50,   241,    66,    67,   238,
      41,    62,    -1,    58,    59,    46,    63,    48,    63,    50,
      61,    66,    67,   175,   143,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    67,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    46,    -1,    48,    46,    50,    48,    -1,
      50,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    67,    -1,    66,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    40,    41,    46,    48,    50,    53,    58,    59,
      63,    66,    67,    70,    71,    72,    73,    74,    75,    76,
      78,    79,    85,    86,    87,    88,    89,    92,    93,    94,
      95,    96,   101,   104,   108,   116,   117,   118,    46,    46,
      53,   111,    41,    53,    67,    74,    99,   102,   103,    66,
      67,    68,    99,   105,   106,   107,   107,   109,   110,    52,
      41,    46,    48,    50,    66,    67,    58,    67,    75,    52,
      45,    46,    50,    52,    53,     0,     0,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    29,    30,
      31,    80,    81,    82,    83,    84,     3,     4,    27,    32,
      33,    34,    35,    36,    37,    38,    39,    42,    55,    56,
      57,    60,    61,    90,    54,    44,    66,   112,   113,   112,
      52,    52,    50,    61,    47,    44,   100,    49,    44,   100,
      45,    43,    51,    61,    75,    77,    78,   111,    99,   102,
      99,   105,   109,    46,    77,    45,    74,    67,    73,    75,
      77,    52,    46,    46,    46,   111,    46,    46,    43,    74,
      67,    72,   117,    72,    55,    44,    47,    47,    58,    67,
      78,    77,   112,    53,    66,   103,   112,    47,    66,   106,
     112,    49,    74,    74,   110,    52,    47,    44,   100,    49,
      44,   100,    51,    74,    72,    47,    51,    51,    77,    28,
      63,    64,    65,    97,    98,    65,    91,    63,   112,    74,
      74,    50,    63,    64,    65,   115,   113,    52,    61,    47,
      49,    61,    77,   112,    47,   112,    49,    47,    44,    47,
      47,    44,    47,    47,    47,   114,   115,    77,   112,    47,
      49,    98,    91,    44,    51,    61,    47,   115
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    69,    70,    71,    71,    71,    72,    72,    72,    72,
      73,    73,    74,    74,    75,    75,    75,    76,    76,    76,
      76,    76,    77,    77,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    80,    80,    80,
      80,    81,    81,    81,    81,    82,    82,    82,    83,    83,
      83,    84,    84,    84,    85,    85,    86,    87,    87,    88,
      89,    89,    90,    90,    90,    90,    90,    91,    92,    93,
      94,    95,    95,    96,    97,    97,    98,    98,    98,    98,
      99,    99,   100,   100,   100,   101,   101,   101,   101,   101,
     101,   102,   102,   103,   103,   104,   104,   104,   104,   104,
     104,   105,   105,   106,   106,   107,   107,   107,   108,   108,
     109,   109,   110,   111,   111,   112,   112,   113,   113,   114,
     114,   115,   115,   115,   116,   117,   117,   118,   118
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     4,     1,     1,     4,     4,
       3,     4,     1,     1,     1,     4,     2,     3,     6,     3,
       4,     5,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     2,     4,     5,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     1,     2,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     5,     2,     5,     7,     2,
       0,     1,     0,     1,     1,     1,     1,     1,     3,     4,
       5,     5,     3,     5,     1,     3,     1,     1,     1,     1,
       0,     1,     0,     1,     2,     3,     4,     5,     4,     5,
       6,     1,     3,     1,     4,     3,     4,     5,     4,     5,
       6,     1,     3,     3,     6,     1,     1,     1,     3,     4,
       1,     3,     3,     0,     3,     1,     3,     3,     5,     1,
       3,     1,     1,     1,     3,     1,     1,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (scanner);
  YYUSE (ast);
  YYUSE (ctx);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, scanner, ast, ctx);
  YYFPRINTF (yyo, ")");
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx)
{
  unsigned long yylno = yyrline[yyrule];
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx)
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
    case 63: /* INTEGER  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1423 "grammar.c" /* yacc.c:1257  */
        break;

    case 64: /* FLOATNUMBER  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1429 "grammar.c" /* yacc.c:1257  */
        break;

    case 65: /* STRINGLIT  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1435 "grammar.c" /* yacc.c:1257  */
        break;

    case 66: /* NAME_LOWER  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1441 "grammar.c" /* yacc.c:1257  */
        break;

    case 67: /* NAME_UPPER  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1447 "grammar.c" /* yacc.c:1257  */
        break;

    case 68: /* NAME_OTHER  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1453 "grammar.c" /* yacc.c:1257  */
        break;

    case 70: /* input  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1459 "grammar.c" /* yacc.c:1257  */
        break;

    case 71: /* datashape_or_module  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1465 "grammar.c" /* yacc.c:1257  */
        break;

    case 72: /* datashape_with_ellipsis  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1471 "grammar.c" /* yacc.c:1257  */
        break;

    case 73: /* fixed_ellipsis  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1477 "grammar.c" /* yacc.c:1257  */
        break;

    case 74: /* datashape  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1483 "grammar.c" /* yacc.c:1257  */
        break;

    case 75: /* dimensions  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1489 "grammar.c" /* yacc.c:1257  */
        break;

    case 76: /* dimensions_nooption  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1495 "grammar.c" /* yacc.c:1257  */
        break;

    case 77: /* dimensions_tail  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1501 "grammar.c" /* yacc.c:1257  */
        break;

    case 78: /* dtype  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1507 "grammar.c" /* yacc.c:1257  */
        break;

    case 79: /* scalar  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1513 "grammar.c" /* yacc.c:1257  */
        break;

    case 85: /* character  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1519 "grammar.c" /* yacc.c:1257  */
        break;

    case 86: /* string  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1525 "grammar.c" /* yacc.c:1257  */
        break;

    case 87: /* fixed_string  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1531 "grammar.c" /* yacc.c:1257  */
        break;

    case 92: /* bytes  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1537 "grammar.c" /* yacc.c:1257  */
        break;

    case 93: /* array  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1543 "grammar.c" /* yacc.c:1257  */
        break;

    case 94: /* fixed_bytes  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1549 "grammar.c" /* yacc.c:1257  */
        break;

    case 95: /* ref  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1555 "grammar.c" /* yacc.c:1257  */
        break;

    case 96: /* categorical  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1561 "grammar.c" /* yacc.c:1257  */
        break;

    case 97: /* typed_value_seq  */
#line 199 "grammar.y" /* yacc.c:1257  */
      { ndt_value_seq_del(((*yyvaluep).typed_value_seq)); }
#line 1567 "grammar.c" /* yacc.c:1257  */
        break;

    case 98: /* typed_value  */
#line 198 "grammar.y" /* yacc.c:1257  */
      { ndt_value_del(((*yyvaluep).typed_value)); }
#line 1573 "grammar.c" /* yacc.c:1257  */
        break;

    case 101: /* tuple_type  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1579 "grammar.c" /* yacc.c:1257  */
        break;

    case 102: /* tuple_field_seq  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1585 "grammar.c" /* yacc.c:1257  */
        break;

    case 103: /* tuple_field  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1591 "grammar.c" /* yacc.c:1257  */
        break;

    case 104: /* record_type  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1597 "grammar.c" /* yacc.c:1257  */
        break;

    case 105: /* record_field_seq  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1603 "grammar.c" /* yacc.c:1257  */
        break;

    case 106: /* record_field  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1609 "grammar.c" /* yacc.c:1257  */
        break;

    case 107: /* field_name_or_tag  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1615 "grammar.c" /* yacc.c:1257  */
        break;

    case 108: /* union_type  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1621 "grammar.c" /* yacc.c:1257  */
        break;

    case 109: /* union_member_seq  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1627 "grammar.c" /* yacc.c:1257  */
        break;

    case 110: /* union_member  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1633 "grammar.c" /* yacc.c:1257  */
        break;

    case 111: /* arguments_opt  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1639 "grammar.c" /* yacc.c:1257  */
        break;

    case 112: /* attribute_seq  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1645 "grammar.c" /* yacc.c:1257  */
        break;

    case 113: /* attribute  */
#line 200 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_del(((*yyvaluep).attribute)); }
#line 1651 "grammar.c" /* yacc.c:1257  */
        break;

    case 114: /* untyped_value_seq  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_string_seq_del(((*yyvaluep).string_seq)); }
#line 1657 "grammar.c" /* yacc.c:1257  */
        break;

    case 115: /* untyped_value  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1663 "grammar.c" /* yacc.c:1257  */
        break;

    case 116: /* function_type  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1669 "grammar.c" /* yacc.c:1257  */
        break;

    case 117: /* type_seq_or_void  */
#line 204 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1675 "grammar.c" /* yacc.c:1257  */
        break;

    case 118: /* type_seq  */
#line 204 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1681 "grammar.c" /* yacc.c:1257  */
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
yyparse (yyscan_t scanner, const ndt_t **ast, ndt_context_t *ctx)
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
#line 75 "grammar.y" /* yacc.c:1431  */
{
   yylloc.first_line = 1;
   yylloc.first_column = 1;
   yylloc.last_line = 1;
   yylloc.last_column = 1;
}

#line 1796 "grammar.c" /* yacc.c:1431  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 209 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[-1].ndt);  *ast = (yyval.ndt); YYACCEPT; }
#line 1990 "grammar.c" /* yacc.c:1652  */
    break;

  case 3:
#line 213 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1996 "grammar.c" /* yacc.c:1652  */
    break;

  case 4:
#line 214 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2002 "grammar.c" /* yacc.c:1652  */
    break;

  case 5:
#line 215 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_module((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2008 "grammar.c" /* yacc.c:1652  */
    break;

  case 6:
#line 219 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2014 "grammar.c" /* yacc.c:1652  */
    break;

  case 7:
#line 220 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2020 "grammar.c" /* yacc.c:1652  */
    break;

  case 8:
#line 221 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2026 "grammar.c" /* yacc.c:1652  */
    break;

  case 9:
#line 222 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_ellipsis((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2032 "grammar.c" /* yacc.c:1652  */
    break;

  case 10:
#line 225 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim(NULL, (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2038 "grammar.c" /* yacc.c:1652  */
    break;

  case 11:
#line 226 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2044 "grammar.c" /* yacc.c:1652  */
    break;

  case 12:
#line 229 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2050 "grammar.c" /* yacc.c:1652  */
    break;

  case 13:
#line 230 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2056 "grammar.c" /* yacc.c:1652  */
    break;

  case 14:
#line 233 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2062 "grammar.c" /* yacc.c:1652  */
    break;

  case 15:
#line 234 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2068 "grammar.c" /* yacc.c:1652  */
    break;

  case 16:
#line 235 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fortran((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2074 "grammar.c" /* yacc.c:1652  */
    break;

  case 17:
#line 238 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_shape((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2080 "grammar.c" /* yacc.c:1652  */
    break;

  case 18:
#line 239 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_attrs((yyvsp[-3].attribute_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2086 "grammar.c" /* yacc.c:1652  */
    break;

  case 19:
#line 240 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_symbolic_dim((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2092 "grammar.c" /* yacc.c:1652  */
    break;

  case 20:
#line 241 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2098 "grammar.c" /* yacc.c:1652  */
    break;

  case 21:
#line 242 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2104 "grammar.c" /* yacc.c:1652  */
    break;

  case 22:
#line 246 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2110 "grammar.c" /* yacc.c:1652  */
    break;

  case 23:
#line 247 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2116 "grammar.c" /* yacc.c:1652  */
    break;

  case 24:
#line 250 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_any_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2122 "grammar.c" /* yacc.c:1652  */
    break;

  case 25:
#line 251 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_scalar_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2128 "grammar.c" /* yacc.c:1652  */
    break;

  case 26:
#line 252 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2134 "grammar.c" /* yacc.c:1652  */
    break;

  case 27:
#line 253 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2140 "grammar.c" /* yacc.c:1652  */
    break;

  case 28:
#line 254 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2146 "grammar.c" /* yacc.c:1652  */
    break;

  case 29:
#line 255 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2152 "grammar.c" /* yacc.c:1652  */
    break;

  case 30:
#line 256 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2158 "grammar.c" /* yacc.c:1652  */
    break;

  case 31:
#line 257 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2164 "grammar.c" /* yacc.c:1652  */
    break;

  case 32:
#line 258 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2170 "grammar.c" /* yacc.c:1652  */
    break;

  case 33:
#line 259 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2176 "grammar.c" /* yacc.c:1652  */
    break;

  case 34:
#line 260 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_typevar((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2182 "grammar.c" /* yacc.c:1652  */
    break;

  case 35:
#line 263 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(Bool, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2188 "grammar.c" /* yacc.c:1652  */
    break;

  case 36:
#line 264 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_signed_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2194 "grammar.c" /* yacc.c:1652  */
    break;

  case 37:
#line 265 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2200 "grammar.c" /* yacc.c:1652  */
    break;

  case 38:
#line 266 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_unsigned_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2206 "grammar.c" /* yacc.c:1652  */
    break;

  case 39:
#line 267 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2212 "grammar.c" /* yacc.c:1652  */
    break;

  case 40:
#line 268 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_float_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2218 "grammar.c" /* yacc.c:1652  */
    break;

  case 41:
#line 269 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BFloat16, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2224 "grammar.c" /* yacc.c:1652  */
    break;

  case 42:
#line 270 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2230 "grammar.c" /* yacc.c:1652  */
    break;

  case 43:
#line 271 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_complex_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2236 "grammar.c" /* yacc.c:1652  */
    break;

  case 44:
#line 272 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BComplex32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2242 "grammar.c" /* yacc.c:1652  */
    break;

  case 45:
#line 273 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2248 "grammar.c" /* yacc.c:1652  */
    break;

  case 46:
#line 274 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_from_alias((yyvsp[0].alias), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2254 "grammar.c" /* yacc.c:1652  */
    break;

  case 47:
#line 275 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2260 "grammar.c" /* yacc.c:1652  */
    break;

  case 48:
#line 276 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2266 "grammar.c" /* yacc.c:1652  */
    break;

  case 49:
#line 277 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_string_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2272 "grammar.c" /* yacc.c:1652  */
    break;

  case 50:
#line 278 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2278 "grammar.c" /* yacc.c:1652  */
    break;

  case 51:
#line 279 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2284 "grammar.c" /* yacc.c:1652  */
    break;

  case 52:
#line 280 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_bytes_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2290 "grammar.c" /* yacc.c:1652  */
    break;

  case 53:
#line 281 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2296 "grammar.c" /* yacc.c:1652  */
    break;

  case 54:
#line 282 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2302 "grammar.c" /* yacc.c:1652  */
    break;

  case 55:
#line 283 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2308 "grammar.c" /* yacc.c:1652  */
    break;

  case 56:
#line 284 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2314 "grammar.c" /* yacc.c:1652  */
    break;

  case 57:
#line 287 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int8; }
#line 2320 "grammar.c" /* yacc.c:1652  */
    break;

  case 58:
#line 288 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int16; }
#line 2326 "grammar.c" /* yacc.c:1652  */
    break;

  case 59:
#line 289 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int32; }
#line 2332 "grammar.c" /* yacc.c:1652  */
    break;

  case 60:
#line 290 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int64; }
#line 2338 "grammar.c" /* yacc.c:1652  */
    break;

  case 61:
#line 293 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint8; }
#line 2344 "grammar.c" /* yacc.c:1652  */
    break;

  case 62:
#line 294 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint16; }
#line 2350 "grammar.c" /* yacc.c:1652  */
    break;

  case 63:
#line 295 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint32; }
#line 2356 "grammar.c" /* yacc.c:1652  */
    break;

  case 64:
#line 296 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint64; }
#line 2362 "grammar.c" /* yacc.c:1652  */
    break;

  case 65:
#line 299 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float16; }
#line 2368 "grammar.c" /* yacc.c:1652  */
    break;

  case 66:
#line 300 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float32; }
#line 2374 "grammar.c" /* yacc.c:1652  */
    break;

  case 67:
#line 301 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float64; }
#line 2380 "grammar.c" /* yacc.c:1652  */
    break;

  case 68:
#line 304 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex32; }
#line 2386 "grammar.c" /* yacc.c:1652  */
    break;

  case 69:
#line 305 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex64; }
#line 2392 "grammar.c" /* yacc.c:1652  */
    break;

  case 70:
#line 306 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex128; }
#line 2398 "grammar.c" /* yacc.c:1652  */
    break;

  case 71:
#line 310 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Intptr; }
#line 2404 "grammar.c" /* yacc.c:1652  */
    break;

  case 72:
#line 311 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Uintptr; }
#line 2410 "grammar.c" /* yacc.c:1652  */
    break;

  case 73:
#line 312 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Size; }
#line 2416 "grammar.c" /* yacc.c:1652  */
    break;

  case 74:
#line 315 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char(Utf32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2422 "grammar.c" /* yacc.c:1652  */
    break;

  case 75:
#line 316 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char((yyvsp[-1].encoding), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2428 "grammar.c" /* yacc.c:1652  */
    break;

  case 76:
#line 319 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_string((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2434 "grammar.c" /* yacc.c:1652  */
    break;

  case 77:
#line 322 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-1].string), Utf8, (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2440 "grammar.c" /* yacc.c:1652  */
    break;

  case 78:
#line 323 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-3].string), (yyvsp[-1].encoding), (yyvsp[-6].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2446 "grammar.c" /* yacc.c:1652  */
    break;

  case 79:
#line 326 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = (yyvsp[-1].uint32) | (yyvsp[0].uint32); }
#line 2452 "grammar.c" /* yacc.c:1652  */
    break;

  case 80:
#line 329 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2458 "grammar.c" /* yacc.c:1652  */
    break;

  case 81:
#line 330 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_OPTION; }
#line 2464 "grammar.c" /* yacc.c:1652  */
    break;

  case 82:
#line 333 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2470 "grammar.c" /* yacc.c:1652  */
    break;

  case 83:
#line 334 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_SYS_BIG_ENDIAN ? NDT_BIG_ENDIAN : NDT_LITTLE_ENDIAN; }
#line 2476 "grammar.c" /* yacc.c:1652  */
    break;

  case 84:
#line 335 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_LITTLE_ENDIAN; }
#line 2482 "grammar.c" /* yacc.c:1652  */
    break;

  case 85:
#line 336 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_BIG_ENDIAN; }
#line 2488 "grammar.c" /* yacc.c:1652  */
    break;

  case 86:
#line 337 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2494 "grammar.c" /* yacc.c:1652  */
    break;

  case 87:
#line 340 "grammar.y" /* yacc.c:1652  */
    { (yyval.encoding) = encoding_from_string((yyvsp[0].string), ctx); if (ndt_err_occurred(ctx)) YYABORT; }
#line 2500 "grammar.c" /* yacc.c:1652  */
    break;

  case 88:
#line 343 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_bytes((yyvsp[0].attribute_seq), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2506 "grammar.c" /* yacc.c:1652  */
    break;

  case 89:
#line 346 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_array((yyvsp[0].ndt), (yyvsp[-3].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2512 "grammar.c" /* yacc.c:1652  */
    break;

  case 90:
#line 349 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_bytes((yyvsp[-1].attribute_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2518 "grammar.c" /* yacc.c:1652  */
    break;

  case 91:
#line 352 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[-1].ndt), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2524 "grammar.c" /* yacc.c:1652  */
    break;

  case 92:
#line 353 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[0].ndt), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2530 "grammar.c" /* yacc.c:1652  */
    break;

  case 93:
#line 356 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_categorical((yyvsp[-1].typed_value_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2536 "grammar.c" /* yacc.c:1652  */
    break;

  case 94:
#line 359 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_new((yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2542 "grammar.c" /* yacc.c:1652  */
    break;

  case 95:
#line 360 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_append((yyvsp[-2].typed_value_seq), (yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2548 "grammar.c" /* yacc.c:1652  */
    break;

  case 96:
#line 363 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValInt64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2554 "grammar.c" /* yacc.c:1652  */
    break;

  case 97:
#line 364 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValFloat64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2560 "grammar.c" /* yacc.c:1652  */
    break;

  case 98:
#line 365 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_string((yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2566 "grammar.c" /* yacc.c:1652  */
    break;

  case 99:
#line 366 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_na(ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2572 "grammar.c" /* yacc.c:1652  */
    break;

  case 100:
#line 369 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2578 "grammar.c" /* yacc.c:1652  */
    break;

  case 101:
#line 370 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2584 "grammar.c" /* yacc.c:1652  */
    break;

  case 102:
#line 373 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2590 "grammar.c" /* yacc.c:1652  */
    break;

  case 103:
#line 374 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2596 "grammar.c" /* yacc.c:1652  */
    break;

  case 104:
#line 375 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2602 "grammar.c" /* yacc.c:1652  */
    break;

  case 105:
#line 378 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2608 "grammar.c" /* yacc.c:1652  */
    break;

  case 106:
#line 379 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2614 "grammar.c" /* yacc.c:1652  */
    break;

  case 107:
#line 380 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2620 "grammar.c" /* yacc.c:1652  */
    break;

  case 108:
#line 381 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2626 "grammar.c" /* yacc.c:1652  */
    break;

  case 109:
#line 382 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2632 "grammar.c" /* yacc.c:1652  */
    break;

  case 110:
#line 383 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2638 "grammar.c" /* yacc.c:1652  */
    break;

  case 111:
#line 386 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2644 "grammar.c" /* yacc.c:1652  */
    break;

  case 112:
#line 387 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2650 "grammar.c" /* yacc.c:1652  */
    break;

  case 113:
#line 390 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2656 "grammar.c" /* yacc.c:1652  */
    break;

  case 114:
#line 391 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2662 "grammar.c" /* yacc.c:1652  */
    break;

  case 115:
#line 394 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2668 "grammar.c" /* yacc.c:1652  */
    break;

  case 116:
#line 395 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2674 "grammar.c" /* yacc.c:1652  */
    break;

  case 117:
#line 396 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2680 "grammar.c" /* yacc.c:1652  */
    break;

  case 118:
#line 397 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2686 "grammar.c" /* yacc.c:1652  */
    break;

  case 119:
#line 398 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2692 "grammar.c" /* yacc.c:1652  */
    break;

  case 120:
#line 399 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2698 "grammar.c" /* yacc.c:1652  */
    break;

  case 121:
#line 402 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2704 "grammar.c" /* yacc.c:1652  */
    break;

  case 122:
#line 403 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2710 "grammar.c" /* yacc.c:1652  */
    break;

  case 123:
#line 406 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2716 "grammar.c" /* yacc.c:1652  */
    break;

  case 124:
#line 407 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-5].string), (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2722 "grammar.c" /* yacc.c:1652  */
    break;

  case 125:
#line 410 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2728 "grammar.c" /* yacc.c:1652  */
    break;

  case 126:
#line 411 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2734 "grammar.c" /* yacc.c:1652  */
    break;

  case 127:
#line 412 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2740 "grammar.c" /* yacc.c:1652  */
    break;

  case 128:
#line 415 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2746 "grammar.c" /* yacc.c:1652  */
    break;

  case 129:
#line 416 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2752 "grammar.c" /* yacc.c:1652  */
    break;

  case 130:
#line 419 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2758 "grammar.c" /* yacc.c:1652  */
    break;

  case 131:
#line 420 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2764 "grammar.c" /* yacc.c:1652  */
    break;

  case 132:
#line 423 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2770 "grammar.c" /* yacc.c:1652  */
    break;

  case 133:
#line 426 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = NULL; }
#line 2776 "grammar.c" /* yacc.c:1652  */
    break;

  case 134:
#line 427 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = (yyvsp[-1].attribute_seq); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2782 "grammar.c" /* yacc.c:1652  */
    break;

  case 135:
#line 430 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_new((yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2788 "grammar.c" /* yacc.c:1652  */
    break;

  case 136:
#line 431 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_append((yyvsp[-2].attribute_seq), (yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2794 "grammar.c" /* yacc.c:1652  */
    break;

  case 137:
#line 434 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2800 "grammar.c" /* yacc.c:1652  */
    break;

  case 138:
#line 435 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr_from_seq((yyvsp[-4].string), (yyvsp[-1].string_seq), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2806 "grammar.c" /* yacc.c:1652  */
    break;

  case 139:
#line 438 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_new((yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2812 "grammar.c" /* yacc.c:1652  */
    break;

  case 140:
#line 439 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_append((yyvsp[-2].string_seq), (yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2818 "grammar.c" /* yacc.c:1652  */
    break;

  case 141:
#line 442 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2824 "grammar.c" /* yacc.c:1652  */
    break;

  case 142:
#line 443 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2830 "grammar.c" /* yacc.c:1652  */
    break;

  case 143:
#line 444 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2836 "grammar.c" /* yacc.c:1652  */
    break;

  case 144:
#line 447 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_function((yyvsp[-2].type_seq), (yyvsp[0].type_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2842 "grammar.c" /* yacc.c:1652  */
    break;

  case 145:
#line 450 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = (yyvsp[0].type_seq); }
#line 2848 "grammar.c" /* yacc.c:1652  */
    break;

  case 146:
#line 451 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_empty(ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2854 "grammar.c" /* yacc.c:1652  */
    break;

  case 147:
#line 454 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_new((ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2860 "grammar.c" /* yacc.c:1652  */
    break;

  case 148:
#line 455 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_append((yyvsp[-2].type_seq), (ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2866 "grammar.c" /* yacc.c:1652  */
    break;


#line 2870 "grammar.c" /* yacc.c:1652  */
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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
