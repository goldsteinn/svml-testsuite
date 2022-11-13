#ifndef SRC_D_UTIL_D_INLINE_MATH_H_
#define SRC_D_UTIL_D_INLINE_MATH_H_


#include "arch/ll-intrin.h"
#include "util/common.h"


#define ROUNDUP(x, y)   ((y) * (((x) + ((y)-1)) / (y)))
#define ROUNDDOWN(x, y) ((y) * ((x) / (y)))

#define ROUNDUP_P2(x, y)   (((x) + ((y)-1)) & (-(y)))
#define ROUNDDOWN_P2(x, y) ((x) & (-(y)))

#define lsb(x)       ((x) & (-(x)))
#define is_p2(x)     (!((x) & ((x)-1)))
#define p2_factor(x) ((x) & (~((x)-1)))

#define I_cmax2(x, y) ((x) > (y) ? (x) : (y))
#define I_cmin2(x, y) ((x) < (y) ? (x) : (y))

#define I_max2_impl(x, y, tmpx, tmpy)                                          \
    ({                                                                         \
        get_type(x) tmpx = (x);                                                \
        get_type(y) tmpy = (y);                                                \
        tmpx > tmpy ? tmpx : tmpy;                                             \
    })


#define I_min2_impl(x, y, tmpx, tmpy)                                          \
    ({                                                                         \
        get_type(x) tmpx = (x);                                                \
        get_type(y) tmpy = (y);                                                \
        tmpx < tmpy ? tmpx : tmpy;                                             \
    })

#define I_max2(x, y) I_max2_impl(x, y, I_UNIQUE_TMP_VAR, I_UNIQUE_TMP_VAR)
#define I_min2(x, y) I_min2_impl(x, y, I_UNIQUE_TMP_VAR, I_UNIQUE_TMP_VAR)


#define MAX(...) APPLY_RECURSE(I_max2, __VA_ARGS__)
#define MIN(...) APPLY_RECURSE(I_min2, __VA_ARGS__)

#define CMAX(...) APPLY_RECURSE(I_cmax2, __VA_ARGS__)
#define CMIN(...) APPLY_RECURSE(I_cmin2, __VA_ARGS__)


#define clz(x)                                                                 \
    choose_const_expr(sizeof(x) >= sizeof(long long), ll_clzll(x),             \
                      choose_const_expr(sizeof(x) >= sizeof(long), ll_clzl(x), \
                                        ll_clz(CAST(unsigned, x))))

#define ctz(x)                                                                 \
    choose_const_expr(sizeof(x) == sizeof(long long), ll_ctzll(x),             \
                      choose_const_expr(sizeof(x) >= sizeof(long), ll_ctzl(x), \
                                        ll_ctz(cast(unsigned, x))))


#define popcnt(x)                                                              \
    choose_const_expr(                                                         \
        sizeof(x) >= sizeof(long long), __builtin_popcountll(x),               \
        choose_const_expr(sizeof(x) >= sizeof(long), __builtin_popcountl(x),   \
                          __builtin_popcount(x)))

#define log2_ru(x) (sizeof_bits(get_type(x)) - clz((x)-1))
#define log2_rd(x) ((sizeof_bits(get_type(x)) - 1) - clz(x))

#define I_const_eval_log2_64(s)                                                \
    (((s)&0xffffffff00000000) ? (32 + I_const_eval_log2_32((s) >> 32))         \
                              : (I_const_eval_log2_32(s)))
#define I_const_eval_log2_32(s)                                                \
    (((s)&0xffff0000) ? (16 + I_const_eval_log2_16((s) >> 16))                 \
                      : (I_const_eval_log2_16(s)))
#define I_const_eval_log2_16(s)                                                \
    (((s)&0xff00) ? (8 + I_const_eval_log2_8((s) >> 8))                        \
                  : (I_const_eval_log2_8(s)))
#define I_const_eval_log2_8(s)                                                 \
    (((s)&0xf0) ? (4 + I_const_eval_log2_4((s) >> 4))                          \
                : (I_const_eval_log2_4(s)))
#define I_const_eval_log2_4(s)                                                 \
    (((s)&0xc) ? (2 + I_const_eval_log2_2((s) >> 2)) : (I_const_eval_log2_2(s)))
