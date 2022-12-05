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
#define MinNorm	0
#define MaxNorm	16
#define iBrkValue	32
#define iOffExpoMask	48
#define One	64
#define sPoly	80

#define xmmA xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(log2f_4_sse4_dev)
	// Code Out Begin
	movdqu	MinNorm + __svml_slog2_data_internal(%rip), %xmmA
	movaps	%xmm0, %xmm2
    psubd   %xmmA, %xmm0
	movaps	MaxNorm + __svml_slog2_data_internal(%rip), %xmm5
    
	pcmpgtd %xmm0, %xmm5
	/* combine and get argument value range mask.  */
	movmskps %xmm5, %eax
	/* reduction: compute r, n.  */
	movdqu	iBrkValue + __svml_slog2_data_internal(%rip), %xmm0
	movaps	%xmm2, %xmm5
	psubd	%xmm0, %xmm2
	pandn	%xmm2, %xmmA
	paddd	%xmm0, %xmmA
	psrad	$0x17, %xmm2
	cvtdq2ps %xmm2, %xmm0
	subps	One + __svml_slog2_data_internal(%rip), %xmmA
	movups	sPoly + 96 + __svml_slog2_data_internal(%rip), %xmm2
	mulps	%xmmA, %xmm2
	addps	sPoly + 112 + __svml_slog2_data_internal(%rip), %xmm2
	movups	sPoly + 32 + __svml_slog2_data_internal(%rip), %xmm4
	mulps	%xmmA, %xmm4
	addps	sPoly + 48 + __svml_slog2_data_internal(%rip), %xmm4
	movups	sPoly + __svml_slog2_data_internal(%rip), %xmm3
	mulps	%xmmA, %xmm3
	addps	sPoly + 16 + __svml_slog2_data_internal(%rip), %xmm3
	movaps	%xmmA, %xmm6
	mulps	%xmmA, %xmmA
	mulps	%xmmA, %xmm3
	addps	%xmm3, %xmm4
	mulps	%xmmA, %xmm4
	movups	sPoly + 64 + __svml_slog2_data_internal(%rip), %xmm3
	mulps	%xmm6, %xmm3
	addps	sPoly + 80 + __svml_slog2_data_internal(%rip), %xmm3
	addps	%xmm4, %xmm3
	mulps	%xmm3, %xmmA
	addps	%xmmA, %xmm2
	mulps	%xmm6, %xmm2
	addps	sPoly + 128 + __svml_slog2_data_internal(%rip), %xmm2
	mulps	%xmm2, %xmm6
	addps	%xmm6, %xmm0
	testl	%eax, %eax	/* N. */
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)	/* N. */
	ret	/* N. */
	/* Restore registers * and exit the function.  */
	/* Cold case. edx has 1s where there was a special value that
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):
	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	/* N.  */
	/* N.  */
    subq	$0x38, %rsp	/* N. */
	movups	%xmm0, 24(%rsp)	/* N. */
	/* N.  */
	movups	%xmm5, 40(%rsp)	/* N. */
	// Code Out End

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

	.section .rodata, "a"
	.align	16

#ifdef __svml_slog2_data_internal_typedef
	typedef	unsigned int VUINT32;
	typedef	struct{
	__declspec (align(16))VUINT32 MinNorm[4][1];
	__declspec (align(16))VUINT32 MaxNorm[4][1];
	__declspec (align(16))VUINT32 iBrkValue[4][1];
	__declspec (align(16))VUINT32 iOffExpoMask[4][1];
	__declspec (align(16))VUINT32 One[4][1];
	__declspec (align(16))VUINT32 sPoly[9][4][1];
	}__svml_slog2_data_internal;
#endif
__svml_slog2_data_internal:
	/* MinNorm.  */
	.long	0xff800000, 0xff800000, 0xff800000, 0xff800000
	/* MaxNorm.  */
	.align	16
	.long	0x01000000, 0x01000000, 0x01000000, 0x01000000
	/* iBrkValue = SP 2/3.  */
	.align	16
	.long	0x3f2aaaab, 0x3f2aaaab, 0x3f2aaaab, 0x3f2aaaab
	/* iOffExpoMask = SP significand mask.  */
	.align	16
	.long	0x007fffff, 0x007fffff, 0x007fffff, 0x007fffff
	/* sOne = SP 1.0.  */
	.align	16
	.long	0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000
	/* spoly[9].  */
	.align	16
	.long	0x3e554012, 0x3e554012, 0x3e554012, 0x3e554012	/* coeff9 */
	.long	0xbe638E14, 0xbe638E14, 0xbe638E14, 0xbe638E14	/* coeff8 */
	.long	0x3e4D660B, 0x3e4D660B, 0x3e4D660B, 0x3e4D660B	/* coeff7 */
	.long	0xbe727824, 0xbe727824, 0xbe727824, 0xbe727824	/* coeff6 */
	.long	0x3e93DD07, 0x3e93DD07, 0x3e93DD07, 0x3e93DD07	/* coeff5 */
	.long	0xbeB8B969, 0xbeB8B969, 0xbeB8B969, 0xbeB8B969	/* coeff4 */
	.long	0x3eF637C0, 0x3eF637C0, 0x3eF637C0, 0x3eF637C0	/* coeff3 */
	.long	0xbf38AA2B, 0xbf38AA2B, 0xbf38AA2B, 0xbf38AA2B	/* coeff2 */
	.long	0x3fB8AA3B, 0x3fB8AA3B, 0x3fB8AA3B, 0x3fB8AA3B	/* coeff1 */
	.align	16
	.type	__svml_slog2_data_internal, @object
	.size	__svml_slog2_data_internal, .-__svml_slog2_data_internal
