#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <stdint.h>
#include <string.h>

#include "util/macro.h"

#define I__st_key_t               uint64_t
#define I__st_val_t               uint64_t
#define I__st_name                I__st_default_name(I__st_key_t, I__st_val_t)
#define I__st_pass_key_t          I__st_default_pass_key_t
#define I__st_hashret_t           I__st_default_hashret_t
#define I__st_hash                MUSR_HASH
#define I__st_rehash(kvp)         I__st_hash((kvp)->key)
#define I__st_kvp_move            I__st_default_kvp_move
#define I__st_kvp_destroy         I__st_default_kvp_destroy
#define I__st_key_cmpeq           I__st_default_key_cmpeq
#define I__st_key_set             I__st_default_key_set
#define I__st_hashret_get_hashval I__st_default_hashret_get_hashval


#define I__st_matchers_per_group I__st_default_matchers_per_group
#define I__st_with_delete        I__st_default_with_delete
#define I__st_robust_resize      I__st_default_robust_resize

#define I__st_assert        I__st_default_assert
#define I__st_aligned_alloc I__st_default_aligned_alloc
#define I__st_free_s        I__st_default_free_s
#define I__st_memset        I__st_default_memset
#define I__st_memcpy        I__st_default_memcpy
#define I__st_memcmpeq      I__st_default_memcmpeq


/********************************************************************/
/* Default Inputs.  */
#ifndef I__st_HAS_DEFAULTS
#define I__st_HAS_DEFAULTS


#define I__st_default_key_t      I__st_error("Must provide a key type")
#define I__st_default_val_t      I__st_zero_t /* Default is as a set.  */
#define I__st_default_name(k, v) CAT(k, _, v)

#define I__st_default_hash(usr_pass_key)                                       \
    I__st_error("Must provide a hash function!")

#define I__st_default_pass_key_t I__st_key_t

#define I__st_default_key_cmpeq(tbl_key_ptr, usr_pass_key, ...)                \
    (!I__st_default_memcmpeq(tbl_key_ptr, I__st_as_ptr(usr_pass_key),          \
                             sizeof(I__st_key_t)))

#define I__st_default_key_set(tbl_key_ptr, usr_pass_key, ...)                  \
    I__st_default_memcpy(tbl_key_ptr, I__st_as_ptr(usr_pass_key),              \
                         sizeof(I__st_key_t))

#define I__st_default_kvp_move(next_kvp_ptr, old_kvp_ptr, ...)                 \
    I__st_default_memcpy(next_kvp_ptr, old_kvp_ptr, sizeof(I__st_kvp_t))

#define I__st_default_kvp_destroy(tbl_kvp_ptr, ...) /* empty.  */

#define I__st_default_hashret_t                    I__st_obj_size_t
#define I__st_default_rehash(tbl_key_ptr)          I__st_default_hash((tbl_key_ptr)->key)
#define I__st_default_hashret_get_hashval(hashret) hashret

#define I__st_default_memcmpeq(s0, s1, size)                                   \
    (I__st_builtin_memcmp_eq(s0, s1, size))
#define I__st_default_memset(dst, val, size)                                   \
    (I__st_builtin_memset(dst, val, size))
#define I__st_default_memcpy(dst, src, size)                                   \
    (I__st_builtin_memcpy(dst, src, size))

#define I__st_default_free_s(p, size)                                          \
    (void)(size);                                                              \
    free((void *)(p))

#define I__st_default_aligned_alloc(alignment, size)                           \
    aligned_alloc(alignment, size)

#define I__st_default_assert(...) assert(__VA_ARGS__)

#define I__st_default_matchers_per_group                                       \
    ((I__st_cache_line_size) / sizeof(I__st_matcher_t))
#define I__st_default_with_delete   1
#define I__st_default_robust_resize 1
#endif

/********************************************************************/
/* Internal Types.  */

#ifndef I__st_HAS_INTERNAL_TYPES
#define I__st_HAS_INTERNAL_TYPES
typedef struct I__st_zero {
    unsigned unused[0];
} I__st_zero_t;

typedef struct I__st_byte {
    uint8_t val;
} I__st_byte_t;


typedef size_t    I__st_obj_size_t;
typedef uintptr_t I__st_ptr_t;
typedef _Bool     I__st_bool_t;

typedef struct I__st_hashtbl {
    I__st_obj_size_t group_mask;
    I__st_ptr_t      mem;
} I__st_hashtbl_t;

typedef I__st_byte_t I__st_tag_t;
#endif

/********************************************************************/
/* Internal Constants.  */
#ifndef I__st_HAS_ENUMS
#define I__st_HAS_ENUMS

#if defined __x86_64__ && defined __SSSE3__
#include <immintrin.h>
typedef __m128i I__st_matcher_t;
enum {
    I__st_cache_line_size = 64,
    I__st_page_size       = 4096,
    I__st_CHAR_BIT        = CHAR_BIT
};


#else
#error "Unimplemented!"
#endif

enum {
    I__st_tag_empty   = 0x80,
    I__st_tag_deleted = 0xff,
    I__st_tag_size    = sizeof(I__st_tag_t)
};

enum {
    I__st_success     = 0,
    I__st_invalid_arg = -1,
    I__st_invalid_ptr = -2,
    I__st_no_mem      = -3
};

