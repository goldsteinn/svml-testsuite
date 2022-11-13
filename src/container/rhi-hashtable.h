#include "hl-defs.h"
#include "hl-user-defs.h"

/********************************************************************/
/* Table specific tunables.  */
#ifndef rh_tunable_max_lookup_log2
# define rh_tunable_max_lookup_log2 4
#endif

#define rh_namer(name) I_hl_namer(hl_name, name)

/********************************************************************/
/* Initial checks on free fits.  */
I_hl_static_assert(I_hl_sizeof_bits(hl_key_t) > (hl_spare_key_bits),
                   "Invalid number of free bits");

I_hl_static_assert(1 || (I_hl_sizeof_bits(hl_key_t) - hl_spare_key_bits) >= 8,
                   "This is not the proper usage of a hashtable");


#define K_rh_max_lookup_log2 (rh_tunable_max_lookup_log2)
#define K_rh_max_lookup      (1 << (K_rh_max_lookup_log2))


#define K_rh_key_bits_req                                                      \
    (I_hl_sizeof_bits(hl_key_t) - (hl_spare_key_bits) + K_rh_max_lookup_log2)

I_hl_static_assert(K_rh_key_bits_req <= I_hl_sizeof_bits(void *));

#define rh_key_plus_meta_type                                                  \
    I_hl_choose_T(                                                             \
        K_rh_key_bits_req <= 16,                                               \
        I_hl_choose_T(K_rh_key_bits_req <= 8, I_hl_byte_t, I_hl_word_t),       \
        I_hl_choose_T(K_rh_key_bits_req <= 32, I_hl_long_t, I_hl_quad_t))


I_hl_static_assert(1 || (hl_spare_key_bits)*2 <= I_hl_sizeof_bits(hl_key_t));


/********************************************************************/
/* Type initialization.  */
typedef rh_key_plus_meta_type rh_namer(mkey_t);
#define rh_mkey_t rh_namer(mkey_t)


typedef struct rh_namer(kvp) {
    rh_mkey_t mkey_;
#if hl_use_as_map
    hl_val_t val_;
#endif
}
rh_namer(kvp_t);
#define rh_kvp_t rh_namer(kvp_t)

#define rh_kvp_get_key(kvp) rh_mkey_get_key((kvp)->mkey_)

#if hl_use_as_map
# define rh_initialize_kvp(key, ...)                                           \
        (rh_kvp_t) {                                                           \
            (rh_mkey_t){ key }, __VA_ARGS__                                    \
        }
#else
# define rh_initialize_kvp(key, ...)                                           \
        (rh_kvp_t) {                                                           \
            (rh_mkey_t) {                                                      \
                key                                                            \
            }                                                                  \
        }

#endif

#define K_rh_meta_inplace 1
#include "rh-table-types.h"
I_hl_static_assert(rh_key_is_entry || hl_use_as_map || !hl_null_key_invalid);

typedef rh_kvp_t * rh_namer(tbl_ptr_t);
#define rh_tbl_ptr_t rh_namer(tbl_ptr_t)

#define rh_meta_reset_t rh_tbl_ptr_t

typedef struct rh_namer(table) {
    rh_tbl_ptr_t p_;
#if (hl_conf_fixed_size) <= 1
# if (hl_conf_fixed_size)
    const
# endif
        hl_obj_size_t mask_and_size_;
#endif
}
rh_namer(table_t);

#define rh_table_t rh_namer(table_t)


/********************************************************************/
/* Setup constants for remainder.  */

I_hl_static_assert(sizeof(rh_kvp_t) <= sizeof(void *), "KVP size to large");
I_hl_static_assert(sizeof(rh_kvp_t) <= sizeof(void *),
                   "Meta + KVP size to large");

I_hl_static_assert(sizeof(rh_kvp_t) + 2 >= sizeof(rh_kvp_t),
                   "Too much overhead for set to be worth it");

#define K_rh_true_spare_key_bits                                               \
    ((I_hl_sizeof_bits(rh_mkey_t) - I_hl_sizeof_bits(hl_key_t)) +              \
     (hl_spare_key_bits))


#if hl_trivial_compare
# define K_rh_key_upper_def 0
# define K_rh_spare_key_bits_def                                               \
        ((K_rh_true_spare_key_bits > (K_rh_max_lookup_log2 + 0))               \
             ? (K_rh_max_lookup_log2 + 1)                                      \
             : K_rh_max_lookup_log2)

