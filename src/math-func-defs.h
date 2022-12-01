#ifndef SRC_D_MATH_FUNC_DEFS_H_
#define SRC_D_MATH_FUNC_DEFS_H_

#include <mpfr.h>

#include "math-defs.h"
#include "mpfr-func-defs.h"

static double
run_acos(double x) {
    return acos(x);
}

static float
run_acosf(float x) {
    return acosf(x);
}
static double
run_acosh(double x) {
    return acosh(x);
}
static float
run_acoshf(float x) {
    return acoshf(x);
}
static double
run_asin(double x) {
    return asin(x);
}
static float
run_asinf(float x) {
    return asinf(x);
}
static double
run_asinh(double x) {
    return asinh(x);
}
static float
run_asinhf(float x) {
    return asinhf(x);
}
static double
run_atan(double x) {
    return atan(x);
}
static double
run_atan2(double y, double x) {
    return atan2(y, x);
}
static float
run_atan2f(float y, float x) {
    return atan2f(y, x);
}
static float
run_atanf(float x) {
    return atanf(x);
}
static double
run_atanh(double x) {
    return atanh(x);
}
static float
run_atanhf(float x) {
    return atanhf(x);
}
static double
run_cbrt(double x) {
    return cbrt(x);
}
static float
run_cbrtf(float x) {
    return cbrtf(x);
}
static double
run_ceil(double x) {
    return ceil(x);
}
static float
run_ceilf(float x) {
    return ceilf(x);
}

static double
run_copysign(double x, double y) {
    return copysign(x, y);
}

