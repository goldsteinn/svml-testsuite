#include "hl-defs.h"
#include "hl-user-defs.h"

/********************************************************************/
/* Universal constants.  */
#ifndef I__rh_UNIVERSAL_CONSTANTS
#define I__rh_UNIVERSAL_CONSTANTS


#define rh_allocation_error I__hl_CAST(void *, 1)
#define rh_resize_error     I__hl_CAST(void *, 2)

#define rh_find_failed  I__hl_failed
#define rh_erase_failed I__hl_failed

static char const * const rh_error_to_str[] = { "None", "Allocation",
                                                "Resize" };


#endif


/********************************************************************/
/* Table specific tunables.  */
#ifndef rh_tunable_max_lookup_log2
#define rh_tunable_max_lookup_log2 5
#endif

#define rh_namer(name) I__hl_namer(hl_name, name)

typedef struct rh_namer(kvp) {
    hl_key_t key_;
#if hl_use_as_map
    hl_val_t val_;
#endif
}
rh_namer(kvp_t);
#define rh_kvp_t rh_namer(kvp_t)

typedef struct rh_namer(insert_ret) {
    rh_kvp_t *   kvp_;
    I__hl_bool_t already_exists_;
}
rh_namer(insert_ret_t);
#define rh_insert_ret_t rh_namer(insert_ret_t)

typedef struct rh_namer(table) {
    hl_obj_size_t mask_and_size_;
    uintptr_t     p_;
#if hl_conf_adaptive_hash
    hl_obj_size_t extra_hash_mul_;
#endif
}
rh_namer(table_t);
#define rh_table_t rh_namer(table_t)

#define rh_meta_t     I__hl_byte_t
#define rh_metav_t    uint8_t
#define rh_meta_2x_t  I__hl_word_t
#define rh_metav_2x_t uint16_t

#define k__rh_max_lookup_log2 (rh_tunable_max_lookup_log2)
#define k__rh_max_lookup      (1 << (k__rh_max_lookup_log2))
#define k__rh_default_size    (64)
#define k__rh_kvps_offset     (k__rh_max_lookup * sizeof(rh_meta_t))

#define k__rh_meta_hash_bits                                                   \
    (I__hl_sizeof_bits(rh_meta_t) - k__rh_max_lookup_log2)
#define k__rh_meta_hash_shift                                                  \
    (I__hl_sizeof_bits(hl_obj_size_t) - k__rh_meta_hash_bits)
#define k__rh_meta_distance_incr (1u << (k__rh_meta_hash_bits))
#define k__rh_meta_distance_mask (-(k__rh_max_lookup))
#define k__rh_meta_max           (1u << I__hl_sizeof_bits(rh_meta_t))
#define k__rh_meta_2x_max        (1u << I__hl_sizeof_bits(rh_meta_2x_t))
#define k__rh_meta_distance_incr_2x                                            \
    ((k__rh_meta_distance_incr) |                                              \
     ((k__rh_meta_distance_incr) << I__hl_sizeof_bits(rh_meta_t)))
#define k__rh_meta_distance_incr_4x                                            \
    ((k__rh_meta_distance_incr_2x) |                                           \
     ((k__rh_meta_distance_incr_2x) << I__hl_sizeof_bits(rh_meta_2x_t)))


#define k__rh_kvp_size_mul ((sizeof(rh_kvp_t)) / sizeof(rh_meta_t))


I__hl_assert_const_eval(k__rh_max_lookup_log2);
I__hl_assert_const_eval(k__rh_max_lookup);
I__hl_assert_const_eval(k__rh_default_size);
I__hl_assert_const_eval(k__rh_kvps_offset);

I__hl_assert_const_eval(k__rh_meta_hash_bits);
I__hl_assert_const_eval(k__rh_meta_hash_shift);
I__hl_assert_const_eval(k__rh_meta_distance_incr);
I__hl_assert_const_eval(k__rh_meta_distance_mask);
I__hl_assert_const_eval(k__rh_meta_max);
I__hl_assert_const_eval(k__rh_meta_2x_max);
I__hl_assert_const_eval(k__rh_meta_distance_incr_2x);

I__hl_assert_const_eval(k__rh_kvp_size_mul);


I__hl_static_assert((k__rh_kvps_offset) >=
                    (k__rh_max_lookup * sizeof(rh_meta_t)));