I_hl_static_assert(K_rh_spare_key_bits_def >= (K_rh_max_lookup_log2));
# define K_rh_meta_is_subtype_def 0
# define K_rh_meta_is_signed_end_def                                           \
        ((K_rh_spare_key_bits_def == (K_rh_max_lookup_log2 + 1)) &&            \
         (sizeof(rh_mkey_t) == sizeof(void *)))
#else
# define K_rh_spare_key_bits_def K_rh_true_spare_key_bits

# define K_rh_meta_is_subtype_def                                              \
        ((K_rh_spare_key_bits_def == 8) || (K_rh_spare_key_bits_def == 16) ||  \
         (K_rh_spare_key_bits_def == 32))

# define K_rh_key_upper_def                                                    \
        (sizeof(rh_mkey_t) == sizeof(void *) || (K_rh_meta_is_subtype_def))
# define K_rh_meta_is_signed_end_def 0

#endif

enum { rh_namer(spare_key_bits) = K_rh_spare_key_bits_def };
#define K_rh_spare_key_bits rh_namer(spare_key_bits)

enum {
    rh_namer(has_spare_meta_bit) = K_rh_spare_key_bits != K_rh_max_lookup_log2
};
#define K_rh_has_spare_meta_bit rh_namer(has_spare_meta_bit)


enum { rh_namer(key_upper) = K_rh_key_upper_def };
#define K_rh_key_upper rh_namer(key_upper)

enum { rh_namer(meta_is_subtype) = K_rh_meta_is_subtype_def };
#define K_rh_meta_is_subtype rh_namer(meta_is_subtype)

enum { rh_namer(meta_is_signed_end) = K_rh_meta_is_signed_end_def };
#define K_rh_meta_is_signed_end rh_namer(meta_is_signed_end)

I_hl_static_assert(K_rh_meta_is_signed_end ? !K_rh_key_upper : 1);


I_hl_static_assert(K_rh_spare_key_bits < I_hl_sizeof_bits(rh_mkey_t));
I_hl_static_assert(sizeof(rh_mkey_t) >= sizeof(hl_key_t));


#define K_rh_key_bits  (I_hl_sizeof_bits(rh_mkey_t) - K_rh_spare_key_bits)
#define K_rh_meta_bits (K_rh_spare_key_bits)

I_hl_static_assert(K_rh_meta_bits <= 32);
I_hl_static_assert(K_rh_meta_bits >= K_rh_max_lookup_log2);


#if hl_trivial_compare
# define K_rh_meta_hash_bits 0
I_hl_static_assert(!K_rh_key_upper);
#else
# define K_rh_meta_hash_bits (K_rh_meta_bits - K_rh_max_lookup_log2)
# define K_rh_meta_hash_shift                                                  \
        (I_hl_sizeof_bits(hl_obj_size_t) - K_rh_meta_hash_bits)

# define K_rh_meta_hash_mask      (((1UL << K_rh_meta_hash_bits) - 1))
# define K_rh_meta_init_hash_mask (K_rh_meta_hash_mask << K_rh_key_bits)

I_hl_static_assert(K_rh_key_upper || K_rh_meta_init_hash_mask < (1UL << 32));
#endif

I_hl_static_assert(K_rh_key_bits < I_hl_sizeof_bits(void *));
I_hl_static_assert(K_rh_meta_bits < I_hl_sizeof_bits(void *));


#define K_rh_key_is_subtype                                                    \
    (!K_rh_key_upper &&                                                        \
     ((K_rh_key_bits == 8) || (K_rh_key_bits == 16) || (K_rh_key_bits == 32)))

#define K_rh_cmp_meta_inplace (!K_rh_key_upper || K_rh_meta_is_subtype)


typedef I_hl_as_member_type(rh_mkey_t, val_) rh_namer(mkey_uint_t);
#define rh_mkey_uint_t rh_namer(mkey_uint_t)

I_hl_static_assert(hl_trivial_compare ? !K_rh_meta_is_subtype : 1);
#define rh_metav_best_type                                                     \
    I_hl_choose_T(K_rh_meta_is_subtype, I_hl_uint_for_bits(K_rh_meta_bits),    \
                  rh_mkey_uint_t)

#define rh_metav_best_tmp_type                                                 \
    I_hl_choose_T(sizeof(rh_metav_best_type) <= sizeof(uint32_t), uint32_t,    \
                  uint64_t)

#define rh_keyv_best_type                                                      \
    I_hl_choose_T(K_rh_key_is_subtype, I_hl_uint_for_bits(K_rh_key_bits),      \
                  rh_mkey_uint_t)


typedef rh_metav_best_type rh_namer(metav_t);
#define rh_metav_t rh_namer(metav_t)

typedef rh_metav_best_tmp_type rh_namer(metav_tmp_t);
#define rh_metav_tmp_t rh_namer(metav_tmp_t)

