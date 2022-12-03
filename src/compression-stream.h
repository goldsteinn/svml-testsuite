#ifndef SRC_D_COMPRESSION_STREAM_H_
#define SRC_D_COMPRESSION_STREAM_H_

#include <stdint.h>
#include <zstd.h>
#include "lib/string-custom.h"
#include "util/common.h"
#include "util/error-util.h"

enum { k_cstream_wr = 1, k_cstream_rd = 0 };
enum { k_cstream_done = 1, k_cstream_cont = 0 };

typedef struct cstream_buf {
    void *   buf_;
    uint64_t size_;
} cstream_buf_t;

typedef struct cstream {
    union {
        ZSTD_DStream * reader_;
        ZSTD_CStream * writer_;
    };

    union {
        ZSTD_outBuffer internal_wbuf_;
        ZSTD_inBuffer  internal_rbuf_;
    };
    cstream_buf_t usr_buf_;
    uint64_t      usr_buf_rd_pos_;
    uint64_t      max_sz_in_;
    uint64_t      max_sz_out_;
    uint64_t      file_size_;
    bool          mode_;
    int32_t       fd_;

} cstream_t;


typedef struct cstream_writer {
    void *  buf_;
    int32_t fd_;
} cstream_writer_t;


void NONNULL(1, 2) cstream_init(cstream_t * restrict cstream,
                                char const * restrict path,
                                bool mode);


void NONNULL(1) cstream_shutdown(cstream_t * restrict cstream);

void NONNULL(1) I_cstream_write(cstream_t * cstream);
bool NONNULL(1) I_cstream_read(cstream_t * cstream);


static NONNULL(1, 2) uint64_t cstream_read(cstream_t * restrict cstream,
                                           void * restrict bytes,
                                           uint64_t sz) {
    uint64_t remaining_bytes;
    uint64_t total_read = 0;
    die_assert(cstream->usr_buf_rd_pos_ < cstream->usr_buf_.size_);
    remaining_bytes = cstream->usr_buf_.size_ - cstream->usr_buf_rd_pos_;

    while (1) {
        if (LIKELY(sz < remaining_bytes)) {
            memcpy_c(bytes,
                     AGU_T(cstream->usr_buf_.buf_, cstream->usr_buf_rd_pos_),
                     sz);
            cstream->usr_buf_rd_pos_ += sz;
            return total_read + sz;
        }
        memcpy_c(bytes, AGU_T(cstream->usr_buf_.buf_, cstream->usr_buf_rd_pos_),
                 remaining_bytes);
        sz -= remaining_bytes;
        total_read += remaining_bytes;
        bytes = AGU_T(bytes, remaining_bytes);
        if (UNLIKELY(I_cstream_read(cstream) != k_cstream_cont)) {
            cstream->usr_buf_rd_pos_ += total_read;
            return total_read;
        }

        die_assert(cstream->usr_buf_rd_pos_ == 0);
        die_assert(cstream->usr_buf_.size_ > 0);
        remaining_bytes = cstream->usr_buf_.size_;
    }
}
static NONNULL(1, 2) void safe_cstream_read(cstream_t * restrict cstream,
                                            void * restrict bytes,
                                            uint64_t sz) {
    die_assert(cstream_read(cstream, bytes, sz) == sz);
}

static void
NONNULL(1, 2) cstream_write(cstream_t * restrict cstream,
                            void const * restrict bytes,
                            uint64_t sz) {
    uint64_t remaining_bytes;
    die_assert(cstream->usr_buf_.size_ < cstream->max_sz_in_);
    remaining_bytes = cstream->max_sz_in_ - cstream->usr_buf_.size_;
    while (1) {
        if (LIKELY(sz < remaining_bytes)) {
            memcpy_c(AGU_T(cstream->usr_buf_.buf_, cstream->usr_buf_.size_),
                     bytes, sz);
            cstream->usr_buf_.size_ += sz;
            return;
        }
        memcpy_c(AGU_T(cstream->usr_buf_.buf_, cstream->usr_buf_.size_), bytes,
                 remaining_bytes);
        sz -= remaining_bytes;
        bytes = AGU_T(bytes, remaining_bytes);
        cstream->usr_buf_.size_ += remaining_bytes;
        I_cstream_write(cstream);

        die_assert(cstream->usr_buf_.size_ == 0);
        remaining_bytes = cstream->max_sz_in_;
    }
}

#endif
