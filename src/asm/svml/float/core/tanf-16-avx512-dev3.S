/* Core .*/
/* Function tanf vectorized with AVX-512.
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
 *      ( optimized for throughput, with small table lookup, works when HW FMA is available )
 *
 *       Implementation reduces argument x to |R|<pi/64
 *       32-entry tables used to store high and low parts of tan(x0)
 *       Argument x = N*pi + x0 + (R);   x0 = k*pi/32, with k in {0, 1, ..., 31}
 *       (very large arguments reduction resolved in _vsreduction_core.i)
 *       Compute result as (tan(x0) + tan(R))/(1-tan(x0)*tan(R))
 *       _HA_ version keeps extra precision for numerator, denominator, and during
 *       final NR-iteration computing quotient.
 *
 *
 */

#include "/home/noah/programs/projects/svml-new/src/asm/libc-asm-common.h"
#define LOCAL_DATA_NAME	__svml_stan_data_internal
#define LOCAL_DATA_NAME_UNALIGNED	__svml_stan_data_internal_unaligned
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-evex512-rodata-offsets.h"

#define AVX512_SHARED_TABLE
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-tanf-rodata.S"

	/* Offsets for data table __svml_stan_data_internal.  */
#define _sInvPI_uisa	0
#define _sRShifter	64
#define _sPI1_uisa	128
#define _sPI2_uisa	192
#define _sPI3_uisa	256
#define _sRangeReductionVal_uisa	320
#define _sPC5_uisa	384
#define _sPC3_uisa	448
#define _Th_tbl_uisa_lo	512
#define _Th_tbl_uisa_hi	576
#define _sRangeVal	640
#define _FLT_1	704
#define _FLT_2	768
#define _FLT_3	832
#define _FLT_4	896
#define _FLT_5	960
#define _FLT_6	1024
#define _FLT_7	1088


#define _FLT_1_1to16	0
#define _FLT_2_1to16	4
#define _FLT_3_1to16	8


#define zmmA	zmm1

/* 0, 1, or 2. The following values get the following ULP breakdowns:
   PRECISION == 0:
       ulp:
	   0  :	 3374033104 (0.7856)
	   1  :   893707604 (0.2081)
	   2  :	   26831634 (0.0062)
	   3  :      393466 (0.0001)
	   4  :        1488 (0.0000)
	   Avg: 0.2209

   PRECISION == 1:
       ulp:
	   0  : 3677094430 (0.8561)
	   1  :  609296734 (0.1419)
	   2  :    8347192 (0.0019)
	   3  :     228138 (0.0001)
	   4  :        802 (0.0000)
	   Avg: 0.1459
	
   PRECISION == 2:
       ulp:
	   error breakdown:
	   0  :	 3722920128 (0.8668)
	   1  :   566817724 (0.1320)
	   2  :	    5022802 (0.0012)
	   3  :      205902 (0.0000)
	   4  :         740 (0.0000)
	   Avg: 0.1345  */
#define PRECISION	0


	.section .text.evex512, "ax", @progbits
ENTRY(tanf_16_avx512_dev)
	/* Main path start arg. reduction.  */
	vmovups	LOCAL_DATA(_sInvPI_uisa)(%rip), %zmm10
	vmovups	LOCAL_DATA(_sRShifter)(%rip), %zmmA
	vfmadd213ps {rn-sae}, %zmmA, %zmm0, %zmm10
	vsubps	{rn-sae}, %zmmA, %zmm10, %zmm5
	vmovups	LOCAL_DATA(_sPI1_uisa)(%rip), %zmm4
	vfnmadd213ps {rn-sae}, %zmm0, %zmm5, %zmm4
	vmovups	LOCAL_DATA(_sPI2_uisa)(%rip), %zmm2
	vfnmadd231ps {rn-sae}, %zmm5, %zmm2, %zmm4
	vmovups	LOCAL_DATA(_sPI3_uisa)(%rip), %zmm3
	vfnmadd213ps {rn-sae}, %zmm4, %zmm3, %zmm5


	/* Reused throughout in large case.  */
	vmovaps	COMMON_DATA(_AbsMask)(%rip), %zmm7

	/* Large values check.  */
	vmovups	LOCAL_DATA(_sRangeReductionVal_uisa)(%rip), %zmm6
	vandps	%zmm7, %zmm0, %zmm11
	vcmpps	$22, {sae}, %zmm6, %zmm11, %k6

	ktestd	%k6, %k6
	/* Go to auxilary branch.  */
	jne	L(AUX_BRANCH)


	/* Table lookup.  */
	vmulps	{rn-sae}, %zmm5, %zmm5, %zmmA
	vmovups	LOCAL_DATA(_sPC5_uisa)(%rip), %zmm4
	vmovups	LOCAL_DATA(_sPC3_uisa)(%rip), %zmm11
	vfmadd231ps {rn-sae}, %zmmA, %zmm4, %zmm11
	vmulps	{rn-sae}, %zmm5, %zmmA, %zmm2
	vmovups	LOCAL_DATA(_Th_tbl_uisa_lo)(%rip), %zmm3
	vpermt2ps LOCAL_DATA(_Th_tbl_uisa_hi)(%rip), %zmm10, %zmm3
	vfmadd213ps {rn-sae}, %zmm5, %zmm2, %zmm11


	/* Computer Denominator:
	   sDenominator - sDlow ~= 1-(sTh+sTl) * (sP+sPlow).  */
	vmulps	{rn-sae}, %zmm11, %zmm3, %zmm7


	vmovups	COMMON_DATA(_OneF)(%rip), %zmm5

	/* Compute Numerator:
	   sNumerator + sNlow ~= sTh+sTl+sP+sPlow.  */
	vaddps	{rn-sae}, %zmm3, %zmm11, %zmm8
	vsubps	{rn-sae}, %zmm7, %zmm5, %zmm9

