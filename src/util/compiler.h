#ifndef SRC_D_UTIL_D_COMPILER_H_
#define SRC_D_UTIL_D_COMPILER_H_

#define compiler_barrier() __asm__ volatile("" : : : "memory")
#define compiler_do_not_optimize_out(x)                                        \
    __asm__ volatile("" : : "r,m,v"(x) : "memory")
#define fence_do_not_optimize_out(x)                                           \
    __asm__ volatile("lfence" : : "r,m,v"(x) : "memory")
#define compiler_code_align(x) __asm__ volatile(".align " V_TO_STR(x) "" : : :);


#define lw_compiler_barrier()              __asm__ volatile("" : : :)
#define lw_compiler_do_not_optimize_out(x) __asm__ volatile("" : : "r,m,v"(x) :)
#define lw_fence_do_not_optimize_out(x)                                        \
    __asm__ volatile("lfence" : : "r,m,v"(x) :)


#endif