#endif


/********************************************************************/
/* Builtins. */

#ifndef I__st_HAS_BUILTINS
#define I__st_HAS_BUILTINS

#ifdef __has_builtin
#define I__st_has_builtin(...) __has_builtin(__VA_ARGS__)
#else
#define I__st_has_builtin(...) 0
#endif

#if I__st_has_builtin(__builtin_choose_expr)
#define I__st_builtin_choose_expr(...) __builtin_choose_expr(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif

#if I__st_has_builtin(__builtin_memset)
#define I__st_builtin_memset(...) __builtin_memset(__VA_ARGS__)
#else
#define I__st_builtin_memset(...) memset(__VA_ARGS__)
#endif

#if I__st_has_builtin(__builtin_memcpy)
#define I__st_builtin_memcpy(...) __builtin_memcpy(__VA_ARGS__)
#else
#define I__st_builtin_memcpy(...) memcpy(__VA_ARGS__)
#endif

#if I__st_has_builtin(__builtin_memcmp_eq)
#define I__st_builtin_memcmp_eq(...) __builtin_memcmp_eq(__VA_ARGS__)
#else
#define I__st_builtin_memcmp_eq(...) (!!memcmp(__VA_ARGS__))
#endif


#if I__st_has_builtin(__builtin_constant_p)
#define I__st_builtin_constant_p(...) __builtin_constant_p(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif

#if I__st_has_builtin(__builtin_unreachable)
#define I__st_builtin_unreachable(...) __builtin_unreachable(__VA_ARGS__)
#else
#define I__st_builtin_unreachable(...) (void)(0)
#endif

#if I__st_has_builtin(__builtin_expect)
#define I__st_builtin_likely(...)   __builtin_expect(__VA_ARGS__, 1)
#define I__st_builtin_unlikely(...) __builtin_expect(__VA_ARGS__, 0)
#else
#define I__st_builtin_likely(...)   (__VA_ARGS__)
#define I__st_builtin_unlikely(...) (__VA_ARGS__)
#endif

#if I__st_has_builtin(__builtin_classify_type)
#define I__st_builtin_classify_type(...) __builtin_classify_type(__VA_ARGS__)
#define I__st_builtin_ptr_type           5
#else
#error "Unimplemented!"
#endif

#if I__st_has_builtin(__builtin_prefetch)
#define I__st_builtin_prefetch(...) __builtin_prefetch(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif


#if I__st_has_builtin(__builtin_ctz) && I__st_has_builtin(__builtin_ctzl) &&   \
    I__st_has_builtin(__builtin_ctzll)
#define I__st_builtin_ctz(...)   __builtin_ctz(__VA_ARGS__)
#define I__st_builtin_ctzl(...)  __builtin_ctzl(__VA_ARGS__)
#define I__st_builtin_ctzll(...) __builtin_ctzll(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif

#if I__st_has_builtin(__builtin_clz) && I__st_has_builtin(__builtin_clzl) &&   \
    I__st_has_builtin(__builtin_clzll)
#define I__st_builtin_clz(...)   __builtin_clz(__VA_ARGS__)
#define I__st_builtin_clzl(...)  __builtin_clzl(__VA_ARGS__)
#define I__st_builtin_clzll(...) __builtin_clzll(__VA_ARGS__)
#else
#error "Unimplemented!"
#endif
#endif

/********************************************************************/
/* Common Macros.  */
#ifndef I__st_HAS_COMMON_MACROS
#define I__st_HAS_COMMON_MACROS

#define I__st_likely      I__st_builtin_likely
#define I__st_unlikely    I__st_builtin_unlikely
#define I__st_unreachable I__st_builtin_unreachable
#define I__st_constant_p  I__st_builtin_constant_p
#define I__st_choose_expr I__st_builtin_choose_expr

#define I__st_no_cmov()         __asm__ volatile("" : : :);
#define I__st_typeof(...)       __typeof__(__VA_ARGS__)
#define I__st_as_typeof(x, ...) ((__typeof__(x))(__VA_ARGS__))
#define I__st_const_cond(...)   (I__st_constant_p(__VA_ARGS__) && (__VA_ARGS__))
#define I__st_sizeof_bits(x)    (sizeof(x) * I__st_CHAR_BIT)
#define I__st_guarantee(x)                                                     \
    if (!(x)) {                                                                \
        I__st_unreachable();                                                   \
    }
#define I__st_agu_t(ptr, offset) ((__typeof__(ptr))(I__st_agu(ptr, offset)))
#define I__st_agu(ptr, offset)   (((I__st_ptr_t)(ptr)) + (offset))
#define I__st_is_ptr(x)                                                        \
    (I__st_builtin_classify_type(x) == I__st_builtin_ptr_type)
#define I__st_prefetch_L1(p) I__st_builtin_prefetch((void const *)(p))
#define I__st_as_ptr(x)      I__st_choose_expr(I__st_is_ptr(x), (x), &(x))

#define I__st_ctz(x)                                                           \
    I__st_choose_expr(                                                         \
        sizeof(x) == sizeof(long long), I__st_builtin_ctzll(x),                \
        I__st_choose_expr(sizeof(x) == sizeof(long), I__st_builtin_ctzl(x),    \
                          I__st_builtin_ctz(x)))
