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

#define _sAbsMask	0
#define _sInvPi	16
#define _sRShifter	32
#define _sPI1	48
#define _sPI2	64
#define _sPI3	80
#define _sPI4	96
#define _sQ2	112
#define _sP1	128
#define _sQ1	144
#define _sP0	160
#define _sRangeReductionVal	176
#define _sRangeReductionVal_uisa	192
#define _sRangeVal	208
#define _sOne	224
#define _sCoeffs	240

#define xmmA	xmm1

	.section .text.sse4, "ax", @progbits
ENTRY(tanf_4_sse4_dev)
	movaps	%xmm0, %xmm15
	movups	_sAbsMask + __svml_stan_data_internal(%rip), %xmm4

	andps	%xmm0, %xmm4

	movups	_sInvPi + __svml_stan_data_internal(%rip), %xmm0
	mulps	%xmm4, %xmm0

	/* Range reduction.  */
	movaps	%xmm4, %xmmA

	/*
	 * Main path (_LA_ and _EP_)
	 *
	 * Octant calculation
	 */
	movups	_sRShifter + __svml_stan_data_internal(%rip), %xmm3

	/* Large values check.  */
	movups	_sPI1 + __svml_stan_data_internal(%rip), %xmm5
	movups	_sPI2 + __svml_stan_data_internal(%rip), %xmm6
	addps	%xmm3, %xmm0
	movaps	%xmm0, %xmm2
	movups	_sPI3 + __svml_stan_data_internal(%rip), %xmm7
	subps	%xmm3, %xmm2

	mulps	%xmm2, %xmm5
	mulps	%xmm2, %xmm6
	mulps	%xmm2, %xmm7

	subps	%xmm5, %xmmA
	mulps	_sPI4 + __svml_stan_data_internal(%rip), %xmm2
	subps	%xmm6, %xmmA
	movups	_sQ2 + __svml_stan_data_internal(%rip), %xmm6


	/* Rational approximation.  */
	movups	_sP1 + __svml_stan_data_internal(%rip), %xmm5

	/* Inversion mask and sign calculation.  */
	pslld	$31, %xmm0
	subps	%xmm7, %xmmA

	/* Exchanged numerator and denominator if necessary.  */
	subps	%xmm2, %xmmA
	movaps	%xmmA, %xmm3
	mulps	%xmmA, %xmmA
	mulps	%xmmA, %xmm6
	mulps	%xmmA, %xmm5
	addps	_sQ1 + __svml_stan_data_internal(%rip), %xmm6
	movups	_sP0 + __svml_stan_data_internal(%rip), %xmm2
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
	pcmpgtd	_sRangeReductionVal + __svml_stan_data_internal(%rip), %xmm3
	pmovmskb %xmm3, %edx

	/*
	 * End of main path (_LA_ and _EP_)
	 */
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

	/*
	 * Get the (2^a / 2pi) mod 1 values from the table.
	 * Because doesn't have I-type gather, we need a trivial cast
	 */
	movaps	%xmm4, %xmmA
	psrld	$0x17, %xmm4
	/* Compute indices in xmm5 (need 4x scale).  */
	movaps	%xmm4, %xmm5
	paddd	%xmm4, %xmm4
	paddd	%xmm4, %xmm5

	pextrq	$0x1, %xmm5, %rcx
	movq	%xmm5, %rdx
	/* Broadcast to xmm9 before eax is in use. Use for 16x16 -> 32
	   mults.  */
	movl	$0xffff, %eax
	movd	%eax, %xmm9

	/* Move indices into GPRs.  */
	movl	%edx, %esi
	movl	%ecx, %edi
	shrq	$0x20, %rdx
	shrq	$0x20, %rcx

	lea	__svml_stan_reduction_data_internal(%rip), %rax
	movq	0(%rax, %rcx, 4), %xmm4
	movq	0(%rax, %rdi, 4), %xmm5
	punpckldq %xmm4, %xmm5
	movq	0(%rax, %rsi, 4), %xmm4
	movq	0(%rax, %rdx, 4), %xmm2
	punpckldq %xmm2, %xmm4
	movaps	%xmm4, %xmm2
	punpcklqdq %xmm5, %xmm4
	punpckhqdq %xmm5, %xmm2

	pshufd	$0x0, %xmm9, %xmm9
	/*
	 * Break the P_xxx and m into 16-bit chunks ready for
	 * the long multiplication via 16x16->32 multiplications
	 */
	movaps	%xmm4, %xmm5
	pand	%xmm9, %xmm4
	psrld	$0x10, %xmm5
	movaps	%xmm4, %xmm6
	psrlq	$0x20, %xmm4
	movaps	_sRangeReductionVal_uisa + __svml_stan_data_internal(%rip), %xmm8
	pand	%xmmA, %xmm8
	/*
	 * Also get the significand as an integer
	 * NB: adding in the integer bit is wrong for denorms!
	 * To make this work for denorms we should do something slightly different
	 */
	movaps	_sRangeVal + __svml_stan_data_internal(%rip), %xmm7
	por	%xmm7, %xmm8
	paddd	%xmmA, %xmm7
	movmskps %xmm7, %r8d
	movaps	%xmm8, %xmmA
	pand	%xmm9, %xmm8
	movaps	%xmm8, %xmm7
	psrlq	$0x20, %xmm8
	movaps	%xmm8, %xmm10
	pmuludq	%xmm4, %xmm8
	psllq	$0x20, %xmm8
	movaps	%xmm7, %xmm11
	pmuludq	%xmm6, %xmm7
	blendps	$0xaa, %xmm8, %xmm7
	movaps	%xmm7, %xmm8
	psrld	$0x10, %xmm7
	pand	%xmm9, %xmm8
	movaps	%xmmA, %xmm13
	psrld	$0x10, %xmmA
	psrlq	$0x30, %xmm13
	pmuludq	%xmmA, %xmm6
	pmuludq	%xmm13, %xmm4
	psllq	$0x20, %xmm4
	blendps	$0xaa, %xmm4, %xmm6
	paddd	%xmm7, %xmm6
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
	pmuludq	%xmmA, %xmm2
	movaps	%xmm5, %xmm7
	psrlq	$0x30, %xmm5
	pand	%xmm9, %xmm7
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
	pmuludq	%xmm7, %xmm6
	movaps	%xmm7, %xmm2
	psrlq	$0x20, %xmm7
	pmuludq	%xmmA, %xmm2
	movaps	%xmm12, %xmm10
	pmuludq	%xmm7, %xmm12
	psllq	$0x20, %xmm12
	pmuludq	%xmm13, %xmm7
	psllq	$0x20, %xmm7
	blendps	$0xaa, %xmm7, %xmm2
	blendps	$0xaa, %xmm12, %xmm6
	movaps	%xmm6, %xmm7
	psrld	$0x10, %xmm6
	pand	%xmm9, %xmm7
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
	pmuludq	%xmmA, %xmm12
	movaps	%xmm2, %xmm6
	psrlq	$0x30, %xmm2
	pand	%xmm9, %xmm6
	pmuludq	%xmm6, %xmmA
	psrlq	$0x20, %xmm6
	pmuludq	%xmm13, %xmm6
	psllq	$0x20, %xmm6
	blendps	$0xaa, %xmm6, %xmmA
	psrld	$0x10, %xmmA
	pmuludq	%xmm2, %xmm13
	pmuludq	%xmm10, %xmm2
	psllq	$0x20, %xmm2
	psllq	$0x20, %xmm13
	blendps	$0xaa, %xmm2, %xmm5
	psrld	$0x10, %xmm5
	blendps	$0xaa, %xmm13, %xmm12
	paddd	%xmm5, %xmm12
	paddd	%xmm12, %xmm7
	paddd	%xmm7, %xmmA
	movaps	%xmmA, %xmm5
	psrld	$0x10, %xmmA
	pand	%xmm9, %xmm5
	paddd	%xmm11, %xmmA
	movaps	%xmmA, %xmm2
	psrld	$0x10, %xmmA
	paddd	%xmm8, %xmmA
	pslld	$0x10, %xmm2
	paddd	%xmm5, %xmm2
	pand	%xmmA, %xmm9
	psrld	$0x10, %xmmA
	paddd	%xmm4, %xmmA
	pslld	$0x10, %xmmA
	paddd	%xmm9, %xmmA
	movaps	%xmmA, %xmm4
	/* Assemble reduced argument from the pieces.  */
	psrld	$0x9, %xmmA
	/*
	 * Create floating-point high part, implicitly adding integer bit 1
	 * Incorporate overall sign at this stage too.
	 */
	por	_sOne + __svml_stan_data_internal(%rip), %xmmA
	movl	$0x47400000, %eax
	movd	%eax, %xmm9
	pshufd	$0x0, %xmm9, %xmm9
	movaps	%xmmA, %xmm5
	addps	%xmm9, %xmmA
	movaps	%xmmA, %xmm6
	subps	%xmm9, %xmmA
	/* Grab our final N value as an integer, appropriately masked
	   mod 2^8.  */
	subps	%xmmA, %xmm5
	movaps	%xmm2, %xmm9
	psrld	$0x12, %xmm2
	movl	$0x1ff, %eax
	movd	%eax, %xmmA
	pshufd	$0x0, %xmmA, %xmmA
	pand	%xmm4, %xmmA
	pslld	$0xe, %xmmA
	por	%xmm2, %xmmA
	movl	$0x34000000, %eax
	movd	%eax, %xmm4
	pshufd	$0x0, %xmm4, %xmm4
	por	%xmm4, %xmmA
	subps	%xmm4, %xmmA
	movaps	%xmm5, %xmm4
	addps	%xmmA, %xmm5

	/* Split RHi into 12-bit leading and trailing parts. */
	movl	$0xfffff000, %eax
	movd	%eax, %xmm0
	pshufd	$0x0, %xmm0, %xmm0
	subps	%xmm5, %xmm4
	addps	%xmm4, %xmmA
	movaps	%xmm0, %xmm4
	andps	%xmm5, %xmm0
	subps	%xmm0, %xmm5
	/*
	 * Do the multiplication as exact top part and "naive" low.
	 */
	movl	$0xb795777a, %eax
	movd	%eax, %xmm2
	pshufd	$0x0, %xmm2, %xmm2
	movaps	%xmm2, %xmm8
	mulps	%xmm5, %xmm2
	movl	$0x3ffff, %eax
	movd	%eax, %xmm10

	mulps	%xmm0, %xmm8
	movl	$0x28800000, %eax
	movd	%eax, %xmm7


	pshufd	$0x0, %xmm10, %xmm10
	pand	%xmm9, %xmm10
	pslld	$0x5, %xmm10

	pshufd	$0x0, %xmm7, %xmm7

	/*
	 * If the magnitude of the input is <= 2^-20, then
	 * just pass through the input, since no reduction will be needed and
	 * the main path will only work accurately if the reduced argument is
	 * about >= 2^-40 (which it is for all large pi multiples)
	 */
	por	%xmm7, %xmm10
	subps	%xmm7, %xmm10
	addps	%xmmA, %xmm10
	/*
	 * Now multiply those numbers all by 2 pi, reasonably accurately.
	 * The top part uses 2pi = s2pi_lead + s2pi_trail, where
	 * s2pi_lead has 12 significant bits.
	 */

	movl	$0x40c90fdb, %eax
	movd	%eax, %xmm9
	pshufd	$0x0, %xmm9, %xmm9
	mulps	%xmm10, %xmm9
	addps	%xmm2, %xmm9
	/* Now add them up into 2 reasonably aligned pieces */
	movl	$0x40c91000, %eax
	movd	%eax, %xmmA
	pshufd	$0x0, %xmmA, %xmmA
	mulps	%xmmA, %xmm0
	mulps	%xmm5, %xmmA
	addps	%xmm8, %xmmA
	addps	%xmm9, %xmmA
	addps	%xmmA, %xmm0
	lea	_sCoeffs + __svml_stan_data_internal(%rip), %rax
	/*
	 * The output is _VRES_R (high) + _VRES_E (low), and the integer part is _VRES_IND
	 * Set sRp2 = _VRES_R^2 and then resume the original code.
	 * Argument reduction is now finished: x = n * pi/128 + r
	 * where n = iIndex and r = sR (high) + sE (low).
	 * But we have n modulo 256, needed for sin/cos with period 2pi
	 * but we want it modulo 128 since tan has period pi.
	 */
	movl	$0x7f, %ecx
	movd	%ecx, %xmm9
	pshufd	$0x0, %xmm9, %xmm9
	pand	%xmm9, %xmm6
	movaps	%xmm6, %xmm9
	/*
	 * Simply combine the two parts of the reduced argument
	 * since we can afford a few ulps in this case.
	 */
	pslld	$0x2, %xmm6
	paddd	%xmm9, %xmm6
	movq	%xmm6, %rcx
	movl	%ecx, %edx
	shrq	$0x20, %rcx
	pextrq	$0x1, %xmm6, %rsi
	movl	%esi, %edi
	shrq	$0x20, %rsi
	movups	16(%rax, %rcx, 8), %xmm9
	movups	16(%rax, %rdx, 8), %xmmA
	movaps	%xmmA, %xmm5
	punpckhdq %xmm9, %xmmA
	punpckldq %xmm9, %xmm5
	movups	16(%rax, %rsi, 8), %xmm9
	movups	16(%rax, %rdi, 8), %xmm2
	movaps	%xmm2, %xmm6
	punpckhdq %xmm9, %xmm2
	punpckldq %xmm9, %xmm6
	movaps	%xmmA, %xmm9
	punpckhqdq %xmm2, %xmmA
	punpcklqdq %xmm2, %xmm9
	/* *  Higher polynomial terms * Stage 1 (with unlimited
	   parallelism) * P3 = C1_lo + C2 * Z.  */
	mulps	%xmm0, %xmmA
	addps	%xmmA, %xmm9
	movq	32(%rax, %rsi, 8), %xmmA
	movq	32(%rax, %rdi, 8), %xmm2
	punpckldq %xmmA, %xmm2
	movq	32(%rax, %rcx, 8), %xmmA
	movq	32(%rax, %rdx, 8), %xmm8
	punpckldq %xmmA, %xmm8
	movaps	%xmm8, %xmmA
	punpckhqdq %xmm2, %xmm8
	punpcklqdq %xmm2, %xmmA
	mulps	%xmm0, %xmm8
	addps	%xmm8, %xmmA
	movaps	%xmm0, %xmm2
	mulps	%xmm0, %xmm0
	/*
	 *  Higher polynomial terms
	 * Stage 1 (with unlimited parallelism)
	 * P3 = C1_lo + C2 * Z
	 */
	mulps	%xmm0, %xmmA
	addps	%xmmA, %xmm9
	/* Final accumulation of low part.  */
	mulps	%xmm2, %xmm9
	movups	0(%rax, %rsi, 8), %xmm0
	movups	0(%rax, %rdi, 8), %xmmA
	movaps	%xmmA, %xmm8
	punpckldq %xmm0, %xmmA
	punpckhdq %xmm0, %xmm8
	movups	0(%rax, %rcx, 8), %xmm0
	movups	0(%rax, %rdx, 8), %xmm7
	movaps	%xmm7, %xmm10
	punpckldq %xmm0, %xmm7
	punpckhdq %xmm0, %xmm10
	movaps	%xmm7, %xmm0
	punpcklqdq %xmmA, %xmm7
	punpckhqdq %xmmA, %xmm0
	/*
	 *  Compute 2-part reciprocal component
	 * Construct a separate reduced argument modulo pi near pi/2 multiples.
	 * i.e. (pi/2 - x) mod pi, simply by subtracting the reduced argument
	 * from an accurate B_hi + B_lo = (128 - n) pi/128. Force the upper part
	 * of this reduced argument to half-length to simplify accurate
	 * reciprocation later on.
	 */
	subps	%xmm2, %xmm7
	movaps	%xmm4, %xmmA
	andps	%xmm7, %xmm4
	subps	%xmm4, %xmm7
	addps	%xmm7, %xmm0
	/*
	 * Now compute an approximate reciprocal to mix into the computation
	 * To avoid any danger of nonportability, force it to 12 bits,
	 * though I suspect it always is anyway on current platforms.
	 */
	rcpps	%xmm4, %xmm7
	andps	%xmmA, %xmm7
	mulps	%xmm7, %xmm4
	movaps	%xmm10, %xmmA
	punpcklqdq %xmm8, %xmm10
	punpckhqdq %xmm8, %xmmA
	movaps	%xmm7, %xmm8
	/* Finally, multiplex both parts so they are only used in
	   cotangent path.  */
	mulps	%xmm10, %xmm7
	movaps	%xmm5, %xmm11
	punpckhqdq %xmm6, %xmm5
	punpcklqdq %xmm6, %xmm11
	/*
	 *  Compensated sum of dominant component(s)
	 * Compute C0_hi + C1_hi * Z + Recip_hi + Recip_lo = H4 (hi) + H9 (lo)
	 * H1 = C1_hi * Z (exact since C1_hi is 1 bit)
	 */
	mulps	%xmm2, %xmm5
	movaps	%xmmA, %xmm2
	/* H2 = high(C0_hi + C1_hi * Z).  */
	addps	%xmm5, %xmmA
	/* H4 = high(H2 + Recip_hi).  */
	/* intermediate in compensated sum.  */
	subps	%xmmA, %xmm2
	/* H5 = low(C0_hi + C1_hi * Z).  */
	addps	%xmm2, %xmm5
	movaps	%xmmA, %xmm2
	addps	%xmm7, %xmmA
	/* intermediate in compensated sum.  */
	subps	%xmmA, %xmm7
	/* H8 = low(H2 + Recip_hi).  */
	addps	%xmm7, %xmm2
	/*
	 * Get a better approximation to  1/sR_hi (not far short of an ulp)
	 * using a third-order polynomial approximation
	 */
	movups	_sOne + __svml_stan_data_internal(%rip), %xmm6
	movaps	%xmm6, %xmm7
	subps	%xmm4, %xmm6
	movaps	%xmm6, %xmm4
	mulps	%xmm6, %xmm6
	addps	%xmm6, %xmm7
	movaps	%xmm8, %xmm6
	mulps	%xmm4, %xmm8
	addps	%xmm6, %xmm8
	mulps	%xmm7, %xmm8
	/*
	 * Multiply by sRecip_ok to make sR_lo relative to sR_hi
	 * Since sR_lo is shifted off by about 12 bits, this is accurate enough.
	 */
	mulps	%xmm8, %xmm0
	movaps	%xmm0, %xmm6
	subps	%xmm4, %xmm0
	/*
	 * Now create a low reciprocal using
	 * (Recip_hi + Er * Recip_ok) * (1 + sR_lo^2 - sR_lo)
	 * =~= Recip_hi + Recip_ok * (Er + sR_lo^2 - sR_lo)
	 */
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
	addps	%xmmA, %xmm11
	/* And now the very final summation.  */
	andps	%xmm14, %xmm11

	/*
	 *  The end of implementation (LA with huge args reduction)
	 * End of large arguments path (_HA_, _LA_ and _EP_)
	 */
	orps	%xmm3, %xmm11
	movups	_sAbsMask + __svml_stan_data_internal(%rip), %xmm3
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
	   encoding for many instructions (as compared with r12/r13).
	 */
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

