#ifndef _SRC__UTIL__ERROR_UTIL_H_
#define _SRC__UTIL__ERROR_UTIL_H_


#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "util/common.h"
#include "util/macro.h"

#define err_assert(...) CAT(err_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)

#define die_assert(...) CAT(die_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)

#define msg_assert(X, msg, args...)                                            \
    if (UNLIKELY(!(X))) {                                                      \
        _msg_die(msg, ##args);                                                 \
    }

#define warn_assert(X, ...)                                                    \
    if (UNLIKELY(!(X))) {                                                      \
        fprintf(stderr, __VA_ARGS__);                                          \
    }

#define err_assert_MANY(X, msg, args...)                                       \
    if (UNLIKELY(!(X))) {                                                      \
        errdie(msg, ##args);                                                   \
    }

#define err_assert_ONE(X)                                                      \
    if (UNLIKELY(!(X))) {                                                      \
        errdie(NULL);                                                          \
    }

#define die_assert_MANY(X, msg, args...)                                       \
    if (UNLIKELY(!(X))) {                                                      \
        die(msg, ##args);                                                      \
    }

#define die_assert_ONE(X)                                                      \
    if (UNLIKELY(!(X))) {                                                      \
        die(NULL);                                                             \
    }

#define die(msg, args...) _die(__FILENAME__, __func__, __LINE__, msg, ##args);
#define errdie(msg, args...)                                                   \
    _errdie(__FILENAME__, __func__, __LINE__, errno, msg, ##args);

//#define WITH_DBG_PRINT
#ifdef WITH_DBG_PRINT
#define dbg_assert(...) die_assert(__VA_ARGS__)
#define dbg_print(...)  fprintf(stderr, __VA_ARGS__)
#define PRINTFFL                                                               \
    fprintf(stderr, "%-20s:%-20s:%-4d\n", __FILENAME__, __func__, __LINE__)
#else
#define dbg_print(...)
#define dbg_assert(...)
#define PRINTFFL
#endif

EXIT_FUNC void _va_errdie(const char * const file_name,
                          const char * const func_name,
                          uint32_t           line_number,
                          int32_t            error_number,
                          const char * const msg,
                          va_list            ap);

EXIT_FUNC void _errdie(const char * const file_name,
                       const char * const func_name,
                       uint32_t           line_number,
                       int32_t            error_number,
                       const char * const msg,
                       ...);

EXIT_FUNC void _die(const char * const file_name,
                    const char * const func_name,
                    uint32_t           line_number,
                    const char * const msg,
                    ...);

EXIT_FUNC void _msg_die(const char * const msg, ...);
#endif
