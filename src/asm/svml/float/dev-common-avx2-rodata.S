#include "dev-common-avx2-rodata-offsets.h"

	.section .rodata.avx2, "a"
	.align	32
	.globl	COMMON_DATA_NAME

COMMON_DATA_NAME:

	/* Used By: atanf, atanhf, log, log10f, log2, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _OneF, 0x3f800000)
	/* Used By: atanhf, log, log10f, log2.  */
	DATA_VEC(COMMON_DATA_NAME, _NotiOffExpoMask, 0xff800000)
	/* Used By: atanhf, log, log10f, log2.  */
	DATA_VEC(COMMON_DATA_NAME, _IBrkValue, 0x3f2aaaab)
	/* Used By: atanf, atanhf, tanf, tanhf.  */
	DATA_VEC(COMMON_DATA_NAME, _AbsMask, 0x7fffffff)
	/* Used By: log, log10f, log2.  */
	DATA_VEC(COMMON_DATA_NAME, _ILoRange, 0x01000000)
	/* Used By: atanhf, log.  */
	DATA_VEC(COMMON_DATA_NAME, _Neg5F, 0xbf000000)
	/* Used By: atanhf, log.  */
	DATA_VEC(COMMON_DATA_NAME, _Ln2, 0x3f317218)
	/* Used By: atanhf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _Neg4096, 0xfffff000)
	/* Used By: atanf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _TanSPI1_FMA, 0x3fc90fdb)


	.type	COMMON_DATA_NAME, @object
	.size	COMMON_DATA_NAME, .-COMMON_DATA_NAME
