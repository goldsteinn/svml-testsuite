#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util/error-util.h"
#include "util/file-util.h"
#include "util/memory-util.h"
#include "util/stack-util.h"

int32_t
I_safe_open2(char const * restrict path,
             int32_t flags,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    int32_t ret = open(path, flags);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno,
                 "Unable to open \"%s\" with [flags=0x%03x]\n", path, flags);
    }
    return ret;
}

int32_t
I_safe_open3(char const * restrict path,
             int32_t flags,
             int32_t mode,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {

    int32_t ret = open(path, flags, mode);

    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno,
                 "Unable to open \"%s\" with [flags=0x%03x][mode=0x%03x])\n",
                 path, flags, mode);
    }
    return ret;
}


int32_t
I_ensure_open2(char const * restrict path,
               int32_t flags,
               char const * restrict fn,
               char const * restrict func,
               uint32_t ln) {
    int32_t fd;
    for (;;) {
        fd = I_safe_open2(path, flags, fn, func, ln);
        if (LIKELY(fd >= 0)) {
            break;
        }
        /* Should be impossible but this isn't really meant to be an all
         * that efficient function. */
        err_assert(errno == EAGAIN);
    }
    return fd;
}
int32_t
I_ensure_open3(char const * restrict path,
               int32_t flags,
               int32_t mode,
               char const * restrict fn,
               char const * restrict func,
               uint32_t ln) {
    int32_t fd;
    for (;;) {
        fd = I_safe_open3(path, flags, mode, fn, func, ln);

        if (LIKELY(fd >= 0)) {
            break;
        }
        /* Should be impossible but this isn't really meant to be an all
         * that efficient function. */
        err_assert(errno == EAGAIN);
    }
    return fd;
}

int64_t
I_safe_read(int32_t fd,
            void * restrict buf,
            size_t count,
            char const * restrict fn,
            char const * restrict func,
            uint32_t ln) {
    int64_t ret = read(fd, buf, count);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}
int64_t
I_safe_write(int32_t fd,
             void const * restrict buf,
             size_t count,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    int64_t ret = write(fd, buf, count);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}


int64_t
I_ensure_read(int32_t fd,
              uint8_t * restrict buf,
              size_t count,
              char const * restrict fn,
              char const * restrict func,
              uint32_t ln) {
    int64_t ret;
    size_t  nread = 0;
    for (;;) {
        if (nread == count) {
            break;
        }
        ret = I_safe_read(fd, buf + nread, count - nread, fn, func, ln);
        if (ret <= 0) {
            if (LIKELY(ret == 0)) {
                break;
            }
            /* Should be impossible, but double checking never hurts. */
            err_assert(errno == EAGAIN);
            continue;
        }
        nread += CAST(size_t, ret);
    }
    return CAST(int64_t, nread);
}


int64_t
I_ensure_write(int32_t fd,
               uint8_t const * restrict buf,
               size_t count,
               char const * restrict fn,
               char const * restrict func,
               uint32_t ln) {
    int64_t ret;
    size_t  nwrote = 0;
    for (;;) {
        if (nwrote == count) {
            break;
        }
        ret = I_safe_write(fd, buf + nwrote, count - nwrote, fn, func, ln);
        if (ret <= 0) {
            if (LIKELY(ret == 0)) {
                break;
            }
            /* Should be impossible, but double checking never hurts. */
            err_assert(errno == EAGAIN);
            continue;
        }
        nwrote += CAST(size_t, ret);
    }
    return CAST(int64_t, nwrote);
}

int32_t
I_safe_stat(char const * restrict path,
            struct stat * restrict stats,
            char const * restrict fn,
            char const * restrict func,
            uint32_t ln) {
    int32_t ret = stat(path, stats);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, "Unable to stat \"%s\"\n", path);
    }
    return ret;
}

int32_t
I_safe_fstat(int32_t fd,
             struct stat * restrict stats,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    int32_t ret = fstat(fd, stats);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}


int32_t
I_safe_close(int32_t fd,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    int32_t ret = close(fd);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}

int32_t
I_ensure_close(int32_t fd,
               char const * restrict fn,
               char const * restrict func,
               uint32_t ln) {
    int32_t ret;
    for (;;) {
        ret = I_safe_close(fd, fn, func, ln);
        if (LIKELY(ret >= 0)) {
            break;
        }
        err_assert(errno == EAGAIN);
    }
    return ret;
}

