#include <immintrin.h>
#include <stdint.h>
#include "util/macro.h"

#ifndef use_fp32
# error "Need specifier"
#endif

#ifndef SVML_FP_EQ_COMMON_H_
# define SVML_FP_EQ_COMMON_H_
static void
printv32(const char * name, __m512i v) {
    uint32_t buf[16];
    uint32_t i;
    memcpy(buf, &v, 64);
    for (i = 0; i < 16; ++i) {
        if (i == 0) {
            fprintf(stderr, "%-24s -> [\n\t%8x\n", name, buf[i]);
        }
        else {
            fprintf(stderr, "\t%8x\n", buf[i]);
        }
    }
    fprintf(stderr, "]\n");
}

static void
printv64(const char * name, __m512i v) {
    uint64_t buf[16];
    uint32_t i;
    memcpy(buf, &v, 64);
    for (i = 0; i < 8; ++i) {
        if (i == 0) {
            fprintf(stderr, "%-24s -> [\n\t%16lx\n", name, buf[i]);
        }
        else {
            fprintf(stderr, "\t%16lx\n", buf[i]);
        }
    }
    fprintf(stderr, "]\n");
}

# if WITH_DBG_OUTPUT
#  define FP_EQ_FAIL()                                                         \
            {                                                                  \
                __m512i inp;                                                   \
                __builtin_memcpy(&inp, scratch + i + k_test_width * 2, 64);    \
                PRINTV(inp);                                                   \
                PRINTV(v0);                                                    \
                PRINTV(v1);                                                    \
                PRINTV(vsub(v0, v1));                                   \
                fprintf(stderr, "---- %u ----\n", __LINE__);                   \
            }
# else
#  define FP_EQ_FAIL()                                                         \
            {                                                                  \
                PRINTV(v0);                                                    \
                PRINTV(v1);                                                    \
                fprintf(stderr, "---- %u ----\n", __LINE__);                   \
            }
# endif

#endif

#if use_fp32
# define mm_int_op(op)  CAT(_mm512_, op, _epi32)
# define mm_uint_op(op) CAT(_mm512_, op, _epu32)
# define mm_fp_op(op)   CAT(_mm512_, op, _ps)
# define mask_T         __mmask16
# define mask_full      0xffff
# define vec_T          __m512
# define func_name      float_eq
# define TMIN           INT32_MIN
# define NEG1           (-1)
# define PRINTV(x)      printv32(V_TO_STR(x), x)
# define VABS_MSK       _mm512_set1_epi32(2147483647)
#else
# define mm_int_op(op)  CAT(_mm512_, op, _epi64)
# define mm_uint_op(op) CAT(_mm512_, op, _epu64)
# define mm_fp_op(op)   CAT(_mm512_, op, _pd)
# define mask_T         __mmask8
# define mask_full      0xff
# define vec_T          __m512d
# define func_name      double_eq
# define TMIN           INT64_MIN
# define NEG1           (-1L)
# define PRINTV(x)      printv64(V_TO_STR(x), x)
# define VABS_MSK       _mm512_set1_epi64((int64_t)((1UL << 63) - 1))
#endif

#define vset1         mm_int_op(set1)
#define vcmpneq       CAT(mm_int_op(cmpneq), _mask)
#define mask_vcmpeq   CAT(mm_int_op(mask_cmpeq), _mask)
#define vsub          mm_int_op(sub)
#define vmask_sub     mm_int_op(mask_sub)
#define vabs_i        mm_int_op(abs)
#define vcmpgt        CAT(mm_uint_op(cmpgt), _mask)
#define vcmplt        CAT(mm_int_op(cmplt), _mask)
#define vfpclass      CAT(mm_fp_op(fpclass), _mask)
#define mask_vfpclass CAT(mm_fp_op(mask_fpclass), _mask)
#define vand          _mm512_and_si512


