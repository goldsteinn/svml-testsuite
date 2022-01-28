#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/commonlib.h"
#include "util/types.h"

#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE  __attribute__((noinline))

#define CONST_FUNC __attribute__((const))
#define PURE_FUNC  __attribute__((const))

#define ALIGNED(x) __attribute__((aligned(x)))

#define UNUSED(x) ((void)(x))

/* NOLINT to suppress warning from clang not supporting noclone attribute. */
#define BENCH_FUNC                                                             \
    __attribute__((noclone /* NOLINT */, noinline, aligned(4096)))
#define EXIT_FUNC                                                              \
    __attribute__((noclone /* NOLINT */, noinline, cold, noreturn))

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#define IMPOSSIBLE(x)                                                          \
    if (x) {                                                                   \
        __builtin_unreachable();                                               \
    }


#endif
