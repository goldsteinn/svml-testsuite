#include "bench/bench-common.h"
#include "bench/bench-generator.h"

#include "thread/rseq/rseq.h"
#include "util/sched-util.h"

#include "util/macro.h"


ll_make_all_bench(bench_sched_getcpu, trials, sched_getcpu(), uint32_t trials);
ll_make_all_bench(bench_rseq_getcpu, trials, rseq_getcpu(), uint32_t trials);
ll_make_all_bench(bench_ll_getcpu, trials, ll_getcpu(), uint32_t trials);

#define run_bench(res_name, res_time, func)                                    \
    (res_name) = V_TO_STR(func);                                               \
    (res_time) = CAT(bench_, func)(trials);


void *
bench_get_cpu(void * bench_args) {
    uint32_t     trials = CAST(uint32_t, CAST(uint64_t, bench_args));
    uint32_t     i;
    uint64_t     times[6] = { 0 };
    char const * names[6] = { 0 };


    enum { DEFAULT_TRIALS = 50 * 1000 * 1000 };
    if (!trials) {
        trials = DEFAULT_TRIALS;
    }

    die_assert(rseq_init() == 0);

    run_bench(names[0], times[0], sched_getcpu_lat);
    run_bench(names[1], times[1], rseq_getcpu_lat);
    run_bench(names[2], times[2], ll_getcpu_lat);

    run_bench(names[3], times[3], sched_getcpu_tput);
    run_bench(names[4], times[4], rseq_getcpu_tput);
    run_bench(names[5], times[5], ll_getcpu_tput);


    for (i = 0; i < 6; ++i) {
        print_res(names[i], times[i], trials, get_ll_units());
    }

    return 0;
}
