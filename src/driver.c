#include <fnmatch.h>

#include "util/arg.h"
#include "util/common.h"
#include "util/error-util.h"
#include "util/memory-util.h"
#include "util/verbosity.h"

#include "sys/sys-info.h"

#include "svml-benchmarker.h"
#include "svml-def-helpers.h"
#include "svml-defs.h"
#include "svml-tester.h"


static int32_t verbosity;

static int32_t      test  = 0;
static int32_t      bench = 0;
static int32_t      list  = 0;
static char const * todo  = NULL;


static bench_conf_t bench_conf = { 0 };
static uint32_t     bench_lat  = 0;
static uint32_t     bench_tput = 0;
static uint32_t     bench_cpu  = ~0u;
static char const * bench_todo = NULL;
static char const * init_list  = NULL;
static uint32_t     parallel   = 0;

static int32_t      run_all    = 0;
static arg_rest_t   test_names = INIT_ARG_REST_T;
static arg_option_t args[]     = {
    /* ADD_ARG(Kind, Method, name, reqd, variable, help) */
    ADD_ARG(KindOption, Integer, "-v", 0, &verbosity, "Turn on verbosity"),
    ADD_ARG(KindOption, Set, "--list", 0, &list, "todo=list"),
    ADD_ARG(KindOption, Set, "--test", 0, &test, "todo=test"),
    ADD_ARG(KindOption, Set, "--bench", 0, &bench, "todo=bench"),
    ADD_ARG(KindOption,
                Integer,
                ("-c", "--cpu"),
                0,
                &(bench_conf.cpu_),
                "Bench cpu"),
    ADD_ARG(KindOption,
                Set,
                ("--lat", "--bench-lat"),
                -0,
                &bench_lat,
                "Measure latency"),
    ADD_ARG(KindOption,
                Set,
                ("--tput", "--bench-tput"),
                -0,
                &bench_tput,
                "Measure throughput"),
    ADD_ARG(KindOption,
                String,
                ("--bench-todo",
             "--bench-todo=",
             "--measure",
             "--measure=",
             "-m",
             "-m="),
                -0,
                &bench_todo,
                "Measure throughput"),
    ADD_ARG(KindOption,
                Integer,
                ("-t", "--trials"),
                -0,
                &(bench_conf.trials_),
                "trials (-1 uses default)"),
    ADD_ARG(KindOption,
                Integer,
                "--init0",
                0,
                &(bench_conf.init_vals_[0]),
                "Hex value to init 0th vec with"),
    ADD_ARG(KindOption,
                Integer,
                "--init1",
                0,
                &(bench_conf.init_vals_[1]),
                "Hex value to init 1th vec with"),
    ADD_ARG(KindOption,
                Integer,
                "--init2",
                0,
                &(bench_conf.init_vals_[2]),
                "Hex value to init 2th vec with"),
    ADD_ARG(KindOption,
                String,
                ("--init-list", "--initl", "-i"),
                0,
                &init_list,
                "CSV of hex values to init values 0-N with"),
    ADD_ARG(KindOption,
                String,
                ("--todo", "--todo="),
                0,
                &todo,
                "todo <bench|test|list>"),
    ADD_ARG(KindOption,
                Integer,
                ("-j", "--parallel"),
                0,
                &parallel,
                "Set parallelism"),
    ADD_ARG(KindOption, Set, "--all", 0, &run_all, "Run all tests"),
    ADD_ARG(KindHelp, Help, "-h", 0, NULL, ""),
    ADD_ARG(KindRest,
                Rest,
                "",
                0,
                &test_names,
                "Tests to run. Just include at end"),
    ADD_ARG(KindEnd, EndOptions, "", 0, NULL, "")
};


static arg_defs_t argp = { args, "C / ASM Wrapper", NULL, NULL };

