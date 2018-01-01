%{
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


#include "bpgrammar.h"
#include "bplexer.h"


#undef bperror
#undef bplex

void
bperror(YYLTYPE *loc, yyscan_t scanner, ndt_t **ast, ndt_context_t *ctx,
        const char *msg)
{
    (void)scanner;
    (void)ast;

    ndt_err_format(ctx, NDT_ParseError, "%d:%d: %s\n", loc->first_line,
                   loc->first_column, msg);
}

int
bplex(YYSTYPE *val, YYLTYPE *loc, yyscan_t scanner, ndt_context_t *ctx)
{
    return bplexfunc(val, loc, scanner, ctx);
}

static uint16_t
add_uint16(uint16_t a, uint16_t b, ndt_context_t *ctx)
{
    uint16_t c = a + b;

    if (c < a) {
        ndt_err_format(ctx, NDT_ValueError, "overflow while adding uint16");
        return UINT16_MAX;
    }

    return c;
}

static ndt_t *
primitive_native(char dtype, ndt_context_t *ctx)
{
    switch (dtype) {
    case '?': return ndt_primitive(Bool, 0, ctx);

    case 'c': return ndt_primitive(Int8, 0, ctx);
    case 'b': return ndt_primitive(Int8, 0, ctx);
    case 'B': return ndt_primitive(Uint8, 0, ctx);

    case 'h': return ndt_signed(sizeof(short), 0, ctx);
    case 'i': return ndt_signed(sizeof(int), 0, ctx);
    case 'l': return ndt_signed(sizeof(long), 0, ctx);
    case 'q': return ndt_signed(sizeof(long long), 0, ctx);
    case 'n': return ndt_signed(sizeof(size_t), 0, ctx);

    case 'H': return ndt_unsigned(sizeof(unsigned short), 0, ctx);
    case 'I': return ndt_unsigned(sizeof(unsigned int), 0, ctx);
    case 'L': return ndt_unsigned(sizeof(unsigned long), 0, ctx);
    case 'Q': return ndt_unsigned(sizeof(unsigned long long), 0, ctx);
    case 'N': return ndt_unsigned(sizeof(size_t), 0, ctx);

    case 'e': return ndt_primitive(Float16, 0, ctx);
    case 'f': return ndt_primitive(Float32, 0, ctx);
    case 'd': return ndt_primitive(Float64, 0, ctx);

    case 'E': return ndt_primitive(Complex32, 0, ctx);
    case 'F': return ndt_primitive(Complex64, 0, ctx);
    case 'D': return ndt_primitive(Complex128, 0, ctx);

    default:
        ndt_err_format(ctx, NDT_ValueError,
            "invalid or unsupported dtype '%c'", dtype);
        return NULL;
    }
}

static ndt_t *
primitive_fixed(char dtype, uint32_t flags, ndt_context_t *ctx)
{
    enum ndt tag;

    switch (dtype) {
    case '?': tag = Bool; break;

    case 'c': tag = Int8; break;
    case 'b': tag = Int8; break;
    case 'B': tag = Uint8; break;

    case 'h': tag = Int16; break;
    case 'i': tag = Int32; break;
    case 'l': tag = Int32; break;
    case 'q': tag = Int64; break;

    case 'H': tag = Uint16; break;
    case 'I': tag = Uint32; break;
    case 'L': tag = Uint32; break;
    case 'Q': tag = Uint64; break;

    case 'e': tag = Float16; break;
    case 'f': tag = Float32; break;
    case 'd': tag = Float64; break;

    case 'E': tag = Complex32; break;
    case 'F': tag = Complex64; break;
    case 'D': tag = Complex128; break;

    default:
        ndt_err_format(ctx, NDT_ValueError,
            "invalid or unsupported dtype '%c'", dtype);
        return NULL;
    }

    return ndt_primitive(tag, flags, ctx);
}

static ndt_t *
mk_dtype(char modifier, char dtype, ndt_context_t *ctx)
{
    switch (modifier) {
    case '@':
        return primitive_native(dtype, ctx);
    case '=':
        return primitive_fixed(dtype, 0, ctx);
    case '<':
        return primitive_fixed(dtype, NDT_LITTLE_ENDIAN, ctx);
    case '>':
        return primitive_fixed(dtype, NDT_BIG_ENDIAN, ctx);
    case '!':
        return primitive_fixed(dtype, NDT_BIG_ENDIAN, ctx);
    default:
        ndt_err_format(ctx, NDT_RuntimeError, "invalid modifier '%c'",
                       modifier);
        return NULL;
    }
}

static ndt_t *
mk_fixed_bytes(char *v, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    int64_t datasize = 1;

    if (v != NULL) {
        datasize = ndt_strtoll(v, 0, INT64_MAX, ctx);
        ndt_free(v);
        if (ndt_err_occurred(ctx)) {
            return NULL;
        }
    }

    if (datasize < 1) {
        ndt_err_format(ctx, NDT_ValueError,
            "fixed bytes datasize must be greater than 0");
        return NULL;
    }

    return ndt_fixed_bytes(datasize, align, ctx);
}

static ndt_t *
mk_dimensions(ndt_string_seq_t *seq, ndt_t *type, ndt_context_t *ctx)
{
    ndt_t *t;
    int64_t shape;
    int64_t i;

    if (seq->len < 1 || seq->len > NDT_MAX_DIM) {
        ndt_err_format(ctx, NDT_ValueError,
            "number of dimensions must be between 1 and %d", NDT_MAX_DIM);
        ndt_string_seq_del(seq);
        ndt_del(type);
        return NULL;
    }

    for (i=0, t=type; i<seq->len; i++, type=t) {
        shape = ndt_strtoll(seq->ptr[i], 0, INT_MAX, ctx);
        if (ndt_err_occurred(ctx)) {
            ndt_string_seq_del(seq);
            return NULL;
        }

        t = ndt_fixed_dim(type, shape, INT64_MAX, ctx);
        if (t == NULL) {
            ndt_string_seq_del(seq);
            return NULL;
        }
    }

    ndt_string_seq_del(seq);

    return t;
}

static ndt_field_t *
mk_field(char *name, ndt_t *type, uint16_t padding, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    uint16_opt_t pad = {Some, 0};

    pad.Some = padding;
    return ndt_field(name, type, align, pack, pad, ctx);
}

static ndt_t *
mk_record(ndt_field_seq_t *fields, ndt_context_t *ctx)
{
    uint16_opt_t align = {None, 0};
    uint16_opt_t pack = {None, 0};
    ndt_t *t;
    int64_t i;

    fields = ndt_field_seq_finalize(fields);
    if (fields == NULL) {
        return ndt_record(Nonvariadic, NULL, 0, align, pack, ctx);
    }

    assert(fields->len >= 1);

    fields->ptr[0].Concrete.align = 1;
    fields->ptr[0].Concrete.explicit_align = true;

    for (i = 1; i < fields->len; i++) {
        uint16_t a = 1;
        if (fields->ptr[i-1].Concrete.pad != 0) {
            a = fields->ptr[i-1].type->align + fields->ptr[i-1].Concrete.pad;
        }
        fields->ptr[i].Concrete.align = a;
        fields->ptr[i].Concrete.explicit_align = true;
    }

    t = ndt_record(Nonvariadic, fields->ptr, fields->len, align, pack, ctx);
    ndt_free(fields);

    return t;
}
%}

