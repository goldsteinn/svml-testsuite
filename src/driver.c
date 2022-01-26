#include "util/arg.h"
#include "util/common.h"
#include "util/error-util.h"
#include "util/verbosity.h"

static int32_t verbosity;

// clang-format off
static ArgOption args[] = {
    //  Kind,           Method,		name,	    reqd,   variable,           help
  {     KindOption,     Integer,	"-v", 		0,      &verbosity,         "Turn on verbosity" },
  {     KindHelp,       Help,       "-h",       0,      NULL,               ""  },
  {     KindEnd,        EndOptions, "",         0,      NULL,               ""  }
};
// clang-format on

static ArgDefs argp = { args, "C / ASM Wrapper", NULL, NULL };


int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);
}
