#ifndef SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_AVX2_RODATA_OFFSETS_H_
#define SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_AVX2_RODATA_OFFSETS_H_

#include "dev-common-data-macros.h.S"

#define COMMON_DATA_NAME dev_common_data_avx2


#define _OneF            0
#define _NotiOffExpoMask 32
#define _IBrkValue       64
#define _AbsMask         96
#define _ILoRange        128
#define _Neg5F           160
#define _Ln2             192
#define _Neg4096         224
#define _TanSPI1_FMA     256

#define COMMON_DATA(offset) ((offset) + (COMMON_DATA_NAME))
#define DATA_VEC(data_name, offset, value)                                     \
    float_vector32(data_name, offset, value)
#define DATA_BLOCK(data_name, offset, ...)                                     \
    float_block(data_name, offset, __VA_ARGS__)

#ifdef LOCAL_DATA_NAME
# define LOCAL_DATA(offset) ((offset) + (LOCAL_DATA_NAME))
#endif

#endif
