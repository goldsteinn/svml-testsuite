#include "thread/rseq/rseq.h"
#include "thread/barrier.h"
#include "thread/cpuset.h"
#include "thread/thread.h"

#include "util/memory-util.h"
#include "util/sched-util.h"


#include "test/test-common.h"

static void NEVER_INLINE
incr_counter_rseq(
    uint32_t * counters /* NOLINT(readability-non-const-parameter) */
) {
    uint64_t cpu = 0;
    __asm__ volatile(RSEQ_AREA_DEF()
                 RSEQ_CS_ARR_DEF()
                 RSEQ_PREP_CS_DEF(%[cpu])

                 RSEQ_START_ABORT_DEF()
                 "jmp 1f\n"
                 RSEQ_END_ABORT_DEF()
                 "1:\n\t"
                 "movl  " RSEQ_CUR_CPU() ", %k[cpu]\n\t"
                 "sall  $6, %k[cpu]\n\t"
                 "incl  (%[counters], %[cpu])\n\t"
                 "2:\n\t"
                     : [cpu]"=&r"(cpu), //"+m" (* (uint32_t (*)[]) counters)
                       "+m" (* counters)
                 : [counters] "r" (counters)
                 : "cc");
}

static void NEVER_INLINE
incr_counter_norm(
    uint32_t * volatile counters /* NOLINT(readability-non-const-parameter) */
) {
    counters[(64 / sizeof(*counters)) * get_cpu()] += 1;
}

static uint32_t * global_counters;

static thread_barrier_t /* NOLINT(fuchsia-statically-constructed-objects) */
    barrier /* NOLINT(fuchsia-statically-constructed-objects) */;

enum { NITER = 100 * 1000 * 1000, DO_RSEQ_INCR = 0 };

static void *
incr_loop(void * arg) {
    uint32_t i, todo;
    todo = CAST(uint32_t, CAST(uint64_t, arg));
    die_assert(rseq_init() == 0);
    thread_barrier_wait(&barrier);
    if (todo == DO_RSEQ_INCR) {
        for (i = NITER; i; --i) {
            incr_counter_rseq(global_counters);
            __asm__ volatile("" : : :);
        }
    }
    else {
        for (i = NITER; i; --i) {
            incr_counter_norm(global_counters);
            __asm__ volatile("" : : :);
        }
    }

    return NULL;
}

static int32_t
test_rseq_corr(void) {
    enum { THREAD_SCALE = 8 };
    uint32_t      i, j, k;
    uint32_t      nprocs = get_num_cpus();
    thread_t      tids[nprocs * THREAD_SCALE];
    thread_attr_t attr;
    cpuset_t      cset;
    int32_t       good_test = 0;

    global_counters = (uint32_t *)safe_calloc(nprocs, 64);
    safe_thread_attr_init(&attr);
    safe_thread_attr_set_stacksize(&attr, 32768);

    cset_zero(&cset);
    /* We want to force a fair amount of contention so we couldn't
     * reasonably luck into correctness. */
    for (i = 0; i < nprocs / 2; ++i) {
        cset_set(i, &cset);
    }
    safe_thread_attr_set_affinity(&attr, &cset);


    for (i = 1; i < nprocs * THREAD_SCALE; i *= 2) {
        for (k = 0; k < 2; ++k) {
            uint32_t sum;
            void *   targ;

            memset_c(global_counters, 0, CAST(size_t, nprocs * 64));
            safe_thread_barrier_init(&barrier, NULL, i);

            targ = (void *)(uint64_t)k; /* NOLINT(performance-no-int-to-ptr) */
            for (j = 0; j < i; ++j) {
                safe_thread_create(tids + j, &attr, &incr_loop, targ);
            }
            for (j = 0; j < i; ++j) {
                pthread_join(tids[j], NULL);
            }


            sum = 0;
            for (j = 0; j < nprocs; ++j) {
                sum += global_counters[j * (64 / sizeof(*global_counters))];
            }

            if (k == DO_RSEQ_INCR) {
                test_assert(NITER * i == sum, "(%u): %u vs %u\n", i, NITER * i,
                            sum);
            }
            else {
                if (NITER * i != sum) {
                    ++good_test;
                }
            }
        }
    }

    warn_assert(
        good_test > 1,
        "counter has been correct even without rseq. Test may be invalid!\n");

    return 0;
}

static int32_t
test_rseq_init(void) {
    test_assert(rseq_init() == 0);

    test_assert(rseq_is_init());
    test_assert(rseq_init() == 0);
    test_assert(rseq_is_init());
    return 0;
}

int32_t test_rseq(void);
int32_t
test_rseq(void) {
    test_assert(test_rseq_init() == 0);
    test_assert(test_rseq_corr() == 0);
    return 0;
}
