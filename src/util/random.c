#include <stdint.h>
#include <sys/random.h>

#include "util/error-util.h"
#include "util/memory-util.h"
#include "util/random.h"

__thread uint64_t I_rseed_;

uint64_t
true_rand64() {
    uint64_t rand_out;
    err_assert(getrandom(&rand_out, sizeof(uint64_t), 0) == sizeof(uint64_t));
    return rand_out;
}

uint32_t
true_rand32() {
    uint32_t rand_out;
    err_assert(getrandom(&rand_out, sizeof(uint32_t), 0) == sizeof(uint32_t));
    return rand_out;
}

void
true_randomize_buffer(void * buf, uint64_t buf_sz) {
    int64_t ret = getrandom(buf, buf_sz, 0);
    err_assert(ret > 0 && CAST(uint64_t, ret) == buf_sz, "%lu != %lu\n", ret,
               buf_sz);
}

uint32_t *
make_true_rand32_buffer(uint64_t nitems) {
    uint32_t * buf = (uint32_t *)safe_malloc(nitems * sizeof(uint32_t));
    true_randomize_buffer(buf, nitems * sizeof(uint32_t));
    return buf;
}

uint64_t *
make_true_rand64_buffer(uint64_t nitems) {
    uint64_t * buf = (uint64_t *)safe_malloc(nitems * sizeof(uint64_t));
    true_randomize_buffer(buf, nitems * sizeof(uint64_t));
    return buf;
}

void
randomize_buffer(uint8_t * buf, uint64_t buf_sz) {
    uint64_t i;
    uint64_t seed = get_rand_seed();
    uint64_t r64;
    for (i = 0; i + 8 < buf_sz; i += 8) {
        r64  = rand64c(seed);
        seed = next_rand_seed(seed);
        __builtin_memcpy(buf + i, &r64, sizeof(r64));
    }
    r64 = rand64c(seed);
    __builtin_memcpy(buf + i, &r64, buf_sz - i);
}