#define I__st_clz(x)                                                           \
    I__st_choose_expr(                                                         \
        sizeof(x) == sizeof(long long), I__st_builtin_clzll(x),                \
        I__st_choose_expr(sizeof(x) == sizeof(long), I__st_builtin_clzl(x),    \
                          I__st_builtin_clz(x)))

#define I__st_lsb(x)                 ((x) & (-(x)))
#define I__st_is_p2(x)               (!((x) & ((x)-1)))
#define I__st_rounddown_p2(x, power) ((x) & (-(power)))

#define I__st_roundup_p2(x, power)                                             \
    I__st_rounddown_p2(((x) + ((power)-1)), power)

#define I__st_unsafe_next_p2(x)                                                \
    (I__st_as_typeof(x, 2) << ((I__st_sizeof_bits(x) - 1) - I__st_clz((x)-1)))
#define I__st_log2_p2(x) I__st_ctz(x)

#define I__st_scale_sz_to(from_obj_size, to_obj_size, val)                     \
    I__st_choose_expr(                                                         \
        ((to_obj_size) >= (from_obj_size)) &&                                  \
            (((to_obj_size) % (from_obj_size)) == 0),                          \
        (val) * ((to_obj_size) / (from_obj_size)),                             \
        I__st_choose_expr(((from_obj_size) >= (to_obj_size)) &&                \
                              (((from_obj_size) % (to_obj_size)) == 0),        \
                          (val) / ((from_obj_size) / (to_obj_size)),           \
                          ((val) / (from_obj_size)) * (to_obj_size)))


#define I__st_static_assert(x, ...) _Static_assert(x)
#define I__st_assert_constant(expr)                                            \
    if (!I__st_constant_p(expr)) {                                             \
        I__st_error();                                                         \
    }


#define I__st_assert_const_eval(expr)                                          \
    {                                                                          \
        enum { I__st_checking_constant_eval##__CURRENT__ = (expr) };           \
    }

#define I__st_err_check(x, err_code)                                           \
    if (I__st_unlikely(!(x))) {                                                \
        return err_code;                                                       \
    }

extern void __attribute__((error("Should be unreachable"))) I__st_error_f();
#define I__st_error(...) I__st_error_f(__VA_ARGS__)

#if defined(__clang__)
#define I__st_noinline __attribute__((noinline))
#else
#define I__st_noinline __attribute__((noinline, noclone))
#endif

#define I__st_internal_attrs           static inline __attribute__((always_inline))
#define I__st_internal_func(func_name) CAT(I__st_, I__st_name, _, func_name)
#endif


/********************************************************************/
/* User based types.  */
typedef struct I__st_kvp {
    const I__st_key_t key;
    I__st_val_t       val;
} I__st_kvp_t;

typedef struct I__st_insert_ret {
    I__st_kvp_t * kvp_start;
    I__st_bool_t  was_in;
} I__st_insert_ret_t;

typedef I__st_kvp_t * I__st_find_ret_t;
typedef I__st_kvp_t * I__st_delete_ret_t;

/********************************************************************/
/* User based Constants.  */

#define I__st_group_size ((I__st_matchers_per_group) * sizeof(I__st_matcher_t))
#define I__st_group_alignment                                                  \
    ((I__st_matchers_per_group) * sizeof(I__st_matcher_t))
#define I__st_group_mask_precision (I__st_group_size)
#define I__st_kvp_obj_size         (sizeof(I__st_kvp_t))


/********************************************************************/
/* Mask Manipulation.  */

#define I__st_group_mask_to_nelem I__st_internal_func(group_mask_to_nelem)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(group_mask_to_nelem)(I__st_obj_size_t group_mask) {
    return group_mask + I__st_group_mask_precision;
}

#define I__st_next_group_mask I__st_internal_func(next_group_mask)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(next_group_mask)(I__st_obj_size_t group_mask) {
    return group_mask + group_mask + I__st_group_mask_precision;
}

#define I__st_nelem_to_group_mask I__st_internal_func(nelem_to_group_mask)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(nelem_to_group_mask)(I__st_obj_size_t nelem) {
    return nelem - I__st_group_mask_precision;
}

/********************************************************************/
/* Setup Utils.  */

#define I__st_usr_nelem_to_tbl_nelem I__st_internal_func(usr_nelem_to_tbl_nelem)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(usr_nelem_to_tbl_nelem)(I__st_obj_size_t usr_nelem) {
    usr_nelem = I__st_roundup_p2(usr_nelem, I__st_group_size);
    usr_nelem = I__st_unsafe_next_p2(usr_nelem);
    return usr_nelem;
}

#define I__st_nelem_to_alloc_size I__st_internal_func(nelem_to_alloc_size)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(nelem_to_alloc_size)(I__st_obj_size_t nelem) {
    return nelem * (I__st_tag_size + I__st_kvp_obj_size);
}

#define I__st_group_mask_to_alloc_sz I__st_internal_func(group_mask_to_alloc_sz)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(group_mask_to_alloc_sz)(I__st_obj_size_t group_mask) {
    return I__st_nelem_to_alloc_size(I__st_group_mask_to_nelem(group_mask));
}


/********************************************************************/
/* Hashval Utils.  */

