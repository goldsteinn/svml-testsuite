#include "util/func-decl-utils.h"

#include "util/error-util.h"
#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/print.h"
#include "util/regex-util.h"

#include <stdint.h>

/* TODO: replace regex logic in fnmatch. */
enum { k_decl_do_nothing = 0, k_decl_re_built = 2, k_decl_ran = 4 };
enum { k_decl_did_one = 1, k_decl_did_any = 0 };


static uint32_t
build_re_wildcard_matcher(regex_t * restrict re,
                          char const * restrict decl_to_run) {
    char const * decl_to_run_next;
    uint64_t     copy_len;
    uint32_t     match_any;
    /* Max possible length. */
    char   pattern_buf[strlen_c(decl_to_run) * 2 + 3];
    char * pattern_buf_pos;
    die_assert(decl_to_run);
    die_assert(re);

    pattern_buf[0]  = '^';
    pattern_buf_pos = pattern_buf + 1;


    match_any = k_decl_did_one;
    for (;;) {
        decl_to_run_next = strchrnul_c(decl_to_run, '*');
        die_assert(decl_to_run_next >= decl_to_run);
        copy_len = CAST(uint64_t, decl_to_run_next - decl_to_run);
        if (!(*decl_to_run_next)) {
            memcpy_c(pattern_buf_pos, decl_to_run, copy_len);
            pattern_buf_pos[copy_len]     = '$';
            pattern_buf_pos[copy_len + 1] = '\0';
            break;
        }

        match_any = k_decl_did_any;
        memcpy_c(pattern_buf_pos, decl_to_run, copy_len);
        pattern_buf_pos[copy_len]     = '.';
        pattern_buf_pos[copy_len + 1] = '*';
        pattern_buf_pos += (copy_len + 2);
        decl_to_run = decl_to_run_next + 1;
    }
    safe_re_compile(re, pattern_buf, REG_ICASE | REG_NOSUB);
    return match_any;
}

static int32_t
is_match(regex_t const * restrict re, char const * restrict decl_name) {
    die_assert(re, "NULL re (should be impossible)!");
    die_assert(decl_name, "NULL decl_name, issue with test struct generator!");

    return !safe_re_exec(re, decl_name, 0, NULL, 0);
}

static int32_t
should_run_decl(char const * restrict decl_name,
                char * restrict const * restrict decls_to_run,
                uint64_t ndecls_to_run,
                uint8_t * restrict decl_states,
                regex_t * restrict re_matchers) {
    uint64_t i;
    int32_t  has_hit;
    uint8_t  decl_state;

    if (!decls_to_run) {
        return 1;
    }
    has_hit = 0;
    for (i = 0; i < ndecls_to_run; ++i) {
        die_assert(decls_to_run[i] != NULL);
        decl_state = decl_states[i];

        if ((decl_state & k_decl_ran) && (decl_state & k_decl_did_one)) {
            continue;
        }
        if (!(decl_state & k_decl_re_built)) {
            uint32_t re_match_res =
                build_re_wildcard_matcher(re_matchers + i, decls_to_run[i]);
            die_assert(re_match_res <= UCHAR_MAX);
            decl_state |= (k_decl_re_built | CAST(uint8_t, re_match_res));
        }
        if (is_match(re_matchers + i, decl_name)) {
            dbg_assert(decl_states[i] != k_decl_do_nothing);

            decl_state |= k_decl_ran;
            has_hit = 1;
        }
        decl_states[i] = decl_state;
    }
    return has_hit;
}

static void
print_decl(func_decl_t const * decl) {
    fprintf_stdout("%s\n", decl->name);
}

void
list_decls_filtered(decl_list_t const * decl_list,
                    char * restrict const * restrict decls_to_list,
                    uint64_t ndecls_to_list) {

    fprintf_stdout("----------------- Listing %s ------------------\n",
                   decl_list->decl_desc);
    run_decls(decl_list, decls_to_list, ndecls_to_list, &print_decl);
    fprintf_stdout("------------- Finished Listing %s -------------\n",
                   decl_list->decl_desc);
}

void
list_decls(decl_list_t const * restrict decl_list) {
    list_decls_filtered(decl_list, NULL, 0);
}

static NONNULL(1, 4) void I_run_decls(
    decl_list_t const * restrict decl_list,
    char * restrict const * restrict decls_to_run,
    uint64_t         ndecls_to_run,
    const run_decl_f run_decl_func) {

    uint64_t            i;
    uint64_t            ndecls = decl_list->ndecls;
    const func_decl_t * decls  = decl_list->decls;
    regex_t             re_matchers[MAX(ndecls, ndecls_to_run, 1UL)];
    uint8_t decl_states[MAX(ndecls, ndecls_to_run, 1UL)], decl_state;
    memset_c(decl_states, 0, MAX(ndecls, ndecls_to_run));


    for (i = 0; i < ndecls; ++i) {
        die_assert(decls[i].name && decls[i].data,
                   "Error, unitialized decl struct!\n");
        if (should_run_decl(decls[i].name, decls_to_run, ndecls_to_run,
                            decl_states, re_matchers)) {
            run_decl_func(decls + i);
        }
    }
    if (!decls_to_run) {
        return;
    }
    for (i = 0; i < ndecls_to_run; ++i) {
        die_assert(decls_to_run[i]);
        decl_state = decl_states[i];
        if (decl_state != k_decl_do_nothing) {

            die_assert(decl_state & k_decl_re_built);
            re_free(re_matchers + i);

            if (decl_state & k_decl_ran) {
                continue;
            }
        }
        fprintf_stdout("Unable To Find - %s\n", decls_to_run[i]);
    }
}

void
run_decls(decl_list_t const * restrict decl_list,
          char * restrict const * restrict decls_to_run,
          uint64_t         ndecls_to_run,
          const run_decl_f run_decl_func) {
    die_assert(decl_list != NULL, "Error, trying to list null decls");
    die_assert(run_decl_func != NULL, "Error, runner function is null");

    I_run_decls(decl_list, decls_to_run, ndecls_to_run, run_decl_func);
}
