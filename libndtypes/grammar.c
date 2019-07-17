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
    NAME_OTHER = 323,
    BELOW_BAR = 324
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

#line 274 "grammar.c" /* yacc.c:352  */
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

#line 306 "grammar.c" /* yacc.c:352  */

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
#define YYFINAL  79
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   462

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  70
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  154
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  269

#define YYUNDEFTOK  2
#define YYMAXUTOK   324

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
      65,    66,    67,    68,    69
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   214,   214,   218,   219,   220,   224,   225,   226,   227,
     228,   231,   232,   235,   236,   239,   240,   241,   244,   245,
     246,   247,   248,   252,   253,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   292,   293,   294,
     295,   298,   299,   300,   301,   304,   305,   306,   309,   310,
     311,   315,   316,   317,   320,   321,   324,   327,   328,   331,
     334,   335,   338,   339,   340,   341,   342,   345,   348,   351,
     352,   353,   356,   357,   360,   363,   364,   367,   370,   371,
     374,   375,   376,   377,   380,   381,   384,   385,   386,   389,
     390,   391,   392,   393,   394,   397,   398,   401,   402,   405,
     406,   407,   408,   409,   410,   413,   414,   417,   418,   421,
     422,   423,   426,   427,   428,   429,   432,   433,   436,   439,
     440,   443,   444,   447,   448,   451,   452,   455,   456,   457,
     460,   463,   464,   467,   468
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
  "NAME_UPPER", "NAME_OTHER", "BELOW_BAR", "$accept", "input",
  "datashape_or_module", "datashape_with_ellipsis", "fixed_ellipsis",
  "datashape", "dimensions", "dimensions_nooption", "dimensions_tail",
  "dtype", "scalar", "signed", "unsigned", "ieee_float", "ieee_complex",
  "alias", "character", "string", "fixed_string", "flags_opt",
  "option_opt", "endian_opt", "encoding", "bytes", "array_dimensions",
  "array", "fixed_bytes", "ref", "categorical", "typed_value_seq",
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324
};
# endif

#define YYPACT_NINF -215

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-215)))

