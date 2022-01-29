#ifndef _SRC__BENCH__ARCH__X86_LL_TIMING_H_
#define _SRC__BENCH__ARCH__X86_LL_TIMING_H_

#include <x86intrin.h>

#include "util/attrs.h"
#include "util/common.h"
#include "util/types.h"

typedef uint64_t ll_time_t;

static ALWAYS_INLINE ll_time_t
get_ll_time() {
    return _rdtsc();
}

static ALWAYS_INLINE const char *
get_ll_units() {
    return "cycles";
}


static ALWAYS_INLINE uint64_t
get_ll_dif(ll_time_t t0, ll_time_t t1) {
    return t0 - t1;
}

#define serialize_ooe() asm volatile("lfence" : : :)

#endif
