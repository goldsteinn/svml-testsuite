#include "thread/rseq/rseq-glibc-support.h"
#include "util/error-util.h"

#ifdef I_HAS_GLIBC_RSEQ
static void __attribute__((constructor)) I_rseq_ensure_glibc_prepared() {
    die_assert(rseq_glibc_prepare() == 0, "Unknown GLIBC version");
}
#endif
