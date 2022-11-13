#include "lib/stats/stats.h"

#include "util/macro.h"

#include "test/test-common.h"


#define stats_test_name(T) CAT(stats_test_, T)
#define run_test(T)        test_assert(stats_test_name(T)() == 0)
#define make_stats_test(T)                                                        \
    static int32_t stats_test_name(T)(void) {                                     \
        typedef struct stats_tests { /* NOLINT(altera-struct-pack-align) */       \
            const stats_result_t expec;                                           \
            uint64_t             arr_size;                                        \
            T                    arr[256];                                        \
        } stats_tests_t;                                                          \
                                                                                  \
        static const stats_tests_t tests_nb[] = {                                 \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 100.000000,                            \
                            .median      = 50.500000,                             \
                            .mean        = 50.500000,                             \
                            .geomean     = 37.992689,                             \
                            .stdev       = 29.011492,                             \
                            .variance    = 841.666667,                            \
                            .percentiles = { 1.0, 1.99, 5.95, 10.9, 25.75, 33.67, \
                                             50.5, 66.34, 75.25,                  \
                                             90.10000000000001, 95.05, 99.01,     \
                                             100.0 } },                           \
              .arr_size = 100,                                                    \
              .arr = { (T)1,  (T)2,  (T)3,  (T)4,  (T)5,  (T)6,  (T)7,  (T)8,     \
                       (T)9,  (T)10, (T)11, (T)12, (T)13, (T)14, (T)15, (T)16,    \
                       (T)17, (T)18, (T)19, (T)20, (T)21, (T)22, (T)23, (T)24,    \
                       (T)25, (T)26, (T)27, (T)28, (T)29, (T)30, (T)31, (T)32,    \
                       (T)33, (T)34, (T)35, (T)36, (T)37, (T)38, (T)39, (T)40,    \
                       (T)41, (T)42, (T)43, (T)44, (T)45, (T)46, (T)47, (T)48,    \
                       (T)49, (T)50, (T)51, (T)52, (T)53, (T)54, (T)55, (T)56,    \
                       (T)57, (T)58, (T)59, (T)60, (T)61, (T)62, (T)63, (T)64,    \
                       (T)65, (T)66, (T)67, (T)68, (T)69, (T)70, (T)71, (T)72,    \
                       (T)73, (T)74, (T)75, (T)76, (T)77, (T)78, (T)79, (T)80,    \
                       (T)81, (T)82, (T)83, (T)84, (T)85, (T)86, (T)87, (T)88,    \
                       (T)89, (T)90, (T)91, (T)92, (T)93, (T)94, (T)95, (T)96,    \
                       (T)97, (T)98, (T)99, (T)100 } },                           \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 99.000000,                             \
                            .median      = 50.000000,                             \
                            .mean        = 50.000000,                             \
                            .geomean     = 37.623100,                             \
                            .stdev       = 28.722813,                             \
                            .variance    = 825.000000,                            \
                            .percentiles = { 1.0, 1.98, 5.9, 10.8, 25.5, 33.34,   \
                                             50.0, 65.68, 74.5, 89.2, 94.1,       \
                                             98.02, 99.0 } },                     \
              .arr_size = 99,                                                     \
              .arr = { (T)1,  (T)2,  (T)3,  (T)4,  (T)5,  (T)6,  (T)7,  (T)8,     \
                       (T)9,  (T)10, (T)11, (T)12, (T)13, (T)14, (T)15, (T)16,    \
                       (T)17, (T)18, (T)19, (T)20, (T)21, (T)22, (T)23, (T)24,    \
                       (T)25, (T)26, (T)27, (T)28, (T)29, (T)30, (T)31, (T)32,    \
                       (T)33, (T)34, (T)35, (T)36, (T)37, (T)38, (T)39, (T)40,    \
                       (T)41, (T)42, (T)43, (T)44, (T)45, (T)46, (T)47, (T)48,    \
                       (T)49, (T)50, (T)51, (T)52, (T)53, (T)54, (T)55, (T)56,    \
                       (T)57, (T)58, (T)59, (T)60, (T)61, (T)62, (T)63, (T)64,    \
                       (T)65, (T)66, (T)67, (T)68, (T)69, (T)70, (T)71, (T)72,    \
                       (T)73, (T)74, (T)75, (T)76, (T)77, (T)78, (T)79, (T)80,    \
                       (T)81, (T)82, (T)83, (T)84, (T)85, (T)86, (T)87, (T)88,    \
                       (T)89, (T)90, (T)91, (T)92, (T)93, (T)94, (T)95, (T)96,    \
                       (T)97, (T)98, (T)99 } },                                   \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 14.000000,                             \
                            .median      = 7.500000,                              \
                            .mean        = 7.500000,                              \
                            .geomean     = 6.045855,                              \
                            .stdev       = 4.183300,                              \
                            .variance    = 17.500000,                             \
                            .percentiles = { 1.0, 1.13, 1.65, 2.3, 4.25, 5.29,    \
                                             7.5, 9.58, 10.75,                    \
                                             12.700000000000001, 13.35, 13.87,    \
                                             14.0 } },                            \
              .arr_size = 14,                                                     \
              .arr = { (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7, (T)8, (T)9,      \
                       (T)10, (T)11, (T)12, (T)13, (T)14 } },                     \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 100.000000,                            \
                            .median      = 50.500000,                             \
                            .mean        = 50.500000,                             \
                            .geomean     = 37.992689,                             \
                            .stdev       = 29.011492,                             \
                            .variance    = 841.666667,                            \
                            .percentiles = { 1.0, 1.99, 5.95, 10.9, 25.75, 33.67, \
                                             50.5, 66.34, 75.25,                  \
                                             90.10000000000001, 95.05, 99.01,     \
                                             100.0 } },                           \
              .arr_size = 100,                                                    \
              .arr = { (T)1,  (T)2,  (T)3,  (T)4,  (T)5,  (T)6,  (T)7,  (T)8,     \
                       (T)9,  (T)10, (T)11, (T)12, (T)13, (T)14, (T)15, (T)16,    \
                       (T)17, (T)18, (T)19, (T)20, (T)21, (T)22, (T)23, (T)24,    \
                       (T)25, (T)26, (T)27, (T)28, (T)29, (T)30, (T)31, (T)32,    \
                       (T)33, (T)34, (T)35, (T)36, (T)37, (T)38, (T)39, (T)40,    \
                       (T)41, (T)42, (T)43, (T)44, (T)45, (T)46, (T)47, (T)48,    \
                       (T)49, (T)50, (T)51, (T)52, (T)53, (T)54, (T)55, (T)56,    \
                       (T)57, (T)58, (T)59, (T)60, (T)61, (T)62, (T)63, (T)64,    \
                       (T)65, (T)66, (T)67, (T)68, (T)69, (T)70, (T)71, (T)72,    \
                       (T)73, (T)74, (T)75, (T)76, (T)77, (T)78, (T)79, (T)80,    \
                       (T)81, (T)82, (T)83, (T)84, (T)85, (T)86, (T)87, (T)88,    \
                       (T)89, (T)90, (T)91, (T)92, (T)93, (T)94, (T)95, (T)96,    \
                       (T)97, (T)98, (T)99, (T)100 } },                           \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 99.000000,                             \
                            .median      = 50.000000,                             \
                            .mean        = 50.000000,                             \
                            .geomean     = 37.623100,                             \
                            .stdev       = 28.722813,                             \
                            .variance    = 825.000000,                            \
                            .percentiles = { 1.0, 1.98, 5.9, 10.8, 25.5, 33.34,   \
                                             50.0, 65.68, 74.5, 89.2, 94.1,       \
                                             98.02, 99.0 } },                     \
              .arr_size = 99,                                                     \
              .arr = { (T)1,  (T)2,  (T)3,  (T)4,  (T)5,  (T)6,  (T)7,  (T)8,     \
                       (T)9,  (T)10, (T)11, (T)12, (T)13, (T)14, (T)15, (T)16,    \
                       (T)17, (T)18, (T)19, (T)20, (T)21, (T)22, (T)23, (T)24,    \
                       (T)25, (T)26, (T)27, (T)28, (T)29, (T)30, (T)31, (T)32,    \
                       (T)33, (T)34, (T)35, (T)36, (T)37, (T)38, (T)39, (T)40,    \
                       (T)41, (T)42, (T)43, (T)44, (T)45, (T)46, (T)47, (T)48,    \
                       (T)49, (T)50, (T)51, (T)52, (T)53, (T)54, (T)55, (T)56,    \
                       (T)57, (T)58, (T)59, (T)60, (T)61, (T)62, (T)63, (T)64,    \
                       (T)65, (T)66, (T)67, (T)68, (T)69, (T)70, (T)71, (T)72,    \
                       (T)73, (T)74, (T)75, (T)76, (T)77, (T)78, (T)79, (T)80,    \
                       (T)81, (T)82, (T)83, (T)84, (T)85, (T)86, (T)87, (T)88,    \
                       (T)89, (T)90, (T)91, (T)92, (T)93, (T)94, (T)95, (T)96,    \
                       (T)97, (T)98, (T)99 } },                                   \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 9.000000,                              \
                            .median      = 5.000000,                              \
                            .mean        = 5.000000,                              \
                            .geomean     = 4.147166,                              \
                            .stdev       = 2.738613,                              \
                            .variance    = 7.500000,                              \
                            .percentiles = { 1.0, 1.08, 1.4, 1.8, 3.0, 3.64, 5.0, \
                                             6.28, 7.0, 8.2, 8.6, 8.92, 9.0 } },  \
              .arr_size = 9,                                                      \
              .arr      = { (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7, (T)8,       \
                            (T)9 } },                                                  \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 14.000000,                             \
                            .median      = 7.500000,                              \
                            .mean        = 7.500000,                              \
                            .geomean     = 6.045855,                              \
                            .stdev       = 4.183300,                              \
                            .variance    = 17.500000,                             \
                            .percentiles = { 1.0, 1.13, 1.65, 2.3, 4.25, 5.29,    \
                                             7.5, 9.58, 10.75,                    \
                                             12.700000000000001, 13.35, 13.87,    \
                                             14.0 } },                            \
              .arr_size = 14,                                                     \
              .arr = { (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7, (T)8, (T)9,      \
                       (T)10, (T)11, (T)12, (T)13, (T)14 } },                     \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 10.000000,                             \
                            .median      = 5.500000,                              \
                            .mean        = 5.500000,                              \
                            .geomean     = 4.528729,                              \
                            .stdev       = 3.027650,                              \
                            .variance    = 9.166667,                              \
                            .percentiles = { 1.0, 1.09, 1.45, 1.9, 3.25, 3.97,    \
                                             5.5, 6.94, 7.75, 9.1,                \
                                             9.549999999999999, 9.91, 10.0 } },   \
              .arr_size = 10,                                                     \
              .arr = { (T)1, (T)2, (T)3, (T)4, (T)5, (T)6, (T)7, (T)8, (T)9,      \
                       (T)10 } },                                                 \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 10.000000,                             \
                            .max         = 11.000000,                             \
                            .median      = 10.500000,                             \
                            .mean        = 10.500000,                             \
                            .geomean     = 10.488088,                             \
                            .stdev       = 0.707107,                              \
                            .variance    = 0.500000,                              \
                            .percentiles = { 10.0, 10.01, 10.05, 10.1, 10.25,     \
                                             10.33, 10.5, 10.66, 10.75, 10.9,     \
                                             10.95, 10.99, 11.0 } },              \
              .arr_size = 2,                                                      \
              .arr      = { (T)10, (T)11 } },                                          \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 10.000000,                             \
                            .max         = 10.000000,                             \
                            .median      = 10.000000,                             \
                            .mean        = 10.000000,                             \
                            .geomean     = 10.000000,                             \
                            .stdev       = 0.000000,                              \
                            .variance    = 0.000000,                              \
                            .percentiles = { 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,  \
                                             10.0, 10.0, 10.0, 10.0, 10.0, 10.0,  \
                                             10.0 } },                            \
              .arr_size = 1,                                                      \
              .arr      = { (T)10 } }                                             \
        };                                                                        \
        static const stats_tests_t tests_b[] = {                                  \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 1.000000,                              \
                            .max         = 1.000000,                              \
                            .median      = 1.000000,                              \
                            .mean        = 1.000000,                              \
                            .geomean     = 1.000000,                              \
                            .stdev       = 0.000000,                              \
                            .variance    = 0.000000,                              \
                            .percentiles = { 1.0, 1.00, 1.00, 1.00, 1.00, 1.00,   \
                                             1.00, 1.00, 1.00, 1.00, 1.00, 1.00,  \
                                             1.00 } },                            \
              .arr_size = 4,                                                      \
              .arr      = { true, true, true, true } },                                \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 0.000000,                              \
                            .max         = 1.000000,                              \
                            .median      = 0.500000,                              \
                            .mean        = 0.500000,                              \
                            .geomean     = 0.000000,                              \
                            .stdev       = 0.707107,                              \
                            .variance    = 0.500000,                              \
                            .percentiles = { 0.0, 0.01, 0.05, 0.10, 0.25, 0.33,   \
                                             0.50, 0.66, 0.75, 0.90, 0.95, 0.99,  \
                                             1.00 } },                            \
              .arr_size = 2,                                                      \
              .arr      = { false, true } },                                           \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 0.000000,                              \
                            .max         = 1.000000,                              \
                            .median      = 0.500000,                              \
                            .mean        = 0.500000,                              \
                            .geomean     = 0.000000,                              \
                            .stdev       = 0.577350,                              \
                            .variance    = 0.333333,                              \
                            .percentiles = { 0.0, 0.00, 0.00, 0.00, 0.00, 0.00,   \
                                             0.50, 0.98, 1.00, 1.00, 1.00, 1.00,  \
                                             1.00 } },                            \
              .arr_size = 4,                                                      \
              .arr      = { false, true, false, true } },                              \
            { .expec    = { .desc        = NULL,                                  \
                            .has_stats   = 1,                                     \
                            .min         = 0.000000,                              \
                            .max         = 1.000000,                              \
                            .median      = 0.000000,                              \
                            .mean        = 0.333333,                              \
                            .geomean     = 0.000000,                              \
                            .stdev       = 0.577350,                              \
                            .variance    = 0.333333,                              \
                            .percentiles = { 0.0, 0.00, 0.00, 0.00, 0.00, 0.00,   \
                                             0.00, 0.32, 0.50, 0.80, 0.90, 0.98,  \
                                             1.00 } },                            \
              .arr_size = 3,                                                      \
              .arr      = { false, true, false } },                                    \
        };                                                                        \
                                                                                  \
        uint32_t ntests_nb = sizeof(tests_nb) / sizeof(tests_nb[0]);              \
        uint32_t ntests_b  = sizeof(tests_b) / sizeof(tests_b[0]);                \
        uint32_t ntests    = GET_IS_BOOL_BASE(T) ? ntests_b : ntests_nb;          \
        const stats_tests_t * tests =                                             \
            GET_IS_BOOL_BASE(T) ? tests_b : tests_nb;                             \
        uint32_t       i, j;                                                      \
        stats_result_t res;                                                       \
        enum { DEFAULT_BUFLEN = 2048 };                                           \
        char buf[DEFAULT_BUFLEN];                                                 \
        if (is_same_type(T, long_double)) {                                       \
            return 0;                                                             \
        }                                                                         \
        for (i = 0; i < ntests; ++i) {                                            \
            gen_stats(&res, tests[i].arr, tests[i].arr_size);                     \
                                                                                  \
            test_assert(float_eq(stats_get_min(&res),                             \
                                 stats_get_min(&(tests[i].expec))),               \
                        "stats_get_min(%u): %lf != %lf\n", i,                     \
                        stats_get_min(&res),                                      \
                        stats_get_min(&(tests[i].expec)));                        \
            test_assert(float_eq(stats_get_max(&res),                             \
                                 stats_get_max(&(tests[i].expec))),               \
                        "stats_get_max(%u): %lf != %lf\n", i,                     \
                        stats_get_max(&res),                                      \
                        stats_get_max(&(tests[i].expec)));                        \
            test_assert(float_eq(stats_get_median(&res),                          \
                                 stats_get_median(&(tests[i].expec))),            \
                        "stats_get_median(%u): %lf != %lf\n", i,                  \
                        stats_get_median(&res),                                   \
                        stats_get_median(&(tests[i].expec)));                     \
            test_assert(float_eq(stats_get_mean(&res),                            \
                                 stats_get_mean(&(tests[i].expec))),              \
                        "stats_get_mean(%u): %lf != %lf\n", i,                    \
                        stats_get_mean(&res),                                     \
                        stats_get_mean(&(tests[i].expec)));                       \
            test_assert(float_eq(stats_get_geomean(&res),                         \
                                 stats_get_geomean(&(tests[i].expec))),           \
                        "stats_get_geomean(%u): %lf != %lf\n", i,                 \
                        stats_get_geomean(&res),                                  \
                        stats_get_geomean(&(tests[i].expec)));                    \
            test_assert(float_eq(stats_get_stdev(&res),                           \
                                 stats_get_stdev(&(tests[i].expec))),             \
                        "stats_get_stdev(%u): %lf != %lf\n", i,                   \
                        stats_get_stdev(&res),                                    \
                        stats_get_stdev(&(tests[i].expec)));                      \
            test_assert(float_eq(stats_get_variance(&res),                        \
                                 stats_get_variance(&(tests[i].expec))),          \
                        "stats_get_variance(%u): %lf != %lf\n", i,                \
                        stats_get_variance(&res),                                 \
                        stats_get_variance(&(tests[i].expec)));                   \
                                                                                  \
            for (j = 0; j < num_percentiles(); ++j) {                             \
                test_assert(                                                      \
                    float_eq(stats_get_percentile(&res, idx_to_percentile(j)),    \
                             stats_get_percentile(&(tests[i].expec),              \
                                                  idx_to_percentile(j))),         \
                    "stats_get_percentile(%u): %lf != %lf\n", i,                  \
                    stats_get_percentile(&res, idx_to_percentile(j)),             \
                    stats_get_percentile(&(tests[i].expec),                       \
                                         idx_to_percentile(j)));                  \
            }                                                                     \
            /* Just to test the interfaces. */                                    \
            stats_prints_csv(buf, DEFAULT_BUFLEN, &res, STATS_P_all,              \
                             STATS_SUPPORTED_PERCENTILES);                        \
            stats_prints(buf, DEFAULT_BUFLEN, &res, STATS_P_all,                  \
                         STATS_SUPPORTED_PERCENTILES);                            \
            stats_prints_cmp(buf, DEFAULT_BUFLEN, &res, &(tests[i].expec),        \
                             STATS_P_all, STATS_SUPPORTED_PERCENTILES);           \
        }                                                                         \
        return 0;                                                                 \
    }


APPLY(make_stats_test, ;, ALL_TYPE_NAMES);

int32_t test_stats(void);
int32_t
test_stats(void) {
    set_max_ulp(-1);
    APPLY(run_test, ;, ALL_TYPE_NAMES);
    return 0;
}
