#define hl_key_t test_key_t
#define hl_name  test_name
#define hl_hash  test_hash

#include "rh-hashtable.h"

#define namer(name) I__hl_namer(test_name, name)

const_assert(sizeof(test_key_t) == sizeof(namer(kvp_t)));

bench_result_t *
namer(bench)(uint32_t test_size, uint32_t type_size, bench_result_t * results) {
    if(type_size && sizeof(test_key_t) != type_size) {
        return results;
    }
    uint32_t     i, t;
    test_key_t * keys =
        (test_key_t *)safe_mmap_alloc(test_size * sizeof(test_key_t));
    ll_time_t  start, end;
    uint64_t * cur_times;
    PRINTFFL;
    namer(table_t) tbl;
    randomize_buffer((uint8_t *)keys, test_size * sizeof(test_key_t));
    PRINTFFL;
    cur_times     = results->times;
    results->name = V_TO_STR(CAT(insert_and_grow, _, test_name));
    results->size = test_size;
    ++results;
    PRINTFFL;
    namer(init_sz)(&tbl, 1);
    for (t = k_trials; t; --t) {
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

    cur_times     = results->times;
    results->name = V_TO_STR(CAT(insert, _, test_name));
    results->size = test_size;
    ++results;
    PRINTFFL;
    namer(init_sz)(&tbl, test_size * 2);
    for (t = k_trials; t; --t) {
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
    for (t = k_trials; t; --t) {
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
    results->name = V_TO_STR(CAT(find_full, _, test_name));
    results->size = test_size;
    ++results;
    for (t = k_trials; t; --t) {
        start = get_ll_time();
        for (i = 0; i < test_size; ++i) {
            compiler_do_not_optimize_out(namer(find)(&tbl, keys[i]));
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
    for (t = k_trials; t; --t) {
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

    safe_munmap(keys, test_size * sizeof(test_key_t));
    PRINTFFL;
    return results;
}

#undef test_key_t
#undef test_name
#undef test_hash
