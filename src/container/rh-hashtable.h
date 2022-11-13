#include "hl-defs.h"
#include "hl-user-defs.h"

/********************************************************************/
/* Table specific tunables.  */
#ifndef rh_tunable_max_lookup_log2
# define rh_tunable_max_lookup_log2 4
#endif

#define rh_mkey_get_key(...) __VA_ARGS__
#define rh_namer(name)       I_hl_namer(hl_name, name)

typedef struct rh_namer(kvp) {
    hl_key_t key_;
#if hl_use_as_map
    hl_val_t val_;
#endif
}
rh_namer(kvp_t);
#define rh_kvp_t rh_namer(kvp_t)

#define rh_kvp_get_key(kvp)     ((kvp)->key_)
#define rh_kvp_val_get_key(kvp) ((kvp).key_)

#if hl_use_as_map
# define rh_initialize_kvp(key, ...)                                           \
        (rh_kvp_t) {                                                           \
            key, __VA_ARGS__                                                   \
        }
#else
# define rh_initialize_kvp(key, ...)                                           \
        (rh_kvp_t) {                                                           \
            key                                                                \
        }

#endif

#define K_rh_meta_inplace 0
#include "rh-table-types.h"

typedef uintptr_t rh_namer(tbl_ptr_t);
#define rh_tbl_ptr_t rh_namer(tbl_ptr_t)

typedef struct rh_namer(table) {
    uintptr_t p_;
#if (hl_conf_fixed_size) <= 1
# if (hl_conf_fixed_size)
    const
# endif
        hl_obj_size_t mask_and_size_;
#endif
}
rh_namer(table_t);

#if hl_conf_die_on_error
# define rh_on_error(...) I_hl_die(__VA_ARGS__)
#else
# define rh_on_error(...)
#endif

#define rh_table_t rh_namer(table_t)

#define rh_meta_t     I_hl_byte_t
#define rh_metav_t    uint8_t
#define rh_meta_2x_t  I_hl_word_t
#define rh_metav_2x_t uint16_t

typedef rh_meta_t * rh_namer(meta_reset_t);
#define rh_meta_reset_t rh_namer(meta_reset_t)


#define K_rh_max_lookup_log2 (rh_tunable_max_lookup_log2)
#define K_rh_max_lookup      (1 << (K_rh_max_lookup_log2))
#define K_rh_default_size    (64)
#define K_rh_kvps_offset     (K_rh_max_lookup * sizeof(rh_meta_t))

#if (hl_conf_fixed_size) > 1 && ((hl_conf_fixed_size) < K_rh_max_lookup)
# error "Invalid fixed size!"
#endif

#define K_rh_meta_hash_bits                                                   \
    (I_hl_sizeof_bits(rh_meta_t) - K_rh_max_lookup_log2)
#define K_rh_meta_hash_shift                                                  \
    (I_hl_sizeof_bits(hl_obj_size_t) - K_rh_meta_hash_bits)
#define K_rh_meta_distance_incr (1u << (K_rh_meta_hash_bits))
#define K_rh_meta_distance_mask (-(K_rh_max_lookup))
#define K_rh_meta_max           (1u << I_hl_sizeof_bits(rh_meta_t))
#define K_rh_meta_2x_max        (1u << I_hl_sizeof_bits(rh_meta_2x_t))
#define K_rh_meta_distance_incr_2x                                            \
    ((K_rh_meta_distance_incr) |                                              \
     ((K_rh_meta_distance_incr) << I_hl_sizeof_bits(rh_meta_t)))
#define K_rh_meta_distance_incr_4x                                            \
    ((K_rh_meta_distance_incr_2x) |                                           \
     ((K_rh_meta_distance_incr_2x) << I_hl_sizeof_bits(rh_meta_2x_t)))


#define K_rh_kvp_size_mul ((sizeof(rh_kvp_t)) / sizeof(rh_meta_t))


I_hl_assert_const_eval(K_rh_default_size);
I_hl_assert_const_eval(K_rh_kvp_size_mul);
I_hl_assert_const_eval(K_rh_kvps_offset);
I_hl_assert_const_eval(K_rh_max_lookup);
I_hl_assert_const_eval(K_rh_max_lookup_log2);
I_hl_assert_const_eval(K_rh_meta_2x_max);
I_hl_assert_const_eval(K_rh_meta_distance_incr);
I_hl_assert_const_eval(K_rh_meta_distance_incr_2x);
I_hl_assert_const_eval(K_rh_meta_distance_incr_4x);
I_hl_assert_const_eval(K_rh_meta_distance_mask);
I_hl_assert_const_eval(K_rh_meta_hash_bits);
I_hl_assert_const_eval(K_rh_meta_hash_shift);
I_hl_assert_const_eval(K_rh_meta_inplace);
I_hl_assert_const_eval(K_rh_meta_max);


