#ifndef use_fp32
# error "Need specifier"
#endif


#ifndef SVML_TESTS_COMMON
# define SVML_TESTS_COMMON
# define svml_T_T_runner(T, scratch_buf, runner)                               \
        {                                                                      \
            uint32_t I_tmp_i_;                                                 \
            for (I_tmp_i_ = 0; I_tmp_i_ < k_test_width;                        \
                 I_tmp_i_ += sizeof(T)) {                                      \
                T I_tmp_arg_;                                                  \
                __builtin_memcpy(&I_tmp_arg_, (scratch_buf) + I_tmp_i_,        \
                                 sizeof(T));                                   \
                I_tmp_arg_ = runner(I_tmp_arg_);                               \
                __builtin_memcpy((scratch_buf) + I_tmp_i_, &I_tmp_arg_,        \
                                 sizeof(T));                                   \
            }                                                                  \
        }

# define svml_T_T_T_runner(T, scratch_buf0, scratch_buf1, runner)              \
        {                                                                      \
            uint32_t I_tmp_i_;                                                 \
            for (I_tmp_i_ = 0; I_tmp_i_ < k_test_width;                        \
                 I_tmp_i_ += sizeof(T)) {                                      \
                T I_tmp_arg0_, I_tmp_arg1_, I_tmp_res_;                        \
                __builtin_memcpy(&I_tmp_arg0_, (scratch_buf0) + I_tmp_i_,      \
                                 sizeof(T));                                   \
                __builtin_memcpy(&I_tmp_arg1_, (scratch_buf1) + I_tmp_i_,      \
                                 sizeof(T));                                   \
                I_tmp_res_ = runner(I_tmp_arg0_, I_tmp_arg1_);                 \
                __builtin_memcpy((scratch_buf0) + I_tmp_i_, &I_tmp_res_,       \
                                 sizeof(T));                                   \
            }                                                                  \
        }

# define svml_v_T_Tp_Tp_runner(T, scratch_buf0, scratch_buf1, runner)          \
        {                                                                      \
            uint32_t I_tmp_i_;                                                 \
            for (I_tmp_i_ = 0; I_tmp_i_ < k_test_width;                        \
                 I_tmp_i_ += sizeof(T)) {                                      \
                T I_tmp_arg_, I_tmp_res0_, I_tmp_res1_;                        \
                __builtin_memcpy(&I_tmp_arg_, (scratch_buf0) + I_tmp_i_,       \
                                 sizeof(T));                                   \
                runner(I_tmp_arg_, &I_tmp_res0_, &I_tmp_res1_);                \
                __builtin_memcpy((scratch_buf0) + I_tmp_i_, &I_tmp_res0_,      \
                                 sizeof(T));                                   \
                __builtin_memcpy((scratch_buf1) + I_tmp_i_, &I_tmp_res1_,      \
                                 sizeof(T));                                   \
            }                                                                  \
        }


# define test_print_progress(op_i)                                             \
        if (UNLIKELY(test_cnt % k_progress_output == 0)) {                     \
            print("[%4u]: %-32s -> %12u\n", tidx, (op_i).name_, test_cnt);     \
        }
#endif


#if use_fp32
# define T                 float
# define T_T               f_f
# define T_T_T             f_f_f
# define v_T_Tp_Tp         v_f_fp_fp
# define T_eq              float_eq
# define run_ref_T_T       run_ref_f_f
# define run_ref_T_T_T     run_ref_f_f_f
# define run_ref_v_T_Tp_Tp run_ref_v_f_fp_fp
# define T_state_t         flt_state_t
# define T_state_next      flt_state_next
# define T_state_next2     flt_state_next2
# define T_state_init      flt_state_init
#else
# define T                 double
# define T_T               d_d
# define T_T_T             d_d_d
# define v_T_Tp_Tp         v_d_dp_dp
# define T_eq              double_eq
# define run_ref_T_T       run_ref_d_d
# define run_ref_T_T_T     run_ref_d_d_d
# define run_ref_v_T_Tp_Tp run_ref_v_d_dp_dp
# define T_state_t         dbl_state_t
# define T_state_next      dbl_state_next
# define T_state_next2     dbl_state_next2
# define T_state_init      dbl_state_init
#endif

