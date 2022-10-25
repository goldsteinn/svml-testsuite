#ifndef _SRC__CONTAINER__HL_DEFS_H_
#define _SRC__CONTAINER__HL_DEFS_H_

#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

#include "lib/hash/hash.h"
#include "lib/string-custom.h"
#include "util/common.h"
#include "util/inline-math.h"
#include "util/memory-util.h"

/********************************************************************/
/* Common info.  */
enum {
    I__hl_cache_line_size = 64,
    I__hl_page_size       = 4096,
    I__hl_CHAR_BIT        = CHAR_BIT
};

/********************************************************************/
/* Builtins.  */
#define I__hl_choose_expr(...)     I_builtin_choose_expr(__VA_ARGS__)
#define I__hl_constant_p(...)      I_builtin_constant_p(__VA_ARGS__)
#define I__hl_expect(cond, expect) I_builtin_expect(cond, expect)
#define I__hl_unreachable()        I_builtin_unreachable()
#define I__hl_memcpy(...)          I_builtin_memcpy(__VA_ARGS__)
#define I__hl_memset(...)          I_builtin_memset(__VA_ARGS__)
#define I__hl_memcmp(...)          I_builtin_memcmp(__VA_ARGS__)
#define I__hl_memcmp_eq(...)       I_builtin_memcmp_eq(__VA_ARGS__)
#define I__hl_classify_type(...)   I_builtin_classify_type(__VA_ARGS__)
#define I__hl_ptr_type             I_builtin_ptr_type
#define I__hl_ctzw(...)            I_builtin_ctz(__VA_ARGS__)
#define I__hl_ctzl(...)            I_builtin_ctzl(__VA_ARGS__)
#define I__hl_ctzll(...)           I_builtin_ctzll(__VA_ARGS__)
#define I__hl_clzw(...)            I_builtin_clz(__VA_ARGS__)
#define I__hl_clzl(...)            I_builtin_clzl(__VA_ARGS__)
#define I__hl_clzll(...)           I_builtin_clzll(__VA_ARGS__)
#define I__hl_prefetch(...)        I_builtin_prefetch(__VA_ARGS__)

/********************************************************************/
/* Common macros.  */
#define I__hl_likely(...)   I__hl_expect(__VA_ARGS__, 1)
#define I__hl_unlikely(...) I__hl_expect(__VA_ARGS__, 0)
#define I__hl_no_cmov()     __asm__ volatile("" : : :)
#define I__hl_typeof(...)   __typeof__(__VA_ARGS__)
#if 0
#define I__hl_objcopy(dst, src, ...)                                           \
    ({                                                                         \
        if (sizeof(*(dst)) <= 24) {                                            \
            *(dst) = *(src);                                                   \
        }                                                                      \
        else {                                                                 \
            hl_memcpy((dst), (src), sizeof(*(dst)));                           \
        };                                                                     \
    })
#else
#define I__hl_objcopy(...) hl_memcpy(__VA_ARGS__);
#endif

#define I__hl_as_typeof(x, ...) ((__typeof__(x))(__VA_ARGS__))
#define I__hl_const_cond(...)   (I__hl_constant_p(__VA_ARGS__) && (__VA_ARGS__))
#define I__hl_sizeof_bits(x)    (sizeof(x) * I__hl_CHAR_BIT)
#define I__hl_guarantee(x)                                                     \
    if (!(x)) {                                                                \
        I__hl_unreachable();                                                   \
    }
#define I__hl_agu_t(ptr, offset) ((__typeof__(ptr))(I__hl_agu(ptr, offset)))
#define I__hl_agu(ptr, offset)   (((uintptr_t)(ptr)) + (offset))
#define I__hl_is_ptr(x)          (I__hl_classify_type(x) == I__hl_ptr_type)

#define I__hl_ctz(x)                                                           \
    I__hl_choose_expr(                                                         \
        sizeof(x) == sizeof(long long), I__hl_builtin_ctzll(x),                \
        I__hl_choose_expr(sizeof(x) == sizeof(long), I__hl_builtin_ctzl(x),    \
                          I__hl_builtin_ctzw(x)))
#define I__hl_clz(x)                                                           \
    I__hl_choose_expr(                                                         \
        sizeof(x) == sizeof(long long), I__hl_builtin_clzll(x),                \
        I__hl_choose_expr(sizeof(x) == sizeof(long), I__hl_builtin_clzl(x),    \
                          I__hl_builtin_clzw(x)))