#define YYTABLE_NINF -131

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,  -215,   -20,     2,   272,   -31,   153,    -9,    -8,   116,
      17,     4,   392,  -215,    79,    96,   102,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,   397,   204,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,     8,  -215,    55,  -215,
    -215,    58,    77,    61,    61,    76,    86,    97,  -215,    38,
      81,   254,   103,   108,  -215,  -215,  -215,   114,   129,  -215,
     145,   -22,   334,    97,   272,   -31,   153,     4,   132,    55,
     148,    35,  -215,   334,   158,   334,    95,   334,   141,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
    -215,  -215,   151,   160,  -215,  -215,   164,    97,  -215,   165,
     172,   -12,  -215,  -215,  -215,   334,  -215,  -215,   334,   153,
     146,   320,   170,   -17,  -215,     5,   383,   334,   116,    61,
    -215,   296,   180,  -215,    27,   183,   334,  -215,  -215,  -215,
    -215,   177,   186,   190,   198,   191,     3,   334,  -215,   320,
     201,    72,   199,   200,  -215,   334,   -19,   184,   189,  -215,
      61,   334,   334,   210,  -215,  -215,  -215,   409,  -215,  -215,
    -215,   152,    61,   202,  -215,    99,    43,  -215,  -215,   -33,
    -215,   -23,  -215,    90,  -215,   170,  -215,    64,  -215,   194,
     334,  -215,   296,   209,  -215,    27,   213,  -215,   216,  -215,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,   124,  -215,
    -215,   219,   133,   138,   220,  -215,   241,  -215,   334,   159,
    -215,  -215,  -215,  -215,  -215,   334,  -215,  -215,  -215,    61,
    -215,   144,  -215,    71,  -215,  -215,   -19,  -215,  -215,   184,
    -215,  -215,  -215,   226,  -215,  -215,    19,  -215,  -215,   -27,
    -215,  -215,  -215,   223,   159,  -215,  -215,  -215,  -215
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      80,   152,     0,   139,    80,   104,     0,     0,    81,     0,
       0,    31,    35,   131,     0,     0,   153,     7,     6,    13,
      15,    14,    27,    48,    49,    51,     0,    82,    52,    10,
      89,    54,    56,    55,    28,    29,     0,    30,   132,   136,
       4,     0,   151,     0,     0,     0,     0,   139,   105,    35,
     117,    82,     0,   106,   115,   129,   130,     0,   106,   125,
       0,     0,    80,   139,    80,   104,     0,    32,   130,   134,
       0,     0,    17,    80,     0,    80,     0,    80,     0,     1,
       2,    36,    37,    57,    58,    59,    60,    39,    61,    62,
      63,    64,    41,    42,    65,    66,    67,    44,    45,    68,
      69,    70,    71,    72,    73,    38,    40,    43,    46,    47,
      25,    26,     0,    74,    76,    50,     0,   139,    53,     0,
       0,     0,    83,    84,    85,    80,    86,    79,    80,     0,
      80,    80,     0,     0,   141,     0,    80,    80,     0,     0,
     109,    80,     0,   119,   107,     0,    80,   133,    24,    11,
      23,     0,     0,   106,     0,   106,     0,    80,    18,    80,
       0,     0,     0,     0,    20,    80,     0,     0,     0,    88,
       0,    80,    80,     0,    96,   138,   137,    35,   153,   150,
     154,     0,     0,     0,   140,    81,    35,     9,    21,     0,
     108,    31,   116,     0,   110,   129,   126,     0,   120,   127,
      80,   112,    80,     0,   122,   107,     0,   135,     0,     5,
      33,     8,    16,    12,   103,   100,   101,   102,     0,    98,
      87,     0,     0,     0,     0,    92,    82,    93,    80,     0,
     147,   148,   149,   143,   142,    80,   118,   111,   121,     0,
      22,     0,   113,     0,   123,    34,     0,    97,    75,     0,
      77,    94,    95,     0,    90,    91,     0,   145,    19,     0,
     114,   124,    99,     0,     0,   144,   128,    78,   146
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -215,  -215,  -215,  -106,   195,    -2,    -5,  -215,   -59,   -54,
    -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,  -215,
       0,  -215,    23,  -215,  -215,  -151,  -215,  -215,  -215,  -215,
      36,    11,   -48,  -215,   221,  -135,  -215,   229,  -131,    -4,
    -215,    -1,   155,   -51,   -41,   113,  -215,  -214,  -215,   169,
    -215
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    14,    15,    16,    17,    18,    19,    20,   149,    21,
      22,   105,   106,   107,   108,   109,    23,    24,    25,    26,
      51,   127,   221,    28,    29,    30,    31,    32,    33,   218,
     219,    52,   142,    34,    53,    54,    35,    58,    59,    36,
      37,    38,    39,    46,   133,   134,   256,   233,    40,    41,
      42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      27,    60,    50,   135,    72,    61,   192,    69,   150,   214,
     145,   182,   151,   196,   158,   257,    57,   182,   164,   150,
    -129,   227,    48,   150,   178,   180,    43,   182,   236,   147,
     183,   172,   181,    63,   266,    55,    56,    13,    64,   129,
      65,   173,    66,    62,   215,   216,   217,  -129,    44,   182,
     268,   128,   184,   209,   207,    45,     1,   148,    67,    68,
      13,    60,    50,   264,   129,   156,   169,   192,   148,    73,
     265,   163,   148,   160,   196,   152,   154,   255,   188,    79,
     190,  -130,   187,   150,    75,   138,  -130,    77,   138,    75,
      77,     2,     3,   195,    56,    13,    80,     4,   189,     5,
     193,     6,    -3,   197,     7,   203,   213,   206,   182,     8,
       9,   150,   130,   238,    10,   182,   129,    11,    12,    13,
     261,   131,   138,   174,    77,    78,   175,   132,   136,   223,
      27,    27,   148,   163,   182,     2,    47,   237,   137,    50,
      60,   240,   139,    44,   199,    64,   150,    65,     7,    66,
     140,     1,   141,    70,     9,   208,     2,    47,    10,    27,
     148,   241,   161,   143,   243,    67,    68,    13,   246,   224,
     225,   247,   226,   144,    70,     9,   258,   249,   157,    10,
     250,   150,   182,    71,    69,   251,     2,     3,   182,    63,
     146,   260,     4,   165,     5,   148,     6,   166,   259,     7,
      50,    60,   229,   159,     8,     9,   167,   110,   111,    10,
     168,   170,    11,   177,    13,   230,   231,   232,   171,    55,
      56,    13,   230,   231,   232,   181,   254,   194,   226,   200,
     148,   112,   198,   201,   202,   205,   113,   114,   115,   116,
     117,   118,   119,   120,   110,   111,   121,   204,   210,   220,
     211,   212,   222,   228,   235,   239,   242,   110,   111,   122,
     123,   124,   244,   245,   125,   126,   248,   252,   112,   172,
     267,   162,   263,   113,   114,   115,   116,   117,   118,   119,
     120,   112,   262,   253,   176,   153,   113,   114,   115,   116,
     117,   118,   119,   120,   155,   234,   122,   123,   124,   179,
       0,   125,   126,     0,     0,     0,     0,     0,     0,   122,
     123,   124,     2,    47,   125,   126,     0,     0,     4,  -104,
       5,     0,     6,     0,     0,    48,     0,     0,     0,     0,
       8,     9,     0,     0,     0,    10,     2,    47,    11,    49,
      13,     0,     4,  -107,     5,     0,     6,     0,     0,   190,
       0,     0,     0,     0,     8,     9,     0,     0,     0,    10,
       2,     3,   191,    49,    13,     0,     4,     0,     5,     0,
       6,     0,     0,     7,     2,    47,     0,     0,     8,     9,
       4,     0,     5,    10,     6,     0,    11,   177,    13,     0,
       0,     0,     8,     9,     0,     0,     0,    10,     0,     0,
      11,    49,    13,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,     0,     0,   102,   103,   104,     4,
       0,     5,     0,     6,     0,  -130,     0,    74,    75,     0,
       0,   185,    76,     0,    77,    78,     0,     0,     0,    11,
     186,    13,  -130,     0,     0,    75,     0,     0,     0,    76,
       0,    77,    78
};

