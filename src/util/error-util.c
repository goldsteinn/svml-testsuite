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
        (errbuf);                                                              \
    })

EXIT_FUNC
static void
I_printdie(char const * restrict msg, va_list ap) {
    if (msg) {
        size_t msg_len = strlen(msg);
        /* va_list warning is a clang-tidy bug */
        (void)vfprintf(stderr, /* NOLINT */
                       msg,    /* NOLINT */
                       ap);    /* NOLINT */
        if (msg_len == 0 || msg[msg_len - 1] != '\n') {
            fprintf_stderr("\n");
        }
    }
    abort();
}

void
I_va_errdie(char const * restrict file_name,
            char const * restrict func_name,
            uint32_t line_number,
            int32_t  error_number,
            char const * restrict msg,
            va_list ap) {
    char tmpbuf[I_STRERROR_BUFLEN];

    fflush_stdout();
    fflush_stderr();

    fprintf_stderr("%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
                   error_number, I_strerror(error_number, tmpbuf));
    I_printdie(msg, ap);
}

void
I_errdie(char const * restrict file_name,
         char const * restrict func_name,
         uint32_t line_number,
         char const * restrict expr_str,
         int32_t error_number,
         char const * restrict msg,
         ...) {
    char tmpbuf[I_STRERROR_BUFLEN];

    fflush_stdout();
    fflush_stderr();


    if (expr_str != NULL) {
        fprintf_stderr("%s:%s:%d: Assertion '%s' failed. [%d] -> %s\n",
                       file_name, func_name, line_number, expr_str,
                       error_number, I_strerror(error_number, tmpbuf));
    }
    else {
        fprintf_stderr("%s:%s:%d: [%d] -> %s\n", file_name, func_name,
                       line_number, error_number,
                       I_strerror(error_number, tmpbuf));
    }
    {
        va_list ap;
        va_start(ap, msg);
        I_printdie(msg, ap);
        va_end(ap);
    }
}

void
I_die(char const * restrict file_name,
      char const * restrict func_name,
      uint32_t line_number,
      char const * restrict expr_str,
      char const * restrict msg,
      ...) {
    fflush_stdout();
    fflush_stderr();
    if (expr_str != NULL) {
        fprintf_stderr("%s:%s:%d: Assertion '%s' failed.\n", file_name,
                       func_name, line_number, expr_str);
    }
    else {
        fprintf_stderr("%s:%s:%d\n", file_name, func_name, line_number);
    }
    {
        va_list ap;
        va_start(ap, msg);
        I_printdie(msg, ap);
        va_end(ap);
    }
}

void
I_msg_die(char const * restrict msg, ...) {
    fflush_stdout();
    fflush_stderr();
    {
        va_list ap;
        va_start(ap, msg);
        I_printdie(msg, ap);
        va_end(ap);
    }
}
