#ifndef SRC_D_UTIL_D_FILE_UTIL_H_
#define SRC_D_UTIL_D_FILE_UTIL_H_


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "util/common.h"
#include "util/macro.h"
#include "util/error-util.h"
/* fd operations. */
#define file_exists(...) CAT(I_file_exists, PP_NARG(__VA_ARGS__))(__VA_ARGS__)
#define safe_open(...)   CAT(safe_open, PP_NARG(__VA_ARGS__))(__VA_ARGS__)
#define ensure_open(...) CAT(ensure_open, PP_NARG(__VA_ARGS__))(__VA_ARGS__)

#define safe_read(fd, buf, count)  I_safe_read(fd, buf, count, I_ERR_ARGS)
#define safe_write(fd, buf, count) I_safe_write(fd, buf, count, I_ERR_ARGS)

#define ensure_read(fd, buf, count)  I_ensure_read(fd, buf, count, I_ERR_ARGS)
#define ensure_write(fd, buf, count) I_ensure_write(fd, buf, count, I_ERR_ARGS)

#define safe_stat(path, buf) I_safe_stat(path, buf, I_ERR_ARGS)
#define safe_fstat(fd, buf)  I_safe_fstat(fd, buf, I_ERR_ARGS)
#define check_access(path, mode, ...)                                          \
    _check_access(path, mode, I_ERR_ARGS, __VA_ARGS__)
#define safe_access(path, mode) access(path, mode)
#define safe_close(fd)          I_safe_close(fd, I_ERR_ARGS)
#define ensure_close(fd)        I_ensure_close(fd, I_ERR_ARGS)

#define check_open2(path, flags, ...)                                          \
    I_check_open2(path, flags, I_ERR_ARGS, __VA_ARGS__)

#define check_open3(path, flags, mode, ...)                                    \
    I_check_open3(path, flags, mode, I_ERR_ARGS, __VA_ARGS__)

#define safe_open2(path, flags) I_safe_open2(path, flags, I_ERR_ARGS)
#define safe_open3(path, flags, mode)                                          \
    I_safe_open3(path, flags, mode, I_ERR_ARGS)

#define ensure_open2(path, flags) I_ensure_open2(path, flags, I_ERR_ARGS)
#define ensure_open3(path, flags, mode)                                        \
    I_ensure_open3(path, flags, mode, I_ERR_ARGS)

/* FILE * operations. */
#define safe_fopen(path, mode) I_safe_fopen(path, mode, I_ERR_ARGS)
#define safe_fdopen(fd, mode)  I_safe_fdopen(fd, mode, I_ERR_ARGS)
#define safe_fread(ptr, size, nmemb, stream)                                   \
    I_safe_fread(ptr, size, nmemb, stream, I_ERR_ARGS)
#define safe_fwrite(ptr, size, nmemb, stream)                                  \
    I_safe_fwrite(ptr, size, nmemb, stream, I_ERR_ARGS)
#define safe_fclose(stream) I_safe_fclose(stream, I_ERR_ARGS)

#define check_fopen(path, mode, ...)                                           \
    I_check_fopen(path, mode, I_ERR_ARGS, __VA_ARGS__)


#define safe_fd_readfile(fd, buf, buf_sz)                                      \
    I_safe_fd_readfile(fd, buf, buf_sz, I_ERR_ARGS)
#define safe_fp_readfile(fp, buf, buf_sz)                                      \
    I_safe_fp_readfile(fp, buf, buf_sz, I_ERR_ARGS)
#define safe_readfile(path, buf, buf_sz)                                       \
    I_safe_readfile(path, buf, buf_sz, I_ERR_ARGS)

#define safe_fd_size(fd)   I_safe_fd_size(fd, I_ERR_ARGS)
#define safe_file_size(fd) I_safe_file_size(path, I_ERR_ARGS)
#define safe_fp_size(fp)   I_safe_fp_size(fp, I_ERR_ARGS)

/* Declarataions. */

int32_t NONNULL(1, 3, 4) I_safe_open2(char const * restrict path,
                                      int32_t flags,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);
int32_t NONNULL(1, 4, 5) I_safe_open3(char const * restrict path,
                                      int32_t flags,
                                      int32_t mode,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);

int32_t NONNULL(1, 3, 4) I_ensure_open2(char const * restrict path,
                                        int32_t flags,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);
int32_t NONNULL(1, 4, 5) I_ensure_open3(char const * restrict path,
                                        int32_t flags,
                                        int32_t mode,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);


int64_t NONNULL(2, 4, 5) I_safe_read(int32_t fd,
                                     void * restrict buf,
                                     size_t count,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);
int64_t NONNULL(2, 4, 5) I_safe_write(int32_t fd,
                                      void const * restrict buf,
                                      size_t count,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);

int64_t NONNULL(2, 4, 5) I_ensure_read(int32_t fd,
                                       uint8_t * restrict buf,
                                       size_t count,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln);
int64_t NONNULL(2, 4, 5) I_ensure_write(int32_t fd,
                                        uint8_t const * restrict buf,
                                        size_t count,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);

int32_t NONNULL(1, 2, 3, 4) I_safe_stat(char const * restrict path,
                                        struct stat * restrict stats,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);
int32_t NONNULL(2, 3, 4) I_safe_fstat(int32_t fd,
                                      struct stat * restrict stats,
                                      char const * restrict fn,
                                      char const * restrict func,
                                      uint32_t ln);

