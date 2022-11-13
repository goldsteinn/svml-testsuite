#include "thread/cpuset.h"
#include "util/string-util.h"

static void __attribute__((constructor)) cset_is_valid(void) {
    die_assert(sizeof_bits(cpuset_t) >= get_num_cpus(), "cpu set too small!");
}

char *
cset_to_str(char * buf, uint64_t buflen, cpuset_t const * cset) {
    uint64_t nwritten;
    int32_t  cpu;
    cpuset_t tmp;
    cset_copy(&tmp, cset);
    die_assert(buflen);
    nwritten = safe_snprintf(buf, buflen, "[");

    while (!cset_empty(&tmp)) {
        cpu = cset_first(&tmp);
        die_assert(cpu != -1);
        die_assert(nwritten < buflen);
        nwritten += safe_snprintf(buf + nwritten, buflen - nwritten, "%s%4d",
                                  nwritten <= 1 ? "" : ",", cpu);
        cset_clr_first(&tmp);
    }
    die_assert(nwritten < buflen);
    nwritten += safe_snprintf(buf + nwritten, buflen, "]");
    die_assert(nwritten < buflen);
    return buf;
}
