/* Core .*/
/* Function logf vectorized with SSE4.
   Copyright (C) 2014-2022 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"

#define _sPoly_1	0
#define _sPoly_2	64
#define _sPoly_3	128
#define _sPoly_4	192
#define _sPoly_5	256
#define _sPoly_6	320
#define _sPoly_7	384
#define _iHiDelta	448
#define _iLoRange	512
#define _iBrkValue	576
#define _iOffExpoMask	640
#define _sOne	704
#define _sLn2	768
#define _sInfs	832
#define _sOnes	896
#define _sZeros	960

.macro float_vector offset value
	.if	.-__svml_dev_slog_data_sse4 != \offset
	.err
	.endif
	.rept	16
	.long	\value
	.endr
.endm

#define xmmA	xmm1
	.section .text.sse4, "ax", @progbits
ENTRY(logf_4_sse4_dev)
	// Code Out Begin
	movdqu	_iLoRange + __svml_dev_slog_data_sse4(%rip), %xmmA
	/* ALGORITHM DESCRIPTION: log(x) = exponent_x*log(2) + for
	   4-ulp, degree 3 for half-precision.  */
	/* reduction: compute r,n.  */
	/* check for working range, set special argument mask
	   (denormals/zero/Inf/NaN).  */
	movdqu	_iHiDelta + __svml_dev_slog_data_sse4(%rip), %xmm2
	movaps	%xmm0, %xmm3
	psubd	%xmm2, %xmm3
	pcmpgtd	%xmm3, %xmmA
	movmskps %xmmA, %eax

	movdqu	_iBrkValue + __svml_dev_slog_data_sse4(%rip), %xmmA
	movaps	%xmm0, %xmm3
	psubd	%xmmA, %xmm0
	pandn	%xmm0, %xmm2
	paddd	%xmmA, %xmm2
	/* reduced argument R.  */
	subps	_sOne + __svml_dev_slog_data_sse4(%rip), %xmm2
	/* exponent_x (mantissa_x<4/3) or exponent_x+1 (mantissa_x>4/3).
	 */
	psrad	$0x17, %xmm0
	/* mantissa_x (mantissa_x<4/3), or 0.5*mantissa_x
	   (mantissa_x>4/3).  */
	cvtdq2ps %xmm0, %xmm0
	/* final reconstruction: add exponent_value*log2 to polynomial
	   result.  */
	mulps	_sLn2 + __svml_dev_slog_data_sse4(%rip), %xmm0
	movups	_sPoly_7 + __svml_dev_slog_data_sse4(%rip), %xmmA
	/* polynomial evaluation starts here.  */
	mulps	%xmm2, %xmmA
	addps	_sPoly_6 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	_sPoly_5 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	_sPoly_4 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	_sPoly_3 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	_sPoly_2 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	_sPoly_1 + __svml_dev_slog_data_sse4(%rip), %xmmA
	mulps	%xmm2, %xmmA
	/* polynomial evaluation end.  */
	mulps	%xmm2, %xmmA
	addps	%xmmA, %xmm2
	addps	%xmm2, %xmm0
	testl	%eax, %eax	/* N. */
	jne	L(SPECIAL_VALUES_BRANCH)	/* N. */
	ret	/* N. */
	/* Cold case. edx has 1s where there was a special value that
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):
	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	/* N.  */
	/* N.  */
	subq	$0x38, %rsp	/* N. */
	/* N.  */
	movups	%xmm0, 24(%rsp)	/* N. */
	/* N.  */
	movups	%xmm3, 40(%rsp)	/* N. */
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
	call	logf@PLT
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
END(logf_4_sse4_dev)

	.section .rodata, "a"
	.align	64

	/* Data table for vector implementations of function logf. The
	   table may contain polynomial, reduction, lookup coefficients
	   and other coefficients obtained through different methods of
	   research and experimental work.  */

	.globl	__svml_dev_slog_data_sse4
__svml_dev_slog_data_sse4:

	/* Polynomial sPoly[] coefficients: *
	   -5.0000000000000000000000000e-01.  */
	float_vector _sPoly_1 0xbf000000

	/* 3.3336564898490905761718750e-01.  */
	float_vector _sPoly_2 0x3eaaaee7

	/* -2.5004664063453674316406250e-01.  */
	float_vector _sPoly_3 0xbe80061d

	/* 1.9822503626346588134765625e-01.  */
	float_vector _sPoly_4 0x3e4afb81

	/* -1.6462457180023193359375000e-01.  */
	float_vector _sPoly_5 0xbe289358

	/* 1.6964881122112274169921875e-01.  */
	float_vector _sPoly_6 0x3e2db86b

	/* -1.5177205204963684082031250e-01.  */
	float_vector _sPoly_7 0xbe1b6a22

	/* Constant for work range check: Delta 80000000-7f800000.  */
	float_vector _iHiDelta 0xff800000

	/* Constant for work range check: 00800000 + Delta.  */
	float_vector _iLoRange 0x01000000

	/* Mantissa break point  SP 2/3.  */
	float_vector _iBrkValue 0x3f2aaaab

	/* SP significand mask.  */
	float_vector _iOffExpoMask 0x007fffff

	/* 1.0f.  */
	float_vector _sOne 0x3f800000

	/* SP log(2).  */
	float_vector _sLn2 0x3f317218

	/* SP infinity, +/-.  */
	.if	.-__svml_dev_slog_data_sse4 != _sInfs
	.err
	.endif
	.long	0x7f800000
	.long	0xff800000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000

	/* SP one, +/-.  */
	.if	.-__svml_dev_slog_data_sse4 != _sOnes
	.err
	.endif
	.long	0x3f800000
	.long	0xbf800000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000

	/* SP zero +/-.  */
	.if	.-__svml_dev_slog_data_sse4 != _sZeros
	.err
	.endif
	.long	0x00000000
	.long	0x80000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.quad	0x0000000000000000
	.type	__svml_dev_slog_data_sse4, @object
	.size	__svml_dev_slog_data_sse4, .-__svml_dev_slog_data_sse4