/********************************************************************/
/* Helpers for converting 'mask_and_size' to size and vice versa.   */
#define rh_mask_and_size_get_byte_sz rh_namer(mask_and_size_get_byte_sz)
#define rh_mask_and_size_get_meta_bytes_sz                                     \
    rh_namer(mask_and_size_get_meta_bytes_sz)
#define rh_mask_and_size_get_sz       rh_namer(mask_and_size_get_sz)
#define rh_mask_and_size_get_cap      rh_namer(mask_and_size_get_cap)
#define rh_get_alloc_sz               rh_namer(get_alloc_sz)
#define rh_mask_and_size_get_alloc_sz rh_namer(mask_and_size_get_alloc_sz)
#define rh_mask_and_size_get_next     rh_namer(mask_and_size_get_next)
#define rh_get_mask_from_size         rh_namer(get_mask_from_size)


I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_byte_sz(hl_obj_size_t mask_and_size) {
    return mask_and_size + sizeof(rh_meta_t);
}


I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_meta_bytes_sz(hl_obj_size_t mask_and_size) {
    return mask_and_size + sizeof(rh_meta_t) +
           k__rh_max_lookup * sizeof(rh_meta_t);
}


I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_sz(hl_obj_size_t mask_and_size) {
    return rh_mask_and_size_get_byte_sz(mask_and_size) / sizeof(rh_meta_t);
}


I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_cap(hl_obj_size_t mask_and_size) {
    return rh_mask_and_size_get_meta_bytes_sz(mask_and_size) /
           sizeof(rh_meta_t);
}


I__hl_cattrs hl_obj_size_t
rh_get_alloc_sz(hl_obj_size_t sz) {
    return sz * (sizeof(rh_meta_t) + sizeof(rh_kvp_t)) + k__rh_kvps_offset +
           k__rh_max_lookup * sizeof(rh_kvp_t);
}


I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_alloc_sz(hl_obj_size_t mask_and_size) {
    return mask_and_size * (k__rh_kvp_size_mul + 1) + sizeof(rh_meta_t) +
           sizeof(rh_kvp_t) + k__rh_kvps_offset +
           k__rh_max_lookup * sizeof(rh_kvp_t);
}

I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_next(hl_obj_size_t mask_and_size) {
    return mask_and_size + mask_and_size + sizeof(rh_meta_t);
}


I__hl_cattrs hl_obj_size_t
rh_get_mask_from_size(hl_obj_size_t sz) {
    return sz - sizeof(rh_meta_t);
}


/********************************************************************/
/* Helpers for accessing memory.  */
#define rh_get_hash_idx rh_namer(get_hash_idx)

I__hl_cattrs hl_obj_size_t
rh_get_hash_idx(hl_obj_size_t mask_and_size, hl_obj_size_t hash_val) {
    return hash_val & mask_and_size;
}

/********************************************************************/
/* minfo <-> kvp pointer helpers.  */

#define rh_mask_and_size_get_kvps_offset                                       \
    rh_namer(rh_mask_and_size_get_kvps_offset)
#define rh_get_meta_info_start rh_namer(rh_get_meta_info_start)
#define rh_get_kvps_start      rh_namer(rh_get_kvps_start)
#define rh_get_kvp_start       rh_namer(rh_get_kvp_start)

I__hl_cattrs hl_obj_size_t
rh_mask_and_size_get_kvps_offset(hl_obj_size_t mask_and_size) {
    return mask_and_size + sizeof(rh_meta_t) + k__rh_kvps_offset;
}

I__hl_cattrs uintptr_t
rh_get_meta_info_start(uintptr_t     minfos,
                       hl_obj_size_t mask_and_size,
                       hl_obj_size_t hash_val) {
    return minfos + rh_get_hash_idx(mask_and_size, hash_val);
}

I__hl_cattrs uintptr_t
rh_get_kvps_start(uintptr_t minfos, hl_obj_size_t mask_and_size) {
    return minfos + rh_mask_and_size_get_kvps_offset(mask_and_size);
}

I__hl_cattrs uintptr_t
rh_get_kvp_start(uintptr_t     minfos,
                 hl_obj_size_t mask_and_size,
                 hl_obj_size_t hash_val) {
    return minfos + rh_mask_and_size_get_kvps_offset(mask_and_size) +
           rh_get_hash_idx(mask_and_size, hash_val) * k__rh_kvp_size_mul;
}


/********************************************************************/
/* Helpers for interacting with rh_meta_t.  */
#define rh_init_minfo                   rh_namer(init_minfo)
#define rh_check_meta_info_end          rh_namer(check_meta_info_end)
#define rh_check_meta_info_end_plus_one rh_namer(check_meta_info_end_plus_one)
#define rh_check_meta_info_end_2x_plus_one                                     \
    rh_namer(check_meta_info_end_2x_plus_one)
