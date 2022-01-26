#ifndef SRC_D_BENCH_D_BENCH_GENERATOR_H_
#define SRC_D_BENCH_D_BENCH_GENERATOR_H_

#include "bench/bench-common.h"

#define custom_make_bench(name, get_start_time, get_end_time, time_dif,        \
                          serializer, ntrials, call, ...)                      \
    static uint64_t name(__VA_ARGS__) {                                        \
        get_start_time;                                                        \
        compiler_barrier();                                                    \
        compiler_code_align(64);                                               \
        for (get_type(ntrials) I_tmp_ntrials_iters = ntrials;                  \
             I_tmp_ntrials_iters; --I_tmp_ntrials_iters) {                     \
            compiler_do_not_optimize_out(call);                                \
            serializer;                                                        \
        }                                                                      \
        compiler_barrier();                                                    \
        get_end_time;                                                          \
        return time_dif;                                                       \
    }

#define ll_make_bench(name, serializer, ntrials, call, ...)                    \
    custom_make_bench(name, ll_time_t I_tmp_start; ll_time_t I_tmp_end;        \
                      I_tmp_start = get_ll_time(), I_tmp_end = get_ll_time(),  \
                      get_ll_dif(I_tmp_end, I_tmp_start), serializer, ntrials, \
                      call, __VA_ARGS__)

#define ll_make_tput_bench(name, ntrials, call, ...)                           \
    ll_make_bench(name, ;, ntrials, call, __VA_ARGS__)
#define ll_make_lat_bench(name, ntrials, call, ...)                            \
    ll_make_bench(name, serialize_ooe(), ntrials, call, __VA_ARGS__)
#define ll_make_all_bench(name, ...)                                           \
    ll_make_tput_bench(CAT(name, _tput), __VA_ARGS__);                         \
    ll_make_lat_bench(CAT(name, _lat), __VA_ARGS__)

#endif
