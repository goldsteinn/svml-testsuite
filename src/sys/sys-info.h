#ifndef _SRC__SYS__SYS_INFO_H_
#define _SRC__SYS__SYS_INFO_H_

#include <sys/sysinfo.h>

#include "util/common.h"

static PURE_FUNC uint32_t
get_num_cpus() {
    return get_nprocs_conf();
}

#endif
