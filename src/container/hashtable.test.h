#define hl_key_t test_key_t
#ifdef test_null_invalid
# define hl_null_key_invalid test_null_invalid
#endif


#ifdef test_val_t
# define hl_val_t             test_val_t
# define test_map_do(...)     __VA_ARGS__
# define test_map_assert(...) test_assert(__VA_ARGS__)
#else
# define test_val_t uint8_t
# define test_map_do(...)
# define test_map_assert(...)
#endif


#ifdef test_erase_copy
# define test_ec_assert(...)     test_assert(__VA_ARGS__)
# define test_ec_map_assert(...) test_map_assert(__VA_ARGS__)
# define hl_erase_return_copy    1
#else
# define test_ec_assert(...)
# define test_ec_map_assert(...)
#endif

#ifdef test_key_eq
# define hl_key_eq(...) test_key_eq(__VA_ARGS__)
#endif

#ifndef test_spare_bits
# define test_spare_bits 0
#endif
#define hl_name test_name
#define hl_hash test_hash

#define hl_spare_key_bits test_spare_bits

#ifndef test_key_equals
# define test_key_equals equals_val
#endif

#ifndef test_val_equals
# define test_val_equals equals_val
#endif

#ifndef test_ent_equals
# define test_ent_equals equals_val
#endif

#ifndef test_pass_key
# define test_pass_key(kp) (kp)
#endif

#ifndef test_pass_val
# define test_pass_val(vp) (vp)
#endif
#ifndef test_key_base_t
# define test_key_base_t test_key_t
#endif
#ifndef test_val_base_t
# define test_val_base_t test_val_t
#endif

/* Just for debug.  */
#define hl_conf_die_on_unreachable 1
#include HASHTABLE_H

#define test_namer(name) I_hl_namer(test_name, name)


#define test_table_t  test_namer(table_t)
#define test_init     test_namer(init)
#define test_init_sz  test_namer(init_sz)
#define test_deinit   test_namer(deinit)
#define test_capacity test_namer(capacity)
#define test_shrink   test_namer(shrink)
#define test_clear    test_namer(clear)

#define test_insert      test_namer(insert)
#define test_find        test_namer(find)
#define test_find_exists test_namer(find_exists)
#define test_erase       test_namer(erase)

#define test_iret_success test_namer(iret_success)
#define test_iret_fail    test_namer(iret_fail)
#define test_iret_key     test_namer(iret_key)
#define test_iret_valp    test_namer(iret_valp)
#define test_iret_val     test_namer(iret_val)
#define test_iret_entry   test_namer(iret_entry)

#define test_eret_success test_namer(eret_success)
#define test_eret_fail    test_namer(eret_fail)
#define test_eret_key     test_namer(eret_key)
#define test_eret_val     test_namer(eret_val)

#define test_fret_success test_namer(fret_success)
#define test_fret_fail    test_namer(fret_fail)
#define test_fret_key     test_namer(fret_key)
#define test_fret_val     test_namer(fret_val)
#define test_fret_entry   test_namer(fret_entry)

#define test_insert_ret_t test_namer(insert_ret_t)
#define test_find_ret_t   test_namer(find_ret_t)
#define test_erase_ret_t  test_namer(erase_ret_t)


#define k_test_size                                                            \
    CAST(                                                                      \
        uint32_t,                                                              \
        (sizeof_bits(test_key_base_t) - test_spare_bits) >= 32                 \
            ? CAST(uint32_t, TEST_SIZE)                                        \
            : CMIN(CAST(uint32_t, TEST_SIZE),                                  \
                   (1UL << (sizeof_bits(test_key_base_t) - test_spare_bits))))

#ifndef NL
# define NL  // NEVER_INLINE
#endif

