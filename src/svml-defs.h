#ifndef SRC_D_SVML_DEFS_H_
#define SRC_D_SVML_DEFS_H_
#include <immintrin.h>

#include "math-defs.h"
#include "util/macro.h"

typedef __m512i (*svml_d_d_64__f)(__m512i);
typedef void (*svml_v_d_dp_dp_16__f)(__m128i, __m128i *, __m128i *);
typedef __m128i (*svml_f_f_16__f)(__m128i);
typedef void (*svml_v_f_fp_fp_32__f)(__m256i, __m256i *, __m256i *);
typedef __m512i (*svml_d_d_d_64__f)(__m512i, __m512i);
typedef __m128i (*svml_f_f_f_16__f)(__m128i, __m128i);
typedef __m128i (*svml_d_d_d_16__f)(__m128i, __m128i);
typedef long long int (*svml_lli_f_4__f)(float);
typedef float (*svml_f_f_4__f)(float);
typedef __m256i (*svml_f_f_f_32__f)(__m256i, __m256i);
typedef __m256i (*svml_f_f_32__f)(__m256i);
typedef int (*svml_i_f_4__f)(float);
typedef __m256i (*svml_d_d_d_32__f)(__m256i, __m256i);
typedef __m512i (*svml_f_f_64__f)(__m512i);
typedef int (*svml_i_d_8__f)(double);
typedef void (*svml_v_f_fp_fp_16__f)(__m128i, __m128i *, __m128i *);
typedef void (*svml_v_f_fp_fp_64__f)(__m512i, __m512i *, __m512i *);
typedef __m256i (*svml_d_d_32__f)(__m256i);
typedef void (*svml_v_d_dp_dp_64__f)(__m512i, __m512i *, __m512i *);
typedef float (*svml_f_f_f_4__f)(float, float);
typedef double (*svml_d_d_8__f)(double);
typedef long long int (*svml_lli_d_8__f)(double);
typedef __m512i (*svml_f_f_f_64__f)(__m512i, __m512i);
typedef double (*svml_d_d_d_8__f)(double, double);
typedef __m128i (*svml_d_d_16__f)(__m128i);
typedef void (*svml_v_d_dp_dp_32__f)(__m256i, __m256i *, __m256i *);

typedef double (*ref_d_d__f)(double);
typedef float (*ref_f_f__f)(float);
typedef double (*ref_d_d_d__f)(double, double);
typedef float (*ref_f_f_f__f)(float, float);
typedef double (*ref_d_d_d_d__f)(double, double, double);
typedef float (*ref_f_f_f_f__f)(float, float, float);
typedef double (*ref_d_ip__f)(double, int *);
typedef float (*ref_f_ip__f)(float, int *);
typedef int (*ref_i_d__f)(double);
typedef int (*ref_i_f__f)(float);
typedef double (*ref_d_i_d__f)(int, double);
typedef float (*ref_f_i_f__f)(int, float);
typedef double (*ref_d_d_i__f)(double, int);
typedef float (*ref_f_f_i__f)(float, int);
typedef long long int (*ref_lli_d__f)(double);
typedef long long int (*ref_lli_f__f)(float);
typedef long int (*ref_li_d__f)(double);
typedef long int (*ref_li_f__f)(float);
typedef double (*ref_d_d_dp__f)(double, double *);
typedef float (*ref_f_f_fp__f)(float, float *);
typedef double (*ref_d_kcp__f)(const char *);
typedef float (*ref_f_kcp__f)(const char *);
typedef double (*ref_d_d_ld__f)(double, long double);
typedef float (*ref_f_f_ld__f)(float, long double);
typedef double (*ref_d_d_d_ip__f)(double, double, int *);
typedef float (*ref_f_f_f_ip__f)(float, float, int *);
typedef float (*ref_f_f_d__f)(float, double);
typedef double (*ref_d_d_li__f)(double, long int);
typedef float (*ref_f_f_li__f)(float, long int);
typedef void (*ref_v_d_dp_dp__f)(double, double *, double *);
typedef void (*ref_v_f_fp_fp__f)(float, float *, float *);
typedef double (*ref_d_cd__f)(cdouble_t);
typedef float (*ref_f_cf__f)(cfloat_t);
typedef cdouble_t (*ref_cd_cd__f)(cdouble_t);
typedef cfloat_t (*ref_cf_cf__f)(cfloat_t);
typedef cdouble_t (*ref_cd_cd_cd__f)(cdouble_t, cdouble_t);
typedef cfloat_t (*ref_cf_cf_cf__f)(cfloat_t, cfloat_t);


