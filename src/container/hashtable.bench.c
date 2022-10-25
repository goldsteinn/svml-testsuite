#include "bench/bench-common.h"
#include "bench/bench-generator.h"

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
    k_trials             = 20,
    k_nbenches           = 32,
    k_test_size_max_log2 = 20,
    k_test_size_max      = 1 << k_test_size_max_log2
};

#define names b1, b2, b4, b8, b16


#define test_key_t b1_t
#define test_name  b1
#define test_hash  hash32
#include "hashtable.bench.h"

#define test_key_t b2_t
#define test_name  b2
#define test_hash  hash32
#include "hashtable.bench.h"

#define test_key_t b4_t
#define test_name  b4
#define test_hash  hash32
#include "hashtable.bench.h"

#define test_key_t b8_t
#define test_name  b8
#define test_hash  hash64
#include "hashtable.bench.h"

#define test_key_t   b16_t
#define test_name    b16
#define test_hash(x) hash128((x).bytes_)
#include "hashtable.bench.h"

#define run_bench(name)                                                        \
    (results = I__hl_namer(name, bench)(i, ht_type_size, results));

void *
bench_hashtable(void * bench_args) {
    (void)(bench_args);
    enum {
        k_nruns       = (k_test_size_max_log2 + PP_NARG(names)) * k_nbenches,
        k_alloc_size0 = k_nruns * sizeof(bench_result_t),
        k_alloc_size1 = k_nruns * k_trials * sizeof(uint64_t)
    };

    bench_result_t * results = (bench_result_t *)safe_zalloc(k_alloc_size0);
    uint64_t *       times   = (uint64_t *)safe_zalloc(k_alloc_size1);

    bench_result_t * results_start = results;
    bench_result_t * results_free  = results_start;
    uint64_t *       times_start   = times;


    uint32_t i;
    for (i = 0; i < k_nruns; ++i) {
        results[i].times = times;
        times += k_trials;
        die_assert(times <= (times_start + k_nruns * k_trials));
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

    char lastc = '\0';
    for (; results_start < results; ++results_start) {
        stats_result_t stats;
        size_t         len     = strlen(results_start->name);
        char           r_lastc = results_start->name[len ? (len - 1) : 0];
        if (r_lastc != lastc) {
            fprintf(stderr,
                    "------------------------------------------------\n");
            lastc = r_lastc;
        }

        for (i = 0; i < k_trials; ++i) {
            double t = results_start->times[i];
            t /= CAST(double, results_start->size);
            results_start->times_dbl[i] = t;
        }
        gen_stats(&stats, results_start->times_dbl, k_trials);
        fprintf(stderr, "%-20s(%u): %.3lf\n", results_start->name,
                results_start->size, stats.geomean);
    }

    safe_sfree(results_free, k_alloc_size0);
    safe_sfree(times_start, k_alloc_size1);

    return 0;
}
