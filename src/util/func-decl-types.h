#ifndef _SRC__UTIL__FUNC_DECL_TYPES_H_
#define _SRC__UTIL__FUNC_DECL_TYPES_H_

#include <stdint.h>

typedef int32_t (*test_f)();
typedef int32_t (*bench_f)(uint32_t);


typedef struct func_decl {
    const char * name;
    union {
        const void * _func_ptr;

        const test_f  test_func;
        const bench_f bench_func;
    };
} func_decl_t;

typedef void (*run_decl_f)(const func_decl_t *);

typedef struct decl_list {
    const uint64_t    ndecls;
    const char *      decl_desc;
    const func_decl_t decls[];
} decl_list_t;

#endif
