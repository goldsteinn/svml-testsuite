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

#define decl_test(name) extern int32_t name();

#define make_tests(...)                                                        \
    APPLY(decl_test, ;, __VA_ARGS__);                                          \
    static const test_t tests[] = { APPLY(make_test, COMMA, __VA_ARGS__) };    \
    static const size_t ntests  = sizeof(tests) / sizeof(test_t);


/* Append all here. */
#define TESTS test_p2, test_random

make_tests(TESTS);


#endif