#if PRECISION >= 2
	/* High Precision Version.  */
	vrcp14ps %zmm9, %zmm14
	vsubps	{rn-sae}, %zmm3, %zmm8, %zmm2

	vsubps	{rn-sae}, %zmm5, %zmm9, %zmm6
	vmulps	{rn-sae}, %zmm8, %zmm14, %zmm15

	/* One NR iteration to refine sQuotient.  */
	vfmsub213ps {rn-sae}, %zmm8, %zmm15, %zmm9
	vaddps	{rn-sae}, %zmm7, %zmm6, %zmm12
	vfnmadd213ps {rn-sae}, %zmm9, %zmm15, %zmm12
	vsubps	{rn-sae}, %zmm2, %zmm11, %zmm13
	vsubps	{rn-sae}, %zmm13, %zmm12, %zmm0

	vfnmadd213ps {rn-sae}, %zmm15, %zmm14, %zmm0
#else
	/* Low Precision Version.  */
	vdivps	{rn-sae}, %zmm9, %zmm8, %zmm0
#endif
	ret

	.p2align 4
L(AUX_BRANCH):
	/* Hoping k0 doesn't have some long dependency chain attached to
	   it. NB: We really don't need all 1s, we only need the `k6`
	   mask. Currently `vpgatherdps` does not optimize out any
	   loads at zero-bits for the mask.  */
	kxnorw	%k0, %k0, %k2

	/* Multiply indexes by 12. Note we could rearrange the data and
	   then just shift down by 23 saving 2x instructions. This will
	   probably look slightly better on microbenchmarks but as it
	   is now we get some constructive cache interference between
	   the gathers. As well this minimizes the total lines brought
	   in. Its a judgement call but intuitively this will be better
	   for applications. If someone has the time/inclination
	   benchmarking this on some real applications may be worth it.  */
	vpsrld	$23, %zmm11, %zmm8
	vpaddd	%zmm8, %zmm8, %zmmA
	vpaddd	%zmmA, %zmm8, %zmm14

	/* Get the (2^a / 2pi) mod 1 values from the table.
	   Because
	   doesn't have I-type gather, we need a trivial cast.  */
	lea	AVX512_SHARED_DATA(_Reduction)(%rip), %rax

	/* Offset 4 gather has the most work based on it so we want it
	   to be finished first to keep the backend busy.  */

	/* NB: The dependency break is VERY important.  */
	vpxor	%ymm4, %ymm4, %ymm4
	vgatherdps 4(%rax, %zmm14, 4), %zmm4{%k2}


	/* If the magnitude of the input is <= 2^-20, then
	   just pass
	   through the input, since no reduction will be needed and
	   the main path will only work accurately if the reduced
	   argument is
	   about >= 2^-40 (which it is for all large pi
	   multiples).  */
	vmovups	LOCAL_DATA(_sRangeVal)(%rip), %zmm9
	/* `zmm11` already has sign bit cast off. We are checking if the
	   exp was 0xff so we can just use unsigned comparison.  */
	vpcmpd	$5, %zmm9, %zmm11, %k1

	/* Also get the significand as an integer
	   NB: adding in the
	   integer bit is wrong for denorms!
	   To make this work for
	   denorms we should do something slightly different.  */

	/* zmm9 = zmm9 & (~zmm11) | _FLT_1_1to16(%rip).  */
	vpternlogd $0xae, LOCAL_DATA_UNALIGNED(_FLT_1_1to16)(%rip){1to16}, %zmm11, %zmm9

	/* Break the P_xxx and m into 16-bit chunks ready for
	   the
	   long multiplication via 16x16->32 multiplications.  */
	movl	$0x55555555, %ecx
	kmovd	%ecx, %k2

	vpsrld	$16, %zmm9, %zmm8
	vmovdqu16 %zmm11, %zmm9{%k2}{z}

	vpsrld	$16, %zmm4, %zmm15
	vmovdqu16 %zmm4, %zmmA{%k2}{z}

	/* Now do the big multiplication and carry propagation.  */
	vpmulld	%zmm15, %zmm9, %zmm2
	vpmulld	%zmmA, %zmm9, %zmm12

	vpmulld	%zmm15, %zmm8, %zmm15
	vpmulld	%zmmA, %zmm8, %zmmA

	vpsrld	$16, %zmm2, %zmm4

	vpaddd	%zmm4, %zmm15, %zmm4
	vmovdqu16 %zmm2, %zmm15{%k2}{z}


	kxnorw	%k0, %k0, %k3
	vpxor	%ymm3, %ymm3, %ymm3
	vgatherdps (%rax, %zmm14, 4), %zmm3{%k3}
	vpsrld	$16, %zmm3, %zmm6
	vmovdqu16 %zmm3, %zmm3{%k2}{z}


	/* Do this comparison while `zmm11` still contains abs(input).  */
	vmovups	LOCAL_DATA(_FLT_1)(%rip), %zmm2
	vcmpps	$22, {sae}, %zmm2, %zmm11, %k5

	vpmulld	%zmm3, %zmm9, %zmm11
	vpmulld	%zmm3, %zmm8, %zmm3

	kxnorw	%k0, %k0, %k4
	vpxor	%ymm2, %ymm2, %ymm2
	vgatherdps 8(%rax, %zmm14, 4), %zmm2{%k4}
	vpsrld	$16, %zmm2, %zmm14
	vpmulld	%zmm14, %zmm9, %zmm13
	vpmulld	%zmm14, %zmm8, %zmm14

	vmovdqu16 %zmm2, %zmm2{%k2}{z}
	vpmulld	%zmm2, %zmm8, %zmm8
	/* We never take the upperhalf of zmm2.  */
	vpmullw	%zmm6, %zmm9, %zmm2{%k2}{z}

	vpsrld	$16, %zmm12, %zmm9
	vpsrld	$16, %zmm11, %zmm6
	vpsrld	$16, %zmm13, %zmm13

	vpaddd	%zmm9, %zmmA, %zmm9
	vpaddd	%zmm6, %zmm3, %zmm6
	vpaddd	%zmm13, %zmm14, %zmm14

	vpsrld	$16, %zmm8, %zmm8

	vmovdqu16 %zmm12, %zmm13{%k2}{z}
	vpaddd	%zmm14, %zmm13, %zmm3

	vpaddd	%zmm9, %zmm15, %zmm14
	vpaddd	%zmm3, %zmm8, %zmm9
	vpsrld	$16, %zmm9, %zmm12

	vpaddd	%zmm14, %zmm12, %zmm8

	/* Now round at the 2^-8 bit position for reduction mod pi/2^7
	   instead of the original 2pi (but still with the same 2pi
	   scaling).
	   Use a shifter of 2^15 + 2^14.
	   The N we get is
	   our final version; it has an offset of
	   2^8 because of the
	   implicit integer bit, and anyway for negative
	   starting
	   value it's a 2s complement thing. But we need to mask
	   off
	   the exponent part anyway so it's fine.  */

	/* We already truncated zmm2.  */
	vpaddd	%zmm6, %zmm2, %zmm13

	vpsrld	$16, %zmm8, %zmm15
	vmovdqu16 %zmm11, %zmm11{%k2}{z}
	vpaddd	%zmm4, %zmm11, %zmmA


	vpaddd	%zmmA, %zmm15, %zmm4
	vpsrld	$16, %zmm4, %zmm12
	vpaddd	%zmm13, %zmm12, %zmm11

	/* Assemble reduced argument from the pieces.  */
	vpslldq	$2, %zmm11, %zmmA
	vpslldq	$2, %zmm8, %zmm11
	vpblendmw %zmm4, %zmmA, %zmm3{%k2}
	vmovdqu16 %zmm9, %zmm11{%k2}
	vmovaps	COMMON_DATA(_OneF)(%rip), %zmm9
	vmovups	LOCAL_DATA(_FLT_2)(%rip), %zmm14
	vpsrld	$9, %zmm3, %zmm2


	/* We want to incorporate the original sign now too.
	   Do it
	   here for convenience in getting the right N value,
	   though
	   we could wait right to the end if we were prepared
	   to
	   modify the sign of N later too.
	   So get the appropriate
	   sign mask now (or sooner).  */
	vpandnd	%zmm0, %zmm7, %zmmA
	vpslld	$5, %zmm11, %zmm13

	/* Create floating-point high part, implicitly adding integer
	   bit 1
	   Incorporate overall sign at this stage too.  */
	vpternlogd $0xfe, %zmm9, %zmmA, %zmm2
	vaddps	{rn-sae}, %zmm2, %zmm14, %zmm12
	vsubps	{rn-sae}, %zmm14, %zmm12, %zmm15
	vsubps	{rn-sae}, %zmm15, %zmm2, %zmm2

	/* Create floating-point low and medium parts, respectively
	   lo_17, ... lo_0, 0, ..., 0
	   hi_8, ... hi_0, lo_31, ...,
	   lo_18
	   then subtract off the implicitly added integer bits,
	   2^-46 and 2^-23, respectively.
	   Put the original sign into
	   all of them at this stage.  */

	/* Save code size by microfusing vpord _FLT_2_1to16, %zmmA. This
	   increase the dependency chain on computing `zmm13` (we could
	   use vptern).  */
	vpord	LOCAL_DATA_UNALIGNED(_FLT_2_1to16)(%rip){1to16}, %zmmA, %zmm15
	/* Don't need to full addition result.  */
	vmovaps	LOCAL_DATA(_FLT_3)(%rip), %zmm6
	vpandd	%zmm6, %zmm4, %zmm3
	/* zmm13 = (zmm13 & ~_NotIOffExpoMask) | zmm15.  */
	vpternlogd $0xdc, COMMON_DATA(_NotiOffExpoMask)(%rip){1to16}, %zmm15, %zmm13

	vsubps	{rn-sae}, %zmm15, %zmm13, %zmm8
	vpsrld	$18, %zmm11, %zmm15

	vpxord	LOCAL_DATA_UNALIGNED(_FLT_3_1to16)(%rip){1to16}, %zmmA, %zmm14
	vpslld	$14, %zmm3, %zmmA

	vpternlogd $0xfe, %zmm15, %zmm14, %zmmA
	vsubps	{rn-sae}, %zmm14, %zmmA, %zmm11


	/* Now add them up into 2 reasonably aligned pieces.  */
	vaddps	{rn-sae}, %zmm11, %zmm2, %zmm13
	vsubps	{rn-sae}, %zmm13, %zmm2, %zmm2
	/* `zmm15` is generally zero. Possibly place for optimization
	   later on.  */
	vaddps	{rn-sae}, %zmm2, %zmm11, %zmm15

	/*

	   The output is _VRES_R (high) + _VRES_E (low), and the
	   integer part is _VRES_IND
	   Set sRp2 = _VRES_R^2 and then
	   resume the original code.  */
	vaddps	{rn-sae}, %zmm8, %zmm15, %zmm15
	vmovups	LOCAL_DATA(_FLT_4)(%rip), %zmm8

	/* Grab our final N value as an integer, appropriately masked
	   mod 2^8.  */
	vpandd	%zmm6, %zmm12, %zmm6

	/* Now multiply those numbers all by 2 pi, reasonably
	   accurately.
	   (RHi + RLo)
	   (pi_lead + pi_trail) ~=
	   RHi
	   pi_lead + (RHi
	   pi_trail + RLo
	   pi_lead).  */
	vmovups	LOCAL_DATA(_FLT_5)(%rip), %zmm12
	vmulps	{rn-sae}, %zmm12, %zmm13, %zmmA
	vblendmps %zmmA, %zmm0, %zmm14{%k5}
	vfmsub231ps {rn-sae}, %zmm12, %zmm13, %zmmA
	vfmadd213ps {rn-sae}, %zmmA, %zmm8, %zmm13
	vfmadd213ps {rn-sae}, %zmm13, %zmm15, %zmm12{%k5}{z}


	vpsrld	$31, %zmm14, %zmm15

	vpsubd	%zmm7, %zmm6, %zmm2
	vpaddd	%zmm7, %zmm15, %zmm3
	vpsubd	%zmm3, %zmm2, %zmm2

	vpsrld	$2, %zmm2, %zmm10{%k6}
	vpslld	$2, %zmm10, %zmm11

	/* End of large arguments path
	   Merge results from main and
	   large paths:.  */
	vpsubd	%zmm11, %zmm6, %zmm6
	vmovups	LOCAL_DATA(_FLT_6)(%rip), %zmm11
	vcvtdq2ps {rn-sae}, %zmm6, %zmmA
	vmovups	LOCAL_DATA(_FLT_7)(%rip), %zmm6
	vfmadd231ps {rn-sae}, %zmmA, %zmm6, %zmm12
	vaddps	{rn-sae}, %zmm14, %zmm12, %zmm5{%k6}
	vfmadd231ps {rn-sae}, %zmmA, %zmm11, %zmm5{%k6}


	/* Table lookup.  */
	vmovups	LOCAL_DATA(_Th_tbl_uisa_lo)(%rip), %zmm3
	vmovups	LOCAL_DATA(_sPC3_uisa)(%rip), %zmm4
	vmulps	{rn-sae}, %zmm5, %zmm5, %zmmA
	vpermt2ps LOCAL_DATA(_Th_tbl_uisa_hi)(%rip), %zmm10, %zmm3
	vmovups	LOCAL_DATA(_sPC5_uisa)(%rip), %zmm10
	vfmadd231ps {rn-sae}, %zmmA, %zmm10, %zmm4
	vmulps	{rn-sae}, %zmm5, %zmm4, %zmm15
	vfmadd213ps {rn-sae}, %zmm5, %zmmA, %zmm15

	/* Computer Denominator:
	   sDenominator - sDlow ~= 1-(sTh+sTl) * (sP+sPlow).  */
	vmulps	{rn-sae}, %zmm15, %zmm3, %zmm7

	/* Compute Numerator:
	   sNumerator + sNlow ~= sTh+sTl+sP+sPlow.  */
	vaddps	{rn-sae}, %zmm3, %zmm15, %zmm8
	vsubps	{rn-sae}, %zmm7, %zmm9, %zmm11

