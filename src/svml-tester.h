#ifndef SRC_D_SVML_TESTS_H_
#define SRC_D_SVML_TESTS_H_

#include "svml-defs.h"

void run_svml_tests(svml_op_t const * svml_ops,
                    uint32_t          num_svml_ops,
                    uint32_t          parallel);

#endif
