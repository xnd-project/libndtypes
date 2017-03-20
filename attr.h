#ifndef ATTR_H
#define ATTR_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ndtypes.h"
#include "seq.h"


#define MAX_ATTR 8

typedef struct {
   const size_t min;
   const size_t max;
   const char *names[MAX_ATTR];
   const enum ndt tags[MAX_ATTR];
} attr_spec;

const attr_spec *ndt_get_attr_spec(enum ndt tag, ndt_context_t *ctx);
int ndt_parse_attr(enum ndt tag, ndt_context_t *ctx, ndt_attr_seq_t *seq, ...);


#endif
