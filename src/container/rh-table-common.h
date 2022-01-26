#ifndef I_hl_UNDEF_ALL

# if K_rh_meta_inplace
#  define K_rh_mas_piece_sz ((hl_obj_size_t)1)
#  define K_rh_mas_scale    (sizeof(rh_kvp_t) / K_rh_mas_piece_sz)
# else
#  define K_rh_mas_piece_sz sizeof(rh_meta_t)
#  define K_rh_mas_scale    ((hl_obj_size_t)1)
# endif

I_hl_assert_const_eval(K_rh_mas_piece_sz);
I_hl_assert_const_eval(K_rh_mas_scale);


/********************************************************************/
/* Helpers for 'hl_conf_die_on_unreachable'.  */
# if hl_conf_die_on_unreachable
#  define rh_unreachable() I_hl_die("Unreachable")
# else
#  define rh_unreachable()                                                     \
            I_hl_unreachable();                                               \
            I_hl_die("Unreachable")
# endif

/********************************************************************/
/* Helpers for 'hl_conf_aggressive_prefetch'.  */

# if hl_conf_aggressive_prefetch
#  define rh_table_aggressive_prefetch(...) I_hl_prefetch(__VA_ARGS__)
# else
#  define rh_table_aggressive_prefetch(...)
# endif

/********************************************************************/
/* Error handling.  */
# if hl_conf_die_on_error
#  define rh_on_error(...) I_hl_die(__VA_ARGS__)
# else
#  define rh_on_error(...)
# endif

/********************************************************************/
/* Access helpers.  */
# define rh_tbl_get_mask_and_size rh_namer(tbl_get_mask_and_size)

I_hl_pattrs hl_obj_size_t
rh_tbl_get_mask_and_size(rh_table_t const * rh_tbl) {
# if (hl_conf_fixed_size) > 1
    (void)(rh_tbl);
    enum {
        rh_namer(fixed_mask_and_size) =
            I_hl_const_eval_next_p2(hl_conf_fixed_size) * K_rh_mas_piece_sz -
            K_rh_mas_piece_sz
    };
    return rh_namer(fixed_mask_and_size);
# else
    return rh_tbl->mask_and_size_;
# endif
}

/********************************************************************/
/* Helpers for converting 'mask_and_size' to size and vice versa.   */

# define rh_mask_and_size_get_meta_bytes_sz                                    \
        rh_namer(mask_and_size_get_meta_bytes_sz)
# define rh_size_get_meta_bytes_sz     rh_namer(size_get_meta_bytes_sz)
# define rh_mask_and_size_get_sz       rh_namer(mask_and_size_get_sz)
# define rh_mask_and_size_get_cap      rh_namer(mask_and_size_get_cap)
# define rh_get_alloc_sz               rh_namer(get_alloc_sz)
# define rh_mask_and_size_get_alloc_sz rh_namer(mask_and_size_get_alloc_sz)
# define rh_mask_and_size_get_next     rh_namer(mask_and_size_get_next)
# define rh_get_mask_from_size         rh_namer(get_mask_from_size)

I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_meta_bytes_sz(hl_obj_size_t mask_and_size) {
    return (mask_and_size + K_rh_mas_piece_sz) * K_rh_mas_scale +
           K_rh_max_lookup * K_rh_mas_piece_sz * K_rh_mas_scale;
}

I_hl_cattrs hl_obj_size_t
rh_size_get_meta_bytes_sz(hl_obj_size_t size) {
    return size * K_rh_mas_piece_sz * K_rh_mas_scale +
           K_rh_max_lookup * K_rh_mas_piece_sz * K_rh_mas_scale;
}


I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_sz(hl_obj_size_t mask_and_size) {
    return (mask_and_size + K_rh_mas_piece_sz) / K_rh_mas_piece_sz;
}

I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_cap(hl_obj_size_t mask_and_size) {
    return ((mask_and_size + K_rh_mas_piece_sz) / K_rh_mas_piece_sz) +
           K_rh_max_lookup;
}


I_hl_cattrs hl_obj_size_t
rh_get_alloc_sz(hl_obj_size_t sz) {
# if K_rh_meta_inplace
    return sz * sizeof(rh_kvp_t) + K_rh_max_lookup * sizeof(rh_kvp_t);
# else
    return sz * (sizeof(rh_meta_t) + sizeof(rh_kvp_t)) + K_rh_kvps_offset +
           K_rh_max_lookup * sizeof(rh_kvp_t);
# endif
}


