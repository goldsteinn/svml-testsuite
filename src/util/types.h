#ifndef SRC_UTIL_TYPES_H_
#define SRC_UTIL_TYPES_H_

#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <wchar.h>

#include "util/attrs.h"
#include "util/builtin-portability.h"
#include "util/macro.h"
#include "util/portability.h"

#ifndef __cplusplus
typedef _Bool bool;
static const bool false = ((bool)0);
static const bool true  = ((bool)1);
#endif

typedef signed char      signed_char;
typedef signed short     signed_short;
typedef signed int       signed_int;
typedef signed long      signed_long;
typedef signed long long signed_long_long;
typedef long double      long_double;

typedef unsigned char      unsigned_char;
typedef unsigned short     unsigned_short;
typedef unsigned int       unsigned_int;
typedef unsigned long      unsigned_long;
typedef unsigned long long unsigned_long_long;

typedef long long long_long;


typedef signed_char *      signed_charp;
typedef signed_short *     signed_shortp;
typedef signed_int *       signed_intp;
typedef signed_long *      signed_longp;
typedef signed_long_long * signed_long_longp;
typedef long_double *      long_doublep;

typedef unsigned_char *      unsigned_charp;
typedef unsigned_short *     unsigned_shortp;
typedef unsigned_int *       unsigned_intp;
typedef unsigned_long *      unsigned_longp;
typedef unsigned_long_long * unsigned_long_longp;

typedef long_long * long_longp;

#define MISC_TYPES bool
#define SIGNED_INT_TYPES                                                       \
    char, signed_char, signed_short, signed_int, signed_long, signed_long_long
#define SIGNED_FLOAT_TYPES float, double, long_double

#define UNSIGNED_INT_TYPES                                                     \
    unsigned_char, unsigned_short, unsigned_int, unsigned_long,                \
        unsigned_long_long
#define UNSIGNED_FLOAT_TYPES

#define ALL_UNIQUE_INT_TYPES   SIGNED_INT_TYPES, UNSIGNED_INT_TYPES
#define ALL_UNIQUE_FLOAT_TYPES SIGNED_FLOAT_TYPES

#define ALL_UNIQUE_TYPES ALL_UNIQUE_INT_TYPES, SIGNED_FLOAT_TYPES, MISC_TYPES

#define ALL_INT_NAMES                                                          \
    wchar_t, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t,  \
        int64_t, short, int, long, long_long, ALL_UNIQUE_INT_TYPES

#define ALL_TYPE_NAMES ALL_INT_NAMES, ALL_UNIQUE_FLOAT_TYPES, MISC_TYPES


/* Use safe_<type> for aliasing casts. */
#define I_make_safe(T) typedef T CAT(safe_, T) I_attr_may_alias;
APPLY(I_make_safe, ;, ALL_TYPE_NAMES);


/* Any types pointer is Tp. */
#define I_make_ptr(T) typedef T * CAT(T, p)
APPLY(I_make_ptr, ;, ALL_TYPE_NAMES);

#define safe_T(T) CAT(safe_, T)
#define ptr_T(T)  CAT(T, p)


#define sizeof_bits(x) (sizeof(x) * CHAR_BIT)

#define CAST_TO_FUNC(func, x) ((FUNC_T(func))(x))
#define FUNC_T(func)          __typeof__(&(func))

#define CAST(x, y)          ((x)(y))
#define AGU(base, offset)   (CAST(uintptr_t, base) + CAST(uintptr_t, offset))
#define AGU_T(base, offset) CAST(get_type(base), AGU(base, offset))
#define PTRDIF(end, begin)  (CAST(uintptr_t, end) - CAST(uintptr_t, begin))


#define get_type(x)        __typeof__(x)
#define is_same_type(x, y) I_is_same_type(get_type(x), get_type(y))

#define is_same_func_type(decl, func) is_same_type(decl, &(func))


#ifdef __cplusplus


