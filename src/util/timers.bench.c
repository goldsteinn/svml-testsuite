#include "bench/bench-common.h"

#include "util/error-util.h"
#include "util/time-util.h"

#define run_bench(res_name, res_time, func, ...)                               \
    start = get_ll_time();                                                     \
    for (i = trials; i; --i) {                                                 \
        compiler_do_not_optimize_out(func(__VA_ARGS__));                       \
    }                                                                          \
    end      = get_ll_time();                                                  \
    res_name = V_TO_STR(func);                                                 \
    res_time = get_ll_dif(end, start);


void *
bench_timers(void * bench_args) {
    uint32_t  trials = CAST(uint32_t, CAST(uint64_t, bench_args));
    ll_time_t start, end;
    uint32_t  i;

    enum { NUM_TIMERS = 8, DEFAULT_TRIALS = 1000 * 1000 };

    uint64_t     times[NUM_TIMERS] = { 0 };
    const char * names[NUM_TIMERS] = { 0 };

    if (!trials) {
        trials = DEFAULT_TRIALS;
    }

    die_assert(with_vdso() && vdso_is_full_init());

    run_bench(names[0], times[0], vdso_get_ns);
    run_bench(names[1], times[1], get_ns);
    run_bench(names[2], times[2], direct_get_ns);
    run_bench(names[3], times[3], get_ll_time);
    run_bench(names[4], times[4], gtod_ns);
    run_bench(names[5], times[5], vdso_gtod_ns);
    run_bench(names[6], times[6], get_thread_ns);
    run_bench(names[7], times[7], get_proc_ns);    

    for (i = 0; i < NUM_TIMERS; ++i) {
        print_res(names[i], times[i], trials, get_ll_units());
    }

    return 0;
}
