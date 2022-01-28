#include <stdint.h>

#include "util/common.h"
#include "util/inline-math.h"
#include "util/macro.h"

#include "test/test-common.h"


#define testeq(impl, ref, val)                                                 \
    res   = impl(val);                                                         \
    expec = ref(val);                                                          \
    test_assert(res == expec, "%-20s(%lu): %lu != %lu\n", V_TO_STR(ref),       \
                CAST(uint64_t, (val)), res, expec);


#define testall(v)                                                             \
    testeq(next_p2, simple_next_p2_32, CAST(uint32_t, v));                     \
    testeq(prev_p2, simple_prev_p2_32, CAST(uint32_t, v));                     \
    testeq(next_p2, simple_next_p2_64, CAST(uint64_t, v));                     \
    testeq(prev_p2, simple_prev_p2_64, CAST(uint64_t, v));                     \
    testeq(next_p2, simple_next_p2_64, CAST(int64_t, v));                      \
    testeq(prev_p2, simple_prev_p2_64, CAST(int64_t, v));


static uint64_t
simple_next_p2_64(uint64_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v = v | (v >> 32);
    v++;
    return v;
}

static uint64_t
simple_prev_p2_64(uint64_t x) {
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    x = x | (x >> 32);
    return x - (x >> 1);
}

static uint64_t
simple_next_p2_32(uint64_t v) {
    return CAST(uint32_t, simple_next_p2_64(v));
}

static uint64_t
simple_prev_p2_32(uint64_t v) {
    return CAST(uint32_t, simple_prev_p2_64(v));
}

int32_t
test_p2() {
    uint64_t res, expec, i, j;

    for (i = 0; i < RAND_MAX;) {
        for (j = 1; j; j <<= 1) {
            testall(i);
            testall(RAND_MAX - i);
            testall(CAST(uint32_t, ~0) - i);
            testall(~CAST(uint64_t, 0) - i);
            testall(CAST(uint32_t, ~0) + i);
            testall(~CAST(uint64_t, 0) + i);

            testall(i + j);
            testall(RAND_MAX - i + j);
            testall(CAST(uint32_t, ~0) - i + j);
            testall(~CAST(uint64_t, 0) - i + j);
            testall(CAST(uint32_t, ~0) + i + j);
            testall(~CAST(uint64_t, 0) + i + j);

            testall(i - j);
            testall(RAND_MAX - i - j);
            testall(CAST(uint32_t, ~0) - i - j);
            testall(~CAST(uint64_t, 0) - i - j);
            testall(CAST(uint32_t, ~0) + i - j);
            testall(~CAST(uint64_t, 0) + i - j);

            testall(j - i);
        }
        if (i < 128) {
            ++i;
        }
        else {
            i += rand() / 256;
        }
    }
    return 0;
}