I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_alloc_sz(hl_obj_size_t mask_and_size) {
# if K_rh_meta_inplace
    return rh_mask_and_size_get_meta_bytes_sz(mask_and_size);
# else
    I_hl_static_assert(sizeof(rh_kvp_t) % sizeof(rh_meta_t) == 0);
    return mask_and_size * (sizeof(rh_kvp_t) / sizeof(rh_meta_t) + 1) +
           (sizeof(rh_kvp_t) + sizeof(rh_meta_t)) + K_rh_kvps_offset +
           K_rh_max_lookup * sizeof(rh_kvp_t);
# endif
}

I_hl_cattrs hl_obj_size_t
rh_mask_and_size_get_next(hl_obj_size_t mask_and_size) {
    return mask_and_size + mask_and_size + K_rh_mas_piece_sz;
}


I_hl_cattrs hl_obj_size_t
rh_get_mask_from_size(hl_obj_size_t sz) {
    return sz - K_rh_mas_piece_sz;
}

/********************************************************************/
/* Helpers for allocating/freeing table memory.  */

# define rh_reset_table_meta_info     rh_namer(reset_table_meta_info)
# define rh_reset_table               rh_namer(reset_table)
# define rh_mask_and_size_reset_table rh_namer(mask_and_size_reset_table)
# define rh_alloc_table               rh_namer(alloc_table)
# define rh_free_table_mask_and_size  rh_namer(free_table_mask_and_size)

I_hl_attrs void
rh_reset_table_meta_info(rh_tbl_ptr_t p, hl_obj_size_t bytes) {
    hl_memset(I_hl_CAST(rh_meta_reset_t, p), 0, bytes);
}


I_hl_attrs void
rh_reset_table(rh_tbl_ptr_t p, hl_obj_size_t sz) {
    rh_reset_table_meta_info(p, rh_size_get_meta_bytes_sz(sz));
}


I_hl_attrs void
rh_mask_and_size_reset_table(rh_tbl_ptr_t p, hl_obj_size_t mask_and_size) {
    rh_reset_table_meta_info(p,
                             rh_mask_and_size_get_meta_bytes_sz(mask_and_size));
}


I_hl_attrs rh_tbl_ptr_t
rh_alloc_table(hl_obj_size_t sz) {
    rh_tbl_ptr_t p;
# if K_rh_meta_inplace
/* Ideally we would have a fast aligned zalloc function but requires real
 * impl to do so.  */
#  if hl_conf_fast_aligned_zalloc
    p = I_hl_CAST(rh_tbl_ptr_t, hl_aligned_zalloc(I_hl_cache_line_size,
                                                   rh_get_alloc_sz(sz)));
#  else
    p = I_hl_CAST(rh_tbl_ptr_t, hl_zalloc(rh_get_alloc_sz(sz)));
#  endif

    I_hl_prefetch(p + I_hl_cache_line_size / sizeof(*p));
    I_hl_prefetch(p + I_hl_cache_line_size * 2 / sizeof(*p));
    I_hl_prefetch(p + I_hl_cache_line_size * 3 / sizeof(*p));
# else
    p = I_hl_CAST(uintptr_t, hl_aligned_alloc(I_hl_cache_line_size,
                                               rh_get_alloc_sz(sz)));
    I_hl_prefetch((void const *)(p + I_hl_cache_line_size));
    I_hl_prefetch((void const *)(p + I_hl_cache_line_size * 2));
    I_hl_prefetch((void const *)(p + I_hl_cache_line_size * 3));
    rh_reset_table(p, sz);
# endif

    return p;
}

I_hl_attrs void
rh_free_table_mask_and_size(rh_tbl_ptr_t p, hl_obj_size_t mask_and_size) {
    hl_sfree(I_hl_CAST(void *, p),
             rh_mask_and_size_get_alloc_sz(mask_and_size));
}


/********************************************************************/
/* Querying API.  */

# define rh_capacity rh_namer(capacity)

I_hl_pattrs hl_obj_size_t
rh_capacity(rh_table_t const * rh_tbl) {
    return rh_mask_and_size_get_cap(rh_tbl_get_mask_and_size(rh_tbl));
}


/********************************************************************/
/* Initialization / Destruction API.  */
# define rh_init_sz          rh_namer(init_sz)
# define rh_internal_init_sz rh_namer(internal_init_sz)
# define rh_init             rh_namer(init)
# define rh_new_sz           rh_namer(new_sz)
# define rh_new              rh_namer(new)
# define rh_deinit           rh_namer(deinit)
# define rh_destroy          rh_namer(destroy)
# define rh_clear            rh_namer(clear)
# define rh_shrink           rh_namer(shrink)

