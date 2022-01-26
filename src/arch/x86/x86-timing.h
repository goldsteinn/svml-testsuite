#ifndef SRC_ARCH_X86_X86_TIMING_H_
#define SRC_ARCH_X86_X86_TIMING_H_

#include <x86intrin.h>

#include "util/attrs.h"
#include "util/common.h"
#include "util/types.h"

#include "arch/ll-syscall.h"

typedef uint64_t ll_time_t;

static ALWAYS_INLINE ll_time_t
get_ll_time(void) {
    return _rdtsc();
}

static ALWAYS_INLINE const char *
get_ll_units(void) {
    return "cycles";
}


static ALWAYS_INLINE uint64_t
get_ll_dif(ll_time_t t0, ll_time_t t1) {
    return t0 - t1;
}

static ALWAYS_INLINE uint64_t
ll_time_to_u64(ll_time_t t) {
    return t;
}

#define serialize_ooe() __asm__ volatile("lfence" : : :)

#endif
