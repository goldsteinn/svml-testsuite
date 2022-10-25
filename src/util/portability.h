#ifndef _SRC__UTIL__PORTABILITY_H_
#define _SRC__UTIL__PORTABILITY_H_

/* TODO: Do all this with config. */

// clang-format off
#if defined(__STDC__)
# if defined(__STDC_VERSION__)
#  if (__STDC_VERSION__ >= 201710L)
#   define I_STDC_VERSION 2017
#  elif (__STDC_VERSION__ >= 201112L)
#   define I_STDC_VERSION 2011
#  elif (__STDC_VERSION__ >= 199901L)
#   define I_STDC_VERSION 1999
#  elif (__STDC_VERSION__ >= 199409L)
#   define I_STDC_VERSION 1994
#  else
#  define I_STDC_VERSION 1990
#  endif
# else
# define I_STDC_VERSION 1989
# endif
#else
# error "NO __STDC__"
#endif
#ifndef I_STDC_VERSION
# define I_STDC_VERSION 0
#endif

#if __cplusplus
# if (__cplusplus >= 202002L)
#  define I_STDCPP_VERSION 2020
# elif (__cplusplus >= 201703L)
#  define I_STDCPP_VERSION 2017
# elif (__cplusplus >= 201402L)
#  define I_STDCPP_VERSION 2014
# elif (__cplusplus >= 201103L)
#  define I_STDCPP_VERSION 2011
# else
# define I_STDCPP_VERSION 1997
# endif
#endif
#ifndef I_STDCPP_VERSION
# define I_STDCPP_VERSION 0
#endif

// clang-format on

#ifdef __has_builtin
#define I_has_builtin(...) __has_builtin(__VA_ARGS__)
#else
#define I_has_builtin(...) 0
#endif

#ifdef __has_attribute
#define I_has_attr(...) __has_attribute(__VA_ARGS__)
#else
#define I_has_attr(...) 0
#endif

#ifdef __has_include
#define I_has_include(...) __has_include(__VA_ARGS__)
#else
#define I_has_include(...) 0
#endif


#if defined(_GNU_SOURCE) && defined(__GLIBC_PREREQ)
#define I_glibc_version_ge(major, minor) __GLIBC_PREREQ(major, minor)
#else
#define I_glibc_version_ge(major, minor) 0
#endif


/* Fall through check.  */
#if I_has_attr(fallthrough)
#define fall_through __attribute__((fallthrough))
#elif I_STDCPP_VERSION >= 2017
#define fall_through [[fallthrough]];
#else
#define fall_through /* fall through.  */
#endif

/* Restrict check.  */
#if I_STDC_VERSION >= 1999
/* Nothing.  */
#define I_restrict restrict
#elif defined(__GNUC__)
#if ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define restrict __restrict
#endif
#else
#ifndef restrict
#define restrict
#endif
#endif
#ifndef __cplusplus
#define I_arr_restrict restrict
#else
#define I_arr_restrict
#endif

/* Inline check.  */
#if I_STDC_VERSION >= 1999
/* Nothing. */
#elif defined(__GNUC__)
#if ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define inline __inline
#endif
#else
#ifndef inline
#define inline
#endif
#endif

#ifdef __COUNTER__
#define I_COUNTER __COUNTER__
#else
#define I_COUNTER 0
#endif

#if I_STDCPP_VERSION >= 2011
#define I_static_assert(...) static_assert(__VA_ARGS__)
#elif I_STDC_VERSION >= 2011
#define I_static_assert(...) _Static_assert(__VA_ARGS__)
#else
#define I_arr_list_static_assert(cond, msg)                                    \
    typedef struct {                                                           \
        int I_arr_list_static_assertion_failed : !!(cond);                     \
    } I_arr_list_CAT(I_arr_list_static_assertion_failed_, I_arr_list_COUNTER);
#endif

#ifdef __cplusplus
#define extern_C_start() extern "C" {
#define extern_C_end()   }

#else
#define extern_C_start()
#define extern_C_end()

#define constexpr
#endif

#if I_STDC_VERSION >= 2011
#define Generic(...) _Generic(__VA_ARGS__)
#elif I_STDCPP_VERSION < 2011
#error "Older standard unsupported"
#endif

#endif
