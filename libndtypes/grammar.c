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
    OF = 297,
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
    LESS = 310,
    GREATER = 311,
    QUESTIONMARK = 312,
    BANG = 313,
    AMPERSAND = 314,
    BAR = 315,
    ERRTOKEN = 316,
    INTEGER = 317,
    FLOATNUMBER = 318,
    STRINGLIT = 319,
    NAME_LOWER = 320,
    NAME_UPPER = 321,
    NAME_OTHER = 322
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

#line 272 "grammar.c" /* yacc.c:352  */
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

#line 304 "grammar.c" /* yacc.c:352  */

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
#define YYFINAL  74
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   330

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  146
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  254

#define YYUNDEFTOK  2
#define YYMAXUTOK   322

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
      65,    66,    67
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   208,   208,   212,   213,   214,   218,   219,   220,   221,
     224,   225,   228,   229,   232,   233,   234,   237,   238,   239,
     240,   241,   245,   246,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   285,   286,   287,   288,
     291,   292,   293,   294,   297,   298,   299,   302,   303,   304,
     308,   309,   310,   313,   314,   317,   320,   321,   324,   327,
     328,   331,   332,   333,   334,   335,   338,   341,   344,   347,
     348,   351,   354,   355,   358,   359,   360,   361,   364,   365,
     368,   369,   370,   373,   374,   375,   376,   377,   378,   381,
     382,   385,   386,   389,   390,   391,   392,   393,   394,   397,
     398,   401,   402,   405,   406,   407,   410,   411,   414,   415,
     418,   421,   422,   425,   426,   429,   430,   433,   434,   437,
     438,   439,   442,   445,   446,   449,   450
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
  "FIXED", "VAR", "OF", "COMMA", "COLON", "LPAREN", "RPAREN", "LBRACE",
  "RBRACE", "LBRACK", "RBRACK", "STAR", "ELLIPSIS", "RARROW", "EQUAL",
  "LESS", "GREATER", "QUESTIONMARK", "BANG", "AMPERSAND", "BAR",
  "ERRTOKEN", "INTEGER", "FLOATNUMBER", "STRINGLIT", "NAME_LOWER",
  "NAME_UPPER", "NAME_OTHER", "$accept", "input", "datashape_or_module",
  "datashape_with_ellipsis", "fixed_ellipsis", "datashape", "dimensions",
  "dimensions_nooption", "dimensions_tail", "dtype", "scalar", "signed",
  "unsigned", "ieee_float", "ieee_complex", "alias", "character", "string",
  "fixed_string", "flags_opt", "option_opt", "endian_opt", "encoding",
  "bytes", "fixed_bytes", "ref", "categorical", "typed_value_seq",
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
     315,   316,   317,   318,   319,   320,   321,   322
};
# endif

#define YYPACT_NINF -206

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-206)))