static const yytype_int16 yycheck[] =
{
       0,     5,     4,    44,     9,     6,   141,     8,    62,    28,
      58,    44,    63,   144,    73,   229,     5,    44,    77,    73,
      43,   172,    53,    77,   130,   131,    46,    44,    61,    51,
      47,    43,    55,    41,    61,    66,    67,    68,    46,    61,
      48,    53,    50,    52,    63,    64,    65,    43,    46,    44,
     264,    43,    47,   159,    51,    53,     5,    62,    66,    67,
      68,    65,    64,    44,    61,    66,   117,   202,    73,    52,
      51,    76,    77,    75,   205,    64,    65,   228,   137,     0,
      53,    43,   136,   137,    46,    50,    43,    52,    50,    46,
      52,    40,    41,    66,    67,    68,     0,    46,   139,    48,
     141,    50,     0,   144,    53,   153,   165,   155,    44,    58,
      59,   165,    54,    49,    63,    44,    61,    66,    67,    68,
      49,    44,    50,   125,    52,    53,   128,    66,    52,   170,
     130,   131,   137,   138,    44,    40,    41,    47,    52,   141,
     144,   200,    61,    46,   146,    46,   200,    48,    53,    50,
      47,     5,    44,    58,    59,   157,    40,    41,    63,   159,
     165,   202,    67,    49,   205,    66,    67,    68,    44,   171,
     172,    47,   172,    44,    58,    59,   235,    44,    46,    63,
      47,   235,    44,    67,   185,    47,    40,    41,    44,    41,
      45,    47,    46,    52,    48,   200,    50,    46,   239,    53,
     202,   205,    50,    45,    58,    59,    46,     3,     4,    63,
      46,    46,    66,    67,    68,    63,    64,    65,    46,    66,
      67,    68,    63,    64,    65,    55,   228,    47,   228,    52,
     235,    27,    49,    47,    44,    44,    32,    33,    34,    35,
      36,    37,    38,    39,     3,     4,    42,    49,    47,    65,
      51,    51,    63,    43,    52,    61,    47,     3,     4,    55,
      56,    57,    49,    47,    60,    61,    47,    47,    27,    43,
      47,    76,   249,    32,    33,    34,    35,    36,    37,    38,
      39,    27,   246,    42,   129,    64,    32,    33,    34,    35,
      36,    37,    38,    39,    65,   182,    55,    56,    57,   130,
      -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    40,    41,    60,    61,    -1,    -1,    46,    47,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    63,    40,    41,    66,    67,
      68,    -1,    46,    47,    48,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    63,
      40,    41,    66,    67,    68,    -1,    46,    -1,    48,    -1,
      50,    -1,    -1,    53,    40,    41,    -1,    -1,    58,    59,
      46,    -1,    48,    63,    50,    -1,    66,    67,    68,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    63,    -1,    -1,
      66,    67,    68,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    46,
      -1,    48,    -1,    50,    -1,    43,    -1,    45,    46,    -1,
      -1,    58,    50,    -1,    52,    53,    -1,    -1,    -1,    66,
      67,    68,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    52,    53
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    40,    41,    46,    48,    50,    53,    58,    59,
      63,    66,    67,    68,    71,    72,    73,    74,    75,    76,
      77,    79,    80,    86,    87,    88,    89,    90,    93,    94,
      95,    96,    97,    98,   103,   106,   109,   110,   111,   112,
     118,   119,   120,    46,    46,    53,   113,    41,    53,    67,
      75,    90,   101,   104,   105,    66,    67,   101,   107,   108,
     109,   111,    52,    41,    46,    48,    50,    66,    67,   111,
      58,    67,    76,    52,    45,    46,    50,    52,    53,     0,
       0,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    29,    30,    31,    81,    82,    83,    84,    85,
       3,     4,    27,    32,    33,    34,    35,    36,    37,    38,
      39,    42,    55,    56,    57,    60,    61,    91,    43,    61,
      54,    44,    66,   114,   115,   114,    52,    52,    50,    61,
      47,    44,   102,    49,    44,   102,    45,    51,    76,    78,
      79,   113,   101,   104,   101,   107,   111,    46,    78,    45,
      75,    67,    74,    76,    78,    52,    46,    46,    46,   113,
      46,    46,    43,    53,    75,    75,   112,    67,    73,   119,
      73,    55,    44,    47,    47,    58,    67,    79,    78,   114,
      53,    66,   105,   114,    47,    66,   108,   114,    49,    75,
      52,    47,    44,   102,    49,    44,   102,    51,    75,    73,
      47,    51,    51,    78,    28,    63,    64,    65,    99,   100,
      65,    92,    63,   114,    75,    75,    90,    95,    43,    50,
      63,    64,    65,   117,   115,    52,    61,    47,    49,    61,
      78,   114,    47,   114,    49,    47,    44,    47,    47,    44,
      47,    47,    47,    42,    75,    95,   116,   117,    78,   114,
      47,    49,   100,    92,    44,    51,    61,    47,   117
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    70,    71,    72,    72,    72,    73,    73,    73,    73,
      73,    74,    74,    75,    75,    76,    76,    76,    77,    77,
      77,    77,    77,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    81,    81,    81,
      81,    82,    82,    82,    82,    83,    83,    83,    84,    84,
      84,    85,    85,    85,    86,    86,    87,    88,    88,    89,
      90,    90,    91,    91,    91,    91,    91,    92,    93,    94,
      94,    94,    95,    95,    96,    97,    97,    98,    99,    99,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   103,
     103,   103,   103,   103,   103,   104,   104,   105,   105,   106,
     106,   106,   106,   106,   106,   107,   107,   108,   108,   109,
     109,   109,   110,   110,   110,   110,   111,   111,   112,   113,
     113,   114,   114,   115,   115,   116,   116,   117,   117,   117,
     118,   119,   119,   120,   120
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     4,     1,     1,     4,     4,
       1,     3,     4,     1,     1,     1,     4,     2,     3,     6,
       3,     4,     5,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     2,     4,     5,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     5,     2,     5,     7,     2,
       0,     1,     0,     1,     1,     1,     1,     1,     3,     1,
       5,     5,     4,     4,     5,     5,     3,     5,     1,     3,
       1,     1,     1,     1,     0,     1,     0,     1,     2,     3,
       4,     5,     4,     5,     6,     1,     3,     1,     4,     3,
       4,     5,     4,     5,     6,     1,     3,     3,     6,     1,
       1,     1,     1,     3,     2,     4,     1,     3,     3,     0,
       3,     1,     3,     3,     5,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3
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
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1455 "grammar.c" /* yacc.c:1257  */
        break;

    case 64: /* FLOATNUMBER  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1461 "grammar.c" /* yacc.c:1257  */
        break;

    case 65: /* STRINGLIT  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1467 "grammar.c" /* yacc.c:1257  */
        break;

    case 66: /* NAME_LOWER  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1473 "grammar.c" /* yacc.c:1257  */
        break;

    case 67: /* NAME_UPPER  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1479 "grammar.c" /* yacc.c:1257  */
        break;

    case 68: /* NAME_OTHER  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1485 "grammar.c" /* yacc.c:1257  */
        break;

    case 71: /* input  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1491 "grammar.c" /* yacc.c:1257  */
        break;

    case 72: /* datashape_or_module  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1497 "grammar.c" /* yacc.c:1257  */
        break;

    case 73: /* datashape_with_ellipsis  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1503 "grammar.c" /* yacc.c:1257  */
        break;

    case 74: /* fixed_ellipsis  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1509 "grammar.c" /* yacc.c:1257  */
        break;

    case 75: /* datashape  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1515 "grammar.c" /* yacc.c:1257  */
        break;

    case 76: /* dimensions  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1521 "grammar.c" /* yacc.c:1257  */
        break;

    case 77: /* dimensions_nooption  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1527 "grammar.c" /* yacc.c:1257  */
        break;

    case 78: /* dimensions_tail  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1533 "grammar.c" /* yacc.c:1257  */
        break;

    case 79: /* dtype  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1539 "grammar.c" /* yacc.c:1257  */
        break;

    case 80: /* scalar  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1545 "grammar.c" /* yacc.c:1257  */
        break;

    case 86: /* character  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1551 "grammar.c" /* yacc.c:1257  */
        break;

    case 87: /* string  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1557 "grammar.c" /* yacc.c:1257  */
        break;

    case 88: /* fixed_string  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1563 "grammar.c" /* yacc.c:1257  */
        break;

    case 93: /* bytes  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1569 "grammar.c" /* yacc.c:1257  */
        break;

    case 94: /* array_dimensions  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1575 "grammar.c" /* yacc.c:1257  */
        break;

    case 95: /* array  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1581 "grammar.c" /* yacc.c:1257  */
        break;

    case 96: /* fixed_bytes  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1587 "grammar.c" /* yacc.c:1257  */
        break;

    case 97: /* ref  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1593 "grammar.c" /* yacc.c:1257  */
        break;

    case 98: /* categorical  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1599 "grammar.c" /* yacc.c:1257  */
        break;

    case 99: /* typed_value_seq  */
#line 200 "grammar.y" /* yacc.c:1257  */
      { ndt_value_seq_del(((*yyvaluep).typed_value_seq)); }
#line 1605 "grammar.c" /* yacc.c:1257  */
        break;

    case 100: /* typed_value  */
#line 199 "grammar.y" /* yacc.c:1257  */
      { ndt_value_del(((*yyvaluep).typed_value)); }
#line 1611 "grammar.c" /* yacc.c:1257  */
        break;

    case 103: /* tuple_type  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1617 "grammar.c" /* yacc.c:1257  */
        break;

    case 104: /* tuple_field_seq  */
#line 198 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1623 "grammar.c" /* yacc.c:1257  */
        break;

    case 105: /* tuple_field  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1629 "grammar.c" /* yacc.c:1257  */
        break;

    case 106: /* record_type  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1635 "grammar.c" /* yacc.c:1257  */
        break;

    case 107: /* record_field_seq  */
#line 198 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1641 "grammar.c" /* yacc.c:1257  */
        break;

    case 108: /* record_field  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1647 "grammar.c" /* yacc.c:1257  */
        break;

    case 109: /* field_name_or_tag  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1653 "grammar.c" /* yacc.c:1257  */
        break;

    case 110: /* union_type  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1659 "grammar.c" /* yacc.c:1257  */
        break;

    case 111: /* union_member_seq  */
#line 198 "grammar.y" /* yacc.c:1257  */
      { ndt_field_seq_del(((*yyvaluep).field_seq)); }
#line 1665 "grammar.c" /* yacc.c:1257  */
        break;

    case 112: /* union_member  */
#line 197 "grammar.y" /* yacc.c:1257  */
      { ndt_field_del(((*yyvaluep).field)); }
#line 1671 "grammar.c" /* yacc.c:1257  */
        break;

    case 113: /* arguments_opt  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1677 "grammar.c" /* yacc.c:1257  */
        break;

    case 114: /* attribute_seq  */
#line 202 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_seq_del(((*yyvaluep).attribute_seq)); }
#line 1683 "grammar.c" /* yacc.c:1257  */
        break;

    case 115: /* attribute  */
#line 201 "grammar.y" /* yacc.c:1257  */
      { ndt_attr_del(((*yyvaluep).attribute)); }
#line 1689 "grammar.c" /* yacc.c:1257  */
        break;

    case 116: /* untyped_value_seq  */
#line 204 "grammar.y" /* yacc.c:1257  */
      { ndt_string_seq_del(((*yyvaluep).string_seq)); }
#line 1695 "grammar.c" /* yacc.c:1257  */
        break;

    case 117: /* untyped_value  */
#line 203 "grammar.y" /* yacc.c:1257  */
      { ndt_free(((*yyvaluep).string)); }
#line 1701 "grammar.c" /* yacc.c:1257  */
        break;

    case 118: /* function_type  */
#line 196 "grammar.y" /* yacc.c:1257  */
      { ndt_decref(((*yyvaluep).ndt)); }
#line 1707 "grammar.c" /* yacc.c:1257  */
        break;

    case 119: /* type_seq_or_void  */
#line 205 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1713 "grammar.c" /* yacc.c:1257  */
        break;

    case 120: /* type_seq  */
#line 205 "grammar.y" /* yacc.c:1257  */
      { ndt_type_seq_del(((*yyvaluep).type_seq)); }
#line 1719 "grammar.c" /* yacc.c:1257  */
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

#line 1834 "grammar.c" /* yacc.c:1431  */
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
#line 214 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[-1].ndt);  *ast = (yyval.ndt); YYACCEPT; }
#line 2028 "grammar.c" /* yacc.c:1652  */
    break;

  case 3:
#line 218 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2034 "grammar.c" /* yacc.c:1652  */
    break;

  case 4:
#line 219 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2040 "grammar.c" /* yacc.c:1652  */
    break;

  case 5:
#line 220 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_module((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2046 "grammar.c" /* yacc.c:1652  */
    break;

  case 6:
#line 224 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2052 "grammar.c" /* yacc.c:1652  */
    break;

  case 7:
#line 225 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2058 "grammar.c" /* yacc.c:1652  */
    break;

  case 8:
#line 226 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2064 "grammar.c" /* yacc.c:1652  */
    break;

  case 9:
#line 227 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_ellipsis((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2070 "grammar.c" /* yacc.c:1652  */
    break;

  case 10:
#line 228 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2076 "grammar.c" /* yacc.c:1652  */
    break;

  case 11:
#line 231 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim(NULL, (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2082 "grammar.c" /* yacc.c:1652  */
    break;

  case 12:
#line 232 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ellipsis_dim((yyvsp[-3].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2088 "grammar.c" /* yacc.c:1652  */
    break;

  case 13:
#line 235 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2094 "grammar.c" /* yacc.c:1652  */
    break;

  case 14:
#line 236 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2100 "grammar.c" /* yacc.c:1652  */
    break;

  case 15:
#line 239 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2106 "grammar.c" /* yacc.c:1652  */
    break;

  case 16:
#line 240 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_contig((yyvsp[-3].string), (ndt_t *)(yyvsp[-1].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2112 "grammar.c" /* yacc.c:1652  */
    break;

  case 17:
#line 241 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fortran((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2118 "grammar.c" /* yacc.c:1652  */
    break;

  case 18:
#line 244 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_shape((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2124 "grammar.c" /* yacc.c:1652  */
    break;

  case 19:
#line 245 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_dim_from_attrs((yyvsp[-3].attribute_seq), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2130 "grammar.c" /* yacc.c:1652  */
    break;

  case 20:
#line 246 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_symbolic_dim((yyvsp[-2].string), (yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2136 "grammar.c" /* yacc.c:1652  */
    break;

  case 21:
#line 247 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2142 "grammar.c" /* yacc.c:1652  */
    break;

  case 22:
#line 248 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_var_dim((yyvsp[-2].attribute_seq), (yyvsp[0].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2148 "grammar.c" /* yacc.c:1652  */
    break;

  case 23:
#line 252 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2154 "grammar.c" /* yacc.c:1652  */
    break;

  case 24:
#line 253 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2160 "grammar.c" /* yacc.c:1652  */
    break;

  case 25:
#line 256 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_any_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2166 "grammar.c" /* yacc.c:1652  */
    break;

  case 26:
#line 257 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_scalar_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2172 "grammar.c" /* yacc.c:1652  */
    break;

  case 27:
#line 258 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2178 "grammar.c" /* yacc.c:1652  */
    break;

  case 28:
#line 259 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2184 "grammar.c" /* yacc.c:1652  */
    break;

  case 29:
#line 260 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2190 "grammar.c" /* yacc.c:1652  */
    break;

  case 30:
#line 261 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2196 "grammar.c" /* yacc.c:1652  */
    break;

  case 31:
#line 262 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2202 "grammar.c" /* yacc.c:1652  */
    break;

  case 32:
#line 263 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_nominal((yyvsp[0].string), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2208 "grammar.c" /* yacc.c:1652  */
    break;

  case 33:
#line 264 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2214 "grammar.c" /* yacc.c:1652  */
    break;

  case 34:
#line 265 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_constr((yyvsp[-3].string), (yyvsp[-1].ndt), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2220 "grammar.c" /* yacc.c:1652  */
    break;

  case 35:
#line 266 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_typevar((yyvsp[0].string), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2226 "grammar.c" /* yacc.c:1652  */
    break;

  case 36:
#line 269 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(Bool, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2232 "grammar.c" /* yacc.c:1652  */
    break;

  case 37:
#line 270 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_signed_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2238 "grammar.c" /* yacc.c:1652  */
    break;

  case 38:
#line 271 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2244 "grammar.c" /* yacc.c:1652  */
    break;

  case 39:
#line 272 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_unsigned_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2250 "grammar.c" /* yacc.c:1652  */
    break;

  case 40:
#line 273 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2256 "grammar.c" /* yacc.c:1652  */
    break;

  case 41:
#line 274 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_float_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2262 "grammar.c" /* yacc.c:1652  */
    break;

  case 42:
#line 275 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BFloat16, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2268 "grammar.c" /* yacc.c:1652  */
    break;

  case 43:
#line 276 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2274 "grammar.c" /* yacc.c:1652  */
    break;

  case 44:
#line 277 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_complex_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2280 "grammar.c" /* yacc.c:1652  */
    break;

  case 45:
#line 278 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive(BComplex32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2286 "grammar.c" /* yacc.c:1652  */
    break;

  case 46:
#line 279 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_primitive((yyvsp[0].tag), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2292 "grammar.c" /* yacc.c:1652  */
    break;

  case 47:
#line 280 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_from_alias((yyvsp[0].alias), (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2298 "grammar.c" /* yacc.c:1652  */
    break;

  case 48:
#line 281 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2304 "grammar.c" /* yacc.c:1652  */
    break;

  case 49:
#line 282 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2310 "grammar.c" /* yacc.c:1652  */
    break;

  case 50:
#line 283 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_string_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2316 "grammar.c" /* yacc.c:1652  */
    break;

  case 51:
#line 284 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2322 "grammar.c" /* yacc.c:1652  */
    break;

  case 52:
#line 285 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2328 "grammar.c" /* yacc.c:1652  */
    break;

  case 53:
#line 286 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_fixed_bytes_kind((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2334 "grammar.c" /* yacc.c:1652  */
    break;

  case 54:
#line 287 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2340 "grammar.c" /* yacc.c:1652  */
    break;

  case 55:
#line 288 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2346 "grammar.c" /* yacc.c:1652  */
    break;

  case 56:
#line 289 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2352 "grammar.c" /* yacc.c:1652  */
    break;

  case 57:
#line 292 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int8; }
#line 2358 "grammar.c" /* yacc.c:1652  */
    break;

  case 58:
#line 293 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int16; }
#line 2364 "grammar.c" /* yacc.c:1652  */
    break;

  case 59:
#line 294 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int32; }
#line 2370 "grammar.c" /* yacc.c:1652  */
    break;

  case 60:
#line 295 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Int64; }
#line 2376 "grammar.c" /* yacc.c:1652  */
    break;

  case 61:
#line 298 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint8; }
#line 2382 "grammar.c" /* yacc.c:1652  */
    break;

  case 62:
#line 299 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint16; }
#line 2388 "grammar.c" /* yacc.c:1652  */
    break;

  case 63:
#line 300 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint32; }
#line 2394 "grammar.c" /* yacc.c:1652  */
    break;

  case 64:
#line 301 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Uint64; }
#line 2400 "grammar.c" /* yacc.c:1652  */
    break;

  case 65:
#line 304 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float16; }
#line 2406 "grammar.c" /* yacc.c:1652  */
    break;

  case 66:
#line 305 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float32; }
#line 2412 "grammar.c" /* yacc.c:1652  */
    break;

  case 67:
#line 306 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Float64; }
#line 2418 "grammar.c" /* yacc.c:1652  */
    break;

  case 68:
#line 309 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex32; }
#line 2424 "grammar.c" /* yacc.c:1652  */
    break;

  case 69:
#line 310 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex64; }
#line 2430 "grammar.c" /* yacc.c:1652  */
    break;

  case 70:
#line 311 "grammar.y" /* yacc.c:1652  */
    { (yyval.tag) = Complex128; }
#line 2436 "grammar.c" /* yacc.c:1652  */
    break;

  case 71:
#line 315 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Intptr; }
#line 2442 "grammar.c" /* yacc.c:1652  */
    break;

  case 72:
#line 316 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Uintptr; }
#line 2448 "grammar.c" /* yacc.c:1652  */
    break;

  case 73:
#line 317 "grammar.y" /* yacc.c:1652  */
    { (yyval.alias) = Size; }
#line 2454 "grammar.c" /* yacc.c:1652  */
    break;

  case 74:
#line 320 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char(Utf32, (yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2460 "grammar.c" /* yacc.c:1652  */
    break;

  case 75:
#line 321 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_char((yyvsp[-1].encoding), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2466 "grammar.c" /* yacc.c:1652  */
    break;

  case 76:
#line 324 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = ndt_string((yyvsp[-1].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2472 "grammar.c" /* yacc.c:1652  */
    break;

  case 77:
#line 327 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-1].string), Utf8, (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2478 "grammar.c" /* yacc.c:1652  */
    break;

  case 78:
#line 328 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_string((yyvsp[-3].string), (yyvsp[-1].encoding), (yyvsp[-6].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2484 "grammar.c" /* yacc.c:1652  */
    break;

  case 79:
#line 331 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = (yyvsp[-1].uint32) | (yyvsp[0].uint32); }
#line 2490 "grammar.c" /* yacc.c:1652  */
    break;

  case 80:
#line 334 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2496 "grammar.c" /* yacc.c:1652  */
    break;

  case 81:
#line 335 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_OPTION; }
#line 2502 "grammar.c" /* yacc.c:1652  */
    break;

  case 82:
#line 338 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2508 "grammar.c" /* yacc.c:1652  */
    break;

  case 83:
#line 339 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_SYS_BIG_ENDIAN ? NDT_BIG_ENDIAN : NDT_LITTLE_ENDIAN; }
#line 2514 "grammar.c" /* yacc.c:1652  */
    break;

  case 84:
#line 340 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_LITTLE_ENDIAN; }
#line 2520 "grammar.c" /* yacc.c:1652  */
    break;

  case 85:
#line 341 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = NDT_BIG_ENDIAN; }
#line 2526 "grammar.c" /* yacc.c:1652  */
    break;

  case 86:
#line 342 "grammar.y" /* yacc.c:1652  */
    { (yyval.uint32) = 0; }
#line 2532 "grammar.c" /* yacc.c:1652  */
    break;

  case 87:
#line 345 "grammar.y" /* yacc.c:1652  */
    { (yyval.encoding) = encoding_from_string((yyvsp[0].string), ctx); if (ndt_err_occurred(ctx)) YYABORT; }
#line 2538 "grammar.c" /* yacc.c:1652  */
    break;

  case 88:
#line 348 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_bytes((yyvsp[0].attribute_seq), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2544 "grammar.c" /* yacc.c:1652  */
    break;

  case 89:
#line 351 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = (yyvsp[0].ndt); }
#line 2550 "grammar.c" /* yacc.c:1652  */
    break;

  case 90:
#line 352 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_array_ellipsis((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2556 "grammar.c" /* yacc.c:1652  */
    break;

  case 91:
#line 353 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_array_ellipsis((yyvsp[0].ndt), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2562 "grammar.c" /* yacc.c:1652  */
    break;

  case 92:
#line 356 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_array((yyvsp[0].ndt), (yyvsp[-3].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2568 "grammar.c" /* yacc.c:1652  */
    break;

  case 93:
#line 357 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_array((yyvsp[0].ndt), (yyvsp[-3].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2574 "grammar.c" /* yacc.c:1652  */
    break;

  case 94:
#line 360 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_fixed_bytes((yyvsp[-1].attribute_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2580 "grammar.c" /* yacc.c:1652  */
    break;

  case 95:
#line 363 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[-1].ndt), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2586 "grammar.c" /* yacc.c:1652  */
    break;

  case 96:
#line 364 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_ref((yyvsp[0].ndt), (yyvsp[-2].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2592 "grammar.c" /* yacc.c:1652  */
    break;

  case 97:
#line 367 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_categorical((yyvsp[-1].typed_value_seq), (yyvsp[-4].uint32), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2598 "grammar.c" /* yacc.c:1652  */
    break;

  case 98:
#line 370 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_new((yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2604 "grammar.c" /* yacc.c:1652  */
    break;

  case 99:
#line 371 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value_seq) = ndt_value_seq_append((yyvsp[-2].typed_value_seq), (yyvsp[0].typed_value), ctx); if ((yyval.typed_value_seq) == NULL) YYABORT; }
#line 2610 "grammar.c" /* yacc.c:1652  */
    break;

  case 100:
#line 374 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValInt64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2616 "grammar.c" /* yacc.c:1652  */
    break;

  case 101:
#line 375 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_number(ValFloat64, (yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2622 "grammar.c" /* yacc.c:1652  */
    break;

  case 102:
#line 376 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_from_string((yyvsp[0].string), ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2628 "grammar.c" /* yacc.c:1652  */
    break;

  case 103:
#line 377 "grammar.y" /* yacc.c:1652  */
    { (yyval.typed_value) = ndt_value_na(ctx); if ((yyval.typed_value) == NULL) YYABORT; }
#line 2634 "grammar.c" /* yacc.c:1652  */
    break;

  case 104:
#line 380 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2640 "grammar.c" /* yacc.c:1652  */
    break;

  case 105:
#line 381 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2646 "grammar.c" /* yacc.c:1652  */
    break;

  case 106:
#line 384 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2652 "grammar.c" /* yacc.c:1652  */
    break;

  case 107:
#line 385 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Nonvariadic; }
#line 2658 "grammar.c" /* yacc.c:1652  */
    break;

  case 108:
#line 386 "grammar.y" /* yacc.c:1652  */
    { (yyval.variadic_flag) = Variadic; }
#line 2664 "grammar.c" /* yacc.c:1652  */
    break;

  case 109:
#line 389 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2670 "grammar.c" /* yacc.c:1652  */
    break;

  case 110:
#line 390 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2676 "grammar.c" /* yacc.c:1652  */
    break;

  case 111:
#line 391 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2682 "grammar.c" /* yacc.c:1652  */
    break;

  case 112:
#line 392 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2688 "grammar.c" /* yacc.c:1652  */
    break;

  case 113:
#line 393 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2694 "grammar.c" /* yacc.c:1652  */
    break;

  case 114:
#line 394 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_tuple(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2700 "grammar.c" /* yacc.c:1652  */
    break;

  case 115:
#line 397 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2706 "grammar.c" /* yacc.c:1652  */
    break;

  case 116:
#line 398 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2712 "grammar.c" /* yacc.c:1652  */
    break;

  case 117:
#line 401 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2718 "grammar.c" /* yacc.c:1652  */
    break;

  case 118:
#line 402 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field(NULL, (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2724 "grammar.c" /* yacc.c:1652  */
    break;

  case 119:
#line 405 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2730 "grammar.c" /* yacc.c:1652  */
    break;

  case 120:
#line 406 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2736 "grammar.c" /* yacc.c:1652  */
    break;

  case 121:
#line 407 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2742 "grammar.c" /* yacc.c:1652  */
    break;

  case 122:
#line 408 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), NULL, NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2748 "grammar.c" /* yacc.c:1652  */
    break;

  case 123:
#line 409 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record((yyvsp[-1].variadic_flag), (yyvsp[-2].field_seq), NULL, true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2754 "grammar.c" /* yacc.c:1652  */
    break;

  case 124:
#line 410 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_record(Nonvariadic, (yyvsp[-3].field_seq), (yyvsp[-1].attribute_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2760 "grammar.c" /* yacc.c:1652  */
    break;

  case 125:
#line 413 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2766 "grammar.c" /* yacc.c:1652  */
    break;

  case 126:
#line 414 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2772 "grammar.c" /* yacc.c:1652  */
    break;

  case 127:
#line 417 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2778 "grammar.c" /* yacc.c:1652  */
    break;

  case 128:
#line 418 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-5].string), (yyvsp[-3].ndt), (yyvsp[-1].attribute_seq), ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2784 "grammar.c" /* yacc.c:1652  */
    break;

  case 129:
#line 421 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2790 "grammar.c" /* yacc.c:1652  */
    break;

  case 130:
#line 422 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2796 "grammar.c" /* yacc.c:1652  */
    break;

  case 131:
#line 423 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2802 "grammar.c" /* yacc.c:1652  */
    break;

  case 132:
#line 426 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[0].field_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2808 "grammar.c" /* yacc.c:1652  */
    break;

  case 133:
#line 427 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), false, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2814 "grammar.c" /* yacc.c:1652  */
    break;

  case 134:
#line 428 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[0].field_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2820 "grammar.c" /* yacc.c:1652  */
    break;

  case 135:
#line 429 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_union((yyvsp[-1].field_seq), true, ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2826 "grammar.c" /* yacc.c:1652  */
    break;

  case 136:
#line 432 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_new((yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2832 "grammar.c" /* yacc.c:1652  */
    break;

  case 137:
#line 433 "grammar.y" /* yacc.c:1652  */
    { (yyval.field_seq) = ndt_field_seq_append((yyvsp[-2].field_seq), (yyvsp[0].field), ctx); if ((yyval.field_seq) == NULL) YYABORT; }
#line 2838 "grammar.c" /* yacc.c:1652  */
    break;

  case 138:
#line 436 "grammar.y" /* yacc.c:1652  */
    { (yyval.field) = mk_field((yyvsp[-2].string), (yyvsp[0].ndt), NULL, ctx); if ((yyval.field) == NULL) YYABORT; }
#line 2844 "grammar.c" /* yacc.c:1652  */
    break;

  case 139:
#line 439 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = NULL; }
#line 2850 "grammar.c" /* yacc.c:1652  */
    break;

  case 140:
#line 440 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = (yyvsp[-1].attribute_seq); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2856 "grammar.c" /* yacc.c:1652  */
    break;

  case 141:
#line 443 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_new((yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2862 "grammar.c" /* yacc.c:1652  */
    break;

  case 142:
#line 444 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute_seq) = ndt_attr_seq_append((yyvsp[-2].attribute_seq), (yyvsp[0].attribute), ctx); if ((yyval.attribute_seq) == NULL) YYABORT; }
#line 2868 "grammar.c" /* yacc.c:1652  */
    break;

  case 143:
#line 447 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr((yyvsp[-2].string), (yyvsp[0].string), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2874 "grammar.c" /* yacc.c:1652  */
    break;

  case 144:
#line 448 "grammar.y" /* yacc.c:1652  */
    { (yyval.attribute) = mk_attr_from_seq((yyvsp[-4].string), (yyvsp[-1].string_seq), ctx); if ((yyval.attribute) == NULL) YYABORT; }
#line 2880 "grammar.c" /* yacc.c:1652  */
    break;

  case 145:
#line 451 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_new((yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2886 "grammar.c" /* yacc.c:1652  */
    break;

  case 146:
#line 452 "grammar.y" /* yacc.c:1652  */
    { (yyval.string_seq) = ndt_string_seq_append((yyvsp[-2].string_seq), (yyvsp[0].string), ctx); if ((yyval.string_seq) == NULL) YYABORT; }
#line 2892 "grammar.c" /* yacc.c:1652  */
    break;

  case 147:
#line 455 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2898 "grammar.c" /* yacc.c:1652  */
    break;

  case 148:
#line 456 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2904 "grammar.c" /* yacc.c:1652  */
    break;

  case 149:
#line 457 "grammar.y" /* yacc.c:1652  */
    { (yyval.string) = (yyvsp[0].string); if ((yyval.string) == NULL) YYABORT; }
#line 2910 "grammar.c" /* yacc.c:1652  */
    break;

  case 150:
#line 460 "grammar.y" /* yacc.c:1652  */
    { (yyval.ndt) = mk_function((yyvsp[-2].type_seq), (yyvsp[0].type_seq), ctx); if ((yyval.ndt) == NULL) YYABORT; }
#line 2916 "grammar.c" /* yacc.c:1652  */
    break;

  case 151:
#line 463 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = (yyvsp[0].type_seq); }
#line 2922 "grammar.c" /* yacc.c:1652  */
    break;

  case 152:
#line 464 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_empty(ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2928 "grammar.c" /* yacc.c:1652  */
    break;

  case 153:
#line 467 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_new((ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2934 "grammar.c" /* yacc.c:1652  */
    break;

  case 154:
#line 468 "grammar.y" /* yacc.c:1652  */
    { (yyval.type_seq) = ndt_type_seq_append((yyvsp[-2].type_seq), (ndt_t *)(yyvsp[0].ndt), ctx); if ((yyval.type_seq) == NULL) YYABORT; }
#line 2940 "grammar.c" /* yacc.c:1652  */
    break;


#line 2944 "grammar.c" /* yacc.c:1652  */
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
