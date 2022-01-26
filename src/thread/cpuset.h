#ifndef SRC_D_THREAD_D_CPUSET_H_
#define SRC_D_THREAD_D_CPUSET_H_


#include <sched.h>
#include <stdint.h>

#include "util/error-util.h"
#include "util/inline-math.h"

#include "sys/sys-info.h"

#ifdef I_CPU_SET_OPTIMIZED
typedef uint32_t cpuset_t;
#else
typedef cpu_set_t cpuset_t;
#endif

static void
NONNULL(1) cset_zero(cpuset_t * cset) {
    __builtin_memset(cset, 0, sizeof(cpuset_t));
}

static void
NONNULL(1, 2) cset_copy(cpuset_t * lhs, cpuset_t const * rhs) {
    __builtin_memcpy(lhs, rhs, sizeof(*rhs));
}

static cpuset_t
new_cpuset_zero(void) {
    cpuset_t cset;
    cset_zero(&cset);
    return cset;
}


#ifdef I_CPU_SET_OPTIMIZED

static void
NONNULL(1) cset_all(cpuset_t * cset) {
    uint32_t nprocs = get_num_cpus();
    cpuset_t ones   = TYPE_MAX(cpuset_t);
    ones >>= (-nprocs) % sizeof_bits(cpuset_t);
    *cset = ones;
}

static void
NONNULL(2) cset_set(uint32_t cpu, cpuset_t * cset) {
    *cset |= (1UL << cpu);
}

static void
NONNULL(2) cset_clr(uint32_t cpu, cpuset_t * cset) {
    *cset &= (~(1UL << cpu));
}

static void
NONNULL(1) cset_clr_first(cpuset_t * cset) {
    cpuset_t tmp = *cset;
    tmp &= (tmp - 1);
    *cset = tmp;
}

static uint64_t
NONNULL(2) cset_isset(uint32_t cpu, cpuset_t const * cset) {
    return (*cset & (1UL << cpu));
}

static uint32_t
NONNULL(1) cset_count(cpuset_t const * cset) {
    return popcnt(*cset);
}

static uint32_t
NONNULL(1) cset_empty(cpuset_t const * cset) {
    return (*cset) == 0;
}

static int32_t
NONNULL(1) cset_first(cpuset_t const * cset) {
    return cset_empty(cset) ? -1 : (int32_t)ctz(*cset);
}

static void
NONNULL(1, 2) cset_copy_first(cpuset_t * lhs, cpuset_t const * rhs) {
    cpuset_t tmp = *rhs;
    tmp &= (-tmp);
    *lhs = tmp;
}

static uint32_t
NONNULL(1, 2) cset_equal(cpuset_t const * lhs, cpuset_t const * rhs) {
    return *lhs == *rhs;
}

static void
NONNULL(1, 2)
    cset_or(cpuset_t * dst, cpuset_t const * src0, cpuset_t const * src1) {
    *dst = (*src0) | (*src1);
}

static void
NONNULL(1, 2) cset_or_eq(cpuset_t * dst, cpuset_t const * src) {
    *dst |= *src;
}

static uint32_t
NONNULL(1, 2) cset_test(cpuset_t const * lhs, cpuset_t const * rhs) {
    return (*lhs & *rhs) != 0;
}

static ALWAYS_INLINE CONST_FUNC cpu_set_t const *
NONNULL(1, 2) cset_copy_to_std(cpuset_t const * orig, cpu_set_t * other) {
    const_assert(sizeof(*other) >= sizeof(*orig));
    __builtin_memset(other, 0, sizeof(*other));
    __builtin_memcpy(other, orig, sizeof(*orig));
    return other;
}


#else


# define cset_set(...)   CPU_SET(__VA_ARGS__)
# define cset_isset(...) CPU_ISSET(__VA_ARGS__)
# define cset_count(...) CAST(uint32_t, CPU_COUNT(__VA_ARGS__))
# define cset_equal(...) CPU_EQUAL(__VA_ARGS__)
# define cset_or(...)    CPU_OR(__VA_ARGS__)
# define cset_clr(...)   CPU_CLR(__VA_ARGS__)
# define cset_and(...)   CPU_AND(__VA_ARGS__)


static void
NONNULL(1, 2) cset_or_eq(cpuset_t * dst, cpuset_t const * src) {
    cset_or(dst, dst, src);
}

static int32_t
NONNULL(1) cset_empty(cpuset_t const * cset) {
    cpuset_t zero_set;
    cset_zero(&zero_set);
    return cset_equal(&zero_set, cset);
}

static uint32_t
NONNULL(1, 2) cset_test(cpuset_t const * lhs, cpuset_t const * rhs) {
    cpuset_t tmp;
    cset_and(&tmp, lhs, rhs);
    return !cset_empty(&tmp);
}

static int32_t
NONNULL(1) cset_first(cpuset_t const * cset) {
    uint32_t i;
    uint32_t ncpus = get_num_cpus();

    for (i = 0; i < ncpus; ++i) {
        if (cset_isset(i, cset)) {
            return CAST(int32_t, i);
        }
    }
    return -1;
}

static void
NONNULL(1, 2) cset_copy_first(cpuset_t * lhs, cpuset_t const * rhs) {
    int32_t cpu = cset_first(rhs);
    cset_zero(lhs);
    die_assert(cpu >= 0);
    cset_set(CAST(uint32_t, cpu), lhs);
}

static void
NONNULL(1) cset_clr_first(cpuset_t * cset) {
    int32_t cpu = cset_first(cset);
    die_assert(cpu >= 0);
    cset_clr(CAST(uint32_t, cpu), cset);
}


static void
NONNULL(1) cset_all(cpuset_t * cset) {
    uint32_t nprocs = get_num_cpus();
    uint32_t i;
    cset_zero(cset);
    __builtin_memset(cset, -1, nprocs / CHAR_BITU);
    for (i = ROUNDDOWN_P2(nprocs, CHAR_BITU); i != nprocs; ++i) {
        cset_set(i, cset);
    }
}

static CONST_FUNC ALWAYS_INLINE cpu_set_t const *
NONNULL(1, 2)
    cset_copy_to_std(cpuset_t const * orig, cpu_set_t * other MAYBE_UNUSED) {
    return orig;
}

#endif


NONNULL(1, 3)
char * cset_to_str(char * buf, uint64_t buflen, cpuset_t const * cset);

#endif
