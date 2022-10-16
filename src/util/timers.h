#ifndef _SRC__UTIL__TIMERS_H_
#define _SRC__UTIL__TIMERS_H_

#include <time.h>

#include "util/attrs.h"
#include "util/types.h"


#include "arch/ll-timing.h"
#include "util/internal/timers.h"
#include "util/vdso-util.h"

static ALWAYS_INLINE
NONNULL(2) void direct_gettime(clockid_t clk, struct timespec * ts) {
    direct_clock_gettime(clk, ts);
}

static ALWAYS_INLINE
NONNULL(1) void gettime(struct timespec * ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}

static ALWAYS_INLINE
NONNULL(1) void vdso_gettime(struct timespec * ts) {
    vdso_clock_gettime(CLOCK_MONOTONIC, ts);
}

static ALWAYS_INLINE
NONNULL(1) void vdso_gtod(struct timeval * tv) {
    vdso_gettimeofday(tv, NULL);
}

static ALWAYS_INLINE
NONNULL(1) void gtod(struct timeval * tv) {
    gettimeofday(tv, NULL);
}

static ALWAYS_INLINE
NONNULL(1) void direct_gtod(struct timeval * tv) {
    direct_gettimeofday(tv);
}


#endif
