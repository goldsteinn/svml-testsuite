#ifndef SRC_D_THREAD_D_SEMA_H_
#define SRC_D_THREAD_D_SEMA_H_


#include "util/error-util.h"

#include <semaphore.h>
#include <unistd.h>

#define safe_sema_init(sema, n)  I_safe_sema_init(sema, n, I_ERR_ARGS)
#define safe_sema_destroy(sema)  I_safe_sema_destroy(sema, I_ERR_ARGS)
#define safe_sema_wait(sema)     I_safe_sema_wait(sema, I_ERR_ARGS)
#define safe_sema_trywait(sema)  I_safe_sema_trywait(sema, I_ERR_ARGS)
#define safe_sema_post(sema)     I_safe_sema_post(sema, I_ERR_ARGS)
#define safe_sema_getvalue(sema) I_safe_sema_getvalue(sema, I_ERR_ARGS)
#define safe_sema_wait_till_n(sema, n)                                         \
    I_safe_sema_wait_till_n(sema, n, I_ERR_ARGS)

typedef sem_t sema_t;

static void
I_safe_sema_init(sema_t * sema,
                 uint32_t n,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln) {
    if (UNLIKELY(sem_init(sema, 0, n) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static void
I_safe_sema_destroy(sema_t * sema,
                    char const * restrict fn,
                    char const * restrict func,
                    uint32_t ln) {
    if (UNLIKELY(sem_destroy(sema) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static void
I_safe_sema_wait(sema_t * sema,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln) {
    if (UNLIKELY(sem_wait(sema) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}


static void
I_safe_sema_trywait(sema_t * sema,
                    char const * restrict fn,
                    char const * restrict func,
                    uint32_t ln) {
    if (UNLIKELY(sem_trywait(sema) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}


static void
I_safe_sema_post(sema_t * sema,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln) {
    if (UNLIKELY(sem_post(sema) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
}

static uint32_t
I_safe_sema_getvalue(sema_t * sema,
                     char const * restrict fn,
                     char const * restrict func,
                     uint32_t ln) {
    int32_t v;
    if (UNLIKELY(sem_getvalue(sema, &v) != 0)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    die_assert(v >= 0);
    return CAST(uint32_t, v);
}

static void
I_safe_sema_wait_till_n(sema_t * sema,
                        uint32_t n,
                        char const * restrict fn,
                        char const * restrict func,
                        uint32_t ln) {

    while (I_safe_sema_getvalue(sema, fn, func, ln) != n) {
        usleep(1000 * 1000);
    }
}

#endif