static NL int32_t
test_namer(test_init)(void) {
    test_table_t tbl;
    uint32_t     i;
    vprint("Running: " V_TO_STR(test_namer(test_init)) " -> %u\n", k_test_size);
    test_assert(test_init(&tbl) == hl_operation_success);
    test_deinit(&tbl);
    for (i = 0; i <= (1 << 12); ++i) {
        vvprint("\tTest Size: %u\n", i);
        test_assert(test_init_sz(&tbl, i) == hl_operation_success);
        test_assert(test_capacity(&tbl) >= i);
        test_deinit(&tbl);
    }
    for (; i < k_test_size; i += i) {
        vvprint("\tTest Size: %u\n", i);
        test_assert(test_init_sz(&tbl, i) == hl_operation_success);
        test_assert(test_capacity(&tbl) >= i);
        test_deinit(&tbl);
    }
    return 0;
}

static NL void
test_namer(deinit_kvps)(test_key_base_t * keys,
                        test_key_base_t * keys_dup,
                        test_val_base_t * vals) {
    die_assert(keys && vals && keys_dup);

    safe_sfree(keys, k_test_size * 2 * sizeof(test_key_base_t));
    safe_sfree(vals, k_test_size * sizeof(test_val_base_t));
}

static NL void
test_namer(init_kvps)(test_key_base_t ** keys_out,
                      test_key_base_t ** keys_dup_out,
                      test_val_base_t ** vals_out) {
    test_key_base_t * keys = (test_key_base_t *)safe_malloc(
        k_test_size * 2 * sizeof(test_key_base_t));
    test_key_base_t * keys_dup = keys + k_test_size;
    test_val_base_t * vals =
        (test_val_base_t *)safe_malloc(k_test_size * sizeof(test_val_base_t));
    uint32_t * rindexes = make_true_rand32_buffer(k_test_size);
    uint32_t   i;
    die_assert(keys_out && vals_out && keys_dup_out);

    for (i = 0; i < k_test_size; ++i) {
        size_t   j;
        uint32_t iv = k_test_size + i;
        uint32_t ik = i;
#ifdef test_skip_zero_key
        ik += 1;
        { const_assert(sizeof(test_key_base_t) > 2); }
#endif

        {
            const_assert(sizeof(iv) == sizeof(ik) &&
                         sizeof(ik) == sizeof(wchar_t));
        }
        __builtin_memset(keys + i, 0, sizeof(test_key_base_t));
        __builtin_memset(vals + i, 0, sizeof(test_val_base_t));

        if (sizeof(ik) >= sizeof(test_key_base_t)) {
            __builtin_memcpy(keys + i, &ik, sizeof(test_key_base_t));
        }
        else {
            size_t n = sizeof(test_key_base_t) - sizeof(iv);
            for (j = 0; j <= n; j += sizeof(ik)) {
                __builtin_memcpy(CAST(safe_uint8_t *, keys + i) + j, &ik,
                                 sizeof(ik));
            }
            __builtin_memcpy(CAST(safe_uint8_t *, keys + i) +
                                 sizeof(test_key_base_t) - sizeof(ik),
                             &ik, sizeof(ik));
        }
        if (sizeof(iv) >= sizeof(test_val_base_t)) {
            __builtin_memcpy(vals + i, &iv, sizeof(test_val_base_t));
        }
        else {
            size_t n = sizeof(test_val_base_t) - sizeof(iv);
            for (j = 0; j <= n; j += sizeof(iv)) {
                __builtin_memcpy(CAST(safe_uint8_t *, vals + i) + j, &iv,
                                 sizeof(iv));
            }
            __builtin_memcpy(CAST(safe_uint8_t *, vals + i) +
                                 sizeof(test_val_base_t) - sizeof(iv),
                             &iv, sizeof(iv));
        }
    }
    for (i = 0; 0 && i < k_test_size * 2; ++i) {
        uint32_t        from_idx = rindexes[i % k_test_size] % k_test_size;
        test_key_base_t from_k   = keys[from_idx];
        keys[from_idx]           = keys[i % k_test_size];
        keys[i % k_test_size]    = from_k;
    }
    memcpy(keys_dup, keys, k_test_size * sizeof(test_key_base_t));
    safe_sfree(rindexes, k_test_size * sizeof(uint32_t));

    *keys_out     = keys;
    *keys_dup_out = keys_dup;
    *vals_out     = vals;
}