#define YYTABLE_NINF -102

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     135,  -206,    16,    24,   -19,    27,   112,    45,   106,    -6,
      58,  -206,   -20,   117,   125,   139,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,   279,    51,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,   113,   131,    95,    95,   130,
     144,   151,  -206,   107,   142,   166,   182,  -206,  -206,  -206,
    -206,   183,   186,  -206,   188,   192,    -2,  -206,   218,   151,
     -19,    27,   112,  -206,   190,   195,   162,  -206,   218,   193,
     218,    61,   218,   189,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,   196,   197,  -206,
    -206,   203,   151,  -206,   205,   207,  -206,  -206,  -206,   218,
    -206,  -206,   158,   204,   200,     7,  -206,    34,    77,   218,
      -6,    95,  -206,   181,   209,  -206,   152,   212,   218,   218,
    -206,   112,  -206,  -206,  -206,   206,   222,   228,   224,   230,
      31,   218,  -206,   204,   235,   157,   232,   256,  -206,   218,
     -22,   210,   245,  -206,    95,   218,  -206,    63,  -206,  -206,
    -206,   215,    95,   260,  -206,   120,   267,  -206,  -206,   -24,
    -206,   200,  -206,    86,  -206,   200,  -206,   -25,  -206,   253,
    -206,  -206,   218,  -206,   181,   268,  -206,   152,   269,  -206,
     270,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
      98,  -206,  -206,   272,   103,   111,   273,   126,  -206,  -206,
    -206,  -206,  -206,   218,  -206,  -206,  -206,    95,  -206,   127,
    -206,    90,  -206,  -206,   -22,  -206,  -206,   210,  -206,  -206,
    -206,    32,  -206,  -206,   -23,  -206,  -206,  -206,   274,   126,
    -206,  -206,  -206,  -206
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      79,   144,     0,   131,    79,    98,     0,     0,    80,     0,
       0,    30,    34,     0,     0,   145,     7,     6,    12,    14,
      13,    26,    47,    48,    50,     0,    81,    51,    53,    55,
      54,    27,    28,    29,     4,     0,   143,     0,     0,     0,
       0,   131,    99,    34,   111,     0,   100,   109,   123,   124,
     125,     0,   100,   119,     0,     0,     0,   128,    79,   131,
      79,    98,     0,    31,     0,     0,     0,    16,    79,     0,
      79,     0,    79,     0,     1,     2,    35,    36,    56,    57,
      58,    59,    38,    60,    61,    62,    63,    40,    41,    64,
      65,    66,    43,    44,    67,    68,    69,    70,    71,    72,
      37,    39,    42,    45,    46,    24,    25,     0,    73,    75,
      49,     0,   131,    52,     0,     0,    82,    83,    84,    79,
      85,    78,    79,    79,     0,     0,   133,     0,    79,    79,
       0,     0,   103,    79,     0,   113,   101,     0,    79,    79,
     126,     0,    23,    10,    22,     0,     0,   100,     0,   100,
       0,    79,    17,    79,     0,     0,     0,     0,    19,    79,
       0,     0,     0,    87,     0,    79,    90,    34,   145,   142,
     146,     0,     0,     0,   132,    80,    34,     9,    20,     0,
     102,    30,   110,     0,   104,   123,   120,     0,   114,   121,
     130,   129,    79,   106,    79,     0,   116,   101,     0,   127,
       0,     5,    32,     8,    15,    11,    97,    94,    95,    96,
       0,    92,    86,     0,     0,     0,     0,     0,   139,   140,
     141,   135,   134,    79,   112,   105,   115,     0,    21,     0,
     107,     0,   117,    33,     0,    91,    74,     0,    76,    88,
      89,     0,   137,    18,     0,   108,   118,    93,     0,     0,
     136,   122,    77,   138
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -206,  -206,  -206,  -108,   244,    -3,    -9,  -206,   -64,   -55,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,    84,  -206,  -206,  -206,  -206,  -206,    88,    11,
     -50,  -206,   263,  -126,  -206,   264,  -127,     4,  -206,   262,
     185,   -48,   -33,   155,  -206,  -205,  -206,   208,  -206
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,   143,    20,
      21,   100,   101,   102,   103,   104,    22,    23,    24,    25,
      26,   121,   213,    27,    28,    29,    30,   210,   211,    45,
     134,    31,    46,    47,    32,    52,    53,    54,    33,    56,
      57,    40,   125,   126,   241,   221,    34,    35,    36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      67,    44,   137,   144,   152,   127,   206,   182,   158,   186,
      55,   145,   242,   144,   168,   170,    51,   144,   172,   172,
     172,     2,    41,   226,    69,    70,     4,   -98,     5,    71,
       6,    72,    73,    42,     2,    41,   224,   251,     8,     9,
     207,   208,   209,    10,   253,   201,    11,    43,   140,   142,
     172,    65,     9,   173,   105,   106,    10,    44,   141,   142,
      66,    37,   157,   142,   163,   178,    55,   154,   182,    38,
     186,   146,   148,   177,   144,   249,    39,   172,   107,    42,
     174,   199,   250,   108,   109,   110,   111,   112,   113,   114,
     115,   141,    48,    49,    50,   205,    58,   195,   179,   198,
     183,     2,    41,   187,   144,   116,   117,   118,    70,    68,
     119,   120,    71,     7,    72,    73,   166,    74,    65,     9,
     142,   157,     4,    10,     5,    75,     6,   155,   228,   172,
      44,   215,   225,   172,   175,   189,   190,   144,   246,    -3,
       1,   234,    11,   176,   235,    55,   237,    59,   200,   238,
     142,    60,    70,    61,   172,    62,   130,   239,    72,   243,
     124,   229,   216,     1,   231,    60,   122,    61,   144,    62,
     172,    63,    64,   245,   123,     2,     3,    48,    49,    50,
       4,   128,     5,   142,     6,    63,    64,     7,   218,   219,
     220,    44,     8,     9,   244,   129,    38,    10,     2,     3,
      11,    12,   131,     4,   180,     5,   130,     6,    72,    73,
       7,   130,   132,    72,   142,     8,     9,   185,    49,    50,
      10,     2,    41,    11,   167,   133,     4,  -101,     5,   136,
       6,   135,   138,   180,   139,   151,    59,   153,     8,     9,
     159,   160,   161,    10,     2,     3,   181,    43,   162,     4,
     164,     5,   165,     6,   171,   184,     7,   192,     2,    41,
     188,     8,     9,     4,   217,     5,    10,     6,   193,    11,
     167,   194,   196,   197,   212,     8,     9,   218,   219,   220,
      10,   202,   203,    11,    43,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,   204,   214,    97,    98,
      99,   223,    70,   227,   230,   156,   233,   232,   236,   240,
     252,   248,   247,   147,   150,   149,   191,   222,     0,     0,
     169
};

