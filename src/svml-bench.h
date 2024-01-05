#include <immintrin.h>

#include "svml-func-defs.h"
#include "util/compiler.h"
#include "util/error-util.h"
#include "util/macro.h"
#include "util/time-util.h"

#define type_size_4  float
#define type_size_8  double
#define type_size_16 __m128i
#define type_size_32 __m256i
#define type_size_64 __m512i

#define init_size_4(x)                                                         \
    ({                                                                         \
        float I_tmp_v_;                                                        \
        __builtin_memcpy(&I_tmp_v_, &(x), sizeof(float));                      \
        I_tmp_v_;                                                              \
    })
#define init_size_8(x)                                                         \
    ({                                                                         \
        double I_tmp_v_;                                                       \
        __builtin_memcpy(&I_tmp_v_, &(x), sizeof(double));                     \
        I_tmp_v_;                                                              \
    })
#define init_size_16(x) _mm_set1_epi64x((int64_t)(x))
#define init_size_32(x) _mm256_set1_epi64x((int64_t)(x))
#define init_size_64(x) _mm512_set1_epi64((int64_t)(x))

#define bTYPE(sz) CAT(type_size_, sz)
#define bINIT(sz) CAT(init_size_, sz)

#define I_make_unused(v)       (void)(v);
#define bench_make_unused(...) APPLY(I_make_unused, ;, __VA_ARGS__)

#define bench_func_name(svml_func, ext) CAT(bench_, svml_func, _, ext)

#define make_bench_func(svml_func, sz, init, run, ext)                         \
    static double bench_func_name(svml_func, ext)(                             \
        uint32_t trials, uint64_t init0, uint64_t init1, uint64_t init2) {     \
        uint64_t start, end, dif = 0, total = 0;                               \
        bTYPE(sz) inp0, inp1, inp2;                                            \
        init(sz, inp0, inp1, inp2, init0, init1, init2);                       \
        do {                                                                   \
            uint32_t i;                                                        \
            start = get_ll_time();                                             \
            compiler_code_align(64);                                           \
            for (i = trials; i; --i) {                                         \
                run(svml_func, inp0, inp1, inp2);                              \
            }                                                                  \
            end = get_ll_time();                                               \
            dif += (end - start);                                              \
            total += trials;                                                   \
        } while (UNLIKELY(dif < (1000UL * 1000UL * 1000UL)));                  \
        return ((double)dif) / ((double)total);                                    \
    }


#define I_make_bench_empty(svml_func, ext)                                     \
    static double bench_func_name(svml_func, ext)(                             \
        uint32_t trials, uint64_t init0, uint64_t init1, uint64_t init2) {     \
        die("Unimplemented");                                                  \
        bench_make_unused(trials, init0, init1, init2);                        \
    }

#define make_bench_empty(svml_func, ...)                                       \
    I_make_bench_empty(svml_func, tput);                                       \
    I_make_bench_empty(svml_func, lat);

#define bench_init_T_T(sz, out0, out1, out2, in0, in1, in2)                    \
    (out0) = bINIT(sz)(in0);                                                   \
    bench_make_unused(out1, out2, in1, in2);
#define bench_run_T_T_tput(svml_func, inp0, ...)                               \
    compiler_do_not_optimize_out(svml_func(inp0))
#define bench_run_T_T_lat(svml_func, inp0, ...)                                \
    fence_do_not_optimize_out(svml_func(inp0))

#define make_bench_f_f(svml_func, sz)                                          \
    make_bench_func(svml_func, sz, bench_init_T_T, bench_run_T_T_tput, tput);  \
    make_bench_func(svml_func, sz, bench_init_T_T, bench_run_T_T_lat, lat)

#define make_bench_f_f_f(svml_func, sz)       make_bench_empty(svml_func)
#define make_bench_i_f(svml_func, sz)         make_bench_empty(svml_func)
#define make_bench_lli_f(svml_func, sz)       make_bench_empty(svml_func)
#define make_bench_v_f_fp_fp(svml_func, sz)   make_bench_empty(svml_func)
#define make_bench_v_f_vfp_vfp(svml_func, sz) make_bench_empty(svml_func)

#define make_bench_d_d(...)         make_bench_f_f(__VA_ARGS__)
#define make_bench_d_d_d(...)       make_bench_f_f_f(__VA_ARGS__)
#define make_bench_i_d(...)         make_bench_i_f(__VA_ARGS__)
#define make_bench_lli_d(...)       make_bench_lli_f(__VA_ARGS__)
#define make_bench_v_d_dp_dp(...)   make_bench_v_f_fp_fp(__VA_ARGS__)
#define make_bench_v_d_vdp_vdp(...) make_bench_v_f_vfp_vfp(__VA_ARGS__)


#define ADD_ONE_DEF(svml_func, unused_ref_func, sz, unused_cnt, unused_is_fp,  \
                    unused_ulp, test_type)                                     \
    CAT(make_bench_, test_type)(svml_func, sz)

#define SEP ;
#define ADD_DEF(svml_func, ...)                                                \
    ADD_ONE_DEF(CAT(svml_func, _dev), __VA_ARGS__);                            \
    ADD_ONE_DEF(CAT(svml_func, _glibc), __VA_ARGS__)

#include "svml-raw-defs.h"

#define get_bench_funcs(svml_func)                                             \
    { bench_func_name(svml_func, tput) }, {                                    \
        bench_func_name(svml_func, lat)                                        \
    }

#undef ADD_ONE_DEF
#undef ADD_DEF
#undef SEP
