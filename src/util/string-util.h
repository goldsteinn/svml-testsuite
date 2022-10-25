#ifndef _SRC__UTIL__STRING_UTIL_H_
#define _SRC__UTIL__STRING_UTIL_H_

#include "util/common.h"
#include "util/macro.h"

#define safe_snprintf(dst, n, fmt, ...)                                        \
    I_safe_snprintf(dst, n, ERR_ARGS, fmt, __VA_ARGS__)

#define safe_bprintf(dst, dst_end, fmt, ...)                                   \
    I_safe_bprintf(dst, dst_end, ERR_ARGS, fmt, __VA_ARGS__)

static NONNULL(1, 3, 4, 6) FORMATF(6, 7) uint64_t
    I_safe_snprintf(char * restrict dst,
                    uint64_t n,
                    char const * restrict file_name,
                    char const * restrict func_name,
                    uint32_t line_number,
                    char const * restrict fmt,
                    ...) {

    int     ret;
    va_list ap;
    va_start(ap, fmt);
    ret = vsnprintf(dst, n, fmt, ap);
    va_end(ap);

    if (UNLIKELY(ret < 0 || ret >= n)) {
        I_die(file_name, func_name, line_number, NULL, NULL);
    }

    return CAST(uint64_t, ret);
}

static NONNULL(1, 2, 3, 4, 6)
    FORMATF(6, 7) char * I_safe_bprintf(char * restrict dst,
                                        char * restrict dst_end,
                                        char const * restrict file_name,
                                        char const * restrict func_name,
                                        uint32_t line_number,
                                        char const * restrict fmt,
                                        ...) {

    int     ret;
    va_list ap;
    if (UNLIKELY(dst_end <= dst)) {
        goto err;
    }

    va_start(ap, fmt);
    ret = vsnprintf(dst, dst_end - dst, fmt, ap);
    va_end(ap);

    if (UNLIKELY(ret < 0 || (dst + ret >= dst_end))) {
        goto err;
    }
    return dst + ret;

err:
    I_die(file_name, func_name, line_number, NULL, NULL);
}


#endif
