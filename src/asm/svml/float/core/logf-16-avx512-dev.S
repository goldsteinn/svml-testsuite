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
#define LOCAL_DATA_NAME	__svml_slog_data_internal
#define LOCAL_DATA_NAME_UNALIGNED	__svml_slog_data_internal_unaligned
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-evex512-rodata-offsets.h"


	/* Offsets for data table __svml_slog_data_internal_unaligned.
	   4 bytes each.  */
#define _iLoRange	0

	/* Offsets for data table __svml_slog_data_internal
	   64 bytes each.  */
#define _iBrkValue	0
#define _sPoly_7	64
#define _sPoly_6	128
#define _sPoly_5	192
#define _sPoly_4	256
#define _sPoly_3	320
#define _sPoly_2	384
#define _sLn2	448

#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/core/stubs/common-wrapper-dev.S"

	.section .text.evex512, "ax", @progbits
ENTRY(logf_16_avx512_dev)
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
	vmovups	LOCAL_DATA(_iBrkValue)(%rip), %zmm4
	vmovups	LOCAL_DATA(_sPoly_7)(%rip), %zmm8

	/* check for working range, set special argument mask
	   (denormals/zero/Inf/NaN).  */
	vmovups	COMMON_DATA(_NotiOffExpoMask)(%rip), %zmm3
	vpsubd	%zmm3, %zmm0, %zmm1

	/* reduction: compute r,n.  */
	vpsubd	%zmm4, %zmm0, %zmm2
	vpcmpd	$2, LOCAL_DATA_UNALIGNED(_iLoRange)(%rip){1to16}, %zmm1, %k0

	/* exponent_x (mantissa_x<4/3),
	   or exponent_x+1 (mantissa_x>4/3).  */
	vpsrad	$23, %zmm2, %zmm5
	vpandnd	%zmm2, %zmm3, %zmm3

	/* mantissa_x (mantissa_x<4/3),
	   or 0.5 mantissa_x (mantissa_x>4/3).  */
	vpaddd	%zmm4, %zmm3, %zmm7

	/* reduced argument R.  */
	vsubps	COMMON_DATA(_OneF)(%rip), %zmm7, %zmm9

	/* polynomial evaluation starts here.  */
	vfmadd213ps LOCAL_DATA(_sPoly_6)(%rip), %zmm9, %zmm8
	vfmadd213ps LOCAL_DATA(_sPoly_5)(%rip), %zmm9, %zmm8
	vfmadd213ps LOCAL_DATA(_sPoly_4)(%rip), %zmm9, %zmm8
	vfmadd213ps LOCAL_DATA(_sPoly_3)(%rip), %zmm9, %zmm8
	vfmadd213ps LOCAL_DATA(_sPoly_2)(%rip), %zmm9, %zmm8
	vfmadd213ps COMMON_DATA(_Neg5F)(%rip), %zmm9, %zmm8
	vmulps	%zmm9, %zmm8, %zmm10

	/* polynomial evaluation end.  */
	vfmadd213ps %zmm9, %zmm9, %zmm10
	vcvtdq2ps {rn-sae}, %zmm5, %zmm1
	kmovw	%k0, %ecx
	/* final reconstruction: add exponent_value
	   log2 to polynomial result.  */
	vfmadd132ps LOCAL_DATA(_sLn2)(%rip), %zmm10, %zmm1
	/* Branch to process special inputs.  */
	testl	%ecx, %ecx
	jne	L(SPECIAL_VALUES_BRANCH)

	vmovaps	%zmm1, %zmm0
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



	.section .rodata.evex512, "a"
	.align	64

	/* Data table for vector implementations of function logf. The
	   table may contain polynomial, reduction, lookup coefficients
	   and other coefficients obtained through different methods of
	   research and experimental work.  */
LOCAL_DATA_NAME_UNALIGNED:
	float_block (LOCAL_DATA_NAME_UNALIGNED, _iLoRange, 0x00ffffff)

	.type	LOCAL_DATA_NAME_UNALIGNED, @object
	.size	LOCAL_DATA_NAME_UNALIGNED, .-LOCAL_DATA_NAME_UNALIGNED

LOCAL_DATA_NAME:
	DATA_VEC (LOCAL_DATA_NAME, _iBrkValue, 0x3f2aaaab)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_7, 0xbe1b6a22)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_6, 0x3e2db86b)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_5, 0xbe289358)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_4, 0x3e4afb81)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_3, 0xbe80061d)
	DATA_VEC (LOCAL_DATA_NAME, _sPoly_2, 0x3eaaaee7)
	DATA_VEC (LOCAL_DATA_NAME, _sLn2, 0x3f317218)

	.type	LOCAL_DATA_NAME, @object
	.size	LOCAL_DATA_NAME, .-LOCAL_DATA_NAME
