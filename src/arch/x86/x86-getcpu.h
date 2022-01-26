#ifndef SRC_D_ARCH_D_X86_D_X86_GETCPU_H_
#define SRC_D_ARCH_D_X86_D_X86_GETCPU_H_

#ifdef __RDPID__

# define ll_cpustate __builtin_ia32_rdpid

#else

static cpu_state_t
ll_cpustate_impl() {
    uint32_t v;
    __builtin_ia32_rdtscp(&v);
    return v;
}

# define ll_cpustate ll_cpustate_impl

#endif


#endif