%code requires {
  #include <ctype.h>
  #include <assert.h>
  #include "ndtypes.h"
  #include "seq.h"
  #define YY_TYPEDEF_YY_SCANNER_T
  typedef void * yyscan_t;
}

%code provides {
  #define YY_DECL extern int bplexfunc(YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, ndt_context_t *ctx)
  extern int bplexfunc(YYSTYPE *, YYLTYPE *, yyscan_t, ndt_context_t *);
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
    ndt_field_t *field;
    ndt_field_seq_t *field_seq;
    ndt_string_seq_t *string_seq;
    char *string;
    unsigned char uchar;
    uint16_t uint16;
}

%start input
%type <ndt> input
%type <ndt> datatype

%type <ndt> record
%type <field> field
%type <field_seq> field_seq

%type <ndt> dtype

%type <string_seq> dimensions
%type <string> repeat

%type <uchar> modifier
%type <uint16> padding

%token
 BYTES RECORD PAD

AT EQUAL LESS GREATER BANG COMMA COLON LPAREN RPAREN LBRACE RBRACE
ERRTOKEN

%token <uchar>
  DTYPE

%token <string>
  INTEGER NAME

%token ENDMARKER 0 "end of file"

%destructor { ndt_del($$); } <ndt>
%destructor { ndt_field_del($$); } <field>
%destructor { ndt_field_seq_del($$); } <field_seq>
%destructor { ndt_string_seq_del($$); } <string_seq>
%destructor { ndt_free($$); } <string>

%%

input:
  datatype ENDMARKER { $$ = $1;  *ast = $$; YYACCEPT; }

datatype:
  LPAREN dimensions RPAREN dtype { $$ = mk_dimensions($2, $4, ctx); if ($$ == NULL) YYABORT; }
| dtype                          { $$ = $1; }

dimensions:
  INTEGER                  { $$ = ndt_string_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| dimensions COMMA INTEGER { $$ = ndt_string_seq_append($1, $3, ctx); if ($$ == NULL) YYABORT; }

dtype:
  modifier DTYPE { $$ = mk_dtype($1, $2, ctx); if ($$ == NULL) YYABORT; }
| repeat BYTES   { $$ = mk_fixed_bytes($1, ctx); if ($$ == NULL) YYABORT; }
| record         { $$ = $1; }

record:
  RECORD LBRACE field_seq RBRACE { $$ = mk_record($3, ctx); if ($$ == NULL) YYABORT; }

field_seq:
  field           { $$ = ndt_field_seq_new($1, ctx); if ($$ == NULL) YYABORT; }
| field_seq field { $$ = ndt_field_seq_append($1, $2, ctx); if ($$ == NULL) YYABORT; }

field:
  datatype COLON NAME COLON padding { $$ = mk_field($3, $1, $5, ctx); if ($$ == NULL) YYABORT; }

modifier:
 %empty   { $$ = '@'; }
| AT      { $$ = '@'; }
| EQUAL   { $$ = '='; }
| LESS    { $$ = '<'; }
| GREATER { $$ = '>'; }
| BANG    { $$ = '!'; }

repeat:
  %empty  { $$ = NULL; }
| INTEGER { $$ = $1; if ($$ == NULL) YYABORT; }

padding:
  %empty      { $$ = 0; }
| padding PAD { $$ = add_uint16($1, 1, ctx); if (ndt_err_occurred(ctx)) YYABORT; }