#define rh_check_meta_info_2x_zero rh_namer(check_meta_info_2x_zero)

I__hl_cattrs uint32_t
rh_init_minfo(hl_obj_size_t hash_val) {
    return (hash_val >> k__rh_meta_hash_shift) | k__rh_meta_distance_incr;
}

I__hl_cattrs int32_t
rh_check_meta_info_end(rh_metav_t minfo) {
    return minfo >= (k__rh_meta_max - k__rh_meta_distance_incr);
}


I__hl_cattrs int32_t
rh_check_meta_info_end_plus_one(rh_metav_t minfo) {
    return minfo >= (k__rh_meta_max - 2 * k__rh_meta_distance_incr);
}


I__hl_cattrs int32_t
rh_check_meta_info_end_2x_plus_one(uint32_t minfo_hi) {
    return minfo_hi >= ((k__rh_meta_max - 2 * k__rh_meta_distance_incr)
                        << I__hl_sizeof_bits(rh_meta_t));
}


I__hl_cattrs int32_t
rh_check_meta_info_2x_zero(uint32_t minfo_hi) {
    return minfo_hi <= k__rh_meta_max - 1;
}

#define rh_load_meta_t(p)     (I__hl_load_T(rh_meta_t, p).val_)
#define rh_store_meta_t(p, v) I__hl_store_T(rh_meta_t, p, v)


/********************************************************************/
/* Helpers for allocating/freeing table memory.  */
#define rh_reset_meta_info_bytes         rh_namer(reset_meta_info_bytes)
#define rh_reset_meta_info               rh_namer(reset_meta_info)
#define rh_mask_and_size_reset_meta_info rh_namer(mask_and_size_reset_meta_info)
#define rh_alloc_table                   rh_namer(alloc_table)
#define rh_free_table_mask_and_size      rh_namer(free_table_mask_and_size)

I__hl_attrs void
rh_reset_meta_info_bytes(uintptr_t p, hl_obj_size_t bytes) {
    hl_memset(I__hl_CAST(rh_meta_t *, p), 0, bytes);
}

I__hl_attrs void
rh_reset_meta_info(uintptr_t p, hl_obj_size_t sz) {
    rh_reset_meta_info_bytes(
        p, sz * sizeof(rh_meta_t) + k__rh_max_lookup * sizeof(rh_meta_t));
}


I__hl_attrs void
rh_mask_and_size_reset_meta_info(uintptr_t p, hl_obj_size_t mask_and_size) {
    rh_reset_meta_info_bytes(p,
                             rh_mask_and_size_get_meta_bytes_sz(mask_and_size));
}


I__hl_attrs uintptr_t
rh_alloc_table(hl_obj_size_t sz) {
    uintptr_t p = I__hl_CAST(uintptr_t, hl_alloc(rh_get_alloc_sz(sz)));
    rh_reset_meta_info(p, sz);
    return p;
}


I__hl_attrs void
rh_free_table_mask_and_size(uintptr_t p, hl_obj_size_t mask_and_size) {
    hl_sfree(I__hl_CAST(void *, p),
             rh_mask_and_size_get_alloc_sz(mask_and_size));
}


/********************************************************************/
/* Helpers for 'hl_conf_adaptive_hash'.  */

#define rh_get_adapted_hash            rh_namer(get_adapted_hash)
#define rh_get_next_extra_hash_mul     rh_namer(get_next_extra_hash_mul)
#define rh_table_update_extra_hash_mul rh_namer(table_update_extra_hash_mul)
#define rh_table_get_extra_hash_mul    rh_namer(table_get_extra_hash_mul)
#define rh_table_get_adapted_hash      rh_namer(table_get_adapted_hash)
#define rh_table_init_extra_hash       rh_namer(table_init_extra_hash)

I__hl_cattrs hl_obj_size_t
rh_get_adapted_hash(hl_obj_size_t extra_hash_mul I__hl_unused,
                    hl_obj_size_t                hash_val) {
#if hl_conf_adaptive_hash
    hl_obj_size_t _tmp = extra_hash_mul * hash_val;
    return _tmp ^ (_tmp >> (1 + I__hl_sizeof_bits(hl_obj_size_t)));
#else
    return hash_val;
#endif
}


