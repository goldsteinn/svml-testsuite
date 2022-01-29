#ifndef _SRC__UTIL__SYS_UTIL_H_
#define _SRC__UTIL__SYS_UTIL_H_

#include <sched.h>
#include <unistd.h>

#include "util/error-util.h"

#define setcpu(pid, cpu) _setcpu(pid, cpu, __FILENAME__, __func__, __LINE__)
#define setcpu_aff(pid, mask)                                                  \
    _setcpu_aff(pid, sizeof(cpu_set_t), mask, __FILENAME__, __func__, __LINE__)
#define getcpu_aff(pid, mask)                                                  \
    _setcpu_aff(pid, sizeof(cpu_set_t), mask, __FILENAME__, __func__, __LINE__)

#define proc_setcpu(cpu)      setcpu(0, cpu)
#define proc_setcpu_aff(mask) setcpu_aff(0, mask)
#define proc_getcpu_aff(mask) getcpu_aff(0, mask)

#define safe_get_cpu() _safe_get_cpu(__FILENAME__, __func__, __LINE__)
#define safe_yield()   _safe_yield(__FILENAME__, __func__, __LINE__)

static uint32_t
get_cpu() {
    return sched_getcpu();
}

static void
yield() {
    sched_yield();
}

void NONNULL(1, 2)
    _safe_yield(const char * fn, const char * func, const int32_t ln);

uint32_t NONNULL(1, 2)
    _safe_get_cpu(const char * fn, const char * func, const int32_t ln);

void NONNULL(3, 4) _setcpu(pid_t         pid,
                           uint32_t      cpu,
                           const char *  fn,
                           const char *  func,
                           const int32_t ln);

void NONNULL(3, 4, 5) _setcpu_aff(pid_t         pid,
                                  size_t        cpusetsize,
                                  cpu_set_t *   mask,
                                  const char *  fn,
                                  const char *  func,
                                  const int32_t ln);

void NONNULL(3, 4, 5) _getcpu_aff(pid_t         pid,
                                  size_t        cpusetsize,
                                  cpu_set_t *   mask,
                                  const char *  fn,
                                  const char *  func,
                                  const int32_t ln);


void setcpu_and_wait(pid_t pid, uint32_t cpu);

static void
proc_setcpu_and_wait(uint32_t cpu) {
    setcpu_and_wait(0, cpu);
}

static uint32_t
get_nprocs() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

#endif
