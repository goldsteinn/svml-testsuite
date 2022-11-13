#include "bench/bench-common.h"
#include "bench/bench-generator.h"

#include "lib/hash/hash.h"
#include "lib/stats/stats.h"
#include "util/inline-math.h"
#include "util/macro.h"
#include "util/random.h"

extern uint32_t ht_type_size;
extern uint32_t ht_test_size;

typedef struct bench_result {
    char const * name;
    union {
        uint64_t * times;
        double *   times_dbl;
    };
    uint32_t size;
} bench_result_t;

typedef uint8_t  b1_t;
typedef uint16_t b2_t;
typedef uint32_t b4_t;
typedef uint64_t b8_t;
typedef struct b16 {
    uint8_t bytes_[16];
} b16_t;


enum {
    k_trials             = 1000,
    k_nbenches           = 32,
    k_test_size_max_log2 = 20,
    k_test_size_max      = 1 << k_test_size_max_log2
};

#define names b1, b2, b4, b8, b16, b1i, b2i, b4i, b8i


#define test_key_t  b1_t
#define test_name   b1
#define test_hash   xxhashT
#define HASHTABLE_H "rh-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t  b2_t
#define test_name   b2
#define test_hash   xxhashT
#define HASHTABLE_H "rh-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t  b4_t
#define test_name   b4
#define test_hash   xxhashT
#define HASHTABLE_H "rh-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t  b8_t
#define test_name   b8
#define test_hash   xxhashT
#define HASHTABLE_H "rh-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t          b1_t
#define test_name           b1i
#define test_hash           xxhashT
#define test_spare_key_bits 5
#define HASHTABLE_H         "rhi-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t          b2_t
#define test_name           b2i
#define test_hash           xxhashT
#define test_spare_key_bits 5
#define HASHTABLE_H         "rhi-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t          b4_t
#define test_name           b4i
#define test_hash           xxhashT
#define test_spare_key_bits 5
#define HASHTABLE_H         "rhi-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t          b8_t
#define test_name           b8i
#define test_hash           xxhashT
#define test_spare_key_bits 5
#define HASHTABLE_H         "rhi-hashtable.h"
#include "hashtable.bench.h"

#define test_key_t   b16_t
#define test_name    b16
#define test_hash(x) xxhashT(x)
#define HASHTABLE_H  "rh-hashtable.h"
#include "hashtable.bench.h"

#define run_bench(name)                                                        \
    (results = I_hl_namer(name, bench)(i, ht_type_size, trials, results));


void * bench_hashtable(void * bench_args);
void *
bench_hashtable(void * bench_args) {
    uint32_t trials = (uint32_t)(uint64_t)bench_args;
    enum {
        k_nruns       = (k_test_size_max_log2 + PP_NARG(names)) * k_nbenches,
        k_ntrial_runs = CAST(size_t, k_nruns * k_trials),
        k_alloc_size0 = k_nruns * sizeof(bench_result_t),
        k_alloc_size1 = k_ntrial_runs * sizeof(uint64_t)
    };


    bench_result_t * results;
    uint64_t *       times;

    bench_result_t * results_start;
    bench_result_t * results_free;
    uint64_t *       times_start;


    uint32_t i;

    if (trials == 0) {
        trials = k_trials;
    }
    die_assert(trials <= k_trials);
    results = (bench_result_t *)safe_zalloc(k_alloc_size0);
    times   = (uint64_t *)safe_zalloc(k_alloc_size1);

    results_start = results;
    results_free  = results_start;
    times_start   = times;

    for (i = 0; i < k_nruns; ++i) {
        results[i].times = times;
        times += k_trials;
        die_assert(times <= (times_start + k_ntrial_runs));
    }

    if (ht_test_size) {
        i = ht_test_size;
        APPLY(run_bench, ;, names);
    }
    else {
        for (i = 32; i < k_test_size_max; i += i) {
            APPLY(run_bench, ;, names);
        }
    }
    {
        char lastc = '\0';
        for (; results_start < results; ++results_start) {
            stats_result_t stats;
            size_t         len     = strlen(results_start->name);
            char           r_lastc = results_start->name[len ? (len - 1) : 0];
            if (r_lastc != lastc) {
                print("------------------------------------------------\n");
                lastc = r_lastc;
            }

            for (i = 0; i < trials; ++i) {
                double t = CAST(double, results_start->times[i]);
                t /= CAST(double, results_start->size);
                results_start->times_dbl[i] = t;
            }
            gen_stats(&stats, results_start->times_dbl, trials);
            print("%-20s(%u): %.3lf\n", results_start->name,
                  results_start->size, stats.geomean);
        }
    }
    safe_sfree(results_free, k_alloc_size0);
    safe_sfree(times_start, k_alloc_size1);

    return 0;
}
