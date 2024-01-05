/* Core .*/
/* Function log2f vectorized with AVX2.
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

/* Offsets for data table __svml_slog2_data_internal
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_slog2_data_internal
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-avx2-rodata-offsets.h"


#define _Coeff_9	0
#define _Coeff_8	32
#define _Coeff_7	64
#define _Coeff_6	96
#define _Coeff_5	128
#define _Coeff_4	160
#define _Coeff_3	192
#define _Coeff_2	224
#define _Coeff_1	256


	.section .text.avx2, "ax", @progbits
ENTRY(log2f_8_avx2_dev)
	/* reduction: compute r, n.  */
	vmovups	COMMON_DATA(_IBrkValue)(%rip), %ymm4


	vpsubd	%ymm4, %ymm0, %ymm1
	vmovups	COMMON_DATA(_NotiOffExpoMask)(%rip), %ymm2
	vpsubd	%ymm2, %ymm0, %ymm5
	vmovups	COMMON_DATA(_ILoRange)(%rip), %ymm6
	vpcmpgtd %ymm5, %ymm6, %ymm6
	vpandn	%ymm1, %ymm2, %ymm3
	vpsrad	$23, %ymm1, %ymm2
	vmovups	LOCAL_DATA(_Coeff_9)(%rip), %ymm1
	vpaddd	%ymm4, %ymm3, %ymm3

	vsubps	COMMON_DATA(_OneF)(%rip), %ymm3, %ymm3
	vfmadd213ps LOCAL_DATA(_Coeff_8)(%rip), %ymm3, %ymm1
	vmovups	LOCAL_DATA(_Coeff_7)(%rip), %ymm4
	vfmadd213ps LOCAL_DATA(_Coeff_6)(%rip), %ymm3, %ymm4
	vmulps	%ymm3, %ymm3, %ymm5
	vmovups	LOCAL_DATA(_Coeff_5)(%rip), %ymm7
	vfmadd213ps LOCAL_DATA(_Coeff_4)(%rip), %ymm3, %ymm7
	vmovmskps %ymm6, %edx

	vmovups	LOCAL_DATA(_Coeff_3)(%rip), %ymm6
	vfmadd213ps LOCAL_DATA(_Coeff_2)(%rip), %ymm3, %ymm6
	vfmadd213ps %ymm4, %ymm5, %ymm1
	vfmadd213ps %ymm7, %ymm5, %ymm1
	vfmadd213ps %ymm6, %ymm5, %ymm1
	vfmadd213ps LOCAL_DATA(_Coeff_1)(%rip), %ymm3, %ymm1
	vcvtdq2ps %ymm2, %ymm2

	/* combine and get argument value range mask.  */

	vfmadd213ps %ymm2, %ymm3, %ymm1
	testl	%edx, %edx

	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)


	vmovaps	%ymm1, %ymm0
	ret
	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a atanhf call. Optimize for code size
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):

	/* Use r13 to save/restore the stack. This allows us to use rbp
	   as callee save register saving code size.  */
	pushq	%r13
	cfi_adjust_cfa_offset (8)
	cfi_offset (r13, -16)
	/* Need to callee save registers to preserve state across tanhf
	   calls.  */
	pushq	%rbx
	cfi_adjust_cfa_offset (8)
	cfi_offset (rbx, -24)
	pushq	%rbp
	cfi_adjust_cfa_offset (8)
	cfi_offset (rbp, -32)
	movq	%rsp, %r13
	cfi_def_cfa_register (r13)

	/* Align stack and make room for 2x ymm vectors.  */
	andq	$-32, %rsp
	addq	$-64, %rsp

	/* Save all already computed inputs.  */
	vmovups	%ymm1, (%rsp)
	/* Save original input (ymm0 unchanged up to this point).  */
	vmovups	%ymm0, 32(%rsp)

	vzeroupper

	/* edx has 1s where there was a special value that needs to be
	   handled by a atanhf call.  */
	movl	%edx, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to atanhf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 28] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop. Realigning
	   also costs more code size.  */
	xorl	%ebp, %ebp
	tzcntl	%ebx, %ebp

	/* Scalar math fucntion call to process special input.  */
	vmovss	32(%rsp, %rbp, 4), %xmm0
	call	log2f@PLT

	/* No good way to avoid the store-forwarding fault this will
	   cause on return. `lfence` avoids the SF fault but at greater
	   cost as it serialized stack/callee save restoration.  */
	vmovss	%xmm0, (%rsp, %rbp, 4)

	blsrl	%ebx, %ebx
	jnz	L(SPECIAL_VALUES_LOOP)



	/* All results have been written to (%rsp).  */
	vmovups	(%rsp), %ymm0
	/* Restore rsp.  */
	movq	%r13, %rsp
	cfi_def_cfa_register (rsp)
	/* Restore callee save registers.  */
	popq	%rbp
	cfi_adjust_cfa_offset (-8)
	cfi_restore (rbp)
	popq	%rbx
	cfi_adjust_cfa_offset (-8)
	cfi_restore (rbp)
	popq	%r13
	cfi_adjust_cfa_offset (-8)
	cfi_restore (r13)
	ret
END(log2f_8_avx2_dev)

	.section .rodata.avx2, "a"
	.align	32

LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_9, 0x3e554012)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_8, 0xbe638e14)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_7, 0x3e4d660b)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_6, 0xbe727824)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_5, 0x3e93dd07)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_4, 0xbeb8b969)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_3, 0x3ef637c0)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_2, 0xbf38aa2b)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_1, 0x3fb8aa3b)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
