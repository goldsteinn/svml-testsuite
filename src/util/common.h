#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/commonlib.h"
#include "util/attrs.h"
#include "util/portability.h"
#include "util/types.h"

#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE  __attribute__((noinline))

#define ALIGNED(x) __attribute__((aligned(x)))

#define UNUSED(x) ((void)(x))

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#define IMPOSSIBLE(x)                                                          \
    if (x) {                                                                   \
        __builtin_unreachable();                                               \
    }


#define _static_assert(...)                                                    \
    CAT(_static_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)
#define _static_assert_ONE(x)       _static_assert_base(x, "")
#define _static_assert_MANY(x, ...) _static_assert_base(x, __VA_ARGS__)

#endif
