#ifndef SRC_D_SVML_FLT_STATE_H_
#define SRC_D_SVML_FLT_STATE_H_
#include <immintrin.h>
#include "util/common.h"
#include "util/random.h"

#include "svml-test-constants.h"
typedef struct flt_state {
    __m512i  cnt_;
    uint32_t loop_cnt_;
    uint32_t state2_;
} flt_state_t;


#define V0 0
#if V0 == 1
# define FPC(x) ((int32_t)(((1UL << 32) / 16) * (x)))
#elif V0 == 2
# define FPC(x) x
#else
# define FPC(x) ((x) | (int32_t)0xc6000000)
#endif

static void
flt_state_init(flt_state_t * flt_state) {
    seed_rand(12345);

    flt_state->cnt_ = _mm512_set_epi32(
        FPC(0), FPC(1), FPC(2), FPC(3), FPC(4), FPC(5), FPC(6), FPC(7), FPC(8),
        FPC(9), FPC(10), FPC(11), FPC(12), FPC(13), FPC(14), FPC(15));

    flt_state->loop_cnt_ = 0;
    flt_state->state2_   = 0;
}

static void
flt_state_deinit(flt_state_t * flt_state) {
    (void)flt_state;
}

static bool
I_flt_state_is_done(flt_state_t * flt_state) {
    uint32_t loop_cnt = flt_state->loop_cnt_;
    if (loop_cnt == ((1UL << 32) / (16 * (k_test_width / 64)))) {
        return true;
    }
    flt_state->loop_cnt_ = loop_cnt + 1;
    return false;
}
static bool
flt_state_next(flt_state_t * flt_state, uint8_t * scratch) {
    uint32_t i;
    __m512i  cnt, incr, mask;

    if (I_flt_state_is_done(flt_state)) {
        return true;
    }
#if V0 == 2
    mask = _mm512_set_epi32(0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, -1);
#else
    (void)(mask);
#endif

    cnt = flt_state->cnt_;
#if V0 == 1
    incr = _mm512_set1_epi32(1);
#elif V0 == 2
    incr = _mm512_set1_epi32(16);
#else
    incr = _mm512_set1_epi32(16);
#endif


    for (i = 0; i < k_test_width; i += sizeof(__m512i)) {
#if V0 == 2
        cnt = _mm512_or_si512(cnt, mask);
#endif
        __builtin_memcpy(scratch + i, &cnt, sizeof(__m512i));
        __builtin_memcpy(scratch + i + k_test_width, &cnt, sizeof(__m512i));
        __builtin_memcpy(scratch + i + k_test_width * 2, &cnt, sizeof(__m512i));

        cnt = _mm512_add_epi32(cnt, incr);
    }

    flt_state->cnt_ = cnt;
    return false;
}

