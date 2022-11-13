#ifndef SRC_UTIL_ATTRS_H_
#define SRC_UTIL_ATTRS_H_

#include "util/attr-portability.h"
#include "util/macro.h"

#if I_has_attr(fallthrough)
# define fall_through __attribute__((fallthrough))
#elif STDCPP_VERSION >= 2017
# define fall_through [[fallthrough]]
#else
# define fall_through /* fall through.  */
#endif


#define ALWAYS_INLINE inline I_attr_always_inline
#define NEVER_INLINE  I_attr_noinline I_attr_noclone


#define MAYBE_UNUSED I_attr_unused

#define CONST_FUNC    I_attr_const constexpr
#define PURE_FUNC     I_attr_pure
#define OPTIMIZE_SIZE I_attr_optimize("-Os")

#define BENCH_FUNC I_attr_noinline I_attr_aligned(4096) I_attr_noclone
#define EXIT_FUNC                                                              \
    I_attr_cold I_attr_noreturn I_attr_noinline I_attr_noclone OPTIMIZE_SIZE


#define NONNULL(...) I_attr_nonnull(__VA_ARGS__)
#define FORMATF(...) I_attr_format(printf, ##__VA_ARGS__)

#define ATTR_COPY(...) I_attr_copy(__VA_ARGS__)

#define ATTR_TLS(...) I_attr_tls_model(__VA_ARGS__)

#define ATTR_TLS_INIT_EXEC ATTR_TLS("initial-exec")


#define SYS_MALLOC_FUNC(...)                                                   \
    I_attr_c_malloc(munmap, 1) I_attr_assume_aligned(4096)                     \
        I_attr_alloc_size(__VA_ARGS__)

#define MALLOC_FUNC_BASE(...) I_attr_c_malloc(__VA_ARGS__)

#define MALLOC_FUNC_COMMON(...)                                                \
    I_attr_malloc I_attr_assume_aligned(16) I_attr_alloc_size(__VA_ARGS__)

#define ALIGNED_ALLOC_FUNC(alignment, sz)                                      \
    I_attr_malloc I_attr_assume_aligned(16) I_attr_alloc_align(alignment)      \
        I_attr_alloc_size(sz)


#endif
