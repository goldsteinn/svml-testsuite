#include "test/test-common.h"

#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/random.h"
#include "util/types.h"

#include "lib/hash/city-hash.h"

typedef uint32_t k_t;
typedef uint32_t v_t;

#define hl_key_t   k_t
#define hl_val_t   v_t
#define hl_hash(x) hash64(x)
#define hl_name    uu

#include "rh-hashtable.h"

#define hl_key_t   k_t
#define hl_val_t   v_t
#define hl_hash(x) hash64(x)
#define hl_name    u

#include "rh-hashtable.h"


enum { TEST_SIZE = (1 << 24) };

static int32_t
test_init() {
    vprint("Running: test_init\n");
    u_table_t tbl;
    uint32_t  i;

    test_assert(u_init(&tbl) == NULL);
    u_deinit(&tbl);
    for (i = 0; i <= (1 << 12); ++i) {
        test_assert(u_init_sz(&tbl, i) == NULL);
        test_assert(u_capacity(&tbl) >= i);
        u_deinit(&tbl);
    }
    for (; i < (1 << 24); i += i) {
        test_assert(u_init_sz(&tbl, i) == NULL);
        test_assert(u_capacity(&tbl) >= i);
        u_deinit(&tbl);
    }
    return 0;
}


static void
init_kvps(k_t * keys, v_t * vals) {
    uint32_t * rindexes = make_true_rand32_buffer(TEST_SIZE);
    uint32_t   i;
    for (i = 0; i < TEST_SIZE; ++i) {
        keys[i] = i;
        vals[i] = TEST_SIZE + i;
    }
    for (i = 0; i < TEST_SIZE * 2; ++i) {
        uint32_t from_idx   = rindexes[i % TEST_SIZE] % TEST_SIZE;
        k_t      from_k     = keys[from_idx];
        keys[from_idx]      = keys[i % TEST_SIZE];
        keys[i % TEST_SIZE] = from_k;
    }
    safe_sfree(rindexes, TEST_SIZE * sizeof(uint32_t));
}

static int32_t
test_i() {
    vprint("Running: test_i\n");
    uint32_t i, t;
    k_t *    keys = (k_t *)safe_malloc(TEST_SIZE * sizeof(k_t));
    v_t *    vals = (v_t *)safe_malloc(TEST_SIZE * sizeof(v_t));
    init_kvps(keys, vals);

    for (i = 1; i <= TEST_SIZE; i += ((i * 3) / 2)) {
        vprint("\tTest Size: %u\n", i);
        uint32_t       j;
        u_table_t      tbl;
        u_insert_ret_t ret0, ret1;
        u_init(&tbl);
        for (t = 0; t < 10; ++t) {
            for (j = 0; j < i; ++j) {
                ret0 = u_insert(&tbl, keys[j]);
                test_assert(ret0.already_exists_ == 0);
                test_assert(ret0.kvp_->key_ == keys[j]);
                ret0.kvp_->val_ = vals[j];

                ret1 = u_insert(&tbl, keys[j]);
                test_assert(ret1.already_exists_ != 0);
                test_assert(ret1.kvp_->key_ == keys[j]);
                test_assert(ret1.kvp_->val_ == vals[j]);

                test_assert(ret0.kvp_ == ret1.kvp_);
            }

            for (j = 0; j < i; ++j) {
                ret0 = u_insert(&tbl, keys[j]);
                test_assert(ret0.already_exists_ != 0);
                test_assert(ret0.kvp_->key_ == keys[j]);
                test_assert(ret0.kvp_->val_ == vals[j]);
            }

            u_shrink(&tbl, true_rand32() % (TEST_SIZE * 2));
        }


        u_deinit(&tbl);
    }
    safe_sfree(keys, TEST_SIZE * sizeof(k_t));
    safe_sfree(vals, TEST_SIZE * sizeof(v_t));
    return 0;
}

