#ifndef SRC_BENCH_ARCH_LL_TIMING_H_
#define SRC_BENCH_ARCH_LL_TIMING_H_

#ifdef __x86_64__
#include "arch/x86/x86-timing.h"
#else
#error "Unsupported architecture"
#endif


#endif
