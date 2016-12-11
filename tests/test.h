#ifndef TEST_H
#define TEST_H


#include "ndtypes.h"


typedef struct {
    const char *pattern;
    const char *candidate;
    int expected;
} match_testcase_t;

extern const char *parse_tests[];
extern const char *parse_roundtrip_tests[];
extern const char *parse_error_tests[];
extern const char *typedef_tests[];
extern const char *typedef_error_tests[];
extern const match_testcase_t match_tests[];


#endif /* TEST_H */
