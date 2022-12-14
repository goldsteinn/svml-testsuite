#include <sys/syscall.h>
#include <unistd.h>

#include "util/sched-util.h"


/* Use syscalls directly to avoid casting mask to 'cpu_set_t *' and
 * possible violating strict-aliasing. */
static int64_t
I_sched_setaffinity(pid_t pid, size_t cpusetsize, cpuset_t * mask) {
    return syscall(SYS_sched_setaffinity, pid, cpusetsize, mask);
}

static int64_t
I_sched_getaffinity(pid_t pid, size_t cpusetsize, cpuset_t * mask) {
    return syscall(SYS_sched_getaffinity, pid, cpusetsize, mask);
}

void
I_setcpu_aff(pid_t        pid,
             size_t       cpusetsize,
             cpuset_t *   mask,
             const char * fn,
             const char * func,
             uint32_t     ln) {
    if (UNLIKELY(I_sched_setaffinity(pid, cpusetsize, mask))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}


void
I_getcpu_aff(pid_t        pid,
             size_t       cpusetsize,
             cpuset_t *   mask,
             const char * fn,
             const char * func,
             uint32_t     ln) {
    if (UNLIKELY(I_sched_getaffinity(pid, cpusetsize, mask))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

void
setcpu_and_wait(pid_t pid, uint32_t cpu) {
    int32_t nattempts;
    enum { IMM8_MAX_MINUS_ONE = 127 };
    if (cpu >= get_num_cpus()) {
        return;
    }

    setcpu(pid, cpu);
    nattempts = IMM8_MAX_MINUS_ONE;
    while (safe_get_cpu() != cpu) {
        safe_yield();
        die_assert(--nattempts, "Setting CPU failed");
    }
}

uint32_t
I_safe_get_cpu(const char * fn, const char * func, uint32_t ln) {
    int32_t cpu = CAST(int32_t, get_cpu());
    if (UNLIKELY(cpu < 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return CAST(uint32_t, cpu);
}

void
I_safe_yield(const char * fn, const char * func, uint32_t ln) {
    if (UNLIKELY(sched_yield())) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}
