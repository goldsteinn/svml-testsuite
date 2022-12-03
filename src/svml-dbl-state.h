#ifndef SRC_D_SVML_DBL_STATE_H_
#define SRC_D_SVML_DBL_STATE_H_
#include <immintrin.h>
#include "svml-flt-state.h"
#include "svml-test-constants.h"
#include "util/common.h"
#include "util/random.h"
enum {
    k_dbl_arr_sz  = 8192,
    k_dbl_mul_max = (1000UL * 1000UL * 1000UL * 1000UL * 1000UL * 1000UL)
};
const_assert(k_dbl_arr_sz % k_test_width == 0);

#define k_dbl_mul_scale (1.0001)
typedef struct dbl_state {
    __m512i     mulv_;
    uint8_t     buf_[k_dbl_arr_sz] ALIGNED(64);
    uint32_t    offset_;
    uint16_t    state2_;
    uint8_t     ge_;
    uint8_t     flt_done_;
    flt_state_t flt_state_;
} dbl_state_t;

void dbl_state_init(dbl_state_t * dbl_state);
static void
dbl_state_deinit(dbl_state_t * dbl_state) {
    (void)dbl_state;
}

static bool
I_dbl_update_mul(dbl_state_t * dbl_state) {
    __m512i  mulv;
    uint32_t i;
    uint32_t ge = dbl_state->ge_;
    mulv        = dbl_state->mulv_;
    ge |= _mm512_cmpnlt_pd_mask(_mm512_abs_pd((__m512d)mulv),
                                _mm512_set1_pd(k_dbl_mul_max));
    if (ge == 0xff) {
        return true;
    }

    dbl_state->ge_ = (uint8_t)ge;

    for (i = 0; i < k_dbl_arr_sz; i += sizeof(__m512i)) {
        __m512d bufv;
        __builtin_memcpy(&bufv, dbl_state->buf_ + i, sizeof(__m512i));
        bufv = _mm512_mul_pd((__m512d)bufv, (__m512d)mulv);
        __builtin_memcpy(dbl_state->buf_ + i, &bufv, sizeof(__m512i));
    }

    dbl_state->mulv_ =
        (__m512i)_mm512_mul_pd((__m512d)mulv, _mm512_set1_pd(k_dbl_mul_scale));
    return false;
}

static bool
dbl_state_next(dbl_state_t * dbl_state, uint8_t * scratch) {
    if (dbl_state->flt_done_ ||
        flt_state_next(&(dbl_state->flt_state_), scratch)) {
        uint32_t offset = dbl_state->offset_;
        if (offset == k_dbl_arr_sz) {
            offset = 0;
            if (I_dbl_update_mul(dbl_state)) {
                return true;
            }
        }
        __builtin_memcpy(scratch, dbl_state->buf_, k_test_width);
        __builtin_memcpy(scratch + k_test_width, dbl_state->buf_, k_test_width);
        __builtin_memcpy(scratch + k_test_width * 2, dbl_state->buf_, k_test_width);

        dbl_state->offset_ = offset + k_test_width;
        return false;
    }
    return false;
}

static bool
dbl_state_next2(dbl_state_t * dbl_state,
                uint8_t *     scratch0,
                uint8_t *     scratch1) {
    uint32_t offset = dbl_state->offset_;
    uint32_t rv, state2, i;
    uint8_t *buf0, *buf1;
    __m512i  tmp, v;

    if (offset + k_test_width > k_dbl_arr_sz / 2) {
        offset = 0;
        if (I_dbl_update_mul(dbl_state)) {
            return true;
        }
    }
    buf0 = dbl_state->buf_ + offset;
    __builtin_memcpy(scratch0, buf0, k_test_width);
    __builtin_memcpy(scratch0 + k_test_width, buf0, k_test_width);
    __builtin_memcpy(scratch0 + k_test_width * 2, buf0, k_test_width);

    buf1 = dbl_state->buf_ + k_dbl_arr_sz - offset - k_test_width;
    __builtin_memcpy(scratch1, buf1, k_test_width);
    __builtin_memcpy(scratch1 + k_test_width, buf1, k_test_width);
    __builtin_memcpy(scratch1 + k_test_width * 2, buf1, k_test_width);
    state2 = dbl_state->state2_;
    switch (state2 % 8) {
        case 0: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf0 + i, sizeof(v));
                v = _mm512_xor_si512(v, tmp);
                __builtin_memcpy(buf0 + i, &v, sizeof(v));
            }
        } break;
        case 1: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf1 + i, sizeof(v));
                v = _mm512_xor_si512(v, tmp);
                __builtin_memcpy(buf1 + i, &v, sizeof(v));
            }
        } break;
        case 2: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf0 + i, sizeof(v));
                v = (__m512i)_mm512_min_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf0 + i, &v, sizeof(v));
            }
        } break;
        case 3: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf1 + i, sizeof(v));
                v = (__m512i)_mm512_min_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf1 + i, &v, sizeof(v));
            }
        } break;
        case 4: {
            tmp = (__m512i)_mm512_set1_pd(.00000001);
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf0 + i, sizeof(v));
                v = (__m512i)_mm512_mul_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf0 + i, &v, sizeof(v));
            }
        } break;
        case 5: {
            tmp = (__m512i)_mm512_set1_pd(.00000001);
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf1 + i, sizeof(v));
                v = (__m512i)_mm512_mul_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf1 + i, &v, sizeof(v));
            }
        } break;
        case 6: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf0 + i, sizeof(v));
                v = (__m512i)_mm512_mul_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf0 + i, &v, sizeof(v));
            }
        } break;
        case 7: {
            tmp = _mm512_set1_epi32(randi32());
            for (i = 0; i < k_test_width; i += 64) {
                __builtin_memcpy(&v, buf1 + i, sizeof(v));
                v = (__m512i)_mm512_mul_pd((__m512d)v, (__m512d)tmp);
                __builtin_memcpy(buf1 + i, &v, sizeof(v));
            }
            do {
                rv = rand32() % k_test_width;
            } while (rv == 0);
            dbl_state->offset_ = offset + rv;

        } break;
    }

    dbl_state->state2_ += 1;
    return false;
}


#endif