static bool
flt_state_next2(flt_state_t * flt_state,
                uint8_t *     scratch0,
                uint8_t *     scratch1) {
    uint32_t i, state2;
    __m512i  cnt, incr, ncnt, nincr, cnt0, cnt1, incr0, incr1;

    cnt  = flt_state->cnt_;
    ncnt = _mm512_sub_epi32(_mm512_set1_epi32(0), cnt);

    incr  = _mm512_set1_epi32(16);
    nincr = _mm512_set1_epi32(-16);

    state2 = flt_state->state2_;
    switch (state2 % 26) {
        case 0: {
            cnt0  = cnt;
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 1: {
            cnt0  = cnt;
            cnt1  = ncnt;
            incr0 = incr;
            incr1 = nincr;
        } break;
        case 2: {
            cnt0  = ncnt;
            cnt1  = cnt;
            incr0 = nincr;
            incr1 = incr;
        } break;
        case 3: {
            cnt0  = (__m512i)_mm512_mul_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 4: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_mul_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            incr0 = incr;
            incr1 = incr;
        } break;
        case 5: {
            cnt0  = (__m512i)_mm512_mul_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            cnt1  = ncnt;
            incr0 = incr;
            incr1 = nincr;
        } break;
        case 6: {
            cnt0  = ncnt;
            cnt1  = (__m512i)_mm512_mul_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            incr0 = nincr;
            incr1 = incr;
        } break;
        case 7: {
            cnt0  = (__m512i)_mm512_set1_ps(1.0);
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 8: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_set1_ps(1.0);
            incr0 = incr;
            incr1 = incr;
        } break;
        case 9: {
            cnt0  = (__m512i)_mm512_set1_ps(1.0);
            cnt1  = cnt;
            incr0 = _mm512_set1_epi32(randi32());
            incr1 = incr;
        } break;
        case 10: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_set1_ps(1.0);
            incr0 = incr;
            incr1 = _mm512_set1_epi32(randi32());
        } break;
        case 11: {
            cnt0  = (__m512i)_mm512_set1_ps(1.0);
            cnt1  = cnt;
            incr0 = nincr;
            incr1 = incr;
        } break;
        case 12: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_set1_ps(1.0);
            incr0 = incr;
            incr1 = nincr;
        } break;
        case 13: {
            cnt0  = (__m512i)_mm512_set1_ps(FLT_MIN);
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 14: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_set1_ps(FLT_MIN);
            incr0 = incr;
            incr1 = incr;
        } break;
        case 15: {
            cnt0  = (__m512i)_mm512_set1_ps(9.99999975e-5f);
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 16: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_set1_ps(9.99999975e-5f);
            incr0 = incr;
            incr1 = incr;
        } break;
        case 17: {
            cnt0  = _mm512_set1_epi32(randi32());
            cnt1  = cnt;
            incr0 = _mm512_set1_epi32(randi32());
            incr1 = _mm512_set1_epi32(randi32());
        } break;
        case 18: {
            cnt0  = (__m512i)_mm512_add_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 19: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_add_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            incr0 = incr;
            incr1 = incr;
        } break;
        case 20: {
            cnt0  = _mm512_xor_si512(cnt, _mm512_set1_epi32(randi32()));
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 21: {
            cnt0  = cnt;
            cnt1  = _mm512_set1_epi32(0);
            incr0 = incr;
            incr1 = incr;
        } break;
        case 22: {
            cnt0  = _mm512_set1_epi32(0);
            cnt1  = cnt;
            incr0 = incr;
            incr1 = incr;
        } break;
        case 23: {
            cnt0  = cnt;
            cnt1  = _mm512_set1_epi32(0);
            incr0 = incr;
            incr1 = nincr;
        } break;
        case 24: {
            cnt0  = _mm512_set1_epi32(0);
            cnt1  = cnt;
            incr0 = nincr;
            incr1 = incr;
        } break;
        case 25: {
            cnt0  = cnt;
            cnt1  = (__m512i)_mm512_mul_ps((__m512)cnt,
                                           (__m512)_mm512_set1_epi32(randi32()));
            incr0 = _mm512_set1_epi32(randi32());
            incr1 = _mm512_set1_epi32(randi32());
        } break;
        case 26: {
            if (I_flt_state_is_done(flt_state)) {
                return true;
            }

            cnt0  = cnt;
            cnt1  = _mm512_set1_epi32(randi32());
            incr0 = _mm512_set1_epi32(randi32());
            incr1 = _mm512_set1_epi32(randi32());

            flt_state->cnt_ = _mm512_add_epi32(
                cnt, _mm512_set1_epi32(16 * (k_test_width / 64)));
        } break;
    }

    for (i = 0; i < k_test_width; i += sizeof(__m512i)) {
        __builtin_memcpy(scratch0 + i, &cnt0, sizeof(__m512i));
        __builtin_memcpy(scratch0 + i + k_test_width, &cnt0, sizeof(__m512i));
        __builtin_memcpy(scratch0 + i + k_test_width * 2, &cnt0, sizeof(__m512i));

        __builtin_memcpy(scratch1 + i, &cnt1, sizeof(__m512i));
        __builtin_memcpy(scratch1 + i + k_test_width, &cnt1, sizeof(__m512i));
        __builtin_memcpy(scratch1 + i + k_test_width * 2, &cnt1, sizeof(__m512i));

        cnt0 = _mm512_add_epi32(cnt0, incr0);
        cnt1 = _mm512_add_epi32(cnt1, incr1);
    }
    flt_state->state2_ += 1;
    return false;
}


#endif
