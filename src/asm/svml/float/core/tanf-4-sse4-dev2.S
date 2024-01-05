/* Core .*/
/* Function tanf vectorized with SSE4.
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
 *      1) Range reduction to [-Pi/4; +Pi/4] interval
 *         a) Grab sign from source argument and save it.
 *         b) Remove sign using AND 0x7fffffff operation
 *         c) Getting octant Y by 2/Pi multiplication
 *         d) Add "Right Shifter" (0x4B000000) value
 *         e) Treat obtained value as integer for destination sign setting.
 *            Shift first bit of this value to the last (sign) position (S << 31)
 *         f) Change destination sign if source sign is negative
 *            using XOR operation.
 *         g) Subtract "Right Shifter" (0x4B000000) value
 *         h) Subtract Y*(PI/2) from X argument, where PI/2 divided to 4 parts:
 *            X = X - Y*PI1 - Y*PI2 - Y*PI3 - Y*PI4;
 *      2) Rational polynomial approximation ( at [-Pi/4; +Pi/4] interval)
 *         a) Calculate X^2 = X * X
 *         b) Calculate 2 polynomials:
 *            P = X * (P0 + X^2 * P1);
 *            Q = Q0 + X^2 * (Q1 + x^2 * Q2);
 *         c) Swap P and Q if first bit of obtained value after
 *            Right Shifting is set to 1. Using And, Andnot & Or operations.
 *         d) Divide R = P / Q;
 *      3) Destination sign setting
 *         a) Set shifted destination sign using XOR operation:
 *            R = XOR( R, S );
 *
 */

/* Offsets for data table __svml_stan_data_internal
 */
#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_stan_data_internal
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-sse4-rodata-offsets.h"

#define AVX2_SHARED_OFFSETS
#define AVX512_SHARED_OFFSETS
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-tanf-rodata.S"

#define _sPI1	0
#define _sPI2	16
#define _sPI3	32
#define _sPI4	48
#define _sRangeVal	64
#define _FLT_0	80
#define _FLT_1	96


#define xmmA	xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(tanf_4_sse4_dev)
	movaps	%xmm0, %xmm15
	movups	COMMON_DATA(_AbsMask)(%rip), %xmm4

	andps	%xmm0, %xmm4

	movups	AVX2_SHARED_DATA(_sInvPi)(%rip), %xmm0
	mulps	%xmm4, %xmm0

	/* Range reduction.  */
	movaps	%xmm4, %xmmA

	/* 
	   Main path (_LA_ and _EP_)
	   
	   Octant calculation.  */
	movups	AVX2_SHARED_DATA(_sRShifter)(%rip), %xmm3

	/* Large values check.  */
	movups	LOCAL_DATA(_sPI1)(%rip), %xmm5
	movups	LOCAL_DATA(_sPI2)(%rip), %xmm6
	addps	%xmm3, %xmm0
	movaps	%xmm0, %xmm2
	movups	LOCAL_DATA(_sPI3)(%rip), %xmm7
	subps	%xmm3, %xmm2

	mulps	%xmm2, %xmm5
	mulps	%xmm2, %xmm6
	mulps	%xmm2, %xmm7

	subps	%xmm5, %xmmA
	mulps	LOCAL_DATA(_sPI4)(%rip), %xmm2
	subps	%xmm6, %xmmA
	movups	AVX2_SHARED_DATA(_sQ2)(%rip), %xmm6


	/* Rational approximation.  */
	movups	AVX2_SHARED_DATA(_sP1)(%rip), %xmm5

	/* Inversion mask and sign calculation.  */
	pslld	$31, %xmm0
	subps	%xmm7, %xmmA

	/* Exchanged numerator and denominator if necessary.  */
	subps	%xmm2, %xmmA
	movaps	%xmmA, %xmm3
	mulps	%xmmA, %xmmA
	mulps	%xmmA, %xmm6
	mulps	%xmmA, %xmm5
	addps	AVX2_SHARED_DATA(_sQ1)(%rip), %xmm6
	movups	AVX2_SHARED_DATA(_sP0)(%rip), %xmm2
	addps	%xmm2, %xmm5
	mulps	%xmm6, %xmmA
	mulps	%xmm5, %xmm3
	addps	%xmm2, %xmmA

	movaps	%xmm3, %xmm2
	blendvps %xmm0, %xmmA, %xmm3
	blendvps %xmm0, %xmm2, %xmmA

	/* Division.  */
	divps	%xmmA, %xmm3

	/* Sign setting.  */
	pxor	%xmm3, %xmm0

	movaps	%xmm4, %xmm3
	pcmpgtd	AVX2_SHARED_DATA(_sRangeReductionVal)(%rip), %xmm3
	pmovmskb %xmm3, %edx

	/* End of main path (_LA_ and _EP_).  */
	testl	%edx, %edx
	/* Go to auxilary branch.  */
	jne	L(AUX_BRANCH)

	/* Set sign.  */
	andnps	%xmm15, %xmm4
	pxor	%xmm4, %xmm0
	ret

