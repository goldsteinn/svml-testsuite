/* Stub .*/
// Ftype: 3
/* Function atan2f vectorized with AVX2, wrapper version.
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
ENTRY (atan2f_8_wrapper_v_dev)
WRAPPER_IMPL_AVX_ff_dev atan2f_4_wrapper_v_dev
END (atan2f_8_wrapper_v_dev)

#ifndef USE_MULTIARCH
 libmvec_hidden_def (atan2f_8_wrapper_v_dev)
#endif
