#ifndef _SRC__UTIL__MEMORY_UTIL_H_
#define _SRC__UTIL__MEMORY_UTIL_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <wchar.h>

#include "util/common.h"
#include "util/error-util.h"
#include "util/macro.h"

#define PAGE_SIZE 4096

#define safe_calloc(n, sz) _safe_calloc(n, sz, __FILENAME__, __func__, __LINE__)
#define safe_realloc(p, sz)                                                    \
    _safe_realloc(p, sz, __FILENAME__, __func__, __LINE__)
#define safe_srealloc(p, sz_old, sz_new)                                       \
    _safe_srealloc(p, sz_old, sz_new, __FILENAME__, __func__, __LINE__)
#define safe_malloc(sz) _safe_malloc(sz, __FILENAME__, __func__, __LINE__)
#define safe_mmap(addr, sz, prot_flags, mmap_flags, fd, offset)                \
    _safe_mmap(addr, sz, prot_flags, mmap_flags, fd, offset, __FILENAME__,     \
               __func__, __LINE__)
#define safe_mmap_alloc(sz)                                                    \
    safe_mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,   \
              -1, 0)
#define safe_munmap(addr, sz)                                                  \
    _safe_munmap(addr, sz, __FILENAME__, __func__, __LINE__)
#define safe_mprotect(addr, sz, prot_flags)                                    \
    _safe_mprotect(addr, sz, prot_flags, __FILENAME__, __func__, __LINE__)

#define safe_free(addr)  _safe_free(addr)
#define safe_sfree(addr) _safe_sfree(addr)

#define is_valid_addr(addr)                                                    \
    (CAST(uint64_t, addr) > CAST(uint64_t, 65536) &&                           \
     (!(CAST(uint64_t, addr) >> 48)))

static MALLOC_FUNC
NONNULL(3, 4) void * _safe_calloc(uint64_t n,
                                  uint64_t sz,
                                  char const * restrict fn,
                                  char const * restrict func,
                                  int32_t ln) {
    void * p = calloc_c(n, sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return p;
}

static MALLOC_FUNC
NONNULL(2, 3) void * _safe_malloc(uint64_t sz,
                                  char const * restrict fn,
                                  char const * restrict func,
                                  int32_t ln) {
    void * p = malloc_c(sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return p;
}

static MALLOC_FUNC
NONNULL(1, 3, 4) void * _safe_realloc(void * restrict p,
                                      uint64_t sz,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      int32_t ln) {
    void * newp = realloc_c(p, sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return newp;
}


static MALLOC_FUNC
NONNULL(1, 4, 5) void * _safe_srealloc(void * restrict p,
                                       uint64_t sz_old,
                                       uint64_t sz_new,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       int32_t ln) {
    void * newp = srealloc_c(p, sz_old, sz_new);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return newp;
}

static void
_safe_free(void * addr) {
    if (LIKELY(addr != NULL)) {
        free_c(addr);
    }
}


static void
_safe_sfree(void * addr, uint64_t sz) {
    if (LIKELY(addr != NULL)) {
        sfree_c(addr, sz);
    }
}


MALLOC_FUNC NONNULL(7, 8) void * _safe_mmap(void * restrict addr,
                                            uint64_t sz,
                                            int32_t  prot_flags,
                                            int32_t  mmap_flags,
                                            int32_t  fd,
                                            int32_t  offset,
                                            char const * restrict fn,
                                            char const * restrict func,
                                            int32_t ln);

NONNULL(1, 3, 4)
void _safe_munmap(void * restrict addr,
                  uint64_t sz,
                  char const * restrict fn,
                  char const * restrict func,
                  int32_t ln);

NONNULL(1, 4, 5)
void _safe_mprotect(void * restrict addr,
                    uint64_t sz,
                    int32_t  prot_flags,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);

#endif
