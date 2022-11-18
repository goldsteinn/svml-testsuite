/* Stub .*/
// Ftype: 3
/* Function hypot vectorized with AVX-512. Wrapper to AVX2 version.
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

	Section: .section .text.evex512, "ax", @progbits
ENTRY (hypot_8_wrapper_v_dev)
WRAPPER_IMPL_AVX512_ff_dev hypot_4_wrapper_v_dev
END (hypot_8_wrapper_v_dev)
