#include "lib/algo/sort.h"
#include "sys/sys-info.h"
#include "thread/thread-common.h"
#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/verbosity.h"

#include "svml-tests.h"
#include "svml-func-defs.h"

typedef struct svml_groups {
    uint16_t    sizes_[k_test_end];
    svml_op_t * groups_[k_test_end];
} svml_groups_t;

typedef struct test_group {
    uint32_t          num_ops_;
    svml_op_t const * ops_;
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

#define svml_T_T_runner(T, scratch_buf, runner)                                \
    {                                                                          \
        uint32_t I_tmp_i_;                                                     \
        for (I_tmp_i_ = 0; I_tmp_i_ < 64; I_tmp_i_ += sizeof(T)) {             \
            T I_tmp_arg_;                                                      \
            __builtin_memcpy(&I_tmp_arg_, (scratch_buf) + i, sizeof(T));       \
            I_tmp_arg_ = runner(I_tmp_arg_);                                   \
            __builtin_memcpy((scratch_buf) + i, &I_tmp_arg_, sizeof(T));       \
        }                                                                      \
    }

#define test_print_progress(op_i)                                              \
    if (ntests % k_partial == 0) {                                             \
        print("%32s -> %4u / %4u\n", (op_i).name_, ntests / k_partial,         \
              k_report_unit);                                                  \
    }

static void *
svml_tester_f_f(void * arg) {
    enum {
        k_test_width  = 64,
        k_report_unit = 1024,
        k_partial     = (1ul << 32) / k_report_unit
    };

    test_group_t const *    tgroup  = (test_group_t const *)arg;
    uint32_t const          num_ops = tgroup->num_ops_;
    svml_op_t const * const ops     = tgroup->ops_;
    uint32_t                i, ntests;
    uint8_t                 scratch[k_test_width * 2];
    __m512i                 cur_cnt, incr;
    safe_thread_detach_self();


    cur_cnt =
        _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    incr = _mm512_set1_epi32(16);


    die_assert(num_ops);
    ntests = 0;

    for (;;) {
        for (i = 0; i < k_test_width; i += 64) {
            __builtin_memcpy(scratch + i, &cur_cnt, 64);
            __builtin_memcpy(scratch + i + k_test_width, &cur_cnt, 64);
            cur_cnt = _mm512_add_epi32(cur_cnt, incr);
        }
        svml_T_T_runner(float, scratch, ops->ref_func_.run_ref_f_f);

        {
            uint32_t sz;
            i = 0;
            trace_print();
            for (sz = ops[i].sz_; i < num_ops && sz == 16; ++i) {
                test_print_progress(ops[i]);
                svml_T_T_runner(__m128i, scratch + k_test_width,
                                ops[i].svml_func_.run_f_f_16);
                trace_print();
            }
            goto done;
            for (sz = ops[i].sz_; i < num_ops && sz == 32; ++i) {
                test_print_progress(ops[i]);
                svml_T_T_runner(__m256i, scratch + k_test_width,
                                ops[i].svml_func_.run_f_f_32);
            }
            for (; i < num_ops; ++i) {
                test_print_progress(ops[i]);
                svml_T_T_runner(__m512i, scratch + k_test_width,
                                ops[i].svml_func_.run_f_f_64);
            }
        }

        ntests += 16 * (k_test_width / 64);
        if (ntests == 0) {
            break;
        }
    }

done:
    push_tgroup(arg);
    safe_sema_post(&sema);
    return NULL;
}


static void *
svml_tester_d_d(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_d_d_d(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_v_d_dp_dp(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_v_f_fp_fp(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

static void *
svml_tester_f_f_f(void * arg) {
    test_group_t const * tgroup = (test_group_t const *)arg;
    (void)tgroup;
    die("Unimplemented!");
    return NULL;
}

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
typedef void * (*svml_tester__f)(void *);
static const svml_tester__f svml_testers[] = {
    svml_tester_f_f,       svml_tester_d_d,       svml_tester_d_d_d,
    svml_tester_v_d_dp_dp, svml_tester_v_f_fp_fp, svml_tester_f_f_f,
    svml_tester_lli_d,     svml_tester_lli_f,     svml_tester_i_d,
    svml_tester_i_f,
};

static void
svml_groups_deinit(svml_groups_t * sgroups, uint32_t num_svml_ops) {
    safe_sfree(sgroups->groups_[0], num_svml_ops * sizeof(svml_op_t));
}

static uint32_t
svml_groups_init(svml_groups_t *   sgroups,
                 svml_op_t const * svml_ops,
                 uint32_t          num_svml_ops) {
    uint32_t    i, j, max_tgroup_sz;
    uint32_t    offset;
    svml_op_t * ordered_ops =
        (svml_op_t *)safe_zalloc(num_svml_ops * sizeof(svml_op_t));

    __builtin_memset(sgroups, 0, sizeof(*sgroups));
    for (i = 0; i < num_svml_ops; ++i) {
        sgroups->sizes_[svml_ops[i].test_type_] += 1;
        __builtin_memcpy(ordered_ops + i, svml_ops + i, sizeof(*svml_ops));
    }

    lsort(ordered_ops, num_svml_ops, lhs->test_type_ > rhs->test_type_);

    offset = 0;
    for (i = 0; i < k_test_end; ++i) {
        sgroups->groups_[i] = ordered_ops + offset;
        offset += sgroups->sizes_[i];
    }
    die_assert(offset == num_svml_ops);
    for (i = 0; i < k_test_end; ++i) {
        for (j = 0; j < sgroups->sizes_[i]; ++j) {
            die_assert(sgroups->groups_[i][j].test_type_ == i);
        }
    }

    for (i = 0; i < k_test_end; ++i) {
        lsort(sgroups->groups_[i], sgroups->sizes_[i],
              (lhs->ref_func_.p > rhs->ref_func_.p)
                  ? 1
                  : (lhs->ref_func_.p == rhs->ref_func_.p ? lhs->sz_ > rhs->sz_
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
            void const * curp = sgroups->groups_[i][j].ref_func_.p;
            die_assert(curp >= lastp);
            if (curp != lastp) {
                max_tgroup_sz = MAX(max_tgroup_sz, (j - lastj));

                lastj       = j;
                lastp       = curp;
                dbg_last_sz = sgroups->groups_[i][j].sz_;
            }
            else {

                die_assert(dbg_last_sz <= sgroups->groups_[i][j].sz_);
                dbg_last_sz = sgroups->groups_[i][j].sz_;
            }
        }
        max_tgroup_sz = MAX(max_tgroup_sz, (j - lastj));
    }

    die_assert(max_tgroup_sz < 16);
    return max_tgroup_sz;
}

static uint32_t
tgroup_init(uint32_t          idx,
            uint32_t          max_group_sz,
            svml_op_t const * ops,
            uint32_t          num_ops,
            test_group_t *    tgroup) {
    svml_op_t const * init_op;
    uint32_t          init_idx;
    die_assert(idx < num_ops);

    init_idx = idx;
    init_op  = ops + idx;
    do {
        ++idx;
        --max_group_sz;
    } while (max_group_sz && idx < num_ops &&
             init_op->ref_func_.p == ops[idx].ref_func_.p);

    tgroup->num_ops_ = (idx - init_idx);
    tgroup->ops_     = init_op;

    return idx;
}

static uint32_t
clamp_parallel(uint32_t parallel) {
    if (parallel == 0) {
        parallel = 1;
    }
    else if (parallel > get_num_cpus()) {
        parallel = get_num_cpus();
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
            uint32_t       t_idx;
            safe_sema_wait(&sema);
            tgroup = pop_tgroup();
            die_assert(avail_tgroups >= avail_tgroups_base);
            die_assert(avail_tgroups <= avail_tgroups_end);
            die_assert(tgroup - tgroups <= CAST(int64_t, nthreads));
            j     = tgroup_init(j, max_tgroup_sz, sgroups.groups_[i],
                                sgroups.sizes_[i], tgroup);
            t_idx = CAST(uint32_t, tgroup - tgroups);
            safe_thread_create(tids + t_idx, &attr, test_func, (void *)tgroup);
        }
    }

    safe_sema_wait_till_n(&sema, nthreads);
    safe_sema_destroy(&sema);
    safe_thread_attr_destroy(&attr);
    safe_sfree(avail_tgroups_base, nthreads * sizeof(void *));
    svml_groups_deinit(&sgroups, num_svml_ops);
}
