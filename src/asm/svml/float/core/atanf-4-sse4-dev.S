/* Core .*/
/* Function atanf vectorized with SSE4.
   Copyright (C) 2021-2022 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   https://www.gnu.org/licenses/.  */

/*
 * ALGORITHM DESCRIPTION:
 *
 *      For    0.0    <= x <=  7.0/16.0: atan(x) = atan(0.0) + atan(s), where s=(x-0.0)/(1.0+0.0*x)
 *      For  7.0/16.0 <= x <= 11.0/16.0: atan(x) = atan(0.5) + atan(s), where s=(x-0.5)/(1.0+0.5*x)
 *      For 11.0/16.0 <= x <= 19.0/16.0: atan(x) = atan(1.0) + atan(s), where s=(x-1.0)/(1.0+1.0*x)
 *      For 19.0/16.0 <= x <= 39.0/16.0: atan(x) = atan(1.5) + atan(s), where s=(x-1.5)/(1.0+1.5*x)
 *      For 39.0/16.0 <= x <=    inf   : atan(x) = atan(inf) + atan(s), where s=-1.0/x
 *      Where atan(s) ~= s+s^3*Poly11(s^2) on interval |s|<7.0/0.16.
 *
 */

/* Offsets for data table __svml_satan_data_internal
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_satan_data_internal
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"
#define _SignMask	0
#define _sPIO2	16
#define _sPC7	32
#define _sPC5	48
#define _sPC3	64
#define _sPC1	80
#define _sPC8	96
#define _sPC6	112
#define _sPC4	128
#define _sPC2	144
#define _sPC0	160

#define xmmA	xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(atanf_4_sse4_dev)
	/* 1) If x>1,      then r=-1/x, PIO2=Pi/2
	   2) If -1<=x<=1, then r=x,    PIO2=0
	   3) If x<-1,     then r=-1/x, PIO2=-Pi/2.  */
	movups	COMMON_DATA(_OneF)(%rip), %xmm1
	/* use minud\maxud operations for argument reduction.  */
	movups	LOCAL_DATA(_SignMask)(%rip), %xmm5
	movaps	%xmm5, %xmm6
	andnps	%xmm0, %xmm5
	andps	%xmm6, %xmm0
	movaps	%xmm5, %xmm7
    
	movaps	%xmmA, %xmm4
	pminud	%xmm5, %xmmA
	pmaxud	%xmm4, %xmm7
    pcmpgtd	%xmmA, %xmm5
	divps	%xmm7, %xmmA
    
	andps	%xmm5, %xmm6
	pxor	%xmm0, %xmm6
	andps	LOCAL_DATA(_sPIO2)(%rip), %xmm5
	pxor	%xmm0, %xmm5
	pxor	%xmmA, %xmm6
	/* Polynomial.  */
	mulps	%xmmA, %xmmA
	movaps	%xmmA, %xmm0
	mulps	%xmmA, %xmmA
	movups	LOCAL_DATA(_sPC7)(%rip), %xmm2
	mulps	%xmmA, %xmm2
	addps	LOCAL_DATA(_sPC5)(%rip), %xmm2
	mulps	%xmmA, %xmm2
	addps	LOCAL_DATA(_sPC3)(%rip), %xmm2
	mulps	%xmmA, %xmm2
	addps	LOCAL_DATA(_sPC1)(%rip), %xmm2
	movups	LOCAL_DATA(_sPC8)(%rip), %xmm3
	mulps	%xmmA, %xmm3
	addps	LOCAL_DATA(_sPC6)(%rip), %xmm3
	mulps	%xmmA, %xmm3
	addps	LOCAL_DATA(_sPC4)(%rip), %xmm3
	mulps	%xmmA, %xmm3
	addps	LOCAL_DATA(_sPC2)(%rip), %xmm3
	mulps	%xmm0, %xmm3
	addps	%xmm3, %xmm2
	mulps	%xmm2, %xmm0
	addps	%xmm4, %xmm0
	/* Reconstruction.  */
	mulps	%xmm6, %xmm0
	addps	%xmm5, %xmm0
	ret
END(atanf_4_sse4_dev)

	.section .rodata.sse4, "a"
	.align	16
LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _SignMask, 0x80000000)
	DATA_VEC (LOCAL_DATA_NAME, _sPIO2, 0x3fc90fdb)
	DATA_VEC (LOCAL_DATA_NAME, _sPC7, 0xBC7F2631)
	DATA_VEC (LOCAL_DATA_NAME, _sPC5, 0xBD987629)
	DATA_VEC (LOCAL_DATA_NAME, _sPC3, 0xBE1161F8)
	DATA_VEC (LOCAL_DATA_NAME, _sPC1, 0xBEAAAA49)
	DATA_VEC (LOCAL_DATA_NAME, _sPC8, 0x3B322CC0)
	DATA_VEC (LOCAL_DATA_NAME, _sPC6, 0x3D2BC384)
	DATA_VEC (LOCAL_DATA_NAME, _sPC4, 0x3DD96474)
	DATA_VEC (LOCAL_DATA_NAME, _sPC2, 0x3E4CB79F)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
