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


#ifndef PARSEFUNCS_H
#define PARSEFUNCS_H


#include "ndtypes.h"
#include "seq.h"


/*****************************************************************************/
/*                        Functions used in the lexer                        */
/*****************************************************************************/

char *mk_stringlit(const char *src, ndt_context_t *ctx);


/*****************************************************************************/
/*                        Functions used in the parser                       */
/*****************************************************************************/

ndt_t *mk_fixed_dim(char *v, ndt_t *type, ndt_context_t *ctx);
ndt_t *mk_var_dim(ndt_string_seq_t *seq, ndt_t *type, ndt_context_t *ctx);
ndt_t *mk_primitive(enum ndt tag, ndt_attr_seq_t *attrs, ndt_context_t *ctx);
ndt_t *mk_alias(enum ndt tag, ndt_attr_seq_t *seq, ndt_context_t *ctx);
ndt_t *mk_fixed_string(const char *v, enum ndt_encoding encoding, ndt_context_t *ctx);
ndt_t *mk_bytes(ndt_attr_seq_t *seq, ndt_context_t *ctx);
ndt_t *mk_fixed_bytes(ndt_attr_seq_t *seq, ndt_context_t *ctx);
ndt_t *mk_array(ndt_t *dims, ndt_attr_seq_t *attrs, ndt_context_t *ctx);
ndt_tuple_field_t *mk_tuple_field(ndt_t *type, ndt_attr_seq_t *seq, ndt_context_t *ctx);
ndt_t *mk_tuple(enum ndt_variadic_flag flag, ndt_tuple_field_seq_t *fields, ndt_attr_seq_t *attrs, ndt_context_t *ctx);
ndt_record_field_t * mk_record_field(char *name, ndt_t *type, ndt_attr_seq_t *seq, ndt_context_t *ctx);
ndt_t *mk_record(enum ndt_variadic_flag flag, ndt_record_field_seq_t *fields, ndt_attr_seq_t *attrs, ndt_context_t *ctx);
ndt_t *mk_function(ndt_t *ret,
                   enum ndt_variadic_flag tflag, ndt_tuple_field_seq_t *tseq,
                   enum ndt_variadic_flag rflag, ndt_record_field_seq_t *rseq,
                   ndt_context_t *ctx);
ndt_t *mk_function_from_tuple(ndt_t *ret, ndt_t *pos, ndt_context_t *ctx);
ndt_t *mk_categorical(ndt_memory_seq_t *seq, ndt_context_t *ctx);
ndt_attr_t *mk_attr(char *name, char *value, ndt_context_t *ctx);
ndt_attr_t *mk_attr_from_seq(char *name, ndt_string_seq_t *seq, ndt_context_t *ctx);


#endif /*  PARSEFUNCS_H */