static void *
CAT(svml_tester_, T_T)(void * arg) {
    test_group_t const *   tgroup  = (test_group_t const *)arg;
    uint32_t const         num_ops = tgroup->num_ops_;
    uint32_t const         tidx    = tgroup->tidx_;
    test_svml_op_t * const ops     = tgroup->ops_;
    uint32_t               i, test_cnt;
    uint8_t                scratch[k_test_width * (2 + WITH_DBG_OUTPUT)];
    uint8_t                hist[num_ops][64 * (k_max_ulp + 1)];
    T_state_t              test_state;


    safe_thread_detach_self();
    __builtin_memset(hist, 0, num_ops * 64 * (k_max_ulp + 1));

    T_state_init(&test_state);

    die_assert(num_ops);

    test_cnt = 0;
    for (;;) {
        if (T_state_next(&test_state, scratch)) {
            break;
        }


#if WITH_DBG_OUTPUT
        __builtin_memcpy(scratch + k_test_width * 2, scratch + k_test_width,
                         k_test_width);
#endif
        svml_T_T_runner(T, scratch, ops->op_.ref_func_.run_ref_T_T);
        {
            uint32_t sz;
            i = 0;
            for (sz = ops[i].op_.sz_; i < num_ops && sz == 16; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_runner(__m128i, scratch + k_test_width,
                                ops[i].op_.svml_func_.run_f_f_16);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }

            for (sz = ops[i].op_.sz_; i < num_ops && sz == 32; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_runner(__m256i, scratch + k_test_width,
                                ops[i].op_.svml_func_.run_f_f_32);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }
            for (; i < num_ops; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_runner(__m512i, scratch + k_test_width,
                                ops[i].op_.svml_func_.run_f_f_64);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }
        }
        test_cnt += 1;
    }
    for (i = 0; i < num_ops; ++i) {
        create_ulp_hist(sizeof(T), (uint8_t *)&hist[i], &(ops[i].hist_));
    }

    push_tgroup(arg);
    safe_sema_post(&sema);
    return NULL;
}