static int32_t
test_if() {
    vprint("Running: test_if\n");
    uint32_t i;
    k_t *    keys = (k_t *)safe_malloc(TEST_SIZE * sizeof(k_t));
    v_t *    vals = (v_t *)safe_malloc(TEST_SIZE * sizeof(v_t));
    init_kvps(keys, vals);

    for (i = 1; i <= TEST_SIZE; i += ((i * 3) / 2)) {
        vprint("\tTest Size: %u\n", i);
        uint32_t       j;
        u_table_t      tbl;
        u_insert_ret_t iret;
        u_kvp_t *      fkvp;
        u_init(&tbl);

        for (j = 0; j < i; ++j) {
            fkvp = u_find(&tbl, keys[j]);
            test_assert(fkvp == rh_find_failed);

            iret = u_insert(&tbl, keys[j]);
            test_assert(iret.already_exists_ == 0);
            test_assert(iret.kvp_->key_ == keys[j]);
            iret.kvp_->val_ = vals[j];

            fkvp = u_find(&tbl, keys[j]);
            test_assert(fkvp == iret.kvp_);
            test_assert(fkvp->key_ == keys[j]);
            test_assert(fkvp->val_ == vals[j]);

            iret = u_insert(&tbl, keys[j]);
            test_assert(iret.already_exists_ == 1);
            test_assert(fkvp == iret.kvp_);
        }

        for (j = 0; j < i; ++j) {
            fkvp = u_find(&tbl, keys[j]);
            test_assert(fkvp != rh_find_failed);
            test_assert(fkvp->key_ == keys[j]);
            test_assert(fkvp->val_ == vals[j]);

            iret = u_insert(&tbl, keys[j]);
            test_assert(iret.already_exists_ != 0);
            test_assert(iret.kvp_->key_ == keys[j]);
            test_assert(iret.kvp_->val_ == vals[j]);

            test_assert(fkvp == iret.kvp_);
        }

        u_clear(&tbl);
        for (j = 0; j < i; ++j) {
            fkvp = u_find(&tbl, keys[j]);
            test_assert(fkvp == rh_find_failed);

            iret = u_insert(&tbl, keys[j]);
            test_assert(iret.already_exists_ == 0);
            test_assert(iret.kvp_->key_ == keys[j]);
            iret.kvp_->val_ = vals[j];

            fkvp = u_find(&tbl, keys[j]);
            test_assert(fkvp == iret.kvp_);
            test_assert(fkvp->key_ == keys[j]);
            test_assert(fkvp->val_ == vals[j]);

            iret = u_insert(&tbl, keys[j]);
            test_assert(iret.already_exists_ == 1);
            test_assert(fkvp == iret.kvp_);
        }


        u_deinit(&tbl);
    }
    safe_sfree(keys, TEST_SIZE * sizeof(k_t));
    safe_sfree(vals, TEST_SIZE * sizeof(v_t));
    return 0;
}


static int32_t
test_ie() {
    vprint("Running: test_ie\n");
    uint32_t i;
    k_t *    keys = (k_t *)safe_malloc(TEST_SIZE * sizeof(k_t));
    v_t *    vals = (v_t *)safe_malloc(TEST_SIZE * sizeof(v_t));
    init_kvps(keys, vals);

    for (i = 1; i <= TEST_SIZE; i += ((i * 3) / 2)) {
        vprint("\tTest Size: %u\n", i);
        uint32_t       j, r;
        u_table_t      tbl;
        u_insert_ret_t ret0, ret1;
        u_init(&tbl);

        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {
                test_assert(u_erase(&tbl, keys[j]));

                ret0 = u_insert(&tbl, keys[j]);
                test_assert(ret0.already_exists_ == 0);
                test_assert(ret0.kvp_->key_ == keys[j]);
                ret0.kvp_->val_ = vals[j];

                ret1 = u_insert(&tbl, keys[j]);
                test_assert(ret1.already_exists_ != 0);
                test_assert(ret1.kvp_->key_ == keys[j]);
                test_assert(ret1.kvp_->val_ == vals[j]);

                test_assert(ret0.kvp_ == ret1.kvp_);

                test_assert(u_erase(&tbl, keys[j]) == 0);

                ret0 = u_insert(&tbl, keys[j]);
                test_assert(ret0.already_exists_ == 0);
                test_assert(ret0.kvp_->key_ == keys[j]);
                ret0.kvp_->val_ = vals[j];

                ret1 = u_insert(&tbl, keys[j]);
                test_assert(ret1.already_exists_ != 0);
                test_assert(ret1.kvp_->key_ == keys[j]);
                test_assert(ret1.kvp_->val_ == vals[j]);

                test_assert(ret0.kvp_ == ret1.kvp_);
            }

            for (j = 0; j < i; ++j) {
                ret0 = u_insert(&tbl, keys[j]);
                test_assert(ret0.already_exists_ != 0);
                test_assert(ret0.kvp_->key_ == keys[j]);
                test_assert(ret0.kvp_->val_ == vals[j]);

                test_assert(u_erase(&tbl, keys[j]) == 0);
                test_assert(u_erase(&tbl, keys[j]));
            }
        }

        u_clear(&tbl);
        for (j = 0; j < i; ++j) {
            test_assert(u_erase(&tbl, keys[j]));

            ret0 = u_insert(&tbl, keys[j]);
            test_assert(ret0.already_exists_ == 0);
            test_assert(ret0.kvp_->key_ == keys[j]);
            ret0.kvp_->val_ = vals[j];

            ret1 = u_insert(&tbl, keys[j]);
            test_assert(ret1.already_exists_ != 0);
            test_assert(ret1.kvp_->key_ == keys[j]);
            test_assert(ret1.kvp_->val_ == vals[j]);

            test_assert(ret0.kvp_ == ret1.kvp_);

            test_assert(u_erase(&tbl, keys[j]) == 0);

            ret0 = u_insert(&tbl, keys[j]);
            test_assert(ret0.already_exists_ == 0);
            test_assert(ret0.kvp_->key_ == keys[j]);
            ret0.kvp_->val_ = vals[j];

            ret1 = u_insert(&tbl, keys[j]);
            test_assert(ret1.already_exists_ != 0);
            test_assert(ret1.kvp_->key_ == keys[j]);
            test_assert(ret1.kvp_->val_ == vals[j]);

            test_assert(ret0.kvp_ == ret1.kvp_);
        }

        u_deinit(&tbl);
    }
    safe_sfree(keys, TEST_SIZE * sizeof(k_t));
    safe_sfree(vals, TEST_SIZE * sizeof(v_t));
    return 0;
}


