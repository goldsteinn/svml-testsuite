#include "dev-common-sse4-rodata-offsets.h"

	.section .rodata.sse4, "a"
	.align	16
	.globl	COMMON_DATA_NAME

COMMON_DATA_NAME:

	/* Used By: atanf, atanhf, log10f, log2f, logf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _OneF, 0x3f800000)
	/* Used By: atanhf, log10f, log2f, logf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _NotiOffExpoMask, 0xff800000)
	/* Used By: atanhf, log10f, log2f, logf.  */
	DATA_VEC(COMMON_DATA_NAME, _IBrkValue, 0x3f2aaaab)
	/* Used By: log10f, log2f, logf.  */
	DATA_VEC(COMMON_DATA_NAME, _ILoRange, 0x01000000)
	/* Used By: atanhf, logf.  */
	DATA_VEC(COMMON_DATA_NAME, _Neg5F, 0xbf000000)
	/* Used By: atanhf, logf.  */
	DATA_VEC(COMMON_DATA_NAME, _Ln2, 0x3f317218)
	/* Used By: atanhf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _AbsMask, 0x7fffffff)
	/* Used By: atanhf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _Neg4096, 0xfffff000)

	.type	COMMON_DATA_NAME, @object
	.size	COMMON_DATA_NAME, .-COMMON_DATA_NAME
