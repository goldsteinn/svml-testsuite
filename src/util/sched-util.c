#include <sys/syscall.h>
#include <unistd.h>

#include "util/sched-util.h"

/* Use syscalls directly to avoid casting mask to 'cpu_set_t *' and possible
 * violating strict-aliasing. */
static long
_sched_setaffinity(pid_t pid, size_t cpusetsize, cpuset_t * mask) {
    return syscall(SYS_sched_setaffinity, pid, cpusetsize, mask);
}

static long
_sched_getaffinity(pid_t pid, size_t cpusetsize, cpuset_t * mask) {
    return syscall(SYS_sched_getaffinity, pid, cpusetsize, mask);
}

void
_setcpu_aff(pid_t         pid,
            size_t        cpusetsize,
            cpuset_t *    mask,
            const char *  fn,
            const char *  func,
            const int32_t ln) {
    if (UNLIKELY(_sched_setaffinity(pid, cpusetsize, mask))) {
        _errdie(fn, func, ln, errno, NULL);
    }
}


void
_getcpu_aff(pid_t         pid,
            size_t        cpusetsize,
            cpuset_t *    mask,
            const char *  fn,
            const char *  func,
            const int32_t ln) {
    if (UNLIKELY(_sched_getaffinity(pid, cpusetsize, mask))) {
        _errdie(fn, func, ln, errno, NULL);
    }
}

void
setcpu_and_wait(pid_t pid, uint32_t cpu) {
    int nattempts;
    if (cpu >= get_nprocs()) {
        return;
    }

    setcpu(pid, cpu);
    nattempts = 127;
    while (safe_get_cpu() != cpu) {
        safe_yield();
        die_assert(--nattempts, "Setting CPU failed");
    }
}

uint32_t
_safe_get_cpu(const char * fn, const char * func, const int32_t ln) {
    int32_t cpu = get_cpu();
    if (UNLIKELY(cpu < 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return cpu;
}

void
_safe_yield(const char * fn, const char * func, const int32_t ln) {
    if (UNLIKELY(sched_yield())) {
        _errdie(fn, func, ln, errno, NULL);
    }
}
