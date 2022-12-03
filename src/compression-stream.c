#include "compression-stream.h"

#include "lib/string-custom.h"
#include "util/error-util.h"
#include "util/file-util.h"
#include "util/inline-math.h"
#include "util/memory-util.h"

#define cstream_check(ret)                                                     \
    if (UNLIKELY(ZSTD_isError(ret))) {                                         \
        die("%s\n", ZSTD_getErrorName(ret));                                   \
    }

static bool
check_usr_overwrite(char const * path) {
    printf("%s already exists; overwrite (y/n)? ", path);
    return getchar() == 'y';
}

void
cstream_init(cstream_t * restrict cstream,
             char const * restrict path,
             bool mode) {
    int32_t  fd;
    void *   buf_;
    uint64_t sz_in, sz_out;
    memset_c(cstream, 0, sizeof(cstream_t));

    if (mode == k_cstream_rd) {
        die_assert(file_exists(path, O_RDONLY));
        fd               = ensure_open(path, O_RDONLY);
        sz_in            = ZSTD_DStreamInSize();
        sz_out           = ZSTD_DStreamOutSize();
        cstream->reader_ = ZSTD_createDStream();
        die_assert(cstream->reader_ != NULL);
    }
    else {
        if (file_exists(path)) {
            die_assert(check_usr_overwrite(path));
        }
        fd               = ensure_open(path, O_CREAT | O_WRONLY | O_TRUNC,
                                       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        sz_in            = (32 * 1024);
        sz_out           = (256 * 1024 * 1024);
        cstream->writer_ = ZSTD_createCStream();
        die_assert(cstream->writer_ != NULL);
        cstream_check(ZSTD_initCStream(cstream->writer_, 19));
    }
    sz_in  = ROUNDUP_P2(sz_in, 4096UL);
    sz_out = ROUNDUP_P2(sz_out, 4096UL);

    buf_                 = safe_aligned_alloc(4096, sz_in + sz_out);
    cstream->max_sz_in_  = sz_in;
    cstream->max_sz_out_ = sz_out;

    cstream->mode_ = mode;
    cstream->fd_   = fd;

    if (mode == k_cstream_rd) {
        uint64_t init_rd;
        cstream->file_size_ = safe_fd_size(fd);
        init_rd             = MIN(cstream->file_size_, sz_in);
        cstream->file_size_ -= init_rd;
        ensure_read(fd, buf_, init_rd);
        cstream->internal_rbuf_ = (ZSTD_inBuffer){ buf_, init_rd, 0 };
        cstream->usr_buf_ = (cstream_buf_t){ (void *)AGU(buf_, sz_in), 0 };
        I_cstream_read(cstream);
    }

    if (mode == k_cstream_wr) {
        cstream->internal_wbuf_ = (ZSTD_outBuffer){ buf_, sz_out, 0 };
        cstream->usr_buf_   = (cstream_buf_t){ (void *)AGU(buf_, sz_out), 0 };
        cstream->file_size_ = 0;
    }
}

static void
cstream_writeback(cstream_t * cstream) {
    die_assert(cstream->mode_ == k_cstream_wr);
    die_assert(cstream->internal_wbuf_.pos <= cstream->internal_wbuf_.size);
    die_assert(cstream->internal_wbuf_.size == cstream->max_sz_out_);

    if (cstream->internal_wbuf_.pos) {
        cstream->file_size_ += cstream->internal_wbuf_.pos;
        ensure_write(cstream->fd_, cstream->internal_wbuf_.dst,
                     cstream->internal_wbuf_.pos);

        cstream->internal_wbuf_.pos = 0;
    }
}

static void
cstream_maybe_writeback(cstream_t * cstream) {
    die_assert(cstream->mode_ == k_cstream_wr);
    if (cstream->internal_wbuf_.pos == cstream->internal_wbuf_.size) {
        cstream_writeback(cstream);
    }
}

void
cstream_shutdown(cstream_t * cstream) {
    if (cstream->mode_ == k_cstream_wr) {
        size_t zstd_ret;
        if (cstream->usr_buf_.size_) {
            I_cstream_write(cstream);
        }
        die_assert(cstream->internal_wbuf_.pos < cstream->internal_wbuf_.size);
        die_assert(cstream->internal_wbuf_.size == cstream->max_sz_out_);

        do {
            zstd_ret =
                ZSTD_endStream(cstream->writer_, &(cstream->internal_wbuf_));
            cstream_maybe_writeback(cstream);
        } while (zstd_ret);
        cstream_writeback(cstream);

        cstream_check(ZSTD_freeCStream(cstream->writer_));
    }
    else {
        cstream_check(ZSTD_freeDStream(cstream->reader_));
    }


    safe_close(cstream->fd_);
    safe_sfree(cstream->internal_wbuf_.dst,
               cstream->max_sz_in_ + cstream->max_sz_out_);
}


void
I_cstream_write(cstream_t * cstream) {
    ZSTD_inBuffer buf_in;
    die_assert(cstream->mode_ == k_cstream_wr);
    die_assert(cstream->internal_wbuf_.pos < cstream->internal_wbuf_.size);
    die_assert(cstream->internal_wbuf_.size == cstream->max_sz_out_);

    die_assert(cstream->usr_buf_.buf_ != NULL);
    die_assert(cstream->usr_buf_.size_ > 0 &&
               cstream->usr_buf_.size_ <= cstream->max_sz_in_);


    buf_in =
        (ZSTD_inBuffer){ cstream->usr_buf_.buf_, cstream->usr_buf_.size_, 0 };

    do {
        uint64_t before_c = cstream->internal_wbuf_.pos;
        uint64_t before_d = buf_in.pos;

        cstream_check(ZSTD_compressStream(cstream->writer_,
                                          &(cstream->internal_wbuf_), &buf_in));
        before_c = cstream->internal_wbuf_.pos - before_c;
        before_d = buf_in.pos - before_d;

        //        fprintf(stderr, "Compression Ratio: %lf\n", ((double)before_c) / ((double)before_d));


        cstream_maybe_writeback(cstream);
    } while (buf_in.pos < buf_in.size);
    die_assert(buf_in.pos == buf_in.size);

    cstream->usr_buf_.size_ = 0;
}

bool
I_cstream_read(cstream_t * cstream) {
    ZSTD_outBuffer buf_out;
    die_assert(cstream->mode_ == k_cstream_rd);
    die_assert(cstream->internal_rbuf_.size <= cstream->max_sz_in_);

    cstream->usr_buf_rd_pos_ = 0;
    if (cstream->file_size_ == 0 &&
        cstream->internal_rbuf_.pos == cstream->internal_rbuf_.size) {
        cstream->usr_buf_.size_ = 0;
        return k_cstream_done;
    }


    buf_out =
        (ZSTD_outBuffer){ cstream->usr_buf_.buf_, cstream->max_sz_out_, 0 };

    do {
        size_t zstd_ret;

        zstd_ret = ZSTD_decompressStream(cstream->reader_, &buf_out,
                                         &(cstream->internal_rbuf_));
        cstream_check(zstd_ret);


        if (cstream->internal_rbuf_.pos >= cstream->internal_rbuf_.size) {
            die_assert(cstream->internal_rbuf_.pos ==
                       cstream->internal_rbuf_.size);


            if (cstream->file_size_ == 0) {
                break;
            }

            if (cstream->file_size_ < cstream->internal_rbuf_.size) {
                die_assert(cstream->internal_rbuf_.size == cstream->max_sz_in_);
                cstream->internal_rbuf_.size = cstream->file_size_;
            }

            ensure_read(cstream->fd_, cstream->internal_wbuf_.dst,
                        cstream->internal_rbuf_.size);
            cstream->file_size_ -= cstream->internal_rbuf_.size;
            cstream->internal_rbuf_.pos = 0;
            die_assert(cstream->internal_rbuf_.size == cstream->max_sz_in_ ||
                       cstream->file_size_ == 0);
        }
    } while (buf_out.pos != buf_out.size);


    die_assert(buf_out.pos > 0);
    cstream->usr_buf_.size_ = buf_out.pos;
    return k_cstream_cont;
}
