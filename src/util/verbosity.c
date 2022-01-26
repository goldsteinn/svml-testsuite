#include <stdarg.h>

#include "util/verbosity.h"

static int32_t verbose = 0;

int32_t
check_verbosity(int32_t verbosity_bound) {
    return verbosity_bound <= verbose;
}

void
set_verbosity(int32_t verbosity) {
    verbose = verbosity;
}

NEVER_INLINE void
vcheck_print(int32_t verbosity_bound, FILE * fp, const char * msg, ...) {
    if (check_verbosity(verbosity_bound)) {
        va_list ap;
        va_start(ap, msg);
        vfprintf(fp,   // NOLINT
                 msg,  // NOLINT
                 ap);  // NOLINT /* This warning is a clang-tidy bug */
        va_end(ap);
    }
}
