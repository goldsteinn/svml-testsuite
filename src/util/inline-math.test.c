#include <stdint.h>

#include "util/common.h"
#include "util/inline-math.h"
#include "util/macro.h"

#include "test/test-common.h"


/********************************************************************/
/* Static tests. */
_static_assert(p2_factor(24) == 8);
_static_assert(p2_factor(1) == 1);
_static_assert(p2_factor(8) == 8);
_static_assert(p2_factor(2) == 2);

_static_assert(is_p2(0));
_static_assert(is_p2(1));
_static_assert(is_p2(1U << 31));
_static_assert(is_p2(1UL << 63));

_static_assert(!is_p2(-1));
_static_assert(!is_p2(3));


#define testeq(impl, ref, val)                                                 \
    res   = impl(val);                                                         \
    expec = ref(val);                                                          \
    test_assert(res == expec && is_p2(res) && p2_factor(res) == res,           \
                "%-20s(%lu): %lu != %lu\n", V_TO_STR(ref),                     \
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
test_log2() {
    uint64_t j, expec;
    for (expec = 0, j = 1; j; ++expec, j <<= 1) {
        test_assert(expec == log2_ru(j), "%lu (%lu != %lu)\n", j, expec,
                    log2_ru(j));
        test_assert(expec == log2_rd(j));

    }

    for (expec = 2, j = 4; j; ++expec, j <<= 1) {
        test_assert(expec == log2_ru(j - 1));
        test_assert(expec - 1 == log2_rd(j - 1));

        test_assert(expec + 1 == log2_ru(j + 1));
        test_assert(expec == log2_rd(j + 1));
    }

    return 0;
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
