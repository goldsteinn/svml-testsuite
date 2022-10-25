#ifndef _SRC__ARCH__X86__X86_TLS_H_
#define _SRC__ARCH__X86__X86_TLS_H_


#include <stdint.h>

static uintptr_t
ll_tls_start() {
    uintptr_t p;
    __asm__("mov %%fs:0, %[p]" : [p] "=r"(p) : :);
    return p;
}


#endif
