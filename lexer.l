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


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <setjmp.h>
#include "ndtypes.h"
#include "parsefuncs.h"
#include "grammar.h"

/* From PostgreSQL: avoid exit() on fatal scanner errors. */
#undef fprintf
#define fprintf(file, fmt, msg) fprintf_to_longjmp(fmt, msg, yyscanner)

extern jmp_buf ndt_lexerror;
static void
fprintf_to_longjmp(const char *fmt, const char *msg, yyscan_t yyscanner)
{
    (void)fmt; (void)msg; (void)yyscanner;

    /* We don't have access to the parse context here:  discard the error
       message, which is always either an allocation failure or an internal
       flex error. */
    longjmp(ndt_lexerror, 1);
}

#undef yyalloc
#undef yyrealloc
#undef yyfree

void *
yyalloc(size_t size, yyscan_t yyscanner)
{
    (void)yyscanner;

    return ndt_alloc(1, size);
}

void *
yyrealloc(void *ptr, size_t size, yyscan_t yyscanner)
{
    (void)yyscanner;

    return ndt_realloc(ptr, 1, size);
}

void
yyfree(void *ptr, yyscan_t yyscanner)
{
    (void)yyscanner;

    ndt_free(ptr);
}

%}

%option bison-bridge bison-locations reentrant noyywrap
%option nounput noinput noyyalloc noyyrealloc noyyfree
%option never-interactive
%option yylineno
%option 8bit
%option extra-type="ndt_context_t *"
%option warn nodefault


newline        [\n\r]
space          [ \t\f]
non_newline    [^\n\r]
comment        #{non_newline}*

escapeseq        \\.
single_strchar [^\\\n']
double_strchar [^\\\n"]
single_str     '({single_strchar}|{escapeseq})*'
double_str     \"({double_strchar}|{escapeseq})*\"
stringlit      {single_str}|{double_str}

octdigit       [0-7]
octinteger     0[oO]{octdigit}+
nonzerodigit   [1-9]
digit          [0-9]
decimalinteger {nonzerodigit}{digit}*|0+
hexdigit       {digit}|[a-f]|[A-F]
hexinteger     0[xX]{hexdigit}+
integer        -?({decimalinteger}|{octinteger}|{hexinteger})

intpart        {digit}+
fraction       \.{digit}+
exponent       [eE][+-]?{digit}+
pointfloat     {intpart}?{fraction}|{intpart}\.
exponentfloat  ({intpart}|{pointfloat}){exponent}
floatnumber    -?({pointfloat}|{exponentfloat})

name_lower     [a-z][a-zA-Z0-9_]*
name_upper     [A-Z][a-zA-Z0-9_]*
name_other     _[a-zA-Z0-9_]*


%%

%code {
yycolumn = 1;

#undef YY_USER_ACTION
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn;                  \
    yylloc->last_column = yycolumn+yyleng-1;          \
    yycolumn += yyleng;

}

"Any"          { return ANY_KIND; }
"Scalar"       { return SCALAR_KIND; }

"void"         { return VOID; }
"bool"         { return BOOL; }

"Signed"       { return SIGNED_KIND; }
"int8"         { return INT8; }
"int16"        { return INT16; }
"int32"        { return INT32; }
"int64"        { return INT64; }
"int128"       { return INT128; }

"Unsigned"     { return UNSIGNED_KIND; }
"uint8"        { return UINT8; }
"uint16"       { return UINT16; }
"uint32"       { return UINT32; }
"uint64"       { return UINT64; }
"uint128"      { return UINT128; }

"Real"         { return REAL_KIND; }
"float16"      { return FLOAT16; }
"float32"      { return FLOAT32; }
"float64"      { return FLOAT64; }
"float128"     { return FLOAT128; }

"Complex"      { return COMPLEX_KIND; }
"complex64"    { return COMPLEX64; }
"complex128"   { return COMPLEX128; }

"intptr"       { return INTPTR; }
"uintptr"      { return UINTPTR; }
"size"         { return SIZE; }
"real"         { return REAL; }
"complex"      { return COMPLEX; }
"int"          { return INT; }
"char"         { return CHAR; }
"string"       { return STRING; }
"bytes"        { return BYTES; }

"FixedString"  { return FIXED_STRING_KIND; }
"fixed_string" { return FIXED_STRING; }

"FixedBytes"   { return FIXED_BYTES_KIND; }
"fixed_bytes"  { return FIXED_BYTES; }

"categorical"  { return CATEGORICAL; }

"nominal"      { return NOMINAL; }

"pointer"      { return POINTER; }

"option"       { return OPTION; }

"Fixed"        { return FIXED_DIM_KIND; }
"fixed"        { return FIXED; }
"var"          { return VAR; }

"align"        { return ALIGN; }

"..."          { return ELLIPSIS; }
"**"           { return DOUBLESTAR; }
"->"           { return RARROW; }
","            { return COMMA; }
":"            { return COLON; }
"("            { return LPAREN; }
")"            { return RPAREN; }
"{"            { return LBRACE; }
"}"            { return RBRACE; }
"["            { return LBRACK; }
"]"            { return RBRACK; }
"*"            { return STAR; }
"="            { return EQUAL; }
"?"            { return QUESTIONMARK; }

{name_lower}   { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return NAME_LOWER; }
{name_upper}   { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return NAME_UPPER; }
{name_other}   { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return NAME_OTHER; }

{stringlit}    { yylval->string = mk_stringlit(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return STRINGLIT; }
{integer}      { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return INTEGER; }
{floatnumber}  { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return FLOATNUMBER; }

{newline}      { yycolumn = 1; }
{space}        {} /* ignore */
{comment}      {} /* ignore */
.              { return ERRTOKEN; }

%%
