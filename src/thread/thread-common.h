#ifndef _SRC__THREAD__THREAD_COMMON_H_
#define _SRC__THREAD__THREAD_COMMON_H_

#include "thread/cpuset.h"

#ifdef WITH_THREAD
#include "thread/barrier.h"
#include "thread/mutex.h"
#include "thread/spinlock.h"
#include "thread/thread.h"
#else
#warning                                                                       \
    "Including thread headers with threading disabled. Maybe rebuild with 'WITH_THREAD'"
#endif

#endif
