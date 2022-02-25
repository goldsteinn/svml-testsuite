#ifndef _SRC__UTIL__SCHED_UTIL_H_
#define _SRC__UTIL__SCHED_UTIL_H_

#include <sched.h>
#include <unistd.h>

#include "util/error-util.h"

#include "arch/ll-getcpu.h"
#include "arch/ll-syscall.h"

#include "thread/cpuset.h"

#define setcpu(pid, cpu)      _setcpu(pid, cpu, ERR_ARGS)
#define setcpu_aff(pid, mask) _setcpu_aff(pid, sizeof(cpuset_t), mask, ERR_ARGS)
#define getcpu_aff(pid, mask) _setcpu_aff(pid, sizeof(cpuset_t), mask, ERR_ARGS)

#define proc_setcpu(cpu)      setcpu(0, cpu)
#define proc_setcpu_aff(mask) setcpu_aff(0, mask)
#define proc_getcpu_aff(mask) getcpu_aff(0, mask)

#define safe_get_cpu() _safe_get_cpu(ERR_ARGS)
#define safe_yield()   _safe_yield(ERR_ARGS)

static uint32_t
get_cpu() {
    return _get_cpu();
}

static void
yield() {
    _syscall_cc(SYS_sched_yield, /* None */, /* None */, /* None */,
                /* None */);
}

void NONNULL(1, 2)
    _safe_yield(const char * fn, const char * func, const int32_t ln);

uint32_t NONNULL(1, 2)
    _safe_get_cpu(const char * fn, const char * func, const int32_t ln);


void NONNULL(3, 4, 5) _setcpu_aff(pid_t         pid,
                                  size_t        cpusetsize,
                                  cpuset_t *    mask,
                                  const char *  fn,
                                  const char *  func,
                                  const int32_t ln);

void NONNULL(3, 4, 5) _getcpu_aff(pid_t         pid,
                                  size_t        cpusetsize,
                                  cpuset_t *    mask,
                                  const char *  fn,
                                  const char *  func,
                                  const int32_t ln);


void setcpu_and_wait(pid_t pid, uint32_t cpu);

static void
NONNULL(3, 4) _setcpu(pid_t         pid,
                      uint32_t      cpu,
                      const char *  fn,
                      const char *  func,
                      const int32_t ln) {
    cpuset_t cset;
    cset_zero(&cset);
    cset_set(cpu, &cset);
    _setcpu_aff(pid, sizeof(cpu_set_t), &cset, fn, func, ln);
}


static void
proc_setcpu_and_wait(uint32_t cpu) {
    setcpu_and_wait(0, cpu);
}

static uint32_t
get_nprocs() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

#endif
