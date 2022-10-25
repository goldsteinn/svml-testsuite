#ifndef _SRC__ARCH__X86__X86_GETCPU_H_
#define _SRC__ARCH__X86__X86_GETCPU_H_

#ifdef __RDPID__

#define ll_cpustate  __builtin_ia32_rdpid

#else

static uint32_t
ll_cpustate_impl() {
    uint32_t v;
    __builtin_ia32_rdtscp(&v);
    return v;
}

#define ll_cpustate ll_cpustate_impl

#endif


#endif