I__hl_cattrs hl_obj_size_t
rh_get_next_extra_hash_mul(hl_obj_size_t extra_hash_mul I__hl_unused) {
#if hl_conf_adaptive_hash
    return extra_hash_mul + I__hl_cast(hl_obj_size_t, 0xc4ceb9fe1a85ec54);
#else
    return 0;
#endif
}


I__hl_attrs void
rh_table_update_extra_hash_mul(rh_table_t * rh_tbl          I__hl_unused,
                               hl_obj_size_t extra_hash_mul I__hl_unused) {
#if hl_conf_adaptive_hash
    rh_tbl->extra_hash_mul_ = rh_get_next_extra_hash_mul(extra_hash_mul);
#endif
}


I__hl_pattrs hl_obj_size_t
rh_table_get_extra_hash_mul(rh_table_t const * rh_tbl I__hl_unused) {
#if hl_conf_adaptive_hash
    return rb_tbl->extra_hash_mul_;
#else
    return 0;
#endif
}


I__hl_pattrs hl_obj_size_t
rh_table_get_adapted_hash(rh_table_t const * rh_tbl I__hl_unused,
                          hl_obj_size_t             hash_val) {
#if hl_conf_adaptive_hash
    return rh_get_adapted_hash(rh_table_get_extra_hash_mul(rh_tbl), hash_val);
#else
    return hash_val;
#endif
}


I__hl_attrs void
rh_table_init_extra_hash(rh_table_t * rh_tbl I__hl_unused) {
#if hl_conf_adaptive_hash
    rh_table_update_extra_hash_mul(
        rh_tbl, sizeof(hl_obj_size_t) == 8 ? 0xc3a5c85c97cb3127 : 0xc4653e41);
#endif
}


/********************************************************************/
/* Helpers for 'hl_conf_aggressive_prefetch'.  */

#if hl_conf_aggressive_prefetch
#define rh_table_aggressive_prefetch(...) I__hl_prefetch(__VA_ARGS__)
#else
#define rh_table_aggressive_prefetch(...)
#endif

/********************************************************************/
/* Querying API.  */

#define rh_capacity rh_namer(capacity)

I__hl_pattrs hl_obj_size_t
rh_capacity(rh_table_t const * rh_tbl) {
    return rh_mask_and_size_get_cap(rh_tbl->mask_and_size_);
}

/********************************************************************/
/* Initialization / Destruction API.  */
#define rh_init_sz rh_namer(init_sz)
#define rh_init    rh_namer(init)
#define rh_new_sz  rh_namer(new_sz)
#define rh_new     rh_namer(new)
#define rh_deinit  rh_namer(deinit)
#define rh_destroy rh_namer(destroy)
#define rh_clear   rh_namer(clear)
#define rh_shrink  rh_namer(shrink)

static void *
rh_init_sz(rh_table_t * rh_tbl, hl_obj_size_t sz) {
    sz          = I__hl_next_p2(sz);
    uintptr_t p = I__hl_CAST(uintptr_t, rh_alloc_table(sz));
    if (I__hl_unlikely(p == 0)) {
        return rh_allocation_error;
    }
    rh_tbl->mask_and_size_ = rh_get_mask_from_size(sz);
    rh_tbl->p_             = p;
    rh_table_init_extra_hash(rh_tbl);
    return NULL;
}


static void *
rh_init(rh_table_t * rh_tbl) {
    return rh_init_sz(rh_tbl, k__rh_default_size);
}


static rh_table_t *
rh_new_sz(hl_obj_size_t sz) {
    rh_table_t * rh_tbl = (rh_table_t *)hl_alloc(sizeof(rh_table_t));
    if (I__hl_unlikely(rh_tbl == NULL || rh_init_sz(rh_tbl, sz))) {
        return rh_allocation_error;
    }
    return rh_tbl;
}


static rh_table_t *
rh_new() {
    return rh_new_sz(k__rh_default_size);
}

static void
rh_deinit(rh_table_t * rh_tbl) {
    rh_free_table_mask_and_size(rh_tbl->p_, rh_tbl->mask_and_size_);
}

static void
rh_destroy(rh_table_t * rh_tbl) {
    rh_deinit(rh_tbl);
    hl_sfree(rh_tbl, sizeof(rh_tbl));
}

static void
rh_clear(rh_table_t * rh_tbl) {
    rh_mask_and_size_reset_meta_info(rh_tbl->p_, rh_tbl->mask_and_size_);
}

