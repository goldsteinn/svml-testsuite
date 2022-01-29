#ifndef _SRC__UTIL__FUNC_DECL_GENERATOR_H_
#define _SRC__UTIL__FUNC_DECL_GENERATOR_H_

#include "util/func-decl-types.h"

#define make_decl(name)                                                        \
    {                                                                          \
        V_TO_STR(name), {                                                      \
            &name                                                              \
        }                                                                      \
    }

#define make_decls(decl_name, fwd_decl, ...)                                   \
    APPLY(fwd_decl, ;, __VA_ARGS__);                                           \
    const decl_list_t decl_name = { PP_NARG(__VA_ARGS__),                      \
                                    V_TO_STR(decl_name),                       \
                                    { APPLY(make_decl, COMMA,                  \
                                            __VA_ARGS__) } };


#endif