typedef rh_keyv_best_type rh_namer(keyv_t);
#define rh_keyv_t rh_namer(keyv_t)

I_hl_static_assert(sizeof(rh_mkey_uint_t) >= sizeof(hl_key_t));

#define K_rh_meta_mask                                                         \
    (((1UL << K_rh_meta_bits) - 1) << (K_rh_key_upper ? 0 : K_rh_key_bits))
#define K_rh_key_mask (((1UL << K_rh_key_bits) - 1))

#define K_rh_meta_distance_incr_def                                            \
    (K_rh_key_upper ? (1UL << K_rh_meta_hash_bits)                             \
                    : (1UL << (I_hl_sizeof_bits(rh_mkey_t) -                   \
                               (K_rh_meta_bits - K_rh_meta_hash_bits))))


/* Allow enum exceeding int capacity.  */
#if I_hl_USING_LLVM
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wpedantic"
#endif
enum { rh_namer(meta_distance_incr) = K_rh_meta_distance_incr_def };
#if I_hl_USING_LLVM
# pragma clang diagnostic pop
#endif
#define K_rh_meta_distance_incr rh_namer(meta_distance_incr)


I_hl_static_assert(K_rh_meta_distance_incr == (K_rh_meta_distance_incr_def));

#define K_rh_meta_max_def                                                      \
    I_hl_p2_m(K_rh_key_upper                                                   \
                  ? (K_rh_max_lookup_log2 + K_rh_meta_hash_bits)               \
                  : I_hl_sizeof_bits(rh_mkey_t) - K_rh_has_spare_meta_bit,     \
              K_rh_meta_distance_incr)

/* Allow enum exceeding int capacity.  */
#if I_hl_USING_LLVM
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wpedantic"
#endif
enum { rh_namer(meta_max) = K_rh_meta_max_def };
#if I_hl_USING_LLVM
# pragma clang diagnostic pop
#endif
#define K_rh_meta_max rh_namer(meta_max)
I_hl_static_assert(K_rh_meta_max == (K_rh_meta_max_def));

#define K_rh_default_size (64)

#if I_hl_USING_LLVM
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wpedantic"
#endif
I_hl_assert_const_eval(K_rh_cmp_meta_inplace);
I_hl_assert_const_eval(K_rh_default_size);
I_hl_assert_const_eval(K_rh_has_spare_meta_bit);
I_hl_assert_const_eval(K_rh_key_bits);
I_hl_assert_const_eval(K_rh_key_bits_req);
I_hl_assert_const_eval(K_rh_key_is_subtype);
I_hl_assert_const_eval(K_rh_key_mask);
I_hl_assert_const_eval(K_rh_key_upper);
I_hl_assert_const_eval(K_rh_key_upper_def);
I_hl_assert_const_eval(K_rh_max_lookup);
I_hl_assert_const_eval(K_rh_max_lookup_log2);
I_hl_assert_const_eval(K_rh_meta_bits);
I_hl_assert_const_eval(K_rh_meta_distance_incr);
I_hl_assert_const_eval(K_rh_meta_distance_incr_def);
I_hl_assert_const_eval(K_rh_meta_hash_bits);
#ifdef K_rh_meta_hash_mask
I_hl_assert_const_eval(K_rh_meta_hash_mask);
#endif
#ifdef K_rh_meta_hash_shift
I_hl_assert_const_eval(K_rh_meta_hash_shift);
#endif
#ifdef K_rh_meta_init_hash_mask
I_hl_assert_const_eval(K_rh_meta_init_hash_mask);
#endif
I_hl_assert_const_eval(K_rh_meta_inplace);
I_hl_assert_const_eval(K_rh_meta_is_signed_end);
I_hl_assert_const_eval(K_rh_meta_is_signed_end_def);
I_hl_assert_const_eval(K_rh_meta_is_subtype);
I_hl_assert_const_eval(K_rh_meta_is_subtype_def);
I_hl_assert_const_eval(K_rh_meta_mask);
I_hl_assert_const_eval(K_rh_meta_max);
I_hl_assert_const_eval(K_rh_meta_max_def);
I_hl_assert_const_eval(K_rh_spare_key_bits);
I_hl_assert_const_eval(K_rh_spare_key_bits_def);
I_hl_assert_const_eval(K_rh_true_spare_key_bits);
#if I_hl_USING_LLVM
# pragma clang diagnostic pop
#endif

/********************************************************************/
/* minfo <-> kvp pointer helpers.  */