#if PRECISION >= 1
	/* High Precision Version.  */
	vrcp14ps %zmm11, %zmm14
	vsubps	{rn-sae}, %zmm3, %zmm8, %zmm2
	vsubps	{rn-sae}, %zmm9, %zmm11, %zmm6
	vsubps	{rn-sae}, %zmm2, %zmm15, %zmm13
	vmulps	{rn-sae}, %zmm8, %zmm14, %zmm4

	vaddps	{rn-sae}, %zmm7, %zmm6, %zmm12
	/* One NR iteration to refine sQuotient.  */
	vfmsub213ps {rn-sae}, %zmm8, %zmm4, %zmm11
	vfnmadd213ps {rn-sae}, %zmm11, %zmm4, %zmm12
	kmovw	%k1, %edx
	testl	%edx, %edx
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)

	vsubps	{rn-sae}, %zmm13, %zmm12, %zmm0
	vfnmadd213ps {rn-sae}, %zmm4, %zmm14, %zmm0
#else
	/* Low Precision Version.  */
	kmovw	%k1, %edx
	testl	%edx, %edx
	/* Go to special inputs processing branch.  */
	jne	L(SPECIAL_VALUES_BRANCH)
	vdivps	%zmm11, %zmm8, %zmm0
#endif
	/* Restore registers
	   and exit the function.  */
	ret


	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a tanf call. Optimize for code size
	   moreso than speed here.  */
