#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util/common.h"
#include "util/macro.h"

/* fd operations. */
#define safe_open(...) CAT(safe_open, PP_ARG_N(__VA_ARGS__))(__VA_ARGS__)
#define safe_read(fd, buf, count)                                              \
    _safe_read(fd, buf, count, __FILENAME__, __func__, __LINE__)
#define safe_write(fd, buf, count)                                             \
    _safe_write(fd, buf, count, __FILENAME__, __func__, __LINE__)
#define safe_stat(path, buf)                                                   \
    _safe_stat(path, buf, __FILENAME__, __func__, __LINE__)
#define safe_fstat(fd, buf)                                                    \
    _safe_fstat(fd, buf, __FILENAME__, __func__, __LINE__)
#define check_access(path, mode, ...)                                          \
    _check_access(path, mode, __FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define safe_access(path, mode)                                                \
    _safe_access(path, mode, __FILENAME__, __func__, __LINE__)
#define safe_close(fd) _safe_close(fd, __FILENAME__, __func__, __LINE__)

#define check_open2(path, flags, ...)                                          \
    _check_open2(path, flags, __FILENAME__, __func__, __LINE__, __VA_ARGS__)

#define check_open3(path, flags, mode, ...)                                    \
    _check_open3(path, flags, mode, __FILENAME__, __func__, __LINE__,          \
                 __VA_ARGS__)

#define safe_open2(path, flags)                                                \
    _safe_open2(path, flags, __FILENAME__, __func__, __LINE__)
#define safe_open3(path, flags, mode)                                          \
    _safe_open3(path, flags, mode, __FILENAME__, __func__, __LINE__)

/* FILE * operations. */
#define safe_fopen(path, mode)                                                 \
    _safe_fopen(path, mode, __FILENAME__, __func__, __LINE__)
#define safe_fdopen(fd, mode)                                                  \
    _safe_fdopen(fd, mode, __FILENAME__, __func__, __LINE__)
#define safe_fread(ptr, size, nmemb, stream)                                   \
    _safe_fread(ptr, size, nmemb, stream, __FILENAME__, __func__, __LINE__)
#define safe_fwrite(ptr, size, nmemb, stream)                                  \
    _safe_fread(ptr, size, nmemb, stream, __FILENAME__, __func__, __LINE__)
#define safe_fclose(stream)                                                    \
    _safe_fclose(stream, __FILENAME__, __func__, __LINE__)

#define check_fopen(path, mode, ...)                                           \
    _check_fopen(path, mode, __FILENAME__, __func__, __LINE__, __VA_ARGS__)

/* Declarataions. */

int     _safe_open2(const char * path,
                    int          flags,
                    const char * fn,
                    const char * func,
                    int          ln);
int     _safe_open3(const char * path,
                    int          flags,
                    int          mode,
                    const char * fn,
                    const char * func,
                    int          ln);
ssize_t _safe_read(int          fd,
                   void *       buf,
                   size_t       count,
                   const char * fn,
                   const char * func,
                   int          ln);
ssize_t _safe_write(int          fd,
                    void *       buf,
                    size_t       count,
                    const char * fn,
                    const char * func,
                    int          ln);
int     _safe_stat(const char *  path,
                   struct stat * buf,
                   const char *  fn,
                   const char *  func,
                   int           ln);
int     _safe_fstat(int           fd,
                    struct stat * buf,
                    const char *  fn,
                    const char *  func,
                    int           ln);
int     _safe_access(const char * path,
                     int          mode,
                     const char * fn,
                     const char * func,
                     int          ln);
int     _safe_close(int fd, const char * fn, const char * func, int ln);

int _check_open2(const char * path,
                 int          flags,
                 const char * fn,
                 const char * func,
                 int          ln,
                 const char * msg,
                 ...);
int _check_open3(const char * path,
                 int          flags,
                 int          mode,
                 const char * fn,
                 const char * func,
                 int          ln,
                 const char * msg,
                 ...);

int _check_access(const char * path,
                  int          mode,
                  const char * fn,
                  const char * func,
                  int          ln,
                  const char * msg,
                  ...);

FILE * _safe_fopen(const char * restrict path,
                   const char * restrict mode,
                   const char * fn,
                   const char * func,
                   int          ln);
FILE * _safe_fdopen(int          fd,
                    const char * mode,
                    const char * fn,

                    const char * func,
                    int          ln);
size_t _safe_fread(void * restrict ptr,
                   size_t size,
                   size_t nmemb,
                   FILE * restrict stream,
                   const char * fn,
                   const char * func,
                   int          ln);
size_t _safe_fwrite(const void * restrict ptr,
                    size_t size,
                    size_t nmemb,
                    FILE * restrict stream,
                    const char * fn,
                    const char * func,
                    int          ln);
FILE * _check_fopen(const char * restrict path,
                    const char * restrict mode,
                    const char * fn,
                    const char * func,
                    int          ln,
                    const char * msg,
                    ...);
int    _safe_fclose(FILE * restrict stream,
                    const char * fn,
                    const char * func,
                    int          ln);
#endif
