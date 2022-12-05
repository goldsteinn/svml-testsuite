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
#define MinNorm	0
#define MaxNorm	16
#define L2H	32
#define L2L	48
#define iBrkValue	64
#define iOffExpoMask	80
#define One	96
#define sPoly	112
#define L2	256

#define xmmA	xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(log10f_4_sse4_dev)
	// Code Out Begin
	movdqu	MinNorm + __svml_slog10_data_internal(%rip), %xmm2
	movaps	%xmm0, %xmm3
	psubd	%xmm2, %xmm0
	movaps	MaxNorm + __svml_slog10_data_internal(%rip), %xmm4
	pcmpgtd	%xmm0, %xmm4
	/* combine and get argument value range mask.  */
	movmskps %xmm4, %eax
	movups	L2L + __svml_slog10_data_internal(%rip), %xmm0
	/* reduction: compute r, n.  */
	movdqu	iBrkValue + __svml_slog10_data_internal(%rip), %xmm4
	movaps	%xmm3, %xmm6
	psubd	%xmm4, %xmm3
	pandn	%xmm3, %xmm2
	paddd	%xmm4, %xmm2
	subps	One + __svml_slog10_data_internal(%rip), %xmm2
	psrad	$0x17, %xmm3
	cvtdq2ps %xmm3, %xmm4
	mulps	%xmm4, %xmm0
	movaps	%xmm2, %xmm3
	mulps	%xmm2, %xmm2
	movups	sPoly + __svml_slog10_data_internal(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	sPoly + 16 + __svml_slog10_data_internal(%rip), %xmmA
	mulps	%xmm2, %xmmA
	movups	sPoly + 32 + __svml_slog10_data_internal(%rip), %xmm5
	mulps	%xmm3, %xmm5
	addps	sPoly + 48 + __svml_slog10_data_internal(%rip), %xmm5
	addps	%xmmA, %xmm5
	mulps	%xmm2, %xmm5
	movups	sPoly + 64 + __svml_slog10_data_internal(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	sPoly + 80 + __svml_slog10_data_internal(%rip), %xmmA
	addps	%xmm5, %xmmA
	mulps	%xmmA, %xmm2
	movups	sPoly + 96 + __svml_slog10_data_internal(%rip), %xmmA
	mulps	%xmm3, %xmmA
	addps	sPoly + 112 + __svml_slog10_data_internal(%rip), %xmmA
	addps	%xmm2, %xmmA
	mulps	%xmm3, %xmmA
	addps	sPoly + 128 + __svml_slog10_data_internal(%rip), %xmmA
	mulps	%xmmA, %xmm3
	addps	%xmm3, %xmm0
	movups	L2H + __svml_slog10_data_internal(%rip), %xmm2
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

	.section .rodata, "a"
	.align	16

#ifdef __svml_slog10_data_internal_typedef
	typedef	unsigned int VUINT32;
	typedef	struct{
	__declspec (align(16))VUINT32 MinNorm[4][1];
	__declspec (align(16))VUINT32 MaxNorm[4][1];
	__declspec (align(16))VUINT32 L2H[4][1];
	__declspec (align(16))VUINT32 L2L[4][1];
	__declspec (align(16))VUINT32 iBrkValue[4][1];
	__declspec (align(16))VUINT32 iOffExpoMask[4][1];
	__declspec (align(16))VUINT32 One[4][1];
	__declspec (align(16))VUINT32 sPoly[9][4][1];
	__declspec (align(16))VUINT32 L2[4][1];
	}__svml_slog10_data_internal;
#endif
__svml_slog10_data_internal:
	/* MinNorm.  */
	.long	0xff800000, 0xff800000, 0xff800000, 0xff800000
	/* MaxNorm.  */
	.align	16
	.long	0x01000000, 0x01000000, 0x01000000, 0x01000000
	/* L2H.  */
	.align	16
	.long	0x3e9a2100, 0x3e9a2100, 0x3e9a2100, 0x3e9a2100
	/* L2L.  */
	.align	16
	.long	0xb64AF600, 0xb64AF600, 0xb64AF600, 0xb64AF600
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
	.long	0x3d8063B4, 0x3d8063B4, 0x3d8063B4, 0x3d8063B4	/* coeff9 */
	.long	0xbd890073, 0xbd890073, 0xbd890073, 0xbd890073	/* coeff8 */
	.long	0x3d775317, 0x3d775317, 0x3d775317, 0x3d775317	/* coeff7 */
	.long	0xbd91FB27, 0xbd91FB27, 0xbd91FB27, 0xbd91FB27	/* coeff6 */
	.long	0x3dB20B96, 0x3dB20B96, 0x3dB20B96, 0x3dB20B96	/* coeff5 */
	.long	0xbdDE6E20, 0xbdDE6E20, 0xbdDE6E20, 0xbdDE6E20	/* coeff4 */
	.long	0x3e143CE5, 0x3e143CE5, 0x3e143CE5, 0x3e143CE5	/* coeff3 */
	.long	0xbe5E5BC5, 0xbe5E5BC5, 0xbe5E5BC5, 0xbe5E5BC5	/* coeff2 */
	.long	0x3eDE5BD9, 0x3eDE5BD9, 0x3eDE5BD9, 0x3eDE5BD9	/* coeff1 */
	/* L2.  */
	.align	16
	.long	0x3e9a209b, 0x3e9a209b, 0x3e9a209b, 0x3e9a209b
	.align	16
	.type	__svml_slog10_data_internal, @object
	.size	__svml_slog10_data_internal, .-__svml_slog10_data_internal