L(AUX_BRANCH):
	movaps	%xmm3, %xmm14
	andnps	%xmm0, %xmm3

	/* Get the (2^a / 2pi) mod 1 values from the table.  */
	movaps	%xmm4, %xmmA
	psrld	$0x17, %xmm4
	/* Compute indices in xmm5 (need 4x scale).  */
	movaps	%xmm4, %xmm5
	paddd	%xmm4, %xmm4
	paddd	%xmm4, %xmm5

	pextrq	$0x1, %xmm5, %rcx
	movq	%xmm5, %rdx


	/* Move indices into GPRs.  */
	movl	%edx, %esi
	movl	%ecx, %edi
	shrq	$0x20, %rdx
	shrq	$0x20, %rcx

	lea	AVX512_SHARED_DATA(_Reduction)(%rip), %rax
	movq	0(%rax, %rcx, 4), %xmm4
	movq	0(%rax, %rdi, 4), %xmm5
	punpckldq %xmm4, %xmm5
	movq	0(%rax, %rsi, 4), %xmm4
	movq	0(%rax, %rdx, 4), %xmm2
	movaps	AVX2_SHARED_DATA(_Low16)(%rip), %xmm9
	punpckldq %xmm2, %xmm4
	movaps	%xmm4, %xmm2
	punpcklqdq %xmm5, %xmm4
	punpckhqdq %xmm5, %xmm2

	/* 
	   Break the P_xxx and m into 16-bit chunks ready for
	   the long multiplication via 16x16->32 multiplications.  */
	movaps	%xmm4, %xmm5
	pand	%xmm9, %xmm4
	psrld	$0x10, %xmm5
	movaps	%xmm4, %xmm6
	psrlq	$0x20, %xmm4
	movaps	COMMON_DATA(_NotiOffExpoMask)(%rip), %xmm8
	pandn	%xmmA, %xmm8
	/* 
	   Also get the significand as an integer
	   NB: adding in the integer bit is wrong for denorms!
	   To make this work for denorms we should do something
	   slightly different.  */
	movaps	LOCAL_DATA(_sRangeVal)(%rip), %xmm7
	paddd	%xmm7, %xmmA
	movmskps %xmmA, %r8d

	por	%xmm8, %xmm7

	pand	%xmm9, %xmm8
	movaps	%xmm8, %xmmA

	psrlq	$0x20, %xmm8
	movaps	%xmm8, %xmm10
	pmuludq	%xmm4, %xmm8
	psllq	$0x20, %xmm8
	movaps	%xmmA, %xmm11
	pmuludq	%xmm6, %xmmA
	blendps	$0xaa, %xmm8, %xmmA
	movaps	%xmmA, %xmm8
	psrld	$0x10, %xmmA
	pand	%xmm9, %xmm8
	movaps	%xmm7, %xmm13
	psrld	$0x10, %xmm7
	psrlq	$0x30, %xmm13
	pmuludq	%xmm7, %xmm6
	pmuludq	%xmm13, %xmm4
	psllq	$0x20, %xmm4
	blendps	$0xaa, %xmm4, %xmm6
	paddd	%xmmA, %xmm6
	movaps	%xmm5, %xmm4
	psrlq	$0x20, %xmm5
	pmuludq	%xmm11, %xmm4
	pmuludq	%xmm10, %xmm5
	psllq	$0x20, %xmm5
	blendps	$0xaa, %xmm5, %xmm4
	pand	%xmm9, %xmm4
	paddd	%xmm6, %xmm4
	movaps	%xmm2, %xmm5
	psrld	$0x10, %xmm2
	movaps	%xmm11, %xmm6
	pmuludq	%xmm2, %xmm11
	pmuludq	%xmm7, %xmm2
	movaps	%xmm5, %xmmA
	psrlq	$0x30, %xmm5
	pand	%xmm9, %xmmA
	movaps	%xmm10, %xmm12
	pmuludq	%xmm5, %xmm10
	psllq	$0x20, %xmm10
	blendps	$0xaa, %xmm10, %xmm11
	pmuludq	%xmm13, %xmm5
	psllq	$0x20, %xmm5
	blendps	$0xaa, %xmm5, %xmm2
	movaps	%xmm11, %xmm5
	pand	%xmm9, %xmm11
	psrld	$0x10, %xmm5
	paddd	%xmm5, %xmm2
	paddd	%xmm2, %xmm8
	movaps	%xmm6, %xmm5
	pmuludq	%xmmA, %xmm6
	movaps	%xmmA, %xmm2
	psrlq	$0x20, %xmmA
	pmuludq	%xmm7, %xmm2
	movaps	%xmm12, %xmm10
	pmuludq	%xmmA, %xmm12
	psllq	$0x20, %xmm12
	pmuludq	%xmm13, %xmmA
	psllq	$0x20, %xmmA
	blendps	$0xaa, %xmmA, %xmm2
	blendps	$0xaa, %xmm12, %xmm6
	movaps	%xmm6, %xmmA
	psrld	$0x10, %xmm6
	pand	%xmm9, %xmmA
	paddd	%xmm6, %xmm2
	paddd	%xmm2, %xmm11
	movd	8(%rax, %rcx, 4), %xmm2
	movd	8(%rax, %rdi, 4), %xmm6
	punpckldq %xmm2, %xmm6
	movd	8(%rax, %rdx, 4), %xmm2
	movd	8(%rax, %rsi, 4), %xmm12
	punpckldq %xmm2, %xmm12
	punpcklqdq %xmm6, %xmm12
	movaps	%xmm12, %xmm2
	psrld	$0x10, %xmm12
	pmuludq	%xmm12, %xmm5
	pmuludq	%xmm7, %xmm12
	movaps	%xmm2, %xmm6
	psrlq	$0x30, %xmm2
	pand	%xmm9, %xmm6
	pmuludq	%xmm6, %xmm7
	psrlq	$0x20, %xmm6
	pmuludq	%xmm13, %xmm6
	psllq	$0x20, %xmm6
	blendps	$0xaa, %xmm6, %xmm7
	psrld	$0x10, %xmm7
	pmuludq	%xmm2, %xmm13
	pmuludq	%xmm10, %xmm2
	psllq	$0x20, %xmm2
	psllq	$0x20, %xmm13
	blendps	$0xaa, %xmm2, %xmm5
	psrld	$0x10, %xmm5
	blendps	$0xaa, %xmm13, %xmm12
	paddd	%xmm5, %xmm12
	paddd	%xmm12, %xmmA
	paddd	%xmmA, %xmm7
	movaps	%xmm7, %xmm5
	psrld	$0x10, %xmm7
	pand	%xmm9, %xmm5
	paddd	%xmm11, %xmm7
	movaps	%xmm7, %xmm2
	psrld	$0x10, %xmm7
	paddd	%xmm8, %xmm7
	pslld	$0x10, %xmm2
	paddd	%xmm5, %xmm2
	pand	%xmm7, %xmm9
	psrld	$0x10, %xmm7
	paddd	%xmm4, %xmm7
	pslld	$0x10, %xmm7
	paddd	%xmm9, %xmm7
	movaps	%xmm7, %xmm4
	/* Assemble reduced argument from the pieces.  */
	psrld	$0x9, %xmm7
	/* Create floating-point high part, implicitly adding integer
	   bit 1
	   Incorporate overall sign at this stage too.  */
	por	COMMON_DATA(_OneF)(%rip), %xmm7
	movaps	AVX2_SHARED_DATA(_SH_FLT_1)(%rip), %xmm9
	movaps	%xmm7, %xmm5
	addps	%xmm9, %xmm7
	movaps	%xmm7, %xmm6
	subps	%xmm9, %xmm7
	/* Grab our final N value as an integer, appropriately masked
	   mod 2^8.  */
	subps	%xmm7, %xmm5

	movaps	%xmm2, %xmm9
	psrld	$0x12, %xmm2
	movaps	AVX2_SHARED_DATA(_Low9)(%rip), %xmm7
	pand	%xmm4, %xmm7
	pslld	$0xe, %xmm7
	por	%xmm2, %xmm7
	movaps	AVX2_SHARED_DATA(_SH_FLT_3)(%rip), %xmm4
	por	%xmm4, %xmm7
	subps	%xmm4, %xmm7
	movaps	%xmm5, %xmm4
	addps	%xmm7, %xmm5

	/* Split RHi into 12-bit leading and trailing parts.  */
	movaps	COMMON_DATA(_Neg4096)(%rip), %xmm0
	subps	%xmm5, %xmm4
	addps	%xmm4, %xmm7
	movaps	%xmm0, %xmm4
	andps	%xmm5, %xmm0
	subps	%xmm0, %xmm5
	/* 
	   Do the multiplication as exact top part and "naive" low.  */
	movaps	LOCAL_DATA(_FLT_0)(%rip), %xmm2
	movaps	%xmm2, %xmm8
	mulps	%xmm5, %xmm2
	movaps	AVX2_SHARED_DATA(_Low18)(%rip), %xmm10

	mulps	%xmm0, %xmm8


	pand	%xmm9, %xmm10
	pslld	$0x5, %xmm10
	movaps	AVX2_SHARED_DATA(_SH_FLT_2)(%rip), %xmmA

	/* If the magnitude of the input is <= 2^-20, then
	   just pass through the input, since no reduction will be needed and
	   the main path will only work accurately if the reduced argument is
	   about >= 2^-40 (which it is for all large pi multiples).  */

	por	%xmmA, %xmm10
	subps	%xmmA, %xmm10
	addps	%xmm7, %xmm10

	/* Now multiply those numbers all by 2 pi, reasonably accurately.
	   The top part uses 2pi = s2pi_lead + s2pi_trail, where
	   s2pi_lead has 12 significant bits.  */
	movaps	AVX2_SHARED_DATA(_SH_FLT_4)(%rip), %xmm9
	mulps	%xmm10, %xmm9
	addps	%xmm2, %xmm9
	/* Now add them up into 2 reasonably aligned pieces.  */
	movaps	LOCAL_DATA(_FLT_1)(%rip), %xmm7
	mulps	%xmm7, %xmm0
	mulps	%xmm5, %xmm7
	addps	%xmm8, %xmm7
	addps	%xmm9, %xmm7
	addps	%xmm7, %xmm0
	lea	AVX2_SHARED_DATA(_Coeffs)(%rip), %rax

	/* The output is _VRES_R (high) + _VRES_E (low), and the integer
	   part is _VRES_IND Set sRp2 = _VRES_R^2 and then resume the
	   original code. Argument reduction is now finished: x = n *
	   pi/128 + r where n = iIndex and r = sR (high) + sE (low).
	   But we have n modulo 256, needed for sin/cos with period 2pi
	   but we want it modulo 128 since tan has period pi.  */
	pand	AVX2_SHARED_DATA(_Low7)(%rip), %xmm6
	movaps	%xmm6, %xmm9
	/* 
	   Simply combine the two parts of the reduced argument
	   since we can afford a few ulps in this case.  */
	pslld	$0x2, %xmm6
	paddd	%xmm9, %xmm6
	movq	%xmm6, %rcx
	movl	%ecx, %edx
	shrq	$0x20, %rcx
	pextrq	$0x1, %xmm6, %rsi
	movl	%esi, %edi
	shrq	$0x20, %rsi
	movups	16(%rax, %rcx, 8), %xmm9
	movups	16(%rax, %rdx, 8), %xmm7
	movaps	%xmm7, %xmm5
	punpckhdq %xmm9, %xmm7
	punpckldq %xmm9, %xmm5
	movups	16(%rax, %rsi, 8), %xmm9
	movups	16(%rax, %rdi, 8), %xmm2
	movaps	%xmm2, %xmm6
	punpckhdq %xmm9, %xmm2
	punpckldq %xmm9, %xmm6
	movaps	%xmm7, %xmm9
	punpckhqdq %xmm2, %xmm7
	punpcklqdq %xmm2, %xmm9

	/* Higher polynomial terms
	   Stage 1 (with unlimited parallelism)
	   P3 = C1_lo + C2 * Z.  */
	mulps	%xmm0, %xmm7
	addps	%xmm7, %xmm9
	movq	32(%rax, %rsi, 8), %xmm7
	movq	32(%rax, %rdi, 8), %xmm2
	punpckldq %xmm7, %xmm2
	movq	32(%rax, %rcx, 8), %xmm7
	movq	32(%rax, %rdx, 8), %xmm8
	punpckldq %xmm7, %xmm8
	movaps	%xmm8, %xmm7
	punpckhqdq %xmm2, %xmm8
	punpcklqdq %xmm2, %xmm7
	mulps	%xmm0, %xmm8
	addps	%xmm8, %xmm7
	movaps	%xmm0, %xmm2
	mulps	%xmm0, %xmm0

	mulps	%xmm0, %xmm7
	addps	%xmm7, %xmm9
	/* Final accumulation of low part.  */
	mulps	%xmm2, %xmm9
	movups	0(%rax, %rsi, 8), %xmm0
	movups	0(%rax, %rdi, 8), %xmm7
	movaps	%xmm7, %xmm8
	punpckldq %xmm0, %xmm7
	punpckhdq %xmm0, %xmm8
	movups	0(%rax, %rcx, 8), %xmm0
	movups	0(%rax, %rdx, 8), %xmmA
	movaps	%xmmA, %xmm10
	punpckldq %xmm0, %xmmA
	punpckhdq %xmm0, %xmm10
	movaps	%xmmA, %xmm0
	punpcklqdq %xmm7, %xmmA
	punpckhqdq %xmm7, %xmm0

	/* Compute 2-part reciprocal component Construct a separate
	   reduced argument modulo pi near pi/2 multiples. i.e. (pi/2 -
	   x) mod pi, simply by subtracting the reduced argument from
	   an accurate B_hi + B_lo = (128 - n) pi/128. Force the upper
	   part of this reduced argument to half-length to simplify
	   accurate reciprocation later on.  */
	subps	%xmm2, %xmmA
	movaps	%xmm4, %xmm7
	andps	%xmmA, %xmm4
	subps	%xmm4, %xmmA
	addps	%xmmA, %xmm0

	/* Now compute an approximate reciprocal to mix into the computation
	   To avoid any danger of nonportability, force it to 12 bits,
	   though I suspect it always is anyway on current platforms.  */
	rcpps	%xmm4, %xmmA
	andps	%xmm7, %xmmA
	mulps	%xmmA, %xmm4
	movaps	%xmm10, %xmm7
	punpcklqdq %xmm8, %xmm10
	punpckhqdq %xmm8, %xmm7
	movaps	%xmmA, %xmm8
	/* Finally, multiplex both parts so they are only used in
	   cotangent path.  */
	mulps	%xmm10, %xmmA
	movaps	%xmm5, %xmm11
	punpckhqdq %xmm6, %xmm5
	punpcklqdq %xmm6, %xmm11

	/* Compensated sum of dominant component(s) Compute C0_hi +
	   C1_hi * Z + Recip_hi + Recip_lo = H4 (hi) + H9 (lo) H1 =
	   C1_hi * Z (exact since C1_hi is 1 bit).  */
	mulps	%xmm2, %xmm5
	movaps	%xmm7, %xmm2
	/* H2 = high(C0_hi + C1_hi * Z).  */
	addps	%xmm5, %xmm7
	/* H4 = high(H2 + Recip_hi).  */

	subps	%xmm7, %xmm2
	/* H5 = low(C0_hi + C1_hi * Z).  */
	addps	%xmm2, %xmm5
	movaps	%xmm7, %xmm2
	addps	%xmmA, %xmm7

	/* intermediate in compensated sum.  */
	subps	%xmm7, %xmmA
	/* H8 = low(H2 + Recip_hi).  */
	addps	%xmmA, %xmm2

	/* Get a better approximation to  1/sR_hi (not far short of an ulp)
	   using a third-order polynomial approximation.  */
	movups	COMMON_DATA(_OneF)(%rip), %xmm6
	movaps	%xmm6, %xmmA
	subps	%xmm4, %xmm6
	movaps	%xmm6, %xmm4
	mulps	%xmm6, %xmm6
	addps	%xmm6, %xmmA
	movaps	%xmm8, %xmm6
	mulps	%xmm4, %xmm8
	addps	%xmm6, %xmm8
	mulps	%xmmA, %xmm8

	/* Multiply by sRecip_ok to make sR_lo relative to sR_hi Since
	   sR_lo is shifted off by about 12 bits, this is accurate
	   enough.  */
	mulps	%xmm8, %xmm0
	movaps	%xmm0, %xmm6
	subps	%xmm4, %xmm0

	/* Now create a low reciprocal using
	   (Recip_hi + Er * Recip_ok) * (1 + sR_lo^2 - sR_lo)
	   =~= Recip_hi + Recip_ok * (Er + sR_lo^2 - sR_lo).  */
	mulps	%xmm6, %xmm6
	/* P4 = C3 + C4 * Z.  */
	subps	%xmm0, %xmm6
	mulps	%xmm6, %xmm8
	mulps	%xmm8, %xmm10
	/* H7 = low(C0_hi + C1_hi * Z) + Recip_lo.  */
	addps	%xmm5, %xmm10
	/* Z2 = Z^2.  */

	/* Now H4 + H9 should be that part.  */
	addps	%xmm2, %xmm10
	/* P9 = trail(dominant part) + C0_lo.  */
	addps	%xmm10, %xmm11
	/* Merge results from main and large paths:.  */
	addps	%xmm9, %xmm11
	addps	%xmm7, %xmm11
	/* And now the very final summation.  */
	andps	%xmm14, %xmm11

	/* The end of implementation (LA with huge args reduction)
	   End of large arguments path (_HA_, _LA_ and _EP_).  */
	orps	%xmm3, %xmm11
	movups	COMMON_DATA(_AbsMask)(%rip), %xmm3
	andnps	%xmm15, %xmm3

	/* Incorperate original sign.  */
	xorps	%xmm3, %xmm11
	/* Return to main vector processing path.  */
	testl	%r8d, %r8d
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)
	movaps	%xmm11, %xmm0
	ret


	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a tanhf call. Optimize for code size
	   more so than speed here.  */
