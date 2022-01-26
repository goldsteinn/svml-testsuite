#ifndef SRC_D_THREAD_D_RSEQ_D_RSEQ_GLIBC_SUPPORT_H_
#define SRC_D_THREAD_D_RSEQ_D_RSEQ_GLIBC_SUPPORT_H_


#include "thread/rseq/rseq-linux-support.h"

#ifdef I_HAS_RSEQ

#include "thread/rseq/rseq-defs.h"
#include "thread/tls.h"

#include "util/common.h"
#include "util/types.h"

#include "arch/ll-syscall.h"

#if check_has_include("sys/rseq.h")
#define I_HAS_GLIBC_RSEQ
#include <sys/rseq.h>
#elif __GLIBC_PREREQ(2, 35) && defined(_GNU_SOURCE)
#define I_HAS_GLIBC_RSEQ
/*
 * As of GLIBC 2.35 rseq is supported and pre-initialized at TLS
 * startup. We are going to disable it for now so we can just use
 * I_rseq_area as a shared resource.
 */
extern const ptrdiff_t    __rseq_offset;
extern const unsigned int __rseq_size;
#endif

#ifdef I_HAS_GLIBC_RSEQ

#if __GLIBC_PREREQ(2, 35)
#define RSEQ_GLIBC_OFFSET 2464
#else
#error "Unknown rseq offset"
#endif

const_assert(RSEQ_GLIBC_OFFSET <= UINT_MAX);

static uint32_t
I_get_glibc_rseq_size(void) {
    return __rseq_size;
}
static uint32_t
I_get_glibc_rseq_offset(void) {
    return CAST(uint32_t, __rseq_offset);
}
static struct rseq *
I_get_glibc_rseq_area(void) {
    return CAST(struct rseq *, get_tls_start() + I_get_glibc_rseq_offset());
}

static uint32_t
rseq_glibc_prepare(void) {
    return (I_get_glibc_rseq_size() == sizeof(struct rseq) &&
            I_get_glibc_rseq_offset() == RSEQ_GLIBC_OFFSET)
               ? 0
               : 1;
}
#else
static uint32_t
rseq_glibc_prepare(void) {
    return 0;
}
#endif
#endif

#endif
