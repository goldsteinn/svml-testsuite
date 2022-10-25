#ifndef _SRC__ARCH__LL_TLS_H_
#define _SRC__ARCH__LL_TLS_H_

#include <features.h>

#if __GNUC_PREREQ(11, 1)
static uintptr_t
ll_tls_start() {
    return (uintptr_t)__builtin_thread_pointer();
}
#elif defined(__x86_64__)
#include "arch/x86/x86-tls.h"
#else
#error "Unsupported arch"
#endif


#endif
