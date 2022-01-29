#ifndef _SRC__UTIL__VERBOSITY_H_
#define _SRC__UTIL__VERBOSITY_H_


#include <stdint.h>
#include <stdio.h>

#include "util/common.h"

#define vprint(...)   vfprint(stdout, __VA_ARGS__)
#define vvprint(...)  vvfprint(stdout, __VA_ARGS__)
#define vvvprint(...) vvvfprint(stdout, __VA_ARGS__)

#define vfprint(fp, ...)   vnfprint(1, fp, __VA_ARGS__)
#define vvfprint(fp, ...)  vnfprint(2, fp, __VA_ARGS__)
#define vvvfprint(fp, ...) vnfprint(3, fp, __VA_ARGS__)

#define vprint_if(cond, ...)   vfprint_if(cond, stdout, __VA_ARGS__)
#define vvprint_if(cond, ...)  vvfprint_if(cond, stdout, __VA_ARGS__)
#define vvvprint_if(cond, ...) vvvfprint_if(cond, stdout, __VA_ARGS__)

#define vfprint_if(cond, fp, ...)   vnfprint_if(cond, 1, fp, __VA_ARGS__)
#define vvfprint_if(cond, fp, ...)  vnfprint_if(cond, 2, fp, __VA_ARGS__)
#define vvvfprint_if(cond, fp, ...) vnfprint_if(cond, 3, fp, __VA_ARGS__)

#define vnprint(n, ...)      vnfprint(n, stdout, __VA_ARGS__)
#define vnfprint(n, fp, ...) vcheck_print(n, fp, __VA_ARGS__)

#define vnfprint_if(cond, n, fp, ...)                                          \
    if (check_verbosity(n) && (cond)) {                                        \
        fprintf(fp, __VA_ARGS__);                                              \
    }

int32_t      check_verbosity(int32_t verbosity_bound);
void         set_verbosity(int32_t verbosity);
NEVER_INLINE NONNULL(2, 3) void vcheck_print(int32_t      verbosity_bound,
                                             FILE *       fp,
                                             const char * msg,
                                             ...);


#endif