I_hl_static_assert((K_rh_kvps_offset) >=
                   (K_rh_max_lookup * sizeof(rh_meta_t)));

/********************************************************************/
/* Common methods.  */
#include "rh-table-common.h"


/********************************************************************/
/* minfo <-> kvp pointer helpers.  */

#define rh_mask_and_size_get_kvps_offset                                       \
    rh_namer(rh_mask_and_size_get_kvps_offset)
#define rh_get_meta_info_start rh_namer(rh_get_meta_info_start)
#define rh_get_kvps_start      rh_namer(rh_get_kvps_start)
#define rh_get_kvp_start       rh_namer(rh_get_kvp_start)

I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_kvps_offset(hl_obj_size_t mask_and_size) {
    return mask_and_size + K_rh_mas_piece_sz + K_rh_kvps_offset;
}

I_hl_cattrs uintptr_t
rh_get_meta_info_start(uintptr_t     minfos,
                       hl_obj_size_t mask_and_size,
                       hl_obj_size_t hash_val) {
    return minfos + rh_get_hash_idx(mask_and_size, hash_val);
}

I_hl_cattrs uintptr_t
rh_get_kvps_start(uintptr_t minfos, hl_obj_size_t mask_and_size) {
    return minfos + rh_mask_and_size_get_kvps_offset(mask_and_size);
}

I_hl_cattrs uintptr_t
rh_get_kvp_start(uintptr_t     minfos,
                 hl_obj_size_t mask_and_size,
                 hl_obj_size_t hash_val) {
    return minfos + rh_mask_and_size_get_kvps_offset(mask_and_size) +
           rh_get_hash_idx(mask_and_size, hash_val) * K_rh_kvp_size_mul;
}


/********************************************************************/
/* Helpers for interacting with rh_meta_t.  */
#define rh_init_minfo                   rh_namer(init_minfo)
#define rh_check_meta_info_end          rh_namer(check_meta_info_end)
#define rh_check_meta_info_end_plus_one rh_namer(check_meta_info_end_plus_one)
#define rh_check_meta_info_end_2x_plus_one                                     \
    rh_namer(check_meta_info_end_2x_plus_one)
#define rh_check_meta_info_2x_zero rh_namer(check_meta_info_2x_zero)

I_hl_cattrs uint32_t
rh_init_minfo(hl_obj_size_t hash_val) {
    return I_hl_CAST(uint32_t, (hash_val >> K_rh_meta_hash_shift)) +
           K_rh_meta_distance_incr;
}

I_hl_cattrs I_hl_bool_t
rh_check_meta_info_end(rh_metav_t minfo) {
    return minfo >= (K_rh_meta_max - K_rh_meta_distance_incr);
}


I_hl_cattrs I_hl_bool_t
rh_check_meta_info_end_plus_one(rh_metav_t minfo) {
    return minfo >= (K_rh_meta_max - 2 * K_rh_meta_distance_incr);
}


I_hl_cattrs I_hl_bool_t
rh_check_meta_info_end_2x_plus_one(uint32_t minfo_hi) {
    return minfo_hi >= ((K_rh_meta_max - 2 * K_rh_meta_distance_incr)
                        << I_hl_sizeof_bits(rh_meta_t));
}


I_hl_cattrs I_hl_bool_t
rh_check_meta_info_2x_zero(uint32_t minfo_hi) {
    return minfo_hi <= K_rh_meta_max - 1;
}

#define rh_load_meta_t(p)     (I_hl_load_T(rh_meta_t, p).val_)
#define rh_store_meta_t(p, v) I_hl_store_T(rh_meta_t, p, v)


/********************************************************************/
/* Insert.  */

#define rh_insert_at_i        rh_namer(insert_at_i)
#define rh_insert_no_conflict rh_namer(insert_no_conflict)
#define rh_shift_indexes      rh_namer(shift_indexes)
#define rh_resize             rh_namer(resize)
#define rh_insert             rh_namer(insert)

