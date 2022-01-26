#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdlib.h>
#include "util/macro.h"

typedef void (*test_f)();

typedef struct test {
    const char * name;
    const test_f func;
} test_t;

#define make_test(name)                                                        \
    { V_TO_STR(name), &name }


/* Replace NULL, NULL before running tests. */
static const test_t tests[] = { { NULL, NULL } };
static const size_t ntests  = sizeof(tests) / sizeof(test_t);

#endif
