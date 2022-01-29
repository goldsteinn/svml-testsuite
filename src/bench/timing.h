#ifndef _SRC__BENCH__TIMING_H_
#define _SRC__BENCH__TIMING_H_

#include <time.h>

#include "util/attrs.h"
#include "util/types.h"

#include "bench/arch/ll-timing.h"

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t to_ns(struct timespec * ts) {
    return 1000UL * 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec;
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t to_us(struct timespec * ts) {
    return 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec / 1000UL;
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t to_ms(struct timespec * ts) {
    return 1000UL * ts->tv_sec + ts->tv_nsec / (1000UL * 1000UL);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t dif_ns(struct timespec * ts0, struct timespec * ts1) {
    return to_ns(ts0) - to_ns(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t dif_us(struct timespec * ts0, struct timespec * ts1) {
    return to_us(ts0) - to_us(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t dif_ms(struct timespec * ts0, struct timespec * ts1) {
    return to_ms(ts0) - to_ms(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double to_ns_dbl(struct timespec * ts) {
    return CAST(double, to_ns(ts));
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double to_us_dbl(struct timespec * ts) {
    return CAST(double, 1000UL * 1000UL * ts->tv_sec) +
           CAST(double, ts->tv_nsec) / CAST(double, 1000UL);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double to_ms_dbl(struct timespec * ts) {
    return CAST(double, 1000UL * ts->tv_sec) +
           CAST(double, ts->tv_nsec) / CAST(double, 1000UL * 1000UL);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_ns_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_ns_dbl(ts0) - to_ns_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_us_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_us_dbl(ts0) - to_us_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_ms_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_ms_dbl(ts0) - to_ms_dbl(ts1);
}

static ALWAYS_INLINE
NONNULL(1) void gettime(struct timespec * ts) {
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, ts);
}

static ALWAYS_INLINE uint64_t
get_ns() {
    struct timespec ts;
    gettime(&ts);
    return to_ns(&ts);
}

void print_res(const char * desc,
               uint64_t     time,
               uint64_t     trials,
               const char * units);

#endif
