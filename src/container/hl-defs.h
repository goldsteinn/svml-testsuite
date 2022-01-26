#ifndef SRC_D_CONTAINER_D_HL_DEFS_H_
#define SRC_D_CONTAINER_D_HL_DEFS_H_

#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

#include "lib/hash/hash.h"
#include "lib/string-custom.h"
#include "util/common.h"
#include "util/error-util.h"
#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/portability.h"

/********************************************************************/
/* Common info.  */
enum {
    I_hl_cache_line_size = 64,
    I_hl_page_size       = 4096,
    I_hl_CHAR_BIT        = CHAR_BIT
};

/********************************************************************/
/* Builtins.  */
#define I_hl_choose_expr(...)     I_builtin_choose_expr(__VA_ARGS__)
#define I_hl_constant_p(...)      I_builtin_constant_p(__VA_ARGS__)
#define I_hl_expect(cond, expect) I_builtin_expect(cond, expect)
#define I_hl_unreachable()        I_builtin_unreachable()
#define I_hl_memcpy(...)          I_builtin_memcpy(__VA_ARGS__)
#define I_hl_memset(...)          I_builtin_memset(__VA_ARGS__)
#define I_hl_memcmp(...)          I_builtin_memcmp(__VA_ARGS__)
#define I_hl_memcmp_eq(...)       I_builtin_memcmp_eq(__VA_ARGS__)
#define I_hl_classify_type(...)   I_builtin_classify_type(__VA_ARGS__)
#define I_hl_ptr_type             I_builtin_ptr_type
#define I_hl_ctzw(...)            I_builtin_ctz(__VA_ARGS__)
#define I_hl_ctzl(...)            I_builtin_ctzl(__VA_ARGS__)
#define I_hl_ctzll(...)           I_builtin_ctzll(__VA_ARGS__)
#define I_hl_clzw(...)            I_builtin_clz(__VA_ARGS__)
#define I_hl_clzl(...)            I_builtin_clzl(__VA_ARGS__)
#define I_hl_clzll(...)           I_builtin_clzll(__VA_ARGS__)
#define I_hl_prefetch(...)        I_builtin_prefetch(__VA_ARGS__)

/********************************************************************/
/* Platform.  */
#define I_hl_USING_LLVM USING_LLVM
#define I_hl_USING_GCC  USING_GCC

/********************************************************************/
/* Type macros.  */
#define I_hl_as_typeof(x, ...)         ((__typeof__(x))(__VA_ARGS__))
#define I_hl_as_member_type(T, member) __typeof__(((T){ 0 }).member)
#define I_hl_uint_for_bits(nbits)                                              \
    I_hl_choose_T((nbits) <= 16,                                               \
                  I_hl_choose_T((nbits) <= 8, uint8_t, uint16_t),              \
                  I_hl_choose_T((nbits) <= 32, uint32_t, uint64_t))

#define I_hl_is_ptr(x) (I_hl_classify_type(x) == I_hl_ptr_type)

#define I_hl_choose_T(cond, T0, T1) choose_T(cond, T0, T1)

/********************************************************************/
/* Common macros.  */
#define I_hl_likely(...)   I_hl_expect(__VA_ARGS__, 1)
#define I_hl_unlikely(...) I_hl_expect(__VA_ARGS__, 0)


#define I_hl_no_cmov()   __asm__ volatile("" : : :)
#define I_hl_typeof(...) __typeof__(__VA_ARGS__)

#define I_hl_const_cond(...) (I_hl_constant_p(__VA_ARGS__) && (__VA_ARGS__))
#define I_hl_sizeof_bits(x)  (sizeof(x) * I_hl_CHAR_BIT)
#define I_hl_guarantee(x)                                                      \
    if (!(x)) {                                                                \
        I_hl_unreachable();                                                    \
    }

#define I_hl_p2_m(p, m)                                                        \
    (((p) == I_hl_sizeof_bits(unsigned long)) ? (0UL - (m))                    \
                                              : ((1UL << (p)) - (m)))
#define I_hl_agu_t(ptr, offset) ((__typeof__(ptr))(I_hl_agu(ptr, offset)))
#define I_hl_agu(ptr, offset)   (((uintptr_t)(ptr)) + (offset))

#define I_hl_ctz(x)                                                            \
    I_hl_choose_expr(                                                          \
        sizeof(x) == sizeof(long long), I_hl_builtin_ctzll(x),                 \
        I_hl_choose_expr(sizeof(x) == sizeof(long), I_hl_builtin_ctzl(x),      \
                         I_hl_builtin_ctzw(x)))
#define I_hl_clz(x)                                                            \
    I_hl_choose_expr(                                                          \
        sizeof(x) == sizeof(long long), I_hl_builtin_clzll(x),                 \
        I_hl_choose_expr(sizeof(x) == sizeof(long), I_hl_builtin_clzl(x),      \
                         I_hl_builtin_clzw(x)))