static int32_t
test_ief() {
    vprint("Running: test_ief\n");
    uint32_t i;
    k_t *    keys = (k_t *)safe_malloc(TEST_SIZE * sizeof(k_t));
    v_t *    vals = (v_t *)safe_malloc(TEST_SIZE * sizeof(v_t));
    init_kvps(keys, vals);

    for (i = 1; i <= TEST_SIZE; i += ((i * 3) / 2)) {
        vprint("\tTest Size: %u\n", i);
        uint32_t       j, r;
        u_table_t      tbl;
        u_insert_ret_t iret;
        u_kvp_t *      fkvp;
        u_init(&tbl);

        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {
                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp == rh_find_failed);
                test_assert(u_erase(&tbl, keys[j]));

                iret = u_insert(&tbl, keys[j]);
                test_assert(iret.already_exists_ == 0);
                test_assert(iret.kvp_->key_ == keys[j]);
                iret.kvp_->val_ = vals[j];

                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp == iret.kvp_);
                test_assert(fkvp->key_ == keys[j]);
                test_assert(fkvp->val_ == vals[j]);

                test_assert(u_erase(&tbl, keys[j]) == 0);
                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp == rh_find_failed);

                iret = u_insert(&tbl, keys[j]);
                test_assert(iret.already_exists_ == 0);
                test_assert(iret.kvp_->key_ == keys[j]);
                iret.kvp_->val_ = vals[j];

                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp == iret.kvp_);
                test_assert(fkvp->key_ == keys[j]);
                test_assert(fkvp->val_ == vals[j]);

                iret = u_insert(&tbl, keys[j]);
                test_assert(iret.already_exists_ == 1);
                test_assert(fkvp == iret.kvp_);
            }

            for (j = 0; j < i; ++j) {
                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp != rh_find_failed);
                test_assert(fkvp->key_ == keys[j]);
                test_assert(fkvp->val_ == vals[j]);

                iret = u_insert(&tbl, keys[j]);
                test_assert(iret.already_exists_ != 0);
                test_assert(iret.kvp_->key_ == keys[j]);
                test_assert(iret.kvp_->val_ == vals[j]);

                test_assert(fkvp == iret.kvp_);

                test_assert(u_erase(&tbl, keys[j]) == 0);
                fkvp = u_find(&tbl, keys[j]);
                test_assert(fkvp == rh_find_failed);

                test_assert(u_erase(&tbl, keys[j]));
            }
        }

        u_deinit(&tbl);
    }
    safe_sfree(keys, TEST_SIZE * sizeof(k_t));
    safe_sfree(vals, TEST_SIZE * sizeof(v_t));
    return 0;
}