int
main(int argc, char * argv[]) {
    uint32_t          i, j, num_to_run, num_svml_ops_to_run;
    bool              should_free;
    char * const *    to_run;
    svml_op_t *       svml_ops_to_run_builder;
    svml_op_t const * svml_ops_to_run;
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);

    to_run     = run_all ? NULL : test_names.ptrs;
    num_to_run = CAST(uint32_t, test_names.n);

    if (todo != NULL) {
        if (!strcasecmp(todo, "test")) {
            test = 1;
        }
        if (!strcasecmp(todo, "bench")) {
            bench = 1;
        }
        if (!strcasecmp(todo, "list")) {
            list = 1;
        }
    }
    if (bench_todo != NULL) {
        if (!strcasecmp(bench_todo, "latency") ||
            !strcasecmp(bench_todo, "lat") || !strcasecmp(bench_todo, "l")) {
            bench_conf.type_ |= k_bench_lat;
        }
        if (!strcasecmp(bench_todo, "throughput") ||
            !strcasecmp(bench_todo, "tput") || !strcasecmp(bench_todo, "t")) {
            bench_conf.type_ |= k_bench_tput;
        }
    }
    bench_conf.cpu_ =
        CAST(uint16_t,
             bench_cpu > get_num_cpus() ? get_num_cpus() : (uint16_t)bench_cpu);

    if (bench_lat) {
        bench_conf.type_ |= k_bench_lat;
    }
    if (bench_tput) {
        bench_conf.type_ |= k_bench_tput;
    }

    if (init_list != NULL) {
        char * next;
        next = (char *)(uintptr_t)init_list;
        for (i = 0; i < 3; ++i) {
            uint64_t v;

            v = strtoull(init_list, &next, 16);
            if (next == init_list) {
                break;
            }
            init_list = (char const *)next;


            if (bench_conf.init_vals_[i] == 0) {
                bench_conf.init_vals_[i] = v;
            }
            if (*init_list != ',') {
                break;
            }
        }
    }


    if (run_all || (list && num_to_run == 0)) {
        svml_ops_to_run     = &all_svml_defs[0];
        num_svml_ops_to_run = num_svml_defs;
        should_free         = false;
    }
    else {
        uint64_t svmls_already_in[(num_svml_defs + 63) / 64];
        __builtin_memset(svmls_already_in, 0, 8 * ((num_svml_defs + 63) / 64));
        should_free = true;
        svml_ops_to_run_builder =
            (svml_op_t *)safe_zalloc(num_svml_defs * sizeof(svml_op_t));
        num_svml_ops_to_run = 0;
        for (i = 0; i < num_to_run; ++i) {
            uint32_t     tt       = 0;
            char const * pat      = to_run[i];
            int32_t      patlen   = CAST(int32_t, strlen(pat));
            uint64_t     mask     = 1;
            int32_t      success  = 0;
            bool         fp_only  = false;
            bool         dbl_only = false;
            bool         changed;
            do {
                die_assert(patlen >= 0);
                die_assert(pat[patlen] == '\0');
                changed = false;
                if (pat[0] == '!' || pat[0] == '~') {
                    ++pat;
                    --patlen;
                    success = FNM_NOMATCH;
                    changed = true;
                }
                else if (pat[0] == 'F' && pat[1] == 'P') {
                    changed = true;
                    fp_only = true;
                    pat += 2;
                    patlen -= 2;
                }
                else if (pat[0] == 'D' && pat[1] == 'B' && pat[2] == 'L') {
                    changed  = true;
                    dbl_only = true;
                    patlen -= 3;
                }
                else if (pat[0] == '[') {
                    uint32_t     check_sz;
                    char const * end_pat =
                        memchr(pat, ']', CAST(size_t, patlen));
                    if (end_pat == NULL) {
                        continue;
                    }
                    die_assert(end_pat > pat);
                    check_sz = CAST(uint32_t, end_pat - pat);
                    if (check_sz <= 1) {
                        continue;
                    }
                    tt |= str_to_test_enum(pat + 1, check_sz - 1);
                    ++end_pat;
                    patlen -= CAST(int32_t, end_pat - pat);
                    pat     = end_pat;
                    changed = true;
                }
            } while (changed);

            if (tt == 0) {
                tt = ~0u;
            }
            for (j = 0; j < num_svml_defs; ++j) {
                if (svmls_already_in[j / 64] & mask) {
                    goto cont;
                }
                if (fp_only && (!all_svml_defs[j].fp_)) {
                    goto cont;
                }
                if (dbl_only && all_svml_defs[j].fp_) {
                    goto cont;
                }
                if (!(tt & (1u << all_svml_defs[j].test_type_))) {
                    goto cont;
                }
                if (fnmatch(pat, all_svml_defs[j].name_, FNM_CASEFOLD) ==
                    success) {
                    svmls_already_in[j / 64] |= mask;
                    __builtin_memcpy(
                        svml_ops_to_run_builder + num_svml_ops_to_run,
                        &all_svml_defs[j], sizeof(svml_op_t));
                    ++num_svml_ops_to_run;
                }
            cont:
                mask = (mask << 1) | (mask >> 63);
            }
        }
        svml_ops_to_run = svml_ops_to_run_builder;
    }

    if (list) {
        for (i = 0; i < num_svml_ops_to_run; ++i) {
            print("%s\n", svml_ops_to_run[i].name_);
        }
    }
    if (test) {
        run_svml_tests(svml_ops_to_run, num_svml_ops_to_run, parallel);
    }
    if (bench) {
        run_svml_benches(svml_ops_to_run, num_svml_ops_to_run, &bench_conf);
    }

    if (should_free) {
        safe_sfree(svml_ops_to_run_builder,
                   num_svml_ops_to_run * sizeof(svml_op_t));
    }

    return 0;
}
