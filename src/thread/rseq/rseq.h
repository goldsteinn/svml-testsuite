#ifndef _SRC__THREAD__RSEQ__RSEQ_H_
#define _SRC__THREAD__RSEQ__RSEQ_H_


#include "thread/internal/thread-internal-common.h"
#include "thread/rseq/rseq-defs.h"
#include "thread/rseq/rseq-portable.h"

#include "arch/ll-syscall.h"

#include "util/common.h"
#include "util/error-util.h"
#include "util/types.h"


#ifdef I_HAS_RSEQ
#define HAS_RSEQ 1

#ifdef I_HAS_GLIBC_RSEQ


static uint32_t
rseq_is_init() {
    return 1;
}

static int32_t
rseq_init() {
    return 0;
}

static struct rseq *
rseq_get_area() {
    return (struct rseq *)(ll_tls_start() + RSEQ_GLIBC_OFFSET);
}

#else
extern __thread struct rseq I_rseq_area;
extern __thread uint32_t    I_rseq_is_init;


static void
I_rseq_set_is_init() {
    I_rseq_is_init = 1;
}

static int32_t
rseq_init_first() {
    int32_t ret;

    rseq_glibc_prepare();
    ret = ll_syscall_cc(I_NR_rseq,
                        (&I_rseq_area, sizeof(I_rseq_area), 0, I_RSEQ_SIG),
                        /* no +m */, /* no m */,
                        /* no =m */);
    if (LIKELY(ret == 0)) {
        I_rseq_set_is_init();
    }
    else {
        rseq_is_supported(ret);
    }
    return ret;
}

static uint32_t
rseq_is_init() {
    return I_rseq_is_init;
}

static int32_t
rseq_init() {
    if (LIKELY(rseq_is_init())) {
        return 0;
    }

    return rseq_init_first();
}

static struct rseq *
rseq_get_area() {
    return &I_rseq_area;
}
#endif


static uint32_t
rseq_getcpu() {
    return READ_ONCE(rseq_get_area()->cpu_id);
}


#else

#define HAS_RSEQ 0
static uint32_t
rseq_is_init() {
    return 0;
}

static void
I_rseq_set_is_init() {
    /* nop. */
}

static int32_t
rseq_init_first() {
    return 0;
}

static int32_t
rseq_init() {
    return 0;
}


static uint32_t
rseq_get_cpu() {
    die("Unimplemented");
}

#endif


#endif
