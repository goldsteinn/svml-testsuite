#include <dlfcn.h>

#include "util/error-util.h"
#include "util/vdso-util.h"

#include "util/internal/timers.h"


void const * vdso_funcs[] = { CAST(void const *, &_clock_gettime),
                              CAST(void const *, &gettimeofday),
                              CAST(void const *, &getcpu),
                              CAST(void const *, &time) };


static uint64_t
get_x86_vdso_expec_mask() {
    return (1UL << vdso_clock_gettime_offset) |
           (1UL << vdso_gettimeofday_offset) | (1UL << vdso_getcpu_offset) |
           (1UL << vdso_time_offset);
}

uint64_t
get_vdso_expec_mask() {
#if __x86_64__
    return get_x86_vdso_expec_mask();
#else
    die("Unsupported arch!");
#endif
}


static uint64_t
set_vdso_func(void const * fptr, size_t offset) {
    if (fptr) {
        vdso_funcs[offset] = fptr;
        return 1UL << offset;
    }
    return 0;
}

int32_t
safe_vdso_init() {
    uint64_t ret = vdso_init();
    err_assert(ret != -1UL, "%s\n", dlerror());

    if (ret != get_vdso_expec_mask()) {
        return -1;
    }

    return 0;
}

uint64_t
vdso_init() {
    void *  vdso_lib;
    int32_t ret = 0;
    vdso_lib    = CAST(void *, dlopen("linux-vdso.so.1",
                                   RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD));
    if (vdso_lib == NULL) {
        return -1UL;
    }

    ret |= set_vdso_func(
        CAST(void const *, dlsym(vdso_lib, "__vdso_clock_gettime")),
        vdso_clock_gettime_offset);
    ret |= set_vdso_func(
        CAST(void const *, dlsym(vdso_lib, "__vdso_gettimeofday")),
        vdso_gettimeofday_offset);
    ret |= set_vdso_func(CAST(void const *, dlsym(vdso_lib, "__vdso_getcpu")),
                         vdso_getcpu_offset);
    ret |= set_vdso_func(CAST(void const *, dlsym(vdso_lib, "__vdso_time")),
                         vdso_time_offset);

    dlclose(vdso_lib);

    return ret;
}
