/* Core .*/
/* Round argument to nearest integral value according to current rounding
   direction.
   Copyright (C) 2002-2022 Free Software Foundation, Inc.
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



	.text
ENTRY(llrintf_1_sse2_dev)
	cvtss2si %xmm0,%rax
	ret
END(llrintf_1_sse2_dev)
libm_alias_float (llrint_1_sse2_dev, llrint)
#ifndef __ILP32__
strong_alias (llrintf_1_sse2_dev, __lrintf)
libm_alias_float (llrint_1_sse2_dev, lrint)
#endif
