#ifndef SRC_UTIL_FUNC_DECL_TYPES_H_
#define SRC_UTIL_FUNC_DECL_TYPES_H_

#include "util/common.h"

typedef int32_t (*test_f)(void);
typedef void * (*bench_f)(void *);


typedef struct func_decl {
    const char * name;
    union {
        const void *  data;
        const test_f  test_func;
        const bench_f bench_func;
    };
} func_decl_t;

typedef void (*run_decl_f)(const func_decl_t *);

typedef struct decl_list {
    const uint64_t ndecls;
    const char *   decl_desc;
#if (defined __cplusplus) && (defined __clang__)
    const func_decl_t decls[30];
#else
    const func_decl_t decls[];
#endif
} decl_list_t;

#endif
