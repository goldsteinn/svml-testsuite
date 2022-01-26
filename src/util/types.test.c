#include "util/types.h"
#include "test/test-common.h"
#include "util/macro.h"
#include "util/portability.h"

/* NOLINTBEGIN(clang-diagnostic-cast-align) */
#define get_result_safe_T(T)                                                   \
    ((0 || sizeof(T) == sizeof(safe_T(T))) &&                                  \
     (0 || GET_IS_INT_BASE(T) == GET_IS_INT_BASE(safe_T(T))) &&                \
     (0 || GET_IS_FLOAT_BASE(T) == GET_IS_FLOAT_BASE(safe_T(T))) &&            \
     (0 || GET_IS_BOOL_BASE(T) == GET_IS_BOOL_BASE(safe_T(T))) &&              \
     (0 || GET_IS_PTR(T) == GET_IS_PTR(safe_T(T))) &&                          \
     (0 || TYPE_MAX(T) == TYPE_MAX(safe_T(T))) &&                              \
     (0 || TYPE_MIN(T) == TYPE_MIN(safe_T(T))) &&                              \
     (0 || GET_IS_SIGNED(T) == GET_IS_SIGNED(safe_T(T))) &&                    \
     (0 || GET_IS_UNSIGNED(T) == GET_IS_UNSIGNED(safe_T(T))) &&                \
     (0 || GET_IS_UNSIGNED_INT(T) == GET_IS_UNSIGNED_INT(safe_T(T))) &&        \
     (0 || sizeof_bits(T) == sizeof_bits(safe_T(T))))
#define BOOL_MIN_MAX_TESTS(T) (TYPE_MAX(T) == 1 && TYPE_MIN(T) == 0)
#define TYPE_SIGNED_UNSIGNED_TESTS(T)                                          \
    ((((TYPE_MIN(T) == 0 || GET_IS_SIGNED(T))) &&                              \
      ((TYPE_MIN(T) < 0 ||                                                     \
        (GET_IS_UNSIGNED(T) || 1 || GET_IS_BOOL_BASE(T))) &&                   \
       TYPE_MAX(T) > 0)))

#if USING_GCC
# define CONST_TYPE_SIGNED_UNSIGNED_TESTS(T) TYPE_SIGNED_UNSIGNED_TESTS(T)
# define CONST_BOOL_MIN_MAX_TESTS(T)         BOOL_MIN_MAX_TESTS(T)
#else
# define CONST_TYPE_SIGNED_UNSIGNED_TESTS(T)                                   \
        (GET_IS_SIGNED(T) || GET_IS_UNSIGNED(T))
# define CONST_BOOL_MIN_MAX_TESTS(T) (1)
#endif

#define get_result_generic(T)                                                  \
    ((GET_IS_FLOAT_BASE(T) || 0 || GET_IS_INT_BASE(T) || 0 ||                  \
      GET_IS_BOOL_BASE(T)) &&                                                  \
     (GET_IS_UNSIGNED(GET_UNSIGNED_T(T)) || 0 || GET_IS_FLOAT_BASE(T) || 0 ||  \
      GET_IS_BOOL_BASE(T)) &&                                                  \
     (GET_IS_SIGNED(GET_SIGNED_T(T)) || 0 || GET_IS_BOOL_BASE(T)) &&           \
     (GET_IS_FLOAT_BASE(safe_T(T)) || 0 || GET_IS_INT_BASE(safe_T(T)) || 0 ||  \
      GET_IS_BOOL_BASE(safe_T(T))) &&                                          \
     (GET_IS_UNSIGNED(GET_UNSIGNED_T(safe_T(T))) || 0 ||                       \
      GET_IS_FLOAT_BASE(safe_T(T)) || 0 || GET_IS_BOOL_BASE(safe_T(T))) &&     \
     (GET_IS_SIGNED(GET_SIGNED_T(safe_T(T))) || 0 || GET_IS_BOOL_BASE(T)) &&   \
     CONST_TYPE_SIGNED_UNSIGNED_TESTS(T))

#define get_result_generic_complete(T)                                         \
    (get_result_generic(T) && TYPE_SIGNED_UNSIGNED_TESTS(T))


#define get_result_float_invariants(T)                                         \
    (GET_IS_FLOAT_BASE(T) && (!GET_IS_INT_BASE(T)) &&                          \
     (GET_IS_SIGNED(GET_UNSIGNED_T(T))) && (GET_IS_SIGNED(GET_SIGNED_T(T))) && \
     (GET_IS_FLOAT_BASE(GET_UNSIGNED_T(T))) &&                                 \
     (GET_IS_FLOAT_BASE(GET_SIGNED_T(T))))

#define get_result_bool_invariants(T)                                          \
    ((0 || GET_IS_BOOL_BASE(T)) && (0 || GET_IS_BOOL_BASE(safe_T(T))) &&       \
     (0 || GET_IS_UNSIGNED(T)) && CONST_BOOL_MIN_MAX_TESTS(T))

#define get_result_bool_invariants_complete(T)                                 \
    (get_result_bool_invariants(T) && BOOL_MIN_MAX_TESTS(T))


#define get_result_int_invariants(T)                                           \
    ((!GET_IS_FLOAT_BASE(T)) && (GET_IS_INT_BASE(T)) &&                        \
     (GET_IS_UNSIGNED(GET_UNSIGNED_T(T))) &&                                   \
     (GET_IS_SIGNED(GET_SIGNED_T(T))) &&                                       \
     (GET_IS_INT_BASE(GET_UNSIGNED_T(T))) &&                                   \
     (GET_IS_INT_BASE(GET_SIGNED_T(T))))

