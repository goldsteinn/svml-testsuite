/* Core .*/
/* Function log10f vectorized with SSE4.
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
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"

#define _L2L	0
#define _Coeff_9	16
#define _Coeff_8	32
#define _Coeff_7	48
#define _Coeff_6	64
#define _Coeff_5	80
#define _Coeff_4	96
#define _Coeff_3	112
#define _Coeff_2	128
#define _Coeff_1	144
#define _L2H	160

#define xmmA	xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(log10f_4_sse4_dev)
	// Code Out Begin
	movdqu	COMMON_DATA(_NotiOffExpoMask)(%rip), %xmm2
	movaps	%xmm0, %xmm3
	psubd	%xmm2, %xmm0
	movaps	COMMON_DATA(_ILoRange)(%rip), %xmm4
	pcmpgtd	%xmm0, %xmm4
	/* combine and get argument value range mask.  */
	movmskps %xmm4, %eax
	movups	LOCAL_DATA(_L2L)(%rip), %xmm0
	/* reduction: compute r, n.  */
	movdqu	COMMON_DATA(_IBrkValue)(%rip), %xmm4
	movaps	%xmm3, %xmm6
	psubd	%xmm4, %xmm3
	pandn	%xmm3, %xmm2
	paddd	%xmm4, %xmm2
	subps	COMMON_DATA(_OneF)(%rip), %xmm2
	psrad	$0x17, %xmm3
	cvtdq2ps %xmm3, %xmm4
	mulps	%xmm4, %xmm0
	movaps	%xmm2, %xmm3
	mulps	%xmm2, %xmm2
	movups	LOCAL_DATA(_Coeff_9)(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	LOCAL_DATA(_Coeff_8)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	movups	LOCAL_DATA(_Coeff_7)(%rip), %xmm5
	mulps	%xmm3, %xmm5
	addps	LOCAL_DATA(_Coeff_6)(%rip), %xmm5
	addps	%xmmA, %xmm5
	mulps	%xmm2, %xmm5
	movups	LOCAL_DATA(_Coeff_5)(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	LOCAL_DATA(_Coeff_4)(%rip), %xmmA
	addps	%xmm5, %xmmA
	mulps	%xmmA, %xmm2
	movups	LOCAL_DATA(_Coeff_3)(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	LOCAL_DATA(_Coeff_2)(%rip), %xmmA
	addps	%xmm2, %xmmA
	mulps	%xmm3, %xmmA
	addps	LOCAL_DATA(_Coeff_1)(%rip), %xmmA
	mulps	%xmmA, %xmm3
	addps	%xmm3, %xmm0
	movups	LOCAL_DATA(_L2H)(%rip), %xmm2
	mulps	%xmm4, %xmm2
	addps	%xmm2, %xmm0
	/* N.  */
	testl	%eax, %eax	/* N. */
	/* Go to special inputs processing branch.  */
	/* N.  */
	jne	L(SPECIAL_VALUES_BRANCH)	/* N. */
	/* Restore registers.  */
	/* N.  */
	/* N.  */
	ret	/* N. */
	/* Cold case. edx has 1s where there was a special value that
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):
	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	/* N.  */
	subq	$0x38, %rsp	/* N. */
	movups	%xmm0, 24(%rsp)	/* N. */
	movups	%xmm6, 40(%rsp)	/* N. */
	// Code Out End

	/* Use rbx/rbp for callee save registers as they get short
	   encoding for many instructions (as compared with r12/r13).
	 */
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
	call	log10f@PLT
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
END(log10f_4_sse4_dev)

	.section .rodata.sse4, "a"
	.align	16

LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _L2L, 0xb64af600)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_9, 0x3d8063b4)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_8, 0xbd890073)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_7, 0x3d775317)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_6, 0xbd91fb27)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_5, 0x3db20b96)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_4, 0xbdde6e20)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_3, 0x3e143ce5)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_2, 0xbe5e5bc5)
	DATA_VEC (LOCAL_DATA_NAME, _Coeff_1, 0x3ede5bd9)
	DATA_VEC (LOCAL_DATA_NAME, _L2H, 0x3e9a2100)
	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