#define rh_mkey_get_key               rh_namer(mkey_get_key)
#define rh_mkey_meta_lt               rh_namer(mkey_meta_lt)
#define rh_mkey_meta_eq               rh_namer(mkey_meta_eq)
#define rh_mkey_eq                    rh_namer(mkey_eq)
#define rh_mkey_empty                 rh_namer(mkey_empty)
#define rh_mkey_finalize_meta         rh_namer(mkey_finalize_meta)
#define rh_check_meta_at_end          rh_namer(check_meta_at_end)
#define rh_check_mkey_at_end_plus_one rh_namer(check_mkey_at_end_plus_one)
#define rh_finish_combine_meta        rh_namer(finish_combine_meta)

I_hl_cattrs hl_key_t
rh_mkey_get_key(rh_mkey_t mkey) {
    if (K_rh_key_upper) {
        return (hl_key_t)(mkey.val_ >> K_rh_meta_bits);
    }
    else {
        return (hl_key_t)(mkey.val_ & K_rh_key_mask);
    }
}

I_hl_cattrs I_hl_bool_t
rh_mkey_meta_lt(rh_metav_tmp_t lhsv, rh_mkey_t rhs) {
    rh_metav_t lhs = I_hl_CAST(rh_metav_t, lhsv);
#if hl_trivial_compare
    return lhs < rhs.val_;
#else
    if (K_rh_key_upper) {
        return lhs < (rhs.val_ & K_rh_meta_mask);
    }
    else {
# if 0
        return (lhs + K_rh_meta_distance_incr) <= rhs.val_;
# else
        return lhs < (rhs.val_ & K_rh_meta_mask);
# endif
    }
#endif
}

I_hl_cattrs I_hl_bool_t
rh_mkey_meta_eq(rh_metav_tmp_t lhsv, rh_mkey_t rhs) {
    rh_metav_t lhs = I_hl_CAST(rh_metav_t, lhsv);
#if hl_trivial_compare
    I_hl_static_assert(sizeof(lhsv) >= sizeof(lhs));
    return lhs == rhs.val_;
#else
    I_hl_static_assert(K_rh_meta_mask < (1UL << 32));
    return lhs == (rhs.val_ & K_rh_meta_mask);
#endif
}


I_hl_pattrs I_hl_bool_t
rh_mkey_eq(I_hl_dconst hl_pass_key_t pk,
           const rh_mkey_t           rhs,
           const hl_hashret_t        hr) {
#if hl_trivial_compare
    (void)(pk);
    (void)(rhs);
    (void)(hr);
    return 1;
#else
    (void)(hr);
    return (hl_key_eq(rh_mkey_get_key(rhs), pk, hr) == 0);
#endif
}

I_hl_cattrs I_hl_bool_t
rh_mkey_empty(const rh_mkey_t mkey) {
    return mkey.val_ == 0;
}


I_hl_cattrs rh_mkey_t
rh_mkey_finalize_meta(rh_mkey_t mkey, hl_key_t key, rh_metav_tmp_t metav) {
    rh_mkey_uint_t meta = I_hl_CAST(rh_mkey_uint_t, metav);
#if hl_trivial_compare
    (void)(key);
    mkey.val_ = meta;
#else
    if (K_rh_key_upper) {
        mkey.val_ = (mkey.val_ & (~((rh_mkey_uint_t)K_rh_meta_mask))) |
                    ((rh_mkey_uint_t)meta);
    }
    else {
        mkey.val_ = ((rh_mkey_uint_t)key) | meta;
    }
#endif
    return mkey;
}

I_hl_cattrs I_hl_bool_t
rh_check_meta_at_end(rh_metav_tmp_t metav) {
    rh_metav_t meta = I_hl_CAST(rh_metav_t, metav);
    if (K_rh_meta_is_signed_end) {
        return (meta & (1UL << (I_hl_sizeof_bits(rh_metav_t) - 1))) ? 1 : 0;
    }
    else {
        return meta >= (K_rh_meta_max - 1 * K_rh_meta_distance_incr);
    }
}


I_hl_cattrs I_hl_bool_t
rh_check_mkey_at_end_plus_one(rh_mkey_t mkey) {
#if hl_trivial_compare
    if (K_rh_meta_is_signed_end) {
        return (mkey.val_ & (1UL << (I_hl_sizeof_bits(rh_mkey_t) - 1))) ? 1 : 0;
    }
    else {
        return mkey.val_ >= (K_rh_meta_max - 2 * K_rh_meta_distance_incr);
    }
#else
    return (mkey.val_ & K_rh_meta_mask) >=
           (K_rh_meta_max - 2 * K_rh_meta_distance_incr);
#endif
}


