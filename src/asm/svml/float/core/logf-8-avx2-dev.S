/* Core .*/
/* Function logf vectorized with AVX2.
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
#define LOCAL_DATA_NAME	__svml_dev_slog_data_avx2
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-avx2-rodata-offsets.h"

#define _sPoly_7	0
#define _sPoly_6	32
#define _sPoly_5	64
#define _sPoly_4	96
#define _sPoly_3	128
#define _sPoly_2	160



#define ymmA	ymm1

	.section .text.avx2, "ax", @progbits
ENTRY(logf_8_avx2_dev)

	INC_FALLBACK1

	/* ALGORITHM DESCRIPTION: log(x) = exponent_x*log(2) +
	   log(mantissa_x),         if mantissa_x<4/3 log(x) =
	   (exponent_x+1)*log(2) + log(0.5*mantissa_x), if
	   mantissa_x>4/3 R = mantissa_x - 1,     if mantissa_x<4/3 R =
	   0.5*mantissa_x - 1, if mantissa_x>4/3 |R|< 1/3 log(1+R) is
	   approximated as a polynomial: degree 9 for 1-ulp, degree 7
	   for 4-ulp, degree 3 for half-precision.  */
	vmovups	COMMON_DATA(_IBrkValue)(%rip), %ymm6


	vmovups	COMMON_DATA(_NotiOffExpoMask)(%rip), %ymm7

	/* reduction: compute r,n.  */
	vpsubd	%ymm6, %ymm0, %ymm4

	/* exponent_x (mantissa_x<4/3) or exponent_x+1 (mantissa_x>4/3).
	 */
	vpsrad	$23, %ymm4, %ymm3
	vpandn	%ymm4, %ymm7, %ymm5
	vmovups	LOCAL_DATA(_sPoly_7)(%rip), %ymm4
	vcvtdq2ps %ymm3, %ymm2

	/* mantissa_x (mantissa_x<4/3), or 0.5*mantissa_x
	   (mantissa_x>4/3).  */
	vpaddd	%ymm6, %ymm5, %ymm3

	/* reduced argument R.  */
	vsubps	COMMON_DATA(_OneF)(%rip), %ymm3, %ymm5

	/* polynomial evaluation starts here.  */
	vfmadd213ps LOCAL_DATA(_sPoly_6)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPoly_5)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPoly_4)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPoly_3)(%rip), %ymm5, %ymm4
	vfmadd213ps LOCAL_DATA(_sPoly_2)(%rip), %ymm5, %ymm4
	vfmadd213ps COMMON_DATA(_Neg5F)(%rip), %ymm5, %ymm4
	vmulps	%ymm5, %ymm4, %ymm6

	/* polynomial evaluation end.  */
	vfmadd213ps %ymm5, %ymm5, %ymm6

	vmovups	COMMON_DATA(_ILoRange)(%rip), %ymmA
	/* check for working range, set special argument mask
	   (denormals/zero/Inf/NaN).  */
	vpsubd	%ymm7, %ymm0, %ymm7


	vpcmpgtd %ymm7, %ymmA, %ymmA
	vmovmskps %ymmA, %ecx

	/* final reconstruction: add exponent_value*log2 to polynomial
	   result.  */
	vfmadd132ps COMMON_DATA(_Ln2)(%rip), %ymm6, %ymm2
	testl	%ecx, %ecx
	jne	L(SPECIAL_VALUES_BRANCH)
	vmovaps	%ymm2, %ymm0
	ret
	/* Branch to process * special inputs.  */

	/* Cold case. edx has 1s where there was a special value that
	   needs to be handled by a atanhf call. Optimize for code size
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

	/* Save all already computed inputs.  */
	vmovups	%ymm2, (%rsp)
	/* Save original input (ymm2 unchanged up to this point).  */
	vmovups	%ymm0, 32(%rsp)

	vzeroupper

	/* edx has 1s where there was a special value that needs to be
	   handled by a atanhf call.  */
	movl	%ecx, %ebx
L(SPECIAL_VALUES_LOOP):

	/* use rbp as index for special value that is saved across calls
	   to atanhf. We technically don't need a callee save register
	   here as offset to rsp is always [0, 28] so we can restore
	   rsp by realigning to 64. Essentially the tradeoff is 1 extra
	   save/restore vs 2 extra instructions in the loop. Realigning
	   also costs more code size.  */
	xorl	%ebp, %ebp
	tzcntl	%ebx, %ebp

	/* Scalar math fucntion call to process special input.  */
	vmovss	32(%rsp, %rbp, 4), %xmm0
	call	logf@PLT
	INC_FALLBACK0
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
END(logf_8_avx2_dev)

	.section .rodata.avx2, "a"
	.align	32

	/* Data table for vector implementations of function logf. The
	   table may contain polynomial, reduction, lookup coefficients
	   and other coefficients obtained through different methods of
	   research and experimental work.  */
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
