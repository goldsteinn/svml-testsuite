#include "util/sys-util.h"


void
_setcpu(pid_t         pid,
        uint32_t      cpu,
        const char *  fn,
        const char *  func,
        const int32_t ln) {
    cpu_set_t cset;
    CPU_ZERO(&cset);
    CPU_SET(cpu, &cset);
    return _setcpu_aff(pid, sizeof(cpu_set_t), &cset, fn, func, ln);
}


void
_setcpu_aff(pid_t         pid,
            size_t        cpusetsize,
            cpu_set_t *   mask,
            const char *  fn,
            const char *  func,
            const int32_t ln) {
    if (UNLIKELY(sched_setaffinity(pid, cpusetsize, mask))) {
        _errdie(fn, func, ln, errno, NULL);
    }
}


void
_getcpu_aff(pid_t         pid,
            size_t        cpusetsize,
            cpu_set_t *   mask,
            const char *  fn,
            const char *  func,
            const int32_t ln) {
    if (UNLIKELY(sched_getaffinity(pid, cpusetsize, mask))) {
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
    uint32_t cpu = get_cpu();
    if (UNLIKELY(cpu)) {
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