I_hl_cattrs rh_mkey_uint_t
rh_finish_combine_meta(rh_metav_tmp_t metav, rh_mkey_uint_t key) {
    rh_mkey_uint_t meta = I_hl_CAST(rh_mkey_uint_t, metav);
#if hl_trivial_compare
    (void)(key);
    return meta;
#else
    if (K_rh_key_upper) {
        return (key << K_rh_meta_bits) + meta;
    }
    else {
        return meta + key;
    }
#endif
}

/********************************************************************/
/* Common methods.  */
#include "rh-table-common.h"


/********************************************************************/
/* Helpers for manipulating hash value.  */
#define rh_init_meta      rh_namer(init_meta)
#define rh_get_kvps_start rh_namer(get_kvps_start)

I_hl_cattrs rh_metav_tmp_t
rh_init_meta(hl_obj_size_t hash_val, rh_mkey_uint_t key) {
    rh_metav_tmp_t metav;
#if hl_trivial_compare
    (void)(hash_val);
    metav = key;
    {
        I_hl_static_assert(!K_rh_key_upper);
        I_hl_static_assert(sizeof(metav) >= sizeof(key));
    }
#else
    (void)(key);
    if (K_rh_key_upper) {
        I_hl_static_assert(K_rh_meta_hash_bits == 0 ||
                           (K_rh_meta_hash_shift < sizeof_bits(hash_val)));
        metav = K_rh_meta_hash_bits
                    ? I_hl_CAST(
                          rh_metav_tmp_t,
                          (hash_val) >>
                              (K_rh_meta_hash_bits ? K_rh_meta_hash_shift : 0))
                    : 0;
    }
    else {
        metav =
            I_hl_CAST(rh_metav_tmp_t, (hash_val) & (K_rh_meta_init_hash_mask));
    }
#endif
    return metav + K_rh_meta_distance_incr;
}

I_hl_cattrs rh_kvp_t *
rh_get_kvps_start(rh_kvp_t *    p,
                  hl_obj_size_t mask_and_size,
                  hl_obj_size_t hash_val) {
    return p + rh_get_hash_idx(mask_and_size, hash_val);
}


/********************************************************************/
/* Insert.  */
#define rh_insert_at          rh_namer(insert_at)
#define rh_insert_no_conflict rh_namer(insert_no_conflict)
#define rh_shift_indexes      rh_namer(shift_indexes)
#define rh_resize             rh_namer(resize)
#define rh_insert             rh_namer(insert)


I_hl_attrs rh_insert_ret_t
rh_insert_at(rh_kvp_t *                   kvps,
             rh_metav_tmp_t               metav,
             rh_mkey_uint_t               key,
             I_hl_dconst hl_pass_key_t pk I_hl_unused,
             const hl_hashret_t hr        I_hl_unused) {
    rh_mkey_t      mkey;
    rh_mkey_uint_t meta;
    hl_kvp_set_prepare(pk, hr);
    meta = rh_finish_combine_meta(metav, key);

    mkey = (rh_mkey_t){ meta };
#ifdef hl_pass_key_extract_val
    *kvps = rh_initialize_kvp(mkey, hl_pass_key_extract_val(pk));
#else
    kvps->mkey_ = mkey;
#endif

    return rh_insert_make_success_return(key, mkey, kvps);
}

I_hl_attrs I_hl_bool_t
rh_shift_indexes(rh_kvp_t * restrict kvps) {
    rh_kvp_t * restrict kvps_place = kvps;
    ++kvps;
    for (;;) {

        rh_mkey_t mkey = (*kvps).mkey_;
        if (rh_mkey_empty(mkey)) {
            break;
        }
        if (I_hl_unlikely(rh_check_mkey_at_end_plus_one(mkey))) {
            return 1;
        }

        ++kvps;
    }

    I_hl_guarantee(kvps > kvps_place);
    for (; I_hl_unlikely(kvps > kvps_place); --kvps) {
        rh_kvp_t kvp = *(kvps - 1);

        kvp.mkey_.val_ += K_rh_meta_distance_incr;
        *kvps = kvp;
        __asm__ volatile("" : : :);
    }
    I_hl_guarantee(kvps == kvps_place);
    return 0;
}