#define I__hl_lsb(x)                 ((x) & (-(x)))
#define I__hl_is_p2(x)               (!((x) & ((x)-1)))
#define I__hl_rounddown_p2(x, power) ((x) & (-(power)))

#define I__hl_roundup_p2(x, power)                                             \
    I__hl_rounddown_p2(((x) + ((power)-1)), power)

#define I__hl_unsafe_next_p2(x)                                                \
    (I__hl_as_typeof(x, 2) << ((I__hl_sizeof_bits(x) - 1) - I__hl_clz((x)-1)))

#define I__hl_next_p2(x) next_p2(x)


#define I__hl_log2_p2(x) I__hl_ctz(x)

#define I__hl_scale_sz_to(from_obj_size, to_obj_size, val)                     \
    I__hl_choose_expr(                                                         \
        ((to_obj_size) >= (from_obj_size)) &&                                  \
            (((to_obj_size) % (from_obj_size)) == 0),                          \
        (val) * ((to_obj_size) / (from_obj_size)),                             \
        I__hl_choose_expr(((from_obj_size) >= (to_obj_size)) &&                \
                              (((from_obj_size) % (to_obj_size)) == 0),        \
                          (val) / ((from_obj_size) / (to_obj_size)),           \
                          ((val) / (from_obj_size)) * (to_obj_size)))

#define I__hl_CAST(T, ...)        CAST(T, __VA_ARGS__)
#define I__hl_cat(...)            CAT(__VA_ARGS__)
#define I__hl_namer(prefix, name) I__hl_cat(prefix, _, name)

#define I__hl_load_T(T, p)                                                     \
    ({                                                                         \
        T I__tmp_;                                                             \
        if (sizeof(T) <= 24) {                                                 \
            I__tmp_ = *((T const *)(p));                                       \
        }                                                                      \
        else {                                                                 \
            __builtin_memcpy(&I__tmp_, (T const *)(p), sizeof(T));             \
        }                                                                      \
        I__tmp_;                                                               \
    })

#define I__hl_store_T(T, p, v)                                                 \
    ({                                                                         \
        if (sizeof(T) <= 24) {                                                 \
            *((T *)(p)) = (v);                                                 \
        }                                                                      \
        else {                                                                 \
            T I__tmp_ = (v);                                                   \
            __builtin_memcpy((T *)(p), &I__tmp_, sizeof(T));                   \
        }                                                                      \
    })


/********************************************************************/
/* Asserts.  */
#define I__hl_static_assert(...) const_assert(__VA_ARGS__)
#define I__hl_assert_constant(expr)                                            \
    if (!I__hl_constant_p(expr)) {                                             \
        I__hl_make_error();                                                    \
    }


#define I__hl_assert_const_eval(expr)                                          \
    enum {                                                                     \
        I__hl_cat(I__hl_checking_constant_eval, __LINE__, _, __COUNTER__) =    \
            (expr)                                                             \
    };

#define I__hl_err_check(x, err_code)                                           \
    if (I__hl_unlikely(!(x))) {                                                \
        return err_code;                                                       \
    }

extern void I_attr_error("Should be unreachable") I__hl_make_error_f();
#define I__hl_make_error(...) I__hl_error_f(__VA_ARGS__)


/********************************************************************/
/* Attributes.  */
#define I__hl_noinline NEVER_INLINE
#define I__hl_pure     I_attr_pure
#define I__hl_const    I_attr_const
#define I__hl_unused   I_attr_unused

#define I__hl_attrs  static ALWAYS_INLINE
#define I__hl_cattrs I__hl_attrs I__hl_const
#define I__hl_pattrs I__hl_attrs I__hl_pure

/********************************************************************/
/* Types.  */
typedef struct I__hl_byte {
    uint8_t val_;
} I__hl_byte_t;

typedef struct I__hl_word {
    uint16_t val_;
} I__hl_word_t;

typedef struct I__hl_long {
    uint32_t val_;
} I__hl_long_t;

typedef struct I__hl_quad {
    uint32_t val_;
} I__hl_quad_t;

typedef _Bool     I__hl_bool_t;
typedef size_t    I__hl_obj_size_t;
typedef uintptr_t I__hl_uptr_t;


/********************************************************************/
/* Returns.  */
#define I__hl_error_max I__hl_CAST(void *, 65535UL)
#define I__hl_failed    NULL

#define I__hl_was_error(p) ((I__hl_CAST(uintptr_t, p) - 1) < I__hl_error_max)

#endif
