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
#define LOCAL_DATA_NAME	__svml_dev_slog_data_sse4
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"

#define _sPoly_7	0
#define _sPoly_6	16
#define _sPoly_5	32
#define _sPoly_4	48
#define _sPoly_3	64
#define _sPoly_2	80


#define xmmA	xmm1
	.section .text.sse4, "ax", @progbits
ENTRY(logf_4_sse4_dev)
	movdqu	COMMON_DATA(_ILoRange)(%rip), %xmmA
	/* ALGORITHM DESCRIPTION:
	   if mantissa_x<4/3
        log(x) = exponent_x*log(2) + log(mantissa_x)
	   if mantissa_x>4/3
        log(x) = (exponent_x+1)*log(2) + log(0.5*mantissa_x)
	   
	   R = mantissa_x - 1,     if mantissa_x<4/3
	   R = 0.5*mantissa_x - 1, if mantissa_x>4/3
	   |R|< 1/3
	   
	   log(1+R) is approximated as a polynomial: degree 9 for
	   1-ulp, degree 7 for 4-ulp, degree 3 for half-precision.  */

	/* check for working range, set special argument mask
	   (denormals/zero/Inf/NaN).  */
	movdqu	COMMON_DATA(_NotiOffExpoMask)(%rip), %xmm2
	movaps	%xmm0, %xmm3
	psubd	%xmm2, %xmm3
	pcmpgtd	%xmm3, %xmmA
	movmskps %xmmA, %eax

	movdqu	COMMON_DATA(_IBrkValue)(%rip), %xmmA
	movaps	%xmm0, %xmm3
	psubd	%xmmA, %xmm0
	pandn	%xmm0, %xmm2
	paddd	%xmmA, %xmm2
	/* reduced argument R.  */
	subps	COMMON_DATA(_OneF)(%rip), %xmm2
	/* exponent_x (mantissa_x<4/3),
	   or exponent_x+1 (mantissa_x>4/3).  */
	psrad	$0x17, %xmm0
	/* mantissa_x (mantissa_x<4/3),
	   or 0.5 mantissa_x (mantissa_x>4/3).  */
	cvtdq2ps %xmm0, %xmm0
	/* final reconstruction: add exponent_value * log2 to polynomial
	   result.  */
	mulps	COMMON_DATA(_Ln2)(%rip), %xmm0
	movups	LOCAL_DATA(_sPoly_7)(%rip), %xmmA
	/* polynomial evaluation starts here.  */
	mulps	%xmm2, %xmmA
	addps	LOCAL_DATA(_sPoly_6)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	LOCAL_DATA(_sPoly_5)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	LOCAL_DATA(_sPoly_4)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	LOCAL_DATA(_sPoly_3)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	LOCAL_DATA(_sPoly_2)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	addps	COMMON_DATA(_Neg5F)(%rip), %xmmA
	mulps	%xmm2, %xmmA
	/* polynomial evaluation end.  */
	mulps	%xmm2, %xmmA
	addps	%xmmA, %xmm2
	addps	%xmm2, %xmm0
	testl	%eax, %eax
	jne	L(SPECIAL_VALUES_BRANCH)
	ret

	/* Cold case. edx has 1s where there was a special value that
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):
	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	subq	$0x38, %rsp
	movups	%xmm0, 24(%rsp)
	movups	%xmm3, 40(%rsp)

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

	.section .rodata.sse4, "a"
	.align	16

	/* Data table for vector implementations of function logf. The
	   table may contain polynomial, reduction, lookup coefficients
	   and other coefficients obtained through different methods of
	   research and experimental work.  */

	.globl	LOCAL_DATA_NAME
LOCAL_DATA_NAME:
	/* Polynomial sPoly[] coefficients:.  */
	/* -1.5177205204963684082031250e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_7, 0xbe1b6a22)

	/* 1.6964881122112274169921875e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_6, 0x3e2db86b)

	/* -1.6462457180023193359375000e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_5, 0xbe289358)

	/* 1.9822503626346588134765625e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_4, 0x3e4afb81)

	/* -2.5004664063453674316406250e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_3, 0xbe80061d)

	/* 3.3336564898490905761718750e-01.  */
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_2, 0x3eaaaee7)


	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
