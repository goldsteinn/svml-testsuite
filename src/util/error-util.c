#include "util/error-util.h"

EXIT_FUNC void
_va_errdie(const char * const file_name,
           const char * const func_name,
           uint32_t           line_number,
           int32_t            error_number,
           const char * const msg,
           va_list            ap) {
    fprintf(stderr, "%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
            error_number, strerror(error_number));
    if (msg) {
        vfprintf(stderr,  // NOLINT
                 msg,     // NOLINT
                 ap);     // NOLINT /* This warning is a clang-tidy bug */
    }
    abort();
}

EXIT_FUNC void
_errdie(const char * const file_name,
        const char * const func_name,
        uint32_t           line_number,
        int32_t            error_number,
        const char * const msg,
        ...) {
    fprintf(stderr, "%s:%s:%d: [%d] -> %s\n", file_name, func_name, line_number,
            error_number, strerror(error_number));
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        vfprintf(stderr,  // NOLINT
                 msg,     // NOLINT
                 ap);     // NOLINT /* This warning is a clang-tidy bug */
        va_end(ap);
    }
    abort();
}

EXIT_FUNC void
_die(const char * const file_name,
     const char * const func_name,
     uint32_t           line_number,
     const char * const msg,
     ...) {
    fprintf(stderr, "%s:%s:%d\n", file_name, func_name, line_number);
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        vfprintf(stderr,  // NOLINT
                 msg,     // NOLINT
                 ap);     // NOLINT /* This warning is a clang-tidy bug */
        va_end(ap);
    }
    abort();
}

EXIT_FUNC void
_msg_die(const char * const msg, ...) {
    va_list ap;
    if (msg) {
        va_start(ap, msg);
        vfprintf(stderr,  // NOLINT
                 msg,     // NOLINT
                 ap);     // NOLINT /* This warning is a clang-tidy bug */
        va_end(ap);
    }
    abort();
}
