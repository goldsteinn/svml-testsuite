#include "dev-common-evex512-rodata-offsets.h"	

	.section .rodata.evex512, "a"
	.align	64
	.globl	COMMON_DATA_NAME

COMMON_DATA_NAME:

	/* Used By: atanf, atanhf, log10f, log2f, logf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _OneF, 0x3f800000)
	/* Used By: tanf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _AbsMask, 0x7fffffff)
	/* Used By: logf, tanf.  */
	DATA_VEC(COMMON_DATA_NAME, _NotiOffExpoMask, 0xff800000)


	.type	COMMON_DATA_NAME, @object
	.size	COMMON_DATA_NAME, .-COMMON_DATA_NAME