#ifdef __svml_stan_data_internal_typedef
	typedef	unsigned int VUINT32;
	typedef	struct{
	__declspec (align(16))VUINT32 _sAbsMask[4][1];
	__declspec (align(16))VUINT32 _sInvPi[4][1];
	__declspec (align(16))VUINT32 _sRShifter[4][1];
	__declspec (align(16))VUINT32 _sPI1[4][1];
	__declspec (align(16))VUINT32 _sPI2[4][1];
	__declspec (align(16))VUINT32 _sPI3[4][1];
	__declspec (align(16))VUINT32 _sPI4[4][1];
	__declspec (align(16))VUINT32 _sQ2[4][1];
	__declspec (align(16))VUINT32 _sP1[4][1];
	__declspec (align(16))VUINT32 _sQ1[4][1];
	__declspec (align(16))VUINT32 _sP0[4][1];
	__declspec (align(16))VUINT32 _sRangeReductionVal[4][1];
	__declspec (align(16))VUINT32 _sRangeReductionVal_uisa[4][1];
	__declspec (align(16))VUINT32 _sRangeVal[4][1];
	__declspec (align(16))VUINT32 _sOne[4][1];
	__declspec (align(16))VUINT32 _sCoeffs[128][10][1];
	}__svml_stan_data_internal;
