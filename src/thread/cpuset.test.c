#include <stdint.h>

#include "util/common.h"
#include "util/inline-math.h"
#include "util/macro.h"
#include "util/random.h"

#include "thread/cpuset.h"

#include "test/test-common.h"
int32_t test_cpuset(void);
int32_t
test_cpuset(void) {
    uint32_t i, ncpus;
    cpuset_t cset0 = new_cpuset_zero();
    cpuset_t cset1 = new_cpuset_zero();
    cpuset_t cset2, cset3;
    ncpus = get_num_cpus();
    test_assert(cset_empty(&cset0));
    test_assert(cset_empty(&cset1));
    test_assert(cset_test(&cset0, &cset1) == 0);
    test_assert(cset_count(&cset0) == 0);
    test_assert(cset_count(&cset1) == 0);
    test_assert(cset_equal(&cset0, &cset1));

    for (i = 0; i < ncpus; ++i) {
        test_assert(cset_equal(&cset0, &cset1));
        test_assert(!cset_isset(i, &cset0));
        test_assert(!cset_isset(i, &cset1));

        test_assert(cset_first(&cset0) == -1);
        test_assert(cset_first(&cset1) == -1);
    }

    for (i = 0; i < ncpus; ++i) {
        test_assert(cset_equal(&cset0, &cset1));
        test_assert(!cset_isset(i, &cset0));
        test_assert(!cset_isset(i, &cset1));

        cset_set(i, &cset0);
        test_assert(!cset_equal(&cset0, &cset1));
        cset_set(i, &cset1);
        test_assert(cset_equal(&cset0, &cset1));
        test_assert(cset_isset(i, &cset0));
        test_assert(cset_isset(i, &cset1));

        test_assert(cset_first(&cset0) == 0);
        test_assert(cset_first(&cset1) == 0);

        test_assert(!cset_empty(&cset0));
        test_assert(!cset_empty(&cset1));
        test_assert(cset_test(&cset0, &cset1));
        test_assert(cset_count(&cset0) == i + 1);
        test_assert(cset_count(&cset1) == i + 1);
    }

    cset_all(&cset2);

    test_assert(!cset_empty(&cset2));
    test_assert(cset_count(&cset2) == ncpus);
    test_assert(cset_test(&cset2, &cset0));
    test_assert(cset_test(&cset2, &cset1));

    test_assert(cset_equal(&cset2, &cset0));
    test_assert(cset_equal(&cset2, &cset1));
    test_assert(cset_equal(&cset0, &cset1));

    for (i = 0; i < ncpus; ++i) {
        if (i & 1) {
            cset_clr(i, &cset0);
        }
        else {
            cset_clr(i, &cset1);
        }
    }
    cset_or(&cset3, &cset0, &cset1);

    test_assert(!cset_equal(&cset0, &cset1));
    test_assert(!cset_empty(&cset0));
    test_assert(!cset_empty(&cset1));
    test_assert(cset_equal(&cset3, &cset2));

    test_assert(cset_test(&cset0, &cset1) == 0);
    test_assert(cset_count(&cset0) == ncpus / 2);
    test_assert(cset_count(&cset1) == ncpus / 2);

    test_assert(cset_test(&cset0, &cset2) == 1);
    test_assert(cset_test(&cset1, &cset2) == 1);

    cset_or_eq(&cset0, &cset1);
    test_assert(cset_equal(&cset2, &cset0));
    test_assert(!cset_equal(&cset2, &cset1));
    test_assert(!cset_equal(&cset0, &cset1));

    for (i = 0; i < ncpus; ++i) {
        test_assert(cset_test(&cset0, &cset2) == 1);
        test_assert(cset_test(&cset1, &cset2) == 1);
        test_assert(cset_test(&cset1, &cset0) == 1);

        test_assert(cset_first(&cset0) == (int32_t)i);
        test_assert(cset_first(&cset1) == (int32_t)((i & 1) ? i : (i + 1)));
        if (i & 1) {
            cset_clr_first(&cset1);
        }
        cset_clr_first(&cset0);
    }

    test_assert(cset_empty(&cset0));
    test_assert(cset_empty(&cset1));
    test_assert(cset_test(&cset0, &cset1) == 0);
    test_assert(cset_count(&cset0) == 0);
    test_assert(cset_count(&cset1) == 0);
    test_assert(cset_equal(&cset0, &cset1));

    for (i = 0; i < ncpus; ++i) {
        test_assert(cset_equal(&cset0, &cset1));
        test_assert(!cset_isset(i, &cset0));
        test_assert(!cset_isset(i, &cset1));

        test_assert(cset_first(&cset0) == -1);
        test_assert(cset_first(&cset1) == -1);
    }
    for (i = 0; i < ncpus; ++i) {
        cset_copy_first(&cset0, &cset2);
        test_assert(cset_first(&cset0) == (int32_t)i);
        test_assert(cset_first(&cset2) == (int32_t)i);

        test_assert(cset_count(&cset0) == 1);
        test_assert(cset_count(&cset2) == (ncpus - i));
        cset_clr_first(&cset2);
        cset_clr_first(&cset0);
        test_assert(cset_empty(&cset0));
    }
    test_assert(cset_count(&cset0) == 0);
    test_assert(cset_count(&cset2) == 0);
    test_assert(!cset_equal(&cset0, &cset3));
    cset_copy(&cset0, &cset3);
    test_assert(cset_equal(&cset0, &cset3));

    return 0;
}