# include <limits>
# include <type_traits>

/* Custom struct instead of just using std::conditional in the macro
 * because the
 * ',' mess up macro parsing. */
template<typename T>
struct I_default_type {
    using type =
        typename std::conditional<std::is_void<T>::value, char, T>::type;
};

typedef
    typename std::make_unsigned<wchar_t>::type safe_uwchar_t I_attr_may_alias;


template<typename T>
struct I_type_info {
    static constexpr bool is_bool =
        std::is_same<T, bool>::value || std::is_same<T, safe_bool>::value;
    static constexpr bool is_boolp =
        std::is_same<T, bool *>::value || std::is_same<T, safe_bool *>::value;
    static constexpr bool is_void = std::is_void<T>::value;

    template<typename Q = T>
    using I_to_signed_T =
        typename std::conditional<std::is_integral<Q>::value && !is_bool &&
                                      !is_boolp,
                                  std::make_signed<Q>,
                                  I_default_type<Q>>::type::type;

    template<typename Q = T>
    using I_to_unsigned_T =
        typename std::conditional<std::is_integral<Q>::value && !is_bool &&
                                      !is_boolp,
                                  std::make_unsigned<Q>,
                                  I_default_type<Q>>::type::type;


    using I_signed_T   = I_to_signed_T<T>;
    using I_unsigned_T = I_to_unsigned_T<T>;

    using I_signed_ptr_T =
        typename std::conditional<std::is_pointer<T>::value,
                                  typename std::add_pointer<I_to_signed_T<
                                      typename std::remove_pointer<T>::type>>,
                                  I_default_type<T>>::type::type;


    using I_unsigned_ptr_T =
        typename std::conditional<std::is_pointer<T>::value,
                                  typename std::add_pointer<I_to_unsigned_T<
                                      typename std::remove_pointer<T>::type>>,
                                  I_default_type<T>>::type::type;


    using I_pass_T = typename std::conditional<sizeof(T) <= 16, T, T *>::type;
    using I_cpass_T =
        typename std::conditional<sizeof(T) <= 16, T const, T const *>::type;


    template<class Q = T>
    static constexpr typename std::enable_if<std::is_pointer<Q>::value, Q>::type
    as_ptr(Q x) {
        return x;
    }

    template<class Q = T>
    static constexpr
        typename std::enable_if<!std::is_pointer<Q>::value, void **>::type
        as_ptr(Q x MAYBE_UNUSED) {
        return (void **)(NULL);
    }
};

# define pass_T(T)  I_type_info<T>::I_pass_T
# define cpass_T(T) I_type_info<T>::I_cpass_T

# define DEFAULT_VALUE(T) CAST(I_AS_NOT_VOID_T(T), (T){})

# define IS_PTR(x) (std::is_pointer<get_type(x)>::value)

# define I_AS_NOT_VOID_T(T) typename I_default_type<T>::type

# define I_PTR_TYPE(x)                                                         \
        typename std::conditional<std::is_pointer<get_type(x)>::value,         \
                                  get_type(x), void **>::type


# define sizeof_deref(x)        sizeof(*CAST(I_PTR_TYPE(x), x))
# define I_is_same_type(T0, T1) std::is_same<T0, T1>::value


template<uint64_t N>
struct I_int_of_size {
    using by_byte_t = typename std::conditional<
        N <= 1,
        uint8_t,
        typename std::conditional<
            N <= 2,
            uint16_t,
            typename std::conditional<N <= 4, uint32_t, uint64_t>::type>::
            type>::type;

    using by_size_t = typename std::conditional<
        N <= UCHAR_MAX,
        uint8_t,
        typename std::conditional<
            N <= USHRT_MAX,
            uint16_t,
            typename std::conditional<N <= UINT_MAX, uint32_t, uint64_t>::
                type>::type>::type;
};


# define I_AS_PTR(x) (I_type_info<get_type(x)>::as_ptr(x))

