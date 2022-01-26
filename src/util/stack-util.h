#ifndef SRC_D_UTIL_D_STACK_UTIL_H_
#define SRC_D_UTIL_D_STACK_UTIL_H_

#include "util/error-util.h"
#include "util/types.h"

extern uintptr_t I_stack_start_;
extern uintptr_t I_stack_end_;

static PURE_FUNC uintptr_t
stack_start(void) {
    die_assert(I_stack_start_);
    return I_stack_start_;
}

static PURE_FUNC uintptr_t
stack_end(void) {
    die_assert(I_stack_end_);
    return I_stack_end_;
}


static PURE_FUNC uint64_t
I_stack_contains(uintptr_t p) {
    return p >= stack_start() && p <= stack_end();
}

#define stack_contains(p) I_stack_contains(CAST(uintptr_t, p))


#endif
