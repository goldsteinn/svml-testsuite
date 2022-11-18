/* Stub .*/
// Ftype: 3
/* Function sinf vectorized with AVX-512. Wrapper to AVX2 version.
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


#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/core/stubs/common-wrapper-dev.S"

	.section .text.evex512, "ax", @progbits
ENTRY (sinf_16_wrapper_dev)
WRAPPER_IMPL_AVX512_dev sinf_8_wrapper_dev
END (sinf_16_wrapper_dev)
