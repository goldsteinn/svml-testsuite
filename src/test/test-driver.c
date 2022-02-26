#include "test/test-common.h"
#include "test/tests.h"

#include "util/arg.h"
#include "util/error-util.h"
#include "util/verbosity.h"

#include "util/func-decl-utils.h"

static int32_t    verbosity  = 0;
static int32_t    list_all   = 0;
static int32_t    run_all    = 0;
static arg_rest_t test_names = INIT_ARG_REST_T;

// clang-format off
static ArgOption args[] = {
    //  Kind,           Method,		name,	    reqd,   variable,           help
  {     KindOption,     Integer,	"-v", 		0,      &verbosity,         "Turn on verbosity" },
  {     KindOption,     Set,		"--list",	0,      &list_all,          "List all tests" },
  {     KindOption,     Set,		"--all",    0,      &run_all,           "Run all tests" },
  {     KindHelp,       Help,       "-h",       0,      NULL,               ""  },
  {     KindRest,       Rest,		"",         0,      &test_names,        "Tests to run. Just include at end" },
  {     KindEnd,        EndOptions, "",         0,      NULL,               ""  }
};
// clang-format on

static ArgDefs argp = { args, "Test Driver", NULL, NULL };


static void
run_test(const func_decl_t * test) {
    const char * res = "PASSED";
    fprintf(stderr, "Running - %-24s ...", test->name);
    if (test->test_func()) {
        res = "FAILED";
    }
    fprintf(stderr, "\rRunning - %-24s -> %s\n", test->name, res);
}

int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);

    if (list_all || (!run_all && test_names.n == 0)) {
        list_decls(&tests);
    }
    else {
        run_decls(&tests, run_all ? NULL : test_names.ptrs, test_names.n,
                  &run_test);
    }

    return 0;
}
