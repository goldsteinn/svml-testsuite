#include "test/test-common.h"

#include "util/inline-math.h"
#include "util/memory-util.h"
#include "util/random.h"
#include "util/types.h"

#include "lib/hash/hash.h"
#define equals_val(rhs, lhs)                                                   \
    ({                                                                         \
        get_type(lhs) I_tmp_lhs_ = (lhs);                                      \
        get_type(rhs) I_tmp_rhs_ = (rhs);                                      \
        (!memcmpeq_c((uint8_t const *)(&I_tmp_lhs_),                           \
                     (uint8_t const *)(&I_tmp_rhs_), sizeof(rhs)));            \
    })
#define equals_ptr(rhs, lhs)                                                   \
    ({                                                                         \
        get_type(lhs) I_tmp_lhs_ = (lhs);                                      \
        get_type(rhs) I_tmp_rhs_ = (rhs);                                      \
        (!memcmpeq_c((uint8_t const *)(I_tmp_lhs_),                            \
                     (uint8_t const *)(I_tmp_rhs_), sizeof(rhs)));             \
    })

enum { TEST_SIZE = 1 << 18 };

typedef struct b16 {
    uint8_t bytes_[16];
} b16_t; /* NOLINT(altera-struct-pack-align) */

#define test_key_t   uint32_t
#define test_hash(x) xxhashT(x)
#define test_name    k4
#define HASHTABLE_H  "rh-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t   uint32_t
#define test_hash(x) xxhashT(x)
#define test_name    k4i
#define HASHTABLE_H  "rh-hashtable.h"
#include "hashtable.test.h"

#define test_key_t                    uint32_t
#define test_hash(x)                  xxhashT(x)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4_nt
#define HASHTABLE_H                   "rh-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_hash(x)                  xxhashT(x)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4i_nt
#define HASHTABLE_H                   "rh-hashtable.h"
#include "hashtable.test.h"


#define test_key_t   uint32_t
#define test_val_t   uint32_t
#define test_hash(x) (x)
#define test_name    k4v4
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t   uint32_t
#define test_val_t   uint32_t
#define test_hash(x) (x)
#define test_name    k4v4_nt
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t   uint32_t
#define test_val_t   uint32_t
#define test_hash(x) (x)
#define test_name    k4v4e
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t   uint32_t
#define test_val_t   uint32_t
#define test_hash(x) (x)
#define test_name    k4v4e_nt
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_key_t   uint16_t
#define test_hash(x) (x)
#define test_name    k2
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_key_t   uint16_t
#define test_val_t   uint8_t
#define test_hash(x) (x)
#define test_name    k2v1
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_key_t   uint8_t
#define test_hash(x) (x)
#define test_name    k1
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_key_t   uint8_t
#define test_val_t   uint8_t
#define test_hash(x) (x)
#define test_name    k1v1
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_key_t   uint8_t
#define test_val_t   uint32_t
#define test_hash(x) (x)
#define test_name    k1v4
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_key_t   uint8_t
#define test_val_t   b16_t
#define test_hash(x) (x)
#define test_name    k1v16
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t   uint8_t
#define test_val_t   b16_t
#define test_hash(x) (x)
#define test_name    k1v16e
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_key_t   b16_t
#define test_val_t   uint64_t
#define test_hash(x) xxhashT(x)
#define test_name    k16v8
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t   b16_t
#define test_val_t   uint64_t
#define test_hash(x) xxhashT(x)
#define test_name    k16e
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t   b16_t
#define test_val_t   uint64_t
#define test_hash(x) xxhashT(x)
#define test_name    k16v8_nt
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_val_t uint64_t
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pv8
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_val_t uint64_t
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pv8e
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8p
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pe
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8p_nt
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_erase_copy
#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pe_nt
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_pass_val(vp) &(vp)
#define test_val_t        uint64_t *
#define test_val_base_t   uint64_t
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pv8p
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_pass_key(kp) &(kp)
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_pass_val(vp) &(vp)
#define test_val_t        uint64_t *
#define test_val_base_t   uint64_t
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k8pv8pe
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"