static NL int32_t
test_namer(test_i)(void) {

    uint32_t          i, t;
    test_key_base_t * keys, *keys_dup;
    test_val_base_t * vals;
    test_namer(init_kvps)(&keys, &keys_dup, &vals);
    vprint("Running: " V_TO_STR(test_namer(test_i)) " -> %u\n", k_test_size);


    for (i = 1; i <= k_test_size; i += ((i * 3) / 2)) {

        uint32_t          j;
        test_table_t      tbl;
        test_insert_ret_t ret0, ret1;
        vvprint("\tTest Size: %u\n", i);
        test_init(&tbl);
        for (t = 0; t < 10; ++t) {
            for (j = 0; j < i; ++j) {
                ret0 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_success(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys[j])));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys_dup[j])));
                test_map_do((*test_iret_valp(ret0)) = test_pass_val(vals[j]));
                ret1 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(ret1));
                test_assert(test_key_equals(test_iret_key(ret1),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_iret_val(ret1),
                                                test_pass_val(vals[j])));

                test_assert(test_ent_equals(test_iret_entry(ret0),
                                            test_iret_entry(ret1)));

                ret1 = test_insert(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_iret_fail(ret1));
                test_assert(test_key_equals(test_iret_key(ret1),
                                            test_pass_key(keys_dup[j])));
                test_assert(test_ent_equals(test_iret_entry(ret0),
                                            test_iret_entry(ret1)));
            }

            for (j = 0; j < i; ++j) {
                ret0 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_iret_val(ret0),
                                                test_pass_val(vals[j])));
            }

            test_shrink(&tbl, true_rand32() % (k_test_size * 2));
        }


        test_deinit(&tbl);
    }
    test_namer(deinit_kvps)(keys, keys_dup, vals);
    return 0;
}

static NL int32_t
test_namer(test_if)(void) {

    uint32_t          i;
    test_key_base_t * keys, *keys_dup;
    test_val_base_t * vals;
    test_namer(init_kvps)(&keys, &keys_dup, &vals);
    vprint("Running: " V_TO_STR(test_namer(init_if)) " -> %u\n", k_test_size);

    for (i = 1; i <= k_test_size; i += ((i * 3) / 2)) {

        uint32_t          j;
        test_table_t      tbl;
        test_insert_ret_t iret;
        test_find_ret_t   fret;
        vvprint("\tTest Size: %u\n", i);
        test_init(&tbl);
        for (j = 0; j < i; ++j) {
            fret = test_find(&tbl, test_pass_key(keys[j]));
            test_assert(test_fret_fail(fret));
            test_assert(fret == test_find(&tbl, test_pass_key(keys_dup[j])));

            iret = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_success(iret));
            test_assert(
                test_key_equals(test_iret_key(iret), test_pass_key(keys[j])));
            test_map_do(*test_iret_valp(iret) = test_pass_val(vals[j]));

            iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
            test_assert(test_iret_fail(iret));
            test_assert(test_key_equals(test_iret_key(iret),
                                        test_pass_key(keys_dup[j])));

            fret = test_find(&tbl, test_pass_key(keys[j]));
            test_assert(test_fret_success(fret));
            test_assert(fret ==
                        test_find_exists(&tbl, test_pass_key(keys_dup[j])));
            test_assert(fret == test_find(&tbl, test_pass_key(keys_dup[j])));
            test_assert(
                test_ent_equals(test_fret_entry(fret), test_iret_entry(iret)));
            test_assert(
                test_key_equals(test_fret_key(fret), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_fret_val(fret), test_pass_val(vals[j])));

            iret = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_fail(iret));
            test_assert(
                test_ent_equals(test_fret_entry(fret), test_iret_entry(iret)));
        }

        for (j = 0; j < i; ++j) {
            fret = test_find(&tbl, test_pass_key(keys[j]));
            test_assert(fret == test_find(&tbl, test_pass_key(keys_dup[j])));
            test_assert(test_fret_success(fret));
            test_assert(fret ==
                        test_find_exists(&tbl, test_pass_key(keys_dup[j])));
            test_assert(
                test_key_equals(test_fret_key(fret), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_fret_val(fret), test_pass_val(vals[j])));

            iret = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_fail(iret));
            test_assert(
                test_key_equals(test_iret_key(iret), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_iret_val(iret), test_pass_val(vals[j])));

            test_assert(
                test_ent_equals(test_fret_entry(fret), test_iret_entry(iret)));
            iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
            test_assert(test_iret_fail(iret));
            test_assert(test_key_equals(test_iret_key(iret),
                                        test_pass_key(keys_dup[j])));
            test_map_assert(
                test_val_equals(test_iret_val(iret), test_pass_val(vals[j])));
        }

        test_clear(&tbl);
        for (j = 0; j < i; ++j) {
            fret = test_find(&tbl, test_pass_key(keys[j]));
            test_assert(test_fret_fail(fret));
            test_assert(fret == test_find(&tbl, test_pass_key(keys_dup[j])));

            iret = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_success(iret));
            test_assert(
                test_key_equals(test_iret_key(iret), test_pass_key(keys[j])));
            test_map_do((*test_iret_valp(iret)) = test_pass_val(vals[j]));

            iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
            test_assert(test_iret_fail(iret));
            test_assert(test_key_equals(test_iret_key(iret),
                                        test_pass_key(keys_dup[j])));

            fret = test_find(&tbl, test_pass_key(keys[j]));
            test_assert(fret == test_find(&tbl, test_pass_key(keys_dup[j])));
            test_assert(
                test_ent_equals(test_fret_entry(fret), test_iret_entry(iret)));
            test_assert(
                test_key_equals(test_fret_key(fret), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_fret_val(fret), test_pass_val(vals[j])));

            iret = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_fail(iret));
            test_assert(
                test_ent_equals(test_fret_entry(fret), test_iret_entry(iret)));
        }


        test_deinit(&tbl);
    }
    test_namer(deinit_kvps)(keys, keys_dup, vals);
    return 0;
}


