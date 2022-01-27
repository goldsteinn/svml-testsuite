#ifndef _SRC__UTIL__MEMORY_UTIL_H_
#define _SRC__UTIL__MEMORY_UTIL_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <wchar.h>

#include "util/common.h"
#include "util/error-util.h"
#include "util/macro.h"


/* Use safe_<type> for aliasing casts. */
typedef wchar_t  safe_wchar_t __attribute__((may_alias));
typedef uint32_t safe_uint32_t __attribute__((may_alias, aligned(1)));
typedef int32_t  safe_int32_t __attribute__((may_alias, aligned(1)));
typedef uint64_t safe_uint64_t __attribute__((may_alias, aligned(1)));
typedef int64_t  safe_int64_t __attribute__((may_alias, aligned(1)));

typedef char               safe_char __attribute__((may_alias, aligned(1)));
typedef unsigned char      safe_uchar __attribute__((may_alias, aligned(1)));
typedef short              safe_short __attribute__((may_alias, aligned(1)));
typedef unsigned short     safe_ushort __attribute__((may_alias, aligned(1)));
typedef int                safe_int __attribute__((may_alias, aligned(1)));
typedef unsigned int       safe_uint __attribute__((may_alias, aligned(1)));
typedef long               safe_long __attribute__((may_alias, aligned(1)));
typedef unsigned long      safe_ulong __attribute__((may_alias, aligned(1)));
typedef long long          safe_llong __attribute__((may_alias, aligned(1)));
typedef unsigned long long safe_ullong __attribute__((may_alias, aligned(1)));
typedef float              safe_float __attribute__((may_alias, aligned(1)));
typedef double             safe_double __attribute__((may_alias, aligned(1)));
typedef float              safe_float __attribute__((may_alias, aligned(1)));
typedef long double        safe_ldouble __attribute__((may_alias, aligned(1)));

#define PAGE_SIZE 4096

#define safe_calloc(n, sz) _safe_calloc(n, sz, __FILENAME__, __func__, __LINE__)
#define safe_realloc(p, sz)                                                    \
    _safe_realloc(p, sz, __FILENAME__, __func__, __LINE__)
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


static void *
_safe_calloc(uint64_t           n,
             uint64_t           sz,
             const char * const fn,
             const char *       func,
             int32_t            ln) {
    void * p = calloc_c(n, sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return p;
}

static void *
_safe_malloc(uint64_t           sz,
             const char * const fn,
             const char *       func,
             int32_t            ln) {
    void * p = malloc_c(sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return p;
}

static void *
_safe_realloc(void *             p,
              uint64_t           sz,
              const char * const fn,
              const char *       func,
              int32_t            ln) {
    void * newp = realloc_c(p, sz);
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
    UNUSED(sz);
    if (LIKELY(addr != NULL)) {
        sfree_c(addr, sz);
    }
}


void * _safe_mmap(void *             addr,
                  uint64_t           sz,
                  int32_t            prot_flags,
                  int32_t            mmap_flags,
                  int32_t            fd,
                  int32_t            offset,
                  const char * const fn,
                  const char *       func,
                  const int32_t      ln);

void _safe_munmap(void *             addr,
                  uint64_t           sz,
                  const char * const fn,
                  const char *       func,
                  const int32_t      ln);

void _safe_mprotect(void *             addr,
                    uint64_t           sz,
                    int32_t            prot_flags,
                    const char * const fn,
                    const char *       func,
                    const int32_t      ln);

#endif
