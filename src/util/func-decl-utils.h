#ifndef _SRC__UTIL__FUNC_DECL_UTILS_H_
#define _SRC__UTIL__FUNC_DECL_UTILS_H_

#include "util/func-decl-types.h"

void list_decls(const decl_list_t * decl_list);
void run_decls(const decl_list_t * decl_list,
               char * const *       decl_names,
               uint64_t            ndecls_to_run,
               const run_decl_f    run_decl_func);

#endif
