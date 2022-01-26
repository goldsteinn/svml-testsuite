#ifndef SRC_D_THREAD_D_RSEQ_D_RSEQ_SIGNATURE_H_
#define SRC_D_THREAD_D_RSEQ_D_RSEQ_SIGNATURE_H_


#ifdef __x86_64__
#include "thread/rseq/x86/x86-rseq-arch.h"
#else
#error "Unsupported architecture"
#endif


#endif