static float
run_copysignf(float x, float y) {
    return copysignf(x, y);
}
static double
run_cos(double x) {
    return cos(x);
}
static float
run_cosf(float x) {
    return cosf(x);
}
static double
run_cosh(double x) {
    return cosh(x);
}
static float
run_coshf(float x) {
    return coshf(x);
}
static double
run_erf(double x) {
    return erf(x);
}
static double
run_erfc(double x) {
    return erfc(x);
}
static float
run_erfcf(float x) {
    return erfcf(x);
}
static float
run_erff(float x) {
    return erff(x);
}
static double
run_exp(double x) {
    return exp(x);
}
static double
run_exp10(double x) {
    return exp10(x);
}
static float
run_exp10f(float x) {
    return exp10f(x);
}
static double
run_exp2(double x) {
    return exp2(x);
}
static float
run_exp2f(float x) {
    return exp2f(x);
}
static float
run_expf(float x) {
    return expf(x);
}
static double
run_expm1(double x) {
    return expm1(x);
}
static float
run_expm1f(float x) {
    return expm1f(x);
}
static double
run_fabs(double x) {
    return fabs(x);
}
static float
run_fabsf(float x) {
    return fabsf(x);
}
static double
run_fdim(double x, double y) {
    return fdim(x, y);
}
static float
run_fdimf(float x, float y) {
    return fdimf(x, y);
}
static double
run_floor(double x) {
    return floor(x);
}
static float
run_floorf(float x) {
    return floorf(x);
}
static double
run_fma(double x, double y, double z) {
    return fma(x, y, z);
}
static float
run_fmaf(float x, float y, float z) {
    return fmaf(x, y, z);
}
static double
run_fmax(double x, double y) {
    return fmax(x, y);
}
static float
run_fmaxf(float x, float y) {
    return fmaxf(x, y);
}
static double
run_fmin(double x, double y) {
    return fmin(x, y);
}
static float
run_fminf(float x, float y) {
    return fminf(x, y);
}
static double
run_fmod(double x, double y) {
    return fmod(x, y);
}
static float
run_fmodf(float x, float y) {
    return fmodf(x, y);
}
static double
run_frexp(double x, int * exp) {
    return frexp(x, exp);
}
static float
run_frexpf(float x, int * exp) {
    return frexpf(x, exp);
}
static double
run_gamma(double x) {
    return gamma(x);
}
static float
run_gammaf(float x) {
    return gammaf(x);
}
static double
run_hypot(double x, double y) {
    return hypot(x, y);
}
static float
run_hypotf(float x, float y) {
    return hypotf(x, y);
}
static int
run_ilogb(double x) {
    return ilogb(x);
}
static int
run_ilogbf(float x) {
    return ilogbf(x);
}
static double
run_j0(double x) {
    return j0(x);
}
static float
run_j0f(float x) {
    return j0f(x);
}
static double
run_j1(double x) {
    return j1(x);
}
static float
run_j1f(float x) {
    return j1f(x);
}
static double
run_jn(int n, double x) {
    return jn(n, x);
}
static float
run_jnf(int n, float x) {
    return jnf(n, x);
}
static double
run_ldexp(double x, int exp) {
    return ldexp(x, exp);
}
static float
run_ldexpf(float x, int exp) {
    return ldexpf(x, exp);
}
static double
run_lgamma(double x) {
    return lgamma(x);
}
static double
run_lgamma_r(double x, int * signp) {
    return lgamma_r(x, signp);
}
static float
run_lgammaf(float x) {
    return lgammaf(x);
}
static float
run_lgammaf_r(float x, int * signp) {
    return lgammaf_r(x, signp);
}
static long long int
run_llrint(double x) {
    return llrint(x);
}
static long long int
run_llrintf(float x) {
    return llrintf(x);
}
static long long int
run_llround(double x) {
    return llround(x);
}
static long long int
run_llroundf(float x) {
    return llroundf(x);
}
static double
run_log(double x) {
    return log(x);
}
static double
run_log10(double x) {
    return log10(x);
}
static float
run_log10f(float x) {
    return log10f(x);
}
static double
run_log1p(double x) {
    return log1p(x);
}
static float
run_log1pf(float x) {
    return log1pf(x);
}
static double
run_log2(double x) {
    return log2(x);
}
static float
run_log2f(float x) {
    return log2f(x);
}
static double
run_logb(double x) {
    return logb(x);
}
static float
run_logbf(float x) {
    return logbf(x);
}
static float
run_logf(float x) {
    return logf(x);
}
static long int
run_lrint(double x) {
    return lrint(x);
}
static long int
run_lrintf(float x) {
    return lrintf(x);
}
static long int
run_lround(double x) {
    return lround(x);
}
static long int
run_lroundf(float x) {
    return lroundf(x);
}
static double
run_modf(double x, double * iptr) {
    return modf(x, iptr);
}
static float
run_modff(float x, float * iptr) {
    return modff(x, iptr);
}
static double
run_nan(const char * tagp) {
    return nan(tagp);
}
static float
run_nanf(const char * tagp) {
    return nanf(tagp);
}
static double
run_nearbyint(double x) {
    return nearbyint(x);
}
static float
run_nearbyintf(float x) {
    return nearbyintf(x);
}
static double
run_nextafter(double x, double y) {
    return nextafter(x, y);
}
static float
run_nextafterf(float x, float y) {
    return nextafterf(x, y);
}
static double
run_nexttoward(double x, long double y) {
    return nexttoward(x, y);
}
static float
run_nexttowardf(float x, long double y) {
    return nexttowardf(x, y);
}
static double
run_pow(double x, double y) {
    return pow(x, y);
}
static float
run_powf(float x, float y) {
    return powf(x, y);
}
static double
run_remainder(double x, double y) {
    return remainder(x, y);
}
static float
run_remainderf(float x, float y) {
    return remainderf(x, y);
}
static double
run_remquo(double x, double y, int * quo) {
    return remquo(x, y, quo);
}
static float
run_remquof(float x, float y, int * quo) {
    return remquof(x, y, quo);
}
static double
run_rint(double x) {
    return rint(x);
}
static float
run_rintf(float x) {
    return rintf(x);
}
static double
run_round(double x) {
    return round(x);
}
static float
run_roundf(float x) {
    return roundf(x);
}
static double
run_roundeven(double x) {
    return roundeven(x);
}
static float
run_roundevenf(float x) {
    return roundevenf(x);
}

static double
run_scalb(double x, double exp) {
    return scalb(x, exp);
}
static float
run_scalbf(float x, float exp) {
    return scalbf(x, exp);
}
static double
run_scalbln(double x, long int exp) {
    return scalbln(x, exp);
}
static float
run_scalblnf(float x, long int exp) {
    return scalblnf(x, exp);
}
static double
run_scalbn(double x, int exp) {
    return scalbn(x, exp);
}
static float
run_scalbnf(float x, int exp) {
    return scalbnf(x, exp);
}
static double
run_significand(double x) {
    return significand(x);
}
static float
run_significandf(float x) {
    return significandf(x);
}
static int
run_signbit(double x) {
    return signbit(x);
}
static int
run_signbitf(float x) {
    return signbit(x);
}
static double
run_sin(double x) {
    return sin(x);
}
static void
run_sincos(double x, double * sin, double * cos) {
    return sincos(x, sin, cos);
}
static void
run_sincosf(float x, float * sin, float * cos) {
    return sincosf(x, sin, cos);
}
static float
run_sinf(float x) {
    return sinf(x);
}
static double
run_sinh(double x) {
    return sinh(x);
}
static float
run_sinhf(float x) {
    return sinhf(x);
}
static double
run_sqrt(double x) {
    return sqrt(x);
}
static float
run_sqrtf(float x) {
    return sqrtf(x);
}
static double
run_tan(double x) {
    return tan(x);
}