# define INT_OF_SIZE_T(x)          I_int_of_size<x>::by_byte_t
# define INT_HAS_CAPACITY_FOR_T(x) I_int_of_size<x>::by_size_t


# define MAKE_UNSIGNED(x)                                                      \
        CAST(typename I_type_info<get_type(x)>::I_unsigned_T, x)

# define MAKE_SIGNED(x) CAST(typename I_type_info<get_type(x)>::I_signed_T, x)

# define MAKE_UNSIGNED_PTR(x)                                                  \
        CAST(typename I_type_info<get_type(x)>::I_unsigned_ptr_T               \
                 I_attr_may_alias,                                             \
             x)

# define MAKE_SIGNED_PTR(x)                                                    \
        CAST(typename I_type_info<get_type(x)>::I_signed_ptr_T                 \
                 I_attr_may_alias,                                             \
             x)


# define IS_SIGNED(x) std::is_signed<get_type(x)>::value
# define IS_UNSIGNED_INT(x)                                                    \
        (std::is_integral<get_type(x)>::value &&                               \
         std::is_unsigned<get_type(x)>::value)

# define IS_FLOAT_BASE(x) std::is_floating_point<get_type(x)>::value
# define IS_BOOL_BASE(x)  std::is_same<get_type(x), bool>::value
# define IS_INT_BASE(x)                                                        \
        (!IS_BOOL_BASE(x) && std::is_integral<get_type(x)>::value)

# define TYPEOF_MAX(x) std::numeric_limits<get_type((x))>::max()
# define TYPEOF_MIN(x) std::numeric_limits<get_type((x))>::min()

# define choose_T(cond, T0, T1)                                                \
        __typeof__(typename std::conditional<(cond), T0, T1>::type)

#else


# define choose_T(cond, T0, T1)                                                \
        get_type(choose_const_expr(cond, DEFAULT_VALUE(T0), DEFAULT_VALUE(T1)))


/* Use to make derefencing never a compiler error/warning.  */
# define I_AS_PTR(x) (choose_const_expr(IS_PTR(x), (x), ((void **)(NULL))))


/* GCC does not const evaluate `__builtin_classify_type` if `x` is void.
 */
# define I_AS_NOT_VOID(x)                                                      \
        (choose_const_expr(I_is_same_type(get_type(x), void), (char)0, (x)))

/* GCC / LLVM only.  */
# define IS_PTR(x)                                                             \
        (__builtin_classify_type(I_AS_NOT_VOID(x)) == 5 /* 5 is ptr type.  */)


# define DEFAULT_VALUE(T) (choose_const_expr(I_is_same_type(T, void), 0, (T){0}))

# define sizeof_deref(x)        sizeof(I_AS_NOT_VOID(*(x)))
# define I_is_same_type(T0, T1) __builtin_types_compatible_p(T0, T1)


# if I_STDC_VERSION >= 2011

#  define INT_OF_SIZE_T(x)                                                     \
            get_type(choose_const_expr(                                        \
                x == 1, (uint8_t)0,                                            \
                choose_const_expr(                                             \
                    x == 2, (uint16_t)0,                                       \
                    choose_const_expr(x == 4, (uint32_t)0, (uint64_t)0))))

#  define INT_HAS_CAPACITY_FOR_T(x)                                            \
            get_type(choose_const_expr(                                        \
                CAST(uint64_t, x) <= UCHAR_MAX, (uint8_t)0,                    \
                choose_const_expr(                                             \
                    CAST(uint64_t, x) <= USHRT_MAX, (uint16_t)0,               \
                    choose_const_expr(CAST(uint64_t, x) <= UINT_MAX,           \
                                      (uint32_t)0, (uint64_t)0))))


// clang-format off
#define MAKE_UNSIGNED(x)                                            \
    Generic((x),                                                    \
            char                       : (unsigned_char)(x),        \
            signed_char                : (unsigned_char)(x),        \
            signed_int                 : (unsigned_int)(x),         \
            signed_short               : (unsigned_short)(x),       \
            signed_long                : (unsigned_long)(x),        \
            signed_long_long           : (unsigned_long_long)(x),   \
            default                    : x)