static I_hl_bool_t
rh_internal_init_sz(rh_table_t * rh_tbl, hl_obj_size_t sz) {
    rh_tbl_ptr_t p;
    if (sz <= K_rh_max_lookup / 2) {
        sz = K_rh_max_lookup;
    }
    else {
        sz = I_hl_next_p2(sz);
    }
    p = rh_alloc_table(sz);
    if (I_hl_unlikely(p == 0)) {
        rh_on_error("Allocation error");
        return hl_operation_failure;
    }
# if (hl_conf_fixed_size) <= 1
    *rh_tbl = (rh_table_t){ p, rh_get_mask_from_size(sz) };
# else
    *rh_tbl = (rh_table_t){ p };
# endif
    return hl_operation_success;
}


static I_hl_bool_t
rh_init(rh_table_t * rh_tbl) {
# if (hl_conf_fixed_size) > 1
    return rh_internal_init_sz(rh_tbl, (hl_conf_fixed_size));
# else
    return rh_internal_init_sz(rh_tbl, K_rh_default_size);
# endif
}

static I_hl_bool_t
rh_init_sz(rh_table_t * rh_tbl, hl_obj_size_t sz) {
# if (hl_conf_fixed_size) > 1
    (void)(sz);
    return rh_internal_init_sz(rh_tbl, (hl_conf_fixed_size));
# else
    return rh_internal_init_sz(rh_tbl, sz);
# endif
}


static rh_table_t *
rh_new_sz(hl_obj_size_t sz) {
    rh_table_t * rh_tbl = (rh_table_t *)hl_alloc(sizeof(rh_table_t));
    if (I_hl_unlikely(rh_tbl == NULL || rh_init_sz(rh_tbl, sz))) {
        rh_on_error("Allocation error");
        return NULL;
    }
    return rh_tbl;
}


static rh_table_t *
rh_new(void) {
    return rh_new_sz(K_rh_default_size);
}

static void
rh_deinit(rh_table_t * rh_tbl) {
# if !hl_trivial_destroy
/* Destroy all.  */
# endif
    rh_free_table_mask_and_size(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl));
}

static void
rh_destroy(rh_table_t * rh_tbl) {
    rh_deinit(rh_tbl);
    hl_sfree(rh_tbl, sizeof(rh_tbl));
}

static void
rh_clear(rh_table_t * rh_tbl) {
    rh_mask_and_size_reset_table(rh_tbl->p_, rh_tbl_get_mask_and_size(rh_tbl));
}

static void
rh_shrink(rh_table_t * rh_tbl, hl_obj_size_t new_sz) {
    hl_obj_size_t cur_mask_and_size = rh_tbl_get_mask_and_size(rh_tbl);
# if !(hl_conf_fixed_size)
    if (I_hl_likely(new_sz > (K_rh_max_lookup / 2) &&
                     new_sz <= cur_mask_and_size)) {
        new_sz                 = I_hl_next_p2(new_sz);
        cur_mask_and_size      = rh_get_mask_from_size(new_sz);
        rh_tbl->mask_and_size_ = cur_mask_and_size;
    }
# else
    (void)(new_sz);
# endif
    rh_mask_and_size_reset_table(rh_tbl->p_, cur_mask_and_size);
}


/********************************************************************/
/* Helpers for manipulating hash value.  */
# define rh_get_hash_idx rh_namer(get_hash_idx)

I_hl_cattrs hl_obj_size_t
rh_get_hash_idx(hl_obj_size_t mask_and_size, hl_obj_size_t hash_val) {
    return hash_val & mask_and_size;
}

/********************************************************************/
/********************************************************************/
/* Return Type Access Method.  */
# define rh_iret_entry   rh_namer(iret_entry)
# define rh_iret_key     rh_namer(iret_key)
# define rh_iret_val     rh_namer(iret_val)
# define rh_iret_valp    rh_namer(iret_valp)
# define rh_iret_success rh_namer(iret_success)
# define rh_iret_fail    rh_namer(iret_fail)

# define rh_fret_entry   rh_namer(fret_entry)
# define rh_fret_key     rh_namer(fret_key)
# define rh_fret_val     rh_namer(fret_val)
# define rh_fret_valp    rh_namer(fret_valp)
# define rh_fret_success rh_namer(fret_success)
# define rh_fret_fail    rh_namer(fret_fail)

# define rh_eret_key     rh_namer(eret_key)
# define rh_eret_val     rh_namer(eret_val)
# define rh_eret_success rh_namer(eret_success)
# define rh_eret_fail    rh_namer(eret_fail)

# if hl_insert_return_entry
I_hl_cattrs rh_find_ret_t
rh_iret_entry(rh_insert_ret_t ret) {
    return (ret.entry_);
}


