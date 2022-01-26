#ifndef SRC_D_TEST_D_TEST_COMMON_H_
#define SRC_D_TEST_D_TEST_COMMON_H_

#include <sys/mman.h>

#include "util/attrs.h"
#include "util/common.h"
#include "util/macro.h"
#include "util/print.h"
#include "util/types.h"


SYS_MALLOC_FUNC(1) uint8_t * make_buf(uint64_t sz);
NONNULL(1) void free_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) void randomize_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) void seq_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) PURE_FUNC int32_t is_seq(uint8_t const * buf, uint64_t sz);

int32_t I_flt_eq(float a, float b);
int32_t I_dbl_eq(double a, double b);

#define float_eq(a, b)                                                         \
    ((is_same_type(get_type(a), get_type(b)) && sizeof(a) <= sizeof(double))   \
         ? (is_same_type(get_type(a), float)                                   \
                ? I_flt_eq(CAST(float, a), CAST(float, b))                     \
                : I_dbl_eq(CAST(double, a), CAST(double, b)))                  \
         : 0)

void set_max_ulp(int32_t value);


#define test_assert(...)                                                       \
    CAT(test_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)
#define I_test_assert(X, todo)                                                 \
    if (UNLIKELY(!(X))) {                                                      \
        fprintf_stderr("\n\t%-20s:%-4u", I_FILENAME_, __LINE__);               \
        (void)(todo);                                                          \
        return -1;                                                             \
    }

#define test_assert_MANY(X, msg, ...)                                          \
    I_test_assert(X, fprintf(stderr, "\t" msg "\n", ##__VA_ARGS__));

#define test_assert_ONE(X) I_test_assert(X, fprintf(stderr, "\n"))


#endif