#define MAKE_UNSIGNED_PTR(x)                                             \
    Generic((x),                                                         \
            char *                     : (safe_unsigned_char *)(x),      \
            signed_char *              : (safe_unsigned_char *)(x),      \
            signed_int *               : (safe_unsigned_int *)(x),       \
            signed_short *             : (safe_unsigned_short *)(x),     \
            signed_long *              : (safe_unsigned_long *)(x),      \
            signed_long_long *         : (safe_unsigned_long_long *)(x), \
            default                    : x)


#define MAKE_SIGNED(x)                                              \
    Generic((x),                                                    \
            unsigned_char              : (char) (x),                \
            unsigned_int               : (signed_int) (x),          \
            unsigned_short             : (signed_short) (x),        \
            unsigned_long              : (signed_long) (x),         \
            unsigned_long_long         : (signed_long_long) (x),    \
            default                    : x)


#define MAKE_SIGNED_PTR(x)                                               \
    Generic((x),                                                         \
            unsigned_char *            : (safe_char *) (x),              \
            unsigned_int *             : (safe_signed_int *) (x),        \
            unsigned_short *           : (safe_signed_short *) (x),      \
            unsigned_long *            : (safe_signed_long *) (x),       \
            unsigned_long_long *       : (safe_signed_long_long *) (x),  \
            default                    : x)

/* TODO: it might be easier to maintain if implement these with
 * APPLY macros based on type classes. */
#define IS_SIGNED(x)                            \
    Generic((x),                                \
            unsigned_char              : 0,     \
            unsigned_short             : 0,     \
            unsigned_int               : 0,     \
            unsigned_long              : 0,     \
            unsigned_long_long         : 0,     \
            signed_char                : 1,     \
            signed_short               : 1,     \
            signed_int                 : 1,     \
            signed_long                : 1,     \
            signed_long_long           : 1,     \
            char                       : 1,     \
            long_double                : 1,     \
            double                     : 1,     \
            float                      : 1,     \
            default                    : 0)


#define DEFAULT_HEX_FMT_TBL(before, x, after)             \
    Generic((x),                                \
            bool                       : before "0x%x" after,     \
            unsigned_char              : before "0x%hhx" after,     \
            unsigned_short             : before "0x%hx" after,     \
            unsigned_int               : before "0x%x" after,     \
            unsigned_long              : before "0x%lx" after,     \
            unsigned_long_long         : before "0x%llx" after,     \
            signed_char                : before "0x%hhx" after,     \
            signed_short               : before "0x%hx" after,     \
            signed_int                 : before "0x%x" after,     \
            signed_long                : before "0x%lx" after,     \
            signed_long_long           : before "0x%llx" after,     \
            char                       : before "0x%hhx" after,     \
            long_double                : before "0x%Lx" after,     \
            double                     : before "0x%lx" after,     \
            float                      : before "0x%x" after,     \
            default                    : before "" after)



#define GET_TYPENAME(x)             \
    Generic((x),                                \
            bool                       :  "bool",      \
            unsigned_char              :  "unsigned_char",     \
            unsigned_short             :  "unsigned_short",     \
            unsigned_int               :  "unsigned_int",     \
            unsigned_long              :  "unsigned_long",     \
            unsigned_long_long         :  "unsigned_long_long",     \
            signed_char                :  "signed_char",     \
            signed_short               :  "signed_short",     \
            signed_int                 :  "signed_int",     \
            signed_long                :  "signed_long",     \
            signed_long_long           :  "signed_long_long",     \
            char                       :  "char",     \
            long_double                :  "long_double",     \
            double                     :  "double",     \
            float                      :  "float",     \
            default                    :  "unknown")


