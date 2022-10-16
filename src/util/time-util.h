#ifndef _SRC__UTIL__TIME_UTIL_H_
#define _SRC__UTIL__TIME_UTIL_H_


#include <time.h>

#include "util/attrs.h"
#include "util/common.h"
#include "util/types.h"

#include "util/timers.h"

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t ts_to_ns(struct timespec const * ts) {
    return 1000UL * 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec;
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t ts_to_us(struct timespec const * ts) {
    return 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec / 1000UL;
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t ts_to_ms(struct timespec const * ts) {
    return 1000UL * ts->tv_sec + ts->tv_nsec / (1000UL * 1000UL);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t tv_to_ns(struct timeval const * tv) {
    return 1000UL * 1000UL * 1000UL * tv->tv_sec + 1000UL * tv->tv_usec;
}
static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t tv_to_us(struct timeval const * tv) {
    return 1000UL * 1000UL * tv->tv_sec + tv->tv_usec;
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) uint64_t tv_to_ms(struct timeval const * tv) {
    return 1000UL * tv->tv_sec + tv->tv_usec / 1000UL;
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    ts_dif_ns(struct timespec const * ts0, struct timespec const * ts1) {
    return ts_to_ns(ts0) - ts_to_ns(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    ts_dif_us(struct timespec const * ts0, struct timespec const * ts1) {
    return ts_to_us(ts0) - ts_to_us(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    ts_dif_ms(struct timespec const * ts0, struct timespec const * ts1) {
    return ts_to_ms(ts0) - ts_to_ms(ts1);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    tv_dif_ns(struct timeval const * tv0, struct timeval const * tv1) {
    return tv_to_ns(tv0) - tv_to_ns(tv1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    tv_dif_us(struct timeval const * tv0, struct timeval const * tv1) {
    return tv_to_us(tv0) - tv_to_us(tv1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) uint64_t
    tv_dif_ms(struct timeval const * tv0, struct timeval const * tv1) {
    return tv_to_ms(tv0) - tv_to_ms(tv1);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double ts_to_ns_dbl(struct timespec const * ts) {
    return CAST(double, ts_to_ns(ts));
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double ts_to_us_dbl(struct timespec const * ts) {
    return CAST(double, 1000UL * 1000UL * ts->tv_sec) +
           CAST(double, ts->tv_nsec) / CAST(double, 1000UL);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double ts_to_ms_dbl(struct timespec const * ts) {
    return CAST(double, 1000UL * ts->tv_sec) +
           CAST(double, ts->tv_nsec) / CAST(double, 1000UL * 1000UL);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double tv_to_ns_dbl(struct timeval const * tv) {
    return CAST(double, tv_to_ns(tv));
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double tv_to_us_dbl(struct timeval const * tv) {
    return CAST(double, tv_to_us(tv));
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1) double tv_to_ms_dbl(struct timeval const * tv) {
    return CAST(double, 1000UL * tv->tv_sec) +
           CAST(double, tv->tv_usec) / CAST(double, 1000UL);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_ns_dbl(struct timespec const * ts0,
                                struct timespec const * ts1) {
    return ts_to_ns_dbl(ts0) - ts_to_ns_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_us_dbl(struct timespec const * ts0,
                                struct timespec const * ts1) {
    return ts_to_us_dbl(ts0) - ts_to_us_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double dif_ms_dbl(struct timespec const * ts0,
                                struct timespec const * ts1) {
    return ts_to_ms_dbl(ts0) - ts_to_ms_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double ts_dif_ns_dbl(struct timespec const * ts0,
                                   struct timespec const * ts1) {
    return ts_to_ns_dbl(ts0) - ts_to_ns_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double ts_dif_us_dbl(struct timespec const * ts0,
                                   struct timespec const * ts1) {
    return ts_to_us_dbl(ts0) - ts_to_us_dbl(ts1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double ts_dif_ms_dbl(struct timespec const * ts0,
                                   struct timespec const * ts1) {
    return ts_to_ms_dbl(ts0) - ts_to_ms_dbl(ts1);
}


static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double tv_dif_ns_dbl(struct timeval const * tv0,
                                   struct timeval const * tv1) {
    return tv_to_ns_dbl(tv0) - tv_to_ns_dbl(tv1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double tv_dif_us_dbl(struct timeval const * tv0,
                                   struct timeval const * tv1) {
    return tv_to_us_dbl(tv0) - tv_to_us_dbl(tv1);
}

static ALWAYS_INLINE PURE_FUNC
NONNULL(1, 2) double tv_dif_ms_dbl(struct timeval const * tv0,
                                   struct timeval const * tv1) {
    return tv_to_ms_dbl(tv0) - tv_to_ms_dbl(tv1);
}


static ALWAYS_INLINE uint64_t
direct_get_ns() {
    struct timespec ts;
    direct_gettime(CLOCK_MONOTONIC, &ts);
    return ts_to_ns(&ts);
}

static ALWAYS_INLINE uint64_t
get_ns() {
    struct timespec ts;
    gettime(&ts);
    return ts_to_ns(&ts);
}

static ALWAYS_INLINE uint64_t
vdso_get_ns() {
    struct timespec ts;
    vdso_gettime(&ts);
    return ts_to_ns(&ts);
}

static ALWAYS_INLINE uint64_t
direct_gtod_ns() {
    struct timeval tv;
    direct_gtod(&tv);
    return tv_to_ns(&tv);
}

static ALWAYS_INLINE uint64_t
gtod_ns() {
    struct timeval tv;
    gtod(&tv);
    return tv_to_ns(&tv);
}

static ALWAYS_INLINE uint64_t
vdso_gtod_ns() {
    struct timeval tv;
    vdso_gtod(&tv);
    return tv_to_ns(&tv);
}

static ALWAYS_INLINE void 
get_thread_ts(struct timespec * ts) {
    direct_gettime(CLOCK_THREAD_CPUTIME_ID, ts);
}

static ALWAYS_INLINE void 
get_proc_ts(struct timespec * ts) {
    direct_gettime(CLOCK_PROCESS_CPUTIME_ID, ts);
}

static ALWAYS_INLINE uint64_t
get_thread_ns() {
    struct timespec ts;
    get_thread_ts(&ts);
    return ts_to_ns(&ts);
}

static ALWAYS_INLINE uint64_t
get_proc_ns() {
    struct timespec ts;
    get_proc_ts(&ts);
    return ts_to_ns(&ts);
}


void print_res(char const * desc,
               uint64_t     time,
               uint64_t     trials,
               char const * units);

#endif