L(SPECIAL_VALUES_BRANCH):

	/* Use r13 to save/restore the stack. This allows us to use rbp
	   as callee save register saving code size.  */
	pushq	%r13
	cfi_def_cfa (rsp, 16)
	/* Need to callee save registers to preserve state across tanf
	   calls.  */
	pushq	%rbx
	cfi_def_cfa (rsp, 24)
	pushq	%rbp
	cfi_def_cfa (rsp, 32)
	movq	%rsp, %r13
	cfi_def_cfa (r13, 32)
#if PRECISION >= 1
	vsubps	{rn-sae}, %zmm13, %zmm12, %zmm1
	vfnmadd213ps {rn-sae}, %zmm4, %zmm1, %zmm14
#else
	vdivps	%zmm11, %zmm8, %zmm14
#endif
	/* Align stack and make room for 2x zmm vectors.  */
	andq	$-64, %rsp
	addq	$-128, %rsp



	/* Save origional input.  */
	vmovaps	%zmm0, 64(%rsp)
	/* Save all already computed inputs.  */
	vmovaps	%zmm14, (%rsp)

	vzeroupper

	/* edx has 1s where there was a special value that needs to be
	   handled by a tanf call.  */
	movl	%edx, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to tanf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 56] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop. Realigning
	   also costs more code size.  */
	xorl	%ebp, %ebp
	tzcntl	%ebx, %ebp

	/* Scalar math fucntion call to process special input.  */
	movss	64(%rsp, %rbp, 4), %xmm0
	call	tanf@PLT
	INC_FALLBACK0
	/* No good way to avoid the store-forwarding fault this will
	   cause on return. `lfence` avoids the SF fault but at greater
	   cost as it serialized stack/callee save restoration.  */
	movss	%xmm0, (%rsp, %rbp, 4)

	blsrl	%ebx, %ebx
	jnz	L(SPECIAL_VALUES_LOOP)

	/* All results have been written to 64(%rsp).  */
	vmovaps	(%rsp), %zmm0
	/* Restore rsp.  */
	movq	%r13, %rsp
	cfi_def_cfa (rsp, 32)
	/* Restore callee save registers.  */
	popq	%rbp
	cfi_def_cfa (rsp, 24)
	popq	%rbx
	cfi_def_cfa (rsp, 16)
	popq	%r13
	ret