L(SPECIAL_VALUES_BRANCH):

	/* Stack coming in 16-byte aligned. Set 8-byte misaligned so on
	   call entry will be 16-byte aligned.  */
	subq	$56, %rsp
	cfi_def_cfa_offset (64)
	movups	%xmm11, 24(%rsp)
	movups	%xmm15, 40(%rsp)

	/* Use rbx/rbp for callee save registers as they get short
	   encoding for many instructions (as compared with r12/r13).  */
	movq	%rbx, (%rsp)
	cfi_offset (rbx, -64)
	movq	%rbp, 8(%rsp)
	cfi_offset (rbp, -56)
	/* r8d has 1s where there was a special value that needs to be
	   handled by a tanf call.  */
	movl	%r8d, %ebx
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
	call	tanf@PLT
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


END(tanf_4_sse4_dev)

	.section .rodata, "a"
	.align	16

LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _sPI1, 0x3FC90000)
	DATA_VEC (LOCAL_DATA_NAME, _sPI2, 0x39FDA000)
	DATA_VEC (LOCAL_DATA_NAME, _sPI3, 0x33A22000)
	DATA_VEC (LOCAL_DATA_NAME, _sPI4, 0x2C34611A)
	DATA_VEC (LOCAL_DATA_NAME, _sRangeVal, 0x00800000)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_0, 0xb795777a)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_1, 0x40c91000)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
