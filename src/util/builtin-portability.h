#ifndef _SRC__UTIL__BUILTIN_PORTABILITY_H_
#define _SRC__UTIL__BUILTIN_PORTABILITY_H_

#include "portability.h"

#if I_has_builtin(__builtin_choose_expr)
#define I_builtin_choose_expr(...) __builtin_choose_expr(__VA_ARGS__)
#else
#define I_builtin_choose_expr(cond, expr_true, expr_false)                     \
    ((cond) ? (expr_true) : (expr_false))
#endif

#if I_has_builtin(__builtin_constant_p)
#define I_builtin_constant_p(...) __builtin_constant_p(__VA_ARGS__)
#else
#define I_builtin_constant_p(...) 0
#endif

#if I_has_builtin(__builtin_expect)
#define I_builtin_expect(cond, expect) __builtin_expect(cond, expect)
#else
#define I_builtin_expect(cond) (cond)
#endif

#if I_has_builtin(__builtin_unreachable)
#define I_builtin_unreachable() __builtin_unreachable()
#else
#define I_builtin_unreachable() abort()
#endif

#if I_has_builtin(__builtin_memcpy)
#define I_builtin_memcpy(...) __builtin_memcpy(__VA_ARGS__)
#else
#define I_builtin_memcpy(...) memcpy(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memset)
#define I_builtin_memset(...) __builtin_memset(__VA_ARGS__)
#else
#define I_builtin_memset(...) memset(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memcmp)
#define I_builtin_memcmp(...) __builtin_memcmp(__VA_ARGS__)
#else
#define I_builtin_memcmp(...) memcmp(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memcmp_eq)
#define I_builtin_memcmp_eq(...) __builtin_memcmp_eq(__VA_ARGS__)
#else
#define I_builtin_memcmp_eq(...) (!!memcmp(__VA_ARGS__))
#endif
#endif
