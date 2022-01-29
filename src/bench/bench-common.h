#ifndef _SRC__BENCH__BENCH_COMMON_H_
#define _SRC__BENCH__BENCH_COMMON_H_

#include "bench/timing.h"

#include "util/attrs.h"
#include "util/common.h"
#include "util/types.h"

#define compiler_barrier() asm volatile("" : : : "memory")
#define compiler_do_not_optimize_out(x)                                        \
    asm volatile("" : : "r,m,v"(x) : "memory")
#define compiler_code_align(x) asm volatile(".align " V_TO_STR(x) "" : : :);
#endif
