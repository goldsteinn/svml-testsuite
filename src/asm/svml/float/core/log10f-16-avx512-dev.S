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

/* Offsets for data table __svml_slog10_data_internal_avx512
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_slog10_data_internal_avx512
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-evex512-rodata-offsets.h"
#define One	0
#define coeff4	64
#define coeff3	128
#define coeff2	192
#define coeff1	256
#define L2	320


#define zmmA	zmm1
	.section .text.evex512, "ax", @progbits
ENTRY(log10f_16_avx512_dev)
	vgetmantps $11, {sae}, %zmm0, %zmm3
	vmovups	LOCAL_DATA(_OneF)(%rip), %zmmA
	vgetexpps {sae}, %zmm0, %zmm5
	vpsrld	$19, %zmm3, %zmm7
	vgetexpps {sae}, %zmm3, %zmm6
	vsubps	{rn-sae}, %zmm1, %zmm3, %zmm11
	vpermps	LOCAL_DATA(coeff4)(%rip), %zmm7, %zmmA
	vpermps	LOCAL_DATA(coeff3)(%rip), %zmm7, %zmm2
	vsubps	{rn-sae}, %zmm6, %zmm5, %zmm9
	vpermps	LOCAL_DATA(coeff2)(%rip), %zmm7, %zmm4
	vpermps	LOCAL_DATA(coeff1)(%rip), %zmm7, %zmm8

	/* x<=0?  */
	vfpclassps $0x5e, %zmm0, %k0
	vfmadd213ps {rn-sae}, %zmm2, %zmm11, %zmmA
	vmulps	LOCAL_DATA(L2)(%rip), %zmm9, %zmm12
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



	/* Branch to process * special inputs.  */
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

	/* Save origional input.  */
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

__svml_slog10_data_internal_avx512:
	/* One.  */
	.long	0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000
	// c4
	.align	64
	.long	0xbdc9ae9b, 0xbda6fcf4
	.long	0xbd8bac76, 0xbd6bca30
	.long	0xbd48a99b, 0xbd2c0a9f
	.long	0xbd1480db, 0xbd00faf2
	.long	0xbe823aa9, 0xbe656348
	.long	0xbe4afbb9, 0xbe346895
	.long	0xbe20ffff, 0xbe103a0b
	.long	0xbe01a91c, 0xbde9e84e
	// c3
	.align	64
	.long	0x3e13d888, 0x3e10a87c
	.long	0x3e0b95c3, 0x3e057f0b
	.long	0x3dfde038, 0x3df080d9
	.long	0x3de34c1e, 0x3dd68333
	.long	0x3dac6e8e, 0x3dd54a51
	.long	0x3df30f40, 0x3e04235d
	.long	0x3e0b7033, 0x3e102c90
	.long	0x3e12ebad, 0x3e141ff8
	// c2
	.align	64
	.long	0xbe5e5a9b, 0xbe5e2677
	.long	0xbe5d83f5, 0xbe5c6016
	.long	0xbe5abd0b, 0xbe58a6fd
	.long	0xbe562e02, 0xbe5362f8
	.long	0xbe68e27c, 0xbe646747
	.long	0xbe619a73, 0xbe5ff05a
	.long	0xbe5f0570, 0xbe5e92d0
	.long	0xbe5e662b, 0xbe5e5c08
	// c1
	.align	64
	.long	0x3ede5bd8, 0x3ede5b45
	.long	0x3ede57d8, 0x3ede4eb1
	.long	0x3ede3d37, 0x3ede2166
	.long	0x3eddf9d9, 0x3eddc5bb
	.long	0x3ede08ed, 0x3ede32e7
	.long	0x3ede4967, 0x3ede5490
	.long	0x3ede597f, 0x3ede5b50
	.long	0x3ede5bca, 0x3ede5bd9
	/* L2.  */
	.align	64
	.long	0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b
	.align	64
	.type	__svml_slog10_data_internal_avx512, @object
	.size	__svml_slog10_data_internal_avx512, .-__svml_slog10_data_internal_avx512
