#ifdef I_hl_UNDEF_ALL

/* Key type. Default -> Error  */
# undef hl_key_t

/* Boolean whether as set or map. Default -> 1 if hl_val_t is defined, 0
 * otherwise.  */
# undef hl_use_as_map

/* Val type. Default -> Error.  */
# undef hl_val_t

/* Type that key will be passed if. Default -> hl_key_t.  */
# undef hl_pass_key_t

/* hl_hashret_t hl_hash(hl_pass_key_t pk); Default -> Error.  */
# undef hl_hash

/* Name prefix. Default -> {key_type}_{hl_val_t}_{set|map}.  */
# undef hl_name

/* Type for obj size (safe to make 32-bit if small table). Default -> size_t. */
# undef hl_obj_size_t

/* rhi only. Number of spare bits in the key. Default -> 0 (will be error if
 * using rhi).  */
# undef hl_spare_key_bits

/* Set if returning NULL in the key slot will indicate it's invalid. Default ->
 * False. */
# undef hl_null_key_invalid


/* Set to make 'erase' return a copy of the rh_kvp_t entry if it succeeds.
 * Default -> False.  */
# undef hl_erase_return_copy

/* Set to make 'erase' return the result (hl_operation_success if the item was
 * deleted, hl_operation_failed otherwise). Default -> True.  */
# undef hl_erase_return_result

/*
 * Set to make insert return a pointer to the new or existing kvp. Default ->
 * True.
 */
# undef hl_insert_return_entry

/*
 * Set to make insert return the result (hl_operation_success if insertion took
 * place, hl_operation_failure otherwise). Default -> True.
 */
# undef hl_insert_return_result


/*
 * rh_key_t hl_pass_key_extract_key(const hl_pass_key_t pk);
 *
 * Get the true key type from pass key.
 *
 * Default -> 'pk'
 */
# undef hl_pass_key_extract_key


/*
 * bool hl_key_eq(hl_key_t tbl_key,
 *                hl_pass_key_t pk,
 *                hl_hashret_t hr);
 *   -> EQ   : False
 *   -> NEQ  : True
 *
 * Default -> hl_memcmp_eq(&tbl_key, &pk);
 */
# undef hl_key_eq

/* Set if comparison is memcmpeq(tbl_key_ptr, &pk, sizeof(hl_key_t)).  */
# undef hl_trivial_compare

/*
 * void hl_kvp_move(hl_kvp_t * restrict next_tbl_kvp_ptr,
 *                  hl_kvp_t const * restrict old_tbl_kvp_ptr);
 *
 * Default -> I_hl_memcpy(next_tbl_kvp_ptr,
 *                         old_tbl_kvp_ptr,
 *                         sizeof(hl_kvp_t));
 */
# undef hl_kvp_move

/*
 * void hl_kvp_set_prepare(hl_pass_key_t pk, hl_hashret_t hr);
 *
 * Hook to run before setting mkvp.val_.
 * Only used in ptr-set.
 *
 * Default -> None
 */
# undef hl_kvp_set_prepare

/*
 * hl_val_t hl_pass_key_extract_val(hl_pass_key_t pk);
 *
 * Extract hl_val_t from hl_pass_key_t.
 *
 * Default -> None
 */
# undef hl_pass_key_extract_val

/*
 * void hl_kvp_destroy(hl_kvp_t * tbl_kvp_ptr);
 *
 * Default -> nop
 */
# undef hl_kvp_destroy

/* Set if destroy is nop.  Default -> True if hl_kvp_destroy is default. False
 * otherwise. */
# undef hl_trivial_destroy

/* Type hl_hash returns. Default -> hl_hashret_t */
# undef hl_hashret_t

/* Macro to convert hl_hashret_t to hl_obj_size_t. Default -> nop */
# undef hl_hashret_get_hashval

/*
 * hl_obj_size_t hl_rehash(hl_key_t key);
 *
 * Default -> hl_hashret_get_hashval(hl_hash(key));
 */
# undef hl_rehash

/*
 * void hl_memset(void * dst, uint8_t v, hl_obj_size_t sz);
 *
 * Default -> memset_c(dst, v, sz);
 */
