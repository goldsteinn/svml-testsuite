#ifndef _SRC__UTIL__TYPES_H_
#define _SRC__UTIL__TYPES_H_

#include <stdint.h>
#include <wchar.h>

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

#define CAST(x, y)        ((x)(y))
#define AGU(base, offset) (CAST(ptr_int_t, base) + CAST(ptr_int_t, idx))

#ifdef __cplusplus
#include <type_traits>
#define UNSIGNED(x) std::make_unsigned<typeof(x)>::type
#else
#define UNSIGNED(x)                                                            \
    _Generic((x), char                                                         \
             : (unsigned char)(x), signed char                                 \
             : (unsigned char)(x), int                                         \
             : (unsigned int)(x), short                                        \
             : (unsigned short)(x), long                                       \
             : (unsigned long)(x), long long                                   \
             : (unsigned long long)(x), default                                \
             : x)

#define get_type(x) __typeof__(x)
#define is_same_type(x, y)                                                     \
    __builtin_types_compatible_p(get_type(x), get_type(y))
#endif

#endif
