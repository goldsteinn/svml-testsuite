#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdlib.h>
#include "util/macro.h"

typedef int32_t (*test_f)();

typedef struct test {
    const char * name;
    const test_f func;
} test_t;

#define make_test(name)                                                        \
    { V_TO_STR(name), &name }


extern int32_t test_p2();

static const test_t tests[] = { make_test(test_p2) };
static const size_t ntests  = sizeof(tests) / sizeof(test_t);

#endif
