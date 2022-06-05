#ifndef _SRC__ARCH__LL_PREFETCH_H_
#define _SRC__ARCH__LL_PREFETCH_H_

#ifdef __x86_64__
#include "arch/x86/x86-prefetch.h"
#else

/* Doesn't break anything to skip these. */
#define ll_prefetchw(...)
#define ll_prefetch_L1D(...)
#define ll_prefetch_L2D(...)
#define ll_prefetch_L3D(...)

#define ll_prefetch_L1I(...)

#endif
#endif
