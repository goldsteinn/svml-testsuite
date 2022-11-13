#include "util/stack-util.h"
#include "util/error-util.h"
#include "util/file-util.h"
#include "util/memory-util.h"

uintptr_t I_stack_start_;
uintptr_t I_stack_end_;

#define I_STACK_FILE_PATH "/proc/self/maps"
#define I_STACK_LINE_IND  "[stack]"
static void __attribute__((constructor)) I_find_stack(void) {
    char *   buf;
    uint64_t buf_sz = 256;
    FILE *   fp;

    die_assert(!I_stack_start_ && !I_stack_end_);
    buf = (char *)safe_zalloc(buf_sz);
    die_assert(I_stack_end_ == 0);
    fp = safe_fopen(I_STACK_FILE_PATH, "r");
    for (;;) {

        int64_t  nreads = getline(&buf, &buf_sz, fp);
        uint64_t nread;
        char *   stack_id_start, *vals_start;
        if (nreads < 0) {

            break;
        }
        nread = CAST(uint64_t, nreads);

        stack_id_start = (char *)memchr(buf, '[', buf_sz);
        if (stack_id_start == NULL || ((CAST(uint64_t, stack_id_start - buf) +
                                        strlen(I_STACK_LINE_IND)) >= nread)) {
            continue;
        }
        if (memcmpeq_c((uint8_t const *)stack_id_start,
                       (uint8_t const *)I_STACK_LINE_IND,
                       strlen(I_STACK_LINE_IND))) {
            continue;
        }

        stack_id_start = buf;
        vals_start     = buf;

        I_stack_start_ = strtoull(vals_start, &stack_id_start, 16);
        die_assert(stack_id_start != vals_start && *stack_id_start == '-');
        vals_start     = stack_id_start + 1;
        stack_id_start = vals_start;

        I_stack_end_ = strtoull(vals_start, &stack_id_start, 16);
        die_assert(stack_id_start != vals_start && *stack_id_start == ' ');

        break;
    }

    safe_fclose(fp);
    safe_sfree(buf, buf_sz);

    die_assert(I_stack_start_ && I_stack_end_);
}