#define test_pass_key(kp) &(kp)
#define test_key_t        uint32_t const *
#define test_key_base_t   uint32_t
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(rkey, tkey, sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint32_t));                                              \
    })
#define test_hash(x) ((*(x)) * 31)
#define test_name    k4p
#define HASHTABLE_H  "rh-hashtable.h"

#include "hashtable.test.h"


#define test_key_t      uint32_t
#define test_spare_bits 4
#define test_hash(x)    ((x)*31)
#define test_name       k4s4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint64_t
#define test_spare_bits 10
#define test_hash(x)    ((x)*31)
#define test_name       k8s10
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t      uint64_t
#define test_spare_bits 10
#define test_hash(x)    ((x)*31)
#define test_name       k8s10e
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint64_t
#define test_spare_bits 4
#define test_hash(x)    ((x)*31)
#define test_name       k8s4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint16_t
#define test_spare_bits 4
#define test_hash(x)    ((x)*31)
#define test_name       k2s4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"


#define test_pass_key(kp) ((uintptr_t)(&(kp)))
#define test_key_t        uintptr_t
#define test_spare_bits   16
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                  \
    })
#define test_name   k8ps16_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"


#define test_erase_copy
#define test_pass_key(kp) ((uintptr_t)(&(kp)))
#define test_key_t        uintptr_t
#define test_spare_bits   16
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps16_nt_e
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_erase_copy
#define test_null_invalid 1
#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   16
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps16_2_nt_e
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   16
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps16_2_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   8
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps8_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"


#define test_null_invalid 1
#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   8
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps8_2_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"


#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   4
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps4_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   4
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps4_2_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   5
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps5_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"

#define test_null_invalid 1
#define test_pass_key(kp) (&(kp))
#define test_key_t        uint64_t const *
#define test_key_base_t   uint64_t
#define test_spare_bits   5
#define test_key_equals(rkey, tkey)                                            \
    (!memcmp(CAST(const uint8_t *, rkey), CAST(const uint8_t *, tkey),         \
             sizeof(test_key_base_t)))
#define test_key_eq(tbl_key, pk, ...)                                          \
    ({                                                                         \
        memcmp(CAST(const uint8_t *, tbl_key), CAST(const uint8_t *, pk),      \
               sizeof(uint64_t));                                              \
    })
#define test_hash(x)                                                           \
    ({                                                                         \
        uint64_t I_tmp_hv_;                                                    \
        __builtin_memcpy(&I_tmp_hv_, (const uint8_t *)(x), sizeof(uint64_t));  \
        xxhashT(I_tmp_hv_);                                                     \
    })
#define test_name   k8ps5_2_nt
#define HASHTABLE_H "rhi-hashtable.h"

#include "hashtable.test.h"


#define test_key_t                    uint32_t
#define test_spare_bits               10
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s10_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t                    uint32_t
#define test_spare_bits               10
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s10_nt_e
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t                    uint32_t
#define test_spare_bits               5
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s5_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t                    uint32_t
#define test_spare_bits               4
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s4_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t                    uint32_t
#define test_spare_bits               8
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s8_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t                    uint32_t
#define test_val_t                    uint32_t
#define test_spare_bits               8
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s8v4_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t                    uint32_t
#define test_val_t                    uint32_t
#define test_spare_bits               8
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s8v4_nt_e
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"


