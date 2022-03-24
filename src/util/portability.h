#ifndef _SRC__UTIL__PORTABILITY_H_
#define _SRC__UTIL__PORTABILITY_H_


#if defined(__GNUC__) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
/* Keep noclone */
#elif defined __clang__
#define noclone
#else
#define noclone
#warning "Untested and likely unsupported compiler"
#endif

#ifdef __cplusplus
#define restrict
#define I_static_assert_base              static_assert
#define I_choose_const_expr(cond, e0, e1) ((cond) ? (e0) : (e1))
#else
#define I_static_assert_base              _Static_assert
#define I_choose_const_expr(cond, e0, e1) __builtin_choose_expr(cond, e0, e1)
#endif

#endif
