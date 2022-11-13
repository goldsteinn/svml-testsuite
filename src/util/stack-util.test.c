#include <stdint.h>

#include "test/test-common.h"
#include "util/common.h"
#include "util/memory-util.h"
#include "util/stack-util.h"

int32_t test_stack(void);

static int32_t
test_stack_impl(void * tmp) {
    char buf[123];
    test_assert(stack_start() && stack_end());
    test_assert(!stack_contains(tmp));
    test_assert(stack_contains(buf));
    return 0;
}
int32_t
test_stack(void) {
    enum { BUF_SZ = 123 };
    void *  tmp = safe_zalloc(BUF_SZ);
    int32_t ret = test_stack_impl(tmp);
    safe_sfree(tmp, BUF_SZ);
    return ret;
}