int32_t NONNULL(2, 3) I_safe_close(int32_t fd,
                                   char const * restrict fn,
                                   char const * restrict func,
                                   uint32_t ln);
int32_t NONNULL(2, 3) I_ensure_close(int32_t fd,
                                     char const * restrict fn,
                                     char const * restrict func,
                                     uint32_t ln);

int32_t NONNULL(1, 3, 4, 6) FORMATF(6, 7)
    I_check_open2(char const * restrict path,
                  int32_t flags,
                  char const * restrict fn,
                  char const * restrict func,
                  uint32_t ln,
                  char const * restrict msg,
                  ...);
int32_t NONNULL(1, 4, 5, 7) FORMATF(7, 8)
    I_check_open3(char const * restrict path,
                  int32_t flags,
                  int32_t mode,
                  char const * restrict fn,
                  char const * restrict func,
                  uint32_t ln,
                  char const * restrict msg,
                  ...);

int32_t NONNULL(1, 3, 4, 6) FORMATF(6, 7)
    I_check_access(char const * restrict path,
                   int32_t mode,
                   char const * restrict fn,
                   char const * restrict func,
                   uint32_t ln,
                   char const * restrict msg,
                   ...);

FILE * NONNULL(1, 2, 3, 4) I_attr_c_malloc(fclose, 1)
    I_safe_fopen(char const * restrict path,
                 char const * restrict mode,
                 char const * restrict fn,
                 char const * restrict func,
                 uint32_t ln);
FILE * NONNULL(2, 3, 4) I_attr_c_malloc(fclose, 1)
    I_safe_fdopen(int32_t fd,
                  char const * restrict mode,
                  char const * restrict fn,
                  char const * restrict func,
                  uint32_t ln);
size_t NONNULL(1, 4, 5, 6) I_safe_fread(void * restrict ptr,
                                        size_t size,
                                        size_t nmemb,
                                        FILE * restrict stream,
                                        char const * restrict fn,
                                        char const * restrict func,
                                        uint32_t ln);
size_t NONNULL(1, 4, 5, 6) I_safe_fwrite(const void * restrict ptr,
                                         size_t size,
                                         size_t nmemb,
                                         FILE * restrict stream,
                                         char const * restrict fn,
                                         char const * restrict func,
                                         uint32_t ln);
FILE * NONNULL(1, 2, 3, 4, 6) I_attr_c_malloc(fclose, 1) FORMATF(6, 7)
    I_check_fopen(char const * restrict path,
                  char const * restrict mode,
                  char const * restrict fn,
                  char const * restrict func,
                  uint32_t ln,
                  char const * restrict msg,
                  ...);


static NONNULL(1, 2, 3) uint64_t
    I_safe_fstat_size(struct stat const * restrict file_stats,
                      char const * restrict fn,
                      char const * restrict func,
                      uint32_t ln) {
    if (file_stats->st_size < 0) {
        I_die(fn, func, ln, NULL, "Invalid File Size: %ld",
              file_stats->st_size);
    }
    return CAST(uint64_t, file_stats->st_size);
}
static NONNULL(2, 3) uint64_t I_safe_fd_size(int fd,
                                             char const * restrict fn,
                                             char const * restrict func,
                                             uint32_t ln) {
    struct stat file_stats;
    I_safe_fstat(fd, &file_stats, fn, func, ln);
    return I_safe_fstat_size(&file_stats, fn, func, ln);
}

static NONNULL(1, 2, 3) uint64_t I_safe_file_size(char const * restrict path,
                                                  char const * restrict fn,
                                                  char const * restrict func,
                                                  uint32_t ln) {
    struct stat file_stats;
    I_safe_stat(path, &file_stats, fn, func, ln);
    return I_safe_fstat_size(&file_stats, fn, func, ln);
}

static NONNULL(1, 2, 3) uint64_t I_safe_fp_size(FILE * restrict fp,
                                                char const * restrict fn,
                                                char const * restrict func,
                                                uint32_t ln);


int32_t NONNULL(1, 2, 3) I_safe_fclose(FILE * restrict stream,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln);


uint64_t NONNULL(1, 2, 3, 4, 5)
    I_safe_fp_readfile(FILE * restrict fp,
                       void * restrict * restrict buf,
                       uint64_t * restrict buf_sz,
                       char const * restrict fn,
                       char const * restrict func,
                       uint32_t ln);

uint64_t NONNULL(2, 3, 4, 5) I_safe_fd_readfile(int fd,
                                                void * restrict * restrict buf,
                                                uint64_t * restrict buf_sz,
                                                char const * restrict fn,
                                                char const * restrict func,
                                                uint32_t ln);


static uint64_t
NONNULL(1, 2, 3, 4, 5) I_safe_readfile(char const * restrict path,
                                       void * restrict * restrict buf,
                                       uint64_t * restrict buf_sz,
                                       char const * restrict fn,
                                       char const * restrict func,
                                       uint32_t ln) {
    int      fd  = I_ensure_open2(path, O_RDONLY, fn, func, ln);
    uint64_t ret = I_safe_fd_readfile(fd, buf, buf_sz, fn, func, ln);
    I_ensure_close(fd, fn, func, ln);
    return ret;
}



static bool
I_file_exists2(char const * path, int32_t mode) {
    return access(path, mode) == 0;
}

static bool
I_file_exists1(char const * path) {
    return I_file_exists2(path, 0);
}

#endif
