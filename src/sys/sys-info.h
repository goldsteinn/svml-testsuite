#ifndef SRC_D_SYS_D_SYS_INFO_H_
#define SRC_D_SYS_D_SYS_INFO_H_

#include <sys/sysinfo.h>

#include "util/common.h"

static PURE_FUNC uint32_t
get_num_cpus(void) {
    int32_t res = get_nprocs_conf();
    GURANTEED(res > 0);
    return CAST(uint32_t, res);
}

#endif
