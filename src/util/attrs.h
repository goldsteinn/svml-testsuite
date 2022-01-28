#ifndef _SRC__UTIL__ATTRS_H_
#define _SRC__UTIL__ATTRS_H_

#define MALLOC_FUNC __attribute__((malloc))
#define CONST_FUNC  __attribute__((const))
#define PURE_FUNC   __attribute__((const))

/* NOLINT to suppress warning from clang not supporting noclone attribute. */
#define BENCH_FUNC                                                             \
    __attribute__((noclone /* NOLINT */, noinline, aligned(4096)))
#define EXIT_FUNC                                                              \
    __attribute__((noclone /* NOLINT */, noinline, cold, noreturn))

#endif
