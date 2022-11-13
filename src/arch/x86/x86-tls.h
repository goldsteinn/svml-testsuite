#ifndef SRC_D_ARCH_D_X86_D_X86_TLS_H_
#define SRC_D_ARCH_D_X86_D_X86_TLS_H_


#include <stdint.h>

static uintptr_t
ll_tls_start() {
    uintptr_t p;
    __asm__("mov %%fs:0, %[p]" : [p] "=r"(p) : :);
    return p;
}


#endif
