#include "bench/bench-common.h"

#include "util/time-util.h"

#define run_bench(res_name, res_time, func, ...)                               \
    start = get_ll_time();                                                     \
    for (i = trials; i; --i) {                                                 \
        compiler_do_not_optimize_out(func(__VA_ARGS__));                       \
    }                                                                          \
    end      = get_ll_time();                                                  \
    res_name = V_TO_STR(func);                                                 \
    res_time = get_ll_dif(end, start);

static ALWAYS_INLINE uint64_t
vdso_direct_get_ns(const FUNC_T(clock_gettime) vdso_direct) {
    struct timespec ts;
    vdso_direct(CLOCK_THREAD_CPUTIME_ID, &ts);
    return to_ns(&ts);
}

int32_t
bench_timers(uint32_t trials) {
    ll_time_t start, end;
    uint32_t  i;

    uint64_t     times[6]             = { 0 };
    const char * names[6]             = { 0 };
    FUNC_T(clock_gettime) vdso_direct = get_vdso_clock_gettime();

    if (!trials) {
        trials = 1000 * 1000;
    }

    run_bench(names[0], times[0], vdso_get_ns);
    safe_vdso_init();
    names[0] = "vdso_get_ns_preinit";
    run_bench(names[1], times[1], vdso_get_ns);
    run_bench(names[2], times[2], get_ns);
    run_bench(names[3], times[3], _get_ns);
    run_bench(names[4], times[4], get_ll_time);
    run_bench(names[5], times[5], vdso_direct_get_ns, vdso_direct);

    for (i = 0; i < 6; ++i) {
        print_res(names[i], times[i], trials, get_ll_units());
    }

    return 0;
}