int32_t
I_check_open2(const char * path,
              int32_t      flags,
              const char * fn,
              const char * func,
              uint32_t     ln,
              const char * msg,
              ...) {
    int32_t ret = open(path, flags);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        va_list ap;
        va_start(ap, msg);
        I_va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

int32_t
I_check_open3(const char * path,
              int32_t      flags,
              int32_t      mode,
              const char * fn,
              const char * func,
              uint32_t     ln,
              const char * msg,
              ...) {
    int32_t ret = open(path, flags, mode);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        va_list ap;
        va_start(ap, msg);
        I_va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

int32_t
I_check_access(const char * path,
               int32_t      mode,
               const char * fn,
               const char * func,
               uint32_t     ln,
               const char * msg,
               ...) {
    int32_t ret = access(path, mode);
    if (UNLIKELY(ret < 0 && errno != EAGAIN)) {
        va_list ap;
        va_start(ap, msg);
        I_va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

FILE *
I_safe_fopen(char const * restrict path,
             char const * restrict mode,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    FILE * fp = fopen(path, mode);
    if (UNLIKELY(fp == NULL && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno,
                 "Unable to open \"%s\" with [mode=%-8s]\n", path, mode);
    }
    return fp;
}

FILE *
I_safe_fdopen(int32_t fd,
              char const * restrict mode,
              char const * restrict fn,

              char const * restrict func,
              uint32_t ln) {
    FILE * fp = fdopen(fd, mode);
    if (UNLIKELY(fp == NULL && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return fp;
}

size_t
I_safe_fread(void * restrict ptr,
             size_t size,
             size_t nmemb,
             FILE * restrict stream,
             char const * restrict fn,
             char const * restrict func,
             uint32_t ln) {
    size_t ret = fread(ptr, size, nmemb, stream);
    if (UNLIKELY(ret != nmemb && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}

size_t
I_safe_fwrite(const void * restrict ptr,
              size_t size,
              size_t nmemb,
              FILE * restrict stream,
              char const * restrict fn,
              char const * restrict func,
              uint32_t ln) {
    size_t ret = fwrite(ptr, size, nmemb, stream);
    if (UNLIKELY(ret != nmemb && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}

FILE *
I_check_fopen(const char * restrict path,
              const char * restrict mode,
              const char * fn,
              const char * func,
              uint32_t     ln,
              const char * msg,
              ...) {
    FILE * fp = fopen(path, mode);
    if (UNLIKELY(fp == NULL && errno != EAGAIN)) {
        va_list ap;
        va_start(ap, msg);
        I_va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return fp;
}

int32_t
I_safe_fclose(FILE * restrict stream,
              char const * restrict fn,
              char const * restrict func,
              uint32_t ln) {
    int32_t ret = fclose(stream);
    if (UNLIKELY(ret != 0 && errno != EAGAIN)) {
        I_errdie(fn, func, ln, NULL, errno, NULL);
    }
    return ret;
}


static uint64_t
I_safe_fp_size(FILE * restrict fp,
               char const * restrict fn,
               char const * restrict func,
               uint32_t ln) {
    int64_t  ret, cur;
    uint64_t sz;
    ret = ftell(fp);
    if (UNLIKELY(ret < 0)) {
        goto err;
    }
    cur = ret;
    ret = fseek(fp, 0L, SEEK_END);
    if (UNLIKELY(ret)) {
        goto err;
    }
    ret = ftell(fp);
    if (UNLIKELY(ret < 0)) {
        goto err;
    }
    sz  = CAST(uint64_t, ret);
    ret = fseek(fp, cur, SEEK_SET);
    if (UNLIKELY(ret)) {
        goto err;
    }

    return sz;
err:
    I_errdie(fn, func, ln, NULL, errno, NULL);
}


static void *
NONNULL(2, 3, 4, 5) I_alloc_file_buf(uint64_t fsize,
                                     void * restrict * restrict buf,
                                     uint64_t * buf_sz,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln) {
    uint64_t lbuf_sz = *buf_sz;
    void *   lbuf    = *buf;
    if (fsize > lbuf_sz) {
        if (lbuf == NULL || stack_contains(lbuf)) {
            lbuf = I_safe_calloc(fsize, 1, fn, func, ln);
        }
        else {
            lbuf = I_safe_rezalloc(lbuf, lbuf_sz, fsize, fn, func, ln);
        }

        *buf    = lbuf;
        *buf_sz = fsize;
    }
    die_assert(lbuf);
    return lbuf;
}

uint64_t
I_safe_fp_readfile(FILE * restrict fp,
                   void * restrict * restrict buf,
                   uint64_t * buf_sz,
                   char const * restrict fn,
                   char const * restrict func,
                   uint32_t ln) {
    uint64_t fsize = I_safe_fp_size(fp, fn, func, ln);
    void *   lbuf  = I_alloc_file_buf(fsize, buf, buf_sz, fn, func, ln);
    return I_safe_fread(lbuf, 1, fsize, fp, fn, func, ln);
}

uint64_t
I_safe_fd_readfile(int fd,
                   void * restrict * restrict buf,
                   uint64_t * restrict buf_sz,
                   char const * restrict fn,
                   char const * restrict func,
                   uint32_t ln) {
    uint64_t fsize = I_safe_fd_size(fd, fn, func, ln);
    void *   lbuf  = I_alloc_file_buf(fsize, buf, buf_sz, fn, func, ln);
    return CAST(uint64_t,
                I_ensure_read(fd, (uint8_t *)lbuf, fsize, fn, func, ln));
}
