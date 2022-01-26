#ifndef I_hl_UNDEF_ALL


# if hl_trivial_compare
#  define rh_get_valid_erase_key(usr_key, mkey, kvp) (usr_key)
# else
#  define rh_get_valid_erase_key(usr_key, mkey, kvp) rh_mkey_get_key(mkey)
# endif

# if !hl_use_as_map && hl_null_key_invalid
typedef hl_key_t rh_namer(find_ret_t);
#  define rh_key_is_entry         (sizeof(hl_key_t) <= sizeof(void *))
#  define rh_invalid_return_entry ((hl_key_t)0)

#  if hl_trivial_compare
#   define rh_get_valid_success_entry(usr_key, mkey, kvp)                      \
                I_hl_choose_expr(rh_key_is_entry, (usr_key), kvp)
#  else
#   define rh_get_valid_success_entry(usr_key, mkey, kvp)                      \
                I_hl_choose_expr(rh_key_is_entry, rh_mkey_get_key(mkey), kvp)
#  endif

#  define rh_get_valid_fail_entry(usr_key, mkey, kvp)                          \
            rh_get_valid_success_entry(usr_key, mkey, kvp)

# else

typedef rh_kvp_t * rh_namer(find_ret_t);
#  define rh_key_is_entry                                0
#  define rh_invalid_return_entry                        (NULL)
#  define rh_get_valid_success_entry(usr_key, mkey, kvp) (kvp)
#  define rh_get_valid_fail_entry(usr_key, mkey, kvp)    (kvp)
# endif


# define rh_find_ret_t rh_namer(find_ret_t)
# if hl_insert_return_entry || hl_insert_return_result
#  define rh_insert_returns                       1
#  define rh_set_insert_return(ret_var, ret_expr) ((ret_var) = (ret_expr))

typedef struct rh_namer(insert_ret) {
#  if hl_insert_return_result
    I_hl_bool_t result_;
#  endif
#  if hl_insert_return_entry
    rh_find_ret_t entry_;
#  endif
}
rh_namer(insert_ret_t);

#  if hl_insert_return_entry && hl_insert_return_result
#   define rh_internal_insert_make_return(result, entry)                       \
                (rh_insert_ret_t) {                                            \
                    (result), (entry)                                          \
                }
#  elif hl_insert_return_result
#   define rh_internal_insert_make_return(result, entry)                       \
                (rh_insert_ret_t) {                                            \
                    (result)                                                   \
                }
#  else
#   define rh_internal_insert_make_return(result, entry)                       \
                (rh_insert_ret_t) {                                            \
                    (entry)                                                    \
                }
#  endif

#  define rh_insert_make_success_return(usr_key, mkey, kvp)                    \
            rh_internal_insert_make_return(                                    \
                hl_operation_success,                                          \
                rh_get_valid_success_entry(usr_key, mkey, kvp))

#  define rh_insert_make_fail_return(usr_key, mkey, kvp)                       \
            rh_internal_insert_make_return(                                    \
                hl_operation_failure,                                          \
                rh_get_valid_fail_entry(usr_key, mkey, kvp))


#  define rh_insert_ret_t rh_namer(insert_ret_t)
# else
#  define rh_insert_returns                       0
#  define rh_set_insert_return(ret_var, ret_expr) (ret_expr)
#  define rh_insert_make_success_return(...)
#  define rh_insert_make_fail_return(...)
#  define rh_insert_ret_t void
# endif

# if hl_erase_return_copy || hl_erase_return_result

typedef struct rh_namer(ekvp) {
    hl_key_t key_;
#  if hl_use_as_map
    hl_val_t val_;
#  endif
}
rh_namer(ekvp_t);
#  define rh_ekvp_t rh_namer(ekvp_t)

#  if hl_use_as_map
#   define rh_initialize_ekvp(key, val)                                        \
                (rh_ekvp_t) {                                                  \
                    (key), (val)                                               \
                }
#  else
#   define rh_initialize_ekvp(key, val)                                        \
                (rh_ekvp_t) {                                                  \
                    (key)                                                      \
                }
#  endif


typedef struct rh_namer(erase_ret) {
#  if hl_erase_return_result
    I_hl_bool_t result_;
#  endif
#  if hl_erase_return_copy
    rh_ekvp_t entry_;
#  endif
}
rh_namer(erase_ret_t);
#  define rh_erase_ret_t rh_namer(erase_ret_t)
#  if hl_erase_return_copy && hl_erase_return_result
#   define rh_internal_erase_make_return(result, entry)                        \
                (rh_erase_ret_t) {                                             \
                    (result), entry                                            \
                }
#  elif hl_erase_return_copy
#   define rh_internal_erase_make_return(result, entry)                        \
                (rh_erase_ret_t) {                                             \
                    entry                                                      \
                }
#  else
#   define rh_internal_erase_make_return(result, entry)                        \
                (rh_erase_ret_t) {                                             \
                    (result)                                                   \
                }
#  endif


#  define rh_get_valid_erase_entry(usr_key, mkey, kvp, val)                    \
            rh_initialize_ekvp(rh_get_valid_erase_key(usr_key, mkey, kvp), val)


#  define rh_erase_make_success_return(usr_key, mkey, kvp)                     \
            rh_internal_erase_make_return(                                     \
                hl_operation_success,                                          \
                rh_get_valid_erase_entry(usr_key, mkey, kvp, (kvp)->val_))

#  define rh_erase_make_fail_return(...)                                       \
            rh_internal_erase_make_return(hl_operation_failure, {})

# else
#  define rh_erase_make_success_return(...)
#  define rh_erase_make_fail_return(...)
#  define rh_erase_ret_t void
# endif
#else
# undef rh_ekvp_t
# undef rh_erase_make_fail_return
# undef rh_erase_make_success_return
# undef rh_erase_ret_t
# undef rh_find_ret_t
# undef rh_get_valid_erase_entry
# undef rh_get_valid_erase_key
# undef rh_get_valid_fail_entry
# undef rh_get_valid_success_entry
# undef rh_initialize_ekvp
# undef rh_insert_make_fail_return
# undef rh_insert_make_success_return
# undef rh_insert_ret_t
# undef rh_insert_returns
# undef rh_internal_erase_make_return
# undef rh_internal_get_valid_erase_entry
# undef rh_internal_insert_make_return
# undef rh_invalid_return_entry
# undef rh_key_is_entry
#endif
