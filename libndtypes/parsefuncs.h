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


#ifndef PARSEFUNCS_H
#define PARSEFUNCS_H


#include "ndtypes.h"
#include "seq.h"
#include "attr.h"


/* LOCAL SCOPE */
NDT_PRAGMA(NDT_HIDE_SYMBOLS_START)


/*****************************************************************************/
/*                        Functions used in the lexer                        */
/*****************************************************************************/

char *mk_stringlit(const char *lexeme, ndt_context_t *ctx);


/*****************************************************************************/
/*                          Parser helper functions                          */
/*****************************************************************************/

enum ndt_encoding encoding_from_string(char *s, ndt_context_t *ctx);
ndt_attr_t *mk_attr(char *name, char *value, ndt_context_t *ctx);
ndt_attr_t *mk_attr_from_seq(char *name, ndt_string_seq_t *seq, ndt_context_t *ctx);


/*****************************************************************************/
/*                    Parser functions for creating types                    */
/*****************************************************************************/

const ndt_t *mk_module(char *name, const ndt_t *type, ndt_context_t *ctx);
const ndt_t *mk_function(ndt_type_seq_t *in, ndt_type_seq_t *out, ndt_context_t *ctx);

const ndt_t *mk_fortran(const ndt_t *type, ndt_context_t *ctx);
const ndt_t *mk_contig(char *name, ndt_t *type, ndt_context_t *ctx);
const ndt_t *mk_fixed_dim_from_shape(char *v, const ndt_t *type, ndt_context_t *ctx);
const ndt_t *mk_fixed_dim_from_attrs(ndt_attr_seq_t *attrs, const ndt_t *type, ndt_context_t *ctx);

const ndt_t *mk_var_dim(ndt_attr_seq_t *seq, const ndt_t *type, bool opt, ndt_context_t *ctx);
const ndt_t *mk_var_ellipsis(const ndt_t *type, ndt_context_t *ctx);

const ndt_t *mk_ellipsis_dim(char *name, const ndt_t *type, ndt_context_t *ctx);
const ndt_t *mk_symbolic_dim(char *name, const ndt_t *type, ndt_context_t *ctx);

const ndt_t *mk_array(const ndt_t *type, bool opt, ndt_context_t *ctx);

ndt_field_t *mk_field(char *name, const ndt_t *type, ndt_attr_seq_t *seq, ndt_context_t *ctx);
const ndt_t *mk_tuple(enum ndt_variadic flag, ndt_field_seq_t *fields, ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx);
const ndt_t *mk_record(enum ndt_variadic flag, ndt_field_seq_t *fields, ndt_attr_seq_t *attrs, bool opt, ndt_context_t *ctx);
const ndt_t * mk_union(ndt_field_seq_t *fields, bool opt, ndt_context_t *ctx);

const ndt_t *mk_constr(char *name, const ndt_t *type, bool opt, ndt_context_t *ctx);
const ndt_t *mk_ref(const ndt_t *type, bool opt, ndt_context_t *ctx);

const ndt_t *mk_categorical(ndt_value_seq_t *seq, bool opt, ndt_context_t *ctx);

const ndt_t *mk_fixed_string(char *v, enum ndt_encoding encoding, bool opt, ndt_context_t *ctx);
const ndt_t *mk_bytes(ndt_attr_seq_t *seq, bool opt, ndt_context_t *ctx);
const ndt_t *mk_fixed_bytes(ndt_attr_seq_t *seq, bool opt, ndt_context_t *ctx);



/* END LOCAL SCOPE */
NDT_PRAGMA(NDT_HIDE_SYMBOLS_END)


#endif /*  PARSEFUNCS_H */
