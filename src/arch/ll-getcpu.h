#ifndef _SRC__ARCH__LL_GETCPU_H_
#define _SRC__ARCH__LL_GETCPU_H_

#ifndef CPUSTATE_CPU_MASK
#define CPUSTATE_CPU_MASK 0xfff
#endif

#ifndef CPUSTATE_NODE_SHIFT
#define CPUSTATE_NODE_SHIFT 12
#endif


#if (defined __x86_64__)
#include "arch/x86/x86-getcpu.h"
#else
#include "util/getcpu-portable.h"
static uint32_t
ll_cpustate_impl() {
    uint32_t cpu, node;
    getcpu_p(&cpu, &node);
    return cpu | (node << CPUSTATE_NODE_SHIFT);
}

#define ll_cpustate ll_cpustate_impl

#endif

#define ll_getcpu()  (ll_cpustate() & CPUSTATE_CPU_MASK)
#define ll_getnode() (ll_cpustate() >> CPUSTATE_NODE_SHIFT)
#endif