I_hl_attrs rh_insert_ret_t
rh_insert_at_i(rh_meta_t * restrict minfos,
               rh_kvp_t * restrict kvps,
               uint32_t                  i,
               rh_meta_t                 minfo,
               I_hl_dconst hl_pass_key_t pk,
               hl_hashret_t hash_val     I_hl_unused) {
    minfos[i] = minfo;
    hl_kvp_set_prepare(pk, hr);
#ifdef hl_pass_key_extract_val
    kvps[i] = rh_initialize_kvp(hl_pass_key_extract_key(pk),
                                hl_pass_key_extract_val(pk));
#else
    kvps[i].key_ = hl_pass_key_extract_key(pk);
#endif
    return rh_insert_make_success_return(
        hl_pass_key_extract_key(pk), hl_pass_key_extract_key(pk), (kvps + i));
}


static void
rh_insert_no_conflict(rh_meta_t * restrict minfos,
                      rh_kvp_t * restrict kvps,
                      rh_kvp_t * restrict kvp,
                      uint32_t minfo) {
    uint32_t i, existing_minfo;

    i = 0;
    while (minfo <= (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += K_rh_meta_distance_incr;
        ++i;
    }

    if (existing_minfo) {
        uint32_t placei = i;
        do {
            ++i;
        } while (rh_load_meta_t(minfos + i));

        I_hl_guarantee(i > placei);
        for (; I_hl_unlikely(i > placei); --i) {
            rh_store_meta_t(minfos + i,
                            (rh_meta_t){ I_hl_CAST(
                                rh_metav_t, rh_load_meta_t(minfos + i - 1) +
                                                K_rh_meta_distance_incr) });
            hl_kvp_move((kvps + i), (kvps + i - 1));
        }
        I_hl_guarantee(i == placei);
    }


    rh_store_meta_t(minfos + i, (rh_meta_t){ I_hl_CAST(rh_metav_t, minfo) });
    hl_kvp_move((kvps + i), kvp);
}

I_hl_attrs I_hl_bool_t
rh_shift_indexes(rh_meta_t * restrict minfos,
                 rh_kvp_t * restrict kvps,
                 uint32_t i) {


    uint32_t placei = i;
    i++;
    for (;;) {
        /* Make this int32_t signed. For GCC(11.1) this is the only way to
         * get `movzwl` and avoid LCP stalls with the imm16 comparisons.
         * Clang doesn't care either way. */
        I_hl_static_assert(sizeof(int32_t) > sizeof(rh_meta_2x_t));
        int32_t minfo = 0;
        hl_memcpy(&minfo, minfos + i, sizeof(rh_meta_2x_t));
        if ((I_hl_CAST(rh_metav_t, minfo) == 0)) {
            break;
        }
        if (I_hl_unlikely(rh_check_meta_info_end_plus_one(
                I_hl_CAST(rh_metav_t, minfo)))) {
            return 1;
        }
        if (rh_check_meta_info_2x_zero(I_hl_CAST(uint32_t, minfo))) {
            ++i;
            break;
        }
        if (I_hl_unlikely(rh_check_meta_info_end_2x_plus_one(
                I_hl_CAST(uint32_t, minfo)))) {
            return 1;
        }
        i += 2;
    }
    I_hl_guarantee(i > placei);

    for (; I_hl_unlikely(i > placei); --i) {
        minfos[i] =
            (rh_meta_t){ CAST(rh_metav_t, rh_load_meta_t(minfos + i - 1) +
                                              K_rh_meta_distance_incr) };
        hl_kvp_move((kvps + i), (kvps + i - 1));
        I_hl_no_cmov();
    }
    I_hl_guarantee(i == placei);
    return 0;
}

#if !(hl_conf_fixed_size)
/* We don't want to inline this even though its only called from one spot
 * (insert). The rational is we 1) get a tail call anyways and 2) don't want
 * to eat the code size cost on the critical path. */
static I_hl_noinline rh_insert_ret_t
rh_resize(rh_table_t * restrict rh_tbl,
          I_hl_dconst hl_pass_key_t pk,
          const hl_hashret_t        hr) {
    /* TODO: Check clang if any loop optimizations take place. If so maybe
     * can make irreducible. */
    hl_obj_size_t mask_and_size      = rh_tbl_get_mask_and_size(rh_tbl);
    hl_obj_size_t next_mask_and_size = rh_mask_and_size_get_next(mask_and_size);
    uintptr_t     next_minfos;
# if rh_insert_returns
    rh_insert_ret_t ret;
# endif
# if hl_conf_robust_resize && hl_conf_retry_max
    int32_t retry_counter;
    retry_counter = 0;
# endif

super_unlikely_retry:
    I_hl_unused;


    {
        uintptr_t next_table =
            rh_alloc_table(rh_mask_and_size_get_alloc_sz(next_mask_and_size));
        /* The only true error case we can have. */
        if (I_hl_unlikely(next_table == 0UL)) {
            rh_on_error("Allocation error");
            return rh_internal_insert_make_return(hl_operation_failure,
                                                  rh_invalid_return_entry);
        }

        next_minfos = next_table;
    }

    {
        uintptr_t     old_minfos = rh_tbl->p_;
        uintptr_t     old_kvps   = rh_get_kvps_start(old_minfos, mask_and_size);
        hl_obj_size_t old_cap    = rh_mask_and_size_get_cap(mask_and_size);

        hl_obj_size_t i = 0;


        I_hl_guarantee(i < old_cap);
        for (; i < old_cap; ++i) {
            if (I_hl_likely(rh_load_meta_t(old_minfos + i))) {
                hl_obj_size_t hash_val = hl_rehash(
                    I_hl_CAST(rh_kvp_t const *, old_kvps + i * sizeof(rh_kvp_t))
                        ->key_);

                rh_meta_t * insert_minfos =
                    I_hl_CAST(rh_meta_t *,
                              rh_get_meta_info_start(
                                  next_minfos, next_mask_and_size, hash_val));

                rh_kvp_t * insert_kvps = I_hl_CAST(
                    rh_kvp_t *, rh_get_kvp_start(next_minfos,
                                                 next_mask_and_size, hash_val));

                rh_insert_no_conflict(
                    insert_minfos, insert_kvps,
                    I_hl_CAST(rh_kvp_t *, old_kvps + i * sizeof(rh_kvp_t)),
                    rh_init_minfo(hash_val));
            }
        }
    }

    {
        hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

        uintptr_t minfos =
            rh_get_meta_info_start(next_minfos, next_mask_and_size, hash_val);
        uint32_t minfo = rh_init_minfo(hash_val);

        uintptr_t kvps =
            rh_get_kvp_start(next_minfos, next_mask_and_size, hash_val);

        uint32_t i, existing_minfo;
        i = 0;
        while (minfo <= (existing_minfo = rh_load_meta_t(minfos + i))) {
            minfo += K_rh_meta_distance_incr;
            ++i;
        }
# if hl_conf_robust_resize
        if (I_hl_unlikely(
                rh_check_meta_info_end(I_hl_CAST(rh_metav_t, minfo)) ||
                (existing_minfo &&
                 (rh_check_meta_info_end_plus_one((rh_metav_t)existing_minfo) ||
                  rh_shift_indexes(I_hl_CAST(rh_meta_t *, minfos),
                                   I_hl_CAST(rh_kvp_t *, kvps), i))))) {
            rh_free_table_mask_and_size(next_minfos, next_mask_and_size);
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
        if (existing_minfo) {
            rh_shift_indexes(I_hl_CAST(rh_meta_t *, minfos),
                             I_hl_CAST(rh_kvp_t *, kvps), i);
        }
# endif
        rh_set_insert_return(
            ret, rh_insert_at_i(I_hl_CAST(rh_meta_t *, minfos),
                                I_hl_CAST(rh_kvp_t *, kvps), i,
                                (rh_meta_t){ (rh_metav_t)minfo }, pk, hr));
    }

    rh_free_table_mask_and_size(rh_tbl->p_, mask_and_size);
    rh_tbl->mask_and_size_ = next_mask_and_size;
    rh_tbl->p_             = next_minfos;
# if rh_insert_returns
    return ret;
# else
    return;
# endif
}
#endif


static rh_insert_ret_t
rh_insert(rh_table_t * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    uintptr_t minfos;
    uintptr_t kvps;
    uint32_t  i, minfo, existing_minfo;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);
    {
        hl_obj_size_t mask_and_size = rh_tbl_get_mask_and_size(rh_tbl);
        uintptr_t     minfos_init   = rh_tbl->p_;

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I_hl_prefetch(I_hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

        /* We need to compute kvps no matter what. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I_hl_CAST(rh_kvp_t const *, kvps));
    }
    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += K_rh_meta_distance_incr;
        ++i;
    }
    while (I_hl_unlikely(minfo == existing_minfo)) {
        rh_kvp_t * kvps_i = I_hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t));
        if (I_hl_likely(hl_key_eq((kvps_i->key_), pk, hr) == 0)) {
            return rh_insert_make_fail_return(hl_pass_key_extract_key(pk),
                                              (kvps_i->key_), kvps_i);
        }

        minfo += K_rh_meta_distance_incr;
        ++i;
        existing_minfo = rh_load_meta_t(minfos + i);
    }
    {
#if rh_insert_returns
        rh_insert_ret_t ret;
#endif
        if (I_hl_unlikely(
                rh_check_meta_info_end(I_hl_CAST(rh_metav_t, minfo)))) {

            rh_set_insert_return(ret, rh_resize(rh_tbl, pk, hr));
            goto insert_return;
        }


        /* Now we might need to memmove so store the start. */
        if (existing_minfo) {
            if (I_hl_unlikely(rh_check_meta_info_end_plus_one(
                                  (rh_metav_t)existing_minfo) ||
                              rh_shift_indexes(I_hl_CAST(rh_meta_t *, minfos),
                                               I_hl_CAST(rh_kvp_t *, kvps),
                                               i))) {
                rh_set_insert_return(ret, rh_resize(rh_tbl, pk, hr));
                goto insert_return;
            }
        }
        rh_set_insert_return(
            ret, rh_insert_at_i(I_hl_CAST(rh_meta_t *, minfos),
                                I_hl_CAST(rh_kvp_t *, kvps), i,
                                (rh_meta_t){ (rh_metav_t)minfo }, pk, hr));
    insert_return:
#if rh_insert_returns
        return ret;
#else
        return;
#endif
    }
}

/********************************************************************/
/* Find API.  */
#define rh_find             rh_namer(find)
#define rh_find_exists      rh_namer(find_exists)
#define rh_find_exists_impl rh_namer(find_exists_impl)
static I_hl_pure rh_find_ret_t
rh_find(rh_table_t const * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    uint32_t      minfo, i, existing_minfo;
    uintptr_t     minfos;
    uintptr_t     kvps;
    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    hl_obj_size_t mask_and_size = rh_tbl_get_mask_and_size(rh_tbl);
    uintptr_t     minfos_init   = rh_tbl->p_;
    {

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I_hl_prefetch(I_hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

#if hl_conf_aggressive_prefetch
        /* Hot path for finding the kvp. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I_hl_CAST(rh_kvp_t const *, kvps));
#endif
    }

    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += K_rh_meta_distance_incr;
        ++i;
    }

    if (I_hl_likely(minfo == existing_minfo)) {
#if !hl_conf_aggressive_prefetch
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
#endif
        do {
            rh_kvp_t * kvps_i =
                I_hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t));
            if (I_hl_likely(hl_key_eq(kvps_i->key_, pk, hr) == 0)) {
                return rh_get_valid_success_entry(hl_pass_key_extract_key(pk),
                                                  kvps_i->key_, kvps_i);
            }
            ++i;
            minfo += K_rh_meta_distance_incr;
        } while (minfo == rh_load_meta_t(minfos + i));
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
    uint32_t      minfo, i, existing_minfo;
    uintptr_t     minfos;
    uintptr_t     kvps;
    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    hl_obj_size_t mask_and_size = rh_tbl_get_mask_and_size(rh_tbl);
    uintptr_t     minfos_init   = rh_tbl->p_;
    {

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I_hl_prefetch(I_hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

#if hl_conf_aggressive_prefetch
        /* Hot path for finding the kvp. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I_hl_CAST(rh_kvp_t const *, kvps));
#endif
    }

    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += K_rh_meta_distance_incr;
        ++i;
    }

    if (I_hl_likely(minfo == existing_minfo)) {
#if !hl_conf_aggressive_prefetch
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
#endif
        do {
            rh_kvp_t * kvps_i =
                I_hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t));
            if (I_hl_likely(hl_key_eq(kvps_i->key_, pk, hr) == 0)) {
                return rh_get_valid_success_entry(hl_pass_key_extract_key(pk),
                                                  kvps_i->key_, kvps_i);
            }
            ++i;
            minfo += K_rh_meta_distance_incr;
        } while (minfo == rh_load_meta_t(minfos + i));
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
#define rh_erase_shift rh_namer(erase_impl)
#define rh_erase       rh_namer(erase)


I_hl_attrs rh_erase_ret_t
rh_erase_shift(rh_meta_t * restrict minfos,
               I_hl_dconst hl_pass_key_t pk,
               rh_kvp_t * restrict kvps,
               uint32_t              minfo,
               const hl_hashret_t hr I_hl_unused,
               uint32_t              i) {
    uint32_t existing_minfo;
    do {
        if (I_hl_likely(hl_key_eq((kvps + i)->key_, pk, hr) == 0)) {
#if hl_erase_return_copy
            rh_erase_ret_t ret = rh_erase_make_success_return(
                hl_pass_key_extract_key(pk), (kvps + i)->key_, (kvps + i));
#endif
            hl_kvp_destroy((kvps + i));
            while (I_hl_unlikely(
                (existing_minfo = rh_load_meta_t(minfos + i + 1)) >=
                (K_rh_meta_distance_incr * 2))) {
                rh_store_meta_t(
                    minfos + i,
                    (rh_meta_t){
                        I_hl_CAST(rh_metav_t,
                                  existing_minfo - K_rh_meta_distance_incr) });
                hl_kvp_move((kvps + i), (kvps + i + 1));
                ++i;
                I_hl_no_cmov();
            }
            rh_store_meta_t(minfos + i, (rh_meta_t){ 0 });
#if hl_erase_return_copy
            return ret;
#else
            return rh_erase_make_success_return(INVALID_SYMBOL, INVALID_SYMBOL,
                                                INVALID_SYMBOL);
#endif
        }
        ++i;
        minfo += K_rh_meta_distance_incr;
    } while (minfo == rh_load_meta_t(minfos + i));

    return rh_erase_make_fail_return();
}


static rh_erase_ret_t
rh_erase(rh_table_t const * restrict rh_tbl, I_hl_dconst hl_pass_key_t pk) {
    uint32_t      minfo, i, existing_minfo;
    uintptr_t     minfos;
    uintptr_t     kvps;
    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);

    hl_obj_size_t mask_and_size = rh_tbl_get_mask_and_size(rh_tbl);
    uintptr_t     minfos_init   = rh_tbl->p_;
    {

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I_hl_prefetch(I_hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

#if hl_conf_aggressive_prefetch
        /* Hot path for finding the kvp. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I_hl_CAST(rh_kvp_t const *, kvps));
#endif
    }

    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += K_rh_meta_distance_incr;
        ++i;
    }

    if (minfo == existing_minfo) {
#if !hl_conf_aggressive_prefetch
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
#endif
        return rh_erase_shift(I_hl_CAST(rh_meta_t *, minfos), pk,
                              I_hl_CAST(rh_kvp_t *, kvps), minfo, hr, i);
    }

    return rh_erase_make_fail_return();
}


/********************************************************************/
/* Undefs.  */

#undef K_rh_default_size
#undef K_rh_kvp_size_mul
#undef K_rh_kvps_offset
#undef K_rh_max_lookup
#undef K_rh_max_lookup_log2
#undef K_rh_meta_2x_max
#undef K_rh_meta_distance_incr
#undef K_rh_meta_distance_incr_2x
#undef K_rh_meta_distance_incr_4x
#undef K_rh_meta_distance_mask
#undef K_rh_meta_hash_bits
#undef K_rh_meta_hash_shift
#undef K_rh_meta_inplace
#undef K_rh_meta_max
#undef rh_check_meta_info_2x_zero
#undef rh_check_meta_info_end
#undef rh_check_meta_info_end_2x_plus_one
#undef rh_check_meta_info_end_plus_one
#undef rh_erase
#undef rh_erase_shift
#undef rh_find
#undef rh_find_exists
#undef rh_find_exists_impl
#undef rh_get_kvp_start
#undef rh_get_kvps_start
#undef rh_get_meta_info_start
#undef rh_init_minfo
#undef rh_initialize_kvp
#undef rh_insert
#undef rh_insert_at_i
#undef rh_insert_no_conflict
#undef rh_kvp_get_key
#undef rh_kvp_t
#undef rh_kvp_val_get_key
#undef rh_load_meta_t
#undef rh_mask_and_size_get_kvps_offset
#undef rh_meta_2x_t
#undef rh_meta_reset_t
#undef rh_meta_t
#undef rh_metav_2x_t
#undef rh_metav_t
#undef rh_mkey_get_key
#undef rh_namer
#undef rh_on_error
#undef rh_resize
#undef rh_shift_indexes
#undef rh_store_meta_t
#undef rh_table_t
#undef rh_tbl_ptr_t
#undef rh_tunable_max_lookup_log2
#define I_hl_UNDEF_ALL
#include "hl-user-defs.h"
#include "rh-table-common.h"
#include "rh-table-types.h"
#undef I_hl_UNDEF_ALL
