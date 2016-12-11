#ifndef TEST_H
#define TEST_H


#include "ndtypes.h"


typedef struct {
    const char *pattern;
    const char *candidate;
    int expected;
} match_testcase_t;

extern const match_testcase_t match_tests[];


#endif /* TEST_H */
