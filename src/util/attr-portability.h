#ifndef _SRC__UTIL__ATTR_PORTABILITY_H_
#define _SRC__UTIL__ATTR_PORTABILITY_H_

#include "util/portability.h"

/* Generated
 *	By: attr-checks.py
 *	On: 2022-09-02 09:37:40
 */

#if I_has_attr(noclone)
#define I_attr_noclone __attribute__((noclone))
#else
#define I_attr_noclone
#endif

#if I_has_attr(always_inline)
#define I_attr_always_inline __attribute__((always_inline))
#else
#define I_attr_always_inline
#endif

#if I_has_attr(noinline)
#define I_attr_noinline __attribute__((noinline))
#else
#define I_attr_noinline
#endif

#if I_has_attr(malloc) && I_has_attr(alloc_size)
#define I_attr_malloc(...) __attribute__((malloc, alloc_size(__VA_ARGS__)))
#else
#define I_attr_malloc(...)
#endif

#if I_has_attr(const)
#define I_attr_const __attribute__((const))
#else
#define I_attr_const
#endif

#if I_has_attr(pure)
#define I_attr_pure __attribute__((pure))
#else
#define I_attr_pure
#endif

#if I_has_attr(unused)
#define I_attr_unused __attribute__((unused))
#else
#define I_attr_unused
#endif

#if I_has_attr(tls_model)
#define I_attr_tls_model(...) __attribute__((tls_model(__VA_ARGS__)))
#else
#define I_attr_tls_model(...)
#endif

#if I_has_attr(cold)
#define I_attr_cold __attribute__((cold))
#else
#define I_attr_cold
#endif

#if I_has_attr(noreturn)
#define I_attr_noreturn __attribute__((noreturn))
#else
#define I_attr_noreturn
#endif

#if I_has_attr(aligned)
#define I_attr_aligned(...) __attribute__((aligned(__VA_ARGS__)))
#else
#define I_attr_aligned(...)
#endif

#if I_has_attr(nonnull)
#define I_attr_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define I_attr_nonnull(...)
#endif

#if I_has_attr(format)
#define I_attr_format(...) __attribute__((format(__VA_ARGS__)))
#else
#define I_attr_format(...)
#endif

#if I_has_attr(optimize)
#define I_attr_optimize(...) __attribute__((optimize(__VA_ARGS__)))
#else
#define I_attr_optimize(...)
#endif

#if I_has_attr(copy)
#define I_attr_copy(...) __attribute__((copy(__VA_ARGS__)))
#else
#define I_attr_copy(...)
#endif


#endif