static void *
CAT(svml_tester_, T_T_T)(void * arg) {
    test_group_t const *   tgroup  = (test_group_t const *)arg;
    uint32_t const         num_ops = tgroup->num_ops_;
    uint32_t const         tidx    = tgroup->tidx_;
    test_svml_op_t * const ops     = tgroup->ops_;
    uint32_t               i, test_cnt;
    uint8_t                scratch0[k_test_width * (2 + WITH_DBG_OUTPUT)];
    uint8_t                scratch1[k_test_width * (2 + WITH_DBG_OUTPUT)];
    uint8_t                hist[num_ops][64 * (k_max_ulp + 1)];
    T_state_t              test_state;


    safe_thread_detach_self();
    __builtin_memset(hist, 0, num_ops * 64 * (k_max_ulp + 1));

    T_state_init(&test_state);

    die_assert(num_ops);

    test_cnt = 0;
    for (;;) {
        if (T_state_next2(&test_state, scratch0, scratch1)) {
            break;
        }


#if WITH_DBG_OUTPUT
        __builtin_memcpy(scratch0 + k_test_width * 2, scratch0 + k_test_width,
                         k_test_width);
        __builtin_memcpy(scratch1 + k_test_width * 2, scratch1 + k_test_width,
                         k_test_width);
#endif
        svml_T_T_T_runner(T, scratch0, scratch1,
                          ops->op_.ref_func_.run_ref_T_T_T);
        {
            uint32_t sz;
            i = 0;
            for (sz = ops[i].op_.sz_; i < num_ops && sz == 16; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_T_runner(__m128i, scratch0 + k_test_width,
                                  scratch1 + k_test_width,
                                  ops[i].op_.svml_func_.run_f_f_f_16);
                die_assert(T_eq(scratch0, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }

            for (sz = ops[i].op_.sz_; i < num_ops && sz == 32; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_T_runner(__m256i, scratch0 + k_test_width,
                                  scratch1 + k_test_width,
                                  ops[i].op_.svml_func_.run_f_f_f_32);
                die_assert(T_eq(scratch0, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }
            for (; i < num_ops; ++i) {
                test_print_progress(ops[i].op_);
                svml_T_T_T_runner(__m512i, scratch0 + k_test_width,
                                  scratch1 + k_test_width,
                                  ops[i].op_.svml_func_.run_f_f_f_64);
                die_assert(T_eq(scratch0, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
            }
        }
        test_cnt += 1;
    }
    for (i = 0; i < num_ops; ++i) {
        create_ulp_hist(sizeof(T), (uint8_t *)&hist[i], &(ops[i].hist_));
    }

    push_tgroup(arg);
    safe_sema_post(&sema);
    return NULL;
}


static void *
CAT(svml_tester_, v_T_Tp_Tp)(void * arg) {
    test_group_t const *   tgroup  = (test_group_t const *)arg;
    uint32_t const         num_ops = tgroup->num_ops_;
    uint32_t const         tidx    = tgroup->tidx_;
    test_svml_op_t * const ops     = tgroup->ops_;
    uint32_t               i, test_cnt;
    uint8_t                scratch[k_test_width * (2 + WITH_DBG_OUTPUT)];
    uint8_t                scratch_res[k_test_width * 2];
    uint8_t                hist[num_ops][64 * (k_max_ulp + 1)];
    T_state_t              test_state;


    safe_thread_detach_self();
    __builtin_memset(hist, 0, num_ops * 64 * (k_max_ulp + 1));

    T_state_init(&test_state);

    die_assert(num_ops);

    test_cnt = 0;
    for (;;) {
        if (T_state_next(&test_state, scratch)) {
            break;
        }


#if WITH_DBG_OUTPUT
        __builtin_memcpy(scratch + k_test_width * 2, scratch + k_test_width,
                         k_test_width);
#endif
        svml_v_T_Tp_Tp_runner(T, scratch, scratch_res,
                              ops->op_.ref_func_.run_ref_v_T_Tp_Tp);
        {
            uint32_t sz;
            i = 0;
            for (sz = ops[i].op_.sz_; i < num_ops && sz == 16; ++i) {
                test_print_progress(ops[i].op_);
                svml_v_T_Tp_Tp_runner(__m128i, scratch + k_test_width,
                                scratch_res + k_test_width,
                                ops[i].op_.svml_func_.run_v_f_fp_fp_16);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
                die_assert(
                    T_eq(scratch_res, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                    "Failed(%u): %s\n", ops[i].op_.ulp_, ops[i].op_.name_);
            }

            for (sz = ops[i].op_.sz_; i < num_ops && sz == 32; ++i) {
                test_print_progress(ops[i].op_);
                svml_v_T_Tp_Tp_runner(__m256i, scratch + k_test_width,
                                scratch_res + k_test_width,
                                ops[i].op_.svml_func_.run_v_f_fp_fp_32);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
                die_assert(
                    T_eq(scratch_res, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                    "Failed(%u): %s\n", ops[i].op_.ulp_, ops[i].op_.name_);
            }
            for (; i < num_ops; ++i) {
                test_print_progress(ops[i].op_);
                svml_v_T_Tp_Tp_runner(__m512i, scratch + k_test_width,
                                scratch_res + k_test_width,
                                ops[i].op_.svml_func_.run_v_f_fp_fp_64);
                die_assert(T_eq(scratch, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                           "Failed(%u): %s\n", ops[i].op_.ulp_,
                           ops[i].op_.name_);
                die_assert(
                    T_eq(scratch_res, ops[i].op_.ulp_, (uint8_t *)&hist[i]),
                    "Failed(%u): %s\n", ops[i].op_.ulp_, ops[i].op_.name_);
            }
        }
        test_cnt += 1;
    }
    for (i = 0; i < num_ops; ++i) {
        create_ulp_hist(sizeof(T), (uint8_t *)&hist[i], &(ops[i].hist_));
    }

    push_tgroup(arg);
    safe_sema_post(&sema);
    return NULL;
}


#undef T
#undef T_T
#undef T_T_T
#undef v_T_Tp_Tp
#undef T_eq
#undef run_ref_T_T
#undef run_ref_T_T_T
#undef run_ref_v_T_Tp_Tp
#undef T_state_t
#undef T_state_next
#undef T_state_init
#undef T_state_next2