static bool
func_name(uint8_t const * scratch, int32_t max_ulp_i, uint8_t * hist) {
    enum {
        k_qnan              = 0x01, /* qnan */
        k_positive_zero     = 0x02, /* positive zero */
        k_negative_zero     = 0x04, /* negative zero */
        k_positive_infinity = 0x08, /* positive infinity */
        k_negative_infinity = 0x10, /* negative infinity */
        k_denormal          = 0x20, /* denormal */
        k_negative          = 0x40, /* negative */
        k_snan              = 0x80, /* snan */
    };

    __m512i  v0, v1, a0, a1, max_ulp;
    uint32_t i;

    max_ulp = vset1(max_ulp_i);
    for (i = 0; i < k_test_width; i += 64) {
        __m512i tmpv0, tmpv1;
        mask_T  res0, res1, resneq, orig_resneq;


        __builtin_memcpy(&v0, scratch + i, 64);
        __builtin_memcpy(&v1, scratch + i + k_test_width, 64);
#if 1
        if (vcmpneq(v0, v1) == 0) {
            __m512i histv;
            __builtin_memcpy(&histv, hist, 64);
            histv = vsub(histv, vset1(NEG1));
            __builtin_memcpy(hist, &histv, 64);
            continue;
        }
#endif

#if 0
        resneq = vcmplt(v0, vset1(0)) ^ vcmplt(v1, vset1(0));
        if (UNLIKELY(1 && resneq != 0)) {
            FP_EQ_FAIL();
            return false;
        }
        a0 = v0;
        a1 = v1;
#else
        a0 = vand(v0, VABS_MSK);
        a1 = vand(v1, VABS_MSK);
#endif

        
        tmpv0 = v0;
        tmpv1 = v1;

        tmpv0 = vsub(tmpv0, tmpv1);
        tmpv0 = vabs_i(tmpv0);

        resneq      = vcmpgt(tmpv0, max_ulp);
        orig_resneq = ~resneq;
        if (LIKELY(resneq == 0)) {
            goto update_hist;
        }


        res0 = vfpclass((vec_T)a0, k_negative);
        res1 = vfpclass((vec_T)a1, k_negative);
        if (UNLIKELY(res0 != res1)) {
            FP_EQ_FAIL();
            return false;
        }

        res0 = mask_vfpclass(resneq, (vec_T)a0, k_snan);
        res1 = mask_vfpclass(resneq, (vec_T)a1, k_snan);
        if (UNLIKELY(res0 != res1)) {
            FP_EQ_FAIL();
            return false;
        }
        resneq ^= res0;

        if (LIKELY(resneq == 0)) {
            goto update_hist;
        }


        res0  = mask_vfpclass(resneq, (vec_T)a0, k_positive_infinity);
        res1  = mask_vfpclass(resneq, (vec_T)a1, k_positive_infinity);
        if (UNLIKELY(res0 != res1)) {
            FP_EQ_FAIL();
            return false;
        }
        if (UNLIKELY(resneq ^ res0)) {
            FP_EQ_FAIL();
            return false;
        }
    update_hist : {
        __m512i histv;
        mask_T  hist_res;
        int32_t hist_i;

        die_assert(max_ulp_i <= k_max_ulp);
        for (hist_i = 0; hist_i <= max_ulp_i; ++hist_i) {
            hist_res = mask_vcmpeq(orig_resneq, tmpv0, vset1(hist_i));
            __builtin_memcpy(&histv, hist + 64 * hist_i, 64);
            histv = vmask_sub(histv, hist_res, histv, vset1(NEG1));
            __builtin_memcpy(hist + 64 * hist_i, &histv, 64);
        }
    }
    }
    return true;
}


#undef mm_int_op
#undef mm_uint_op
#undef mm_fp_op
#undef mask_T
#undef mask_full
#undef vec_T
#undef func_name
#undef vcmpneq
#undef mask_vcmpeq
#undef vmask_sub
#undef vsub
#undef vabs_i
#undef vcmpgt
#undef vcmplt
#undef vand
#undef vfpclass
#undef vset1
#undef TMIN
#undef NEG1
#undef mask_vfpclass
#undef PRINTV
#undef VABS_MSK
