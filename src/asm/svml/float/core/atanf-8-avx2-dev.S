/* Core .*/
/* Function atanf vectorized with AVX2.
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
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-avx2-rodata-offsets.h"
#define _sPC8	0
#define _sPC7	32
#define _sPC6	64
#define _sPC5	96
#define _sPC4	128
#define _sPC3	160
#define _sPC2	192
#define _sPC1	224
#define _sPC0	256


	.section .text.avx2, "ax", @progbits
ENTRY(atanf_8_avx2_dev)
	/* 1) If x>1,      then r=-1/x, PIO2=Pi/2
	   2) If -1<=x<=1, then r=x,    PIO2=0
	   3) If x<-1,     then r=-1/x, PIO2=-Pi/2.  */
	vmovups	COMMON_DATA(_AbsMask)(%rip), %ymm7
	vmovups	COMMON_DATA(_OneF)(%rip), %ymm2


	/* Use minps\maxps operations for argument reduction.  */
	vandps	%ymm0, %ymm7, %ymm3
	vpmaxud	%ymm3, %ymm2, %ymm5
	vpminud	%ymm3, %ymm2, %ymm4

	vdivps	%ymm5, %ymm4, %ymm4

	vpcmpgtd %ymm2, %ymm3, %ymm6
	vandnps	%ymm0, %ymm7, %ymm3
	vandnps	%ymm6, %ymm7, %ymm7
	vxorps	%ymm3, %ymm7, %ymm5
	vxorps	%ymm4, %ymm5, %ymm7
	/* Polynomial.  */
	vmulps	%ymm4, %ymm4, %ymm1
	vmovups	LOCAL_DATA(_sPC8)(%rip), %ymm0
	vmovups	LOCAL_DATA(_sPC7)(%rip), %ymm4
	vmulps	%ymm1, %ymm1, %ymm5
	vfmadd213ps LOCAL_DATA(_sPC6)(%rip), %ymm5, %ymm0
	vfmadd213ps LOCAL_DATA(_sPC5)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPC4)(%rip), %ymm5, %ymm0
	vfmadd213ps LOCAL_DATA(_sPC3)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPC2)(%rip), %ymm5, %ymm0
	vfmadd213ps LOCAL_DATA(_sPC1)(%rip), %ymm5, %ymm4
	vfmadd213ps %ymm4, %ymm1, %ymm0
	vfmadd213ps %ymm2, %ymm1, %ymm0
	vandps	COMMON_DATA(_TanSPI1_FMA)(%rip), %ymm6, %ymm1
	vxorps	%ymm3, %ymm1, %ymm1

	/* Reconstruction.  */
	vfmadd213ps %ymm1, %ymm7, %ymm0
	ret

END(atanf_8_avx2_dev)

	.section .rodata.avx2, "a"
	.align	32
LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _sPC8, 0x3B322CC0)
	DATA_VEC (LOCAL_DATA_NAME, _sPC7, 0xBC7F2631)
	DATA_VEC (LOCAL_DATA_NAME, _sPC6, 0x3D2BC384)
	DATA_VEC (LOCAL_DATA_NAME, _sPC5, 0xBD987629)
	DATA_VEC (LOCAL_DATA_NAME, _sPC4, 0x3DD96474)
	DATA_VEC (LOCAL_DATA_NAME, _sPC3, 0xBE1161F8)
	DATA_VEC (LOCAL_DATA_NAME, _sPC2, 0x3E4CB79F)
	DATA_VEC (LOCAL_DATA_NAME, _sPC1, 0xBEAAAA49)
	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
