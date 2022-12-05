/* Core .*/
/* Function atanhf vectorized with SSE4.
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
 *   Compute atanh(x) as 0.5 * log((1 + x)/(1 - x))
 *
 *   Special cases:
 *
 *   atanh(0)  = 0
 *   atanh(+1) = +INF
 *   atanh(-1) = -INF
 *   atanh(x)  = NaN if |x| > 1, or if x is a NaN or INF
 *
 */

/* Offsets for data table __svml_satanh_data_internal_avx512. Ordered
   by use in the function. On cold-starts this might help the
   prefetcher. Possibly a better idea is to interleave start/end so
   that the prefetcher is less likely to detect a stream and pull
   irrelivant lines into cache.  */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_satanh_data_internal
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"

#define _Poly_1	0
#define _Poly_2	16
#define _Poly_3	32
#define _Poly_4	48
#define _Poly_5	64
#define _Poly_6	80
#define _Poly_7	96
#define _TinyRange	112




	.section .text.sse4, "ax", @progbits
ENTRY(atanhf_4_sse4_dev)
	movaps	%xmm0, %xmm5

	/* Load constants including One = 1.  */
	movups	COMMON_DATA(_OneF)(%rip), %xmm4
	movaps	%xmm5, %xmm3

	/* Strip off the sign, so treat X as positive until right at the
	   end.  */
	movups	COMMON_DATA(_AbsMask)(%rip), %xmm1
	movaps	%xmm4, %xmm2
	andps	%xmm1, %xmm0
	movaps	%xmm4, %xmm10
	movups	COMMON_DATA(_Neg4096)(%rip), %xmm11
	movaps	%xmm4, %xmm14
	movaps	%xmm11, %xmm9


	/* * Compute V = 2 * X trivially, and UHi + U_lo = 1 - X in two
	   pieces, * the upper part UHi being <= 12 bits long. Then we
	   have * atanh(X) = 1/2 * log((1 + X) / (1 - X)) = 1/2 *
	   log1p(V / (UHi + ULo)).  */
	movaps	%xmm0, %xmm6
	mulps	%xmm5, %xmm3
	subps	%xmm0, %xmm2
	addps	%xmm0, %xmm6
	subps	%xmm2, %xmm10
	addps	%xmm5, %xmm3
	subps	%xmm0, %xmm10
	andps	%xmm2, %xmm9


	/* * Check whether |X| < 1, in which case we use the main
	   function. * Otherwise set the rangemask so that the callout
	   will get used. * Note that this will also use the callout
	   for NaNs since not(NaN < 1).  */
	rcpps	%xmm9, %xmm7
	subps	%xmm9, %xmm2
	andps	%xmm11, %xmm7


	/* * Split V as well into upper 12 bits and lower part, so that
	   we can get * a preliminary quotient estimate without
	   rounding error.  */
	andps	%xmm6, %xmm11
	mulps	%xmm7, %xmm9
	addps	%xmm2, %xmm10
	subps	%xmm11, %xmm6

	/* Hence get initial quotient estimate QHi + QLo = R * VHi + R *
	   VLo.  */
	mulps	%xmm7, %xmm11
	mulps	%xmm7, %xmm10
	subps	%xmm9, %xmm14
	mulps	%xmm6, %xmm7
	subps	%xmm10, %xmm14

	/* Compute D = E + E^2.  */
	movaps	%xmm14, %xmm13
	movaps	%xmm4, %xmm8
	mulps	%xmm14, %xmm13

	/* reduction: compute r,n.  */
	movdqu	COMMON_DATA(_IBrkValue)(%rip), %xmm9
	addps	%xmm13, %xmm14

	/* * Compute R * (VHi + VLo) * (1 + E + E^2) * = R *  (VHi +
	   VLo) * (1 + D) * = QHi + (QHi * D + QLo + QLo * D).  */
	movaps	%xmm14, %xmm2
	mulps	%xmm7, %xmm14
	mulps	%xmm11, %xmm2
	addps	%xmm14, %xmm7
	movdqu	COMMON_DATA(_NotiOffExpoMask)(%rip), %xmm12
	movaps	%xmm4, %xmm14

	/* Record the sign for eventual reincorporation.  */
	addps	%xmm7, %xmm2


	/* * Now finally accumulate the high and low parts of the *
	   argument to log1p, H + L, with a final compensated
	   summation.  */
	movaps	%xmm2, %xmm6
	andnps	%xmm5, %xmm1
	movaps	%xmm4, %xmm7
	/* Or the sign bit in with the tiny result to handle atanh(-0)
	   correctly.  */
	addps	%xmm11, %xmm6
	maxps	%xmm6, %xmm7
	minps	%xmm6, %xmm8
	subps	%xmm6, %xmm11
	movaps	%xmm7, %xmm10
	addps	%xmm8, %xmm10
	addps	%xmm11, %xmm2
	subps	%xmm10, %xmm7
	psubd	%xmm9, %xmm10
	addps	%xmm8, %xmm7
	pandn	%xmm10, %xmm12
	psrad	$23, %xmm10
	cvtdq2ps %xmm10, %xmm13
	addps	%xmm7, %xmm2

	/* final reconstruction.  */
	pslld	$23, %xmm10
	paddd	%xmm9, %xmm12
	psubd	%xmm10, %xmm14

	/* polynomial evaluation.  */
	subps	%xmm4, %xmm12
	mulps	%xmm14, %xmm2
	movups	LOCAL_DATA(_Poly_1)(%rip), %xmm7
	addps	%xmm12, %xmm2
	mulps	%xmm2, %xmm7


	/* Finally, halve the result and reincorporate the sign.  */
	addps	LOCAL_DATA(_Poly_2)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	addps	LOCAL_DATA(_Poly_3)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	addps	LOCAL_DATA(_Poly_4)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	addps	LOCAL_DATA(_Poly_5)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	addps	LOCAL_DATA(_Poly_6)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	addps	LOCAL_DATA(_Poly_7)(%rip), %xmm7
	mulps	%xmm2, %xmm7
	movaps	COMMON_DATA(_Neg5F)(%rip), %xmm6
	addps	%xmm6, %xmm7
	mulps	%xmm2, %xmm7
	mulps	%xmm2, %xmm7
	mulps	COMMON_DATA(_Ln2)(%rip), %xmm13
	/* We can build `sHalf` with `_Poly & sOne`.  */
	andps	%xmm4, %xmm6
	orps	%xmm1, %xmm3
	xorps	%xmm6, %xmm1

	addps	%xmm2, %xmm7
	addps	%xmm13, %xmm7
	mulps	%xmm7, %xmm1

	/* Finish check of NaNs.  */
	cmpleps	%xmm0, %xmm4
	movmskps %xmm4, %edx
	cmpltps	LOCAL_DATA(_TinyRange)(%rip), %xmm0

	andps	%xmm0, %xmm3
	andnps	%xmm1, %xmm0
	orps	%xmm3, %xmm0

	testl	%edx, %edx
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)

	/* No registers to restore on fast path.  */
	ret


	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a atanhf call. Optimize for code size
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):

	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	subq	$56, %rsp
	cfi_def_cfa_offset (64)
	movups	%xmm0, 24(%rsp)
	movups	%xmm5, 40(%rsp)

	/* Use rbx/rbp for callee save registers as they get short
	   encoding for many instructions (as compared with r12/r13).
	 */
	movq	%rbx, (%rsp)
	cfi_offset (rbx, -64)
	movq	%rbp, 8(%rsp)
	cfi_offset (rbp, -56)
	/* edx has 1s where there was a special value that needs to be
	   handled by a tanhf call.  */
	movl	%edx, %ebx
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
	call	atanhf@PLT
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
END(atanhf_4_sse4_dev)

	.section .rodata.sse4, "a"
	.align	16

LOCAL_DATA_NAME:
	/* _Poly[] = SP polynomial.  */
	/* 1.3820238411426544189453125e-01 P7.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_1, 0x3e0d84ed)
	/* -1.5122179687023162841796875e-01 P6.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_2, 0xbe1ad9e3)
	/* 1.4042308926582336425781250e-01 P5.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_3, 0x3e0fcb12)
	/* -1.6472326219081878662109375e-01 P4.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_4, 0xbe28ad37)
	/* 2.0007920265197753906250000e-01 P3.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_5, 0x3e4ce190)
	/* -2.5004237890243530273437500e-01 P2.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_6, 0xbe80058e)
	/* 3.3333265781402587890625000e-01 P1.  */
	DATA_VEC (LOCAL_DATA_NAME, _Poly_7, 0x3eaaaa94)
	DATA_VEC (LOCAL_DATA_NAME, _TinyRange, 0x0C000000)
	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