static void
rh_shrink(rh_table_t * rh_tbl, hl_obj_size_t new_sz) {
    hl_obj_size_t cur_mask_and_size = rh_tbl->mask_and_size_;
    if (I__hl_likely(new_sz <= cur_mask_and_size)) {
        new_sz                 = I__hl_next_p2(new_sz);
        cur_mask_and_size      = rh_get_mask_from_size(new_sz);
        rh_tbl->mask_and_size_ = cur_mask_and_size;
    }
    rh_mask_and_size_reset_meta_info(rh_tbl->p_, cur_mask_and_size);
}

/********************************************************************/
/* Insert.  */

#define rh_insert_at_i        rh_namer(insert_at_i)
#define rh_insert_no_conflict rh_namer(insert_no_conflict)
#define rh_shift_indexes      rh_namer(shift_indexes)
#define rh_resize             rh_namer(resize)
#define rh_insert             rh_namer(insert)

I__hl_attrs uintptr_t
rh_insert_at_i(uintptr_t             minfos,
               uintptr_t             kvps,
               uint32_t              i,
               rh_meta_t             minfo,
               const hl_pass_key_t   pk,
               hl_hashret_t hash_val I__hl_unused) {
    rh_store_meta_t(minfos + i, minfo);
    hl_kvp_set(I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)), pk,
               hash_val);

    return kvps + i * sizeof(rh_kvp_t);
}


static void
rh_insert_no_conflict(uintptr_t     minfos_init,
                      uintptr_t     kvps,
                      uintptr_t     kvp,
                      hl_obj_size_t hash_val,
                      hl_obj_size_t mask_and_size) {
    uint32_t  minfo, i, existing_minfo;
    uintptr_t minfos;
    {
        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        kvps   = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        minfo  = rh_init_minfo(hash_val);
    }


    i = 0;

    while (minfo <= (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += k__rh_meta_distance_incr;
        ++i;
    }

    if (existing_minfo) {
        uint32_t placei = i;
        do {
            ++i;
        } while (rh_load_meta_t(minfos + i));

        I__hl_guarantee(i > placei);
        for (; I__hl_unlikely(i > placei); --i) {
            rh_store_meta_t(minfos + i,
                            (rh_meta_t){ rh_load_meta_t(minfos + i - 1) +
                                         k__rh_meta_distance_incr });
            hl_kvp_move(I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)),
                        I__hl_CAST(rh_kvp_t const *,
                                   kvps + (i - 1) * sizeof(rh_kvp_t)));
        }
        I__hl_guarantee(i == placei);
    }
    rh_store_meta_t(minfos + i, (rh_meta_t){ minfo });
    hl_kvp_move(I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)),
                I__hl_CAST(rh_kvp_t const *, kvp));
}

static int32_t
rh_shift_indexes(uintptr_t minfos, uintptr_t kvps, uint32_t i) {


    uint32_t placei = i;
    i++;
    for (;;) {
        /* Make this int32_t signed. For GCC(11.1) this is the only way to
         * get `movzwl` and avoid LCP stalls with the imm16 comparisons.
         * Clang doesn't care either way. */
        I__hl_static_assert(sizeof(int32_t) > sizeof(rh_meta_2x_t));
        int32_t minfo = I__hl_load_T(rh_meta_2x_t, minfos + i).val_;
        if ((I__hl_CAST(rh_metav_t, minfo) == 0)) {
            break;
        }
        if (I__hl_unlikely(rh_check_meta_info_end_plus_one(minfo))) {
            return 1;
        }
        if (rh_check_meta_info_2x_zero(minfo)) {
            ++i;
            break;
        }
        if (I__hl_unlikely(rh_check_meta_info_end_2x_plus_one(minfo))) {
            return 1;
        }
        i += 2;
    }

    I__hl_guarantee(i > placei);
    /* We can get about 10-20% on intert speed by optimizing this. */
    for (; I__hl_unlikely(i > placei); --i) {
        rh_meta_t tmp_rh_meta = { rh_load_meta_t(minfos + i - 1) +
                                  k__rh_meta_distance_incr };
        rh_store_meta_t(minfos + i, tmp_rh_meta);

        hl_kvp_move(
            I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)),
            I__hl_CAST(rh_kvp_t const *, kvps + (i - 1) * sizeof(rh_kvp_t)));
        __asm__ volatile("" : : :);
    }

    I__hl_guarantee(i == placei);
    return 0;
}


/* We don't want to inline this even though its only called from one spot
 * (insert). The rational is we 1) get a tail call anyways and 2) don't want
 * to eat the code size cost on the critical path. */