static NL int32_t
test_namer(test_ie)(void) {

    uint32_t          i;
    test_key_base_t * keys, *keys_dup;
    test_val_base_t * vals;
    test_namer(init_kvps)(&keys, &keys_dup, &vals);
    vprint("Running: " V_TO_STR(test_namer(init_ie)) " -> %u\n", k_test_size);

    for (i = 1; i <= k_test_size; i += ((i * 3) / 2)) {

        uint32_t          j, r;
        test_table_t      tbl;
        test_insert_ret_t ret0, ret1;
        test_erase_ret_t  eret;
        vvprint("\tTest Size: %u\n", i);
        test_init(&tbl);

        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {
                test_assert(
                    test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));
                test_assert(test_eret_fail(
                    test_erase(&tbl, test_pass_key(keys_dup[j]))));

                ret0 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_success(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys[j])));
                test_map_do((*test_iret_valp(ret0)) = test_pass_val(vals[j]));

                ret0 = test_insert(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_iret_fail(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys_dup[j])));

                ret1 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(ret1));
                test_assert(test_key_equals(test_iret_key(ret1),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_iret_val(ret1),
                                                test_pass_val(vals[j])));

                test_assert(test_ent_equals(test_iret_entry(ret0),
                                            test_iret_entry(ret1)));

                eret = test_erase(&tbl, test_pass_key(keys[j]));
                test_assert(test_eret_success(eret));

                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys[j])));
                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys_dup[j])));
                test_ec_map_assert(test_val_equals(test_eret_val(eret),
                                                   test_pass_val(vals[j])));

                test_assert(test_eret_fail(
                    test_erase(&tbl, test_pass_key(keys_dup[j]))));

                ret0 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_success(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys[j])));
                test_map_do((*test_iret_valp(ret0)) = test_pass_val(vals[j]));

                ret1 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(ret1));
                test_assert(test_key_equals(test_iret_key(ret1),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_iret_val(ret1),
                                                test_pass_val(vals[j])));

                test_assert(test_ent_equals(test_iret_entry(ret0),
                                            test_iret_entry(ret1)));
            }

            for (j = 0; j < i; ++j) {
                ret0 = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(ret0));
                test_assert(test_key_equals(test_iret_key(ret0),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_iret_val(ret0),
                                                test_pass_val(vals[j])));

                eret = test_erase(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_eret_success(eret));

                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys[j])));
                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys_dup[j])));
                test_ec_map_assert(test_val_equals(test_eret_val(eret),
                                                   test_pass_val(vals[j])));


                test_assert(
                    test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));
            }
        }

        test_clear(&tbl);
        for (j = 0; j < i; ++j) {
            test_assert(
                test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));

            ret0 = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_success(ret0));
            test_assert(
                test_key_equals(test_iret_key(ret0), test_pass_key(keys[j])));
            test_map_do((*test_iret_valp(ret0)) = test_pass_val(vals[j]));

            ret1 = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_fail(ret1));
            test_assert(
                test_key_equals(test_iret_key(ret1), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_iret_val(ret1), test_pass_val(vals[j])));

            ret1 = test_insert(&tbl, test_pass_key(keys_dup[j]));
            test_assert(test_iret_fail(ret1));
            test_assert(test_key_equals(test_iret_key(ret1),
                                        test_pass_key(keys_dup[j])));

            test_assert(
                test_ent_equals(test_iret_entry(ret0), test_iret_entry(ret1)));

            eret = test_erase(&tbl, test_pass_key(keys[j]));
            test_assert(test_eret_success(eret));

            test_ec_assert(
                test_key_equals(test_eret_key(eret), test_pass_key(keys[j])));
            test_ec_assert(test_key_equals(test_eret_key(eret),
                                           test_pass_key(keys_dup[j])));
            test_ec_map_assert(
                test_val_equals(test_eret_val(eret), test_pass_val(vals[j])));

            ret0 = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_success(ret0));
            test_assert(
                test_key_equals(test_iret_key(ret0), test_pass_key(keys[j])));
            test_map_do((*test_iret_valp(ret0)) = test_pass_val(vals[j]));

            ret1 = test_insert(&tbl, test_pass_key(keys[j]));
            test_assert(test_iret_fail(ret1));
            test_assert(
                test_key_equals(test_iret_key(ret1), test_pass_key(keys[j])));
            test_map_assert(
                test_val_equals(test_iret_val(ret1), test_pass_val(vals[j])));

            ret1 = test_insert(&tbl, test_pass_key(keys_dup[j]));
            test_assert(test_iret_fail(ret1));
            test_assert(
                test_key_equals(test_iret_key(ret1), test_pass_key(keys[j])));

            test_assert(
                test_ent_equals(test_iret_entry(ret0), test_iret_entry(ret1)));
        }

        test_deinit(&tbl);
    }
    test_namer(deinit_kvps)(keys, keys_dup, vals);
    return 0;
}


