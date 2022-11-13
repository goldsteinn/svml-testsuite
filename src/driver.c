#include "util/arg.h"
#include "util/common.h"
#include "util/error-util.h"
#include "util/verbosity.h"

static int32_t verbosity;


static arg_option_t args[] = {
    /* ADD_ARG(Kind, Method, name, reqd, variable, help) */
    ADD_ARG(KindOption, Integer, "-v", 0, &verbosity, "Turn on verbosity"),
    ADD_ARG(KindHelp, Help, "-h", 0, NULL, ""),
    ADD_ARG(KindEnd, EndOptions, "", 0, NULL, "")
};


static arg_defs_t argp = { args, "C / ASM Wrapper", NULL, NULL };

int
main(int argc, char * argv[]) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    set_verbosity(verbosity);
    return 0;
}
