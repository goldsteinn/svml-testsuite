#ifndef SRC_UTIL_VDSO_DEFS_H_
#define SRC_UTIL_VDSO_DEFS_H_

#include <sched.h>
#include <sys/time.h>
#include <time.h>

#include "util/attrs.h"
#include "util/getcpu-portable.h"
#include "util/internal/timers.h"
#include "util/types.h"

#define I_FALLBACK_clock_gettime direct_clock_gettime
#define I_FALLBACK_gettimeofday  gettimeofday
#define I_FALLBACK_getcpu        getcpu_p
#define I_FALLBACK_time          time


#ifdef WITH_VDSO
extern NONNULL(2) void (*I_vdso_clock_gettime)(clockid_t         clk,
                                               struct timespec * ts);
NONNULL(1)
extern int32_t (*I_vdso_gettimeofday)(struct timeval * restrict tv,
                                      void * restrict tz);

extern NONNULL(1)
    int32_t (*I_vdso_getcpu)(unsigned int * cpu, unsigned int * node);
extern NONNULL(1) time_t (*I_vdso_time)(time_t * t);

#else
#define I_vdso_clock_gettime I_FALLBACK_clock_gettime
#define I_vdso_gettimeofday  I_FALLBACK_gettimeofday
#define I_vdso_getcpu        I_FALLBACK_getcpu
#define I_vdso_time          I_FALLBACK_time
#endif


#endif
