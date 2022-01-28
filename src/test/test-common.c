#include "test/test-common.h"

#include "util/random.h"

uint8_t *
make_buf(uint64_t sz) {
    uint8_t * buf = safe_mmap(NULL, 2 * PAGE_SIZE + sz, PROT_NONE,
                              MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    safe_mprotect(buf + PAGE_SIZE, sz, PROT_READ | PROT_WRITE);
    return buf + PAGE_SIZE;
}

void
free_buf(uint8_t * buf, uint64_t sz) {
    safe_munmap(buf - PAGE_SIZE, sz + 2 * PAGE_SIZE);
}


void
randomize_buf(uint8_t * buf, uint64_t sz) {
    for (uint64_t i = 0; i < sz; ++i) {
        buf[i] = CAST(uint8_t, rand32());
    }
}

void
seq_buf(uint8_t * buf, uint64_t sz) {
    for (uint64_t i = 0; i < sz; ++i) {
        buf[i] = CAST(uint8_t, i);
    }
}

int32_t
is_seq(uint8_t * buf, uint64_t sz) {
    for (uint64_t i = 0; i < sz; ++i) {
        uint8_t bufi = buf[i];
        if (buf[i] != CAST(uint8_t, i)) {
            return 0;
        }
    }
    return 1;
}
