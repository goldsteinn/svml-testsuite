#ifndef _SRC__UTIL__FILE_UTIL_H_
#define _SRC__UTIL__FILE_UTIL_H_


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "util/common.h"
#include "util/macro.h"

/* fd operations. */
#define safe_open(...)             CAT(safe_open, PP_ARG_N(__VA_ARGS__))(__VA_ARGS__)
#define safe_read(fd, buf, count)  _safe_read(fd, buf, count, ERR_ARGS)
#define safe_write(fd, buf, count) _safe_write(fd, buf, count, ERR_ARGS)
#define safe_stat(path, buf)       _safe_stat(path, buf, ERR_ARGS)
#define safe_fstat(fd, buf)        _safe_fstat(fd, buf, ERR_ARGS)
#define check_access(path, mode, ...)                                          \
    _check_access(path, mode, ERR_ARGS, __VA_ARGS__)
#define safe_access(path, mode) _safe_access(path, mode, ERR_ARGS)
#define safe_close(fd)          _safe_close(fd, ERR_ARGS)

#define check_open2(path, flags, ...)                                          \
    _check_open2(path, flags, ERR_ARGS, __VA_ARGS__)

#define check_open3(path, flags, mode, ...)                                    \
    _check_open3(path, flags, mode, ERR_ARGS, __VA_ARGS__)

#define safe_open2(path, flags)       _safe_open2(path, flags, ERR_ARGS)
#define safe_open3(path, flags, mode) _safe_open3(path, flags, mode, ERR_ARGS)

/* FILE * operations. */
#define safe_fopen(path, mode) _safe_fopen(path, mode, ERR_ARGS)
#define safe_fdopen(fd, mode)  _safe_fdopen(fd, mode, ERR_ARGS)
#define safe_fread(ptr, size, nmemb, stream)                                   \
    _safe_fread(ptr, size, nmemb, stream, ERR_ARGS)
#define safe_fwrite(ptr, size, nmemb, stream)                                  \
    _safe_fread(ptr, size, nmemb, stream, ERR_ARGS)
#define safe_fclose(stream) _safe_fclose(stream, ERR_ARGS)

#define check_fopen(path, mode, ...)                                           \
    _check_fopen(path, mode, ERR_ARGS, __VA_ARGS__)

/* Declarataions. */

int32_t NONNULL(1, 3, 4) _safe_open2(char const * restrict path,
                                     int32_t flags,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
int32_t NONNULL(1, 4, 5) _safe_open3(char const * restrict path,
                                     int32_t flags,
                                     int32_t mode,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
int64_t NONNULL(2, 4, 5) _safe_read(int32_t fd,
                                    void * restrict buf,
                                    size_t count,
                                    char const * restrict fn,
                                    char const * restrict func,
                                    uint32_t ln);
int64_t NONNULL(2, 4, 5) _safe_write(int32_t fd,
                                     void const * restrict buf,
                                     size_t count,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
int32_t NONNULL(1, 2, 3, 4) _safe_stat(char const * restrict path,
                                       struct stat * restrict buf,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln);
int32_t NONNULL(2, 3, 4) _safe_fstat(int32_t fd,
                                     struct stat * restrict buf,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
int32_t NONNULL(1, 3, 4) _safe_access(char const * restrict path,
                                      int32_t mode,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);
int32_t NONNULL(2, 3) _safe_close(int32_t fd,
                                  char const * restrict fn,
                                  char const * restrict func,
                                  uint32_t ln);
int32_t NONNULL(1, 3, 4, 6) FORMATF(6, 7)
    _check_open2(char const * restrict path,
                 int32_t flags,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln,
                 char const * restrict msg,
                 ...);
int32_t NONNULL(1, 4, 5, 7) FORMATF(7, 8)
    _check_open3(char const * restrict path,
                 int32_t flags,
                 int32_t mode,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln,
                 char const * restrict msg,
                 ...);

int32_t NONNULL(1, 3, 4, 6) FORMATF(6, 7)
    _check_access(char const * restrict path,
                  int32_t mode,
                  char const * restrict fn,
                  char const * restrict func,
                  uint32_t ln,
                  char const * restrict msg,
                  ...);

FILE * NONNULL(1, 2, 3, 4) _safe_fopen(char const * restrict path,
                                       char const * restrict mode,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln);
FILE * NONNULL(2, 3, 4) _safe_fdopen(int32_t fd,
                                     char const * restrict mode,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
size_t NONNULL(1, 4, 5, 6) _safe_fread(void * restrict ptr,
                                       size_t size,
                                       size_t nmemb,
                                       FILE * restrict stream,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln);
size_t NONNULL(1, 4, 5, 6) _safe_fwrite(const void * restrict ptr,
                                        size_t size,
                                        size_t nmemb,
                                        FILE * restrict stream,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);
FILE * NONNULL(1, 2, 3, 4, 6) FORMATF(6, 7)
    _check_fopen(char const * restrict path,
                 char const * restrict mode,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln,
                 char const * restrict msg,
                 ...);
int32_t NONNULL(1, 2, 3) _safe_fclose(FILE * restrict stream,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);


#endif