#define I__st_hashval_to_tag I__st_internal_func(hashval_to_tag)
I__st_internal_attrs uint32_t
I__st_internal_func(hashval_to_tag)(I__st_obj_size_t hashval) {
    return hashval >>
           (I__st_sizeof_bits(hashval) - (I__st_sizeof_bits(I__st_tag_t) - 1));
}

#define I__st_hashval_to_group_offset                                          \
    I__st_internal_func(hashval_to_group_offset)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(hashval_to_group_offset)(I__st_obj_size_t hashval,
                                             I__st_obj_size_t group_mask) {
    return hashval & group_mask;
}

#define I__st_hashval_to_match_offset                                          \
    I__st_internal_func(hashval_to_match_offset)
I__st_internal_attrs uint32_t
I__st_internal_func(hashval_to_match_offset)(I__st_obj_size_t hashval) {
    return hashval & (I__st_group_mask_precision - sizeof(I__st_matcher_t));
}

#define I__st_rehashval_to_match_offset                                        \
    I__st_internal_func(rehashval_to_match_offset)
I__st_internal_attrs uint32_t
I__st_internal_func(rehashval_to_match_offset)(I__st_obj_size_t rehashval) {
    return I__st_hashval_to_match_offset(rehashval) /
           (sizeof(I__st_matcher_t) / 2);
}

#define I__st_hashval_to_resize_offsets_idx                                    \
    I__st_internal_func(hashval_to_resize_offsets_idx)
I__st_internal_attrs uint32_t
I__st_internal_func(hashval_to_resize_offsets_idx)(
    I__st_obj_size_t rehashval,
    I__st_obj_size_t next_tag_offset) {
    uint32_t mask_match = I__st_rehashval_to_match_offset(rehashval);
    return mask_match + (next_tag_offset != 0);
}

#define I__st_hashval_to_resize_tag_offset                                     \
    I__st_internal_func(hashval_to_resize_tag_offset)
I__st_internal_attrs uint32_t
I__st_internal_func(hashval_to_resize_tag_offset)(I__st_obj_size_t rehashval,
                                                  I__st_obj_size_t old_nelem) {
    return rehashval & old_nelem;
}


/********************************************************************/
/* Getters & Setters.  */
#define I__st_set_tag_at I__st_internal_func(set_tag_at)
I__st_internal_attrs void
I__st_internal_func(set_tag_at)(I__st_tag_t * group_pos, uint32_t tag) {
    I__st_memcpy(group_pos, &tag, sizeof(I__st_tag_t));
}

#define I__st_get_tag_at I__st_internal_func(get_tag_at)
I__st_internal_attrs uint32_t
I__st_internal_func(get_tag_at)(I__st_tag_t const * group_pos) {
    I__st_tag_t b = *group_pos;
    return b.val;
}

/********************************************************************/
/* Offset Utils.  */

#define I__st_tag_sz_to_kvp_sz I__st_internal_func(tag_sz_to_kvp_sz)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(tag_sz_to_kvp_sz)(I__st_obj_size_t tag_sz) {
    return tag_sz * I__st_kvp_obj_size;
}

#define I__st_group_offset_to_kvp_offset                                       \
    I__st_internal_func(group_offset_to_kvp_offset)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(group_offset_to_kvp_offset)(I__st_obj_size_t group_offset) {
    return I__st_tag_sz_to_kvp_sz(group_offset);
}

#define I__st_nelem_to_kvp_offset I__st_internal_func(nelem_to_kvp_offset)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(nelem_to_kvp_offset)(I__st_obj_size_t nelem) {
    return nelem;
}

#define I__st_get_kvp_pos I__st_internal_func(get_kvp_pos)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(get_kvp_pos)(uint32_t match_offset, uint32_t tag_offset) {
    return I__st_tag_sz_to_kvp_sz(match_offset + tag_offset);
}

#define I__st_tag_offset_to_kvp_offset                                         \
    I__st_internal_func(tag_offset_to_kvp_offset)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(tag_offset_to_kvp_offset)(uint32_t tag_offset) {
    return I__st_tag_sz_to_kvp_sz(tag_offset);
}

#define I__st_group_info_to_kvp_offset                                         \
    I__st_internal_func(group_info_to_kvp_offset)
I__st_internal_attrs I__st_obj_size_t
I__st_internal_func(group_info_to_kvp_offset)(I__st_obj_size_t group_offset,
                                              I__st_obj_size_t group_mask) {
    return I__st_tag_sz_to_kvp_sz(group_offset) +
           I__st_group_mask_to_nelem(group_mask);
}

#define I__st_get_next_match_offset I__st_internal_func(get_next_match_offset)
I__st_internal_attrs uint32_t
I__st_internal_func(get_next_match_offset)(uint32_t match_offset, uint32_t i) {
#if 0
    return match_offset ^ I__st_lsb(i);
#else
    (void)(i);
    return (match_offset + sizeof(I__st_matcher_t)) & (I__st_group_size - 1);
#endif
}

/********************************************************************/
/* Matcher x86_64 API.  */

#define I__st_matcher_init_tag I__st_internal_func(matcher_init_tag)
I__st_internal_attrs I__st_matcher_t
I__st_internal_func(matcher_init_tag)(uint32_t scan_tag) {
    return _mm_set1_epi8(scan_tag);
}

