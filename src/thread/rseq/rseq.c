#include "thread/rseq/rseq.h"

#if defined(I_HAS_RSEQ) && !defined(I_HAS_GLIBC_RSEQ)
__thread struct rseq I_rseq_area ALIGNED(I_rseq_alignment);
__thread uint32_t                I_rseq_is_init = 0;
#endif
