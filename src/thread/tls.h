#ifndef SRC_D_THREAD_D_TLS_H_
#define SRC_D_THREAD_D_TLS_H_

#include "util/types.h"

#include "arch/ll-tls.h"

static uint8_t *
get_tls_start(void) {
    return CAST(uint8_t *, ll_tls_start());
}


#endif