static NL int32_t
test_namer(test_ief)(void) {

    uint32_t          i;
    test_key_base_t * keys, *keys_dup;
    test_val_base_t * vals;
    test_namer(init_kvps)(&keys, &keys_dup, &vals);
    vprint("Running: " V_TO_STR(test_namer(init_ief)) " -> %u\n", k_test_size);

    for (i = 1; i <= k_test_size; i += ((i * 3) / 2)) {

        uint32_t          j, r;
        test_table_t      tbl;
        test_insert_ret_t iret;
        test_find_ret_t   fret;
        test_erase_ret_t  eret;
        vvprint("\tTest Size: %u\n", i);
        test_init(&tbl);

        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {
                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(test_fret_fail(fret));
                test_assert(fret ==
                            test_find(&tbl, test_pass_key(keys_dup[j])));
                test_assert(
                    test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));
                test_assert(test_eret_fail(
                    test_erase(&tbl, test_pass_key(keys_dup[j]))));

                iret = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_success(iret));
                test_assert(test_key_equals(test_iret_key(iret),
                                            test_pass_key(keys[j])));

                iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_iret_fail(iret));
                test_assert(test_key_equals(test_iret_key(iret),
                                            test_pass_key(keys[j])));

                test_map_do((*test_iret_valp(iret)) = test_pass_val(vals[j]));

                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(fret ==
                            test_find(&tbl, test_pass_key(keys_dup[j])));
                test_assert(test_ent_equals(test_fret_entry(fret),
                                            test_iret_entry(iret)));
                test_assert(test_key_equals(test_fret_key(fret),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_fret_val(fret),
                                                test_pass_val(vals[j])));

                eret = test_erase(&tbl, test_pass_key(keys[j]));
                test_assert(test_eret_success(eret));

                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys[j])));
                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys_dup[j])));
                test_ec_map_assert(test_val_equals(test_eret_val(eret),
                                                   test_pass_val(vals[j])));

                test_assert(test_eret_fail(
                    test_erase(&tbl, test_pass_key(keys_dup[j]))));

                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(test_fret_fail(fret));
                test_assert(fret ==
                            test_find(&tbl, test_pass_key(keys_dup[j])));

                iret = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_success(iret));
                test_assert(test_key_equals(test_iret_key(iret),
                                            test_pass_key(keys[j])));
                test_map_do((*test_iret_valp(iret)) = test_pass_val(vals[j]));

                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(fret ==
                            test_find(&tbl, test_pass_key(keys_dup[j])));
                test_assert(test_ent_equals(test_fret_entry(fret),
                                            test_iret_entry(iret)));
                test_assert(test_key_equals(test_fret_key(fret),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_fret_val(fret),
                                                test_pass_val(vals[j])));

                iret = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(iret));
                test_assert(test_ent_equals(test_fret_entry(fret),
                                            test_iret_entry(iret)));

                iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_iret_fail(iret));
                test_assert(test_ent_equals(test_fret_entry(fret),
                                            test_iret_entry(iret)));
            }

            for (j = 0; j < i; ++j) {
                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(fret ==
                            test_find(&tbl, test_pass_key(keys_dup[j])));
                test_assert(test_fret_success(fret));
                test_assert(fret ==
                            test_find_exists(&tbl, test_pass_key(keys[j])));
                test_assert(test_key_equals(test_fret_key(fret),
                                            test_pass_key(keys[j])));
                test_map_assert(test_val_equals(test_fret_val(fret),
                                                test_pass_val(vals[j])));

                iret = test_insert(&tbl, test_pass_key(keys[j]));
                test_assert(test_iret_fail(iret));
                test_assert(test_key_equals(test_iret_key(iret),
                                            test_pass_key(keys[j])));

                iret = test_insert(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_iret_fail(iret));
                test_assert(test_key_equals(test_iret_key(iret),
                                            test_pass_key(keys_dup[j])));
                test_map_assert(test_val_equals(test_iret_val(iret),
                                                test_pass_val(vals[j])));

                test_assert(test_ent_equals(test_fret_entry(fret),
                                            test_iret_entry(iret)));

                eret = test_erase(&tbl, test_pass_key(keys_dup[j]));
                test_assert(test_eret_success(eret));

                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys[j])));
                test_ec_assert(test_key_equals(test_eret_key(eret),
                                               test_pass_key(keys_dup[j])));
                test_ec_map_assert(test_val_equals(test_eret_val(eret),
                                                   test_pass_val(vals[j])));

                test_assert(
                    test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));
                fret = test_find(&tbl, test_pass_key(keys[j]));
                test_assert(test_fret_fail(fret));

                test_assert(
                    test_eret_fail(test_erase(&tbl, test_pass_key(keys[j]))));
            }
        }

        test_deinit(&tbl);
    }
    test_namer(deinit_kvps)(keys, keys_dup, vals);
    return 0;
}


