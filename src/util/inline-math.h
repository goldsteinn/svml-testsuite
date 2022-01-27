#ifndef _SRC__UTIL__INLINE_MATH_H_
#define _SRC__UTIL__INLINE_MATH_H_


#include "util/common.h"


#define ROUNDUP(x, y)   ((y) * (((x) + ((y)-1)) / (y)))
#define ROUNDDOWN(x, y) ((y) * ((x) / (y)))

#define ROUNDUP_P2(x, y)   (((x) + ((y)-1)) & (-(y)))
#define ROUNDDOWN_P2(x, y) ((x) & (-(y)))

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define is_p2(x) (!((x) & ((x)-1)))
#define next_p2(x)                                                             \
    ({                                                                         \
        typeof(UNSIGNED(x)) _next_p2_tmp_ = (x);                               \
        /* If constant compiler can calculate this version at compile          \
         * time. */                                                            \
        if (__builtin_constant_p(_next_p2_tmp_)) {                             \
            --_next_p2_tmp_;                                                   \
            _next_p2_tmp_ |= _next_p2_tmp_ >> 1;                               \
            _next_p2_tmp_ |= _next_p2_tmp_ >> 2;                               \
            _next_p2_tmp_ |= _next_p2_tmp_ >> 4;                               \
            _next_p2_tmp_ |= _next_p2_tmp_ >> 8;                               \
            _next_p2_tmp_ |= _next_p2_tmp_ >> 16;                              \
            if (sizeof(x) == 8) {                                              \
                _next_p2_tmp_ |= _next_p2_tmp_ >> (4 * sizeof(x));             \
            }                                                                  \
            ++_next_p2_tmp_;                                                   \
        }                                                                      \
        else {                                                                 \
            /* Dont see any clever way to drop the branch. */                  \
            if (UNLIKELY(_next_p2_tmp_ <= 2)) {                                \
                ;                                                              \
            }                                                                  \
            else if (sizeof(x) == sizeof(long long)) {                         \
                _next_p2_tmp_ = (2UL << ((8 * sizeof(long long) - 1) -         \
                                         __builtin_clzll(_next_p2_tmp_ - 1))); \
            }                                                                  \
            else if (sizeof(x) == sizeof(long)) {                              \
                _next_p2_tmp_ = (2UL << ((8 * sizeof(long) - 1) -              \
                                         __builtin_clz(_next_p2_tmp_ - 1)));   \
            }                                                                  \
            else {                                                             \
                _next_p2_tmp_ =                                                \
                    CAST(uint32_t,                                             \
                         (2UL << (31 - __builtin_clz(_next_p2_tmp_ - 1))));    \
            }                                                                  \
        }                                                                      \
        _next_p2_tmp_;                                                         \
    })


#define prev_p2(x)                                                             \
    ({                                                                         \
        typeof(UNSIGNED(x)) _prev_p2_tmp_ = (x);                               \
        /* If constant compiler can calculate this version at compile          \
         * time. */                                                            \
        if (__builtin_constant_p(_prev_p2_tmp_)) {                             \
            _prev_p2_tmp_ |= _prev_p2_tmp_ >> 1;                               \
            _prev_p2_tmp_ |= _prev_p2_tmp_ >> 2;                               \
            _prev_p2_tmp_ |= _prev_p2_tmp_ >> 4;                               \
            _prev_p2_tmp_ |= _prev_p2_tmp_ >> 8;                               \
            _prev_p2_tmp_ |= _prev_p2_tmp_ >> 16;                              \
            if (sizeof(x) == 8) {                                              \
                _prev_p2_tmp_ |= _prev_p2_tmp_ >> (4 * sizeof(x));             \
            }                                                                  \
            _prev_p2_tmp_ -= (_prev_p2_tmp_ >> 1);                             \
        }                                                                      \
        else {                                                                 \
            if (sizeof(x) == sizeof(long long)) {                              \
                _prev_p2_tmp_ &= (1UL << ((8 * sizeof(long long) - 1) -        \
                                          __builtin_clzll(_prev_p2_tmp_)));    \
            }                                                                  \
            else if (sizeof(x) == sizeof(long)) {                              \
                _prev_p2_tmp_ &= (1UL << ((8 * sizeof(long) - 1) -             \
                                          __builtin_clz(_prev_p2_tmp_)));      \
            }                                                                  \
            else {                                                             \
                _prev_p2_tmp_ &=                                               \
                    (1UL << (31 -                                              \
                             __builtin_clz(CAST(uint32_t, _prev_p2_tmp_))));   \
            }                                                                  \
        }                                                                      \
        _prev_p2_tmp_;                                                         \
    })

#endif
