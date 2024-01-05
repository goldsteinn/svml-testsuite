#include "lib/algo/sort.h"

#include "sys/sys-info.h"

#include "thread/thread-common.h"

#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/verbosity.h"

#include "math-func-defs.h"
#include "math-defs.h"

#include "svml-func-defs.h"
#include "svml-test-constants.h"
#include "svml-tester.h"
#define INIT_ITER  28
#define DBG_OUTPUT 1


#define WITH_DBG_OUTPUT (!!(DBG_OUTPUT))


#include "svml-dbl-state.h"
#include "svml-flt-state.h"

__thread uint64_t G_total_fallbacks0 = 0;
__thread uint64_t G_total_fallbacks1 = 0;


typedef struct ulp_hist {
    uint64_t fallbacks0_;
    uint64_t fallbacks1_;
    uint64_t ulps_[k_max_ulp + 1];
} ulp_hist_t;

typedef struct test_svml_op {
    svml_op_t const op_;
    ulp_hist_t      hist_;
} test_svml_op_t;

typedef struct svml_groups {
    uint16_t         sizes_[k_test_end];
    test_svml_op_t * groups_[k_test_end];
} svml_groups_t;

typedef struct test_group {
    uint32_t         num_ops_;
    uint32_t         tidx_;
    test_svml_op_t * ops_;
} test_group_t;


static test_group_t ** volatile avail_tgroups;
static sema_t sema;

static void
push_tgroup(void * tgroup) {
    test_group_t ** volatile push_pos =
        __atomic_sub_fetch(&avail_tgroups, sizeof(void *), __ATOMIC_RELAXED);
    *push_pos = (test_group_t *)tgroup;
}

static test_group_t *
pop_tgroup(void) {
    test_group_t ** volatile pop_pos =
        __atomic_fetch_add(&avail_tgroups, sizeof(void *), __ATOMIC_RELAXED);
    return *pop_pos;
}


static void
create_ulp_hist(uint32_t     item_width,
                uint8_t *    hist_bytes,
                ulp_hist_t * hist_out,
                uint64_t     fallbacks0,
                uint64_t     fallbacks1) {
    uint32_t i, j;
    die_assert(item_width == 4 || item_width == 8);
    for (i = 0; i <= k_max_ulp; ++i) {
        uint64_t sum = 0;
        for (j = 0; j < 64; j += item_width) {
            uint64_t v = 0;
            __builtin_memcpy(&v, hist_bytes + i * 64 + j, item_width);
            sum += v;
        }
        hist_out->ulps_[i] = sum;
    }
    hist_out->fallbacks0_ = fallbacks0;
    hist_out->fallbacks1_ = fallbacks1;
}

static void
test_op_out(test_svml_op_t const * op) {
    uint32_t i;
    uint64_t sum = 0, wsum = 0;
    double   sum_dbl, wsum_dbl;
    for (i = 0; i <= k_max_ulp; ++i) {
        sum += op->hist_.ulps_[i];
        wsum += op->hist_.ulps_[i] * i;
    }
    wsum_dbl = (double)wsum;
    sum_dbl  = (double)sum;
    print("%12s -> %32s: %.4lf (%lu / %lu / %lu)\n", op->op_.base_name_,
          op->op_.name_, (wsum_dbl / sum_dbl), op->hist_.fallbacks0_,
          op->hist_.fallbacks1_, sum);
    for (i = 0; i <= k_max_ulp; ++i) {
        uint64_t vi     = op->hist_.ulps_[i];
        double   vi_dbl = (double)vi;
        printf("\t%2u: %10lu (%.4f)\n", i, vi, vi_dbl / sum_dbl);
    }
}


#define use_fp32 1
#include "svml-fp-eq.h"
#include "svml-tests.h"

#undef use_fp32
#define use_fp32 0
#include "svml-fp-eq.h"
#include "svml-tests.h"
#undef use_fp32