#define I__st_matcher_load_mem I__st_internal_func(matcher_load_mem)
I__st_internal_attrs I__st_matcher_t
I__st_internal_func(matcher_load_mem)(I__st_tag_t const * tags_mem) {
    return _mm_loadu_si128((__m128i const *)tags_mem);
}

#define I__st_matcher_find_matches I__st_internal_func(matcher_find_matches)
I__st_internal_attrs uint32_t
I__st_internal_func(matcher_find_matches)(I__st_matcher_t tag_matcher,
                                          I__st_matcher_t tbl_matcher) {
    return _mm_movemask_epi8(_mm_cmpeq_epi8(tag_matcher, tbl_matcher));
}


#define I__st_matcher_find_empty I__st_internal_func(matcher_find_empty)
I__st_internal_attrs uint32_t
I__st_internal_func(matcher_find_empty)(I__st_matcher_t tbl_matcher) {
    I__st_assert_const_eval(I__st_with_delete);
    if (I__st_with_delete) {
        tbl_matcher = _mm_abs_epi8(tbl_matcher);
    }
    return _mm_movemask_epi8(tbl_matcher);
}

#define I__st_matcher_find_deleted I__st_internal_func(matcher_find_deleted)
I__st_internal_attrs uint32_t
I__st_internal_func(matcher_find_deleted)(I__st_matcher_t tbl_matcher) {
    I__st_assert_const_eval(I__st_with_delete);
    if (!I__st_with_delete) {
        I__st_error("Should be unreachable");
    }
    return _mm_movemask_epi8(
        _mm_cmpeq_epi8(tbl_matcher, _mm_set1_epi8((char)I__st_tag_deleted)));
}

#define I__st_matcher_find_inuse I__st_internal_func(matcher_find_inuse)
I__st_internal_attrs uint32_t
I__st_internal_func(matcher_find_inuse)(I__st_matcher_t tbl_matcher) {
    return _mm_movemask_epi8(tbl_matcher) ^ 0xffff;
}

/********************************************************************/
/* Allocation / Free.  */

#define I__st_alloc_tbl I__st_internal_func(alloc_tbl)
I__st_internal_attrs I__st_ptr_t
I__st_internal_func(alloc_tbl)(I__st_obj_size_t group_mask) {
    return (I__st_ptr_t)I__st_aligned_alloc(
        I__st_group_alignment, I__st_group_mask_to_alloc_sz(group_mask));
}

#define I__st_initialize_group I__st_internal_func(initialize_group)
I__st_internal_attrs void
I__st_internal_func(initialize_group)(I__st_tag_t * group) {
    I__st_memset(group, I__st_tag_empty, I__st_group_size);
}

#define I__st_free_tbl I__st_internal_func(free_tbl)
I__st_internal_attrs void
I__st_internal_func(free_tbl)(I__st_ptr_t p, I__st_obj_size_t nelem) {
    I__st_free_s(p, I__st_nelem_to_alloc_size(nelem));
}

#define I__st_zero I__st_internal_func(zero)
I__st_internal_attrs int32_t
I__st_internal_func(zero)(I__st_hashtbl_t * tbl) {
    I__st_err_check(tbl != NULL, I__st_invalid_ptr);
    I__st_memset(tbl, 0, sizeof(*tbl));
    return I__st_success;
}

/********************************************************************/
/* Destroy / Create API.  */

static I__st_obj_size_t
I__st_capacity(I__st_hashtbl_t const * tbl) {
    return I__st_group_mask_to_nelem(tbl->group_mask);
}

static int32_t
I__st_init(I__st_hashtbl_t * tbl, I__st_obj_size_t usr_nelem) {
    I__st_ptr_t      mem;
    I__st_obj_size_t nelem, alloc_sz;
    I__st_err_check(tbl != NULL, I__st_invalid_ptr);
    I__st_err_check(usr_nelem != 0, I__st_invalid_arg);

    nelem    = I__st_usr_nelem_to_tbl_nelem(usr_nelem);
    alloc_sz = I__st_nelem_to_alloc_size(nelem);
    mem = (I__st_ptr_t)I__st_aligned_alloc(I__st_group_alignment, alloc_sz);
    I__st_err_check(mem != 0, I__st_no_mem);

    tbl->group_mask = I__st_nelem_to_group_mask(nelem);
    tbl->mem        = mem;

    I__st_memset((I__st_byte_t *)mem, I__st_tag_empty, nelem);
    return I__st_success;
}

static int32_t
I__st_destroy(I__st_hashtbl_t const * tbl) {
    I__st_obj_size_t alloc_sz;

    I__st_err_check(tbl != NULL, I__st_invalid_ptr);

    alloc_sz = I__st_group_mask_to_alloc_sz(tbl->group_mask);
    I__st_free_s(tbl->mem, alloc_sz);
    return I__st_success;
}


