#include <immintrin.h>
#include <stdint.h>
#include "util/macro.h"

#ifndef make_float_equals
# error "Need specifier"
#endif

#if make_float_equals
# define mm_int_op(op) CAT(_mm512_, op, _epi32)
# define mm_fp_op(op)  CAT(_mm512_, op, _ps)
# define mask_T        __mmask16
# define mask_full     0xffff
# define vec_T         __m512
# define func_name     float_eq
# define TMIN          INT32_MIN
# define NEG1          (-1)
#else
# define mm_int_op(op) CAT(_mm512_, op, _epi64)
# define mm_fp_op(op)  CAT(_mm512_, op, _pd)
# define mask_T        __mmask8
# define mask_full     0xff
# define vec_T         __m512d
# define func_name     double_eq
# define TMIN          INT64_MIN
# define NEG1          (-1L)
#endif

#define vset1         mm_int_op(set1)
#define vcmpneq       CAT(mm_int_op(cmpneq), _mask)
#define mask_vcmpeq   CAT(mm_int_op(mask_cmpeq), _mask)
#define vsub          mm_int_op(sub)
#define vmask_sub    mm_int_op(mask_sub)
#define vabs_i        mm_int_op(abs)
#define vcmpgt        CAT(mm_int_op(cmpgt), _mask)
#define vabs_fp       mm_fp_op(abs)
#define vfpclass      CAT(mm_fp_op(fpclass), _mask)
#define mask_vfpclass CAT(mm_fp_op(mask_fpclass), _mask)

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

    __m512i  v0, v1, max_ulp;
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


        tmpv0       = (__m512i)vabs_fp((vec_T)v0);
        tmpv1       = (__m512i)vabs_fp((vec_T)v1);
        tmpv0       = vsub(tmpv0, tmpv1);
        tmpv0       = vabs_i(tmpv0);
        resneq      = vcmpgt(tmpv0, max_ulp);
        orig_resneq = ~resneq;
        if (LIKELY(resneq == 0)) {
            goto update_hist;
        }

        res0 = vfpclass((vec_T)v0, k_negative);
        res1 = vfpclass((vec_T)v1, k_negative);
        if (UNLIKELY(res0 != res1)) {
            return false;
        }

        res0 = mask_vfpclass(resneq, (vec_T)v0, k_snan);
        res1 = mask_vfpclass(resneq, (vec_T)v1, k_snan);
        if (UNLIKELY(res0 != res1)) {
            return false;
        }
        resneq ^= res0;

        if (LIKELY(resneq == 0)) {
            goto update_hist;
        }


        v0   = (__m512i)vabs_fp((vec_T)v0);
        v1   = (__m512i)vabs_fp((vec_T)v1);
        res0 = mask_vfpclass(resneq, (vec_T)v0, k_positive_infinity);
        res1 = mask_vfpclass(resneq, (vec_T)v1, k_positive_infinity);
        if (UNLIKELY(res0 != res1)) {
            return false;
        }
        if (UNLIKELY(resneq ^ res0)) {
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

#undef make_float_equals
#undef mm_int_op
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
#undef vabs_fp
#undef vfpclass
#undef vset1
#undef TMIN
#undef NEG1
#undef mask_vfpclass