#define test_key_t      uint32_t
#define test_val_t      uint32_t
#define test_spare_bits 8
#define test_hash(x)    ((x)*31)
#define test_name       k4s8v4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint16_t
#define test_val_t      uint16_t
#define test_spare_bits 5
#define test_hash(x)    ((x)*31)
#define test_name       k2s5v2
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_key_t      uint16_t
#define test_val_t      uint16_t
#define test_spare_bits 5
#define test_hash(x)    ((x)*31)
#define test_name       k2s5v2e
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint16_t
#define test_val_t      uint32_t
#define test_spare_bits 5
#define test_hash(x)    ((x)*31)
#define test_name       k2s5v4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint16_t
#define test_val_t      uint32_t
#define test_spare_bits 4
#define test_hash(x)    ((x)*31)
#define test_name       k2s4v4
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"


#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_spare_bits               10
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s10_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_spare_bits               5
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s5_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_spare_bits               4
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s4_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_spare_bits               4
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s4_2_nt_e
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_spare_bits               8
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s8_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_val_t                    uint32_t
#define test_spare_bits               8
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s8v4_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_val_t                    uint32_t
#define test_spare_bits               4
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s4v4_2_nt
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t      uint32_t
#define test_val_t      uint32_t
#define test_spare_bits 8
#define test_hash(x)    ((x)*31)
#define test_name       k4s8v4_2
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t                    uint32_t
#define test_val_t                    uint32_t
#define test_spare_bits               4
#define test_hash(x)                  ((x)*31)
#define test_key_eq(tbl_key, pk, ...) ((tbl_key) != (pk))
#define test_name                     k4s4v4_2_nt_e
#define HASHTABLE_H                   "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_erase_copy
#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t      uint32_t
#define test_val_t      uint32_t
#define test_spare_bits 8
#define test_hash(x)    ((x)*31)
#define test_name       k4s8v4_2_e
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"


#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t      uint32_t
#define test_spare_bits 10
#define test_hash(x)    ((x)*31)
#define test_name       k4s10_2
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_null_invalid 1
#define test_skip_zero_key
#define test_key_t      uint32_t
#define test_spare_bits 4
#define test_hash(x)    ((x)*31)
#define test_name       k4s4_2
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint32_t
#define test_spare_bits 10
#define test_hash(x)    ((x)*31)
#define test_name       k4s10
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"

#define test_key_t      uint16_t
#define test_spare_bits 6
#define test_hash(x)    ((x)*31)
#define test_name       k2s10
#define HASHTABLE_H     "rhi-hashtable.h"
#include "hashtable.test.h"


#define RH_NAMES                                                               \
    k4i, k4i_nt, k8pe_nt, k8p_nt, k4_nt, k4v4_nt, k4v4e_nt, k16v8_nt, k8p,     \
        k8pe, k4v4e, k1v16e, k16e, k8pv8e, k8pv8pe, k1v1, k4, k4v4, k2, k1,    \
        k16v8, k8pv8, k8pv8p, k4p, k1v16, k1v4, k2v1
#define RHI_NAMES                                                              \
    k4s8v4_2_e, k4s4v4_2_nt_e, k8ps16_nt_e, k8ps16_2_nt_e, k4s10_nt_e,         \
        k4s8v4_nt_e, k2s5v2e, k4s4_2_nt_e, k8s10e, k4s10, k4s4, k8s10, k8s4,   \
        k2s10, k2s4, k8ps16_nt, k8ps16_2_nt, k8ps8_nt, k8ps8_2_nt, k8ps4_nt,   \
        k8ps4_2_nt, k8ps5_nt, k8ps5_2_nt, k4s10_nt, k4s5_nt, k4s4_nt, k4s8_nt, \
        k4s8v4_nt, k4s8v4, k2s5v2, k2s5v4, k2s4v4, k4s10_2_nt, k4s5_2_nt,      \
        k4s4_2_nt, k4s8_2_nt, k4s8v4_2_nt, k4s4v4_2_nt, k4s8v4_2, k4s10_2,     \
        k4s4_2

int32_t test_hashtable(void);

#define run_test(name) test_assert(CAT(name, _runall)() == 0);
int32_t
test_hashtable(void) {
    APPLY(run_test, ;, RHI_NAMES);
    APPLY(run_test, ;, RH_NAMES);
    return 0;
}
