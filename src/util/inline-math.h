#ifndef _INLINE_MATH_H_
#define _INLINE_MATH_H_

#include "util/common.h"

#define ROUNDUP(x, y)   ((y) * (((x) + ((y)-1)) / (y)))
#define ROUNDDOWN(x, y) ((y) * ((x) / (y)))

#define ROUNDUP_P2(x, y)   (((x) + ((y)-1)) & (-(y)))
#define ROUNDDOWN_P2(x, y) ((x) & (-(y)))

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

static ALWAYS_INLINE uint64_t
next_p2_64(uint64_t n) {
    /* If constant compiler can calculate this version at compile time. */
    if (__builtin_constant_p(n)) {
        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n |= n >> 32;
        return ++n;
    }
    else {
        /* Probably some fancy way to get rid of branch. */
        return UNLIKELY(n <= 2) ? n : (1UL << (64 - __builtin_clzll(n)));
    }
}

static ALWAYS_INLINE uint32_t
next_p2_32(uint32_t n) {
    /* If constant compiler can calculate this version at compile time. */
    if (__builtin_constant_p(n)) {
        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        return ++n;
    }
    else {
        /* Probably some fancy way to get rid of branch. */
        return UNLIKELY(n <= 2) ? n : (1u << (32 - __builtin_clz(n)));
    }
}

#endif
