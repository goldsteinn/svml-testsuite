#ifndef _SRC__UTIL__VDSO_UTIL_H_
#define _SRC__UTIL__VDSO_UTIL_H_

#include <sched.h>
#include <sys/time.h>
#include <time.h>

#include "util/attrs.h"
#include "util/types.h"
#include "util/verbosity.h"

#include "util/getcpu-portable.h"
#include "util/vdso-defs.h"


uint32_t vdso_is_full_init();

static CONST_FUNC uint32_t
with_vdso() {
#ifdef WITH_VDSO
    return 1;
#else
    return 0;
#endif
}

static void
NONNULL(2) vdso_clock_gettime(clockid_t clockid, struct timespec * ts) {
    I_vdso_clock_gettime(clockid, ts);
}

static int32_t
NONNULL(1) vdso_gettimeofday(struct timeval * restrict tv, void * restrict tz) {
    return I_vdso_gettimeofday(tv, tz);
}

static int32_t
NONNULL(1) vdso_getcpu(uint32_t * restrict cpu, uint32_t * restrict node) {
    return I_vdso_getcpu(cpu, node);
}

static time_t
NONNULL(1) vdso_time(time_t * t) {
    return I_vdso_time(t);
}


#endif
