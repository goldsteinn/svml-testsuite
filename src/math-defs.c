#include "math-defs.h"

#include <float.h>
#include <mpfr.h>
#include <stdint.h>
#include <string.h>

__thread mpfr_t G_d_op0_;
__thread mpfr_t G_d_op1_;
__thread mpfr_t G_d_op2_;
__thread mpfr_t G_d_op3_;
__thread mpfr_t G_flt_op0_;
__thread mpfr_t G_flt_op1_;
__thread mpfr_t G_flt_op2_;
__thread mpfr_t G_flt_op3_;
__thread mpfr_t G_ld_op0_;
__thread mpfr_t G_si_op0_;

void
init_mpfr_state(void) {
    mpfr_init2(G_d_op0_, 53);
    mpfr_init2(G_d_op1_, 53);
    mpfr_init2(G_d_op2_, 53);
    mpfr_init2(G_d_op3_, 53);

    mpfr_init2(G_flt_op0_, 24);
    mpfr_init2(G_flt_op1_, 24);
    mpfr_init2(G_flt_op2_, 24);
    mpfr_init2(G_flt_op3_, 24);

    mpfr_init2(G_ld_op0_, LDBL_MANT_DIG + 1);
    mpfr_init_set_ui(G_si_op0_, 0, MPFR_RNDN);
}

void
deinit_mpfr_state(void) {
    mpfr_clear(G_d_op0_);
    mpfr_clear(G_d_op1_);
    mpfr_clear(G_d_op2_);
    mpfr_clear(G_d_op3_);

    mpfr_clear(G_flt_op0_);
    mpfr_clear(G_flt_op1_);
    mpfr_clear(G_flt_op2_);
    mpfr_clear(G_flt_op3_);

    mpfr_clear(G_ld_op0_);
    mpfr_clear(G_si_op0_);
    mpfr_free_cache();
}
