/* Core .*/
/* Function tanf vectorized with AVX2.
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
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-avx2-rodata-offsets.h"

#define AVX2_SHARED_TABLE
#define AVX512_SHARED_OFFSETS
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-tanf-rodata.S"


#define _sPI2_FMA	0
#define _sPI3_FMA	32
#define _FLT_0	64
#define _FLT_1	96
#define _FLT_2	128
#define _FLT_3	160


#define xmmA	xmm1
#define ymmA	ymm1

	.section .text.avx2, "ax", @progbits
ENTRY(tanf_8_avx2_dev)
	vmovups	COMMON_DATA(_AbsMask)(%rip), %ymm6
	/* Main path (_LA_ and _EP_)
	   Octant calculation.  */
	vmovups	AVX2_SHARED_DATA(_sInvPi)(%rip), %ymm5
	vmovups	AVX2_SHARED_DATA(_sRShifter)(%rip), %ymm2


	vandps	%ymm6, %ymm0, %ymmA

	vfmadd213ps %ymm2, %ymmA, %ymm5
	vsubps	%ymm2, %ymm5, %ymm7

	/* Range reduction.  */
	vmovups	COMMON_DATA(_TanSPI1_FMA)(%rip), %ymm3
	vfnmadd213ps %ymmA, %ymm7, %ymm3

	vfnmadd231ps LOCAL_DATA(_sPI2_FMA)(%rip), %ymm7, %ymm3
	vfnmadd132ps LOCAL_DATA(_sPI3_FMA)(%rip), %ymm3, %ymm7
	vmovups	AVX2_SHARED_DATA(_sQ2)(%rip), %ymm3
	/* Rational approximation.  */
	vmovups	AVX2_SHARED_DATA(_sP1)(%rip), %ymm4

	vmulps	%ymm7, %ymm7, %ymm2
	vfmadd213ps AVX2_SHARED_DATA(_sQ1)(%rip), %ymm2, %ymm3
	vmovups	AVX2_SHARED_DATA(_sP0)(%rip), %ymm8
	vfmadd213ps %ymm8, %ymm2, %ymm4
	vfmadd213ps %ymm8, %ymm2, %ymm3
	vmulps	%ymm4, %ymm7, %ymm4
	/* Inversion mask and sign calculation.  */
	vpslld	$31, %ymm5, %ymm2


	vandnps	%ymm0, %ymm6, %ymm7

	/* Exchanged numerator and denominator if necessary.  */
	vblendvps %ymm2, %ymm3, %ymm4, %ymm6
	vblendvps %ymm2, %ymm4, %ymm3, %ymm3

	/* Large values check.  */
	vpcmpgtd AVX2_SHARED_DATA(_sRangeReductionVal)(%rip), %ymmA, %ymm10
	vpmovmskb %ymm10, %edx

	/* Division.  */
	vdivps	%ymm3, %ymm6, %ymm3

	/* End of main path (_LA_ and _EP_).  */
	testl	%edx, %edx

	/* Go to auxilary branch.  */
	jne	L(AUX_BRANCH)

	vxorps	%ymm2, %ymm7, %ymm7
	/* Sign setting.  */
	vxorps	%ymm7, %ymm3, %ymm0
	ret