I_hl_cattrs hl_key_t
rh_iret_key(rh_insert_ret_t ret) {
    return I_hl_choose_expr(
        rh_key_is_entry, ret.entry_,
        rh_kvp_get_key(I_hl_choose_expr(rh_key_is_entry, (rh_kvp_t *)(NULL),
                                         ret.entry_)));
}


#  if hl_use_as_map

I_hl_cattrs hl_val_t
rh_iret_val(rh_insert_ret_t ret) {
    return ret.entry_->val_;
}


I_hl_cattrs hl_val_t *
rh_iret_valp(rh_insert_ret_t ret) {
    return &(ret.entry_->val_);
}
#  endif
# endif
# if hl_insert_return_result
I_hl_cattrs I_hl_bool_t
rh_iret_success(rh_insert_ret_t ret) {
    return ret.result_ == hl_operation_success;
}


I_hl_cattrs I_hl_bool_t
rh_iret_fail(rh_insert_ret_t ret) {
    return ret.result_ != hl_operation_success;
}
# endif

I_hl_cattrs rh_find_ret_t
rh_fret_entry(rh_find_ret_t ret) {
    return ret;
}

I_hl_cattrs hl_key_t
rh_fret_key(rh_find_ret_t ret) {
    return I_hl_choose_expr(rh_key_is_entry, ret,
                             rh_kvp_get_key(I_hl_choose_expr(
                                 rh_key_is_entry, (rh_kvp_t *)(NULL), ret)));
}

# if hl_use_as_map

I_hl_cattrs hl_val_t
rh_fret_val(rh_find_ret_t ret) {
    return ret->val_;
}


I_hl_cattrs hl_val_t *
rh_fret_valp(rh_find_ret_t ret) {
    return &(ret->val_);
}
# endif


I_hl_cattrs I_hl_bool_t
rh_fret_success(rh_find_ret_t ret) {
    return ret != rh_invalid_return_entry;
}


I_hl_cattrs I_hl_bool_t
rh_fret_fail(rh_find_ret_t ret) {
    return ret == rh_invalid_return_entry;
}

# if hl_erase_return_copy


I_hl_cattrs hl_key_t
rh_eret_key(rh_erase_ret_t ret) {
    return ret.entry_.key_;
}


#  if hl_use_as_map

I_hl_cattrs hl_val_t
rh_eret_val(rh_erase_ret_t ret) {
    return ret.entry_.val_;
}
#  endif
# endif

# if hl_erase_return_result

I_hl_cattrs I_hl_bool_t
rh_eret_success(rh_erase_ret_t ret) {
    return ret.result_ == hl_operation_success;
}


I_hl_cattrs I_hl_bool_t
rh_eret_fail(rh_erase_ret_t ret) {
    return ret.result_ != hl_operation_success;
}
# endif

/********************************************************************/
/* Resize Error Case.  */
# if (hl_conf_fixed_size)
I_hl_cattrs rh_insert_ret_t
rh_resize(rh_table_t * rh_tbl, const hl_pass_key_t pk, const hl_hashret_t hr) {
    (void)(rh_tbl);
    (void)(pk);
    (void)(hr);

    rh_on_error("Table full");
    return rh_internal_insert_make_return(hl_operation_failure,
                                          rh_invalid_return_entry);
}
# endif


#else

# undef K_rh_mas_piece_sz
# undef K_rh_mas_scale
# undef rh_tbl_get_mask_and_size
# undef rh_mask_and_size_get_meta_bytes_sz
# undef rh_size_get_meta_bytes_sz
# undef rh_mask_and_size_get_sz
# undef rh_mask_and_size_get_cap
# undef rh_get_alloc_sz
# undef rh_mask_and_size_get_alloc_sz
# undef rh_mask_and_size_get_next
# undef rh_get_mask_from_size
# undef rh_reset_table_meta_info
# undef rh_reset_table
# undef rh_mask_and_size_reset_table
# undef rh_alloc_table
# undef rh_free_table_mask_and_size
# undef rh_table_aggressive_prefetch
# undef rh_capacity
# undef rh_init_sz
# undef rh_internal_init_sz
# undef rh_init
# undef rh_new_sz
# undef rh_new
# undef rh_deinit
# undef rh_destroy
# undef rh_clear
# undef rh_shrink
# undef rh_get_hash_idx
# undef rh_iret_entry
# undef rh_iret_key
# undef rh_iret_val
# undef rh_iret_valp
# undef rh_iret_success
# undef rh_iret_fail
# undef rh_fret_entry
# undef rh_fret_key
# undef rh_fret_val
# undef rh_fret_valp
# undef rh_fret_success
# undef rh_fret_fail
# undef rh_eret_key
# undef rh_eret_val
# undef rh_eret_success
# undef rh_eret_fail
#endif