/********************************************************************/
/* Insert/Find/Delete API.  */
static I__st_noinline I__st_insert_ret_t
I__st_resize(I__st_hashtbl_t *      tbl,
             const I__st_pass_key_t usr_pass_key,
             I__st_hashret_t        hashret) {

    I__st_ptr_t          old_tbl_mem;
    I__st_byte_t const * old_group_start;
    I__st_kvp_t const *  old_kvp_start;

    I__st_ptr_t    next_tbl_mem;
    I__st_byte_t * next_group_start;
    I__st_kvp_t *  next_kvp_start;

    I__st_obj_size_t i, next_group_mask, old_group_mask, old_nelem;

    uint32_t robust_resize_counter;
    if (I__st_robust_resize) {
        I__st_choose_expr(I__st_robust_resize, robust_resize_counter = 3,
                          (void)(robust_resize_counter));

    super_unlikely_restart:
        (void)(0);
    }

    old_group_mask  = tbl->group_mask;
    next_group_mask = I__st_next_group_mask(old_group_mask);

    next_tbl_mem     = (I__st_ptr_t)I__st_alloc_tbl(next_group_mask);
    next_group_start = (I__st_byte_t *)next_tbl_mem;

    old_nelem       = I__st_group_mask_to_nelem(old_group_mask);
    old_tbl_mem     = tbl->mem;
    old_group_start = (I__st_byte_t const *)old_tbl_mem;

    tbl->group_mask = next_group_mask;
    tbl->mem        = next_tbl_mem;

    old_kvp_start = (I__st_kvp_t const *)I__st_agu(
        old_tbl_mem, I__st_nelem_to_kvp_offset(old_nelem));
    next_kvp_start = (I__st_kvp_t *)I__st_agu_t(
        next_tbl_mem, I__st_group_mask_to_nelem(next_group_mask));

    for (i               = 0; i < old_nelem; i += I__st_group_size,
        next_group_start = I__st_agu_t(next_group_start, I__st_group_size),
        next_kvp_start   = I__st_agu_t(
              next_kvp_start, I__st_tag_sz_to_kvp_sz(I__st_group_size))) {
        uint32_t j;
        uint8_t  next_offsets[I__st_matchers_per_group * 2] = { 0 };
        I__st_initialize_group(next_group_start);
        I__st_initialize_group(I__st_agu_t(next_group_start, old_nelem));

        for (j = 0; j < I__st_group_size; j += sizeof(I__st_matcher_t),
            old_group_start = I__st_agu_t(old_group_start,
                                          sizeof(I__st_matcher_t)),
            old_kvp_start   = I__st_agu_t(
                  old_kvp_start,
                  I__st_tag_sz_to_kvp_sz(sizeof(I__st_matcher_t)))) {

            I__st_matcher_t old_tbl_matcher;
            uint32_t        old_matches;
            old_tbl_matcher = I__st_matcher_load_mem(old_group_start);

            old_matches = I__st_matcher_find_inuse(old_tbl_matcher);

            if (I__st_likely(old_matches)) {

                do {
                    uint32_t            old_offset = I__st_ctz(old_matches);
                    I__st_kvp_t const * old_kvp_pos =
                        I__st_agu_t(old_kvp_start,
                                    I__st_tag_offset_to_kvp_offset(old_offset));

                    uint32_t old_tag = I__st_get_tag_at(
                        I__st_agu_t(old_group_start, old_offset));

                    assert(!(old_tag & I__st_tag_empty));
                    I__st_obj_size_t rehashval = I__st_rehash(old_kvp_pos);
                    I__st_obj_size_t rehash_next_tag_offset =
                        I__st_hashval_to_resize_tag_offset(rehashval,
                                                           old_nelem);

                    uint32_t next_offset_idx =
                        I__st_hashval_to_resize_offsets_idx(
                            rehashval, rehash_next_tag_offset);


                    uint32_t inner_i = 0;
                    for (;;) {

                        uint32_t next_match_offset =
                            next_offsets[next_offset_idx];

                        if (I__st_likely(next_match_offset !=
                                         sizeof(I__st_matcher_t))) {
                            I__st_obj_size_t next_full_offset =
                                rehash_next_tag_offset +
                                (next_offset_idx / 2) *
                                    sizeof(I__st_matcher_t) +
                                next_match_offset;

                            I__st_byte_t * inner_next_group_pos =
                                I__st_agu_t(next_group_start, next_full_offset);
                            I__st_kvp_t * inner_next_kvp_pos = I__st_agu_t(
                                next_kvp_start, I__st_tag_offset_to_kvp_offset(
                                                    next_full_offset));


                            I__st_set_tag_at(inner_next_group_pos, old_tag);

                            I__st_kvp_move(inner_next_kvp_pos, old_kvp_pos);

                            assert(next_match_offset < sizeof(I__st_matcher_t));
                            I__st_guarantee(next_match_offset <
                                            sizeof(I__st_matcher_t));
                            next_offsets[next_offset_idx] =
                                next_match_offset + 1;

                            break;
                        }

                        if (I__st_unlikely(inner_i ==
                                           I__st_matchers_per_group * 2 - 2)) {
                            assert(0);
                        }
                        inner_i += 2;
#if 0
                        next_offset_idx ^= I__st_lsb(inner_i);
#else
                        next_offset_idx += 2;
                        next_offset_idx &= (I__st_matchers_per_group * 2 - 1);
#endif
                    }

                } while (old_matches &= (old_matches - 1));
            }
        }
    }

    I__st_free_tbl(old_tbl_mem, old_nelem);
    {
        I__st_obj_size_t hashval, group_mask, group_offset, kvp_offset;
        I__st_ptr_t      tbl_mem;
        I__st_byte_t *   group_start;
        I__st_kvp_t *    kvp_start;
        uint32_t         match_offset, i, tag;

        hashval      = I__st_hashret_get_hashval(hashret);
        group_mask   = tbl->group_mask;
        group_offset = I__st_hashval_to_group_offset(hashval, group_mask);
        kvp_offset   = I__st_group_info_to_kvp_offset(group_offset, group_mask);

        tbl_mem     = tbl->mem;
        group_start = (I__st_byte_t *)tbl_mem;
        kvp_start   = (I__st_kvp_t *)I__st_agu_t(tbl_mem, kvp_offset);
        group_start = I__st_agu_t(group_start, group_offset);
        tag         = I__st_hashval_to_tag(hashval);

        match_offset = I__st_hashval_to_match_offset(hashval);
        i            = 0;
        for (;;) {
            I__st_matcher_t tbl_matcher;
            uint32_t        matches, offset;
            I__st_kvp_t *   key_pos;

            tbl_matcher =
                I__st_matcher_load_mem(I__st_agu_t(group_start, match_offset));

            matches = I__st_matcher_find_empty(tbl_matcher);
            if (I__st_likely(matches)) {
                offset = I__st_ctz(matches);
                I__st_guarantee(offset <= sizeof(I__st_matcher_t));

                I__st_set_tag_at(
                    I__st_agu_t(group_start, match_offset + offset), tag);
                key_pos = I__st_agu_t(kvp_start,
                                      I__st_get_kvp_pos(match_offset, offset));
                I__st_key_set(key_pos, usr_pass_key, hashret);

                return (I__st_insert_ret_t){ key_pos, 0 };
            }


            if (I__st_unlikely(i ==
                               I__st_group_size - sizeof(I__st_matcher_t))) {
                I__st_assert_const_eval(I__st_robust_resize);
                if (I__st_robust_resize) {
                    if (--robust_resize_counter) {
                        goto super_unlikely_restart;
                    }
                    I__st_assert(0 && "Unable to resize!\n");
                }
            }

            i += sizeof(I__st_matcher_t);
            match_offset = I__st_get_next_match_offset(match_offset, i);
        }
    }

    I__st_unreachable();
}

