#ifndef SRC_D_SVML_BENCHMARKER_H_
#define SRC_D_SVML_BENCHMARKER_H_

#include "svml-defs.h"

enum { k_bench_tput = 1, k_bench_lat = 2 };

typedef union bench_val_t {
    uint64_t u64_;
    float    f_[2];
    double   d_;
} bench_val_t;

typedef struct bench_conf {
    uint16_t    type_;
    uint16_t    cpu_;
    int32_t     trials_;
    bench_val_t init_vals_[3];
} bench_conf_t;

void run_svml_benches(svml_op_t const *    svml_ops,
                      uint32_t             num_svml_ops,
                      bench_conf_t const * bench_conf);


#endif
