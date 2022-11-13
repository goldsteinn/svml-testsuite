#include <dlfcn.h>

#include "util/error-util.h"
#include "util/vdso-util.h"

#include "util/internal/timers.h"

#ifdef WITH_VDSO
NONNULL(2)
void (*I_vdso_clock_gettime)(clockid_t clk, struct timespec * ts);
NONNULL(1)
int32_t (*I_vdso_gettimeofday)(struct timeval * restrict tv,
                               void * restrict tz);

NONNULL(1)
int32_t (*I_vdso_getcpu)(unsigned int * cpu, unsigned int * node);
NONNULL(1) time_t (*I_vdso_time)(time_t * t);

#endif

#define get_vdso_fallback_func(x) CAT(I_FALLBACK_, x)
#define get_vdso_sym(x)           CAT(__vdso_, x)
#define get_vdso_hook(x)          CAT(I_vdso_, x)

#define set_vdso_to_fallback_func(x)                                           \
    get_vdso_hook(x) = &get_vdso_fallback_func(x)

#define set_vdso_func(x)                                                       \
    ({                                                                         \
        void * fptr = dlsym(vdso_lib, V_TO_STR(get_vdso_sym(x)));              \
        if (fptr != NULL) {                                                    \
            if (!fake) {                                                       \
                get_vdso_hook(x) =                                             \
                    CAST(FUNC_T(get_vdso_fallback_func(x)), fptr);             \
            }                                                                  \
        }                                                                      \
        else {                                                                 \
            if (!fake) {                                                       \
                set_vdso_to_fallback_func(x);                                  \
            }                                                                  \
            ++ret;                                                             \
        }                                                                      \
    });


#define is_vdso_func_fallback(x)                                               \
    (CAST(void *, get_vdso_hook(x)) == CAST(void *, get_vdso_fallback_func(x)))

#define VDSO_FUNCTIONS clock_gettime, gettimeofday, getcpu, time


static uint32_t
I_vdso_init(uint32_t fake) {
    uint32_t ret = 0;
#ifdef WITH_VDSO
    void * vdso_lib;
    vdso_lib = CAST(void *, dlopen("linux-vdso.so.1",
                                   RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD));
    if (vdso_lib == NULL) {
        if (!fake) {
            APPLY(set_vdso_to_fallback_func, ;, VDSO_FUNCTIONS);
        }
        return PP_NARG(VDSO_FUNCTIONS);
    }
    APPLY(set_vdso_func, ;, VDSO_FUNCTIONS);
    dlclose(vdso_lib);
#else
    (void)(fake);
    warn("VDSO is disabled!\n");
#endif
    return ret;
}

static uint32_t
#ifdef WITH_VDSO
    __attribute__((constructor))
#endif
    vdso_init(void) {
    return I_vdso_init(0);
}

uint32_t
vdso_is_full_init() {
    return I_vdso_init(1) == 0;
}