static I__st_insert_ret_t
I__st_insert(I__st_hashtbl_t * tbl, const I__st_pass_key_t usr_pass_key) {
    I__st_ptr_t    tbl_mem;
    I__st_byte_t * group_start;
    I__st_kvp_t *  kvp_start;

    I__st_obj_size_t hashval, group_mask, group_offset, kvp_offset;
    uint32_t         match_offset, tag, i;
    I__st_matcher_t  tag_matcher;
    I__st_hashret_t  hashret = I__st_hash(usr_pass_key);
    hashval                  = I__st_hashret_get_hashval(hashret);

    group_mask   = tbl->group_mask;
    group_offset = I__st_hashval_to_group_offset(hashval, group_mask);
    tbl_mem      = tbl->mem;

    group_start = (I__st_byte_t *)I__st_agu(tbl_mem, group_offset);

    I__st_prefetch_L1(group_start);

    tag         = I__st_hashval_to_tag(hashval);
    tag_matcher = I__st_matcher_init_tag(tag);

    match_offset = I__st_hashval_to_match_offset(hashval);

    kvp_offset = I__st_group_info_to_kvp_offset(group_offset, group_mask);
    kvp_start  = (I__st_kvp_t *)I__st_agu(tbl_mem, kvp_offset);


    i = 0;
    for (;;) {
        I__st_matcher_t tbl_matcher;
        uint32_t        matches, offset;
        I__st_kvp_t *   key_pos;


        tbl_matcher =
            I__st_matcher_load_mem(I__st_agu_t(group_start, match_offset));

        matches = I__st_matcher_find_matches(tag_matcher, tbl_matcher);
        if (matches) {
            do {
                offset = I__st_ctz(matches);

                I__st_guarantee(offset < sizeof(I__st_matcher_t));

                key_pos = I__st_agu_t(kvp_start,
                                      I__st_get_kvp_pos(match_offset, offset));

                if (I__st_key_cmpeq(key_pos, usr_pass_key, hashret)) {
                    return (I__st_insert_ret_t){ key_pos, 1 };
                }
            } while (matches &= (matches - 1));
        }

        matches = I__st_matcher_find_empty(tbl_matcher);
        if (I__st_likely(matches)) {
            offset = I__st_ctz(matches);

            I__st_guarantee(offset <= sizeof(I__st_matcher_t));

            I__st_set_tag_at(I__st_agu_t(group_start, match_offset + offset),
                             tag);
            key_pos =
                I__st_agu_t(kvp_start, I__st_get_kvp_pos(match_offset, offset));
            I__st_key_set(key_pos, usr_pass_key, hashret);

            return (I__st_insert_ret_t){ key_pos, 0 };
        }

        if (I__st_unlikely(i == I__st_group_size - sizeof(I__st_matcher_t))) {
            return I__st_resize(tbl, usr_pass_key, hashret);
        }
        i += sizeof(I__st_matcher_t);
        match_offset = I__st_get_next_match_offset(match_offset, i);
    }
    I__st_unreachable();
}

