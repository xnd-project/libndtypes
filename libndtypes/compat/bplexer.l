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


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <setjmp.h>
#include "ndtypes.h"
#include "bpgrammar.h"


/* From PostgreSQL: avoid exit() on fatal scanner errors. */
#undef fprintf
#define fprintf(file, fmt, msg) fprintf_to_longjmp(fmt, msg, yyscanner)

extern jmp_buf ndt_bp_lexerror;
static void
fprintf_to_longjmp(const char *fmt, const char *msg, yyscan_t yyscanner)
{
    (void)fmt; (void)msg; (void)yyscanner;

    /* We don't have access to the parse context here:  discard the error
       message, which is always either an allocation failure or an internal
       flex error. */
    longjmp(ndt_bp_lexerror, 1);
}

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

%x FIELDNAME

newline        [\n\r]
space          [ \t\f]
non_newline    [^\n\r]
comment        #{non_newline}*

nonzerodigit   [1-9]
digit          [0-9]
decimalinteger {nonzerodigit}{digit}*|0+
integer        -?({decimalinteger})

name           [A-Za-z][a-zA-Z0-9_]*

%%

%code {
yycolumn = 1;

#undef YY_USER_ACTION
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn;                   \
    yylloc->last_column = yycolumn+yyleng-1;           \
    yycolumn += yyleng;

}

<INITIAL>{
"?"        { yylval->uchar = '?'; return DTYPE; }
"c"        { yylval->uchar = 'c'; return DTYPE; }
"b"        { yylval->uchar = 'b'; return DTYPE; }
"B"        { yylval->uchar = 'B'; return DTYPE; }
"h"        { yylval->uchar = 'h'; return DTYPE; }
"H"        { yylval->uchar = 'H'; return DTYPE; }
"i"        { yylval->uchar = 'i'; return DTYPE; }
"I"        { yylval->uchar = 'I'; return DTYPE; }
"l"        { yylval->uchar = 'l'; return DTYPE; }
"L"        { yylval->uchar = 'L'; return DTYPE; }
"q"        { yylval->uchar = 'q'; return DTYPE; }
"Q"        { yylval->uchar = 'Q'; return DTYPE; }
"n"        { yylval->uchar = 'n'; return DTYPE; }
"N"        { yylval->uchar = 'N'; return DTYPE; }
"e"        { yylval->uchar = 'e'; return DTYPE; }
"f"        { yylval->uchar = 'f'; return DTYPE; }
"d"        { yylval->uchar = 'd'; return DTYPE; }
"E"        { yylval->uchar = 'E'; return DTYPE; }
"F"        { yylval->uchar = 'F'; return DTYPE; }
"D"        { yylval->uchar = 'D'; return DTYPE; }

"x"        { return PAD; }
"T"        { return RECORD; }
"s"        { return BYTES; }

"->"       { return RARROW; }
"@"        { return AT; }
"="        { return EQUAL; }
"<"        { return LESS; }
">"        { return GREATER; }
"!"        { return BANG; }
","        { return COMMA; }
":"        { BEGIN(FIELDNAME); return COLON; }
"("        { return LPAREN; }
")"        { return RPAREN; }
"{"        { return LBRACE; }
"}"        { return RBRACE; }

{integer}  { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return INTEGER; }
}

<FIELDNAME>{
":"     { BEGIN(INITIAL); return COLON; }
{name}  { yylval->string = ndt_strdup(yytext, ctx); if (yylval->string == NULL) return ERRTOKEN; return NAME; }
}

<INITIAL,FIELDNAME>{
{newline}  { yycolumn = 1; }
{space}    {} /* ignore */
{comment}  {} /* ignore */
.          { return ERRTOKEN; }
}


%%