static NL int32_t
test_namer(test_ief_rand)(uint32_t keep_small) {

    uint32_t          i;
    test_key_base_t * keys, *keys_dup;
    test_val_base_t * vals;
    bool *            state          = (bool *)safe_zalloc(k_test_size);
    uint32_t *        rindexes       = make_true_rand32_buffer(k_test_size * 2);
    uint32_t *        rindexes_begin = rindexes;

    test_namer(init_kvps)(&keys, &keys_dup, &vals);
    vprint("Running: " V_TO_STR(test_namer(init_ief_rand)) "(%u) -> %u\n",
           keep_small, k_test_size);


    for (i = 1; i <= k_test_size; i += i) {

        uint32_t     j, r;
        test_table_t tbl;
        vvprint("\tTest Size: %u\n", i);
        test_init(&tbl);
        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {

                uint32_t idx  = *(rindexes) & (i - 1);
                uint32_t todo = (*(rindexes) >> 30);
                if (keep_small && todo == 0) {
                    todo = (*(rindexes) >> 28) & 3;
                }
                ++rindexes;
                if (rindexes == (rindexes_begin + (k_test_size * 2))) {
                    rindexes = rindexes_begin;
                }
                switch (todo) {
                    case 0:
                        if (r) {
                            goto rerase;
                        }
                        fall_through;
                    case 1: {
                        test_insert_ret_t iret =
                            test_insert(&tbl, test_pass_key(keys[idx]));
                        test_assert(state[idx] == (test_iret_fail(iret)),
                                    "(r=%d, idx=%u): %d\n", r, idx, state[idx]);
                        if (!state[idx]) {
                            state[idx] = 1;
                            test_map_do((*test_iret_valp(iret)) =
                                            test_pass_val(vals[idx]));
                        }
                        test_assert(test_key_equals(test_iret_key(iret),
                                                    test_pass_key(keys[idx])));
                        test_map_assert(test_val_equals(
                            test_iret_val(iret), test_pass_val(vals[idx])));
                    } break;
                    case 2: {
                        test_find_ret_t fret =
                            test_find(&tbl, test_pass_key(keys[idx]));
                        if (state[idx]) {
                            test_assert(test_fret_success(fret),
                                        "(r=%d, idx=%u)\n", r, idx);
                            test_assert(fret ==
                                        test_find_exists(
                                            &tbl, test_pass_key(keys[idx])));
                            test_assert(test_key_equals(
                                test_fret_key(fret), test_pass_key(keys[idx])));
                            test_map_assert(test_val_equals(
                                test_fret_val(fret), test_pass_val(vals[idx])));
                        }
                        else {
                            test_assert(test_fret_fail(fret));
                        }
                    } break;
                    case 3:
                    rerase:
                        if (state[idx]) {
                            test_erase_ret_t eret;
                            eret =
                                test_erase(&tbl, test_pass_key(keys_dup[idx]));
                            test_assert(test_eret_success(eret));

                            test_ec_assert(test_key_equals(
                                test_eret_key(eret), test_pass_key(keys[idx])));
                            test_ec_assert(
                                test_key_equals(test_eret_key(eret),
                                                test_pass_key(keys_dup[idx])));
                            test_ec_map_assert(test_val_equals(
                                test_eret_val(eret), test_pass_val(vals[idx])));
                        }
                        test_assert(test_eret_fail(
                            test_erase(&tbl, test_pass_key(keys[idx]))));
                        state[idx] = 0;
                        break;
                }
            }

            if (!r) {
                for (j = 0; j < i; ++j) {
                    test_find_ret_t fret =
                        test_find(&tbl, test_pass_key(keys[j]));
                    test_insert_ret_t iret =
                        test_insert(&tbl, test_pass_key(keys[j]));

                    if (state[j]) {
                        test_assert(test_fret_success(fret));
                        test_assert(fret == test_find_exists(
                                                &tbl, test_pass_key(keys[j])));
                        test_assert(test_key_equals(test_fret_key(fret),
                                                    test_pass_key(keys[j])));
                        test_map_assert(test_val_equals(
                            test_fret_val(fret), test_pass_val(vals[j])));
                        test_assert(test_iret_fail(iret));
                        test_assert(test_ent_equals(test_fret_entry(fret),
                                                    test_iret_entry(iret)));
                    }
                    else {
                        test_assert(test_fret_fail(fret));
                        test_map_do((*test_iret_valp(iret)) =
                                        test_pass_val(vals[j]));
                        test_assert(test_key_equals(test_iret_key(iret),
                                                    test_pass_key(keys[j])));
                        test_map_assert(test_val_equals(
                            test_iret_val(iret), test_pass_val(vals[j])));
                    }
                    state[j] = 1;
                    iret     = test_insert(&tbl, test_pass_key(keys[j]));
                    test_assert(test_iret_fail(iret));
                    fret = test_find(&tbl, test_pass_key(keys[j]));
                    test_assert(test_fret_success(fret));
                    test_assert(fret == test_find_exists(
                                            &tbl, test_pass_key(keys_dup[j])));
                }
            }
            else {
                for (j = 0; j < i; ++j) {
                    test_erase_ret_t eret;
                    test_find_ret_t  fret =
                        test_find(&tbl, test_pass_key(keys[j]));


                    if (state[j]) {
                        test_assert(test_fret_success(fret));
                        test_assert(fret == test_find_exists(
                                                &tbl, test_pass_key(keys[j])));
                        test_assert(test_key_equals(test_fret_key(fret),
                                                    test_pass_key(keys[j])));
                        test_map_assert(test_val_equals(
                            test_fret_val(fret), test_pass_val(vals[j])));

                        eret = test_erase(&tbl, test_pass_key(keys[j]));
                        test_assert(test_eret_success(eret));

                        test_ec_assert(test_key_equals(test_eret_key(eret),
                                                       test_pass_key(keys[j])));
                        test_ec_assert(test_key_equals(
                            test_eret_key(eret), test_pass_key(keys_dup[j])));
                        test_ec_map_assert(test_val_equals(
                            test_eret_val(eret), test_pass_val(vals[j])));
                    }
                    else {
                        test_assert(test_fret_fail(fret));
                        test_assert(test_eret_fail(
                            test_erase(&tbl, test_pass_key(keys[j]))));
                    }
                }
            }
        }
        memset_c(state, 0, i);
        test_deinit(&tbl);
    }

    test_namer(deinit_kvps)(keys, keys_dup, vals);
    safe_sfree(state, k_test_size);
    safe_sfree(rindexes_begin, k_test_size * 2 * sizeof(uint32_t));
    return 0;
}