L(AUX_BRANCH):
	/* Sign setting. NB for all special case values this is
	   equivilent to the input (ymm0).  */
	vpandn	%ymm3, %ymm10, %ymm3
	vpxor	%ymm3, %ymm7, %ymm12
	vmovaps	%ymm0, %ymm11


	/* Get the (2^a / 2pi) mod 1 values from the table.  */
	lea	AVX512_SHARED_DATA(_Reduction)(%rip), %rdx

	vpsrld	$23, %ymmA, %ymm6
	vpaddd	%ymm6, %ymm6, %ymm2

	vpaddd	%ymm6, %ymm2, %ymm3

	/* Collect indexes.  */
	vmovq	%xmm3, %rax
	movl	%eax, %ecx
	shrq	$32, %rax

	vmovq	(%rdx, %rcx, 4), %xmm4
	vmovq	(%rdx, %rax, 4), %xmm5
	vpunpckldq %xmm5, %xmm4, %xmm4

	vpextrq	$1, %xmm3, %rdi
	movl	%edi, %esi
	shrq	$32, %rdi

	vmovq	(%rdx, %rsi, 4), %xmm2
	vmovq	(%rdx, %rdi, 4), %xmm5
	vpunpckldq %xmm5, %xmm2, %xmm2

	vextractf128 $1, %ymm3, %xmm7

	vmovq	%xmm7, %r10
	movl	%r10d, %r8d
	shrq	$32, %r10

	vmovq	(%rdx, %r8, 4), %xmm3
	vmovq	(%rdx, %r10, 4), %xmm5
	vpunpckldq %xmm5, %xmm3, %xmm3

	vpextrq	$1, %xmm7, %r11
	movl	%r11d, %r9d
	shrq	$32, %r11

	vmovq	(%rdx, %r9, 4), %xmm7
	vmovq	(%rdx, %r11, 4), %xmm5
	vpunpckldq %xmm5, %xmm7, %xmm7

	vinsertf128 $1, %xmm3, %ymm4, %ymm4
	vinsertf128 $1, %xmm7, %ymm2, %ymm2

	vmovdqa	LOCAL_DATA(_FLT_0)(%rip), %ymm9


	vpunpcklqdq %ymm2, %ymm4, %ymm7
	vpunpckhqdq %ymm2, %ymm4, %ymm6

	/* Break the P_xxx and m into 16-bit chunks ready for
	   the long multiplication via 16x16->32 multiplications.  */
	vpandn	%ymmA, %ymm9, %ymm5
	vpsrld	$16, %ymm5, %ymm3

	vpor	LOCAL_DATA(_FLT_1)(%rip), %ymm3, %ymm4
	vmovd	8(%rdx, %rcx, 4), %xmm5
	vmovd	8(%rdx, %rax, 4), %xmm2
	vpunpckldq %xmm2, %xmm5, %xmm3

	vmovd	8(%rdx, %rsi, 4), %xmm2
	vmovd	8(%rdx, %rdi, 4), %xmm5
	vpunpckldq %xmm5, %xmm2, %xmm2

	vpunpcklqdq %xmm2, %xmm3, %xmm13

	vmovd	8(%rdx, %r8, 4), %xmm3
	vmovd	8(%rdx, %r10, 4), %xmm5
	vpunpckldq %xmm5, %xmm3, %xmm0
	/* Also get the significand as an integer
	   NB: adding in the integer bit is wrong for denorms!
	   To make this work for denorms we should do something
	   slightly different.  */
	vmovd	8(%rdx, %r9, 4), %xmm2
	vmovd	8(%rdx, %r11, 4), %xmm5
	vpunpckldq %xmm5, %xmm2, %xmm2

	/* Better to use `vpand` than `vpblendw`.  */
	vmovdqu	AVX2_SHARED_DATA(_Low16)(%rip), %ymm3

	vpunpcklqdq %xmm2, %xmm0, %xmm0
	vinsertf128 $1, %xmm0, %ymm13, %ymm13

	vpand	%ymm3, %ymmA, %ymm5
	vpand	%ymm3, %ymm6, %ymm0
	vpsrld	$16, %ymm7, %ymm2
	vpand	%ymm3, %ymm7, %ymm7
	vpmulld	%ymm0, %ymm5, %ymm8
	vpmulld	%ymm0, %ymm4, %ymm14
	vpsrld	$16, %ymm6, %ymm0
	vpmulld	%ymm2, %ymm5, %ymm2
	vpand	%ymm3, %ymm2, %ymm15
	vpsrld	$16, %ymm13, %ymm6
	vpand	%ymm3, %ymm13, %ymm2
	vpmulld	%ymm6, %ymm4, %ymm13
	vpmulld	%ymm6, %ymm5, %ymm6
	vpsrld	$16, %ymm6, %ymm6
	vpmulld	%ymm2, %ymm4, %ymm2
	vpaddd	%ymm6, %ymm13, %ymm13
	vpsrld	$16, %ymm2, %ymm6
	vpand	%ymm3, %ymm8, %ymm2
	vpsrld	$16, %ymm8, %ymm8
	vpaddd	%ymm13, %ymm2, %ymm13
	vpmulld	%ymm0, %ymm5, %ymm2
	vpaddd	%ymm13, %ymm6, %ymm13
	vpaddd	%ymm8, %ymm14, %ymm14
	vpand	%ymm3, %ymm2, %ymm8
	vpsrld	$16, %ymm2, %ymm2
	vpsrld	$16, %ymm13, %ymm6

	/* Assemble reduced argument from the pieces.  */
	vpand	%ymm3, %ymm13, %ymm13
	vpaddd	%ymm14, %ymm8, %ymm8
	vpmulld	%ymm7, %ymm5, %ymm5
	vpmulld	%ymm0, %ymm4, %ymm0
	vpaddd	%ymm8, %ymm6, %ymm8
	vpand	%ymm3, %ymm5, %ymm6
	vpaddd	%ymm2, %ymm0, %ymm0
	vpsrld	$16, %ymm5, %ymm14
	vpsrld	$16, %ymm8, %ymm5
	vpslld	$16, %ymm8, %ymm8
	vpaddd	%ymm0, %ymm6, %ymm6
	vpaddd	%ymm6, %ymm5, %ymm0


	/* Now do the big multiplication and carry propagation.  */
	vpmulld	%ymm7, %ymm4, %ymm7
	vpaddd	%ymm14, %ymm7, %ymm5
	vpsrld	$16, %ymm0, %ymm2
	vpand	%ymm3, %ymm0, %ymm0

	vpaddd	%ymm5, %ymm15, %ymm7
	vpaddd	%ymm13, %ymm8, %ymm8

	/* Now round at the 2^-8 bit position for reduction mod pi/2^7
	   instead of the original 2pi (but still with the same 2pi scaling).
	   Use a shifter of 2^15 + 2^14.
	   The N we get is our final version; it has an offset of
	   2^8 because of the implicit integer bit, and anyway for negative
	   starting value it's a 2s complement thing. But we need to mask
	   off the exponent part anyway so it's fine.  */
	vpaddd	%ymm7, %ymm2, %ymm2
	vmovups	AVX2_SHARED_DATA(_SH_FLT_1)(%rip), %ymm14

	/* Create floating-point low and medium parts, respectively
	   lo_17, ... lo_0, 0, ..., 0
	   hi_8, ... hi_0, lo_31, ..., lo_18
	   then subtract off the implicitly added integer bits,
	   2^-46 and 2^-23, respectively.
	   Put the original sign into all of them at this stage.  */
	vmovdqa	AVX2_SHARED_DATA(_SH_FLT_2)(%rip), %ymm7

	vpslld	$16, %ymm2, %ymm2
	vpaddd	%ymm0, %ymm2, %ymm2
	vpand	AVX2_SHARED_DATA(_Low18)(%rip), %ymm8, %ymm0
	vpsrld	$18, %ymm8, %ymm8
	vpsrld	$9, %ymm2, %ymm6
	vpslld	$5, %ymm0, %ymm4
	vmovdqa	COMMON_DATA(_OneF)(%rip), %ymm15
	vpor	%ymm15, %ymm6, %ymm6


	vpand	AVX2_SHARED_DATA(_Low9)(%rip), %ymm2, %ymm3
	vpor	%ymm7, %ymm4, %ymm5
	vmovdqa	AVX2_SHARED_DATA(_SH_FLT_3)(%rip), %ymm4

	/* If the magnitude of the input is <= 2^-20, then
	   just pass through the input, since no reduction will be needed
	   and the main path will only work accurately if the reduced
	   argument is about >= 2^-40 (which it is for all large pi
	   multiples).  */
	vpslld	$14, %ymm3, %ymm2

	/* Now multiply those numbers all by 2 pi, reasonably accurately.
	   (RHi + RLo) * (pi_lead + pi_trail) ~=
	   RHi * pi_lead + (RHi * pi_trail + RLo * pi_lead).  */
	vmovups	AVX2_SHARED_DATA(_SH_FLT_4)(%rip), %ymm3
	vaddps	%ymm14, %ymm6, %ymm13
	vpor	%ymm8, %ymm2, %ymm2
	vsubps	%ymm14, %ymm13, %ymm0

	/* Grab our final N value as an integer, appropriately masked
	   mod 2^8.  */
	vpor	%ymm4, %ymm2, %ymm2
	vsubps	%ymm0, %ymm6, %ymm6
	vsubps	%ymm7, %ymm5, %ymm0
	vsubps	%ymm4, %ymm2, %ymm14

	vmovups	LOCAL_DATA(_FLT_2)(%rip), %ymm4
	/* Now add them up into 2 reasonably aligned pieces.  */
	vaddps	%ymm14, %ymm6, %ymm2
	vsubps	%ymm2, %ymm6, %ymm6
	vmulps	%ymm2, %ymm3, %ymm7
	vaddps	%ymm6, %ymm14, %ymm8
	vaddps	%ymm8, %ymm0, %ymm8
	vmovaps	%ymm3, %ymm0
	vfmsub213ps %ymm7, %ymm2, %ymm0


	vfmadd132ps LOCAL_DATA(_FLT_3)(%rip), %ymm0, %ymm2
	vpcmpgtd %ymm4, %ymmA, %ymm5

	/* The output is _VRES_R (high) + _VRES_E (low), and the integer
	   part is _VRES_IND Set sRp2 = _VRES_R^2 and then resume the
	   original code. Argument reduction is now finished: x = n *
	   pi/128 + r where n = iIndex and r = sR (high) + sE (low).
	   But we have n modulo 256, needed for sin/cos with period 2pi
	   but we want it modulo 128 since tan has period pi.  */
	vpand	AVX2_SHARED_DATA(_Low7)(%rip), %ymm13, %ymm0
	vfmadd213ps %ymm2, %ymm3, %ymm8



	vpslld	$2, %ymm0, %ymm2
	vpaddd	%ymm0, %ymm2, %ymm4

	vpblendvb %ymm5, %ymm7, %ymmA, %ymm6

	vandps	%ymm8, %ymm5, %ymm3
	vaddps	%ymm3, %ymm6, %ymm6




	/* Simply combine the two parts of the reduced argument
	   since we can afford a few ulps in this case.  */

	/* Load constants (not all needed at once).  */
	lea	AVX2_SHARED_DATA(_Coeffs)(%rip), %rdx

	vmovq	%xmm4, %rcx
	movl	%ecx, %eax
	shrq	$32, %rcx

	vmovdqu	(%rdx, %rax, 8), %ymm5
	vmovdqu	(%rdx, %rcx, 8), %ymm7
	vpunpckldq %ymm7, %ymm5, %ymm3
	vpunpckhdq %ymm7, %ymm5, %ymm7

	vpextrq	$1, %xmm4, %rsi
	movl	%esi, %edi
	shrq	$32, %rsi

	vmovdqu	(%rdx, %rdi, 8), %ymm5
	vmovdqu	(%rdx, %rsi, 8), %ymm2
	vpunpckldq %ymm2, %ymm5, %ymm0
	vpunpckhdq %ymm2, %ymm5, %ymm2

	vextractf128 $1, %ymm4, %xmm4

	vmovq	%xmm4, %r8
	movl	%r8d, %r10d
	shrq	$32, %r8

	vmovdqu	(%rdx, %r10, 8), %ymm8
	vmovdqu	(%rdx, %r8, 8), %ymm5
	vpunpckldq %ymm5, %ymm8, %ymm14
	vpunpckhdq %ymm5, %ymm8, %ymm8


	vpextrq	$1, %xmm4, %r11
	movl	%r11d, %r9d
	shrq	$32, %r11

	vmovdqu	(%rdx, %r9, 8), %ymm5
	vmovdqu	(%rdx, %r11, 8), %ymm4

	vpunpckldq %ymm4, %ymm5, %ymm13
	vpunpckhdq %ymm4, %ymm5, %ymm4

	vpunpcklqdq %ymm0, %ymm3, %ymm5
	vpunpckhqdq %ymm0, %ymm3, %ymm3

	vpunpcklqdq %ymm13, %ymm14, %ymm0
	vpunpckhqdq %ymm13, %ymm14, %ymm14

	vinserti128 $0x1, %xmm0, %ymm5, %ymm13
	vperm2i128 $0x31, %ymm0, %ymm5, %ymm5

	vinserti128 $0x1, %xmm14, %ymm3, %ymm0
	vperm2i128 $0x31, %ymm14, %ymm3, %ymm14

	vpunpcklqdq %ymm2, %ymm7, %ymm3
	vpunpckhqdq %ymm2, %ymm7, %ymm2

	vpunpcklqdq %ymm4, %ymm8, %ymm7
	vpunpckhqdq %ymm4, %ymm8, %ymm4

	vinserti128 $0x1, %xmm7, %ymm3, %ymm8
	vperm2i128 $0x31, %ymm7, %ymm3, %ymm3

	vperm2i128 $0x31, %ymm4, %ymm2, %ymm7
	vfmadd213ps %ymm3, %ymm6, %ymm7
	vinserti128 $0x1, %xmm4, %ymm2, %ymm3

	/* Compute 2-part reciprocal component Construct a separate
	   reduced argument modulo pi near pi/2 multiples. i.e. (pi/2 -
	   x) mod pi, simply by subtracting the reduced argument from
	   an accurate B_hi + B_lo = (128 - n) pi/128. Force the upper
	   part of this reduced argument to half-length to simplify
	   accurate reciprocation later on.  */
	vsubps	%ymm6, %ymm13, %ymm2
	vsubps	%ymm2, %ymm13, %ymm13
	vsubps	%ymm6, %ymm13, %ymm13

	/* Higher polynomial terms
	   Stage 1 (with unlimited parallelism)
	   P3 = C1_lo + C2 * Z.  */
	vmovq	32(%rdx, %rax, 8), %xmm4
	vpcmpgtd %ymmA, %ymm9, %ymm9
	vmovmskps %ymm9, %eax

	vmovq	32(%rdx, %rcx, 8), %xmmA

	vinsertf128 $1, 32(%rdx, %r10, 8), %ymm4, %ymm4
	vinsertf128 $1, 32(%rdx, %r8, 8), %ymmA, %ymmA
	vpunpckldq %ymmA, %ymm4, %ymm9

	vmovq	32(%rdx, %rdi, 8), %xmmA
	vmovq	32(%rdx, %rsi, 8), %xmm4

	vinsertf128 $1, 32(%rdx, %r9, 8), %ymmA, %ymmA
	vinsertf128 $1, 32(%rdx, %r11, 8), %ymm4, %ymm4
	vpunpckldq %ymm4, %ymmA, %ymmA

	vpunpckhqdq %ymmA, %ymm9, %ymm4
	vpunpcklqdq %ymmA, %ymm9, %ymm9


	vmovups	COMMON_DATA(_Neg4096)(%rip), %ymmA

	vfmadd213ps %ymm9, %ymm6, %ymm4

	vandps	%ymmA, %ymm2, %ymm9
	vsubps	%ymm9, %ymm2, %ymm2

	/* P4 = C3 + C4 * Z.  */
	vaddps	%ymm2, %ymm0, %ymm0
	vaddps	%ymm0, %ymm13, %ymm0

	/* Now compute an approximate reciprocal to mix into the computation
	   To avoid any danger of nonportability, force it to 12 bits,
	   though I suspect it always is anyway on current platforms.  */
	vrcpps	%ymm9, %ymm13
	vandps	%ymmA, %ymm13, %ymm13

	/* Now compute the error sEr where sRecip_hi = (1/R_hi) * (1 - sEr)
	   so that we can compensate for it.  */
	vfnmadd213ps %ymm15, %ymm13, %ymm9

	/* Get a better approximation to  1/sR_hi (not far short of an ulp)
	   using a third-order polynomial approximation.  */
	vmovaps	%ymm13, %ymm2
	vfmadd231ps %ymm9, %ymm9, %ymm15
	vfmadd213ps %ymm13, %ymm9, %ymm2
	vmulps	%ymm15, %ymm2, %ymmA

	/* Multiply by sRecip_ok to make sR_lo relative to sR_hi Since
	   sR_lo is shifted off by about 12 bits, this is accurate
	   enough.  */
	vmulps	%ymmA, %ymm0, %ymm2

	/* Now create a low reciprocal using
	   (Recip_hi + Er * Recip_ok) * (1 + sR_lo^2 - sR_lo)
	   =~= Recip_hi + Recip_ok * (Er + sR_lo^2 - sR_lo).  */
	vsubps	%ymm9, %ymm2, %ymm9
	vfmsub213ps %ymm9, %ymm2, %ymm2
	vmulps	%ymm2, %ymmA, %ymm9
	vmovaps	%ymm14, %ymmA
	vfmadd213ps %ymm3, %ymm6, %ymmA
	vsubps	%ymmA, %ymm3, %ymm0
	vmovaps	%ymm8, %ymm3
	vfmadd213ps %ymmA, %ymm13, %ymm3
	vfmadd213ps %ymm0, %ymm6, %ymm14
	vfmsub213ps %ymm3, %ymm8, %ymm13
	vfmadd213ps %ymm14, %ymm8, %ymm9
	vaddps	%ymm13, %ymmA, %ymm2

	/* Z2 = Z^2.  */
	vmulps	%ymm6, %ymm6, %ymmA
	vaddps	%ymm2, %ymm9, %ymm2

	/* Stage 2 (with unlimited parallelism)
	   P6 = C1_lo + C2 * Z + C3 * Z^2 + C4 * Z^3.  */
	vfmadd213ps %ymm7, %ymm4, %ymmA

	/* P9 = trail(dominant part) + C0_lo.  */
	vaddps	%ymm5, %ymm2, %ymm4

	/* Final accumulation of low part.  */
	vfmadd213ps %ymm4, %ymm6, %ymmA

	/* And now the very final summation.  */
	vaddps	%ymmA, %ymm3, %ymm6

	/* The end of implementation (LA with huge args reduction)
	   End of large arguments path (_HA_, _LA_ and _EP_).  */
	vpand	%ymm6, %ymm10, %ymm6
	/* Merge results from main and large paths:.  */
	vpxor	%ymm6, %ymm12, %ymm0


	/* `al` has 0 at special values. If all 1s `incb al` will
	   overflow and set zero flag.  */
	incb	%al
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)
	ret

	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a tanhf call. Optimize for code size
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


	/* Save original input (ymm0 unchanged up to this point).  */
	vmovaps	%ymm11, 32(%rsp)
	vmovaps	%ymm0, (%rsp)

	vzeroupper

	/* eax has 1s where there was a special value that needs to be
	   handled by a tanf call.  */
	negb	%al
	movzbl	%al, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to tanhf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 28] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop. Realigning
	   also costs more code size.  */
	xorl	%ebp, %ebp
	tzcntl	%ebx, %ebp

	/* Scalar math function call to process special input.  */
	vmovss	32(%rsp, %rbp, 4), %xmm0
	call	tanf@PLT

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
END(tanf_8_avx2_dev)


	.section .rodata.avx2, "a"
	.align	32
LOCAL_DATA_NAME:

	DATA_VEC (LOCAL_DATA_NAME, _sPI2_FMA, 0xB33BBD2E)	// AVX2
	DATA_VEC (LOCAL_DATA_NAME, _sPI3_FMA, 0xA6F72CED)	// AVX2
	DATA_VEC (LOCAL_DATA_NAME, _FLT_0, 0x7f800000)	// AVX512, AVX2
	DATA_VEC (LOCAL_DATA_NAME, _FLT_1, 0x00000080)	// AVX2
	DATA_VEC (LOCAL_DATA_NAME, _FLT_2, 0x35800000)	// AVX512, AVX2
	DATA_VEC (LOCAL_DATA_NAME, _FLT_3, 0xb43bbd2e)	// AVX512, AVX2

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
