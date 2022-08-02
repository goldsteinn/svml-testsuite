#include <string.h>

#include "util/error-util.h"
#include "util/print.h"

#include "lib/commonlib.h"

enum { I_STRERROR_BUFLEN = 512 };

#define I_strerror(err, errbuf)                                                \
    ({                                                                         \
        if (strerror_r(err, errbuf, I_STRERROR_BUFLEN)) {                      \
            memcpy_c(errbuf, "Error generating strerror msg!",                 \
                     strlen("Error generating strerror msg!"));                \
        }                                                                      \
        errbuf;                                                                \
    })

void
I_va_errdie(char const * restrict file_name,
            char const * restrict func_name,
            uint32_t line_number,
            int32_t  error_number,
            char const * restrict msg,
            va_list ap) {
    char tmpbuf[I_STRERROR_BUFLEN];
    fprintf_stderr("%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
                   error_number, I_strerror(error_number, tmpbuf));
    if (msg) {
        /* va_list warning is a clang-tidy bug */
        (void)vfprintf(stderr, /* NOLINT */
                       msg,    /* NOLINT */
                       ap);    /* NOLINT */
    }
    abort();
}

void
I_errdie(char const * restrict file_name,
         char const * restrict func_name,
         uint32_t line_number,
         int32_t  error_number,
         char const * restrict msg,
         ...) {
    char tmpbuf[I_STRERROR_BUFLEN];
    va_list ap;
    fprintf_stderr("%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
                   error_number, I_strerror(error_number, tmpbuf));

    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        (void)vfprintf(stderr, /* NOLINT */
                       msg,    /* NOLINT */
                       ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}

void
I_die(char const * restrict file_name,
      char const * restrict func_name,
      uint32_t line_number,
      char const * restrict msg,
      ...) {
    fprintf_stderr("%s:%s:%d\n", file_name, func_name, line_number);
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        (void)vfprintf(stderr, /* NOLINT */
                       msg,    /* NOLINT */
                       ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}

void
I_msg_die(char const * restrict msg, ...) {
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        (void)vfprintf(stderr, /* NOLINT */
                       msg,    /* NOLINT */
                       ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}
