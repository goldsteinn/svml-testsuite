/* Stub .*/
// Ftype: 3
/* Function cbrtf vectorized with AVX2, wrapper version.
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


#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/core/stubs/common-wrapper-dev.S"

	.text
ENTRY (cbrtf_8_wrapper_dev)
WRAPPER_IMPL_AVX_dev cbrtf_4_wrapper_dev
END (cbrtf_8_wrapper_dev)

#ifndef USE_MULTIARCH
 libmvec_hidden_def (cbrtf_8_wrapper_dev)
#endif