# undef hl_memset

/*
 * void hl_memcpy(void * restrict dst,
 *                void const * restrict src,
 *                hl_obj_size_t sz);
 *
 * Default -> memcpy_c(dst, src, sz);
 */
# undef hl_memcpy

/*
 * bool hl_memcmp_eq(void const * restrict dst,
 *                  void const * restrict src,
 *                  hl_obj_size_t sz);
 *   -> EQ   : False
 *   -> NEQ  : True
 *
 * Default -> memcmpeq_c(dst, src, sz);
 */
# undef hl_memcmp_eq

/*
 * void * ht_alloc(hl_obj_size_t sz);
 *
 * Default -> safe_malloc(sz);
 */
# undef hl_alloc

/*
 * void * ht_zalloc(hl_obj_size_t sz);
 *
 * Default -> safe_calloc(sz, 1);
 */
# undef hl_zalloc


/*
 * void * ht_aligned_alloc(uint32_t alignment, hl_obj_size_t sz);
 *
 * Default -> safe_aligned_alloc(alignment, sz);
 */
# undef hl_aligned_alloc

/*
 * void * ht_aligned_zalloc(uint32_t alignment, hl_obj_size_t sz);
 *
 * Default -> safe_aligned_zalloc(alignment, sz);
 */
# undef hl_aligned_zalloc

/*
 * void hl_sfree(void * p, hl_obj_size_t sz);
 *
 * Default -> safe_sfree(p, sz);
 */
# undef hl_sfree

/*
 * Some APIs (like `find_exists`) assume the key must be in and are UB if the
 * key isn't. Set this to abort() in cases like that. Default -> False.
 */

# undef hl_conf_die_on_unreachable

/*
 * Set if the return from `find` is predictable. If so this will ensure a branch
 * is emitted (as opposed to cmovcc). Default -> False.
 */
# undef hl_conf_predict_fret

/*
 * Set if hl_aligned_zalloc is fast (i.e not aligned_alloc + memset). Default ->
 * False.
 */
# undef hl_conf_fast_aligned_zalloc

/* Set to have the table enable delete. Default -> True.  */
# undef hl_conf_with_delete

/* Set to have the table robustly check resize (otherwise may assume doubling
 * table size "works"). Default -> True.  */
# undef hl_conf_robust_resize

/* Adapt hashfunction on resize. Default -> False.  */
# undef hl_conf_adaptive_hash

/* Aggressively prefetch. Default -> False.  */
# undef hl_conf_aggressive_prefetch

/* Try max (before returning error), 0 to retry forever. Default -> 0 */
# undef hl_conf_retry_max

/* Table has fixed size, don't grow. If hl_conf_fixed_size is 1, then the fixed
 * size is dynamic. If hl_conf_fixed_size > 1, then fixed size will be used as
 * fixed table size. Default -> 0 */
# undef hl_conf_fixed_size

/* Instead of returning an error code, just abort.  */
# undef hl_conf_die_on_error

#else

# ifndef hl_key_t
#  error "Key type must be defined in 'hl_key_t'"
# endif

# ifndef hl_use_as_map
#  ifdef hl_val_t
#   define hl_use_as_map 1
#  else
#   define hl_use_as_map 0
#  endif
# endif


# if defined(hl_val_t) && !hl_use_as_map
#  error "Providing 'hl_val_t' and 'hl_use_as_map' is false"
# endif


# if !defined(hl_val_t) && hl_use_as_map
#  error                                                                       \
      "Val type must be defined in 'hl_val_t' or 'hl_use_as_map' must be true"
# endif

# ifndef hl_pass_key_t
#  define hl_pass_key_t hl_key_t
# endif


# ifndef hl_hash
#  error "Hash function must be defined in 'hl_hash'"
# endif

# ifndef hl_name
#  if hl_use_as_map
#   define hl_name CAT(hl_key_t, _, hl_val_t, _map)
#  else
#   define hl_name CAT(hl_key_t, _set)
#  endif
# endif

# ifndef hl_obj_size_t
#  define hl_obj_size_t I_hl_obj_size_t
# endif

# ifndef hl_spare_key_bits
#  define hl_spare_key_bits 0
# endif

