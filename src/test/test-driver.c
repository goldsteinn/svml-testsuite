#include "test/test-common.h"
#include "test/tests.h"

#include "util/arg.h"
#include "util/verbosity.h"

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

int
do_test(char ** _test_names, size_t _n_tests_to_run, const char * test_name) {
    die_assert(_test_names, "Error, no tests specified!\n");
    die_assert(test_name, "Error, null test name!\n");
    size_t i;
    for (i = 0; i < _n_tests_to_run; ++i) {
        if (_test_names[i] == NULL) {
            continue;
        }
        if (!strcmp(_test_names[i], test_name)) {
            _test_names[i] = NULL;
            return 1;
        }
    }
    return 0;
}

void
run_tests(char ** _test_names, size_t _n_tests_to_run, int _run_all) {
    size_t       i;
    const char * res;
    fprintf(stdout, "----------------- Starting Tests -----------------\n");
    for (i = 0; i < ntests; ++i) {
        die_assert(tests[i].name && tests[i].func,
                   "Error, unitialized test struct!\n");
        if (_run_all || do_test(_test_names, _n_tests_to_run, tests[i].name)) {
            fprintf(stdout, "Running - %-24s ...", tests[i].name);
            res = "PASSED";
            if (tests[i].func()) {
                res = "FAILED";
            }
            fprintf(stdout, "\rRunning - %-24s -> %s\n", tests[i].name, res);
        }
    }
    fprintf(stdout, "---------------- Finished Testing ----------------\n");
    for (i = 0; (!_run_all) && i < _n_tests_to_run; ++i) {
        if (_test_names[i]) {
            fprintf(stdout, "Unable To Find - %s\n", _test_names[i]);
        }
    }
}

void
list_tests() {
    size_t i;

    fprintf(stdout, "----------------- Listing Tests ------------------\n");
    for (i = 0; i < ntests; ++i) {
        die_assert(tests[i].name && tests[i].func,
                   "Error, unitialized test struct!\n");
        fprintf(stdout, "%-24s\n", tests[i].name);
    }
    fprintf(stdout, "------------- Finished Listing Tests -------------\n");
}

int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);

    if (list_all) {
        list_tests();
    }
    else {
        run_tests(test_names.ptrs, test_names.n, run_all);
    }
}
