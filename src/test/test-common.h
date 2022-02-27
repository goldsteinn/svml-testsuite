#ifndef _SRC__TEST__TEST_COMMON_H_
#define _SRC__TEST__TEST_COMMON_H_

#include "util/attrs.h"
#include "util/common.h"
#include "util/macro.h"
#include "util/types.h"

MALLOC_FUNC uint8_t * make_buf(uint64_t sz);
NONNULL(1) void free_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) void randomize_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) void seq_buf(uint8_t * buf, uint64_t sz);
NONNULL(1) PURE_FUNC int32_t is_seq(uint8_t const * buf, uint64_t sz);

#define test_assert(...)                                                       \
    CAT(test_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)
#define _test_assert(X, todo)                                                  \
    if (UNLIKELY(!(X))) {                                                      \
        fprintf(stderr, "\n\t%-20s:%-4u", __FILENAME__, __LINE__);             \
        todo;                                                                  \
        return -1;                                                             \
    }

#define test_assert_MANY(X, msg, args...)                                      \
    _test_assert(X, fprintf(stderr, "\t" msg "\n", ##args));

#define test_assert_ONE(X) _test_assert(X, fprintf(stderr, "\n"))


#endif
