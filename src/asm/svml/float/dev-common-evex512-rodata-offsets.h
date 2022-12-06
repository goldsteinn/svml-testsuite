#ifndef SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_AVX2_RODATA_OFFSETS_H_
#define SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_AVX2_RODATA_OFFSETS_H_

#include "dev-common-data-macros.h.S"

#define COMMON_DATA_NAME dev_common_data_evex512

#define _OneF            0
#define _NotiOffExpoMask 64
#define _Neg5F           128
#define _AbsMask         192
#define _SignMask        256


#define COMMON_DATA(offset) ((offset) + (COMMON_DATA_NAME))
#define DATA_VEC(data_name, offset, value)                                     \
    float_vector64(data_name, offset, value)

#ifdef LOCAL_DATA_NAME
# define LOCAL_DATA(offset) ((offset) + (LOCAL_DATA_NAME))
#endif

#ifdef LOCAL_DATA_NAME_UNALIGNED
# define LOCAL_DATA_UNALIGNED(offset) ((offset) + (LOCAL_DATA_NAME_UNALIGNED))
#endif

#endif