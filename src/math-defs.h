#ifndef SRC_D_MATH_DEFS_H_
#define SRC_D_MATH_DEFS_H_

#include <math.h>
#include <mpfr.h>
#include <stdint.h>
#include <tgmath.h>


typedef _Complex double cdouble_t;
typedef _Complex float  cfloat_t;

void deinit_mpfr_state(void);
void init_mpfr_state(void);

#endif