I_hl_attrs void
rh_insert_no_conflict(rh_kvp_t *     kvps,
                      hl_key_t       key,
                      rh_metav_tmp_t metav,
                      rh_kvp_t       reinsert_kvp) {
    rh_mkey_t existing_mkey;
    while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {
        metav += K_rh_meta_distance_incr;

        ++kvps;
    }

    if (!rh_mkey_empty(existing_mkey)) {
        rh_kvp_t * place_kvp = kvps;
        do {
            ++kvps;
        } while (!rh_mkey_empty(kvps->mkey_));
        I_hl_guarantee(kvps > place_kvp);
        for (; I_hl_unlikely(kvps > place_kvp); --kvps) {
            rh_kvp_t kvp = *(kvps - 1);
            kvp.mkey_.val_ += K_rh_meta_distance_incr;
            *kvps = kvp;
        }
        I_hl_guarantee(kvps == place_kvp);
    }

    reinsert_kvp.mkey_ = rh_mkey_finalize_meta(reinsert_kvp.mkey_, key, metav);
    *kvps              = reinsert_kvp;
}

#if !(hl_conf_fixed_size)
static I_hl_noinline rh_insert_ret_t
rh_resize(rh_table_t *              rh_tbl,
          I_hl_dconst hl_pass_key_t pk,
          const hl_hashret_t        hr) {
    rh_kvp_t *      next_table;
    rh_insert_ret_t ret;
    hl_obj_size_t   mask_and_size    = rh_tbl_get_mask_and_size(rh_tbl);
    hl_obj_size_t next_mask_and_size = rh_mask_and_size_get_next(mask_and_size);
# if hl_conf_robust_resize && hl_conf_retry_max
    int32_t retry_counter;
    retry_counter = 0;
# endif
    (void)(pk);
    (void)(hr);
super_unlikely_retry:
    I_hl_unused;


    {
        next_table =
            rh_alloc_table(rh_mask_and_size_get_alloc_sz(next_mask_and_size));
        /* The only true error case we can have. */
        if (I_hl_unlikely(next_table == 0UL)) {
            rh_on_error("Allocation error");
            return rh_internal_insert_make_return(hl_operation_failure,
                                                  rh_invalid_return_entry);
        }
    }


    {
        rh_kvp_t *old_kvps, *old_kvps_end;
        {
            old_kvps     = rh_tbl->p_;
            old_kvps_end = old_kvps + rh_mask_and_size_get_cap(mask_and_size);
        }
        I_hl_guarantee(old_kvps < old_kvps_end);
        for (; old_kvps < old_kvps_end; ++old_kvps) {
            rh_kvp_t existing_kvp = *old_kvps;
            if (I_hl_likely(!rh_mkey_empty(existing_kvp.mkey_))) {
                hl_key_t       key      = rh_mkey_get_key(existing_kvp.mkey_);
                hl_obj_size_t  hash_val = hl_rehash(key);
                rh_metav_tmp_t metav =
                    rh_init_meta(hash_val, I_hl_CAST(rh_mkey_uint_t, key));
                I_hl_static_assert(sizeof(hl_key_t) <= sizeof(rh_mkey_uint_t));


                rh_kvp_t * insert_kvps =
                    rh_get_kvps_start(next_table, next_mask_and_size, hash_val);


                rh_insert_no_conflict(insert_kvps, key, metav, existing_kvp);
            }
        }
    }

    {
        rh_mkey_t      existing_mkey;
        hl_obj_size_t  hash_val = hl_hashret_get_hashval(hr);
        rh_mkey_uint_t key;
        rh_metav_tmp_t metav;

        rh_kvp_t * kvps;
        key  = I_hl_CAST(rh_mkey_uint_t, hl_pass_key_extract_key(pk));
        kvps = rh_get_kvps_start(next_table, next_mask_and_size, hash_val);

        metav = rh_init_meta(hash_val, key);
        while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {
            metav += K_rh_meta_distance_incr;
            ++kvps;
        }

# if hl_conf_robust_resize
        if (I_hl_unlikely(rh_check_meta_at_end(metav) ||
                          (!rh_mkey_empty(existing_mkey) &&
                           (rh_check_mkey_at_end_plus_one(existing_mkey) ||
                            rh_shift_indexes(kvps))))) {

            rh_free_table_mask_and_size(next_table, next_mask_and_size);
            next_mask_and_size = rh_mask_and_size_get_next(next_mask_and_size);


#  if hl_conf_retry_max
            if (I_hl_unlikely(retry_counter >= (hl_conf_retry_max))) {
                rh_on_error("Unable to resize table");
                return rh_internal_insert_make_return(hl_operation_failure,
                                                      rh_invalid_return_entry);
            }
            ++retry_counter;
#  endif
            goto super_unlikely_retry;
        }
# else
        if (!rh_mkey_empty(existing_mkey)) {
            rh_shift_indexes(kvps);
        }
# endif
        ret = rh_insert_at(kvps, metav, key, pk, hr);
    }


    rh_free_table_mask_and_size(rh_tbl->p_, mask_and_size);
    rh_tbl->mask_and_size_ = next_mask_and_size;
    rh_tbl->p_             = next_table;
    return ret;
}
#endif

