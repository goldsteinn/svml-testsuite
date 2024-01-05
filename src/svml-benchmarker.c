#include "svml-benchmarker.h"
#include "util/memory-util.h"
#include "util/sched-util.h"
typedef struct bench_result {
    double   time_;
    uint32_t idx_;
    uint32_t type_;
} bench_result_t;
enum { k_default_trials = (200) * (1000 * 1000) };

void
run_svml_benches(svml_op_t const *    svml_ops,
                 uint32_t             num_svml_ops,
                 bench_conf_t const * bench_conf) {
    bench_result_t * results;
    uint32_t         i, trials, todo, num_results, offset;

    trials      = bench_conf->trials_ > 0 ? (uint32_t)bench_conf->trials_
                                          : k_default_trials;
    todo        = bench_conf->type_ ? bench_conf->type_ : k_bench_tput;
    num_results = num_svml_ops;
    proc_setcpu_and_wait(bench_conf->cpu_);


    if (todo == (k_bench_tput | k_bench_lat)) {
        num_results *= 2;
    }
    results =
        (bench_result_t *)safe_zalloc(num_results * sizeof(bench_result_t));

    i = 0;
    if (todo & k_bench_tput) {
        for (; i < num_svml_ops; ++i) {
            vprint("Running: %s (tput)\n", svml_ops[i].name_);
            results[i].time_ = svml_ops[i].bench_tput_.run_bench(
                trials, bench_conf->init_vals_[0].u64_,
                bench_conf->init_vals_[1].u64_, bench_conf->init_vals_[2].u64_);
            results[i].idx_  = i;
            results[i].type_ = k_bench_tput;
        }
    }

    if (todo & k_bench_lat) {
        offset = i;
        for (i = 0; i < num_svml_ops; ++i) {
            vprint("Running: %s (lat)\n", svml_ops[i].name_);
            results[i + offset].time_ = svml_ops[i].bench_lat_.run_bench(
                trials, bench_conf->init_vals_[0].u64_,
                bench_conf->init_vals_[1].u64_, bench_conf->init_vals_[2].u64_);
            results[i].idx_  = i;
            results[i].type_ = k_bench_lat;
        }
    }

    for (i = 0; i < num_results; ++i) {
        double time = (double)results[i].time_;
        print("%-32s (%-4s) -> %.3lf\n", svml_ops[results[i].idx_].name_,
              results[i].type_ == k_bench_lat ? "lat" : "tput", time);
    }

    safe_sfree(results, num_results * sizeof(bench_result_t));
}
