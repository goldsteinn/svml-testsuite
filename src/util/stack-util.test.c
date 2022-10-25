#include <stdint.h>

#include "util/common.h"
#include "util/stack-util.h"
#include "util/memory-util.h"
#include "test/test-common.h"


int32_t
test_stack() {
    void * tmp = safe_zalloc(123);
    char   buf[123];
    test_assert(stack_start() && stack_end());
    test_assert(!stack_contains(tmp));
    test_assert(stack_contains(buf));

    return 0;
}
