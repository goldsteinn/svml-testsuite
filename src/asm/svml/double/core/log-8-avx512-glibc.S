/* Core .*/
/* Function log vectorized with AVX-512. KNL and SKX versions.
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

#include "/home/noah/programs/projects/svml-new/src/asm/svml/double/core/stubs/glibc-log-data.h"
#include "/home/noah/programs/projects/svml-new/src/asm/svml/double/core/stubs/common-wrapper-glibc.S"

	.section .text.evex512, "ax", @progbits
ENTRY (log_8_avx512_glibc)
/*
   ALGORITHM DESCRIPTION:

     log(x) = -log(Rcp) + log(Rcp*x),
       where Rcp ~ 1/x (accuracy ~9 bits,
       obtained by rounding HW approximation to 1+9 mantissa bits)

     Reduced argument R=Rcp*x-1 is used to approximate log(1+R) as polynomial

     log(Rcp) = exponent_Rcp*log(2) + log(mantissa_Rcp)
       -log(mantissa_Rcp) is obtained from a lookup table,
       accessed by a 9-bit index
 */
        pushq     %rbp
        cfi_adjust_cfa_offset (8)
        cfi_rel_offset (%rbp, 0)
        movq      %rsp, %rbp
        cfi_def_cfa_register (%rbp)
        andq      $-64, %rsp
        subq      $1280, %rsp
        movq      GOTREF(glibc_dlog_data)(%rip), %rax
        vmovaps   %zmm0, %zmm3
        kxnorw    %k3, %k3, %k3
        vmovups _Two10(%rax), %zmm2
        vmovups _Threshold(%rax), %zmm14
        vmovups _One(%rax), %zmm11
        vcmppd    $21, _MinNorm(%rax), %zmm3, %k1
        vcmppd    $18, _MaxNorm(%rax), %zmm3, %k2

/* isolate exponent bits */
        vpsrlq    $20, %zmm3, %zmm4

/* preserve mantissa, set input exponent to 2^(-10) */
        vpternlogq $248, _ExpMask(%rax), %zmm3, %zmm2
        vpternlogd $0xff, %zmm1, %zmm1, %zmm1
        vpsrlq    $32, %zmm4, %zmm6

/* reciprocal approximation good to at least 11 bits */
        vrcp14pd  %zmm2, %zmm5

/* exponent*log(2.0) */
        vmovups _poly_coeff_1(%rax), %zmm4
        vpmovqd   %zmm6, %ymm7

/* round reciprocal to nearest integer, will have 1+9 mantissa bits */
        vrndscalepd $8, %zmm5, %zmm0

/* calculate index for table lookup */
        vpsrlq    $40, %zmm0, %zmm12

/* argument reduction started:  R = Mantissa*Rcp - 1 */
        vfmsub213pd %zmm11, %zmm0, %zmm2
        vpmovqd   %zmm12, %ymm13

/* polynomial computation */
        vfmadd213pd _poly_coeff_2(%rax), %zmm2, %zmm4
        vmovaps   %zmm1, %zmm8
        vmovaps   %zmm1, %zmm9
        vpxord    %zmm5, %zmm5, %zmm5
        vgatherdpd _LogRcp_lookup(%rax,%ymm13), %zmm5{%k3}
        vmovups _Bias1(%rax), %zmm13
        vpandnq   %zmm3, %zmm3, %zmm8{%k1}
        vcmppd    $21, %zmm0, %zmm14, %k1
        vpandnq   %zmm14, %zmm14, %zmm1{%k1}
        vmulpd    %zmm2, %zmm2, %zmm14
        vpternlogq $248, _Bias(%rax), %zmm1, %zmm13
        vmovups _poly_coeff_3(%rax), %zmm1
        vfmadd213pd _poly_coeff_4(%rax), %zmm2, %zmm1
        vfmadd213pd %zmm1, %zmm14, %zmm4

/*
   reconstruction:
   (exponent*log(2)) + (LogRcp + (R+poly))
 */
        vfmadd213pd %zmm2, %zmm14, %zmm4
        vaddpd    %zmm5, %zmm4, %zmm2
        vpandnq   %zmm3, %zmm3, %zmm9{%k2}

/* combine and get argument value range mask */
        vorpd     %zmm9, %zmm8, %zmm10
        vcmppd    $3, %zmm10, %zmm10, %k0
        kmovw     %k0, %ecx

/* convert biased exponent to DP format */
        vcvtdq2pd %ymm7, %zmm15

/*
   Table stores -log(0.5*mantissa) for larger mantissas,
   adjust exponent accordingly
 */
        vsubpd    %zmm13, %zmm15, %zmm0
        vfmadd132pd _L2(%rax), %zmm2, %zmm0
        testl     %ecx, %ecx
        jne       .LBL_2_3

