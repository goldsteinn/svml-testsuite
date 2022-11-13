#include "bench/bench-common.h"
#include "bench/benchmarks.h"
#include "lib/commonlib.h"
#include "util/arg.h"
#include "util/error-util.h"
#include "util/func-decl-utils.h"
#include "util/macro.h"
#include "util/sched-util.h"
#include "util/verbosity.h"

static int32_t  verbosity = 0;
static int32_t  list_all  = 0;
static int32_t  run_all   = 0;
static uint32_t trials    = 0;
static int32_t  cpu       = -1;

static char const * type = NULL;

uint32_t ht_type_size = 0;
uint32_t ht_test_size = 0;

static arg_rest_t benchmark_names = INIT_ARG_REST_T;


static arg_option_t args[] = {
    /* ADD_ARG(Kind, Method, name, reqd, variable, help) */
    ADD_ARG(KindOption, Integer, "-v", 0, &verbosity, "Turn on verbosity"),
    ADD_ARG(KindOption, Set, "--list", 0, &list_all, "List all benchmarks"),
    ADD_ARG(KindOption, Set, "--all", 0, &run_all, "Run all benchmarks"),
    ADD_ARG(KindOption,
            Integer,
            ("-c", "--cpu"),
            0,
            &cpu,
            "Set CPU (-1 to skip)"),
    ADD_ARG(KindOption,
            Integer,
            ("--ht-Tsize", "--Tsize"),
            0,
            &ht_type_size,
            "Sizeof type for hashtable benchmark (0 for all)"),
    ADD_ARG(KindOption,
            String,
            ("--ht-T", "-T"),
            0,
            &type,
            "Sizeof type for hashtable benchmark (0 for all)"),
    ADD_ARG(KindOption,
            Integer,
            ("--ht-test-size", "--test-size"),
            0,
            &ht_test_size,
            "Sizeof type for hashtable benchmark (0 for all)"),
    ADD_ARG(KindOption,
            Integer,
            ("-N", "--trials"),
            0,
            &trials,
            "Trials (0 will use default)"),
    ADD_ARG(KindHelp, Help, "-h", 0, NULL, ""),
    ADD_ARG(KindRest,
            Rest,
            "",
            0,
            &benchmark_names,
            "Benchmarks to run. Just include at end"),
    ADD_ARG(KindEnd, EndOptions, "", 0, NULL, ""),
};


static arg_defs_t argp = { args, "Benchmark Driver", NULL, NULL };


static void
run_benchmark(const func_decl_t * benchmark) {

    warn_assert(
        !benchmark->bench_func(
            /* Ideally replaced with NOLINT(performance-no-int-to-ptr) */
            NOANALYZE(CAST(void *, CAST(uint64_t, trials)), NULL)),
        "Issue running: %s\n", benchmark->name);
}

static uint32_t
typename_to_size(char const * T) {
    typedef struct type_size_info {
        const uint8_t  name[16];
        const uint64_t name_len;
        const uint32_t bytes;
    } type_size_info_t;

    uint64_t                slen;
    type_size_info_t const *start, *end;
    /* NOLINTBEGIN(google-runtime-int) */
    static type_size_info_t type_size_info[] = {
        { "u8", strlen("u8"), 1 },
        { "s8", strlen("s8"), 1 },
        { "u16", strlen("u16"), 2 },
        { "s16", strlen("s16"), 2 },
        { "u32", strlen("u32"), 4 },
        { "s32", strlen("s32"), 4 },
        { "u64", strlen("u64"), 8 },
        { "s64", strlen("s64"), 8 },
        { "uint8", strlen("uint8"), 1 },
        { "int8", strlen("int8"), 1 },
        { "uint16", strlen("uint16"), 2 },
        { "int16", strlen("int16"), 2 },
        { "uint32", strlen("uint32"), 4 },
        { "int32", strlen("int32"), 4 },
        { "uint64", strlen("uint64"), 8 },
        { "int64", strlen("int64"), 8 },
        { "uint8_t", strlen("uint8_t"), 1 },
        { "int8_t", strlen("int8_t"), 1 },
        { "uint16_t", strlen("uint16_t"), 2 },
        { "int16_t", strlen("int16_t"), 2 },
        { "uint32_t", strlen("uint32_t"), 4 },
        { "int32_t", strlen("int32_t"), 4 },
        { "uint64_t", strlen("uint64_t"), 8 },
        { "int64_t", strlen("int64_t"), 8 },
        { "byte", strlen("byte"), 1 },
        { "char", strlen("char"), sizeof(char) },
        { "short", strlen("short"), sizeof(short) },
        { "ushort", strlen("ushort"), sizeof(unsigned short) },
        { "unsigned short", strlen("unsigned short"), sizeof(unsigned short) },
        { "int", strlen("int"), sizeof(int) },
        { "uint", strlen("uint"), sizeof(unsigned int) },
        { "unsigned", strlen("unsigned"), sizeof(unsigned) },
        { "unsigned int", strlen("unsigned int"), sizeof(unsigned int) },
        { "long", strlen("long"), sizeof(long) },
        { "ulong", strlen("ulong"), sizeof(long) },
        { "unsigned long", strlen("unsigned long"), sizeof(unsigned long) },
        { "long long", strlen("long long"), sizeof(long long) },
        { "long_long", strlen("long_long"), sizeof(long long) },
        { "ulong_long", strlen("ulong_long"), sizeof(unsigned long long) },
        { "ull", strlen("ull"), sizeof(unsigned long long) },
        { "ll", strlen("ll"), sizeof(long long) },
        { "u128", strlen("u128"), 16 },
    };
    /* NOLINTEND(google-runtime-int) */
    if (T == NULL) {
        return 0;
    }

    slen  = strlen(T);
    start = type_size_info;
    end   = &type_size_info[sizeof(type_size_info) / sizeof(type_size_info[0])];
    for (; start != end; ++start) {
        if (slen == start->name_len &&
            (memcmpeq_c((uint8_t const *)T, start->name, slen) == 0)) {
            return start->bytes;
        }
    }
    return 0;
}

int
main(int argc, char ** argv) {
    char * const * decls_to_run;
    uint64_t       ndecls_to_run;

    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);

    decls_to_run  = run_all ? NULL : benchmark_names.ptrs;
    ndecls_to_run = benchmark_names.n;


    if (ht_type_size == 0) {
        ht_type_size = typename_to_size(type);
    }

    if (list_all || (!run_all && benchmark_names.n == 0)) {
        list_decls_filtered(&benchmarks, decls_to_run, ndecls_to_run);
    }
    else {
        if (cpu != -1) {
            die_assert((uint32_t)cpu < get_num_cpus(),
                       "Cpu out of range %u not in [0, %u)\n", cpu,
                       get_num_cpus());
            proc_setcpu_and_wait(CAST(uint32_t, cpu));
        }
        run_decls(&benchmarks, decls_to_run, ndecls_to_run, &run_benchmark);
    }
    return 0;
}