#endif
__svml_stan_data_internal:
	.align	16
	.long	0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF	/* _sAbsMask */
	.align	16
	.long	0x3F22F983, 0x3F22F983, 0x3F22F983, 0x3F22F983	/* _sInvPi */
	.align	16
	.long	0x4B400000, 0x4B400000, 0x4B400000, 0x4B400000	/* _sRShifter */
	.align	16
	.long	0x3FC90000, 0x3FC90000, 0x3FC90000, 0x3FC90000	/* _sPI1 */
	.align	16
	.long	0x39FDA000, 0x39FDA000, 0x39FDA000, 0x39FDA000	/* _sPI2 */
	.align	16
	.long	0x33A22000, 0x33A22000, 0x33A22000, 0x33A22000	/* _sPI3 */
	.align	16
	.long	0x2C34611A, 0x2C34611A, 0x2C34611A, 0x2C34611A	/* _sPI4 */
	.align	16
	.long	0x3C1F336B, 0x3C1F336B, 0x3C1F336B, 0x3C1F336B	/* _sQ2 */
	.align	16
	.long	0xBDC433B4, 0xBDC433B4, 0xBDC433B4, 0xBDC433B4	/* _sP1 */
	.align	16
	.long	0xBEDBB7AB, 0xBEDBB7AB, 0xBEDBB7AB, 0xBEDBB7AB	/* _sQ1 */
	.align	16
	.long	0x3F7FFFFC, 0x3F7FFFFC, 0x3F7FFFFC, 0x3F7FFFFC	/* _sP0 */
	.align	16
	.long	0x46010000, 0x46010000, 0x46010000, 0x46010000	/* _sRangeReductionVal */
	.align	16
	.long	0x007fffff, 0x007fffff, 0x007fffff, 0x007fffff	/* _sRangeReductionVal_uisa */
	.align	16
	.long	0x00800000, 0x00800000, 0x00800000, 0x00800000	/* _sRangeVal */
	.align	16
	.long	0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000	/* _sOne */

	// _sCoeffs Breakpoint B = 0 * pi/128, function tan(B + x)
	.align	16
	.long	0x3FC90FDB	// B' = pi/2 - B (high single)
	.long	0xB33BBD2E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x00000000	// c0 (high single)
	.long	0x00000000	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x00000000	// c1 (low single)
	.long	0x00000000	// c2
	.long	0x3EAAACDD	// c3
	.long	0x00000000	// c4
	.long	0x3FC5EB9B	// B' = pi/2 - B (high single)
	.long	0x32DE638C	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3CC91A31	// c0 (high single)
	.long	0x2F8E8D1A	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3A1DFA00	// c1 (low single)
	.long	0x3CC9392D	// c2
	.long	0x3EAB1889	// c3
	.long	0x3C885D3B	// c4
	.long	0x3FC2C75C	// B' = pi/2 - B (high single)
	.long	0xB2CBBE8A	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3D49393C	// c0 (high single)
	.long	0x30A39F5B	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3B1E2B00	// c1 (low single)
	.long	0x3D49B5D4	// c2
	.long	0x3EAC4F10	// c3
	.long	0x3CFD9425	// c4
	.long	0x3FBFA31C	// B' = pi/2 - B (high single)
	.long	0x33450FB0	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3D9711CE	// c0 (high single)
	.long	0x314FEB28	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3BB24C00	// c1 (low single)
	.long	0x3D97E43A	// c2
	.long	0x3EAE6A89	// c3
	.long	0x3D4D07E0	// c4
	.long	0x3FBC7EDD	// B' = pi/2 - B (high single)
	.long	0xB1800ADD	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3DC9B5DC	// c0 (high single)
	.long	0x3145AD86	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3C1EEF20	// c1 (low single)
	.long	0x3DCBAAEA	// c2
	.long	0x3EB14E5E	// c3
	.long	0x3D858BB2	// c4
	.long	0x3FB95A9E	// B' = pi/2 - B (high single)
	.long	0xB3651267	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3DFC98C2	// c0 (high single)
	.long	0xB0AE525C	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3C793D20	// c1 (low single)
	.long	0x3E003845	// c2
	.long	0x3EB5271F	// c3
	.long	0x3DAC669E	// c4
	.long	0x3FB6365E	// B' = pi/2 - B (high single)
	.long	0x328BB91C	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E17E564	// c0 (high single)
	.long	0xB1C5A2E4	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3CB440D0	// c1 (low single)
	.long	0x3E1B3D00	// c2
	.long	0x3EB9F664	// c3
	.long	0x3DD647C0	// c4
	.long	0x3FB3121F	// B' = pi/2 - B (high single)
	.long	0xB30F347D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E31AE4D	// c0 (high single)
	.long	0xB1F32251	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3CF6A500	// c1 (low single)
	.long	0x3E3707DA	// c2
	.long	0x3EBFA489	// c3
	.long	0x3DFBD9C7	// c4
	.long	0x3FAFEDDF	// B' = pi/2 - B (high single)
	.long	0x331BBA77	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E4BAFAF	// c0 (high single)
	.long	0x2F2A29E0	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D221018	// c1 (low single)
	.long	0x3E53BED0	// c2
	.long	0x3EC67E26	// c3
	.long	0x3E1568E2	// c4
	.long	0x3FACC9A0	// B' = pi/2 - B (high single)
	.long	0xB2655A50	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E65F267	// c0 (high single)
	.long	0x31B4B1DF	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D4E8B90	// c1 (low single)
	.long	0x3E718ACA	// c2
	.long	0x3ECE7164	// c3
	.long	0x3E2DC161	// c4
	.long	0x3FA9A560	// B' = pi/2 - B (high single)
	.long	0x33719861	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E803FD4	// c0 (high single)
	.long	0xB2279E66	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D807FC8	// c1 (low single)
	.long	0x3E884BD4	// c2
	.long	0x3ED7812D	// c3
	.long	0x3E4636EB	// c4
	.long	0x3FA68121	// B' = pi/2 - B (high single)
	.long	0x31E43AAC	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E8DB082	// c0 (high single)
	.long	0xB132A234	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D9CD7D0	// c1 (low single)
	.long	0x3E988A60	// c2
	.long	0x3EE203E3	// c3
	.long	0x3E63582C	// c4
	.long	0x3FA35CE2	// B' = pi/2 - B (high single)
	.long	0xB33889B6	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3E9B5042	// c0 (high single)
	.long	0xB22A3AEE	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3DBC7490	// c1 (low single)
	.long	0x3EA99AF5	// c2
	.long	0x3EEDE107	// c3
	.long	0x3E80E9AA	// c4
	.long	0x3FA038A2	// B' = pi/2 - B (high single)
	.long	0x32E4CA7E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3EA92457	// c0 (high single)
	.long	0x30B80830	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3DDF8200	// c1 (low single)
	.long	0x3EBB99E9	// c2
	.long	0x3EFB4AA8	// c3
	.long	0x3E9182BE	// c4
	.long	0x3F9D1463	// B' = pi/2 - B (high single)
	.long	0xB2C55799	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3EB73250	// c0 (high single)
	.long	0xB2028823	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E0318F8	// c1 (low single)
	.long	0x3ECEA678	// c2
	.long	0x3F053C67	// c3
	.long	0x3EA41E53	// c4
	.long	0x3F99F023	// B' = pi/2 - B (high single)
	.long	0x33484328	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3EC5800D	// c0 (high single)
	.long	0xB214C3C1	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E185E54	// c1 (low single)
	.long	0x3EE2E342	// c2
	.long	0x3F0DCA73	// c3
	.long	0x3EB8CC21	// c4
	.long	0x3F96CBE4	// B' = pi/2 - B (high single)
	.long	0xB14CDE2E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3ED413CD	// c0 (high single)
	.long	0xB1C06152	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E2FB0CC	// c1 (low single)
	.long	0x3EF876CB	// c2
	.long	0x3F177807	// c3
	.long	0x3ED08437	// c4
	.long	0x3F93A7A5	// B' = pi/2 - B (high single)
	.long	0xB361DEEE	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3EE2F439	// c0 (high single)
	.long	0xB1F4399E	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E49341C	// c1 (low single)
	.long	0x3F07C61A	// c2
	.long	0x3F22560F	// c3
	.long	0x3EEAA81E	// c4
	.long	0x3F908365	// B' = pi/2 - B (high single)
	.long	0x3292200D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3EF22870	// c0 (high single)
	.long	0x325271F4	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E65107A	// c1 (low single)
	.long	0x3F1429F0	// c2
	.long	0x3F2E8AFC	// c3
	.long	0x3F040498	// c4
	.long	0x3F8D5F26	// B' = pi/2 - B (high single)
	.long	0xB30C0105	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F00DC0D	// c0 (high single)
	.long	0xB214AF72	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E81B994	// c1 (low single)
	.long	0x3F218233	// c2
	.long	0x3F3C4531	// c3
	.long	0x3F149688	// c4
	.long	0x3F8A3AE6	// B' = pi/2 - B (high single)
	.long	0x331EEDF0	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F08D5B9	// c0 (high single)
	.long	0xB25EF98E	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E92478D	// c1 (low single)
	.long	0x3F2FEDC9	// c2
	.long	0x3F4BCD58	// c3
	.long	0x3F27AE9E	// c4
	.long	0x3F8716A7	// B' = pi/2 - B (high single)
	.long	0xB2588C6D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F1105AF	// c0 (high single)
	.long	0x32F045B0	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EA44EE2	// c1 (low single)
	.long	0x3F3F8FDB	// c2
	.long	0x3F5D3FD0	// c3
	.long	0x3F3D0A23	// c4
	.long	0x3F83F267	// B' = pi/2 - B (high single)
	.long	0x3374CBD9	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F1970C4	// c0 (high single)
	.long	0x32904848	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EB7EFF8	// c1 (low single)
	.long	0x3F50907C	// c2
	.long	0x3F710FEA	// c3
	.long	0x3F561FED	// c4
	.long	0x3F80CE28	// B' = pi/2 - B (high single)
	.long	0x31FDD672	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F221C37	// c0 (high single)
	.long	0xB20C61DC	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3ECD4F71	// c1 (low single)
	.long	0x3F631DAA	// c2
	.long	0x3F83B471	// c3
	.long	0x3F7281EA	// c4
	.long	0x3F7B53D1	// B' = pi/2 - B (high single)
	.long	0x32955386	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F2B0DC1	// c0 (high single)
	.long	0x32AB7EBA	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EE496C2	// c1 (low single)
	.long	0x3F776C40	// c2
	.long	0x3F9065C1	// c3
	.long	0x3F89AFB6	// c4
	.long	0x3F750B52	// B' = pi/2 - B (high single)
	.long	0x32EB316F	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F344BA9	// c0 (high single)
	.long	0xB2B8B0EA	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EFDF4F7	// c1 (low single)
	.long	0x3F86DCA8	// c2
	.long	0x3F9ED53B	// c3
	.long	0x3F9CBEDE	// c4
	.long	0x3F6EC2D4	// B' = pi/2 - B (high single)
	.long	0xB2BEF0A7	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F3DDCCF	// c0 (high single)
	.long	0x32D29606	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEE6606F	// c1 (low single)
	.long	0x3F9325D6	// c2
	.long	0x3FAF4E69	// c3
	.long	0x3FB3080C	// c4
	.long	0x3F687A55	// B' = pi/2 - B (high single)
	.long	0xB252257B	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F47C8CC	// c0 (high single)
	.long	0xB200F51A	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEC82C6C	// c1 (low single)
	.long	0x3FA0BAE9	// c2
	.long	0x3FC2252F	// c3
	.long	0x3FCD24C7	// c4
	.long	0x3F6231D6	// B' = pi/2 - B (high single)
	.long	0xB119A6A2	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F521801	// c0 (high single)
	.long	0x32AE4178	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEA72938	// c1 (low single)
	.long	0x3FAFCC22	// c2
	.long	0x3FD7BD4A	// c3
	.long	0x3FEBB01B	// c4
	.long	0x3F5BE957	// B' = pi/2 - B (high single)
	.long	0x3205522A	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F5CD3BE	// c0 (high single)
	.long	0x31460308	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBE8306C5	// c1 (low single)
	.long	0x3FC09232	// c2
	.long	0x3FF09632	// c3
	.long	0x4007DB00	// c4
	.long	0x3F55A0D8	// B' = pi/2 - B (high single)
	.long	0x329886FF	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F68065E	// c0 (high single)
	.long	0x32670D1A	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBE36D1D6	// c1 (low single)
	.long	0x3FD35007	// c2
	.long	0x4006A861	// c3
	.long	0x401D4BDA	// c4
	.long	0x3F4F5859	// B' = pi/2 - B (high single)
	.long	0x32EE64E8	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0x3F73BB75	// c0 (high single)
	.long	0x32FC908D	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBDBF94B0	// c1 (low single)
	.long	0x3FE8550F	// c2
	.long	0x40174F67	// c3
	.long	0x4036C608	// c4
	.long	0x3F490FDB	// B' = pi/2 - B (high single)
	.long	0xB2BBBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE8BE60E	// c0 (high single)
	.long	0x320D8D84	// c0 (low single)
	.long	0x3F000000	// c1 (high 1 bit)
	.long	0xBDF817B1	// c1 (low single)
	.long	0xBD8345EB	// c2
	.long	0x3D1DFDAC	// c3
	.long	0xBC52CF6F	// c4
	.long	0x3F42C75C	// B' = pi/2 - B (high single)
	.long	0xB24BBE8A	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE87283F	// c0 (high single)
	.long	0xB268B966	// c0 (low single)
	.long	0x3F000000	// c1 (high 1 bit)
	.long	0xBDFE6529	// c1 (low single)
	.long	0xBD7B1953	// c2
	.long	0x3D18E109	// c3
	.long	0xBC4570B0	// c4
	.long	0x3F3C7EDD	// B' = pi/2 - B (high single)
	.long	0xB1000ADD	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE827420	// c0 (high single)
	.long	0x320B8B4D	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DFB9428	// c1 (low single)
	.long	0xBD7002B4	// c2
	.long	0x3D142A6C	// c3
	.long	0xBC3A47FF	// c4
	.long	0x3F36365E	// B' = pi/2 - B (high single)
	.long	0x320BB91C	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE7B9282	// c0 (high single)
	.long	0xB13383D2	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DF5D211	// c1 (low single)
	.long	0xBD6542B3	// c2
	.long	0x3D0FE5E5	// c3
	.long	0xBC31FB14	// c4
	.long	0x3F2FEDDF	// B' = pi/2 - B (high single)
	.long	0x329BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE724E73	// c0 (high single)
	.long	0x3120C3E2	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DF05283	// c1 (low single)
	.long	0xBD5AD45E	// c2
	.long	0x3D0BAFBF	// c3
	.long	0xBC27B8BB	// c4
	.long	0x3F29A560	// B' = pi/2 - B (high single)
	.long	0x32F19861	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE691B44	// c0 (high single)
	.long	0x31F18936	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DEB138B	// c1 (low single)
	.long	0xBD50B2F7	// c2
	.long	0x3D07BE3A	// c3
	.long	0xBC1E46A7	// c4
	.long	0x3F235CE2	// B' = pi/2 - B (high single)
	.long	0xB2B889B6	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE5FF82C	// c0 (high single)
	.long	0xB170723A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DE61354	// c1 (low single)
	.long	0xBD46DA06	// c2
	.long	0x3D0401F8	// c3
	.long	0xBC14E013	// c4
	.long	0x3F1D1463	// B' = pi/2 - B (high single)
	.long	0xB2455799	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE56E46B	// c0 (high single)
	.long	0x31E3F001	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DE15025	// c1 (low single)
	.long	0xBD3D4550	// c2
	.long	0x3D00462D	// c3
	.long	0xBC092C98	// c4
	.long	0x3F16CBE4	// B' = pi/2 - B (high single)
	.long	0xB0CCDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE4DDF41	// c0 (high single)
	.long	0xB1AEA094	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DDCC85C	// c1 (low single)
	.long	0xBD33F0BE	// c2
	.long	0x3CFA23B0	// c3
	.long	0xBC01FCF7	// c4
	.long	0x3F108365	// B' = pi/2 - B (high single)
	.long	0x3212200D	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE44E7F8	// c0 (high single)
	.long	0xB1CAA3CB	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD87A74	// c1 (low single)
	.long	0xBD2AD885	// c2
	.long	0x3CF3C785	// c3
	.long	0xBBF1E348	// c4
	.long	0x3F0A3AE6	// B' = pi/2 - B (high single)
	.long	0x329EEDF0	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE3BFDDC	// c0 (high single)
	.long	0xB132521A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD464FC	// c1 (low single)
	.long	0xBD21F8F1	// c2
	.long	0x3CEE3076	// c3
	.long	0xBBE6D263	// c4
	.long	0x3F03F267	// B' = pi/2 - B (high single)
	.long	0x32F4CBD9	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE33203E	// c0 (high single)
	.long	0x31FEF5BE	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD0869C	// c1 (low single)
	.long	0xBD194E8C	// c2
	.long	0x3CE8DCA9	// c3
	.long	0xBBDADA55	// c4
	.long	0x3EFB53D1	// B' = pi/2 - B (high single)
	.long	0x32155386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE2A4E71	// c0 (high single)
	.long	0xB19CFCEC	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DCCDE11	// c1 (low single)
	.long	0xBD10D605	// c2
	.long	0x3CE382A7	// c3
	.long	0xBBC8BD97	// c4
	.long	0x3EEEC2D4	// B' = pi/2 - B (high single)
	.long	0xB23EF0A7	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE2187D0	// c0 (high single)
	.long	0xB1B7C7F7	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC96A2B	// c1 (low single)
	.long	0xBD088C22	// c2
	.long	0x3CDE950E	// c3
	.long	0xBBB89AD1	// c4
	.long	0x3EE231D6	// B' = pi/2 - B (high single)
	.long	0xB099A6A2	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE18CBB7	// c0 (high single)
	.long	0xAFE28430	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC629CE	// c1 (low single)
	.long	0xBD006DCD	// c2
	.long	0x3CDA5A2C	// c3
	.long	0xBBB0B3D2	// c4
	.long	0x3ED5A0D8	// B' = pi/2 - B (high single)
	.long	0x321886FF	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE101985	// c0 (high single)
	.long	0xB02FB2B8	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC31BF3	// c1 (low single)
	.long	0xBCF0F04D	// c2
	.long	0x3CD60BC7	// c3
	.long	0xBBA138BA	// c4
	.long	0x3EC90FDB	// B' = pi/2 - B (high single)
	.long	0xB23BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBE07709D	// c0 (high single)
	.long	0xB18A2A83	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC03FA2	// c1 (low single)
	.long	0xBCE15096	// c2
	.long	0x3CD26472	// c3
	.long	0xBB9A1270	// c4
	.long	0x3EBC7EDD	// B' = pi/2 - B (high single)
	.long	0xB0800ADD	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDFDA0CB	// c0 (high single)
	.long	0x2F14FCA0	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DBD93F7	// c1 (low single)
	.long	0xBCD1F71B	// c2
	.long	0x3CCEDD2B	// c3
	.long	0xBB905946	// c4
	.long	0x3EAFEDDF	// B' = pi/2 - B (high single)
	.long	0x321BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDEC708C	// c0 (high single)
	.long	0xB14895C4	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DBB181E	// c1 (low single)
	.long	0xBCC2DEA6	// c2
	.long	0x3CCB5027	// c3
	.long	0xBB7F3969	// c4
	.long	0x3EA35CE2	// B' = pi/2 - B (high single)
	.long	0xB23889B6	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDDB4F55	// c0 (high single)
	.long	0x30F6437E	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB8CB52	// c1 (low single)
	.long	0xBCB40210	// c2
	.long	0x3CC82D45	// c3
	.long	0xBB643075	// c4
	.long	0x3E96CBE4	// B' = pi/2 - B (high single)
	.long	0xB04CDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDCA3BFF	// c0 (high single)
	.long	0x311C95EA	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB6ACDE	// c1 (low single)
	.long	0xBCA55C5B	// c2
	.long	0x3CC5BC04	// c3
	.long	0xBB63A969	// c4
	.long	0x3E8A3AE6	// B' = pi/2 - B (high single)
	.long	0x321EEDF0	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDB93569	// c0 (high single)
	.long	0xAFB9ED00	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB4BC1F	// c1 (low single)
	.long	0xBC96E905	// c2
	.long	0x3CC2E6F5	// c3
	.long	0xBB3E10A6	// c4
	.long	0x3E7B53D1	// B' = pi/2 - B (high single)
	.long	0x31955386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBDA83A77	// c0 (high single)
	.long	0x316D967A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB2F87C	// c1 (low single)
	.long	0xBC88A31F	// c2
	.long	0x3CC0E763	// c3
	.long	0xBB3F1666	// c4
	.long	0x3E6231D6	// B' = pi/2 - B (high single)
	.long	0xB019A6A2	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD974A0D	// c0 (high single)
	.long	0xB14F365B	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB1616F	// c1 (low single)
	.long	0xBC750CD8	// c2
	.long	0x3CBEB595	// c3
	.long	0xBB22B883	// c4
	.long	0x3E490FDB	// B' = pi/2 - B (high single)
	.long	0xB1BBBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD866317	// c0 (high single)
	.long	0xAFF02140	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAFF67D	// c1 (low single)
	.long	0xBC591CD0	// c2
	.long	0x3CBCBEAD	// c3
	.long	0xBB04BBEC	// c4
	.long	0x3E2FEDDF	// B' = pi/2 - B (high single)
	.long	0x319BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD6B08FF	// c0 (high single)
	.long	0xB0EED236	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAEB739	// c1 (low single)
	.long	0xBC3D6D51	// c2
	.long	0x3CBB485D	// c3
	.long	0xBAFFF5BA	// c4
	.long	0x3E16CBE4	// B' = pi/2 - B (high single)
	.long	0xAFCCDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD495A6C	// c0 (high single)
	.long	0xB0A427BD	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DADA345	// c1 (low single)
	.long	0xBC21F648	// c2
	.long	0x3CB9D1B4	// c3
	.long	0xBACB5567	// c4
	.long	0x3DFB53D1	// B' = pi/2 - B (high single)
	.long	0x31155386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD27B856	// c0 (high single)
	.long	0xB0F7EE91	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DACBA4E	// c1 (low single)
	.long	0xBC06AEE3	// c2
	.long	0x3CB8E5DC	// c3
	.long	0xBAEC00EE	// c4
	.long	0x3DC90FDB	// B' = pi/2 - B (high single)
	.long	0xB13BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBD0620A3	// c0 (high single)
	.long	0xB0ECAB40	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DABFC11	// c1 (low single)
	.long	0xBBD7200F	// c2
	.long	0x3CB79475	// c3
	.long	0xBA2B0ADC	// c4
	.long	0x3D96CBE4	// B' = pi/2 - B (high single)
	.long	0xAF4CDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBCC92278	// c0 (high single)
	.long	0x302F2E68	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAB6854	// c1 (low single)
	.long	0xBBA1214F	// c2
	.long	0x3CB6C1E9	// c3
	.long	0x3843C2F3	// c4
	.long	0x3D490FDB	// B' = pi/2 - B (high single)
	.long	0xB0BBBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBC861015	// c0 (high single)
	.long	0xAFD68E2E	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAAFEEB	// c1 (low single)
	.long	0xBB569F3F	// c2
	.long	0x3CB6A84E	// c3
	.long	0xBAC64194	// c4
	.long	0x3CC90FDB	// B' = pi/2 - B (high single)
	.long	0xB03BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0xBC060BF3	// c0 (high single)
	.long	0x2FE251AE	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAABFB9	// c1 (low single)
	.long	0xBAD67C60	// c2
	.long	0x3CB64CA5	// c3
	.long	0xBACDE881	// c4
	.long	0x00000000	// B' = pi/2 - B (high single)
	.long	0x00000000	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x00000000	// c0 (high single)
	.long	0x00000000	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAAAAAB	// c1 (low single)
	.long	0x00000000	// c2
	.long	0x3CB5E28B	// c3
	.long	0x00000000	// c4
	.long	0xBCC90FDB	// B' = pi/2 - B (high single)
	.long	0x303BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3C060BF3	// c0 (high single)
	.long	0xAFE251AE	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAABFB9	// c1 (low single)
	.long	0x3AD67C60	// c2
	.long	0x3CB64CA5	// c3
	.long	0x3ACDE881	// c4
	.long	0xBD490FDB	// B' = pi/2 - B (high single)
	.long	0x30BBBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3C861015	// c0 (high single)
	.long	0x2FD68E2E	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAAFEEB	// c1 (low single)
	.long	0x3B569F3F	// c2
	.long	0x3CB6A84E	// c3
	.long	0x3AC64194	// c4
	.long	0xBD96CBE4	// B' = pi/2 - B (high single)
	.long	0x2F4CDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3CC92278	// c0 (high single)
	.long	0xB02F2E68	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAB6854	// c1 (low single)
	.long	0x3BA1214F	// c2
	.long	0x3CB6C1E9	// c3
	.long	0xB843C2F2	// c4
	.long	0xBDC90FDB	// B' = pi/2 - B (high single)
	.long	0x313BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D0620A3	// c0 (high single)
	.long	0x30ECAB40	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DABFC11	// c1 (low single)
	.long	0x3BD7200F	// c2
	.long	0x3CB79475	// c3
	.long	0x3A2B0ADC	// c4
	.long	0xBDFB53D1	// B' = pi/2 - B (high single)
	.long	0xB1155386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D27B856	// c0 (high single)
	.long	0x30F7EE91	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DACBA4E	// c1 (low single)
	.long	0x3C06AEE3	// c2
	.long	0x3CB8E5DC	// c3
	.long	0x3AEC00EE	// c4
	.long	0xBE16CBE4	// B' = pi/2 - B (high single)
	.long	0x2FCCDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D495A6C	// c0 (high single)
	.long	0x30A427BD	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DADA345	// c1 (low single)
	.long	0x3C21F648	// c2
	.long	0x3CB9D1B4	// c3
	.long	0x3ACB5567	// c4
	.long	0xBE2FEDDF	// B' = pi/2 - B (high single)
	.long	0xB19BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D6B08FF	// c0 (high single)
	.long	0x30EED236	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAEB739	// c1 (low single)
	.long	0x3C3D6D51	// c2
	.long	0x3CBB485D	// c3
	.long	0x3AFFF5BA	// c4
	.long	0xBE490FDB	// B' = pi/2 - B (high single)
	.long	0x31BBBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D866317	// c0 (high single)
	.long	0x2FF02140	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DAFF67D	// c1 (low single)
	.long	0x3C591CD0	// c2
	.long	0x3CBCBEAD	// c3
	.long	0x3B04BBEC	// c4
	.long	0xBE6231D6	// B' = pi/2 - B (high single)
	.long	0x3019A6A2	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3D974A0D	// c0 (high single)
	.long	0x314F365B	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB1616F	// c1 (low single)
	.long	0x3C750CD8	// c2
	.long	0x3CBEB595	// c3
	.long	0x3B22B883	// c4
	.long	0xBE7B53D1	// B' = pi/2 - B (high single)
	.long	0xB1955386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DA83A77	// c0 (high single)
	.long	0xB16D967A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB2F87C	// c1 (low single)
	.long	0x3C88A31F	// c2
	.long	0x3CC0E763	// c3
	.long	0x3B3F1666	// c4
	.long	0xBE8A3AE6	// B' = pi/2 - B (high single)
	.long	0xB21EEDF0	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DB93569	// c0 (high single)
	.long	0x2FB9ED00	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB4BC1F	// c1 (low single)
	.long	0x3C96E905	// c2
	.long	0x3CC2E6F5	// c3
	.long	0x3B3E10A6	// c4
	.long	0xBE96CBE4	// B' = pi/2 - B (high single)
	.long	0x304CDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DCA3BFF	// c0 (high single)
	.long	0xB11C95EA	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB6ACDE	// c1 (low single)
	.long	0x3CA55C5B	// c2
	.long	0x3CC5BC04	// c3
	.long	0x3B63A969	// c4
	.long	0xBEA35CE2	// B' = pi/2 - B (high single)
	.long	0x323889B6	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DDB4F55	// c0 (high single)
	.long	0xB0F6437E	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DB8CB52	// c1 (low single)
	.long	0x3CB40210	// c2
	.long	0x3CC82D45	// c3
	.long	0x3B643075	// c4
	.long	0xBEAFEDDF	// B' = pi/2 - B (high single)
	.long	0xB21BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DEC708C	// c0 (high single)
	.long	0x314895C4	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DBB181E	// c1 (low single)
	.long	0x3CC2DEA6	// c2
	.long	0x3CCB5027	// c3
	.long	0x3B7F3969	// c4
	.long	0xBEBC7EDD	// B' = pi/2 - B (high single)
	.long	0x30800ADD	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3DFDA0CB	// c0 (high single)
	.long	0xAF14FCA0	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DBD93F7	// c1 (low single)
	.long	0x3CD1F71B	// c2
	.long	0x3CCEDD2B	// c3
	.long	0x3B905946	// c4
	.long	0xBEC90FDB	// B' = pi/2 - B (high single)
	.long	0x323BBD2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E07709D	// c0 (high single)
	.long	0x318A2A83	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC03FA2	// c1 (low single)
	.long	0x3CE15096	// c2
	.long	0x3CD26472	// c3
	.long	0x3B9A1270	// c4
	.long	0xBED5A0D8	// B' = pi/2 - B (high single)
	.long	0xB21886FF	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E101985	// c0 (high single)
	.long	0x302FB2B8	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC31BF3	// c1 (low single)
	.long	0x3CF0F04D	// c2
	.long	0x3CD60BC7	// c3
	.long	0x3BA138BA	// c4
	.long	0xBEE231D6	// B' = pi/2 - B (high single)
	.long	0x3099A6A2	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E18CBB7	// c0 (high single)
	.long	0x2FE28430	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC629CE	// c1 (low single)
	.long	0x3D006DCD	// c2
	.long	0x3CDA5A2C	// c3
	.long	0x3BB0B3D2	// c4
	.long	0xBEEEC2D4	// B' = pi/2 - B (high single)
	.long	0x323EF0A7	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E2187D0	// c0 (high single)
	.long	0x31B7C7F7	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DC96A2B	// c1 (low single)
	.long	0x3D088C22	// c2
	.long	0x3CDE950E	// c3
	.long	0x3BB89AD1	// c4
	.long	0xBEFB53D1	// B' = pi/2 - B (high single)
	.long	0xB2155386	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E2A4E71	// c0 (high single)
	.long	0x319CFCEC	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DCCDE11	// c1 (low single)
	.long	0x3D10D605	// c2
	.long	0x3CE382A7	// c3
	.long	0x3BC8BD97	// c4
	.long	0xBF03F267	// B' = pi/2 - B (high single)
	.long	0xB2F4CBD9	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E33203E	// c0 (high single)
	.long	0xB1FEF5BE	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD0869C	// c1 (low single)
	.long	0x3D194E8C	// c2
	.long	0x3CE8DCA9	// c3
	.long	0x3BDADA55	// c4
	.long	0xBF0A3AE6	// B' = pi/2 - B (high single)
	.long	0xB29EEDF0	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E3BFDDC	// c0 (high single)
	.long	0x3132521A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD464FC	// c1 (low single)
	.long	0x3D21F8F1	// c2
	.long	0x3CEE3076	// c3
	.long	0x3BE6D263	// c4
	.long	0xBF108365	// B' = pi/2 - B (high single)
	.long	0xB212200D	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E44E7F8	// c0 (high single)
	.long	0x31CAA3CB	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DD87A74	// c1 (low single)
	.long	0x3D2AD885	// c2
	.long	0x3CF3C785	// c3
	.long	0x3BF1E348	// c4
	.long	0xBF16CBE4	// B' = pi/2 - B (high single)
	.long	0x30CCDE2E	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E4DDF41	// c0 (high single)
	.long	0x31AEA094	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DDCC85C	// c1 (low single)
	.long	0x3D33F0BE	// c2
	.long	0x3CFA23B0	// c3
	.long	0x3C01FCF7	// c4
	.long	0xBF1D1463	// B' = pi/2 - B (high single)
	.long	0x32455799	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E56E46B	// c0 (high single)
	.long	0xB1E3F001	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DE15025	// c1 (low single)
	.long	0x3D3D4550	// c2
	.long	0x3D00462D	// c3
	.long	0x3C092C98	// c4
	.long	0xBF235CE2	// B' = pi/2 - B (high single)
	.long	0x32B889B6	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E5FF82C	// c0 (high single)
	.long	0x3170723A	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DE61354	// c1 (low single)
	.long	0x3D46DA06	// c2
	.long	0x3D0401F8	// c3
	.long	0x3C14E013	// c4
	.long	0xBF29A560	// B' = pi/2 - B (high single)
	.long	0xB2F19861	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E691B44	// c0 (high single)
	.long	0xB1F18936	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DEB138B	// c1 (low single)
	.long	0x3D50B2F7	// c2
	.long	0x3D07BE3A	// c3
	.long	0x3C1E46A7	// c4
	.long	0xBF2FEDDF	// B' = pi/2 - B (high single)
	.long	0xB29BBA77	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E724E73	// c0 (high single)
	.long	0xB120C3E2	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DF05283	// c1 (low single)
	.long	0x3D5AD45E	// c2
	.long	0x3D0BAFBF	// c3
	.long	0x3C27B8BB	// c4
	.long	0xBF36365E	// B' = pi/2 - B (high single)
	.long	0xB20BB91C	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E7B9282	// c0 (high single)
	.long	0x313383D2	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DF5D211	// c1 (low single)
	.long	0x3D6542B3	// c2
	.long	0x3D0FE5E5	// c3
	.long	0x3C31FB14	// c4
	.long	0xBF3C7EDD	// B' = pi/2 - B (high single)
	.long	0x31000ADD	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E827420	// c0 (high single)
	.long	0xB20B8B4D	// c0 (low single)
	.long	0x3E800000	// c1 (high 1 bit)
	.long	0x3DFB9428	// c1 (low single)
	.long	0x3D7002B4	// c2
	.long	0x3D142A6C	// c3
	.long	0x3C3A47FF	// c4
	.long	0xBF42C75C	// B' = pi/2 - B (high single)
	.long	0x324BBE8A	// B' = pi/2 - B (low single)
	.long	0x3F800000	// tau (1 for cot path)
	.long	0x3E87283F	// c0 (high single)
	.long	0x3268B966	// c0 (low single)
	.long	0x3F000000	// c1 (high 1 bit)
	.long	0xBDFE6529	// c1 (low single)
	.long	0x3D7B1953	// c2
	.long	0x3D18E109	// c3
	.long	0x3C4570B0	// c4
	.long	0xBF490FDB	// B' = pi/2 - B (high single)
	.long	0x32BBBD2E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF800000	// c0 (high single)
	.long	0x2B410000	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xB3000000	// c1 (low single)
	.long	0xC0000000	// c2
	.long	0x402AB7C8	// c3
	.long	0xC05561DB	// c4
	.long	0xBF4F5859	// B' = pi/2 - B (high single)
	.long	0xB2EE64E8	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF73BB75	// c0 (high single)
	.long	0xB2FC908D	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBDBF94B0	// c1 (low single)
	.long	0xBFE8550F	// c2
	.long	0x40174F67	// c3
	.long	0xC036C608	// c4
	.long	0xBF55A0D8	// B' = pi/2 - B (high single)
	.long	0xB29886FF	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF68065E	// c0 (high single)
	.long	0xB2670D1A	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBE36D1D6	// c1 (low single)
	.long	0xBFD35007	// c2
	.long	0x4006A861	// c3
	.long	0xC01D4BDA	// c4
	.long	0xBF5BE957	// B' = pi/2 - B (high single)
	.long	0xB205522A	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF5CD3BE	// c0 (high single)
	.long	0xB1460308	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBE8306C5	// c1 (low single)
	.long	0xBFC09232	// c2
	.long	0x3FF09632	// c3
	.long	0xC007DB00	// c4
	.long	0xBF6231D6	// B' = pi/2 - B (high single)
	.long	0x3119A6A2	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF521801	// c0 (high single)
	.long	0xB2AE4178	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEA72938	// c1 (low single)
	.long	0xBFAFCC22	// c2
	.long	0x3FD7BD4A	// c3
	.long	0xBFEBB01B	// c4
	.long	0xBF687A55	// B' = pi/2 - B (high single)
	.long	0x3252257B	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF47C8CC	// c0 (high single)
	.long	0x3200F51A	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEC82C6C	// c1 (low single)
	.long	0xBFA0BAE9	// c2
	.long	0x3FC2252F	// c3
	.long	0xBFCD24C7	// c4
	.long	0xBF6EC2D4	// B' = pi/2 - B (high single)
	.long	0x32BEF0A7	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF3DDCCF	// c0 (high single)
	.long	0xB2D29606	// c0 (low single)
	.long	0x40000000	// c1 (high 1 bit)
	.long	0xBEE6606F	// c1 (low single)
	.long	0xBF9325D6	// c2
	.long	0x3FAF4E69	// c3
	.long	0xBFB3080C	// c4
	.long	0xBF750B52	// B' = pi/2 - B (high single)
	.long	0xB2EB316F	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF344BA9	// c0 (high single)
	.long	0x32B8B0EA	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EFDF4F7	// c1 (low single)
	.long	0xBF86DCA8	// c2
	.long	0x3F9ED53B	// c3
	.long	0xBF9CBEDE	// c4
	.long	0xBF7B53D1	// B' = pi/2 - B (high single)
	.long	0xB2955386	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF2B0DC1	// c0 (high single)
	.long	0xB2AB7EBA	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EE496C2	// c1 (low single)
	.long	0xBF776C40	// c2
	.long	0x3F9065C1	// c3
	.long	0xBF89AFB6	// c4
	.long	0xBF80CE28	// B' = pi/2 - B (high single)
	.long	0xB1FDD672	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF221C37	// c0 (high single)
	.long	0x320C61DC	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3ECD4F71	// c1 (low single)
	.long	0xBF631DAA	// c2
	.long	0x3F83B471	// c3
	.long	0xBF7281EA	// c4
	.long	0xBF83F267	// B' = pi/2 - B (high single)
	.long	0xB374CBD9	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF1970C4	// c0 (high single)
	.long	0xB2904848	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EB7EFF8	// c1 (low single)
	.long	0xBF50907C	// c2
	.long	0x3F710FEA	// c3
	.long	0xBF561FED	// c4
	.long	0xBF8716A7	// B' = pi/2 - B (high single)
	.long	0x32588C6D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF1105AF	// c0 (high single)
	.long	0xB2F045B0	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3EA44EE2	// c1 (low single)
	.long	0xBF3F8FDB	// c2
	.long	0x3F5D3FD0	// c3
	.long	0xBF3D0A23	// c4
	.long	0xBF8A3AE6	// B' = pi/2 - B (high single)
	.long	0xB31EEDF0	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF08D5B9	// c0 (high single)
	.long	0x325EF98E	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E92478D	// c1 (low single)
	.long	0xBF2FEDC9	// c2
	.long	0x3F4BCD58	// c3
	.long	0xBF27AE9E	// c4
	.long	0xBF8D5F26	// B' = pi/2 - B (high single)
	.long	0x330C0105	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBF00DC0D	// c0 (high single)
	.long	0x3214AF72	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E81B994	// c1 (low single)
	.long	0xBF218233	// c2
	.long	0x3F3C4531	// c3
	.long	0xBF149688	// c4
	.long	0xBF908365	// B' = pi/2 - B (high single)
	.long	0xB292200D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBEF22870	// c0 (high single)
	.long	0xB25271F4	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E65107A	// c1 (low single)
	.long	0xBF1429F0	// c2
	.long	0x3F2E8AFC	// c3
	.long	0xBF040498	// c4
	.long	0xBF93A7A5	// B' = pi/2 - B (high single)
	.long	0x3361DEEE	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBEE2F439	// c0 (high single)
	.long	0x31F4399E	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E49341C	// c1 (low single)
	.long	0xBF07C61A	// c2
	.long	0x3F22560F	// c3
	.long	0xBEEAA81E	// c4
	.long	0xBF96CBE4	// B' = pi/2 - B (high single)
	.long	0x314CDE2E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBED413CD	// c0 (high single)
	.long	0x31C06152	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E2FB0CC	// c1 (low single)
	.long	0xBEF876CB	// c2
	.long	0x3F177807	// c3
	.long	0xBED08437	// c4
	.long	0xBF99F023	// B' = pi/2 - B (high single)
	.long	0xB3484328	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBEC5800D	// c0 (high single)
	.long	0x3214C3C1	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E185E54	// c1 (low single)
	.long	0xBEE2E342	// c2
	.long	0x3F0DCA73	// c3
	.long	0xBEB8CC21	// c4
	.long	0xBF9D1463	// B' = pi/2 - B (high single)
	.long	0x32C55799	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBEB73250	// c0 (high single)
	.long	0x32028823	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3E0318F8	// c1 (low single)
	.long	0xBECEA678	// c2
	.long	0x3F053C67	// c3
	.long	0xBEA41E53	// c4
	.long	0xBFA038A2	// B' = pi/2 - B (high single)
	.long	0xB2E4CA7E	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBEA92457	// c0 (high single)
	.long	0xB0B80830	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3DDF8200	// c1 (low single)
	.long	0xBEBB99E9	// c2
	.long	0x3EFB4AA8	// c3
	.long	0xBE9182BE	// c4
	.long	0xBFA35CE2	// B' = pi/2 - B (high single)
	.long	0x333889B6	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE9B5042	// c0 (high single)
	.long	0x322A3AEE	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3DBC7490	// c1 (low single)
	.long	0xBEA99AF5	// c2
	.long	0x3EEDE107	// c3
	.long	0xBE80E9AA	// c4
	.long	0xBFA68121	// B' = pi/2 - B (high single)
	.long	0xB1E43AAC	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE8DB082	// c0 (high single)
	.long	0x3132A234	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D9CD7D0	// c1 (low single)
	.long	0xBE988A60	// c2
	.long	0x3EE203E3	// c3
	.long	0xBE63582C	// c4
	.long	0xBFA9A560	// B' = pi/2 - B (high single)
	.long	0xB3719861	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE803FD4	// c0 (high single)
	.long	0x32279E66	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D807FC8	// c1 (low single)
	.long	0xBE884BD4	// c2
	.long	0x3ED7812D	// c3
	.long	0xBE4636EB	// c4
	.long	0xBFACC9A0	// B' = pi/2 - B (high single)
	.long	0x32655A50	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE65F267	// c0 (high single)
	.long	0xB1B4B1DF	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D4E8B90	// c1 (low single)
	.long	0xBE718ACA	// c2
	.long	0x3ECE7164	// c3
	.long	0xBE2DC161	// c4
	.long	0xBFAFEDDF	// B' = pi/2 - B (high single)
	.long	0xB31BBA77	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE4BAFAF	// c0 (high single)
	.long	0xAF2A29E0	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3D221018	// c1 (low single)
	.long	0xBE53BED0	// c2
	.long	0x3EC67E26	// c3
	.long	0xBE1568E2	// c4
	.long	0xBFB3121F	// B' = pi/2 - B (high single)
	.long	0x330F347D	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE31AE4D	// c0 (high single)
	.long	0x31F32251	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3CF6A500	// c1 (low single)
	.long	0xBE3707DA	// c2
	.long	0x3EBFA489	// c3
	.long	0xBDFBD9C7	// c4
	.long	0xBFB6365E	// B' = pi/2 - B (high single)
	.long	0xB28BB91C	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBE17E564	// c0 (high single)
	.long	0x31C5A2E4	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3CB440D0	// c1 (low single)
	.long	0xBE1B3D00	// c2
	.long	0x3EB9F664	// c3
	.long	0xBDD647C0	// c4
	.long	0xBFB95A9E	// B' = pi/2 - B (high single)
	.long	0x33651267	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBDFC98C2	// c0 (high single)
	.long	0x30AE525C	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3C793D20	// c1 (low single)
	.long	0xBE003845	// c2
	.long	0x3EB5271F	// c3
	.long	0xBDAC669E	// c4
	.long	0xBFBC7EDD	// B' = pi/2 - B (high single)
	.long	0x31800ADD	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBDC9B5DC	// c0 (high single)
	.long	0xB145AD86	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3C1EEF20	// c1 (low single)
	.long	0xBDCBAAEA	// c2
	.long	0x3EB14E5E	// c3
	.long	0xBD858BB2	// c4
	.long	0xBFBFA31C	// B' = pi/2 - B (high single)
	.long	0xB3450FB0	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBD9711CE	// c0 (high single)
	.long	0xB14FEB28	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3BB24C00	// c1 (low single)
	.long	0xBD97E43A	// c2
	.long	0x3EAE6A89	// c3
	.long	0xBD4D07E0	// c4
	.long	0xBFC2C75C	// B' = pi/2 - B (high single)
	.long	0x32CBBE8A	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBD49393C	// c0 (high single)
	.long	0xB0A39F5B	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3B1E2B00	// c1 (low single)
	.long	0xBD49B5D4	// c2
	.long	0x3EAC4F10	// c3
	.long	0xBCFD9425	// c4
	.long	0xBFC5EB9B	// B' = pi/2 - B (high single)
	.long	0xB2DE638C	// B' = pi/2 - B (low single)
	.long	0x00000000	// tau (1 for cot path)
	.long	0xBCC91A31	// c0 (high single)
	.long	0xAF8E8D1A	// c0 (low single)
	.long	0x3F800000	// c1 (high 1 bit)
	.long	0x3A1DFA00	// c1 (low single)
	.long	0xBCC9392D	// c2
	.long	0x3EAB1889	// c3
	.long	0xBC885D3B	// c4
	.align	16
	.type	__svml_stan_data_internal, @object
	.size	__svml_stan_data_internal, .-__svml_stan_data_internal
	.space	16, 0x00
	.align	16

