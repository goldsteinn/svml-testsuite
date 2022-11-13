#ifndef SRC_UTIL_ERROR_UTIL_H_
#define SRC_UTIL_ERROR_UTIL_H_


#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "util/common.h"
#include "util/macro.h"
#include "util/verbosity.h"

#define err_assert(...)                                                        \
    CAT(I_err_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)
#define die_assert(...)                                                        \
    CAT(I_die_assert_, NOT_ONE_NARG(__VA_ARGS__))(__VA_ARGS__)


#define warn(msg, ...) fprint(stderr, "warning: " msg, ##__VA_ARGS__)
#define warn_once(msg, ...)                                                    \
    ({                                                                         \
        static bool I_tmp_warned_;                                             \
        if (I_tmp_warned_ == 0) {                                              \
            I_tmp_warned_ = 1;                                                 \
            warn(msg, ##__VA_ARGS__);                                          \
        }                                                                      \
    })

#define I_assert_macro_base(cond, todo, ...)                                   \
    ({                                                                         \
        __typeof__(cond) I_cond_eval_tmp_ = (cond);                            \
        if (UNLIKELY(!I_cond_eval_tmp_)) {                                     \
            todo(__VA_ARGS__);                                                 \
        }                                                                      \
        I_cond_eval_tmp_;                                                      \
    })

#define warn_once_assert(cond, msg, ...)                                       \
    I_assert_macro_base(cond, warn_once, msg, ##__VA_ARGS__)

#define msg_assert(cond, msg, ...)                                             \
    I_assert_macro_base(cond, I_msg_die, msg, ##__VA_ARGS__)

#define warn_assert(cond, ...) I_assert_macro_base(cond, warn, __VA_ARGS__)

#define I_err_assert_MANY(cond, msg, ...)                                      \
    I_assert_macro_base(cond, I_errdie, I_ERR_ARGS, V_TO_STR(cond), errno,     \
                        msg, ##__VA_ARGS__)

#define I_err_assert_ONE(cond)                                                 \
    I_assert_macro_base(cond, I_errdie, I_ERR_ARGS, V_TO_STR(cond), errno, NULL)

#define I_die_assert_MANY(cond, msg, ...)                                      \
    I_assert_macro_base(cond, I_die, I_ERR_ARGS, V_TO_STR(cond), msg,          \
                        ##__VA_ARGS__)

#define I_die_assert_ONE(cond)                                                 \
    I_assert_macro_base(cond, I_die, I_ERR_ARGS, V_TO_STR(cond), NULL)

#define die(msg, ...)    I_die(I_ERR_ARGS, NULL, msg, ##__VA_ARGS__);
#define errdie(msg, ...) I_errdie(I_ERR_ARGS, NULL, errno, msg, ##__VA_ARGS__);
#define msg_die(...)     I_msg_die(__VA_ARGS__)

// #define WITH_DBG_PRINT
#ifdef WITH_DBG_PRINT
# define dbg_assert(...) die_assert(__VA_ARGS__)
# define dbg_print(...)  fprintf(stderr, __VA_ARGS__)
# define PRINTFFL        fprintf(stderr, "%-20s:%-20s:%-4d\n", I_ERR_ARGS)
#else
# define dbg_print(...)
# define dbg_assert(...)
# define PRINTFFL
#endif

EXIT_FUNC NONNULL(1, 2) void I_va_errdie(char const * restrict file_name,
                                         char const * restrict func_name,
                                         uint32_t line_number,
                                         int32_t  error_number,
                                         char const * restrict msg,
                                         va_list ap);

EXIT_FUNC NONNULL(1, 2)
    FORMATF(6, 7) void I_errdie(char const * restrict file_name,
                                char const * restrict func_name,
                                uint32_t line_number,
                                char const * restrict expr_str,
                                int32_t error_number,
                                char const * restrict msg,
                                ...);

EXIT_FUNC NONNULL(1, 2)
    FORMATF(5, 6) void I_die(char const * restrict file_name,
                             char const * restrict func_name,
                             uint32_t line_number,
                             char const * restrict expr_str,
                             char const * restrict msg,
                             ...);

EXIT_FUNC FORMATF(1, 2) void I_msg_die(char const * restrict msg, ...);
#endif
