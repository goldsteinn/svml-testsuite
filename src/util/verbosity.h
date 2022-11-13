#ifndef SRC_UTIL_VERBOSITY_H_
#define SRC_UTIL_VERBOSITY_H_


#include <stdint.h>
#include <stdio.h>

#include "util/common.h"


#define print(...)    fprint(stdout, __VA_ARGS__)
#define vprint(...)   vfprint(stdout, __VA_ARGS__)
#define vvprint(...)  vvfprint(stdout, __VA_ARGS__)
#define vvvprint(...) vvvfprint(stdout, __VA_ARGS__)

#define printe(...)    fprint(stderr, __VA_ARGS__)
#define vprinte(...)   vfprint(stderr, __VA_ARGS__)
#define vvprinte(...)  vvfprint(stderr, __VA_ARGS__)
#define vvvprinte(...) vvvfprint(stderr, __VA_ARGS__)


#define I_print_var_impl(fmt, var)                                             \
    print(fmt, __func__, __LINE__, V_TO_STR(var), GET_TYPENAME(var), var)
#define I_print_var_2(m, fmt, var)                                             \
    I_print_var_impl("%-16s:%-5u:%-20s (%-12s) ->" fmt "\n", var)
#define I_print_var_1(m, var)                                                  \
    I_print_var_impl(m("%-16s:%-5u:%-20s (%-12s) -> ", var, "\n"), var)

#define print_var(...)                                                         \
    CAT(I_print_var_, PP_NARG(__VA_ARGS__))(DEFAULT_FMT, __VA_ARGS__)
#define print_varx(...)                                                        \
    CAT(I_print_var_, PP_NARG(__VA_ARGS__))(DEFAULT_HEX_FMT, __VA_ARGS__)

#define trace_print(msg, ...)                                                  \
    fprint(stderr, "%-20s:%-6u: " msg, I__FILENAME__, __LINE__, ##__VA_ARGS__)
#define trace_vprint(msg, ...)                                                 \
    vfprint(stderr, "%-20s:%-6u: " msg, I__FILENAME__, __LINE__, ##__VA_ARGS__)
#define trace_vvprint(msg, ...)                                                \
    vvfprint(stderr, "%-20s:%-6u: " msg, I__FILENAME__, __LINE__, ##__VA_ARGS__)
#define trace_vvvprint(msg, ...)                                               \
    vvvfprint(stderr, "%-20s:%-6u: " msg, I__FILENAME__, __LINE__,             \
              ##__VA_ARGS__)

#define fprint(fp, ...)    vnfprint(0, fp, __VA_ARGS__)
#define vfprint(fp, ...)   vnfprint(1, fp, __VA_ARGS__)
#define vvfprint(fp, ...)  vnfprint(2, fp, __VA_ARGS__)
#define vvvfprint(fp, ...) vnfprint(3, fp, __VA_ARGS__)

#define vprint_if(cond, ...)   vfprint_if(cond, stdout, __VA_ARGS__)
#define vvprint_if(cond, ...)  vvfprint_if(cond, stdout, __VA_ARGS__)
#define vvvprint_if(cond, ...) vvvfprint_if(cond, stdout, __VA_ARGS__)

#define vfprint_if(cond, fp, ...)   vnfprint_if(cond, 1, fp, __VA_ARGS__)
#define vvfprint_if(cond, fp, ...)  vnfprint_if(cond, 2, fp, __VA_ARGS__)
#define vvvfprint_if(cond, fp, ...) vnfprint_if(cond, 3, fp, __VA_ARGS__)

#define vnprint(n, ...) vnfprint(n, stdout, __VA_ARGS__)

#define vnfprint_if(cond, n, fp, ...)                                          \
    if (check_verbosity(n) && (cond)) {                                        \
        vnfprint(n, fp, __VA_ARGS__);                                          \
    }

#ifdef SILENCE_ALL
# define vnfprint(...)
#else
# define vnfprint(n, fp, ...) vcheck_print(n, fp, __VA_ARGS__)
#endif

int32_t PURE_FUNC get_verbosity(void);
int32_t PURE_FUNC check_verbosity(int32_t verbosity_bound);
void              set_verbosity(int32_t verbosity);

NEVER_INLINE NONNULL(2, 3)
    FORMATF(3, 4) void vcheck_print(int32_t      verbosity_bound,
                                    FILE *       fp,
                                    const char * msg,
                                    ...);


#endif