static I__hl_noinline rh_insert_ret_t
rh_resize(rh_table_t * rh_tbl, const hl_pass_key_t pk, const hl_hashret_t hr) {
    /* TODO: Check clang if any loop optimizations take place. If so maybe
     * can make irreducible. */
    hl_obj_size_t mask_and_size      = rh_tbl->mask_and_size_;
    hl_obj_size_t next_mask_and_size = rh_mask_and_size_get_next(mask_and_size);

    hl_obj_size_t extra_hash_mul I__hl_unused;
#if hl_conf_adaptive_hash
    extra_hash_mul = rh_table_get_extra_hash_mul(rh_tbl);
#endif
#if hl_conf_robust_resize && hl_conf_retry_max
    int32_t retry_counter I__hl_unused;
    retry_counter = 0;
#endif

super_unlikely_retry:
    I__hl_unused;
    uintptr_t next_minfos;
    uintptr_t next_kvps;

    {
        uintptr_t next_table =
            rh_alloc_table(rh_mask_and_size_get_alloc_sz(next_mask_and_size));
        /* The only true error case we can have. */
        if (I__hl_unlikely(next_table == 0UL)) {
            rh_insert_ret_t ret = { I__hl_CAST(rh_kvp_t *, rh_allocation_error),
                                    0 };
            return ret;
        }

        next_minfos = next_table;
        next_kvps   = rh_get_kvps_start(next_minfos, next_mask_and_size);
    }

super_unlikely_retry_new_hash:
    I__hl_unused;
    {
        uintptr_t     old_minfos = rh_tbl->p_;
        uintptr_t     old_kvps   = rh_get_kvps_start(old_minfos, mask_and_size);
        hl_obj_size_t old_cap    = rh_mask_and_size_get_cap(mask_and_size);

        hl_obj_size_t i = 0;


#if hl_conf_adaptive_hash
        hl_obj_size_t inuse I__hl_unused;
        inuse = 0;
#endif

        I__hl_guarantee(i < old_cap);
        for (; i < old_cap; ++i) {
            if (I__hl_likely(rh_load_meta_t(old_minfos + i))) {
                hl_obj_size_t hash_val = hl_rehash(I__hl_CAST(
                    rh_kvp_t const *, old_kvps + i * sizeof(rh_kvp_t)));

                hash_val = rh_get_adapted_hash(extra_hash_mul, hash_val);
                rh_insert_no_conflict(next_minfos, next_kvps,
                                      old_kvps + i * sizeof(rh_kvp_t), hash_val,
                                      next_mask_and_size);
#if hl_conf_adaptive_hash
                ++inuse;
#endif
            }
        }
#if hl_conf_adaptive_hash
        if (I__hl_unlikely(inuse * 2 < (mask_and_size + k__rh_max_lookup))) {
#if hl_conf_robust_resize && hl_conf_retry_max
            if (I__hl_unlikely(retry_counter >= (hl_conf_retry_max))) {
                rh_insert_ret_t ret = { I__hl_CAST(rh_kvp_t *, rh_resize_error),
                                        0 };
                return ret;
            }
            ++retry_counter;
#endif
            extra_hash_mul = rh_get_next_extra_hash_mul(extra_hash_mul);
            rh_table_update_extra_hash_mul(rh_tbl, extra_hash_mul);
            rh_mask_and_size_reset_meta_info(next_minfos, next_mask_and_size);
            goto super_unlikely_retry_new_hash;
        }
#endif
    }
    {
        hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);
        hash_val               = rh_get_adapted_hash(extra_hash_mul, hash_val);

        uintptr_t minfos =
            rh_get_meta_info_start(next_minfos, next_mask_and_size, hash_val);
        uint32_t minfo = rh_init_minfo(hash_val);

        uintptr_t kvps =
            rh_get_kvp_start(next_minfos, next_mask_and_size, hash_val);

        uint32_t i, existing_minfo;
        i = 0;
        while (minfo <= (existing_minfo = rh_load_meta_t(minfos + i))) {
            minfo += k__rh_meta_distance_incr;
            ++i;
        }
#if hl_conf_robust_resize
        if (I__hl_unlikely(
                rh_check_meta_info_end(minfo) ||
                (existing_minfo && rh_shift_indexes(minfos, kvps, i)))) {
            next_mask_and_size = rh_mask_and_size_get_next(next_mask_and_size);
            rh_free_table_mask_and_size(next_minfos, next_mask_and_size);

#if hl_conf_retry_max
            if (I__hl_unlikely(retry_counter >= (hl_conf_retry_max))) {
                rh_insert_ret_t ret = { I__hl_CAST(rh_kvp_t *, rh_resize_error),
                                        0 };
                return ret;
            }
            ++retry_counter;
#endif
            goto super_unlikely_retry;
        }
#else
        rh_shift_indexes(minfos, kvps, i);
#endif
        next_kvps =
            rh_insert_at_i(minfos, kvps, i, (rh_meta_t){ minfo }, pk, hr);
    }

    rh_free_table_mask_and_size(rh_tbl->p_, mask_and_size);
    rh_tbl->mask_and_size_ = next_mask_and_size;
    rh_tbl->p_             = next_minfos;

    rh_insert_ret_t ret = { I__hl_CAST(rh_kvp_t *, next_kvps), 0 };
    return ret;
}


