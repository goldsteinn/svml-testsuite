#ifndef SRC_D_ARCH_D_X86_D_X86_PREFETCH_H_
#define SRC_D_ARCH_D_X86_D_X86_PREFETCH_H_

#include "util/attrs.h"

#include <x86intrin.h>

#define ll_prefetchw(ptr) _mm_prefetch((void const *)(ptr), _MM_HINT_ET1);


#define ll_prefetch_L1D(ptr) _mm_prefetch((void const *)(ptr), _MM_HINT_T0);


#define ll_prefetch_L2D(ptr) _mm_prefetch((void const *)(ptr), _MM_HINT_T1);


#define ll_prefetch_L3D(ptr) _mm_prefetch((void const *)(ptr), _MM_HINT_T2);


#define ll_prefetch_L1I(...)

#endif