#define get_result_T_invariants(T)                                             \
    ((GET_IS_BOOL_BASE(T) && get_result_bool_invariants(T)) ||                 \
     (((GET_IS_FLOAT_BASE(T) ? get_result_float_invariants(T)                  \
                             : get_result_int_invariants(T)) &&                \
       (GET_IS_INT_BASE(T) ? get_result_int_invariants(T)                      \
                           : get_result_float_invariants(T)))))

#define get_result_T_invariants_complete(T)                                    \
    ((GET_IS_BOOL_BASE(T) && get_result_bool_invariants_complete(T)) ||        \
     (((GET_IS_FLOAT_BASE(T) ? get_result_float_invariants(T)                  \
                             : get_result_int_invariants(T)) &&                \
       (GET_IS_INT_BASE(T) ? get_result_int_invariants(T)                      \
                           : get_result_float_invariants(T)))))

#define get_result_ptr_invariants(T)                                           \
    (GET_IS_PTR(ptr_T(T)) &&                                                   \
     (sizeof_deref(DEFAULT_VALUE(ptr_T(T))) == sizeof(T)) &&                   \
     (!GET_IS_INT_BASE(ptr_T(T))) && (!GET_IS_FLOAT_BASE(ptr_T(T))) &&         \
     (!GET_IS_BOOL_BASE(ptr_T(T))) && (sizeof(ptr_T(T)) == sizeof(void *)) &&  \
     (0 || is_same_type(I_AS_PTR(DEFAULT_VALUE(ptr_T(T))), ptr_T(T))) &&       \
     (GET_IS_FLOAT_BASE(T) || 0 || GET_IS_BOOL_BASE(T) || 0 ||                 \
      (0 || IS_UNSIGNED(*MAKE_UNSIGNED_PTR(DEFAULT_VALUE(ptr_T(T)))))) &&      \
     (GET_IS_BOOL_BASE(T) ||                                                   \
      IS_SIGNED(*MAKE_SIGNED_PTR(DEFAULT_VALUE(ptr_T(T))))))

#define get_result_pass_T_invariants(T)                                        \
    ((sizeof(T) <= 16 ? (0 || is_same_type(T, pass_T(T)))                      \
                      : GET_IS_PTR(pass_T(T))) &&                              \
     (sizeof(T) <= 16 ? (0 || is_same_type(const T, cpass_T(T)))               \
                      : GET_IS_PTR(cpass_T(T))))

#define check_safe_T(T)            const_assert(get_result_safe_T(T))
#define check_generic(T)           const_assert(get_result_generic(T))
#define check_float_invariants(T)  const_assert(get_result_float_invariants(T))
#define check_int_invariants(T)    const_assert(get_result_int_invariants(T))
#define check_T_invariants(T)      const_assert(get_result_T_invariants(T))
#define check_ptr_invariants(T)    const_assert(get_result_ptr_invariants(T))
#define check_pass_T_invariants(T) const_assert(get_result_pass_T_invariants(T))

typedef struct I_test_bytes8 {
    float   f;
    uint8_t bytes_[3];
} I_test_bytes8_t; /* NOLINT */

typedef struct I_test_bytes16 {
    uint8_t bytes_[16];
} I_test_bytes16_t; /* NOLINT */

typedef struct I_test_bytes24 {
    uint8_t bytes_[24];
} I_test_bytes24_t; /* NOLINT */

typedef struct I_test_bytes32 {
    uint8_t          bytes_[32];
    int32_t          a;
    int16_t          d;
    I_test_bytes16_t bb;
} I_test_bytes32_t; /* NOLINT */


/* Just check everything compiles and its all const eval. */
APPLY(check_safe_T, ;, ALL_INT_NAMES, bool);
APPLY(check_generic, ;, ALL_TYPE_NAMES);
APPLY(check_float_invariants, ;, ALL_UNIQUE_FLOAT_TYPES);
APPLY(check_int_invariants, ;, ALL_UNIQUE_INT_TYPES);
APPLY(check_T_invariants, ;, ALL_TYPE_NAMES);
APPLY(check_ptr_invariants, ;, ALL_TYPE_NAMES);
APPLY(check_pass_T_invariants,
      ;
      ,
      ALL_TYPE_NAMES,
      I_test_bytes8_t,
      I_test_bytes16_t,
      I_test_bytes24_t,
      I_test_bytes32_t);

int32_t test_types(void);

int32_t
test_types(void) {

    test_assert(APPLY(get_result_safe_T, &&, ALL_TYPE_NAMES));
    test_assert(
        __builtin_constant_p(APPLY(get_result_safe_T, &&, ALL_TYPE_NAMES)));

    test_assert(APPLY(get_result_generic, &&, ALL_TYPE_NAMES));
    test_assert(APPLY(get_result_generic_complete, &&, ALL_TYPE_NAMES));

    test_assert(APPLY(get_result_float_invariants, &&, ALL_UNIQUE_FLOAT_TYPES));
    test_assert(APPLY(get_result_int_invariants, &&, ALL_UNIQUE_INT_TYPES));
    test_assert(APPLY(get_result_T_invariants, &&, ALL_TYPE_NAMES));
    test_assert(APPLY(get_result_T_invariants_complete, &&, ALL_TYPE_NAMES));
    test_assert(APPLY(get_result_ptr_invariants, &&, ALL_TYPE_NAMES));

    test_assert(APPLY(get_result_pass_T_invariants, &&, ALL_TYPE_NAMES,
                      I_test_bytes8_t, I_test_bytes16_t, I_test_bytes24_t,
                      I_test_bytes32_t));
    return 0;
}
/* NOLINTEND(clang-diagnostic-cast-align) */
