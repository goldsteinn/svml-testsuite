/* Core .*/
/* Function log10f vectorized with AVX-512.
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
 *    Get short reciprocal approximation Rcp ~ 1/mantissa(x)
 *    R = Rcp*x - 1.0
 *    log10(x) = k*log10(2.0) - log10(Rcp) + poly_approximation(R)
 *       log10(Rcp) is tabulated
 *
 *
 */

/* Offsets for data table __svml_slog10_data_internal
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_slog10_data_internal
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-evex512-rodata-offsets.h"
#define _Coeff_4	0
#define _Coeff_3	64
#define _Coeff_2	128
#define _Coeff_1	192
#define _L2	256


#define zmmA	zmm1
	.section .text.evex512, "ax", @progbits
ENTRY(log10f_16_avx512_dev)
	vgetmantps $11, {sae}, %zmm0, %zmm3
	vmovups	COMMON_DATA(_OneF)(%rip), %zmmA
	vgetexpps {sae}, %zmm0, %zmm5
	vpsrld	$19, %zmm3, %zmm7
	vgetexpps {sae}, %zmm3, %zmm6
	vsubps	{rn-sae}, %zmm1, %zmm3, %zmm11
	vpermps	LOCAL_DATA(_Coeff_4)(%rip), %zmm7, %zmmA
	vpermps	LOCAL_DATA(_Coeff_3)(%rip), %zmm7, %zmm2
	vsubps	{rn-sae}, %zmm6, %zmm5, %zmm9
	vpermps	LOCAL_DATA(_Coeff_2)(%rip), %zmm7, %zmm4
	vpermps	LOCAL_DATA(_Coeff_1)(%rip), %zmm7, %zmm8

	/* x<=0?  */
	vfpclassps $0x5e, %zmm0, %k0
	vfmadd213ps {rn-sae}, %zmm2, %zmm11, %zmmA
	vmulps	LOCAL_DATA(_L2)(%rip), %zmm9, %zmm12
	vfmadd213ps {rn-sae}, %zmm4, %zmm11, %zmmA
	vfmadd213ps {rn-sae}, %zmm8, %zmm11, %zmmA
	vfmadd213ps {rn-sae}, %zmm12, %zmm11, %zmmA
	kmovd	%k0, %edx
	testl	%edx, %edx

	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)

	/* Restore registers * and exit the function.  */
	vmovaps	%zmm1, %zmm0
	ret


	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a tanf call. Optimize for code size
	   moreso than speed here.  */
L(SPECIAL_VALUES_BRANCH):

	/* Use r13 to save/restore the stack. This allows us to use rbp
	   as callee save register saving code size.  */
	pushq	%r13
	cfi_def_cfa (rsp, 16)
	/* Need to callee save registers to preserve state across tanf
	   calls.  */
	pushq	%rbx
	cfi_def_cfa (rsp, 24)
	pushq	%rbp
	cfi_def_cfa (rsp, 32)
	movq	%rsp, %r13
	cfi_def_cfa (r13, 32)

	/* Align stack and make room for 2x zmm vectors.  */
	andq	$-64, %rsp
	addq	$-128, %rsp

	/* Save original input.  */
	vmovaps	%zmm0, 64(%rsp)
	/* Save all already computed inputs.  */
	vmovaps	%zmm1, (%rsp)

	vzeroupper

	/* edx has 1s where there was a special value that needs to be
	   handled by a tanf call.  */
	movl	%edx, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to tanf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 56] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop. Realigning
	   also costs more code size.  */
	xorl	%ebp, %ebp
	tzcntl	%ebx, %ebp

	/* Scalar math fucntion call to process special input.  */
	movss	64(%rsp, %rbp, 4), %xmm0
	call	log10f@PLT
	INC_FALLBACK0
	/* No good way to avoid the store-forwarding fault this will
	   cause on return. `lfence` avoids the SF fault but at greater
	   cost as it serialized stack/callee save restoration.  */
	movss	%xmm0, (%rsp, %rbp, 4)

	blsrl	%ebx, %ebx
	jnz	L(SPECIAL_VALUES_LOOP)


	/* All results have been written to 64(%rsp).  */
	vmovaps	(%rsp), %zmm0
	/* Restore rsp.  */
	movq	%r13, %rsp
	cfi_def_cfa (rsp, 32)
	/* Restore callee save registers.  */
	popq	%rbp
	cfi_def_cfa (rsp, 24)
	popq	%rbx
	cfi_def_cfa (rsp, 16)
	popq	%r13
	ret
END(log10f_16_avx512_dev)

	.section .rodata.evex512, "a"
	.align	64

LOCAL_DATA_NAME:
	float_block (LOCAL_DATA_NAME, _Coeff_4,
	0xbdc9ae9b, 0xbda6fcf4,
	0xbd8bac76, 0xbd6bca30,
	0xbd48a99b, 0xbd2c0a9f,
	0xbd1480db, 0xbd00faf2,
	0xbe823aa9, 0xbe656348,
	0xbe4afbb9, 0xbe346895,
	0xbe20ffff, 0xbe103a0b,
	0xbe01a91c, 0xbde9e84e)

	float_block (LOCAL_DATA_NAME, _Coeff_3,
	0x3e13d888, 0x3e10a87c,
	0x3e0b95c3, 0x3e057f0b,
	0x3dfde038, 0x3df080d9,
	0x3de34c1e, 0x3dd68333,
	0x3dac6e8e, 0x3dd54a51,
	0x3df30f40, 0x3e04235d,
	0x3e0b7033, 0x3e102c90,
	0x3e12ebad, 0x3e141ff8)

	float_block (LOCAL_DATA_NAME, _Coeff_2,
	0xbe5e5a9b, 0xbe5e2677,
	0xbe5d83f5, 0xbe5c6016,
	0xbe5abd0b, 0xbe58a6fd,
	0xbe562e02, 0xbe5362f8,
	0xbe68e27c, 0xbe646747,
	0xbe619a73, 0xbe5ff05a,
	0xbe5f0570, 0xbe5e92d0,
	0xbe5e662b, 0xbe5e5c08)

	float_block (LOCAL_DATA_NAME, _Coeff_1,
	0x3ede5bd8, 0x3ede5b45,
	0x3ede57d8, 0x3ede4eb1,
	0x3ede3d37, 0x3ede2166,
	0x3eddf9d9, 0x3eddc5bb,
	0x3ede08ed, 0x3ede32e7,
	0x3ede4967, 0x3ede5490,
	0x3ede597f, 0x3ede5b50,
	0x3ede5bca, 0x3ede5bd9)

	DATA_VEC (LOCAL_DATA_NAME, _L2, 0x3e9a209b)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