#define I_const_eval_log2_2(s) (((s)&0x2) ? (1) : (0))

#define const_eval_next_p2(s) (1UL << (I_const_eval_log2_64((s)-1) + 1))

#define next_p2(x)                                                             \
    ({                                                                         \
        get_type(x) I_tmp_evaluated_x = (x);                                   \
        get_type(MAKE_UNSIGNED(I_tmp_evaluated_x)) I_tmp_next_p2_;             \
        I_tmp_next_p2_ = CAST(get_type(I_tmp_next_p2_), I_tmp_evaluated_x);    \
        /* If constant compiler can calculate this version at compile          \
         * time. */                                                            \
        if (__builtin_constant_p(I_tmp_next_p2_)) {                            \
            --I_tmp_next_p2_;                                                  \
            I_tmp_next_p2_ |= I_tmp_next_p2_ >> 1;                             \
            I_tmp_next_p2_ |= I_tmp_next_p2_ >> 2;                             \
            I_tmp_next_p2_ |= I_tmp_next_p2_ >> 4;                             \
            I_tmp_next_p2_ |= I_tmp_next_p2_ >> 8;                             \
            I_tmp_next_p2_ |= I_tmp_next_p2_ >> 16;                            \
            if (sizeof(I_tmp_evaluated_x) == 8) {                              \
                I_tmp_next_p2_ |=                                              \
                    I_tmp_next_p2_ >> (4 * sizeof(I_tmp_evaluated_x));         \
            }                                                                  \
            ++I_tmp_next_p2_;                                                  \
        }                                                                      \
        else {                                                                 \
            /* Dont see any clever way to drop the branch. */                  \
            if (UNLIKELY(I_tmp_next_p2_ <= 2)) {                               \
                ;                                                              \
            }                                                                  \
            else {                                                             \
                I_tmp_next_p2_ =                                               \
                    (CAST(get_type(I_tmp_next_p2_), 2)                         \
                     << ((sizeof_bits(get_type(I_tmp_next_p2_)) - 1) -         \
                         clz(I_tmp_next_p2_ - 1)));                            \
            }                                                                  \
        }                                                                      \
        CAST(get_type(I_tmp_evaluated_x), I_tmp_next_p2_);                     \
    })


#define prev_p2(x)                                                             \
    ({                                                                         \
        get_type(x) I_tmp_evaluated_x = (x);                                   \
        get_type(MAKE_UNSIGNED(I_tmp_evaluated_x)) I_tmp_prev_p2_;             \
        I_tmp_prev_p2_ = CAST(get_type(I_tmp_prev_p2_), I_tmp_evaluated_x);    \
        /* If constant compiler can calculate this version at compile          \
         * time. */                                                            \
        if (__builtin_constant_p(I_tmp_prev_p2_)) {                            \
            I_tmp_prev_p2_ |= I_tmp_prev_p2_ >> 1;                             \
            I_tmp_prev_p2_ |= I_tmp_prev_p2_ >> 2;                             \
            I_tmp_prev_p2_ |= I_tmp_prev_p2_ >> 4;                             \
            I_tmp_prev_p2_ |= I_tmp_prev_p2_ >> 8;                             \
            I_tmp_prev_p2_ |= I_tmp_prev_p2_ >> 16;                            \
            if (sizeof(I_tmp_evaluated_x) == 8) {                              \
                I_tmp_prev_p2_ |=                                              \
                    I_tmp_prev_p2_ >> (4 * sizeof(I_tmp_evaluated_x));         \
            }                                                                  \
            I_tmp_prev_p2_ -= (I_tmp_prev_p2_ >> 1);                           \
        }                                                                      \
        else {                                                                 \
            I_tmp_prev_p2_ &=                                                  \
                (CAST(get_type(I_tmp_prev_p2_), 1)                             \
                 << ((sizeof_bits(get_type(I_tmp_prev_p2_)) - 1) -             \
                     clz(I_tmp_prev_p2_)));                                    \
        }                                                                      \
        CAST(get_type(I_tmp_evaluated_x), I_tmp_prev_p2_);                     \
    })

#endif
