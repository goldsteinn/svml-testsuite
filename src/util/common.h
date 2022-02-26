#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/commonlib.h"
#include "util/attrs.h"
#include "util/types.h"
#include "util/portability.h"

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


#endif
