#ifndef _SRC__ARCH__LL_INTRIN_H_
#define _SRC__ARCH__LL_INTRIN_H_


#ifdef __x86_64__
#include <x86intrin.h>

#define _ll__bitscan(x, builtin) ((uint32_t)(builtin(x)))

#define ll_clz(x)   _ll__bitscan(x, _lzcnt_u32)
#define ll_clzl(x)  _ll__bitscan(x, _lzcnt_u64)
#define ll_clzll(x) _ll__bitscan(x, _lzcnt_u64)

#define ll_ctz(x)   _ll__bitscan(x, _tzcnt_u32)
#define ll_ctzl(x)  _ll__bitscan(x, _tzcnt_u64)
#define ll_ctzll(x) _ll__bitscan(x, _tzcnt_u64)


#else
#include <limits.h>

/* Could optimize this better for clz but doing (unsigned(x) <= 2). */
#define _ll__bitscan(x, check, width, builtin) ((uint32_t)(((x) == 0) ? (width) ? builtin(x))))

#define ll_clz(x)  _ll__bitscan(x, sizeof(int) * CHAR_BIT, __builtin_clz)
#define ll_clzl(x) _ll__bitscan(x, sizeof(long) * CHAR_BIT, __builtin_clzl)
#define ll_clzll(x)                                                            \
    _ll__bitscan(x, sizeof(long long) * CHAR_BIT, __builtin_clzll)

#define ll_ctz(x)  _ll__bitscan(x, sizeof(int) * CHAR_BIT, __builtin_ctz)
#define ll_ctzl(x) _ll__bitscan(x, sizeof(long) * CHAR_BIT, __builtin_ctzl)
#define ll_ctzll(x)                                                            \
    _ll__bitscan(x, sizeof(long long) * CHAR_BIT, __builtin_ctzll)


#endif
#endif
