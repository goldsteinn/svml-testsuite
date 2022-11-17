/* Stub .*/
// Ftype: 3
/* Function atan2 vectorized with SSE2.
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


#include "/home/noah/programs/projects/svml-new/src/asm/svml/double/core/stubs/common-wrapper-dev.S"

	.text
ENTRY (atan2_2_wrapper_v_dev)
WRAPPER_IMPL_SCALAR_ff_dev atan2
END (atan2_2_wrapper_v_dev)

#ifndef USE_MULTIARCH
 libmvec_hidden_def (atan2_2_wrapper_v_dev)
#endif
