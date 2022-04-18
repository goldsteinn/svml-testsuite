#ifndef _SRC__UTIL__TYPES_H_
#define _SRC__UTIL__TYPES_H_

#include <limits.h>
#include <stdint.h>
#include <wchar.h>

#include "util/portability.h"

/* Use safe_<type> for aliasing casts. */
typedef wchar_t  safe_wchar_t __attribute__((may_alias));
typedef uint32_t safe_uint32_t __attribute__((may_alias, aligned(1)));
typedef int32_t  safe_int32_t __attribute__((may_alias, aligned(1)));
typedef uint64_t safe_uint64_t __attribute__((may_alias, aligned(1)));
typedef int64_t  safe_int64_t __attribute__((may_alias, aligned(1)));

typedef char               safe_char __attribute__((may_alias, aligned(1)));
typedef unsigned char      safe_uchar __attribute__((may_alias, aligned(1)));
typedef short              safe_short __attribute__((may_alias, aligned(1)));
typedef unsigned short     safe_ushort __attribute__((may_alias, aligned(1)));
typedef int                safe_int __attribute__((may_alias, aligned(1)));
typedef unsigned int       safe_uint __attribute__((may_alias, aligned(1)));
typedef long               safe_long __attribute__((may_alias, aligned(1)));
typedef unsigned long      safe_ulong __attribute__((may_alias, aligned(1)));
typedef long long          safe_llong __attribute__((may_alias, aligned(1)));
typedef unsigned long long safe_ullong __attribute__((may_alias, aligned(1)));
typedef float              safe_float __attribute__((may_alias, aligned(1)));
typedef double             safe_double __attribute__((may_alias, aligned(1)));
typedef float              safe_float __attribute__((may_alias, aligned(1)));
typedef long double        safe_ldouble __attribute__((may_alias, aligned(1)));

typedef uint64_t ptr_int_t;

#ifndef __cplusplus
typedef int32_t bool;
static const bool false = 0;
static const bool true  = !false;
#endif

#define sizeof_bits(x) (sizeof(x) * CHAR_BIT)

#define CAST_TO_FUNC(func, x) ((FUNC_T(func))(x))
#define FUNC_T(func)          __typeof__(&(func))

#define CAST(x, y)          ((x)(y))
#define AGU(base, offset)   (CAST(ptr_int_t, base) + CAST(ptr_int_t, offset))
#define AGU_T(base, offset) CAST(get_type(base), AGU(base, offset))


#define get_type(x)        __typeof__(x)
#define is_same_type(x, y) I_is_same_type(get_type(x), get_type(y))

#define is_same_func_type(decl, func) is_same_type(decl, &(func))


/* GCC does not const evaluate `__builtin_classify_type` if `x` is void.  */
#define I_AS_NOT_VOID(x)                                                       \
    (I_choose_const_expr(I_is_same_type(get_type(x), void), 0, (x)))

/* GCC / LLVM only.  */
#define IS_PTR(x)                                                              \
    (__builtin_classify_type(I_AS_NOT_VOID(x)) == 5 /* 5 is ptr type.  */)

/* Use to make derefencing never a compiler error/warning.  */
#define I_AS_PTR(x) (I_choose_const_expr(IS_PTR(x), (x), ((void **)(NULL))) + 0)

#ifdef __cplusplus
#include <type_traits>

#define I_PTR_TYPE(x)                                                          \
    typename std::conditional<std::is_pointer<get_type(x)>::value,             \
                              get_type(x), void **>::type


#define sizeof_deref(x)        sizeof(*CAST(I_PTR_TYPE(x), x))
#define I_is_same_type(T0, T1) std::is_same<T0, T1>::value


#define MAKE_UNSIGNED(x) CAST(std::make_unsigned<get_type(x)>::type, x)
#define MAKE_SIGNED(x)   CAST(std::make_signed<get_type(x)>::type, x)

#define MAKE_UNSIGNED_PTR(x) UNSIGNED(x)
#define MAKE_SIGNED_PTR(x)   SIGNED(x)

#define IS_SIGNED(x) std::is_signed<get_type(x)>::value
#define IS_UNSIGNED_INT(x)                                                     \
    (std::is_integral<get_type(x)>::value &&                                   \
     std::is_unsigned<get_type(x)>::value)

#else

/* Use to make derefencing never a compiler error/warning.  */
#define I_AS_PTR(x) (I_choose_const_expr(IS_PTR(x), (x), ((void **)(NULL))) + 0)

#define sizeof_deref(x)        sizeof(*(x))
#define I_is_same_type(T0, T1) __builtin_types_compatible_p(T0, T1)


#if STDC_VERSION >= 2011
// clang-format off
#define MAKE_UNSIGNED(x)                            \
    _Generic((x),                                   \
             char       : (unsigned char)(x),       \
             signed char: (unsigned char)(x),       \
             int        : (unsigned int)(x),        \
             short      : (unsigned short)(x),      \
             long       : (unsigned long)(x),       \
             long long  : (unsigned long long)(x),  \
             default    : x)

#define MAKE_UNSIGNED_PTR(x)                                \
    _Generic((x),                                           \
             char *         : (unsigned char *)(x),         \
             signed char *  : (unsigned char *)(x),         \
             int *          : (unsigned int *)(x),          \
             short *        : (unsigned short *)(x),        \
             long *         : (unsigned long *)(x),         \
             long long *    : (unsigned long long *)(x),    \
             default        : x)

#define MAKE_SIGNED(x)                          \
    _Generic((x),                               \
             unsigned char      : (char) (x),   \
             unsigned char      : (signed) (x), \
             unsigned int       : (int) (x),    \
             unsigned short     : (short) (x),  \
             unsigned long      : (long) (x),   \
             unsigned long long : (long) (x),   \
             default            : x)

#define MAKE_SIGNED_PTR(x)                              \
    _Generic((x),                                       \
             unsigned char *        : (char *) (x),     \
             unsigned char *        : (signed *) (x),   \
             unsigned int *         : (int *) (x),      \
             unsigned short *       : (short *) (x),    \
             unsigned long *        : (long *) (x),     \
             unsigned long long *   : (long *) (x),     \
             default                : x)

#define IS_SIGNED(x)                            \
    _Generic((x),                               \
             unsigned char          : 0,        \
             unsigned short         : 0,        \
             unsigned int           : 0,        \
             unsigned long          : 0,        \
             unsigned long long     : 0,        \
             signed char            : 1,        \
             signed short           : 1,        \
             signed int             : 1,        \
             signed long            : 1,        \
             signed long long       : 1,        \
             char                   : 1,        \
             default                : 0)


#define IS_UNSIGNED_INT(x)                      \
    _Generic((x),                               \
             unsigned char          : 1,        \
             unsigned short         : 1,        \
             unsigned int           : 1,        \
             unsigned long          : 1,        \
             unsigned long long     : 1,        \
             signed char            : 0,        \
             signed short           : 0,        \
             signed int             : 0,        \
             signed long            : 0,        \
             signed long long       : 0,        \
             char                   : 0,        \
             default                : 0)
// clang-format on


#else
#error "Older standard unsupported"

#define I_SIGN_PROMOTE(A, B) (1 ? (A) : (B))
#define I_PROMOTE_1(EXPR)    I_SIGN_PROMOTE(1, (EXPR))
#define I_PROMOTE_M1(EXPR)   I_SIGN_PROMOTE(-1, (EXPR))
#define IS_SIGNED(EXPR)      (I_PROMOTE_M1(EXPR) < I_PROMOTE_1(EXPR))

#endif

#endif
#define IS_UNSIGNED(x) (!IS_SIGNED(x))

#endif
