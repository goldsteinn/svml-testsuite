#ifndef _SRC__UTIL__VDSO_UTIL_H_
#define _SRC__UTIL__VDSO_UTIL_H_

#include <sched.h>
#include <sys/time.h>
#include <time.h>

#include "util/attrs.h"
#include "util/types.h"

extern void const * vdso_funcs[];

enum {
    vdso_clock_gettime_offset = 0,
    vdso_gettimeofday_offset  = 1,
    vdso_getcpu_offset        = 2,
    vdso_time_offset          = 3
};

uint64_t CONST_FUNC get_vdso_expec_mask();
uint64_t            vdso_init();
int32_t             safe_vdso_init();

static FUNC_T(clock_gettime)
get_vdso_clock_gettime() {
    return vdso_funcs[vdso_clock_gettime_offset];
}

static int32_t
NONNULL(2) vdso_clock_gettime(clockid_t clockid, struct timespec * ts) {
    return get_vdso_clock_gettime()(clockid, ts);
}

static int32_t
NONNULL(1) vdso_gettimeofday(struct timeval * restrict tv,
                             struct timezone * restrict tz) {
    return (*CAST_TO_FUNC(gettimeofday,
                          vdso_funcs[vdso_gettimeofday_offset]))(tv, tz);
}

static int32_t
NONNULL(1) vdso_getcpu(uint32_t * restrict cpu, uint32_t * restrict node) {
    return CAST_TO_FUNC(getcpu, vdso_funcs[vdso_getcpu_offset])(cpu, node);
}

static time_t
NONNULL(1) vdso_time(time_t * tloc) {
    return CAST_TO_FUNC(time, vdso_funcs[vdso_time_offset])(tloc);
}




#endif
