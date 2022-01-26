#ifndef SRC_D_ARCH_D_LL_INTRIN_H_
#define SRC_D_ARCH_D_LL_INTRIN_H_

#include "util/builtin-portability.h"

#ifdef __x86_64__
#include <x86intrin.h>

#define I_ll_intrin_(x, builtin) ((uint32_t)(builtin(x)))

#define ll_clz(x)   I_ll_intrin_(x, _lzcnt_u32)
#define ll_clzl(x)  I_ll_intrin_(x, _lzcnt_u64)
#define ll_clzll(x) I_ll_intrin_(x, _lzcnt_u64)

#define ll_ctz(x)   I_ll_intrin_(x, _tzcnt_u32)
#define ll_ctzl(x)  I_ll_intrin_(x, _tzcnt_u64)
#define ll_ctzll(x) I_ll_intrin_(x, _tzcnt_u64)

#else
#include <limits.h>

/* Could optimize this better for clz but doing (unsigned(x) <= 2). */
#define I_ll_bitscan_(x, check, width, builtin) ((uint32_t)(((x) == 0) ? (width) ? builtin(x))))

#define ll_clz(x)  I_ll_bitscan_(x, sizeof(int) * CHAR_BIT, I_builtin_clz)
#define ll_clzl(x) I_ll_bitscan_(x, sizeof(long) * CHAR_BIT, I_builtin_clzl)
#define ll_clzll(x)                                                            \
    I_ll_bitscan_(x, sizeof(long long) * CHAR_BIT, I_builtin_clzll)

#define ll_ctz(x)  I_ll__bitscan_(x, sizeof(int) * CHAR_BIT, I_builtin_ctz)
#define ll_ctzl(x) I_ll__bitscan_(x, sizeof(long) * CHAR_BIT, I_builtin_ctzl)
#define ll_ctzll(x)                                                            \
    I_ll_bitscan_(x, sizeof(long long) * CHAR_BIT, I_builtin_ctzll)


#endif
#endif
