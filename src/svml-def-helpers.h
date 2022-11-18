#ifndef SRC_D_SVML_HELPERS_H_
#define SRC_D_SVML_HELPERS_H_

#include "svml-defs.h"
#include "util/error-util.h"

#include <string.h>


static uint32_t
str_to_test_enum(char const * s, uint32_t slen) {
    static char const * const tt_strs[] = {
        "d_d",       "d_d_d",       "f_f",       "f_f_f",
        "i_d",       "i_f",         "lli_d",     "lli_f",
        "v_d_dp_dp", "v_d_vdp_vdp", "v_f_fp_fp", "v_f_vfp_vfp",
    };
    uint32_t     i;
    char const * og_s = s;
    if (slen >= strlen("test") &&
        (memcmpeq_c((uint8_t const *)s, (uint8_t const *)"test",
                    strlen("test")) == 0)) {
        s += strlen("test");
        slen -= CAST(uint32_t, strlen("test"));
        if (*s == '-' || *s == '_') {
            --slen;
            ++s;
        }
    }
    for (i = 0; i < k_test_end; ++i) {
        if (strlen(tt_strs[i]) == slen &&
            (memcmpeq_c((uint8_t const *)s, (uint8_t const *)(tt_strs[i]),
                        slen) == 0)) {
            return (1u << i);
        }
    }
    msg_die("Unkown test: %s\n", og_s);
}

#endif
