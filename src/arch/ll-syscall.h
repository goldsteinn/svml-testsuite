#ifndef _SRC__ARCH__LL_SYSCALL_H_
#define _SRC__ARCH__LL_SYSCALL_H_

#include <sys/syscall.h>

#ifdef __x86_64__
#include "arch/x86/x86-syscall.h"
#else
#include <unistd.h>
#define _syscall            syscall
#define _syscall_cc(x, ...) _syscall(DEPAREN(x))
#endif


#endif
