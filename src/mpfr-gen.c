#include <zstd.h>

#include "compression-stream.h"
#include "mpfr-gen.h"
#include "util/file-util.h"
#include "util/memory-util.h"

static void
mpfr_gen_cmp(char const * restrict dst, svml_op_t const * restrict op) {
    cstream_t cstream;
    uint64_t  i;
    init_mpfr_state();
    cstream_init(&cstream, dst, k_cstream_rd);

    for (i = 0; i == ((uint32_t)i); ++i) {
        float    f_res, f_mpfr, f_mpfr2;
        uint32_t i_res, i_mpfr, i_mpfr2;
        if (i == 1048576) {
            break;
        }
        __builtin_memcpy(&f_res, &i, sizeof(f_res));
        f_mpfr2 = op->mpfr_ref_func_.run_ref_f_f(f_res);
        f_res   = op->ref_func_.run_ref_f_f(f_res);
        __builtin_memcpy(&i_res, &f_res, sizeof(f_res));
        __builtin_memcpy(&i_mpfr2, &f_mpfr2, sizeof(f_res));


        safe_cstream_read(&cstream, &f_mpfr, sizeof(f_mpfr));
        __builtin_memcpy(&i_mpfr, &f_mpfr, sizeof(f_mpfr));

        die_assert(i_mpfr == i_mpfr2);

        if (i_res != i_mpfr) {
            printf("%lu: %x vs %x (%f vs %f)\n", i, i_res, i_mpfr, f_res,
                   f_mpfr);
        }
    }
    cstream_shutdown(&cstream);
    deinit_mpfr_state();
}

void
mpfr_gen_results(char const * restrict dst, svml_op_t const * restrict op) {
    enum {
        k_progress_chunks = 1000,
        k_num_tests       = (1UL << 32),
        k_progress_chunk  = k_num_tests / k_progress_chunks
    };
    cstream_t cstream;
    uint64_t  i;
    uint32_t  progress_cnt = 0;
    uint32_t  progress_val = 0;

    init_mpfr_state();
    cstream_init(&cstream, dst, k_cstream_wr);
    for (i = 0; i == ((uint32_t)i); ++i) {
        float f;
        if (i == progress_val) {
            progress_val += k_progress_chunk;
            fprintf(stderr, "\rProgress %4d/%4d", progress_cnt,
                    k_progress_chunks);
            ++progress_cnt;
        }

        __builtin_memcpy(&f, &i, sizeof(f));
        f = op->mpfr_ref_func_.run_ref_f_f(f);
        cstream_write(&cstream, &f, sizeof(f));
    }

    cstream_shutdown(&cstream);
    deinit_mpfr_state();

    mpfr_gen_cmp(dst, op);
    return;
}