#define I_hl_lsb(x)                 ((x) & (-(x)))
#define I_hl_is_p2(x)               (!((x) & ((x)-1)))
#define I_hl_rounddown_p2(x, power) ((x) & (-(power)))

#define I_hl_roundup_p2(x, power) I_hl_rounddown_p2(((x) + ((power)-1)), power)

#define I_hl_unsafe_next_p2(x)                                                 \
    (I_hl_as_typeof(x, 2) << ((I_hl_sizeof_bits(x) - 1) - I_hl_clz((x)-1)))

#define I_hl_max(...) CMAX(__VA_ARGS__)

#define I_hl_next_p2(x)            next_p2(x)
#define I_hl_const_eval_next_p2(x) const_eval_next_p2(x)

#define I_hl_log2_p2(x) I_hl_ctz(x)

#define I_hl_scale_sz_to(from_obj_size, to_obj_size, val)                      \
    I_hl_choose_expr(                                                          \
        ((to_obj_size) >= (from_obj_size)) &&                                  \
            (((to_obj_size) % (from_obj_size)) == 0),                          \
        (val) * ((to_obj_size) / (from_obj_size)),                             \
        I_hl_choose_expr(((from_obj_size) >= (to_obj_size)) &&                 \
                             (((from_obj_size) % (to_obj_size)) == 0),         \
                         (val) / ((from_obj_size) / (to_obj_size)),            \
                         ((val) / (from_obj_size)) * (to_obj_size)))

#define I_hl_CAST(T, ...)        CAST(T, __VA_ARGS__)
#define I_hl_cat(...)            CAT(__VA_ARGS__)
#define I_hl_namer(prefix, name) I_hl_cat(prefix, _, name)

#define I_hl_load_T(T, p)                                                      \
    ({                                                                         \
        T I_tmp_load_var_;                                                     \
        if (sizeof(T) <= 24) {                                                 \
            I_tmp_load_var_ = *((T const *)(p));                               \
        }                                                                      \
        else {                                                                 \
            I_hl_memcpy(&I_tmp_load_var_, (T const *)(p), sizeof(T));          \
        }                                                                      \
        I_tmp_load_var_;                                                       \
    })

#define I_hl_store_T(T, p, v)                                                  \
    ({                                                                         \
        if (sizeof(T) <= 24) {                                                 \
            *((T *)(p)) = (v);                                                 \
        }                                                                      \
        else {                                                                 \
            T I_tmp_store_var_ = (v);                                          \
            I_hl_memcpy((T *)(p), &I_tmp_store_var_, sizeof(T));               \
        }                                                                      \
    })


/********************************************************************/
/* Asserts.  */
#define I_hl_static_assert(...) const_assert(__VA_ARGS__)
#define I_hl_assert_constant(expr)                                             \
    if (!I_hl_constant_p(expr)) {                                              \
        I_hl_make_error();                                                     \
    }


#define I_hl_assert_const_eval(expr)                                           \
    enum {                                                                     \
        I_hl_cat(I_hl_checking_constant_eval, __LINE__, _, __COUNTER__) =      \
            (expr)                                                             \
    };

#define I_hl_err_check(x, err_code)                                            \
    if (I_hl_unlikely(!(x))) {                                                 \
        return err_code;                                                       \
    }

extern void I_attr_error("Should be unreachable") I_hl_make_error_f(void);
#define I_hl_make_error(...) I_hl_make_error_f()


/********************************************************************/
/* Attributes.  */
#if defined(__cplusplus) && I_STDCPP_VERSION >= 2014
# define I_hl_constexpr constexpr
#else
# define I_hl_constexpr
#endif

#define I_hl_noinline NEVER_INLINE
#define I_hl_pure     I_attr_pure
#define I_hl_const    I_attr_const
#define I_hl_unused   I_attr_unused

#define I_hl_attrs  static I_hl_constexpr ALWAYS_INLINE
#define I_hl_cattrs I_hl_attrs I_hl_const
#define I_hl_pattrs I_hl_attrs I_hl_pure

/********************************************************************/
/* Types.  */
typedef struct I_hl_byte {
    uint8_t val_;
} I_hl_byte_t;

typedef struct I_hl_word {
    uint16_t val_;
} I_hl_word_t;

typedef struct I_hl_long {
    uint32_t val_;
} I_hl_long_t;

typedef struct I_hl_quad {
    uint64_t val_;
} I_hl_quad_t;

#ifdef __cplusplus
typedef bool I_hl_bool_t;
# define I_hl_dconst
#else
typedef _Bool I_hl_bool_t;
# define I_hl_dconst const
#endif
typedef size_t    I_hl_obj_size_t;
typedef uintptr_t I_hl_uptr_t;


/********************************************************************/
/* Returns.  */

#define I_hl_die(...) msg_die(__VA_ARGS__)

#define hl_operation_success ((I_hl_bool_t)0)
#define hl_operation_failure ((I_hl_bool_t)1)

#endif