END(tanf_16_avx512_dev)

	.section .rodata.evex512, "a"

	/* Place the minimally aligned pieces at the begining so there
	   is a chance they fit in aligning bytes.  */
	.align	16
LOCAL_DATA_NAME_UNALIGNED:
	float_block (LOCAL_DATA_NAME_UNALIGNED, _FLT_1_1to16, 0x00800000)
	float_block (LOCAL_DATA_NAME_UNALIGNED, _FLT_2_1to16, 0x28800000)
	float_block (LOCAL_DATA_NAME_UNALIGNED, _FLT_3_1to16, 0x34000000)

	.type	LOCAL_DATA_NAME_UNALIGNED, @object
	.size	LOCAL_DATA_NAME_UNALIGNED, .-LOCAL_DATA_NAME_UNALIGNED


	.align	64
LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _sInvPI_uisa, 0x4122f983)
	DATA_VEC (LOCAL_DATA_NAME, _sRShifter, 0x4B400000)
	DATA_VEC (LOCAL_DATA_NAME, _sPI1_uisa, 0x3dc90fda)
	DATA_VEC (LOCAL_DATA_NAME, _sPI2_uisa, 0x31a22168)
	DATA_VEC (LOCAL_DATA_NAME, _sPI3_uisa, 0x25c234c5)
	DATA_VEC (LOCAL_DATA_NAME, _sRangeReductionVal_uisa, 0x46010000)
	DATA_VEC (LOCAL_DATA_NAME, _sPC5_uisa, 0x3e08b888)
	DATA_VEC (LOCAL_DATA_NAME, _sPC3_uisa, 0x3eaaaaa6)

	float_block (LOCAL_DATA_NAME, _Th_tbl_uisa_lo,
	0x80000000, 0x3dc9b5dc, 0x3e4bafaf, 0x3e9b5042,
	0x3ed413cd, 0x3f08d5b9, 0x3f2b0dc1, 0x3f521801,
	0x3f800000, 0x3f9bf7ec, 0x3fbf90c7, 0x3fef789e,
	0x401a827a, 0x4052facf, 0x40a0dff7, 0x41227363)

	float_block (LOCAL_DATA_NAME, _Th_tbl_uisa_hi,
	0xff7fffff, 0xc1227363, 0xc0a0dff7, 0xc052facf,
	0xc01a827a, 0xbfef789e, 0xbfbf90c7, 0xbf9bf7ec,
	0xbf800000, 0xbf521801, 0xbf2b0dc1, 0xbf08d5b9,
	0xbed413cd, 0xbe9b5042, 0xbe4bafaf, 0xbdc9b5dc)

	DATA_VEC (LOCAL_DATA_NAME, _sRangeVal, 0x7f800000)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_1, 0x35800000)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_2, 0x47400000)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_3, 0x000001ff)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_4, 0xb43bbd2e)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_5, 0x40c90fdb)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_6, 0x3cc90fdb)
	DATA_VEC (LOCAL_DATA_NAME, _FLT_7, 0xb03bbd2e)


	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