static void *
svml_tester_lli_d(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_lli_f(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_i_d(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_i_f(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_v_f_vfp_vfp(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_v_d_vdp_vdp(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

typedef void * (*svml_tester__f)(void *);
static const svml_tester__f svml_testers[] = {
    svml_tester_d_d,         svml_tester_d_d_d,     svml_tester_f_f,
    svml_tester_f_f_f,       svml_tester_i_d,       svml_tester_i_f,
    svml_tester_lli_d,       svml_tester_lli_f,     svml_tester_v_d_dp_dp,
    svml_tester_v_d_vdp_vdp, svml_tester_v_f_fp_fp, svml_tester_v_f_vfp_vfp,
};

static void
svml_groups_deinit(svml_groups_t * sgroups, uint32_t num_svml_ops) {
    safe_sfree(sgroups->groups_[0], num_svml_ops * sizeof(svml_op_t));
}

static uint32_t
svml_groups_init(svml_groups_t *   sgroups,
                 svml_op_t const * svml_ops,
                 uint32_t          num_svml_ops) {
    uint32_t         i, j, max_tgroup_sz;
    uint32_t         offset;
    test_svml_op_t * ordered_ops =
        (test_svml_op_t *)safe_zalloc(num_svml_ops * sizeof(test_svml_op_t));

    __builtin_memset(sgroups, 0, sizeof(*sgroups));
    for (i = 0; i < num_svml_ops; ++i) {
        sgroups->sizes_[svml_ops[i].test_type_] += 1;
        __builtin_memcpy(ordered_ops + i, svml_ops + i, sizeof(*svml_ops));
    }

    lsort(ordered_ops, num_svml_ops, lhs->op_.test_type_ > rhs->op_.test_type_);

    offset = 0;
    for (i = 0; i < k_test_end; ++i) {
        sgroups->groups_[i] = ordered_ops + offset;
        offset += sgroups->sizes_[i];
    }
    die_assert(offset == num_svml_ops);
    for (i = 0; i < k_test_end; ++i) {
        for (j = 0; j < sgroups->sizes_[i]; ++j) {
            die_assert(sgroups->groups_[i][j].op_.test_type_ == i);
        }
    }

    for (i = 0; i < k_test_end; ++i) {
        lsort(sgroups->groups_[i], sgroups->sizes_[i],
              (lhs->op_.ref_func_.p > rhs->op_.ref_func_.p)
                  ? 1
                  : (lhs->op_.ref_func_.p == rhs->op_.ref_func_.p
                         ? lhs->op_.sz_ > rhs->op_.sz_
                         : -1));
    }

    max_tgroup_sz = 0;
    for (i = 0; i < k_test_end; ++i) {
        uint32_t     dbg_last_sz = 0;
        uint32_t     lastj;
        void const * lastp;
        lastp = NULL;
        lastj = 0;
        for (j = 0; j < sgroups->sizes_[i]; ++j) {
            void const * curp = sgroups->groups_[i][j].op_.ref_func_.p;
            die_assert(curp >= lastp);
            if (curp != lastp) {
                max_tgroup_sz = MAX(max_tgroup_sz, (j - lastj));

                lastj       = j;
                lastp       = curp;
                dbg_last_sz = sgroups->groups_[i][j].op_.sz_;
            }
            else {

                die_assert(dbg_last_sz <= sgroups->groups_[i][j].op_.sz_);
                dbg_last_sz = sgroups->groups_[i][j].op_.sz_;
            }
        }
        max_tgroup_sz = MAX(max_tgroup_sz, (j - lastj));
    }

    die_assert(max_tgroup_sz < 16);
    return max_tgroup_sz;
}

static uint32_t
tgroup_init(uint32_t         idx,
            uint32_t         max_group_sz,
            test_svml_op_t * ops,
            uint32_t         num_ops,
            test_group_t *   tgroup) {
    test_svml_op_t * init_op;
    uint32_t         init_idx;
    die_assert(idx < num_ops);

    init_idx = idx;
    init_op  = ops + idx;
    do {
        ++idx;
        --max_group_sz;
    } while (max_group_sz && idx < num_ops &&
             init_op->op_.ref_func_.p == ops[idx].op_.ref_func_.p);

    tgroup->num_ops_ = (idx - init_idx);
    tgroup->ops_     = init_op;

    return idx;
}

static uint32_t
clamp_parallel(uint32_t parallel) {
    if (parallel == 0) {
        parallel = 1;
    }
    else if (parallel > 2 * get_num_cpus()) {
        parallel = 2 * get_num_cpus();
    }
    return parallel;
}

void
run_svml_tests(svml_op_t const * svml_ops,
               uint32_t          num_svml_ops,
               uint32_t          parallel) {

    svml_groups_t  sgroups;
    uint32_t       i, j, max_tgroup_sz;
    uint32_t       nthreads = clamp_parallel(parallel);
    thread_t       tids[nthreads];
    test_group_t   tgroups[nthreads];
    test_group_t **avail_tgroups_base, **avail_tgroups_end;
    thread_attr_t  attr;

    die_assert(num_svml_ops);

    avail_tgroups_base =
        (test_group_t **)safe_zalloc(nthreads * sizeof(void *));

    avail_tgroups_end = avail_tgroups_base + nthreads;
    avail_tgroups     = avail_tgroups_base;
    for (i = 0; i < nthreads; ++i) {
        avail_tgroups_base[i] = tgroups + i;
    }

    max_tgroup_sz = svml_groups_init(&sgroups, svml_ops, num_svml_ops);
    max_tgroup_sz =
        MIN(max_tgroup_sz, ((num_svml_ops + nthreads - 1) / nthreads));
    die_assert(max_tgroup_sz);

    safe_sema_init(&sema, nthreads);
    safe_thread_attr_init(&attr);

    for (i = 0; i < k_test_end; ++i) {
        svml_tester__f test_func = svml_testers[i];
        for (j = 0; j < sgroups.sizes_[i];) {

            test_group_t * tgroup;
            uint32_t       tidx;
            safe_sema_wait(&sema);
            tgroup = pop_tgroup();
            die_assert(avail_tgroups >= avail_tgroups_base);
            die_assert(avail_tgroups <= avail_tgroups_end);
            die_assert(tgroup - tgroups <= CAST(int64_t, nthreads));
            j             = tgroup_init(j, max_tgroup_sz, sgroups.groups_[i],
                                        sgroups.sizes_[i], tgroup);
            tidx          = CAST(uint32_t, tgroup - tgroups);
            tgroup->tidx_ = tidx;
            safe_thread_create(tids + tidx, &attr, test_func, (void *)tgroup);
        }
    }

    safe_sema_wait_till_n(&sema, nthreads);
    safe_sema_destroy(&sema);
    safe_thread_attr_destroy(&attr);
    safe_sfree(avail_tgroups_base, nthreads * sizeof(void *));


    for (i = 0; i < k_test_end; ++i) {
        for (j = 0; j < sgroups.sizes_[i]; ++j) {
            test_op_out(sgroups.groups_[i] + j);
        }
    }
    svml_groups_deinit(&sgroups, num_svml_ops);
}