.LBL_2_2:
        cfi_remember_state
        movq      %rbp, %rsp
        cfi_def_cfa_register (%rsp)
        popq      %rbp
        cfi_adjust_cfa_offset (-8)
        cfi_restore (%rbp)
        ret

.LBL_2_3:
        cfi_restore_state
        vmovups   %zmm3, 1152(%rsp)
        vmovups   %zmm0, 1216(%rsp)
        je        .LBL_2_2

        xorb      %dl, %dl
        xorl      %eax, %eax
        kmovw     %k4, 1048(%rsp)
        kmovw     %k5, 1040(%rsp)
        kmovw     %k6, 1032(%rsp)
        kmovw     %k7, 1024(%rsp)
        vmovups   %zmm16, 960(%rsp)
        vmovups   %zmm17, 896(%rsp)
        vmovups   %zmm18, 832(%rsp)
        vmovups   %zmm19, 768(%rsp)
        vmovups   %zmm20, 704(%rsp)
        vmovups   %zmm21, 640(%rsp)
        vmovups   %zmm22, 576(%rsp)
        vmovups   %zmm23, 512(%rsp)
        vmovups   %zmm24, 448(%rsp)
        vmovups   %zmm25, 384(%rsp)
        vmovups   %zmm26, 320(%rsp)
        vmovups   %zmm27, 256(%rsp)
        vmovups   %zmm28, 192(%rsp)
        vmovups   %zmm29, 128(%rsp)
        vmovups   %zmm30, 64(%rsp)
        vmovups   %zmm31, (%rsp)
        movq      %rsi, 1064(%rsp)
        movq      %rdi, 1056(%rsp)
        movq      %r12, 1096(%rsp)
        cfi_offset_rel_rsp (12, 1096)
        movb      %dl, %r12b
        movq      %r13, 1088(%rsp)
        cfi_offset_rel_rsp (13, 1088)
        movl      %ecx, %r13d
        movq      %r14, 1080(%rsp)
        cfi_offset_rel_rsp (14, 1080)
        movl      %eax, %r14d
        movq      %r15, 1072(%rsp)
        cfi_offset_rel_rsp (15, 1072)
        cfi_remember_state

.LBL_2_6:
        btl       %r14d, %r13d
        jc        .LBL_2_12

.LBL_2_7:
        lea       1(%r14), %esi
        btl       %esi, %r13d
        jc        .LBL_2_10

.LBL_2_8:
        incb      %r12b
        addl      $2, %r14d
        cmpb      $16, %r12b
        jb        .LBL_2_6

        kmovw     1048(%rsp), %k4
        kmovw     1040(%rsp), %k5
        kmovw     1032(%rsp), %k6
        kmovw     1024(%rsp), %k7
        vmovups   960(%rsp), %zmm16
        vmovups   896(%rsp), %zmm17
        vmovups   832(%rsp), %zmm18
        vmovups   768(%rsp), %zmm19
        vmovups   704(%rsp), %zmm20
        vmovups   640(%rsp), %zmm21
        vmovups   576(%rsp), %zmm22
        vmovups   512(%rsp), %zmm23
        vmovups   448(%rsp), %zmm24
        vmovups   384(%rsp), %zmm25
        vmovups   320(%rsp), %zmm26
        vmovups   256(%rsp), %zmm27
        vmovups   192(%rsp), %zmm28
        vmovups   128(%rsp), %zmm29
        vmovups   64(%rsp), %zmm30
        vmovups   (%rsp), %zmm31
        vmovups   1216(%rsp), %zmm0
        movq      1064(%rsp), %rsi
        movq      1056(%rsp), %rdi
        movq      1096(%rsp), %r12
        cfi_restore (%r12)
        movq      1088(%rsp), %r13
        cfi_restore (%r13)
        movq      1080(%rsp), %r14
        cfi_restore (%r14)
        movq      1072(%rsp), %r15
        cfi_restore (%r15)
        jmp       .LBL_2_2

.LBL_2_10:
        cfi_restore_state
        movzbl    %r12b, %r15d
        shlq      $4, %r15
        vmovsd    1160(%rsp,%r15), %xmm0
        vzeroupper
        vmovsd    1160(%rsp,%r15), %xmm0

        call      JUMPTARGET(log)

        vmovsd    %xmm0, 1224(%rsp,%r15)
        jmp       .LBL_2_8

.LBL_2_12:
        movzbl    %r12b, %r15d
        shlq      $4, %r15
        vmovsd    1152(%rsp,%r15), %xmm0
        vzeroupper
        vmovsd    1152(%rsp,%r15), %xmm0

        call      JUMPTARGET(log)

        vmovsd    %xmm0, 1216(%rsp,%r15)
        jmp       .LBL_2_7
END (log_8_avx512_glibc)