static float
run_tanf(float x) {
    return tanf(x);
}
static double
run_tanh(double x) {
    return tanh(x);
}
static float
run_tanhf(float x) {
    return tanhf(x);
}
static double
run_tgamma(double x) {
    return tgamma(x);
}
static float
run_tgammaf(float x) {
    return tgammaf(x);
}
static double
run_trunc(double x) {
    return trunc(x);
}
static float
run_truncf(float x) {
    return truncf(x);
}
static double
run_y0(double x) {
    return y0(x);
}
static float
run_y0f(float x) {
    return y0f(x);
}
static double
run_y1(double x) {
    return y1(x);
}
static float
run_y1f(float x) {
    return y1f(x);
}
static double
run_yn(int n, double x) {
    return yn(n, x);
}
static float
run_ynf(int n, float x) {
    return ynf(n, x);
}
static double
run_cabs(cdouble_t z) {
    return cabs(z);
}
static float
run_cabsf(cfloat_t z) {
    return cabsf(z);
}
static cdouble_t
run_cacos(cdouble_t z) {
    return cacos(z);
}
static cfloat_t
run_cacosf(cfloat_t z) {
    return cacosf(z);
}
static cdouble_t
run_cacosh(cdouble_t z) {
    return cacosh(z);
}
static cfloat_t
run_cacoshf(cfloat_t z) {
    return cacoshf(z);
}
static double
run_carg(cdouble_t z) {
    return carg(z);
}
static float
run_cargf(cfloat_t z) {
    return cargf(z);
}
static cdouble_t
run_casin(cdouble_t z) {
    return casin(z);
}
static cfloat_t
run_casinf(cfloat_t z) {
    return casinf(z);
}
static cdouble_t
run_casinh(cdouble_t z) {
    return casinh(z);
}
static cfloat_t
run_casinhf(cfloat_t z) {
    return casinhf(z);
}
static cdouble_t
run_catan(cdouble_t z) {
    return catan(z);
}
static cfloat_t
run_catanf(cfloat_t z) {
    return catanf(z);
}
static cdouble_t
run_catanh(cdouble_t z) {
    return catanh(z);
}
static cfloat_t
run_catanhf(cfloat_t z) {
    return catanhf(z);
}
static cdouble_t
run_ccos(cdouble_t z) {
    return ccos(z);
}
static cfloat_t
run_ccosf(cfloat_t z) {
    return ccosf(z);
}
static cdouble_t
run_ccosh(cdouble_t z) {
    return ccosh(z);
}
static cfloat_t
run_ccoshf(cfloat_t z) {
    return ccoshf(z);
}
static cdouble_t
run_cexp(cdouble_t z) {
    return cexp(z);
}
static cfloat_t
run_cexpf(cfloat_t z) {
    return cexpf(z);
}
static double
run_cimag(cdouble_t z) {
    return cimag(z);
}
static float
run_cimagf(cfloat_t z) {
    return cimagf(z);
}
static cdouble_t
run_clog(cdouble_t z) {
    return clog(z);
}
static cfloat_t
run_clogf(cfloat_t z) {
    return clogf(z);
}
static cdouble_t
run_conj(cdouble_t z) {
    return conj(z);
}
static cfloat_t
run_conjf(cfloat_t z) {
    return conjf(z);
}
static cdouble_t
run_cpow(cdouble_t x, cdouble_t z) {
    return cpow(x, z);
}
static cfloat_t
run_cpowf(cfloat_t x, cfloat_t z) {
    return cpowf(x, z);
}
static cdouble_t
run_cproj(cdouble_t z) {
    return cproj(z);
}
static cfloat_t
run_cprojf(cfloat_t z) {
    return cprojf(z);
}
static double
run_creal(cdouble_t z) {
    return creal(z);
}
static float
run_crealf(cfloat_t z) {
    return crealf(z);
}
static cdouble_t
run_csin(cdouble_t z) {
    return csin(z);
}
static cfloat_t
run_csinf(cfloat_t z) {
    return csinf(z);
}
static cdouble_t
run_csinh(cdouble_t z) {
    return csinh(z);
}
static cfloat_t
run_csinhf(cfloat_t z) {
    return csinhf(z);
}
static cdouble_t
run_csqrt(cdouble_t z) {
    return csqrt(z);
}
static cfloat_t
run_csqrtf(cfloat_t z) {
    return csqrtf(z);
}
static cdouble_t
run_ctan(cdouble_t z) {
    return ctan(z);
}
static cfloat_t
run_ctanf(cfloat_t z) {
    return ctanf(z);
}
static cdouble_t
run_ctanh(cdouble_t z) {
    return ctanh(z);
}
static cfloat_t
run_ctanhf(cfloat_t z) {
    return ctanhf(z);
}

#endif
