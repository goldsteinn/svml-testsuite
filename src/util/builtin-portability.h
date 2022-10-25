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

#if I_has_builtin(__builtin_prefetch)
#define I_builtin_prefetch(...) __builtin_prefetch(__VA_ARGS__)
#else
#define I_builtin_prefetch(...)
#endif

#if I_has_builtin(__builtin_memcpy)
#define I_builtin_memcpy(...) __builtin_memcpy(__VA_ARGS__)
#else
#define I_builtin_memcpy(...) memcpy_c(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memset)
#define I_builtin_memset(...) __builtin_memset(__VA_ARGS__)
#else
#define I_builtin_memset(...) memset_c(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memcmp)
#define I_builtin_memcmp(...) __builtin_memcmp(__VA_ARGS__)
#else
#define I_builtin_memcmp(...) memcmp_c(__VA_ARGS__)
#endif

#if I_has_builtin(__builtin_memcmp_eq)
#define I_builtin_memcmp_eq(...) __builtin_memcmp_eq(__VA_ARGS__)
#else
#define I_builtin_memcmp_eq(...) (memcmpeq_c(__VA_ARGS__))
#endif

#define I_builtin_ptr_type 5
#if I_has_builtin(__builtin_classify_type)
#define I_builtin_classify_type(...) __builtin_classify_type(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif


#if I_has_builtin(__builtin_ctz) && I_has_builtin(__builtin_ctzl) &&           \
    I_has_builtin(__builtin_ctzll)
#define I_builtin_ctz(...)   __builtin_ctz(__VA_ARGS__)
#define I_builtin_ctzl(...)  __builtin_ctzl(__VA_ARGS__)
#define I_builtin_ctzll(...) __builtin_ctzll(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif

#if I_has_builtin(__builtin_clz) && I_has_builtin(__builtin_clzl) &&           \
    I_has_builtin(__builtin_clzll)
#define I_builtin_clz(...)   __builtin_clz(__VA_ARGS__)
#define I_builtin_clzl(...)  __builtin_clzl(__VA_ARGS__)
#define I_builtin_clzll(...) __builtin_clzll(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif

#endif
