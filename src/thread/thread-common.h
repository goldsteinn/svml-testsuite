#ifndef _SRC__THREAD__THREAD_COMMON_H_
#define _SRC__THREAD__THREAD_COMMON_H_

#include "util/types.h"

#define WRITE_ONCE(var, val) (*((volatile get_type(val) *)(&(var))) = (val))
#define READ_ONCE(var)       (*((volatile get_type(var) *)(&(var))))
#define CASTV(var)           CAST(volatile get_type(var), var)


#endif
