#ifndef _SRC__UTIL__ATTRS_H_
#define _SRC__UTIL__ATTRS_H_

#include "util/attr-portability.h"
#include "util/macro.h"

#if I_has_attr(fallthrough)
#define I_fall_through __attribute__((fallthrough))
#elif STDCPP_VERSION >= 2017
#define I_fall_through [[fallthrough]]
#else
#define I_fall_through /* fall through.  */
#endif


#define ALWAYS_INLINE inline I_attr_always_inline
#define NEVER_INLINE  I_attr_noinline


#define MAYBE_UNUSED     I_attr_unused
#define MALLOC_FUNC(...) I_attr_malloc(__VA_ARGS__)
#define CONST_FUNC       I_attr_const constexpr
#define PURE_FUNC        I_attr_pure
#define OPTIMIZE_SIZE    I_attr_optimize("-Os")

#define BENCH_FUNC I_attr_noinline I_attr_aligned(4096) I_attr_noclone
#define EXIT_FUNC                                                              \
    I_attr_cold I_attr_noreturn I_attr_noinline I_attr_noclone OPTIMIZE_SIZE


#define NONNULL(...) I_attr_nonnull(__VA_ARGS__)
#define FORMATF(...) I_attr_format(printf, ##__VA_ARGS__)

#define ATTR_COPY(...) I_attr_copy(__VA_ARGS__)

#define ATTR_TLS(...) I_attr_tls_model(__VA_ARGS__)

#define ATTR_TLS_INIT_EXEC ATTR_TLS("initial-exec")


#endif
