#include <stdint.h>

#include "util/error-util.h"
#include "util/func-decl-utils.h"


void
list_decls(const decl_list_t * decl_list) {
    uint64_t            i, ndecls;
    const func_decl_t * decls;
    die_assert(decl_list != NULL, "Error, trying to list null decls");

    ndecls = decl_list->ndecls;
    fprintf(stdout, "----------------- Listing %s ------------------\n",
            decl_list->decl_desc);

    for (i = 0; i < ndecls; ++i) {
        decls = decl_list->decls;

        die_assert(decls[i].name && decls[i]._func_ptr,
                   "Error, unitialized decl struct!\n");
        fprintf(stdout, "%-24s\n", decls[i].name);
    }
    fprintf(stdout, "------------- Finished Listing %s -------------\n",
            decl_list->decl_desc);
}

static int32_t
should_run_decl(const char *   decl_name,
                char * const * decl_names,
                uint64_t       ndecls_to_run,
                uint8_t * restrict has_run) {
    uint64_t i;
    if (!decl_names) {
        return 1;
    }

    for (i = 0; i < ndecls_to_run; ++i) {
        if (has_run[i]) {
            continue;
        }

        if (!strcmp(decl_name, decl_names[i])) {
            has_run[i] = 1;
            return 1;
        }
    }
    return 0;
}

void
run_decls(const decl_list_t * decl_list,
          char * const *      decl_names,
          uint64_t            ndecls_to_run,
          const run_decl_f    run_decl_func) {
    uint64_t            i, ndecls;
    const func_decl_t * decls;
    uint8_t             has_run[ndecls_to_run];
    __builtin_memset(has_run, 0, ndecls_to_run);

    die_assert(decl_list != NULL, "Error, trying to list null decls");
    die_assert(run_decl_func != NULL, "Error, runner function is null");

    ndecls = decl_list->ndecls;
    decls  = decl_list->decls;
    for (i = 0; i < ndecls; ++i) {
        die_assert(decls[i].name && decls[i]._func_ptr,
                   "Error, unitialized decl struct!\n");

        if (should_run_decl(decls[i].name, decl_names, ndecls_to_run,
                            has_run)) {
            run_decl_func(decls + i);
        }
    }
    if (!decl_names) {
        return;
    }

    for (i = 0; i < ndecls_to_run; ++i) {
        if (has_run[i]) {
            continue;
        }
        fprintf(stdout, "Unable To Find - %s\n", decls[i].name);
    }
}
