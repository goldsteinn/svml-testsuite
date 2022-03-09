#ifndef _SRC__UTIL__ATTRS_H_
#define _SRC__UTIL__ATTRS_H_

#include "util/portability.h"
#include "util/macro.h"


#define MALLOC_FUNC __attribute__((malloc))
#define CONST_FUNC  __attribute__((const))
#define PURE_FUNC   __attribute__((pure))

#define BENCH_FUNC __attribute__((noclone, noinline, aligned(4096)))
#define EXIT_FUNC  __attribute__((noclone, noinline, cold, noreturn))

#define NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#define FORMATF(...) __attribute__((format(printf, ##__VA_ARGS__)))
#endif
