#ifndef SRC_D_UTIL_D_RANDOM_H_
#define SRC_D_UTIL_D_RANDOM_H_

#include <stdint.h>

#include "util/common.h"


extern __thread uint64_t I_rseed_;

uint64_t   true_rand64(void);
uint32_t   true_rand32(void);
void       true_randomize_buffer(void * buf, uint64_t buf_sz);
uint32_t * make_true_rand32_buffer(uint64_t nitems);
uint64_t * make_true_rand64_buffer(uint64_t nitems);

void randomize_buffer(uint8_t * buf, uint64_t buf_sz);


static CONST_FUNC uint64_t
I_rand64c(__uint128_t t) {
    return CAST(uint64_t, t >> 64) ^ CAST(uint64_t, t);
}
static CONST_FUNC uint64_t
rand64c(uint64_t seed) {
    return I_rand64c(CAST(__uint128_t, seed) *
                     (seed ^ CAST(uint64_t, 0xe7037ed1a0b428db)));
}

static uint64_t
next_rand_seed(uint64_t seed) {
    return seed + CAST(uint64_t, 0xa0761d6478bd642f);
}

NONNULL(1) static uint64_t rand64s(uint64_t * seedp) {
    uint64_t seed;
    seed  = *seedp;
    *seedp = next_rand_seed(seed);
    return rand64c(seed);
}

static uint64_t
rand64(void) {
    return rand64s(&I_rseed_);
}

/* Haven't found any rand32 library that is particularly optimized so just use
 * the rand64 function for now. */
static CONST_FUNC uint32_t
rand32c(uint64_t seed) {
    return CAST(uint32_t, rand64c(seed));
}

NONNULL(1) static uint32_t rand32s(uint64_t * seed) {
    return CAST(uint32_t, rand64s(seed));
}

static uint32_t
rand32(void) {
    return CAST(uint32_t, rand64());
}

static void
seed_rand(uint64_t seed) {
    I_rseed_ = seed;
}

static uint64_t
get_rand_seed(void) {
    return I_rseed_;
}

#endif