static const yytype_int16 yycheck[] =
{
       9,     4,    52,    58,    68,    38,    28,   133,    72,   136,
       6,    59,   217,    68,   122,   123,     5,    72,    43,    43,
      43,    40,    41,    48,    44,    45,    45,    46,    47,    49,
      49,    51,    52,    52,    40,    41,    60,    60,    57,    58,
      62,    63,    64,    62,   249,   153,    65,    66,    50,    58,
      43,    57,    58,    46,     3,     4,    62,    60,    60,    68,
      66,    45,    71,    72,   112,   129,    62,    70,   194,    45,
     197,    60,    61,   128,   129,    43,    52,    43,    27,    52,
      46,    50,    50,    32,    33,    34,    35,    36,    37,    38,
      39,    60,    65,    66,    67,   159,    51,   147,   131,   149,
     133,    40,    41,   136,   159,    54,    55,    56,    45,    51,
      59,    60,    49,    52,    51,    52,   119,     0,    57,    58,
     129,   130,    45,    62,    47,     0,    49,    66,   192,    43,
     133,   164,    46,    43,    57,   138,   139,   192,    48,     0,
       5,    43,    65,    66,    46,   141,    43,    41,   151,    46,
     159,    45,    45,    47,    43,    49,    49,    46,    51,   223,
      65,   194,   165,     5,   197,    45,    53,    47,   223,    49,
      43,    65,    66,    46,    43,    40,    41,    65,    66,    67,
      45,    51,    47,   192,    49,    65,    66,    52,    62,    63,
      64,   194,    57,    58,   227,    51,    45,    62,    40,    41,
      65,    66,    60,    45,    52,    47,    49,    49,    51,    52,
      52,    49,    46,    51,   223,    57,    58,    65,    66,    67,
      62,    40,    41,    65,    66,    43,    45,    46,    47,    43,
      49,    48,    44,    52,    42,    45,    41,    44,    57,    58,
      51,    45,    45,    62,    40,    41,    65,    66,    45,    45,
      45,    47,    45,    49,    54,    46,    52,    51,    40,    41,
      48,    57,    58,    45,    49,    47,    62,    49,    46,    65,
      66,    43,    48,    43,    64,    57,    58,    62,    63,    64,
      62,    46,    50,    65,    66,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    50,    62,    29,    30,
      31,    51,    45,    60,    46,    71,    46,    48,    46,    46,
      46,   237,   234,    60,    62,    61,   141,   172,    -1,    -1,
     122
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    40,    41,    45,    47,    49,    52,    57,    58,
      62,    65,    66,    69,    70,    71,    72,    73,    74,    75,
      77,    78,    84,    85,    86,    87,    88,    91,    92,    93,
      94,    99,   102,   106,   114,   115,   116,    45,    45,    52,
     109,    41,    52,    66,    73,    97,   100,   101,    65,    66,
      67,    97,   103,   104,   105,   105,   107,   108,    51,    41,
      45,    47,    49,    65,    66,    57,    66,    74,    51,    44,
      45,    49,    51,    52,     0,     0,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    29,    30,    31,
      79,    80,    81,    82,    83,     3,     4,    27,    32,    33,
      34,    35,    36,    37,    38,    39,    54,    55,    56,    59,
      60,    89,    53,    43,    65,   110,   111,   110,    51,    51,
      49,    60,    46,    43,    98,    48,    43,    98,    44,    42,
      50,    60,    74,    76,    77,   109,    97,   100,    97,   103,
     107,    45,    76,    44,    73,    66,    72,    74,    76,    51,
      45,    45,    45,   109,    45,    45,    73,    66,    71,   115,
      71,    54,    43,    46,    46,    57,    66,    77,    76,   110,
      52,    65,   101,   110,    46,    65,   104,   110,    48,    73,
      73,   108,    51,    46,    43,    98,    48,    43,    98,    50,
      73,    71,    46,    50,    50,    76,    28,    62,    63,    64,
      95,    96,    64,    90,    62,   110,    73,    49,    62,    63,
      64,   113,   111,    51,    60,    46,    48,    60,    76,   110,
      46,   110,    48,    46,    43,    46,    46,    43,    46,    46,
      46,   112,   113,    76,   110,    46,    48,    96,    90,    43,
      50,    60,    46,   113
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    70,    70,    70,    71,    71,    71,    71,
      72,    72,    73,    73,    74,    74,    74,    75,    75,    75,
      75,    75,    76,    76,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    81,    82,    82,    82,
      83,    83,    83,    84,    84,    85,    86,    86,    87,    88,
      88,    89,    89,    89,    89,    89,    90,    91,    92,    93,
      93,    94,    95,    95,    96,    96,    96,    96,    97,    97,
      98,    98,    98,    99,    99,    99,    99,    99,    99,   100,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   103,
     103,   104,   104,   105,   105,   105,   106,   106,   107,   107,
     108,   109,   109,   110,   110,   111,   111,   112,   112,   113,
     113,   113,   114,   115,   115,   116,   116
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
       1,     1,     1,     2,     5,     2,     5,     7,     2,     0,
       1,     0,     1,     1,     1,     1,     1,     3,     5,     5,
       3,     5,     1,     3,     1,     1,     1,     1,     0,     1,
       0,     1,     2,     3,     4,     5,     4,     5,     6,     1,
       3,     1,     4,     3,     4,     5,     4,     5,     6,     1,
       3,     3,     6,     1,     1,     1,     3,     4,     1,     3,
       3,     0,     3,     1,     3,     3,     5,     1,     3,     1,
       1,     1,     3,     1,     1,     1,     3
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
    case 62: /* INTEGER  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1418 "grammar.c" /* yacc.c:1257  */
        break;

    case 63: /* FLOATNUMBER  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1424 "grammar.c" /* yacc.c:1257  */
        break;

    case 64: /* STRINGLIT  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1430 "grammar.c" /* yacc.c:1257  */
        break;

    case 65: /* NAME_LOWER  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1436 "grammar.c" /* yacc.c:1257  */
        break;

    case 66: /* NAME_UPPER  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1442 "grammar.c" /* yacc.c:1257  */
        break;

    case 67: /* NAME_OTHER  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1448 "grammar.c" /* yacc.c:1257  */
        break;

    case 69: /* input  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1454 "grammar.c" /* yacc.c:1257  */
        break;

    case 70: /* datashape_or_module  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1460 "grammar.c" /* yacc.c:1257  */
        break;

    case 71: /* datashape_with_ellipsis  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1466 "grammar.c" /* yacc.c:1257  */
        break;

    case 72: /* fixed_ellipsis  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1472 "grammar.c" /* yacc.c:1257  */
        break;

    case 73: /* datashape  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1478 "grammar.c" /* yacc.c:1257  */
        break;

    case 74: /* dimensions  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1484 "grammar.c" /* yacc.c:1257  */
        break;

    case 75: /* dimensions_nooption  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1490 "grammar.c" /* yacc.c:1257  */
        break;

    case 76: /* dimensions_tail  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1496 "grammar.c" /* yacc.c:1257  */
        break;

    case 77: /* dtype  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1502 "grammar.c" /* yacc.c:1257  */
        break;

    case 78: /* scalar  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1508 "grammar.c" /* yacc.c:1257  */
        break;

    case 84: /* character  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1514 "grammar.c" /* yacc.c:1257  */
        break;

    case 85: /* string  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1520 "grammar.c" /* yacc.c:1257  */
        break;

    case 86: /* fixed_string  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1526 "grammar.c" /* yacc.c:1257  */
        break;

    case 91: /* bytes  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1532 "grammar.c" /* yacc.c:1257  */
        break;

    case 92: /* fixed_bytes  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1538 "grammar.c" /* yacc.c:1257  */
        break;

    case 93: /* ref  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1544 "grammar.c" /* yacc.c:1257  */
        break;

    case 94: /* categorical  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1550 "grammar.c" /* yacc.c:1257  */
        break;

    case 95: /* typed_value_seq  */
#line 198 "grammar.y" /* yacc.c:1257  */
      { ndt_value_seq_del(((*yyvaluep).typed_value_seq)); }
#line 1556 "grammar.c" /* yacc.c:1257  */
        break;

    case 96: /* typed_value  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_value_del(((*yyvaluep).typed_value)); }
#line 1562 "grammar.c" /* yacc.c:1257  */
        break;

    case 99: /* tuple_type  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1568 "grammar.c" /* yacc.c:1257  */
        break;

    case 100: /* tuple_field_seq  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1574 "grammar.c" /* yacc.c:1257  */
        break;

    case 101: /* tuple_field  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1580 "grammar.c" /* yacc.c:1257  */
        break;

    case 102: /* record_type  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1586 "grammar.c" /* yacc.c:1257  */
        break;

    case 103: /* record_field_seq  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1592 "grammar.c" /* yacc.c:1257  */
        break;

    case 104: /* record_field  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1598 "grammar.c" /* yacc.c:1257  */
        break;

    case 105: /* field_name_or_tag  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1604 "grammar.c" /* yacc.c:1257  */
        break;

    case 106: /* union_type  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1610 "grammar.c" /* yacc.c:1257  */
        break;

    case 107: /* union_member_seq  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1616 "grammar.c" /* yacc.c:1257  */
        break;

    case 108: /* union_member  */
#line 195 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1622 "grammar.c" /* yacc.c:1257  */
        break;

    case 109: /* arguments_opt  */
#line 200 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1628 "grammar.c" /* yacc.c:1257  */
        break;

    case 110: /* attribute_seq  */
#line 200 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1634 "grammar.c" /* yacc.c:1257  */
        break;

    case 111: /* attribute  */
#line 199 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_del(((*yyvaluep).attribute)); }
#line 1640 "grammar.c" /* yacc.c:1257  */
        break;

    case 112: /* untyped_value_seq  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_string_seq_del(((*yyvaluep).string_seq)); }
#line 1646 "grammar.c" /* yacc.c:1257  */
        break;

    case 113: /* untyped_value  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1652 "grammar.c" /* yacc.c:1257  */
        break;

    case 114: /* function_type  */
#line 194 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1658 "grammar.c" /* yacc.c:1257  */
        break;

    case 115: /* type_seq_or_void  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1664 "grammar.c" /* yacc.c:1257  */
        break;

    case 116: /* type_seq  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1670 "grammar.c" /* yacc.c:1257  */
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

#line 1785 "grammar.c" /* yacc.c:1431  */
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
#line 208 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[-1].ndt);  *ast = (yyval.ndt); YYACCEPT; }
#line 1979 "grammar.c" /* yacc.c:1652  */
    break;

  case 3:
#line 212 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1985 "grammar.c" /* yacc.c:1652  */
    break;

  case 4:
#line 213 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 1991 "grammar.c" /* yacc.c:1652  */
    break;

  case 5:
#line 214 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_module((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 1997 "grammar.c" /* yacc.c:1652  */
    break;

  case 6:
#line 218 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2003 "grammar.c" /* yacc.c:1652  */
    break;

  case 7:
#line 219 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2009 "grammar.c" /* yacc.c:1652  */
    break;

  case 8:
#line 220 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2015 "grammar.c" /* yacc.c:1652  */
    break;

  case 9:
#line 221 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_ellipsis((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2021 "grammar.c" /* yacc.c:1652  */
    break;

  case 10:
#line 224 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim(NULL, (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2027 "grammar.c" /* yacc.c:1652  */
    break;

  case 11:
#line 225 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2033 "grammar.c" /* yacc.c:1652  */
    break;

  case 12:
#line 228 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2039 "grammar.c" /* yacc.c:1652  */
    break;

  case 13:
#line 229 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2045 "grammar.c" /* yacc.c:1652  */
    break;

  case 14:
#line 232 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2051 "grammar.c" /* yacc.c:1652  */
    break;

  case 15:
#line 233 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2057 "grammar.c" /* yacc.c:1652  */
    break;

  case 16:
#line 234 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fortran((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2063 "grammar.c" /* yacc.c:1652  */
    break;

  case 17:
#line 237 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_shape((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2069 "grammar.c" /* yacc.c:1652  */
    break;

  case 18:
#line 238 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_attrs((yyvsp[-3].attribute_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2075 "grammar.c" /* yacc.c:1652  */
    break;

  case 19:
#line 239 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_symbolic_dim((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2081 "grammar.c" /* yacc.c:1652  */
    break;

  case 20:
#line 240 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2087 "grammar.c" /* yacc.c:1652  */
    break;

  case 21:
#line 241 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2093 "grammar.c" /* yacc.c:1652  */
    break;

  case 22:
#line 245 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2099 "grammar.c" /* yacc.c:1652  */
    break;

  case 23:
#line 246 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2105 "grammar.c" /* yacc.c:1652  */
    break;

  case 24:
#line 249 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_any_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2111 "grammar.c" /* yacc.c:1652  */
    break;

  case 25:
#line 250 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_scalar_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2117 "grammar.c" /* yacc.c:1652  */
    break;

  case 26:
#line 251 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2123 "grammar.c" /* yacc.c:1652  */
    break;

  case 27:
#line 252 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2129 "grammar.c" /* yacc.c:1652  */
    break;

  case 28:
#line 253 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2135 "grammar.c" /* yacc.c:1652  */
    break;

  case 29:
#line 254 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2141 "grammar.c" /* yacc.c:1652  */
    break;

  case 30:
#line 255 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2147 "grammar.c" /* yacc.c:1652  */
    break;

  case 31:
#line 256 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2153 "grammar.c" /* yacc.c:1652  */
    break;

  case 32:
#line 257 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2159 "grammar.c" /* yacc.c:1652  */
    break;

  case 33:
#line 258 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2165 "grammar.c" /* yacc.c:1652  */
    break;

  case 34:
#line 259 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_typevar((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2171 "grammar.c" /* yacc.c:1652  */
    break;

  case 35:
#line 262 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(Bool, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2177 "grammar.c" /* yacc.c:1652  */
    break;

  case 36:
#line 263 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_signed_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2183 "grammar.c" /* yacc.c:1652  */
    break;

  case 37:
#line 264 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2189 "grammar.c" /* yacc.c:1652  */
    break;

  case 38:
#line 265 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_unsigned_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2195 "grammar.c" /* yacc.c:1652  */
    break;

  case 39:
#line 266 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2201 "grammar.c" /* yacc.c:1652  */
    break;

  case 40:
#line 267 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_float_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2207 "grammar.c" /* yacc.c:1652  */
    break;

  case 41:
#line 268 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BFloat16, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2213 "grammar.c" /* yacc.c:1652  */
    break;

  case 42:
#line 269 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2219 "grammar.c" /* yacc.c:1652  */
    break;

  case 43:
#line 270 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_complex_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2225 "grammar.c" /* yacc.c:1652  */
    break;

  case 44:
#line 271 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BComplex32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2231 "grammar.c" /* yacc.c:1652  */
    break;

  case 45:
#line 272 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2237 "grammar.c" /* yacc.c:1652  */
    break;

  case 46:
#line 273 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_from_alias((yyvsp[0].alias), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2243 "grammar.c" /* yacc.c:1652  */
    break;

  case 47:
#line 274 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2249 "grammar.c" /* yacc.c:1652  */
    break;

  case 48:
#line 275 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2255 "grammar.c" /* yacc.c:1652  */
    break;

  case 49:
#line 276 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_string_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2261 "grammar.c" /* yacc.c:1652  */
    break;

  case 50:
#line 277 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2267 "grammar.c" /* yacc.c:1652  */
    break;

  case 51:
#line 278 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2273 "grammar.c" /* yacc.c:1652  */
    break;

  case 52:
#line 279 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_bytes_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2279 "grammar.c" /* yacc.c:1652  */
    break;

  case 53:
#line 280 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2285 "grammar.c" /* yacc.c:1652  */
    break;

  case 54:
#line 281 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2291 "grammar.c" /* yacc.c:1652  */
    break;

  case 55:
#line 282 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2297 "grammar.c" /* yacc.c:1652  */
    break;

  case 56:
#line 285 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int8; }
#line 2303 "grammar.c" /* yacc.c:1652  */
    break;

  case 57:
#line 286 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int16; }
#line 2309 "grammar.c" /* yacc.c:1652  */
    break;

  case 58:
#line 287 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int32; }
#line 2315 "grammar.c" /* yacc.c:1652  */
    break;

  case 59:
#line 288 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int64; }
#line 2321 "grammar.c" /* yacc.c:1652  */
    break;

  case 60:
#line 291 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint8; }
#line 2327 "grammar.c" /* yacc.c:1652  */
    break;

  case 61:
#line 292 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint16; }
#line 2333 "grammar.c" /* yacc.c:1652  */
    break;

  case 62:
#line 293 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint32; }
#line 2339 "grammar.c" /* yacc.c:1652  */
    break;

  case 63:
#line 294 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint64; }
#line 2345 "grammar.c" /* yacc.c:1652  */
    break;

  case 64:
#line 297 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float16; }
#line 2351 "grammar.c" /* yacc.c:1652  */
    break;

  case 65:
#line 298 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float32; }
#line 2357 "grammar.c" /* yacc.c:1652  */
    break;

  case 66:
#line 299 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float64; }
#line 2363 "grammar.c" /* yacc.c:1652  */
    break;

  case 67:
#line 302 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex32; }
#line 2369 "grammar.c" /* yacc.c:1652  */
    break;

  case 68:
#line 303 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex64; }
#line 2375 "grammar.c" /* yacc.c:1652  */
    break;

  case 69:
#line 304 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex128; }
#line 2381 "grammar.c" /* yacc.c:1652  */
    break;

  case 70:
#line 308 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Intptr; }
#line 2387 "grammar.c" /* yacc.c:1652  */
    break;

  case 71:
#line 309 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Uintptr; }
#line 2393 "grammar.c" /* yacc.c:1652  */
    break;

  case 72:
#line 310 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Size; }
#line 2399 "grammar.c" /* yacc.c:1652  */
    break;

  case 73:
#line 313 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char(Utf32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2405 "grammar.c" /* yacc.c:1652  */
    break;

  case 74:
#line 314 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char((yyvsp[-1].encoding), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2411 "grammar.c" /* yacc.c:1652  */
    break;

  case 75:
#line 317 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_string((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2417 "grammar.c" /* yacc.c:1652  */
    break;

  case 76:
#line 320 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-1].string), Utf8, (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2423 "grammar.c" /* yacc.c:1652  */
    break;

  case 77:
#line 321 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-3].string), (yyvsp[-1].encoding), (yyvsp[-6].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2429 "grammar.c" /* yacc.c:1652  */
    break;

  case 78:
#line 324 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = (yyvsp[-1].uint32) | (yyvsp[0].uint32); }
#line 2435 "grammar.c" /* yacc.c:1652  */
    break;

  case 79:
#line 327 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2441 "grammar.c" /* yacc.c:1652  */
    break;

  case 80:
#line 328 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_OPTION; }
#line 2447 "grammar.c" /* yacc.c:1652  */
    break;

  case 81:
#line 331 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2453 "grammar.c" /* yacc.c:1652  */
    break;

  case 82:
#line 332 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_SYS_BIG_ENDIAN ? NDT_BIG_ENDIAN : NDT_LITTLE_ENDIAN; }
#line 2459 "grammar.c" /* yacc.c:1652  */
    break;

  case 83:
#line 333 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_LITTLE_ENDIAN; }
#line 2465 "grammar.c" /* yacc.c:1652  */
    break;

  case 84:
#line 334 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_BIG_ENDIAN; }
#line 2471 "grammar.c" /* yacc.c:1652  */
    break;

  case 85:
#line 335 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2477 "grammar.c" /* yacc.c:1652  */
    break;

  case 86:
#line 338 "grammar.y" /* yacc.c:1652  */
    { (yyval.encoding) = encoding_from_string((yyvsp[0].string), ctx); if (ndt_err_occurred(ctx)) YYABORT; }
#line 2483 "grammar.c" /* yacc.c:1652  */
    break;

  case 87:
#line 341 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_bytes((yyvsp[0].attribute_seq), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2489 "grammar.c" /* yacc.c:1652  */
    break;

  case 88:
#line 344 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_bytes((yyvsp[-1].attribute_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2495 "grammar.c" /* yacc.c:1652  */
    break;

  case 89:
#line 347 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[-1].ndt), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2501 "grammar.c" /* yacc.c:1652  */
    break;

  case 90:
#line 348 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[0].ndt), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2507 "grammar.c" /* yacc.c:1652  */
    break;

  case 91:
#line 351 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_categorical((yyvsp[-1].typed_value_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2513 "grammar.c" /* yacc.c:1652  */
    break;

  case 92:
#line 354 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_new((yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2519 "grammar.c" /* yacc.c:1652  */
    break;

  case 93:
#line 355 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_append((yyvsp[-2].typed_value_seq), (yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2525 "grammar.c" /* yacc.c:1652  */
    break;

  case 94:
#line 358 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValInt64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2531 "grammar.c" /* yacc.c:1652  */
    break;

  case 95:
#line 359 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValFloat64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2537 "grammar.c" /* yacc.c:1652  */
    break;

  case 96:
#line 360 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_string((yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2543 "grammar.c" /* yacc.c:1652  */
    break;

  case 97:
#line 361 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_na(ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2549 "grammar.c" /* yacc.c:1652  */
    break;

  case 98:
#line 364 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2555 "grammar.c" /* yacc.c:1652  */
    break;

  case 99:
#line 365 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2561 "grammar.c" /* yacc.c:1652  */
    break;

  case 100:
#line 368 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2567 "grammar.c" /* yacc.c:1652  */
    break;

  case 101:
#line 369 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2573 "grammar.c" /* yacc.c:1652  */
    break;

  case 102:
#line 370 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2579 "grammar.c" /* yacc.c:1652  */
    break;

  case 103:
#line 373 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2585 "grammar.c" /* yacc.c:1652  */
    break;

  case 104:
#line 374 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2591 "grammar.c" /* yacc.c:1652  */
    break;

  case 105:
#line 375 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2597 "grammar.c" /* yacc.c:1652  */
    break;

  case 106:
#line 376 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2603 "grammar.c" /* yacc.c:1652  */
    break;

  case 107:
#line 377 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2609 "grammar.c" /* yacc.c:1652  */
    break;

  case 108:
#line 378 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2615 "grammar.c" /* yacc.c:1652  */
    break;

  case 109:
#line 381 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2621 "grammar.c" /* yacc.c:1652  */
    break;

  case 110:
#line 382 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2627 "grammar.c" /* yacc.c:1652  */
    break;

  case 111:
#line 385 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2633 "grammar.c" /* yacc.c:1652  */
    break;

  case 112:
#line 386 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2639 "grammar.c" /* yacc.c:1652  */
    break;

  case 113:
#line 389 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2645 "grammar.c" /* yacc.c:1652  */
    break;

  case 114:
#line 390 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2651 "grammar.c" /* yacc.c:1652  */
    break;

  case 115:
#line 391 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2657 "grammar.c" /* yacc.c:1652  */
    break;

  case 116:
#line 392 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2663 "grammar.c" /* yacc.c:1652  */
    break;

  case 117:
#line 393 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2669 "grammar.c" /* yacc.c:1652  */
    break;

  case 118:
#line 394 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2675 "grammar.c" /* yacc.c:1652  */
    break;

  case 119:
#line 397 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2681 "grammar.c" /* yacc.c:1652  */
    break;

  case 120:
#line 398 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2687 "grammar.c" /* yacc.c:1652  */
    break;

  case 121:
#line 401 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2693 "grammar.c" /* yacc.c:1652  */
    break;

  case 122:
#line 402 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-5].string), (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2699 "grammar.c" /* yacc.c:1652  */
    break;

  case 123:
#line 405 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2705 "grammar.c" /* yacc.c:1652  */
    break;

  case 124:
#line 406 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2711 "grammar.c" /* yacc.c:1652  */
    break;

  case 125:
#line 407 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2717 "grammar.c" /* yacc.c:1652  */
    break;

  case 126:
#line 410 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2723 "grammar.c" /* yacc.c:1652  */
    break;

  case 127:
#line 411 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2729 "grammar.c" /* yacc.c:1652  */
    break;

  case 128:
#line 414 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2735 "grammar.c" /* yacc.c:1652  */
    break;

  case 129:
#line 415 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2741 "grammar.c" /* yacc.c:1652  */
    break;

  case 130:
#line 418 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2747 "grammar.c" /* yacc.c:1652  */
    break;

  case 131:
#line 421 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = NULL; }
#line 2753 "grammar.c" /* yacc.c:1652  */
    break;

  case 132:
#line 422 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = (yyvsp[-1].attribute_seq); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2759 "grammar.c" /* yacc.c:1652  */
    break;

  case 133:
#line 425 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_new((yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2765 "grammar.c" /* yacc.c:1652  */
    break;

  case 134:
#line 426 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_append((yyvsp[-2].attribute_seq), (yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2771 "grammar.c" /* yacc.c:1652  */
    break;

  case 135:
#line 429 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2777 "grammar.c" /* yacc.c:1652  */
    break;

  case 136:
#line 430 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr_from_seq((yyvsp[-4].string), (yyvsp[-1].string_seq), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2783 "grammar.c" /* yacc.c:1652  */
    break;

  case 137:
#line 433 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_new((yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2789 "grammar.c" /* yacc.c:1652  */
    break;

  case 138:
#line 434 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_append((yyvsp[-2].string_seq), (yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2795 "grammar.c" /* yacc.c:1652  */
    break;

  case 139:
#line 437 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2801 "grammar.c" /* yacc.c:1652  */
    break;

  case 140:
#line 438 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2807 "grammar.c" /* yacc.c:1652  */
    break;

  case 141:
#line 439 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2813 "grammar.c" /* yacc.c:1652  */
    break;

  case 142:
#line 442 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_function((yyvsp[-2].type_seq), (yyvsp[0].type_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2819 "grammar.c" /* yacc.c:1652  */
    break;

  case 143:
#line 445 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = (yyvsp[0].type_seq); }
#line 2825 "grammar.c" /* yacc.c:1652  */
    break;

  case 144:
#line 446 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_empty(ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2831 "grammar.c" /* yacc.c:1652  */
    break;

  case 145:
#line 449 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_new((ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2837 "grammar.c" /* yacc.c:1652  */
    break;

  case 146:
#line 450 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_append((yyvsp[-2].type_seq), (ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2843 "grammar.c" /* yacc.c:1652  */
    break;


#line 2847 "grammar.c" /* yacc.c:1652  */
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
