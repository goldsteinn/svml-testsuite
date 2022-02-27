#include "util/error-util.h"

void
_va_errdie(char const * restrict file_name,
           char const * restrict func_name,
           uint32_t line_number,
           int32_t  error_number,
           char const * restrict msg,
           va_list ap) {
    fprintf(stderr, "%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
            error_number, strerror(error_number));
    if (msg) {
        /* va_list warning is a clang-tidy bug */
        vfprintf(stderr, /* NOLINT */
                 msg,    /* NOLINT */
                 ap);    /* NOLINT */
    }
    abort();
}

void
_errdie(char const * restrict file_name,
        char const * restrict func_name,
        uint32_t line_number,
        int32_t  error_number,
        char const * restrict msg,
        ...) {
    fprintf(stderr, "%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
            error_number, strerror(error_number));
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        vfprintf(stderr, /* NOLINT */
                 msg,    /* NOLINT */
                 ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}

void
_die(char const * restrict file_name,
     char const * restrict func_name,
     uint32_t line_number,
     char const * restrict msg,
     ...) {
    fprintf(stderr, "%s:%s:%d\n", file_name, func_name, line_number);
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        vfprintf(stderr, /* NOLINT */
                 msg,    /* NOLINT */
                 ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}

void
_msg_die(char const * restrict msg, ...) {
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        /* va_list warning is a clang-tidy bug */
        vfprintf(stderr, /* NOLINT */
                 msg,    /* NOLINT */
                 ap);    /* NOLINT */
        va_end(ap);
    }
    abort();
}