# ifndef hl_null_key_invalid
#  define hl_null_key_invalid 0
# endif

# ifndef hl_erase_return_copy
#  define hl_erase_return_copy 0
# endif

# ifndef hl_erase_return_result
#  define hl_erase_return_result 1
# endif

# ifndef hl_insert_return_entry
#  define hl_insert_return_entry 1
# endif

# ifndef hl_insert_return_result
#  define hl_insert_return_result 1
# endif


# ifndef hl_pass_key_extract_key
#  define hl_pass_key_extract_key(pk) (pk)
# endif

# ifndef hl_key_eq
#  define hl_key_eq(tbl_key, usr_pass_key, ...)                                \
            hl_memcmp_eq(&(tbl_key), &(usr_pass_key), sizeof(hl_key_t))
#  undef hl_trivial_compare
#  define hl_trivial_compare 1
# endif

# ifndef hl_trivial_compare
#  define hl_trivial_compare 0
# endif


# ifndef hl_kvp_move
#  define hl_kvp_move(next_kvp_ptr, old_kvp_ptr, ...)                          \
            (*(next_kvp_ptr) = *(old_kvp_ptr))
# endif

# ifndef hl_kvp_set_prepare
#  define hl_kvp_set_prepare(usr_pass_key, ...)
# endif

# ifndef hl_kvp_destroy
#  define hl_kvp_destroy(tbl_kvp_ptr, ...) /* empty.  */
#  undef hl_trivial_destroy
#  define hl_trivial_destroy 1
# endif

# ifndef hl_trivial_destroy
#  define hl_trivial_destroy 0
# endif


# ifndef hl_hashret_t
#  define hl_hashret_t I_hl_obj_size_t
# endif

# ifndef hl_hashret_get_hashval
#  define hl_hashret_get_hashval(...) __VA_ARGS__
# endif

# ifndef hl_rehash
#  define hl_rehash(key) hl_hashret_get_hashval(hl_hash(key))
# endif

# ifndef hl_memset
#  define hl_memset(...) I_hl_memset(__VA_ARGS__)
# endif

# ifndef hl_memcpy
#  define hl_memcpy(...) I_hl_memcpy(__VA_ARGS__)
# endif

# ifndef hl_memcmp_eq
#  define hl_memcmp_eq(p0, p1, sz)                                             \
            I_hl_memcmp_eq((const uint8_t * restrict)(p0),                    \
                            (const uint8_t * restrict)(p1), (sz))
# endif

# ifndef hl_alloc
#  define hl_alloc(sz) safe_malloc(sz)
# endif

# ifndef hl_zalloc
#  define hl_zalloc(sz) safe_zalloc(sz)
# endif

# ifndef hl_aligned_alloc
#  define hl_aligned_alloc(alignment, sz) safe_aligned_alloc(alignment, sz)
# endif

# ifndef hl_aligned_zalloc
#  define hl_aligned_zalloc(alignment, sz) safe_aligned_zalloc(alignment, sz)
# endif

# ifndef hl_sfree
#  define hl_sfree(p, sz) safe_sfree(p, sz)
# endif

# ifndef hl_conf_die_on_unreachable
#  define hl_conf_die_on_unreachable 0
# endif

# ifndef hl_conf_predict_fret
#  define hl_conf_predict_fret 0
# endif

# ifndef hl_conf_fast_aligned_zalloc
#  define hl_conf_fast_aligned_zalloc 0
# endif

# ifndef hl_conf_with_delete
#  define hl_conf_with_delete 1
# endif

# ifndef hl_conf_robust_resize
#  define hl_conf_robust_resize 1
# endif

# ifndef hl_conf_adaptive_hash
#  define hl_conf_adaptive_hash 0
# endif

# ifndef hl_conf_aggressive_prefetch
#  define hl_conf_aggressive_prefetch 1
# endif

# ifndef hl_conf_retry_max
#  define hl_conf_retry_max 4
# endif

# ifndef hl_conf_fixed_size
#  define hl_conf_fixed_size 0
# endif

# ifndef hl_conf_die_on_error
#  define hl_conf_die_on_error 1
# endif

#endif
