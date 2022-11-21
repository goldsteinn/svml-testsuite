#include "svml-defs.h"
#include "math-func-defs.h"
#include "svml-bench.h"
#include "svml-func-defs.h"

enum { k_default_ulp = 4 };

#define ADD_ONE_DEF(svml_func, ref_func, sz, unused_cnt, is_fp, ulp,           \
                    test_type)                                                 \
    {                                                                          \
        V_TO_STR(svml_func), V_TO_STR(ref_func) + 4, { svml_func },            \
            { ref_func }, sz, is_fp,                                           \
            (ulp) == 0 ? k_default_ulp : (k_default_ulp),                      \
            CAT(k_test_, test_type), get_bench_funcs(svml_func)                 \
    }

#define ADD_DEF(svml_func, ...)                                                \
    ADD_ONE_DEF(CAT(svml_func, _dev), __VA_ARGS__),                            \
        ADD_ONE_DEF(CAT(svml_func, _glibc), __VA_ARGS__)


const svml_op_t all_svml_defs[] = {
#define SEP ,
#include "svml-raw-defs.h"
};
const uint32_t num_svml_defs = sizeof(all_svml_defs) / sizeof(all_svml_defs[0]);
