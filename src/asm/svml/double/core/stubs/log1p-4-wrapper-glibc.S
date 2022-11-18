/* Stub .*/
// Ftype: 3
/* Function log1p vectorized with AVX2, wrapper version.
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
   <https://www.gnu.org/licenses/>.  */


#include "/home/noah/programs/projects/svml-new/src/asm/svml/double/core/stubs/common-wrapper-glibc.S"

	.section .text.avx2, "ax", @progbits
ENTRY (log1p_4_wrapper_glibc)
WRAPPER_IMPL_AVX_glibc log1p_2_wrapper_glibc
END (log1p_4_wrapper_glibc)

#ifndef USE_MULTIARCH
 libmvec_hidden_def (log1p_4_wrapper_glibc)
#endif