static rh_insert_ret_t
rh_insert(rh_table_t * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    rh_kvp_t *     kvps;
    rh_mkey_uint_t key;
    rh_metav_tmp_t metav;
    rh_mkey_t      existing_mkey;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    key = I_hl_CAST(rh_mkey_uint_t, hl_pass_key_extract_key(pk));
    {
        kvps = rh_get_kvps_start(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl),
                                 hash_val);

        I_hl_prefetch(kvps);
        metav = rh_init_meta(hash_val, key);
    }

    while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {

        metav += K_rh_meta_distance_incr;
        ++kvps;
    }

    while (I_hl_unlikely(rh_mkey_meta_eq(metav, existing_mkey))) {
        if (I_hl_likely(rh_mkey_eq(pk, existing_mkey, hr))) {
            return rh_insert_make_fail_return(key, existing_mkey, kvps);
        }
        metav += K_rh_meta_distance_incr;
        ++kvps;
        existing_mkey = kvps->mkey_;
    }

    if (I_hl_unlikely(rh_check_meta_at_end(metav))) {
        return rh_resize(rh_tbl, pk, hr);
    }

    if (!rh_mkey_empty(existing_mkey)) {
        if (I_hl_unlikely(rh_check_mkey_at_end_plus_one(existing_mkey) ||
                          rh_shift_indexes(kvps))) {
            return rh_resize(rh_tbl, pk, hr);
        }
    }
    return rh_insert_at(kvps, metav, key, pk, hr);
}

/********************************************************************/
/* Find API.  */
#define rh_find             rh_namer(find)
#define rh_find_exists      rh_namer(find_exists)
#define rh_find_exists_impl rh_namer(find_exists_impl)

static I_hl_pure rh_find_ret_t
rh_find(rh_table_t const * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    rh_kvp_t *     kvps;
    rh_mkey_uint_t key;
    rh_metav_tmp_t metav;
    rh_mkey_t      existing_mkey;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    key = I_hl_CAST(rh_mkey_uint_t, hl_pass_key_extract_key(pk));
    {
        kvps = rh_get_kvps_start(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl),
                                 hash_val);

        I_hl_prefetch(kvps);
        metav = rh_init_meta(hash_val, key);
    }

    while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {

        metav += K_rh_meta_distance_incr;
        ++kvps;
    }

    while (I_hl_likely(rh_mkey_meta_eq(metav, existing_mkey))) {
        if (I_hl_likely(rh_mkey_eq(pk, existing_mkey, hr))) {
            return rh_get_valid_success_entry(key, existing_mkey, kvps);
        }
        metav += K_rh_meta_distance_incr;
        ++kvps;
        existing_mkey = kvps->mkey_;
    }

    /* Avoid generating cmovcc.  */
#if hl_conf_predict_fret
    I_hl_no_cmov();
#endif
    return rh_invalid_return_entry;
}


static I_hl_pure rh_find_ret_t
rh_find_exists_impl(rh_table_t const * restrict rh_tbl,
                    I_hl_dconst hl_pass_key_t pk) {
    rh_kvp_t *     kvps;
    rh_mkey_uint_t key;
    rh_metav_tmp_t metav;
    rh_mkey_t      existing_mkey;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    key = I_hl_CAST(rh_mkey_uint_t, hl_pass_key_extract_key(pk));
    {
        kvps = rh_get_kvps_start(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl),
                                 hash_val);

        I_hl_prefetch(kvps);
        metav = rh_init_meta(hash_val, key);
    }

    while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {

        metav += K_rh_meta_distance_incr;
        ++kvps;
    }

    while (I_hl_likely(rh_mkey_meta_eq(metav, existing_mkey))) {
        if (I_hl_likely(rh_mkey_eq(pk, existing_mkey, hr))) {
            return rh_get_valid_success_entry(key, existing_mkey, kvps);
        }
        metav += K_rh_meta_distance_incr;
        ++kvps;
        existing_mkey = kvps->mkey_;
    }

    rh_unreachable();
}
I_hl_pattrs rh_find_ret_t
rh_find_exists(rh_table_t const * restrict rh_tbl,
               I_hl_dconst hl_pass_key_t pk) {
    rh_find_ret_t fret = rh_find_exists_impl(rh_tbl, pk);
    if (rh_fret_fail(fret)) {
        I_hl_unreachable();
    }
    return fret;
}

