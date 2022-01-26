#ifndef _SRC__ARCH__LL_PAUSE_H_
#define _SRC__ARCH__LL_PAUSE_H_

#ifdef __x86_64__
#include "arch/x86/x86-pause.h"
#else
/* Safe to just leave empty.  */
#define ll_pause()
#endif

#endif
