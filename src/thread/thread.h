#ifndef SRC_D_THREAD_D_THREAD_H_
#define SRC_D_THREAD_D_THREAD_H_

#ifndef WITH_THREAD
# error "Using thread library with threading disabled!"
#endif

#include <pthread.h>

#include "util/common.h"
#include "util/error-util.h"
#include "util/macro.h"

#include "thread/cpuset.h"

typedef pthread_t      thread_t;
typedef pthread_attr_t thread_attr_t;

enum { k_thread_default_stack_size = 32768 };

typedef void * (*I_thread_func)(void *);

#define safe_thread_create(tid, attr, func, arg)                               \
    I_safe_thread_create(tid, attr, func, arg, I_ERR_ARGS)
#define safe_thread_join(tid, attr) I_safe_thread_join(tid, attr, I_ERR_ARGS)
#define safe_thread_detach(tid)     I_safe_thread_detach(tid, I_ERR_ARGS)
#define safe_thread_detach_self()   I_safe_thread_detach_self(I_ERR_ARGS)
#define safe_thread_attr_init(attr)                                            \
    I_safe_thread_attr_init(attr, I_ERR_ARGS);                                 \
    safe_thread_attr_set_stacksize(attr, k_thread_default_stack_size);
#define safe_thread_attr_destroy(attr)                                         \
    I_safe_thread_attr_destroy(attr, I_ERR_ARGS)
#define safe_thread_attr_set_stacksize(attr, stacksize)                        \
    I_safe_thread_attr_set_stacksize(attr, stacksize, I_ERR_ARGS)

#define safe_thread_attr_set_affinity(attr, cpuset)                            \
    I_safe_thread_attr_set_affinity(attr, cpuset, I_ERR_ARGS)
#define safe_thread_attr_set_cpu(attr, cpu)                                    \
    I_safe_thread_attr_set_cpu(attr, cpu, I_ERR_ARGS)


static NONNULL(1, 3) void I_safe_thread_create(thread_t * restrict tid,
                                               thread_attr_t * restrict attr,
                                               I_thread_func thread_func,
                                               void * restrict arg,
                                               char const * restrict fn,
                                               char const * restrict func,
                                               uint32_t ln) {
    if (UNLIKELY(pthread_create(tid, attr, thread_func, arg))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static void
I_safe_thread_join(thread_t tid,
                   void ** restrict retval,
                   char const * restrict fn,
                   char const * restrict func,
                   uint32_t ln) {
    if (UNLIKELY(pthread_join(tid, retval))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static thread_t
thread_self(void) {
    return pthread_self();
}

static void
I_safe_thread_detach_self(char const * restrict fn,
                          char const * restrict func,
                          uint32_t ln) {
    if (UNLIKELY(pthread_detach(thread_self()))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static void
I_safe_thread_detach(thread_t tid,
                     char const * restrict fn,
                     char const * restrict func,
                     uint32_t ln) {
    if (UNLIKELY(pthread_detach(tid))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static NONNULL(1) void I_safe_thread_attr_init(thread_attr_t * restrict attr,
                                               char const * restrict fn,
                                               char const * restrict func,
                                               uint32_t ln) {
    if (UNLIKELY(pthread_attr_init(attr))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static NONNULL(1) void I_safe_thread_attr_destroy(thread_attr_t * restrict attr,
                                                  char const * restrict fn,
                                                  char const * restrict func,
                                                  uint32_t ln) {
    if (UNLIKELY(pthread_attr_destroy(attr))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}


static NONNULL(1) void I_safe_thread_attr_set_stacksize(
    thread_attr_t * restrict attr,
    uint32_t stacksize,
    char const * restrict fn,
    char const * restrict func,
    uint32_t ln) {
    if (UNLIKELY(pthread_attr_setstacksize(attr, stacksize))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}


static NONNULL(1) void I_safe_thread_attr_set_affinity(
    thread_attr_t * restrict attr,
    cpuset_t const * restrict cpu_set,
    char const * restrict fn,
    char const * restrict func,
    uint32_t ln) {
    cpu_set_t pass_cset;
    if (UNLIKELY(pthread_attr_setaffinity_np(
            attr, sizeof(cpu_set_t), cset_copy_to_std(cpu_set, &pass_cset)))) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static NONNULL(1) void I_safe_thread_attr_set_cpu(thread_attr_t * restrict attr,
                                                  uint32_t cpu,
                                                  char const * restrict fn,
                                                  char const * restrict func,
                                                  uint32_t ln) {
    cpuset_t cset;
    cset_zero(&cset);
    cset_set(cpu, &cset);
    I_safe_thread_attr_set_affinity(attr, &cset, fn, func, ln);
}


#endif