#define DEFAULT_FMT_TBL(before, x, after)            \
    Generic((x),                                \
            bool                       : before "%u" after,     \
            unsigned_char              : before "%hhu" after,     \
            unsigned_short             : before "%hu" after,     \
            unsigned_int               : before "%u" after,     \
            unsigned_long              : before "%lu" after,     \
            unsigned_long_long         : before "%llu" after,     \
            signed_char                : before "%hh" after,     \
            signed_short               : before "%h" after,     \
            signed_int                 : before "%d" after,     \
            signed_long                : before "%ld" after,     \
            signed_long_long           : before "%lld" after,     \
            char                       : before "%h" after,     \
            long_double                : before "%Lf" after,     \
            double                     : before "%lf" after,     \
            float                      : before "%f" after,     \
            default                    : before "" after)


#define TYPEOF_MAX_0(x)                                 \
    Generic((x),                                        \
            bool                       : 1,             \
            unsigned_char              : UCHAR_MAX,     \
            unsigned_short             : USHRT_MAX,     \
            unsigned_int               : UINT_MAX,      \
            unsigned_long              : ULONG_MAX,     \
            unsigned_long_long         : ULLONG_MAX,    \
            signed_char                : SCHAR_MAX,     \
            signed_short               : SHRT_MAX,      \
            signed_int                 : INT_MAX,       \
            signed_long                : LONG_MAX,      \
            signed_long_long           : LLONG_MAX,     \
            char                       : CHAR_MAX,      \
            long_double                : LDBL_MAX,      \
            double                     : DBL_MAX,       \
            float                      : FLT_MAX,       \
            default                    : 0)

#define TYPEOF_MAX_1(x)                                 \
    Generic((x),                                        \
            safe_bool                  : 1,             \
            default                    : 0)

#define TYPEOF_MAX(x) (TYPEOF_MAX_0(x) == 0 ? TYPEOF_MAX_1(x) : TYPEOF_MAX_0(x))

#define TYPEOF_MIN(x)                                   \
    Generic((x),                                        \
            bool                       : 0,             \
            unsigned_char              : 0,             \
            unsigned_short             : 0,             \
            unsigned_int               : 0,             \
            unsigned_long              : 0,             \
            unsigned_long_long         : 0,             \
            signed_char                : SCHAR_MIN,     \
            signed_short               : SHRT_MIN,      \
            signed_int                 : INT_MIN,       \
            signed_long                : LONG_MIN,      \
            signed_long_long           : LLONG_MIN,     \
            char                       : CHAR_MIN,      \
            long_double                : -(LDBL_MAX),   \
            double                     : -(DBL_MAX),    \
            float                      : -(FLT_MAX),    \
            default                    : 0)

#define IS_UNSIGNED_INT(x)                      \
    Generic((x),                                \
            unsigned_char              : 1,     \
            unsigned_short             : 1,     \
            unsigned_int               : 1,     \
            unsigned_long              : 1,     \
            unsigned_long_long         : 1,     \
            signed_char                : 0,     \
            signed_short               : 0,     \
            signed_int                 : 0,     \
            signed_long                : 0,     \
            signed_long_long           : 0,     \
            char                       : 0,     \
            default                    : 0)

#define IS_INT_BASE(x)                          \
    Generic((x),                                \
            unsigned_char              : 1,     \
            unsigned_short             : 1,     \
            unsigned_int               : 1,     \
            unsigned_long              : 1,     \
            unsigned_long_long         : 1,     \
            signed_char                : 1,     \
            signed_short               : 1,     \
            signed_int                 : 1,     \
            signed_long                : 1,     \
            signed_long_long           : 1,     \
            char                       : 1,     \
            default                    : 0)

#define IS_FLOAT_BASE(x)                        \
    Generic((x),                                \
            long_double                : 1,     \
            double                     : 1,     \
            float                      : 1,     \
            default                    : 0)


