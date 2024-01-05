/* Core .*/
/* Function log2f vectorized with SSE4.
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
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"





#define _Coeff_3	0
#define _Coeff_2	16
#define _Coeff_7	32
#define _Coeff_6	48
#define _Coeff_9	64
#define _Coeff_8	80
#define _Coeff_5	96
#define _Coeff_4	112
#define _Coeff_1	128

#define xmmA	xmm1
	.section .text.sse4, "ax", @progbits
ENTRY(log2f_4_sse4_dev)
	movdqu	COMMON_DATA(_NotiOffExpoMask)(%rip), %xmmA
	movaps	%xmm0, %xmm2
	psubd	%xmmA, %xmm0
	movaps	COMMON_DATA(_ILoRange)(%rip), %xmm5

	pcmpgtd	%xmm0, %xmm5
	/* combine and get argument value range mask.  */
	movmskps %xmm5, %eax
	/* reduction: compute r, n.  */
	movdqu	COMMON_DATA(_IBrkValue)(%rip), %xmm0
	movaps	%xmm2, %xmm5
	psubd	%xmm0, %xmm2
	pandn	%xmm2, %xmmA
	paddd	%xmm0, %xmmA
	psrad	$0x17, %xmm2
	cvtdq2ps %xmm2, %xmm0
	subps	COMMON_DATA(_OneF)(%rip), %xmmA
	movups	LOCAL_DATA(_Coeff_3)(%rip), %xmm2
	mulps	%xmmA, %xmm2
	addps	LOCAL_DATA(_Coeff_2)(%rip), %xmm2
	movups	LOCAL_DATA(_Coeff_7)(%rip), %xmm4
	mulps	%xmmA, %xmm4
	addps	LOCAL_DATA(_Coeff_6)(%rip), %xmm4
	movups	LOCAL_DATA(_Coeff_9)(%rip), %xmm3
	mulps	%xmmA, %xmm3
	addps	LOCAL_DATA(_Coeff_8)(%rip), %xmm3
	movaps	%xmmA, %xmm6
	mulps	%xmmA, %xmmA
	mulps	%xmmA, %xmm3
	addps	%xmm3, %xmm4
	mulps	%xmmA, %xmm4
	movups	LOCAL_DATA(_Coeff_5)(%rip), %xmm3
	mulps	%xmm6, %xmm3
	addps	LOCAL_DATA(_Coeff_4)(%rip), %xmm3
	addps	%xmm4, %xmm3
	mulps	%xmm3, %xmmA
	addps	%xmmA, %xmm2
	mulps	%xmm6, %xmm2
	addps	LOCAL_DATA(_Coeff_1)(%rip), %xmm2
	mulps	%xmm2, %xmm6
	addps	%xmm6, %xmm0
	testl	%eax, %eax
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)
	ret

	/* Cold case. edx has 1s where there was a special value that
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):
	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	subq	$0x38, %rsp
	movups	%xmm0, 24(%rsp)
	movups	%xmm5, 40(%rsp)

	/* Use rbx/rbp for callee save registers as they get short
	   encoding for many instructions (as compared with r12/r13).  */
	movq	%rbx, (%rsp)
	cfi_offset (rbx, -64)
	movq	%rbp, 8(%rsp)
	cfi_offset (rbp, -56)
	/* edx has 1s where there was a special value that needs to be
	   handled by a tanhf call.  */
	movl	%eax, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to tanhf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 12] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop.  */
	xorl	%ebp, %ebp
	bsfl	%ebx, %ebp

	/* Scalar math fucntion call to process special input.  */
	movss	40(%rsp, %rbp, 4), %xmm0
	call	log2f@PLT
	INC_FALLBACK0
	/* No good way to avoid the store-forwarding fault this will
	   cause on return. `lfence` avoids the SF fault but at greater
	   cost as it serialized stack/callee save restoration.  */
	movss	%xmm0, 24(%rsp, %rbp, 4)

	leal	-1(%rbx), %eax
	andl	%eax, %ebx
	jnz	L(SPECIAL_VALUES_LOOP)

	/* All results have been written to 24(%rsp).  */
	movups	24(%rsp), %xmm0
	movq	(%rsp), %rbx
	cfi_restore (rbx)
	movq	8(%rsp), %rbp
	cfi_restore (rbp)
	addq	$56, %rsp
	cfi_def_cfa_offset (8)
	ret
END(log2f_4_sse4_dev)

	.section .rodata.sse4, "a"
	.align	16

LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_3, 0x3ef637c0)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_2, 0xbf38aa2b)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_7, 0x3e4d660b)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_6, 0xbe727824)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_9, 0x3e554012)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_8, 0xbe638e14)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_5, 0x3e93dd07)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_4, 0xbeb8b969)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_1, 0x3fb8aa3b)
	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
