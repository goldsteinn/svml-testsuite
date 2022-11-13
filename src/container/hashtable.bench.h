#include "arch/ll-prefetch.h"

#define hl_key_t test_key_t
#define hl_name  test_name
#define hl_hash  test_hash
#ifdef test_spare_key_bits
# define hl_spare_key_bits test_spare_key_bits
#endif


#include HASHTABLE_H

#define namer(name) I_hl_namer(test_name, name)

const_assert(sizeof(test_key_t) == sizeof(namer(kvp_t)));


static void
namer(init_key)(test_key_t * k, uint32_t i) {
    if (sizeof(uint32_t) >= sizeof(test_key_t)) {
        __builtin_memcpy(k, &i, sizeof(test_key_t));
    }
    else {
        uint32_t j;
        for (j = 0; j < sizeof(test_key_t); j += sizeof(uint32_t)) {
            __builtin_memcpy(((uint8_t *)k) + j, &i, sizeof(uint32_t));
        }
        __builtin_memcpy(((uint8_t *)k) + sizeof(test_key_t) - sizeof(uint32_t),
                         &i, sizeof(uint32_t));
    }
#ifdef test_spare_key_bits
    if ((sizeof_bits(test_key_t) - test_spare_key_bits) < 32) {
        uint64_t i64   = (uint64_t)i;
        uint64_t max64 = 1UL << (sizeof_bits(test_key_t) - test_spare_key_bits);
        if (i64 > max64) {
            *k &= (test_key_t)(max64 - 1);
        }
    }
#endif
}

static void
namer(init_keys)(test_key_t * keys, uint32_t test_size) {
    uint32_t     i;
    uint8_t *    inverted_keys = (uint8_t *)(keys + test_size);
    uint8_t *    in_keys       = (uint8_t *)keys;
    test_key_t * ikeys         = keys + test_size * 1;
    test_key_t * mixed_keys    = keys + test_size * 2;
    uint32_t *   rands         = make_true_rand32_buffer(test_size);
    for (i = 0; i < test_size; ++i) {
        namer(init_key)(keys + i, i);
    }

    for (i = 0; i < test_size; ++i) {
        uint32_t   idx_from = (rands[i] >> 4) % test_size;
        test_key_t to       = keys[i];
        keys[i]             = keys[idx_from];
        keys[idx_from]      = to;
    }

    for (i = 0; i < test_size * sizeof(test_key_t); ++i) {
        inverted_keys[i] = ~in_keys[i];
    }

#ifdef test_spare_key_bits
    {
        uint64_t max64 = 1UL << (sizeof_bits(test_key_t) - test_spare_key_bits);
        max64 -= 1;
        for (i = 0; i < test_size; ++i) {
            ikeys[i] &= (test_key_t)(max64);
        }
    }
#endif
    for (i = 0; i < test_size; ++i) {
        mixed_keys[i] = (rands[i] & 1) ? ikeys[i] : keys[i];
    }
    safe_sfree(rands, test_size * sizeof(uint32_t));
}

static void
namer(prefetch_keys)(test_key_t * kmem, uint32_t test_size) {
    uint8_t * mem8  = (uint8_t *)kmem;
    uint32_t  bytes = test_size * sizeof(test_key_t);
    uint32_t  i;
    for (i = 0; i < bytes; i += 64) {
        ll_prefetch_L1D(mem8 + i);
    }

    for (i = 0; i < 16384; i += 64) {
        ll_prefetch_L1D(mem8 + i);
    }
}

static bench_result_t *
namer(bench)(uint32_t         test_size,
             uint32_t         type_size,
             uint32_t         trials,
             bench_result_t * results) {
    uint32_t    i, t, sink;
    test_key_t *keys, *ikeys, *mkeys;
    ll_time_t   start, end;
    uint64_t *  cur_times;
    namer(table_t) tbl;
    if (type_size && sizeof(test_key_t) != type_size) {
        return results;
    }
    keys = (test_key_t *)safe_mmap_alloc(test_size * 3 * sizeof(test_key_t));
    namer(init_keys)(keys, test_size);
    ikeys = keys + test_size;
    mkeys = keys + test_size * 2;
    PRINTFFL;


    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(insert_and_grow, _, test_name));
    results->size = test_size;
    ++results;
    PRINTFFL;
    namer(init_sz)(&tbl, 1);
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(insert)(&tbl, keys[i]));
        }
        end = get_ll_time();
        namer(shrink)(&tbl, 1);

        *cur_times = get_ll_dif(end, start);
        PRINTFFL;
        ++cur_times;
    }
    namer(deinit)(&tbl);

    namer(prefetch_keys)(keys, test_size);

    cur_times     = results->times;
    results->name = V_TO_STR(CAT(insert, _, test_name));
    results->size = test_size;
    ++results;
    PRINTFFL;
    namer(init_sz)(&tbl, test_size * 2);
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();

        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(insert)(&tbl, keys[i]));
        }

        end = get_ll_time();
        PRINTFFL;
        namer(clear)(&tbl);
        PRINTFFL;
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
        PRINTFFL;
    }
    PRINTFFL;


    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(insert_full, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(insert)(&tbl, keys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }


    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_hit_full, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find)(&tbl, keys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_hit_used, _, test_name));
    results->size = test_size;
    ++results;
    sink = 0;
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            if (namer(fret_success)(namer(find)(&tbl, keys[i]))) {
                compiler_do_not_optimize_out(++sink);
            }
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_must_hit, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find_exists)(&tbl, keys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }


    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_miss_full, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(ikeys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find)(&tbl, ikeys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_miss_used, _, test_name));
    results->size = test_size;
    ++results;
    sink = 0;
    namer(prefetch_keys)(ikeys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            if (namer(fret_success)(namer(find)(&tbl, ikeys[i]))) {
                compiler_do_not_optimize_out(++sink);
            }
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_mixed_full, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(mkeys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find)(&tbl, mkeys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_mixed_used, _, test_name));
    results->size = test_size;
    ++results;
    sink = 0;
    namer(prefetch_keys)(mkeys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            namer(find_ret_t) fret = namer(find)(&tbl, mkeys[i]);
            if (namer(fret_success)(fret)) {
                compiler_do_not_optimize_out(++sink);
            }
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }

    namer(deinit)(&tbl);


    PRINTFFL;
    namer(init_sz)(&tbl, 1);


    cur_times     = results->times;
    results->name = V_TO_STR(CAT(find_empty, _, test_name));
    results->size = test_size;
    ++results;
    namer(prefetch_keys)(keys, test_size);
    for (t = trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find)(&tbl, keys[i]));
        }
        end        = get_ll_time();
        *cur_times = get_ll_dif(end, start);
        ++cur_times;
    }
    PRINTFFL;
    namer(deinit)(&tbl);
    PRINTFFL;

    safe_munmap(keys, test_size * 3 * sizeof(test_key_t));
    PRINTFFL;
    return results;
}

#undef test_key_t
#undef test_name
#undef test_hash
#undef test_spare_key_bits
#undef HASHTABLE_H
