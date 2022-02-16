#ifndef _SRC__UTIL__FILE_UTIL_H_
#define _SRC__UTIL__FILE_UTIL_H_


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

int32_t _safe_open2(char const * restrict path,
                    int32_t flags,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
int32_t _safe_open3(char const * restrict path,
                    int32_t flags,
                    int32_t mode,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
int64_t _safe_read(int32_t fd,
                   void *  buf,
                   size_t  count,
                   char const * restrict fn,
                   char const * restrict func,
                   int32_t ln);
int64_t _safe_write(int32_t fd,
                    void const * restrict buf,
                    size_t count,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
int32_t _safe_stat(char const * restrict path,
                   struct stat * restrict buf,
                   char const * restrict fn,
                   char const * restrict func,
                   int32_t ln);
int32_t _safe_fstat(int32_t fd,
                    struct stat * restrict buf,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
int32_t _safe_access(char const * restrict path,
                     int32_t mode,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln);
int32_t _safe_close(int32_t fd,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
int32_t _check_open2(char const * restrict path,
                     int32_t flags,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln,
                     char const * restrict msg,
                     ...);
int32_t _check_open3(char const * restrict path,
                     int32_t flags,
                     int32_t mode,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln,
                     char const * restrict msg,
                     ...);

int32_t _check_access(char const * restrict path,
                      int32_t mode,
                      char const * restrict fn,
                      char const * restrict func,
                      int32_t ln,
                      char const * restrict msg,
                      ...);

FILE *  _safe_fopen(char const * restrict path,
                    char const * restrict mode,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
FILE *  _safe_fdopen(int32_t fd,
                     char const * restrict mode,
                     char const * restrict fn,

                     char const * restrict func,
                     int32_t ln);
size_t  _safe_fread(void * restrict ptr,
                    size_t size,
                    size_t nmemb,
                    FILE * restrict stream,
                    char const * restrict fn,
                    char const * restrict func,
                    int32_t ln);
size_t  _safe_fwrite(const void * restrict ptr,
                     size_t size,
                     size_t nmemb,
                     FILE * restrict stream,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln);
FILE *  _check_fopen(char const * restrict path,
                     char const * restrict mode,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln,
                     char const * restrict msg,
                     ...);
int32_t _safe_fclose(FILE * restrict stream,
                     char const * restrict fn,
                     char const * restrict func,
                     int32_t ln);


#endif