#define IS_BOOL_BASE_0(x)                       \
    Generic((x),                                \
            bool                       : 1,     \
            default                    : 0)
#define IS_BOOL_BASE_1(x)                       \
    Generic((x),                                \
            safe_bool                  : 1,     \
            default                    : 0)
#define IS_BOOL_BASE(x) (IS_BOOL_BASE_0(x) || IS_BOOL_BASE_1(x))


// clang-format on

#  define DEFAULT_HEX_FMT(before, x, after)                                    \
            choose_const_expr(IS_PTR(x), before "%p" after,                    \
                              DEFAULT_HEX_FMT_TBL(before, x, after))

#  define DEFAULT_FMT(before, x, after)                                        \
            choose_const_expr(IS_PTR(x), before "%p" after,                    \
                              DEFAULT_FMT_TBL(before, x, after))

#  define cpass_T(T)                                                           \
            get_type(choose_const_expr(sizeof(T) <= 16, (T const){ 0 },        \
                                       (T const *){ 0 }))

#  define pass_T(T)                                                            \
            get_type(choose_const_expr(sizeof(T) <= 16, (T){ 0 }, (T *){ 0 }))


# else
/* TODO: Implement the type macro in C99 (not sure how). */
#  error "Older standard unsupported"

/* Could start to build around these but so far don't need this suite
 * for any
 * C99 projects and they have issues handling things like `void` input.
 */
#  define I_SIGN_PROMOTE(A, B) (1 ? (A) : (B))
#  define I_PROMOTE_1(EXPR)    I_SIGN_PROMOTE(1, (EXPR))
#  define I_PROMOTE_M1(EXPR)   I_SIGN_PROMOTE(-1, (EXPR))
#  define IS_SIGNED(EXPR)      (I_PROMOTE_M1(EXPR) < I_PROMOTE_1(EXPR))

# endif

#endif
#define IS_UNSIGNED(x) (!IS_SIGNED(x))
#define TYPE_MAX(T)    TYPEOF_MAX(DEFAULT_VALUE(T))
#define TYPE_MIN(T)    TYPEOF_MIN(DEFAULT_VALUE(T))

#define GET_IS_FLOAT_BASE(T)   IS_FLOAT_BASE(DEFAULT_VALUE(T))
#define GET_IS_BOOL_BASE(T)    IS_BOOL_BASE(DEFAULT_VALUE(T))
#define GET_IS_INT_BASE(T)     IS_INT_BASE(DEFAULT_VALUE(T))
#define GET_IS_UNSIGNED(T)     IS_UNSIGNED(DEFAULT_VALUE(T))
#define GET_IS_SIGNED(T)       IS_SIGNED(DEFAULT_VALUE(T))
#define GET_IS_UNSIGNED_INT(T) IS_UNSIGNED_INT(DEFAULT_VALUE(T))
#define GET_IS_PTR(T)          IS_PTR(DEFAULT_VALUE(T))

#define GET_UNSIGNED_T(T)     get_type(MAKE_UNSIGNED(DEFAULT_VALUE(T)))
#define GET_UNSIGNED_PTR_T(T) get_type(MAKE_UNSIGNED_PTR(DEFAULT_VALUE(T)))
#define GET_SIGNED_T(T)       get_type(MAKE_SIGNED(DEFAULT_VALUE(T)))
#define GET_SIGNED_PTR_T(T)   get_type(MAKE_SIGNED_PTR(DEFAULT_VALUE(T)))


#define FLOAT_TO_INT_T(x) GET_SIGNED_T(INT_OF_SIZE_T(sizeof(x)))

#define I_typedef_func(new_name, existing_name)                                \
    get_type(existing_name) new_name __attribute__((                           \
        alias(V_TO_STR(existing_name)), ATTR_COPY(existing_name)))

#define typedef_func(new_name, existing_name)                                  \
    I_typedef_func(new_name, existing_name)

#define CHAR_BITU CAST(uint32_t, CHAR_BIT)

#endif