#ifdef __svml_stan_reduction_data_internal_typedef
	typedef	unsigned int VUINT32;
	typedef	struct{
	__declspec (align(16))VUINT32 _sPtable[257][3][1];
	}__svml_stan_reduction_data_internal;
#endif
__svml_stan_reduction_data_internal:
	/* P_hi                  P_med               P_lo.  */
	.long	0x00000000, 0x00000000, 0x00000000	/* 0 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 1 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 2 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 3 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 4 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 5 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 6 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 7 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 8 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 9 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 10 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 11 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 12 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 13 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 14 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 15 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 16 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 17 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 18 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 19 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 20 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 21 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 22 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 23 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 24 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 25 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 26 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 27 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 28 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 29 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 30 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 31 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 32 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 33 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 34 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 35 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 36 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 37 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 38 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 39 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 40 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 41 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 42 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 43 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 44 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 45 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 46 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 47 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 48 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 49 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 50 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 51 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 52 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 53 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 54 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 55 */
	.long	0x00000000, 0x00000000, 0x00000000	/* 56 */
	.long	0x00000000, 0x00000000, 0x00000001	/* 57 */
	.long	0x00000000, 0x00000000, 0x00000002	/* 58 */
	.long	0x00000000, 0x00000000, 0x00000005	/* 59 */
	.long	0x00000000, 0x00000000, 0x0000000A	/* 60 */
	.long	0x00000000, 0x00000000, 0x00000014	/* 61 */
	.long	0x00000000, 0x00000000, 0x00000028	/* 62 */
	.long	0x00000000, 0x00000000, 0x00000051	/* 63 */
	.long	0x00000000, 0x00000000, 0x000000A2	/* 64 */
	.long	0x00000000, 0x00000000, 0x00000145	/* 65 */
	.long	0x00000000, 0x00000000, 0x0000028B	/* 66 */
	.long	0x00000000, 0x00000000, 0x00000517	/* 67 */
	.long	0x00000000, 0x00000000, 0x00000A2F	/* 68 */
	.long	0x00000000, 0x00000000, 0x0000145F	/* 69 */
	.long	0x00000000, 0x00000000, 0x000028BE	/* 70 */
	.long	0x00000000, 0x00000000, 0x0000517C	/* 71 */
	.long	0x00000000, 0x00000000, 0x0000A2F9	/* 72 */
	.long	0x00000000, 0x00000000, 0x000145F3	/* 73 */
	.long	0x00000000, 0x00000000, 0x00028BE6	/* 74 */
	.long	0x00000000, 0x00000000, 0x000517CC	/* 75 */
	.long	0x00000000, 0x00000000, 0x000A2F98	/* 76 */
	.long	0x00000000, 0x00000000, 0x00145F30	/* 77 */
	.long	0x00000000, 0x00000000, 0x0028BE60	/* 78 */
	.long	0x00000000, 0x00000000, 0x00517CC1	/* 79 */
	.long	0x00000000, 0x00000000, 0x00A2F983	/* 80 */
	.long	0x00000000, 0x00000000, 0x0145F306	/* 81 */
	.long	0x00000000, 0x00000000, 0x028BE60D	/* 82 */
	.long	0x00000000, 0x00000000, 0x0517CC1B	/* 83 */
	.long	0x00000000, 0x00000000, 0x0A2F9836	/* 84 */
	.long	0x00000000, 0x00000000, 0x145F306D	/* 85 */
	.long	0x00000000, 0x00000000, 0x28BE60DB	/* 86 */
	.long	0x00000000, 0x00000000, 0x517CC1B7	/* 87 */
	.long	0x00000000, 0x00000000, 0xA2F9836E	/* 88 */
	.long	0x00000000, 0x00000001, 0x45F306DC	/* 89 */
	.long	0x00000000, 0x00000002, 0x8BE60DB9	/* 90 */
	.long	0x00000000, 0x00000005, 0x17CC1B72	/* 91 */
	.long	0x00000000, 0x0000000A, 0x2F9836E4	/* 92 */
	.long	0x00000000, 0x00000014, 0x5F306DC9	/* 93 */
	.long	0x00000000, 0x00000028, 0xBE60DB93	/* 94 */
	.long	0x00000000, 0x00000051, 0x7CC1B727	/* 95 */
	.long	0x00000000, 0x000000A2, 0xF9836E4E	/* 96 */
	.long	0x00000000, 0x00000145, 0xF306DC9C	/* 97 */
	.long	0x00000000, 0x0000028B, 0xE60DB939	/* 98 */
	.long	0x00000000, 0x00000517, 0xCC1B7272	/* 99 */
	.long	0x00000000, 0x00000A2F, 0x9836E4E4	/* 100 */
	.long	0x00000000, 0x0000145F, 0x306DC9C8	/* 101 */
	.long	0x00000000, 0x000028BE, 0x60DB9391	/* 102 */
	.long	0x00000000, 0x0000517C, 0xC1B72722	/* 103 */
	.long	0x00000000, 0x0000A2F9, 0x836E4E44	/* 104 */
	.long	0x00000000, 0x000145F3, 0x06DC9C88	/* 105 */
	.long	0x00000000, 0x00028BE6, 0x0DB93910	/* 106 */
	.long	0x00000000, 0x000517CC, 0x1B727220	/* 107 */
	.long	0x00000000, 0x000A2F98, 0x36E4E441	/* 108 */
	.long	0x00000000, 0x00145F30, 0x6DC9C882	/* 109 */
	.long	0x00000000, 0x0028BE60, 0xDB939105	/* 110 */
	.long	0x00000000, 0x00517CC1, 0xB727220A	/* 111 */
	.long	0x00000000, 0x00A2F983, 0x6E4E4415	/* 112 */
	.long	0x00000000, 0x0145F306, 0xDC9C882A	/* 113 */
	.long	0x00000000, 0x028BE60D, 0xB9391054	/* 114 */
	.long	0x00000000, 0x0517CC1B, 0x727220A9	/* 115 */
	.long	0x00000000, 0x0A2F9836, 0xE4E44152	/* 116 */
	.long	0x00000000, 0x145F306D, 0xC9C882A5	/* 117 */
	.long	0x00000000, 0x28BE60DB, 0x9391054A	/* 118 */
	.long	0x00000000, 0x517CC1B7, 0x27220A94	/* 119 */
	.long	0x00000000, 0xA2F9836E, 0x4E441529	/* 120 */
	.long	0x00000001, 0x45F306DC, 0x9C882A53	/* 121 */
	.long	0x00000002, 0x8BE60DB9, 0x391054A7	/* 122 */
	.long	0x00000005, 0x17CC1B72, 0x7220A94F	/* 123 */
	.long	0x0000000A, 0x2F9836E4, 0xE441529F	/* 124 */
	.long	0x00000014, 0x5F306DC9, 0xC882A53F	/* 125 */
	.long	0x00000028, 0xBE60DB93, 0x91054A7F	/* 126 */
	.long	0x00000051, 0x7CC1B727, 0x220A94FE	/* 127 */
	.long	0x000000A2, 0xF9836E4E, 0x441529FC	/* 128 */
	.long	0x00000145, 0xF306DC9C, 0x882A53F8	/* 129 */
	.long	0x0000028B, 0xE60DB939, 0x1054A7F0	/* 130 */
	.long	0x00000517, 0xCC1B7272, 0x20A94FE1	/* 131 */
	.long	0x00000A2F, 0x9836E4E4, 0x41529FC2	/* 132 */
	.long	0x0000145F, 0x306DC9C8, 0x82A53F84	/* 133 */
	.long	0x000028BE, 0x60DB9391, 0x054A7F09	/* 134 */
	.long	0x0000517C, 0xC1B72722, 0x0A94FE13	/* 135 */
	.long	0x0000A2F9, 0x836E4E44, 0x1529FC27	/* 136 */
	.long	0x000145F3, 0x06DC9C88, 0x2A53F84E	/* 137 */
	.long	0x00028BE6, 0x0DB93910, 0x54A7F09D	/* 138 */
	.long	0x000517CC, 0x1B727220, 0xA94FE13A	/* 139 */
	.long	0x000A2F98, 0x36E4E441, 0x529FC275	/* 140 */
	.long	0x00145F30, 0x6DC9C882, 0xA53F84EA	/* 141 */
	.long	0x0028BE60, 0xDB939105, 0x4A7F09D5	/* 142 */
	.long	0x00517CC1, 0xB727220A, 0x94FE13AB	/* 143 */
	.long	0x00A2F983, 0x6E4E4415, 0x29FC2757	/* 144 */
	.long	0x0145F306, 0xDC9C882A, 0x53F84EAF	/* 145 */
	.long	0x028BE60D, 0xB9391054, 0xA7F09D5F	/* 146 */
	.long	0x0517CC1B, 0x727220A9, 0x4FE13ABE	/* 147 */
	.long	0x0A2F9836, 0xE4E44152, 0x9FC2757D	/* 148 */
	.long	0x145F306D, 0xC9C882A5, 0x3F84EAFA	/* 149 */
	.long	0x28BE60DB, 0x9391054A, 0x7F09D5F4	/* 150 */
	.long	0x517CC1B7, 0x27220A94, 0xFE13ABE8	/* 151 */
	.long	0xA2F9836E, 0x4E441529, 0xFC2757D1	/* 152 */
	.long	0x45F306DC, 0x9C882A53, 0xF84EAFA3	/* 153 */
	.long	0x8BE60DB9, 0x391054A7, 0xF09D5F47	/* 154 */
	.long	0x17CC1B72, 0x7220A94F, 0xE13ABE8F	/* 155 */
	.long	0x2F9836E4, 0xE441529F, 0xC2757D1F	/* 156 */
	.long	0x5F306DC9, 0xC882A53F, 0x84EAFA3E	/* 157 */
	.long	0xBE60DB93, 0x91054A7F, 0x09D5F47D	/* 158 */
	.long	0x7CC1B727, 0x220A94FE, 0x13ABE8FA	/* 159 */
	.long	0xF9836E4E, 0x441529FC, 0x2757D1F5	/* 160 */
	.long	0xF306DC9C, 0x882A53F8, 0x4EAFA3EA	/* 161 */
	.long	0xE60DB939, 0x1054A7F0, 0x9D5F47D4	/* 162 */
	.long	0xCC1B7272, 0x20A94FE1, 0x3ABE8FA9	/* 163 */
	.long	0x9836E4E4, 0x41529FC2, 0x757D1F53	/* 164 */
	.long	0x306DC9C8, 0x82A53F84, 0xEAFA3EA6	/* 165 */
	.long	0x60DB9391, 0x054A7F09, 0xD5F47D4D	/* 166 */
	.long	0xC1B72722, 0x0A94FE13, 0xABE8FA9A	/* 167 */
	.long	0x836E4E44, 0x1529FC27, 0x57D1F534	/* 168 */
	.long	0x06DC9C88, 0x2A53F84E, 0xAFA3EA69	/* 169 */
	.long	0x0DB93910, 0x54A7F09D, 0x5F47D4D3	/* 170 */
	.long	0x1B727220, 0xA94FE13A, 0xBE8FA9A6	/* 171 */
	.long	0x36E4E441, 0x529FC275, 0x7D1F534D	/* 172 */
	.long	0x6DC9C882, 0xA53F84EA, 0xFA3EA69B	/* 173 */
	.long	0xDB939105, 0x4A7F09D5, 0xF47D4D37	/* 174 */
	.long	0xB727220A, 0x94FE13AB, 0xE8FA9A6E	/* 175 */
	.long	0x6E4E4415, 0x29FC2757, 0xD1F534DD	/* 176 */
	.long	0xDC9C882A, 0x53F84EAF, 0xA3EA69BB	/* 177 */
	.long	0xB9391054, 0xA7F09D5F, 0x47D4D377	/* 178 */
	.long	0x727220A9, 0x4FE13ABE, 0x8FA9A6EE	/* 179 */
	.long	0xE4E44152, 0x9FC2757D, 0x1F534DDC	/* 180 */
	.long	0xC9C882A5, 0x3F84EAFA, 0x3EA69BB8	/* 181 */
	.long	0x9391054A, 0x7F09D5F4, 0x7D4D3770	/* 182 */
	.long	0x27220A94, 0xFE13ABE8, 0xFA9A6EE0	/* 183 */
	.long	0x4E441529, 0xFC2757D1, 0xF534DDC0	/* 184 */
	.long	0x9C882A53, 0xF84EAFA3, 0xEA69BB81	/* 185 */
	.long	0x391054A7, 0xF09D5F47, 0xD4D37703	/* 186 */
	.long	0x7220A94F, 0xE13ABE8F, 0xA9A6EE06	/* 187 */
	.long	0xE441529F, 0xC2757D1F, 0x534DDC0D	/* 188 */
	.long	0xC882A53F, 0x84EAFA3E, 0xA69BB81B	/* 189 */
	.long	0x91054A7F, 0x09D5F47D, 0x4D377036	/* 190 */
	.long	0x220A94FE, 0x13ABE8FA, 0x9A6EE06D	/* 191 */
	.long	0x441529FC, 0x2757D1F5, 0x34DDC0DB	/* 192 */
	.long	0x882A53F8, 0x4EAFA3EA, 0x69BB81B6	/* 193 */
	.long	0x1054A7F0, 0x9D5F47D4, 0xD377036D	/* 194 */
	.long	0x20A94FE1, 0x3ABE8FA9, 0xA6EE06DB	/* 195 */
	.long	0x41529FC2, 0x757D1F53, 0x4DDC0DB6	/* 196 */
	.long	0x82A53F84, 0xEAFA3EA6, 0x9BB81B6C	/* 197 */
	.long	0x054A7F09, 0xD5F47D4D, 0x377036D8	/* 198 */
	.long	0x0A94FE13, 0xABE8FA9A, 0x6EE06DB1	/* 199 */
	.long	0x1529FC27, 0x57D1F534, 0xDDC0DB62	/* 200 */
	.long	0x2A53F84E, 0xAFA3EA69, 0xBB81B6C5	/* 201 */
	.long	0x54A7F09D, 0x5F47D4D3, 0x77036D8A	/* 202 */
	.long	0xA94FE13A, 0xBE8FA9A6, 0xEE06DB14	/* 203 */
	.long	0x529FC275, 0x7D1F534D, 0xDC0DB629	/* 204 */
	.long	0xA53F84EA, 0xFA3EA69B, 0xB81B6C52	/* 205 */
	.long	0x4A7F09D5, 0xF47D4D37, 0x7036D8A5	/* 206 */
	.long	0x94FE13AB, 0xE8FA9A6E, 0xE06DB14A	/* 207 */
	.long	0x29FC2757, 0xD1F534DD, 0xC0DB6295	/* 208 */
	.long	0x53F84EAF, 0xA3EA69BB, 0x81B6C52B	/* 209 */
	.long	0xA7F09D5F, 0x47D4D377, 0x036D8A56	/* 210 */
	.long	0x4FE13ABE, 0x8FA9A6EE, 0x06DB14AC	/* 211 */
	.long	0x9FC2757D, 0x1F534DDC, 0x0DB62959	/* 212 */
	.long	0x3F84EAFA, 0x3EA69BB8, 0x1B6C52B3	/* 213 */
	.long	0x7F09D5F4, 0x7D4D3770, 0x36D8A566	/* 214 */
	.long	0xFE13ABE8, 0xFA9A6EE0, 0x6DB14ACC	/* 215 */
	.long	0xFC2757D1, 0xF534DDC0, 0xDB629599	/* 216 */
	.long	0xF84EAFA3, 0xEA69BB81, 0xB6C52B32	/* 217 */
	.long	0xF09D5F47, 0xD4D37703, 0x6D8A5664	/* 218 */
	.long	0xE13ABE8F, 0xA9A6EE06, 0xDB14ACC9	/* 219 */
	.long	0xC2757D1F, 0x534DDC0D, 0xB6295993	/* 220 */
	.long	0x84EAFA3E, 0xA69BB81B, 0x6C52B327	/* 221 */
	.long	0x09D5F47D, 0x4D377036, 0xD8A5664F	/* 222 */
	.long	0x13ABE8FA, 0x9A6EE06D, 0xB14ACC9E	/* 223 */
	.long	0x2757D1F5, 0x34DDC0DB, 0x6295993C	/* 224 */
	.long	0x4EAFA3EA, 0x69BB81B6, 0xC52B3278	/* 225 */
	.long	0x9D5F47D4, 0xD377036D, 0x8A5664F1	/* 226 */
	.long	0x3ABE8FA9, 0xA6EE06DB, 0x14ACC9E2	/* 227 */
	.long	0x757D1F53, 0x4DDC0DB6, 0x295993C4	/* 228 */
	.long	0xEAFA3EA6, 0x9BB81B6C, 0x52B32788	/* 229 */
	.long	0xD5F47D4D, 0x377036D8, 0xA5664F10	/* 230 */
	.long	0xABE8FA9A, 0x6EE06DB1, 0x4ACC9E21	/* 231 */
	.long	0x57D1F534, 0xDDC0DB62, 0x95993C43	/* 232 */
	.long	0xAFA3EA69, 0xBB81B6C5, 0x2B327887	/* 233 */
	.long	0x5F47D4D3, 0x77036D8A, 0x5664F10E	/* 234 */
	.long	0xBE8FA9A6, 0xEE06DB14, 0xACC9E21C	/* 235 */
	.long	0x7D1F534D, 0xDC0DB629, 0x5993C439	/* 236 */
	.long	0xFA3EA69B, 0xB81B6C52, 0xB3278872	/* 237 */
	.long	0xF47D4D37, 0x7036D8A5, 0x664F10E4	/* 238 */
	.long	0xE8FA9A6E, 0xE06DB14A, 0xCC9E21C8	/* 239 */
	.long	0xD1F534DD, 0xC0DB6295, 0x993C4390	/* 240 */
	.long	0xA3EA69BB, 0x81B6C52B, 0x32788720	/* 241 */
	.long	0x47D4D377, 0x036D8A56, 0x64F10E41	/* 242 */
	.long	0x8FA9A6EE, 0x06DB14AC, 0xC9E21C82	/* 243 */
	.long	0x1F534DDC, 0x0DB62959, 0x93C43904	/* 244 */
	.long	0x3EA69BB8, 0x1B6C52B3, 0x27887208	/* 245 */
	.long	0x7D4D3770, 0x36D8A566, 0x4F10E410	/* 246 */
	.long	0xFA9A6EE0, 0x6DB14ACC, 0x9E21C820	/* 247 */
	.long	0xF534DDC0, 0xDB629599, 0x3C439041	/* 248 */
	.long	0xEA69BB81, 0xB6C52B32, 0x78872083	/* 249 */
	.long	0xD4D37703, 0x6D8A5664, 0xF10E4107	/* 250 */
	.long	0xA9A6EE06, 0xDB14ACC9, 0xE21C820F	/* 251 */
	.long	0x534DDC0D, 0xB6295993, 0xC439041F	/* 252 */
	.long	0xA69BB81B, 0x6C52B327, 0x8872083F	/* 253 */
	.long	0x4D377036, 0xD8A5664F, 0x10E4107F	/* 254 */
	.long	0x9A6EE06D, 0xB14ACC9E, 0x21C820FF	/* 255 */
    .long   0x00000000, 0x00000000, 0x00000000
	.align	16
	.type	__svml_stan_reduction_data_internal, @object
	.size	__svml_stan_reduction_data_internal, .-__svml_stan_reduction_data_internal
	.align	16
