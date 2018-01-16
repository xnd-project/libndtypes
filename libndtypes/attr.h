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


#ifndef ATTR_H
#define ATTR_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ndtypes.h"
#include "seq.h"


#define MAX_ATTR 8

enum ndt_attr {
  AttrBool,
  AttrChar,
  AttrInt8,
  AttrInt16,
  AttrInt32,
  AttrInt64,
  AttrUint8,
  AttrUint16,
  AttrUint32,
  AttrUint64,
  AttrSize,
  AttrFloat32,
  AttrFloat64,
  AttrString,
  AttrInt32List,
  AttrCharOpt,
  AttrInt64Opt,
  AttrUint16Opt
};

enum ndt_attr_tag {
  AttrValue,
  AttrList
};

/* Attribute: name=value or name=[value, value, ...]. */
typedef struct {
    enum ndt_attr_tag tag;
    char *name;
    union {
        char *AttrValue;
        struct {
            int64_t len;
            char **items;
        } AttrList;
    };
} ndt_attr_t;

typedef struct {
   const int min;
   const int max;
   const char *names[MAX_ATTR];
   const enum ndt_attr tags[MAX_ATTR];
} attr_spec;

typedef struct {
    int64_t len;
    int64_t reserved;
    ndt_attr_t *ptr;
} ndt_attr_seq_t;

void ndt_attr_del(ndt_attr_t *attr);
void ndt_attr_array_del(ndt_attr_t *attr, int64_t nattr);

ndt_attr_seq_t *ndt_attr_seq_new(ndt_attr_t *, ndt_context_t *ctx);
void ndt_attr_seq_del(ndt_attr_seq_t *);
ndt_attr_seq_t *ndt_attr_seq_append(ndt_attr_seq_t *, ndt_attr_t *, ndt_context_t *ctx);
ndt_attr_seq_t *ndt_attr_seq_finalize(ndt_attr_seq_t *);

int ndt_parse_attr(const attr_spec *spec, ndt_context_t *ctx, const ndt_attr_seq_t *seq, ...);


#endif
