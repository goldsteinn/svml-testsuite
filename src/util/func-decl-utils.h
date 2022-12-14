#ifndef SRC_UTIL_FUNC_DECL_UTILS_H_
#define SRC_UTIL_FUNC_DECL_UTILS_H_

#include "util/func-decl-types.h"

void list_decls(decl_list_t const * decl_list);
void list_decls_filtered(decl_list_t const * decl_list,
                         char * restrict const * restrict decls_to_list,
                         uint64_t ndecls_to_list);

void run_decls(decl_list_t const * restrict decl_list,
               char * restrict const * restrict decls_to_run,
               uint64_t         ndecls_to_run,
               const run_decl_f run_decl_func);

#endif
