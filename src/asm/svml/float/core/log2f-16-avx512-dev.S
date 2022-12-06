/* Core .*/
/* Function log2f vectorized with AVX-512.
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
 *    log2(x) = k - log2(Rcp) + poly_approximation(R)
 *       log2(Rcp) is tabulated
 *
 *
 */

/* Offsets for data table __svml_slog2_data_internal_avx512
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_slog2_data_internal_avx512
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-evex512-rodata-offsets.h"

#define _Coeff_4	0
#define _Coeff_3	64
#define _Coeff_2	128
#define _Coeff_1	192



	.section .text.evex512, "ax", @progbits
ENTRY(log2f_16_avx512_dev)
	vgetmantps $11, {sae}, %zmm0, %zmm3
	vmovups	COMMON_DATA(_OneF)(%rip), %zmm1
	vgetexpps {sae}, %zmm0, %zmm5

	/* x<=0?  */

	vsubps	{rn-sae}, %zmm1, %zmm3, %zmm9
	vpsrld	$19, %zmm3, %zmm7
	vgetexpps {sae}, %zmm3, %zmm6
	vpermps	LOCAL_DATA(_Coeff_4)(%rip), %zmm7, %zmm1
	vpermps	LOCAL_DATA(_Coeff_3)(%rip), %zmm7, %zmm2
	vpermps	LOCAL_DATA(_Coeff_2)(%rip), %zmm7, %zmm4
	vpermps	LOCAL_DATA(_Coeff_1)(%rip), %zmm7, %zmm8
	vsubps	{rn-sae}, %zmm6, %zmm5, %zmm10
	vfpclassps $94, %zmm0, %k0
	vfmadd213ps {rn-sae}, %zmm2, %zmm9, %zmm1
	kmovw	%k0, %edx
	vfmadd213ps {rn-sae}, %zmm4, %zmm9, %zmm1
	vfmadd213ps {rn-sae}, %zmm8, %zmm9, %zmm1
	vfmadd213ps {rn-sae}, %zmm10, %zmm9, %zmm1
	testl	%edx, %edx

	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)
	vmovaps	%zmm1, %zmm0
	ret


	/* Restore registers * and exit the function.  */

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
	call	log2f@PLT
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
END(log2f_16_avx512_dev)

	.section .rodata.evex512, "a"
	.align	64

LOCAL_DATA_NAME:
	float_block (LOCAL_DATA_NAME, _Coeff_4,
	0xbea77e4a, 0xbe8aae3d,
	0xbe67fe32, 0xbe43d1b6,
	0xbe26a589, 0xbe0ee09b,
	0xbdf6a8a1, 0xbdd63b49,
	0xbf584e51, 0xbf3e80a1,
	0xbf2892f0, 0xbf15d377,
	0xbf05b525, 0xbeef8e30,
	0xbed75c8f, 0xbec24184)

	float_block (LOCAL_DATA_NAME, _Coeff_3,
	0x3ef5910c, 0x3ef045a1,
	0x3ee7d87e, 0x3eddbb84,
	0x3ed2d6df, 0x3ec7bbd2,
	0x3ebcc42f, 0x3eb22616,
	0x3e8f3399, 0x3eb1223e,
	0x3ec9db4a, 0x3edb7a09,
	0x3ee79a1a, 0x3eef77cb,
	0x3ef407a4, 0x3ef607b4)

	float_block (LOCAL_DATA_NAME, _Coeff_2,
	0xbf38a934, 0xbf387de6,
	0xbf37f6f0, 0xbf37048b,
	0xbf35a88a, 0xbf33ed04,
	0xbf31df56, 0xbf2f8d82,
	0xbf416814, 0xbf3daf58,
	0xbf3b5c08, 0xbf39fa2a,
	0xbf393713, 0xbf38d7e1,
	0xbf38b2cd, 0xbf38aa62)

	float_block (LOCAL_DATA_NAME, _Coeff_1,
	0x3fb8aa3b, 0x3fb8a9c0,
	0x3fb8a6e8, 0x3fb89f4e,
	0x3fb890cb, 0x3fb879b1,
	0x3fb858d8, 0x3fb82d90,
	0x3fb8655e, 0x3fb8883a,
	0x3fb89aea, 0x3fb8a42f,
	0x3fb8a848, 0x3fb8a9c9,
	0x3fb8aa2f, 0x3fb8aa3b)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