static NL int32_t
test_namer(runall)(void) {
    test_assert(test_namer(test_i()) == 0);
    test_assert(test_namer(test_init()) == 0);
    test_assert(test_namer(test_ief_rand(0)) == 0);
    test_assert(test_namer(test_ief_rand(1)) == 0);

    test_assert(test_namer(test_ief()) == 0);


    test_assert(test_namer(test_if()) == 0);
    test_assert(test_namer(test_ie()) == 0);

    return 0;
}

#undef test_table_t
#undef test_init
#undef test_init_sz
#undef test_deinit
#undef test_capacity
#undef test_shrink
#undef test_clear

#undef test_insert
#undef test_find
#undef test_find_exists
#undef test_erase

#undef test_iret_success
#undef test_iret_fail
#undef test_iret_key
#undef test_iret_valp
#undef test_iret_val
#undef test_iret_entry

#undef test_eret_success
#undef test_eret_fail
#undef test_eret_key
#undef test_eret_val

#undef test_fret_success
#undef test_fret_fail
#undef test_fret_key
#undef test_fret_val
#undef test_fret_entry

#undef test_insert_ret_t
#undef test_find_ret_t
#undef test_erase_ret_t


#undef test_key_t
#undef test_key_base_t
#undef test_val_t
#undef test_val_base_t
#undef test_map_do
#undef test_map_assert
#undef test_name
#undef test_namer
#undef test_hash
#undef test_spare_bits
#undef test_key_equals
#undef test_val_equals
#undef test_ent_equals
#undef test_pass_key
#undef test_pass_val
#undef test_key_eq
#undef test_null_invalid
#undef test_skip_zero_key
#undef test_erase_copy
#undef test_ec_assert
#undef test_ec_map_assert
#undef HASHTABLE_H