static int32_t
test_ief_rand(uint32_t keep_small) {
    vprint("Running: test_ief_rand\n");
    uint32_t i;
    k_t *    keys  = (k_t *)safe_malloc(TEST_SIZE * sizeof(k_t));
    v_t *    vals  = (v_t *)safe_malloc(TEST_SIZE * sizeof(v_t));
    _Bool *  state = (_Bool *)safe_zalloc(TEST_SIZE);
    init_kvps(keys, vals);


    for (i = 1; i <= TEST_SIZE; i += i) {
        vprint("\tTest Size: %u\n", i);
        uint32_t  j, r;
        u_table_t tbl;
        u_init(&tbl);
        for (r = 0; r < 2; ++r) {
            for (j = 0; j < i; ++j) {

                uint32_t idx  = rand() & (i - 1);
                uint32_t todo = rand() % 4;
                if (keep_small && todo == 0) {
                    todo = rand() % 4;
                }
                switch (todo) {
                    case 0:
                        if (r) {
                            goto rerase;
                        }
                        fall_through;
                    case 1:
                        u_insert_ret_t ir = u_insert(&tbl, keys[idx]);
                        test_assert(state[idx] == ir.already_exists_,
                                    "(r=%d, idx=%u): %d != %d\n", r, idx,
                                    state[idx], ir.already_exists_);
                        if (!state[idx]) {
                            state[idx]    = 1;
                            ir.kvp_->val_ = vals[idx];
                        }
                        test_assert(ir.kvp_->key_ == keys[idx]);
                        test_assert(ir.kvp_->val_ == vals[idx]);
                        break;
                    case 2:
                        u_kvp_t * fr = u_find(&tbl, keys[idx]);
                        if (state[idx]) {
                            test_assert(fr != rh_find_failed,
                                        "(r=%d, idx=%u)\n", r, idx);
                            test_assert(fr->key_ == keys[idx]);
                            test_assert(fr->val_ == vals[idx]);
                        }
                        else {
                            test_assert(fr == rh_find_failed);
                        }
                        break;
                    case 3:
                    rerase:
                        test_assert(u_erase(&tbl, keys[idx]) == !state[idx]);
                        state[idx] = 0;
                        break;
                }
            }

            if (!r) {
                for (j = 0; j < i; ++j) {
                    u_kvp_t *      fr = u_find(&tbl, keys[j]);
                    u_insert_ret_t ir = u_insert(&tbl, keys[j]);

                    if (state[j]) {
                        test_assert(fr != rh_find_failed);
                        test_assert(fr->key_ == keys[j]);
                        test_assert(fr->val_ == vals[j]);
                        test_assert(ir.already_exists_);
                        test_assert(fr == ir.kvp_);
                    }
                    else {
                        test_assert(fr == rh_find_failed);
                        ir.kvp_->val_ = vals[j];
                        test_assert(ir.kvp_->key_ == keys[j]);
                        test_assert(ir.kvp_->val_ == vals[j]);
                    }
                    state[j] = 1;
                    ir       = u_insert(&tbl, keys[j]);
                    test_assert(ir.already_exists_);
                    test_assert(u_find(&tbl, keys[j]) != rh_find_failed);
                }
            }
            else {
                for (j = 0; j < i; ++j) {
                    u_kvp_t * fr = u_find(&tbl, keys[j]);


                    if (state[j]) {
                        test_assert(fr != rh_find_failed);
                        test_assert(fr->key_ == keys[j]);
                        test_assert(fr->val_ == vals[j]);
                    }
                    else {
                        test_assert(fr == rh_find_failed);
                    }
                    test_assert(u_erase(&tbl, keys[j]) != state[j]);
                }
            }
        }
        memset_c(state, 0, i);
        u_deinit(&tbl);
    }

    safe_sfree(keys, TEST_SIZE * sizeof(k_t));
    safe_sfree(vals, TEST_SIZE * sizeof(v_t));
    safe_sfree(state, TEST_SIZE);
    return 0;
}

int32_t
test_rh_hashtable() {
    test_assert(test_init() == 0);
    test_assert(test_i() == 0);
    test_assert(test_if() == 0);
    test_assert(test_ie() == 0);
    test_assert(test_ief() == 0);
    test_assert(test_ief_rand(0) == 0);
    test_assert(test_ief_rand(1) == 0);
    return 0;
}
