#ifndef SRC_UTIL_COMMON_H_
#define SRC_UTIL_COMMON_H_


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/attrs.h"
#include "util/builtin-portability.h"
#include "util/macro.h"
#include "util/portability.h"
#include "util/types.h"


#ifdef __cplusplus
#include <type_traits>
#endif


#define ALIGNED(x) I_attr_aligned((x))

#define UNUSED(x) ((void)(x))

#define LIKELY(x)   I_builtin_expect(!!(x), 1)
#define UNLIKELY(x) I_builtin_expect((x), 0)

#define IMPOSSIBLE(x)                                                          \
    if (x) {                                                                   \
        I_builtin_unreachable();                                               \
    }

#define GURANTEED(x) IMPOSSIBLE(!(x))


#define const_assert(...)                                                      \
    CAT(I_const_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)
#define I_const_assert_ONE(x)       I_static_assert(x, "")
#define I_const_assert_MANY(x, ...) I_static_assert(x, __VA_ARGS__)

#define choose_const_expr I_builtin_choose_expr
#define is_const(...)     I_builtin_constant_p(__VA_ARGS__)
#define check_has_include I_has_include

#define ensure_const_eval(expr)                                                \
    {                                                                          \
        enum { CAT(CHECKING_CONSTANT_EVAL, I_COUNTER) = (expr) };              \
    }

#define const_condition(...) (is_const(__VA_ARGS__) && (__VA_ARGS__))

#define I_UNIQUE_TMP_VAR CAT_BASE(I_unique_tmp_var_, I_COUNTER)

#endif