static rh_insert_ret_t
rh_insert(rh_table_t * rh_tbl, const hl_pass_key_t pk) {
    uintptr_t minfos;
    uintptr_t kvps;
    uint32_t  i, minfo, existing_minfo;

    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);
    hash_val               = rh_table_get_adapted_hash(rh_tbl, hash_val);
    {
        hl_obj_size_t mask_and_size = rh_tbl->mask_and_size_;
        uintptr_t     minfos_init   = rh_tbl->p_;

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I__hl_prefetch(I__hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

        /* We need to compute kvps no matter what. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I__hl_CAST(rh_kvp_t const *, kvps));
    }
    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += k__rh_meta_distance_incr;
        ++i;
    }

    while (I__hl_unlikely(minfo == existing_minfo)) {
        if (I__hl_likely(hl_key_eq(I__hl_CAST(rh_kvp_t const *,
                                              kvps + i * sizeof(rh_kvp_t)),
                                   pk, hr) == 0)) {

            rh_insert_ret_t ret = {
                I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)), 1
            };
            return ret;
        }

        minfo += k__rh_meta_distance_incr;
        ++i;
        existing_minfo = rh_load_meta_t(minfos + i);
    }


    if (I__hl_unlikely(rh_check_meta_info_end(minfo))) {
        /* resize. We intentionally are not passing the hash value here. We
         * want to keep register pressure to a minimum. Maybe add config if
         * key has extremely expensive hash. */
        return rh_resize(rh_tbl, pk, hr);
    }


    /* Now we might need to memmove so store the start. */
    if (existing_minfo) {
        if (I__hl_unlikely(rh_shift_indexes(minfos, kvps, i))) {
            return rh_resize(rh_tbl, pk, hr);
        }
    }
    rh_insert_ret_t ret = {
        I__hl_CAST(rh_kvp_t *, rh_insert_at_i(minfos, kvps, i,
                                              (rh_meta_t){ minfo }, pk, hr)),
        0
    };
    return ret;
}