static I__st_find_ret_t __attribute__((pure))
I__st_find(I__st_hashtbl_t const * tbl, const I__st_pass_key_t usr_pass_key) {
    I__st_ptr_t          tbl_mem;
    I__st_byte_t const * group_start;
    I__st_kvp_t *        kvp_start;

    I__st_obj_size_t hashval, group_mask, group_offset, kvp_offset;
    uint32_t         match_offset, tag, i;
    I__st_matcher_t  tag_matcher;
    I__st_hashret_t  hashret = I__st_hash(usr_pass_key);

    hashval = I__st_hashret_get_hashval(hashret);

    group_mask   = tbl->group_mask;
    group_offset = I__st_hashval_to_group_offset(hashval, group_mask);
    tbl_mem      = tbl->mem;

    group_start = (I__st_byte_t const *)I__st_agu(tbl_mem, group_offset);

    I__st_prefetch_L1(group_start);

    tag         = I__st_hashval_to_tag(hashval);
    tag_matcher = I__st_matcher_init_tag(tag);

    kvp_offset = I__st_group_info_to_kvp_offset(group_offset, group_mask);
    kvp_start  = (I__st_kvp_t *)I__st_agu_t(tbl_mem, kvp_offset);

    match_offset = I__st_hashval_to_match_offset(hashval);

    i = 0;


    for (;;) {
        I__st_matcher_t tbl_matcher;
        uint32_t        matches, offset;
        I__st_kvp_t *   key_pos;


        tbl_matcher =
            I__st_matcher_load_mem(I__st_agu_t(group_start, match_offset));

        matches = I__st_matcher_find_matches(tag_matcher, tbl_matcher);
        if (matches) {
            do {
                offset = I__st_ctz(matches);

                I__st_guarantee(offset <= sizeof(I__st_matcher_t));
                key_pos = I__st_agu_t(kvp_start,
                                      I__st_get_kvp_pos(match_offset, offset));

                if (I__st_key_cmpeq(key_pos, usr_pass_key, hashret)) {
                    return key_pos;
                }
            } while (matches &= (matches - 1));
        }

        matches = I__st_matcher_find_empty(tbl_matcher);
        if (I__st_likely(matches)) {
            return 0;
        }

        if (I__st_unlikely(i == I__st_group_size - sizeof(I__st_matcher_t))) {
            return 0;
        }

        i += sizeof(I__st_matcher_t);
        match_offset = I__st_get_next_match_offset(match_offset, i);
    }
    I__st_unreachable();
}

static I__st_delete_ret_t
I__st_delete(I__st_hashtbl_t const * tbl, const I__st_pass_key_t usr_pass_key) {
    if (!I__st_with_delete) {
        I__st_error("Unsupported");
    }

    I__st_ptr_t    tbl_mem;
    I__st_byte_t * group_start;
    I__st_kvp_t *  kvp_start;

    I__st_obj_size_t hashval, group_mask, group_offset, kvp_offset;
    uint32_t         match_offset, tag, i;
    I__st_matcher_t  tag_matcher;
    I__st_hashret_t  hashret = I__st_hash(usr_pass_key);

    hashval = I__st_hashret_get_hashval(hashret);

    group_mask   = tbl->group_mask;
    group_offset = I__st_hashval_to_group_offset(hashval, group_mask);
    tbl_mem      = tbl->mem;

    group_start = (I__st_byte_t *)I__st_agu(tbl_mem, group_offset);

    I__st_prefetch_L1(group_start);

    tag         = I__st_hashval_to_tag(hashval);
    tag_matcher = I__st_matcher_init_tag(tag);

    kvp_offset = I__st_group_info_to_kvp_offset(group_offset, group_mask);
    kvp_start  = (I__st_kvp_t *)I__st_agu(tbl_mem, kvp_offset);

    match_offset = I__st_hashval_to_match_offset(hashval);

    i = 0;

    for (;;) {
        I__st_matcher_t tbl_matcher;
        uint32_t        matches, offset;
        I__st_kvp_t *   key_pos;

        tbl_matcher =
            I__st_matcher_load_mem(I__st_agu_t(group_start, match_offset));

        matches = I__st_matcher_find_matches(tag_matcher, tbl_matcher);
        if (matches) {
            do {
                offset = I__st_ctz(matches);
                I__st_guarantee(offset <= sizeof(I__st_matcher_t));
                key_pos = I__st_agu_t(kvp_start,
                                      I__st_get_kvp_pos(match_offset, offset));

                if (I__st_key_cmpeq(key_pos, usr_pass_key, hashret)) {
                    group_start =
                        I__st_agu_t(group_start, match_offset + offset);


                    matches = I__st_matcher_find_empty(tbl_matcher);
                    I__st_set_tag_at(group_start, I__st_tag_deleted);
                    if (I__st_likely(matches)) {
                        I__st_no_cmov();
                        I__st_set_tag_at(group_start, I__st_tag_empty);
                    }
                    I__st_kvp_destroy(key_pos, hashret);
                    return key_pos;
                }
            } while (matches &= (matches - 1));
        }

        matches = I__st_matcher_find_empty(tbl_matcher);
        if (I__st_likely(matches)) {
            return 0;
        }

        if (I__st_unlikely(i == I__st_group_size - sizeof(I__st_matcher_t))) {
            return 0;
        }

        i += sizeof(I__st_matcher_t);
        match_offset = I__st_get_next_match_offset(match_offset, i);
    }
    I__st_unreachable();
}