/********************************************************************/
/* Erase API.  */
#define rh_erase rh_namer(erase)
static rh_erase_ret_t
rh_erase(rh_table_t const * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    rh_kvp_t *     kvps;
    rh_mkey_uint_t key;
    rh_metav_tmp_t metav;
    rh_mkey_t      existing_mkey;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    key = I_hl_CAST(rh_mkey_uint_t, hl_pass_key_extract_key(pk));
    {
        kvps = rh_get_kvps_start(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl),
                                 hash_val);

        I_hl_prefetch(kvps);
        metav = rh_init_meta(hash_val, key);
    }

    while (rh_mkey_meta_lt(metav, (existing_mkey = kvps->mkey_))) {

        metav += K_rh_meta_distance_incr;
        ++kvps;
    }

    if (rh_mkey_meta_eq(metav, existing_mkey)) {
        do {
            if (I_hl_likely(rh_mkey_eq(pk, existing_mkey, hr))) {
#if hl_erase_return_copy
                rh_erase_ret_t ret =
                    rh_erase_make_success_return(key, existing_mkey, kvps);
#endif
                rh_kvp_t existing_kvp;
                hl_kvp_destroy(kvps);
                while (I_hl_unlikely(
                    rh_mkey_meta_lt(K_rh_meta_distance_incr * 2 - 1,
                                    (existing_kvp = *(kvps + 1)).mkey_))) {

                    existing_kvp.mkey_.val_ -= K_rh_meta_distance_incr;
                    *kvps = existing_kvp;
                    ++kvps;
                    __asm__ volatile("" : : :);
                }
                kvps->mkey_ = (rh_mkey_t){ 0 };
#if hl_erase_return_copy
                return ret;
#else
                return rh_erase_make_success_return(
                    INVALID_SYMBOL, INVALID_SYMBOL, INVALID_SYMBOL);
#endif
            }
            metav += K_rh_meta_distance_incr;
            ++kvps;
        } while (rh_mkey_meta_eq(metav, existing_mkey = kvps->mkey_));
    }

    return rh_erase_make_fail_return();
}


#undef K_rh_cmp_meta_inplace
#undef K_rh_default_size
#undef K_rh_has_spare_meta_bit
#undef K_rh_key_bits
#undef K_rh_key_bits_req
#undef K_rh_key_is_subtype
#undef K_rh_key_mask
#undef K_rh_key_upper
#undef K_rh_key_upper_def
#undef K_rh_max_lookup
#undef K_rh_max_lookup_log2
#undef K_rh_meta_bits
#undef K_rh_meta_distance_incr
#undef K_rh_meta_distance_incr_def
#undef K_rh_meta_hash_bits
#undef K_rh_meta_hash_mask
#undef K_rh_meta_hash_shift
#undef K_rh_meta_init_hash_mask
#undef K_rh_meta_inplace
#undef K_rh_meta_is_signed_end
#undef K_rh_meta_is_signed_end_def
#undef K_rh_meta_is_subtype
#undef K_rh_meta_is_subtype_def
#undef K_rh_meta_mask
#undef K_rh_meta_max
#undef K_rh_meta_max_def
#undef K_rh_spare_key_bits
#undef K_rh_spare_key_bits_def
#undef K_rh_true_spare_key_bits
#undef rh_check_meta_at_end
#undef rh_check_mkey_at_end_plus_one
#undef rh_erase
#undef rh_find
#undef rh_find_exists
#undef rh_find_exists_impl
#undef rh_finish_combine_meta
#undef rh_get_kvps_start
#undef rh_init_meta
#undef rh_initialize_kvp
#undef rh_insert
#undef rh_insert_at
#undef rh_insert_no_conflict
#undef rh_key_plus_meta_type
#undef rh_keyv_best_type
#undef rh_keyv_t
#undef rh_kvp_get_key
#undef rh_kvp_t
#undef rh_meta_reset_t
#undef rh_metav_best_tmp_type
#undef rh_metav_best_type
#undef rh_metav_t
#undef rh_metav_tmp_t
#undef rh_mkey_empty
#undef rh_mkey_eq
#undef rh_mkey_finalize_meta
#undef rh_mkey_get_key
#undef rh_mkey_meta_eq
#undef rh_mkey_meta_lt
#undef rh_mkey_t
#undef rh_mkey_uint_t
#undef rh_namer
#undef rh_resize
#undef rh_shift_indexes
#undef rh_table_t
#undef rh_tbl_ptr_t
#undef rh_tunable_max_lookup_log2
#undef rh_kvp_val_get_key
#define I_hl_UNDEF_ALL
#include "hl-user-defs.h"
#include "rh-table-common.h"
#include "rh-table-types.h"
#undef I_hl_UNDEF_ALL
