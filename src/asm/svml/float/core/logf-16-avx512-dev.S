/* Core .*/
/* Function logf vectorized with AVX-512. KNL and SKX versions.
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

#define _sPoly_1                      	0
#define _sPoly_2                      	64
#define _sPoly_3                      	128
#define _sPoly_4                      	192
#define _sPoly_5                      	256
#define _sPoly_6                      	320
#define _sPoly_7                      	384
#define _iHiDelta                     	448
#define _iLoRange                     	512
#define _iBrkValue                    	576
#define _iOffExpoMask                 	640
#define _sOne                         	704
#define _sLn2                         	768
#define _sInfs                        	832
#define _sOnes                        	896
#define _sZeros                       	960

.macro float_vector offset value
.if .-__svml_dev_slog_data_avx512 != \offset
.err
.endif
.rept 16
.long \value
.endr
.endm

#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/core/stubs/common-wrapper-dev.S"

	.section .text.evex512, "ax", @progbits

ENTRY(logf_16_avx512_dev)



	/* ALGORITHM DESCRIPTION: log(x) = exponent_x*log(2) +
	   log(mantissa_x),         if mantissa_x<4/3 log(x) =
	   (exponent_x+1)*log(2) + log(0.5*mantissa_x), if
	   mantissa_x>4/3 R = mantissa_x - 1,     if mantissa_x<4/3 R =
	   0.5*mantissa_x - 1, if mantissa_x>4/3 |R|< 1/3 log(1+R) is
	   approximated as a polynomial: degree 9 for 1-ulp, degree 7
	   for 4-ulp, degree 3 for half-precision.  */


	vmovups	_iBrkValue+ __svml_dev_slog_data_avx512(%rip), %zmm4
	vmovups	_sPoly_7+ __svml_dev_slog_data_avx512(%rip), %zmm8

	/* check for working range, set special argument mask
	   (denormals/zero/Inf/NaN).  */
	vmovups	_iHiDelta+ __svml_dev_slog_data_avx512(%rip), %zmm3
	vpaddd	%zmm3, %zmm0, %zmm1
	// vpaddd  _iOffExpoMask+ __svml_dev_slog_data_avx512(%rip), %zmm0, %zmm1

	/* reduction: compute r,n.  */
	vpsubd	%zmm4, %zmm0, %zmm2
	vmovups	_iLoRange+ __svml_dev_slog_data_avx512(%rip), %zmm7
	vpcmpd	$2, %zmm7, %zmm1, %k0

	/* exponent_x (mantissa_x<4/3) or exponent_x+1 (mantissa_x>4/3).
	 */
	vpsrad	$23, %zmm2, %zmm5
	vpternlogd $0x28, %zmm2, %zmm7, %zmm3
	// vpandd  _iOffExpoMask+ __svml_dev_slog_data_avx512(%rip), %zmm2, %zmm3

	/* mantissa_x (mantissa_x<4/3), or 0.5*mantissa_x
	   (mantissa_x>4/3).  */
	vpaddd	%zmm4, %zmm3, %zmm7

	/* reduced argument R.  */
	vsubps	_sOne+ __svml_dev_slog_data_avx512(%rip), %zmm7, %zmm9

	/* polynomial evaluation starts here.  */
	vfmadd213ps _sPoly_6+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vfmadd213ps _sPoly_5+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vfmadd213ps _sPoly_4+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vfmadd213ps _sPoly_3+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vfmadd213ps _sPoly_2+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vfmadd213ps _sPoly_1+ __svml_dev_slog_data_avx512(%rip), %zmm9, %zmm8
	vmulps	%zmm9, %zmm8, %zmm10

	/* polynomial evaluation end.  */
	vfmadd213ps %zmm9, %zmm9, %zmm10
	vcvtdq2ps {rn-sae}, %zmm5, %zmm1
	kmovw	%k0, %ecx
	/* final reconstruction: add exponent_value*log2 to polynomial
	   result.  */
	vfmadd132ps _sLn2+ __svml_dev_slog_data_avx512(%rip), %zmm10, %zmm1
	testl	%ecx, %ecx
	jne	L(SPECIAL_VALUES_BRANCH)

	vmovaps	%zmm1, %zmm0
	ret

	/* Branch to process * special inputs.  */
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

	/* Align stack and make room for 2x zmm vectors.  */
	andq	$-64, %rsp
	addq	$-128, %rsp

	/* Save origional input.  */
	vmovaps	%zmm0, 64(%rsp)
	/* Save all already computed inputs.  */
	vmovaps	%zmm1, (%rsp)

	vzeroupper

	/* edx has 1s where there was a special value that needs to be
	   handled by a tanf call.  */
	movl	%ecx, %ebx
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
	call	logf@PLT
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

END(logf_16_avx512_dev)



	.section .rodata, "a"
	.align	64

	/* Data table for vector implementations of function logf. The
	   table may contain polynomial, reduction, lookup coefficients
	   and other coefficients obtained through different methods of
	   research and experimental work.  */

	.globl	__svml_dev_slog_data_avx512
__svml_dev_slog_data_avx512:

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
	float_vector _iHiDelta 0x00800000

	/* Constant for work range check: 00800000 + Delta.  */
	float_vector _iLoRange 0x00ffffff

	/* Mantissa break point  SP 2/3.  */
	float_vector _iBrkValue 0x3f2aaaab

	/* SP significand mask.  */
	float_vector _iOffExpoMask 0x007fffff

	/* 1.0f.  */
	float_vector _sOne 0x3f800000

	/* SP log(2).  */
	float_vector _sLn2 0x3f317218

	/* SP infinity, +/-.  */
	.if	.-__svml_dev_slog_data_avx512 != _sInfs
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
	.if	.-__svml_dev_slog_data_avx512 != _sOnes
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
	.if	.-__svml_dev_slog_data_avx512 != _sZeros
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
	.type	__svml_dev_slog_data_avx512, @object
	.size	__svml_dev_slog_data_avx512, .-__svml_dev_slog_data_avx512
