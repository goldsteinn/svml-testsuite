#include "bench/bench-common.h"

#include "util/inline-math.h"
#include "util/macro.h"

#define run_bench(res_name, res_time, func)                                    \
    start = get_ll_time();                                                     \
    for (i = trials; i; --i) {                                                 \
        compiler_do_not_optimize_out(func(i));                                 \
    }                                                                          \
    end      = get_ll_time();                                                  \
    res_name = V_TO_STR(func);                                                 \
    res_time = get_ll_dif(end, start);


int32_t
bench_p2(uint32_t trials) {
    ll_time_t start, end;
    uint32_t  i;

    uint64_t     times[4] = { 0 };
    const char * names[4] = { 0 };

    if (!trials) {
        trials = 1000 * 1000;
    }

    run_bench(names[0], times[0], next_p2);
    run_bench(names[1], times[1], prev_p2);

    print_res(names[0], times[0], trials, get_ll_units());
    print_res(names[1], times[1], trials, get_ll_units());

    return 0;
}
