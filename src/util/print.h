#ifndef SRC_D_UTIL_D_PRINT_H_
#define SRC_D_UTIL_D_PRINT_H_

#include <stdio.h>

/* To silence cert-err33-c warning for fprintf.  */
#define fprintf_stderr(...) (void)(fprintf(stderr, __VA_ARGS__));
#define fprintf_stdout(...) (void)(fprintf(stdout, __VA_ARGS__));

#define fflush_stderr() (void)(fflush(stdout));
#define fflush_stdout() (void)(fflush(stdout));

#endif
