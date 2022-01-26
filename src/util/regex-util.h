#ifndef SRC_D_UTIL_D_REGEX_UTIL_H_
#define SRC_D_UTIL_D_REGEX_UTIL_H_

#include <regex.h>

#include "util/common.h"

#define safe_re_compile(re, pattern, cflags)                                   \
    I_safe_re_compile(re, pattern, cflags, I_ERR_ARGS)

#define safe_re_exec(re, str, nmatch, pmatch, eflags)                          \
    I_safe_re_exec(re, str, nmatch, pmatch, eflags, I_ERR_ARGS)

#define re_errdie(ret, re) I_re_errdie(ret, re, I_ERR_ARGS)


NONNULL(1, 2)
static int32_t
re_compile(regex_t * restrict re,
           const char * restrict pattern,
           int32_t flags) {
    return regcomp(re, pattern, flags);
}

NONNULL(1, 2, 4)
static int32_t
re_exec(regex_t const * restrict re,
        char const * restrict str,
        uint64_t   nmatch,
        regmatch_t pmatch[I_arr_restrict],
        int32_t    eflags) {
    return regexec(re, str, nmatch, pmatch, eflags);
}

NONNULL(1) static void re_free(regex_t * re) {
    regfree(re);
}


EXIT_FUNC NONNULL(2, 3, 4) void I_re_errdie(int32_t errcode,
                                            regex_t const * restrict re,
                                            char const * restrict file_name,
                                            char const * restrict func_name,
                                            uint32_t line_num);

NONNULL(1, 2, 4, 5)
int32_t I_safe_re_compile(regex_t * restrict re,
                          char const * restrict pattern,
                          int32_t cflags,
                          char const * restrict file_name,
                          char const * restrict func_name,
                          uint32_t line_num);

NONNULL(1, 2, 6, 7)
int32_t I_safe_re_exec(regex_t const * restrict re,
                       char const * restrict str,
                       uint64_t   nmatch,
                       regmatch_t pmatch[I_arr_restrict],
                       int        eflags,
                       char const * restrict file_name,
                       char const * restrict func_name,
                       uint32_t line_num);


#endif