typedef union svml_func {
    void *               p;
    svml_f_f_16__f       run_f_f_16;
    svml_v_d_dp_dp_32__f run_v_d_dp_dp_32;
    svml_f_f_f_16__f     run_f_f_f_16;
    svml_d_d_d_32__f     run_d_d_d_32;
    svml_v_f_fp_fp_64__f run_v_f_fp_fp_64;
    svml_d_d_d_8__f      run_d_d_d_8;
    svml_d_d_d_64__f     run_d_d_d_64;
    svml_f_f_f_4__f      run_f_f_f_4;
    svml_f_f_f_32__f     run_f_f_f_32;
    svml_v_d_dp_dp_64__f run_v_d_dp_dp_64;
    svml_v_f_fp_fp_32__f run_v_f_fp_fp_32;
    svml_lli_d_8__f      run_lli_d_8;
    svml_d_d_16__f       run_d_d_16;
    svml_d_d_8__f        run_d_d_8;
    svml_f_f_64__f       run_f_f_64;
    svml_d_d_d_16__f     run_d_d_d_16;
    svml_f_f_32__f       run_f_f_32;
    svml_d_d_32__f       run_d_d_32;
    svml_i_d_8__f        run_i_d_8;
    svml_d_d_64__f       run_d_d_64;
    svml_i_f_4__f        run_i_f_4;
    svml_v_f_fp_fp_16__f run_v_f_fp_fp_16;
    svml_v_d_dp_dp_16__f run_v_d_dp_dp_16;
    svml_f_f_4__f        run_f_f_4;
    svml_f_f_f_64__f     run_f_f_f_64;
    svml_lli_f_4__f      run_lli_f_4;
} svml_func_t;

typedef union ref_func {
    void *           p;
    ref_d_d__f       run_ref_d_d;
    ref_f_f__f       run_ref_f_f;
    ref_d_d_d__f     run_ref_d_d_d;
    ref_f_f_f__f     run_ref_f_f_f;
    ref_d_d_d_d__f   run_ref_d_d_d_d;
    ref_f_f_f_f__f   run_ref_f_f_f_f;
    ref_d_ip__f      run_ref_d_ip;
    ref_f_ip__f      run_ref_f_ip;
    ref_i_d__f       run_ref_i_d;
    ref_i_f__f       run_ref_i_f;
    ref_d_i_d__f     run_ref_d_i_d;
    ref_f_i_f__f     run_ref_f_i_f;
    ref_d_d_i__f     run_ref_d_d_i;
    ref_f_f_i__f     run_ref_f_f_i;
    ref_lli_d__f     run_ref_lli_d;
    ref_lli_f__f     run_ref_lli_f;
    ref_li_d__f      run_ref_li_d;
    ref_li_f__f      run_ref_li_f;
    ref_d_d_dp__f    run_ref_d_d_dp;
    ref_f_f_fp__f    run_ref_f_f_fp;
    ref_d_kcp__f     run_ref_d_kcp;
    ref_f_kcp__f     run_ref_f_kcp;
    ref_d_d_ld__f    run_ref_d_d_ld;
    ref_f_f_ld__f    run_ref_f_f_ld;
    ref_d_d_d_ip__f  run_ref_d_d_d_ip;
    ref_f_f_f_ip__f  run_ref_f_f_f_ip;
    ref_f_f_d__f     run_ref_f_f_d;
    ref_d_d_li__f    run_ref_d_d_li;
    ref_f_f_li__f    run_ref_f_f_li;
    ref_v_d_dp_dp__f run_ref_v_d_dp_dp;
    ref_v_f_fp_fp__f run_ref_v_f_fp_fp;
    ref_d_cd__f      run_ref_d_cd;
    ref_f_cf__f      run_ref_f_cf;
    ref_cd_cd__f     run_ref_cd_cd;
    ref_cf_cf__f     run_ref_cf_cf;
    ref_cd_cd_cd__f  run_ref_cd_cd_cd;
    ref_cf_cf_cf__f  run_ref_cf_cf_cf;
} ref_func_t;

enum {
    k_test_f_f,
    k_test_d_d,
    k_test_d_d_d,
    k_test_v_d_dp_dp,
    k_test_v_f_fp_fp,
    k_test_f_f_f,
    k_test_lli_d,
    k_test_lli_f,
    k_test_i_d,
   k_test_i_f,
    k_test_end,
};



typedef struct svml_op {
    char const * const name_;
    char const * const base_name_;
    svml_func_t const  svml_func_;
    ref_func_t const   ref_func_;
    uint32_t const     sz_;
    uint8_t const      fp_;
    uint8_t const      ulp_;
    uint16_t const     test_type_;

} svml_op_t;

extern const svml_op_t all_svml_defs[];
extern const uint32_t  num_svml_defs;
#endif
