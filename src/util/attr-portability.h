#ifndef SRC_UTIL_ATTR_PORTABILITY_H_
#define SRC_UTIL_ATTR_PORTABILITY_H_

#include "util/portability.h"
/* Generated
 *	By: scripts/attr-checks.py
 *	On: 2022-10-20 19:45:39
 */

#if I_has_attr(noclone)
# define I_attr_noclone __attribute__((noclone))
#else
# define I_attr_noclone
#endif

#if I_has_attr(always_inline)
# define I_attr_always_inline __attribute__((always_inline))
#else
# define I_attr_always_inline
#endif

#if I_has_attr(noinline)
# define I_attr_noinline __attribute__((noinline))
#else
# define I_attr_noinline
#endif

#if I_has_attr(may_alias)
# define I_attr_may_alias __attribute__((may_alias))
#else
# define I_attr_may_alias
#endif

#if I_has_attr(malloc)
# define I_attr_malloc __attribute__((malloc))
# ifdef __clang__
#  define I_attr_c_malloc(...) I_attr_malloc
# else
#  define I_attr_c_malloc(...)                                                 \
            __attribute__((malloc, returns_nonnull, malloc(__VA_ARGS__)))
# endif
#else
# define I_attr_malloc
# define I_attr_c_malloc(...)
#endif

#if I_has_attr(const)
# define I_attr_const __attribute__((const))
#else
# define I_attr_const
#endif

#if I_has_attr(pure)
# define I_attr_pure __attribute__((pure))
#else
# define I_attr_pure
#endif

#if I_has_attr(unused)
# define I_attr_unused __attribute__((unused))
#else
# define I_attr_unused
#endif

#if I_has_attr(tls_model)
# define I_attr_tls_model(...) __attribute__((tls_model(__VA_ARGS__)))
#else
# define I_attr_tls_model(...)
#endif

#if I_has_attr(cold)
# define I_attr_cold __attribute__((cold))
#else
# define I_attr_cold
#endif

#if I_has_attr(noreturn)
# define I_attr_noreturn __attribute__((noreturn))
#else
# define I_attr_noreturn
#endif

#if I_has_attr(aligned)
# define I_attr_aligned(...) __attribute__((aligned(__VA_ARGS__)))
#else
# define I_attr_aligned(...)
#endif

#if I_has_attr(nonnull)
# define I_attr_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
#else
# define I_attr_nonnull(...)
#endif

#if I_has_attr(format)
# define I_attr_format(...) __attribute__((format(__VA_ARGS__)))
#else
# define I_attr_format(...)
#endif

#if I_has_attr(optimize)
# define I_attr_optimize(...) __attribute__((optimize(__VA_ARGS__)))
#else
# define I_attr_optimize(...)
#endif

#if I_has_attr(alloc_size)
# define I_attr_alloc_size(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
# define I_attr_alloc_size(...)
#endif

#if I_has_attr(alloc_align)
# define I_attr_alloc_align(...) __attribute__((alloc_align(__VA_ARGS__)))
#else
# define I_attr_alloc_align(...)
#endif

#if I_has_attr(assume_aligned)
# define I_attr_assume_aligned(...) __attribute__((assume_aligned(__VA_ARGS__)))
#else
# define I_attr_assume_aligned(...)
#endif


#if I_has_attr(error)
# define I_attr_error(...) __attribute__((error(__VA_ARGS__)))
#else
# define I_attr_error(...)
#endif


#endif
