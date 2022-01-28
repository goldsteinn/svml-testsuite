#ifndef _SRC__UTIL__RANDOM_H_
#define _SRC__UTIL__RANDOM_H_

#include <stdint.h>

#include "util/common.h"

extern __thread uint64_t _rseed_;

static CONST_FUNC uint64_t
_rand64(uint64_t _seed) {
    __uint128_t t =
        CAST(__uint128_t, _seed) * (_seed ^ CAST(uint64_t, 0xe7037ed1a0b428db));
    return CAST(uint64_t, t >> 64) ^ CAST(uint64_t, t);
}

static uint64_t
rand64s(uint64_t * seed) {
    uint64_t _seed;
    _seed = *seed;
    *seed = _seed + CAST(uint64_t, 0xa0761d6478bd642f);
    return _rand64(_seed);
}

static uint64_t
rand64() {
    return rand64s(&_rseed_);
}

/* Haven't found any rand32 library that is particularly optimized so just use
 * the rand64 function for now. */
static CONST_FUNC uint32_t
_rand32(uint64_t _seed) {
    return CAST(uint32_t, _rand64(_seed));
}

static uint32_t
rand32s(uint64_t * seed) {
    return CAST(uint32_t, rand64s(seed));
}

static uint32_t
rand32() {
    return CAST(uint32_t, rand64());
}

static void
seed_rand(uint64_t _seed) {
    _rseed_ = _seed;
}

static uint64_t
get_rand_seed() {
    return _rseed_;
}

#endif
