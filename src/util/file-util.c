#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util/error-util.h"
#include "util/file-util.h"

int
_safe_open2(const char * path,
            int          flags,
            const char * fn,
            const char * func,
            int          ln) {
    int ret = open(path, flags);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno,
                "Unable to open \"%s\" with [flags=0x%03x]\n", path, flags);
    }
    return ret;
}
int
_safe_open3(const char * path,
            int          flags,
            int          mode,
            const char * fn,
            const char * func,
            int          ln) {
    int ret = open(path, mode);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno,
                "Unable to open \"%s\" with [flags=0x%03x][mode=0x%03x])\n",
                path, flags, mode);
    }
    return ret;
}
ssize_t
_safe_read(int          fd,
           void *       buf,
           size_t       count,
           const char * fn,
           const char * func,
           int          ln) {
    ssize_t ret = read(fd, buf, count);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}
ssize_t
_safe_write(int          fd,
            void *       buf,
            size_t       count,
            const char * fn,
            const char * func,
            int          ln) {
    ssize_t ret = write(fd, buf, count);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}

int
_safe_stat(const char *  path,
           struct stat * buf,
           const char *  fn,
           const char *  func,
           int           ln) {
    int ret = stat(path, buf);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno, "Unable to stat \"%s\"\n", path);
    }
    return ret;
}

int
_safe_fstat(int           fd,
            struct stat * buf,
            const char *  fn,
            const char *  func,
            int           ln) {
    int ret = fstat(fd, buf);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}

int
_safe_access(const char * path,
             int          mode,
             const char * fn,
             const char * func,
             int          ln) {
    int ret = access(path, mode);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno,
                "Unable to access \"%s\" with [mode=0x%03x]\n", path, mode);
    }
    return ret;
}

int
_safe_close(int fd, const char * fn, const char * func, int ln) {
    int ret = close(fd);
    if (UNLIKELY(ret < 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}

int
_check_open2(const char * path,
             int          flags,
             const char * fn,
             const char * func,
             int          ln,
             const char * msg,
             ...) {
    int ret = open(path, flags);
    if (UNLIKELY(ret < 0)) {
        va_list ap;
        va_start(ap, msg);
        _va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

int
_check_open3(const char * path,
             int          flags,
             int          mode,
             const char * fn,
             const char * func,
             int          ln,
             const char * msg,
             ...) {
    int ret = open(path, flags, mode);
    if (UNLIKELY(ret < 0)) {
        va_list ap;
        va_start(ap, msg);
        _va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

int
_check_access(const char * path,
              int          mode,
              const char * fn,
              const char * func,
              int          ln,
              const char * msg,
              ...) {
    int ret = access(path, mode);
    if (UNLIKELY(ret < 0)) {
        va_list ap;
        va_start(ap, msg);
        _va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return ret;
}

FILE *
_safe_fopen(const char * restrict path,
            const char * restrict mode,
            const char * fn,
            const char * func,
            int          ln) {
    FILE * fp = fopen(path, mode);
    if (UNLIKELY(fp == NULL)) {
        _errdie(fn, func, ln, errno, "Unable to open \"%s\" with [mode=%-8s]\n",
                path, mode);
    }
    return fp;
}

FILE *
_safe_fdopen(int          fd,
             const char * mode,
             const char * fn,
             const char * func,
             int          ln) {
    FILE * fp = fdopen(fd, mode);
    if (UNLIKELY(fp == NULL)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return fp;
}

size_t
_safe_fread(void * restrict ptr,
            size_t size,
            size_t nmemb,
            FILE * restrict stream,
            const char * fn,
            const char * func,
            int          ln) {
    size_t ret = fread(ptr, size, nmemb, stream);
    if (UNLIKELY(ret == 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}

size_t
_safe_fwrite(const void * restrict ptr,
             size_t size,
             size_t nmemb,
             FILE * restrict stream,
             const char * fn,
             const char * func,
             int          ln) {
    size_t ret = fwrite(ptr, size, nmemb, stream);
    if (UNLIKELY(ret == 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}

FILE *
_check_fopen(const char * restrict path,
             const char * restrict mode,
             const char * fn,
             const char * func,
             int          ln,
             const char * msg,
             ...) {
    FILE * fp = fopen(path, mode);
    if (UNLIKELY(fp == NULL)) {
        va_list ap;
        va_start(ap, msg);
        _va_errdie(fn, func, ln, errno, msg, ap);
        va_end(ap);
    }
    return fp;
}

int
_safe_fclose(FILE * restrict stream,
             const char * fn,
             const char * func,
             int          ln) {
    int ret = fclose(stream);
    if (UNLIKELY(ret != 0)) {
        _errdie(fn, func, ln, errno, NULL);
    }
    return ret;
}