/********************************************************************/
/* Find API.  */
#define rh_find rh_namer(find)
static I__hl_pure rh_kvp_t *
rh_find(rh_table_t const * rh_tbl, const hl_pass_key_t pk) {
    uint32_t      minfo, i, existing_minfo;
    uintptr_t     minfos;
    uintptr_t     kvps;
    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);
    hash_val               = rh_table_get_adapted_hash(rh_tbl, hash_val);

    hl_obj_size_t mask_and_size = rh_tbl->mask_and_size_;
    uintptr_t     minfos_init   = rh_tbl->p_;
    {

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I__hl_prefetch(I__hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

#if hl_conf_aggressive_prefetch
        /* Hot path for finding the kvp. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I__hl_CAST(rh_kvp_t const *, kvps));
#endif
    }

    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += k__rh_meta_distance_incr;
        ++i;
    }

    if (I__hl_likely(minfo == existing_minfo)) {
#if !hl_conf_aggressive_prefetch
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
#endif
        do {
            if (I__hl_likely(hl_key_eq(I__hl_CAST(rh_kvp_t const *,
                                                  kvps + i * sizeof(rh_kvp_t)),
                                       pk, hr) == 0)) {
                return I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t));
            }
            ++i;
            minfo += k__rh_meta_distance_incr;
        } while (minfo == rh_load_meta_t(minfos + i));
    }
    return rh_find_failed;
}
/********************************************************************/
/* Erase API.  */

#define rh_erase rh_namer(erase)


static I__hl_bool_t
rh_erase(rh_table_t const * rh_tbl, const hl_pass_key_t pk) {
    uint32_t      minfo, i, existing_minfo;
    uintptr_t     minfos;
    uintptr_t     kvps;
    hl_hashret_t  hr       = hl_hash(pk);
    hl_obj_size_t hash_val = hl_hashret_get_hashval(hr);
    hash_val               = rh_table_get_adapted_hash(rh_tbl, hash_val);

    hl_obj_size_t mask_and_size = rh_tbl->mask_and_size_;
    uintptr_t     minfos_init   = rh_tbl->p_;
    {

        minfos = rh_get_meta_info_start(minfos_init, mask_and_size, hash_val);
        I__hl_prefetch(I__hl_CAST(rh_meta_t const *, minfos));

        minfo = rh_init_minfo(hash_val);

#if hl_conf_aggressive_prefetch
        /* Hot path for finding the kvp. */
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
        rh_table_aggressive_prefetch(I__hl_CAST(rh_kvp_t const *, kvps));
#endif
    }

    i = 0;
    while (minfo < (existing_minfo = rh_load_meta_t(minfos + i))) {
        minfo += k__rh_meta_distance_incr;
        ++i;
    }

    if (minfo == existing_minfo) {
#if !hl_conf_aggressive_prefetch
        kvps = rh_get_kvp_start(minfos_init, mask_and_size, hash_val);
#endif
        do {
            if (I__hl_likely(hl_key_eq(I__hl_CAST(rh_kvp_t const *,
                                                  kvps + i * sizeof(rh_kvp_t)),
                                       pk, hr) == 0)) {

                while ((existing_minfo = rh_load_meta_t(minfos + i + 1)) >=
                       (k__rh_meta_distance_incr * 2)) {
                    rh_store_meta_t(minfos + i,
                                    (rh_meta_t){ existing_minfo -
                                                 k__rh_meta_distance_incr });
                    hl_kvp_move(
                        I__hl_CAST(rh_kvp_t *, kvps + i * sizeof(rh_kvp_t)),
                        I__hl_CAST(rh_kvp_t const *,
                                   kvps + (i + 1) * sizeof(rh_kvp_t)));
                    ++i;
                }

                rh_store_meta_t(minfos + i, (rh_meta_t){ 0 });
                return 0;
            }
            ++i;
            minfo += k__rh_meta_distance_incr;
        } while (minfo == rh_load_meta_t(minfos + i));
    }
    return 1;
}


/********************************************************************/
/* Undefs.  */

#undef rh_tunable_max_lookup_log2

#undef rh_namer
#undef rh_kvp_t
#undef rh_insert_ret_t
#undef rh_table_t
#undef rh_meta_t
#undef rh_metav_t
#undef rh_meta_2x_t
#undef rh_metav_2x_t


#undef k__rh_max_lookup_log2
#undef k__rh_max_lookup
#undef k__rh_default_size
#undef k__rh_kvps_offset

#undef k__rh_meta_hash_bits
#undef k__rh_meta_hash_shift
#undef k__rh_meta_distance_incr
#undef k__rh_meta_distance_mask
#undef k__rh_meta_max
#undef k__rh_meta_2x_max
#undef k__rh_meta_distance_incr_2x


#undef k__rh_kvp_size_mul


#undef rh_get_hash_idx

#undef rh_mask_and_size_get_kvps_offset
#undef rh_get_meta_info_start
#undef rh_get_kvps_start
#undef rh_get_kvp_start


#undef rh_init_minfo
#undef rh_check_meta_info_end
#undef rh_check_meta_info_end_plus_one
#undef rh_check_meta_info_end_2x_plus_one
#undef rh_check_meta_info_2x_zero

#undef rh_reset_meta_info
#undef rh_reset_meta_info
#undef rh_mask_and_size_reset_meta_info
#undef rh_alloc_table
#undef rh_free_table_mask_and_size

#undef rh_get_adapted_hash
#undef rh_get_next_extra_hash_mul
#undef rh_table_update_extra_hash_mul
#undef rh_table_get_extra_hash_mul
#undef rh_table_get_adapted_hash
#undef rh_table_init_extra_hash

#undef rh_table_aggressive_prefetch
#undef rh_capacity
#undef rh_init_sz
#undef rh_init
#undef rh_new_sz
#undef rh_new
#undef rh_deinit
#undef rh_destroy
#undef rh_clear
#undef rh_shrink

#undef rh_insert_at_i
#undef rh_insert_no_conflict
#undef rh_shift_indexes
#undef rh_resize
#undef rh_insert
#undef rh_find

#undef rh_erase
#define I__hl_UNDEF_ALL
#include "hl-user-defs.h"
