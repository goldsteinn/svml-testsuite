/* Stub .*/
	// Ftype: 2
/* Datatables for  tanhf AVX2 and tanhf SSE4.
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
   https://www.gnu.org/licenses/.  */

/* Offsets are ordered by use in the function. On cold-starts this
   might help the prefetcher. If the streaming prefetchers kick in it
   will prefetch into the lookup table.  */
#if 0
#include "/home/noah/programs/projects/svml-new/src/asm/svml/float/dev-common-data-macros.h.S"
	
	
#define _sInvPi	0
#define _sRShifter	32
#define _sQ2	128
#define _sP1	160
#define _sQ1	192
#define _sP0	224
#define _sRangeReductionVal	256

#define FLT_2	352
#define FLT_3	384
#define FLT_4	416
#define FLT_5	448
#define FLT_6	480
#define FLT_7	512
#define FLT_8	544
#define FLT_12	672
#define _Coeffs	3776
	
#define _Reduction	0
	.section .rodata.avx2, "a"
	.align	32
	.globl	dev_tanf_data_common_avx2
dev_tanf_data_common_avx2:
	DATA_VEC (LOCAL_DATA_NAME, _sInvPi, 0x3F22F983)
	DATA_VEC (LOCAL_DATA_NAME, _sRShifter, 0x4B400000)
	DATA_VEC (LOCAL_DATA_NAME, _sQ2, 0x3C1F336B)
	DATA_VEC (LOCAL_DATA_NAME, _sP1, 0xBDC433B4)
	DATA_VEC (LOCAL_DATA_NAME, _sQ1, 0xBEDBB7AB)
	DATA_VEC (LOCAL_DATA_NAME, _sP0, 0x3F7FFFFC)
	DATA_VEC (LOCAL_DATA_NAME, _sRangeReductionVal, 0x46010000)
	DATA_VEC (LOCAL_DATA_NAME, FLT_2, 0x0000ffff)
	DATA_VEC (LOCAL_DATA_NAME, FLT_3, 0x47400000)
	DATA_VEC (LOCAL_DATA_NAME, FLT_4, 0x28800000)
	DATA_VEC (LOCAL_DATA_NAME, FLT_5, 0x0003ffff)
	DATA_VEC (LOCAL_DATA_NAME, FLT_6, 0x000001ff)
	DATA_VEC (LOCAL_DATA_NAME, FLT_7, 0x34000000)
	DATA_VEC (LOCAL_DATA_NAME, FLT_8, 0x40c90fdb)
	DATA_VEC (LOCAL_DATA_NAME, FLT_11, 0x0000007f)
	/* _Coeffs Breakpoint B = 0 * pi/128, function tan(B + x).  */
	float_block (dev_tanf_data_common_avx2, _Coeffs,
		0x3FC90FDB,	/* B' = pi/2 - B (high single). */
		0xB33BBD2E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x00000000,	/* c0 (high single). */
		0x00000000,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x00000000,	/* c1 (low single). */
		0x00000000,	/* c2. */
		0x3EAAACDD,	/* c3. */
		0x00000000,	/* c4. */
		0x3FC5EB9B,	/* B' = pi/2 - B (high single). */
		0x32DE638C,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3CC91A31,	/* c0 (high single). */
		0x2F8E8D1A,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3A1DFA00,	/* c1 (low single). */
		0x3CC9392D,	/* c2. */
		0x3EAB1889,	/* c3. */
		0x3C885D3B,	/* c4. */
		0x3FC2C75C,	/* B' = pi/2 - B (high single). */
		0xB2CBBE8A,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3D49393C,	/* c0 (high single). */
		0x30A39F5B,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3B1E2B00,	/* c1 (low single). */
		0x3D49B5D4,	/* c2. */
		0x3EAC4F10,	/* c3. */
		0x3CFD9425,	/* c4. */
		0x3FBFA31C,	/* B' = pi/2 - B (high single). */
		0x33450FB0,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3D9711CE,	/* c0 (high single). */
		0x314FEB28,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3BB24C00,	/* c1 (low single). */
		0x3D97E43A,	/* c2. */
		0x3EAE6A89,	/* c3. */
		0x3D4D07E0,	/* c4. */
		0x3FBC7EDD,	/* B' = pi/2 - B (high single). */
		0xB1800ADD,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3DC9B5DC,	/* c0 (high single). */
		0x3145AD86,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3C1EEF20,	/* c1 (low single). */
		0x3DCBAAEA,	/* c2. */
		0x3EB14E5E,	/* c3. */
		0x3D858BB2,	/* c4. */
		0x3FB95A9E,	/* B' = pi/2 - B (high single). */
		0xB3651267,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3DFC98C2,	/* c0 (high single). */
		0xB0AE525C,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3C793D20,	/* c1 (low single). */
		0x3E003845,	/* c2. */
		0x3EB5271F,	/* c3. */
		0x3DAC669E,	/* c4. */
		0x3FB6365E,	/* B' = pi/2 - B (high single). */
		0x328BB91C,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E17E564,	/* c0 (high single). */
		0xB1C5A2E4,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3CB440D0,	/* c1 (low single). */
		0x3E1B3D00,	/* c2. */
		0x3EB9F664,	/* c3. */
		0x3DD647C0,	/* c4. */
		0x3FB3121F,	/* B' = pi/2 - B (high single). */
		0xB30F347D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E31AE4D,	/* c0 (high single). */
		0xB1F32251,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3CF6A500,	/* c1 (low single). */
		0x3E3707DA,	/* c2. */
		0x3EBFA489,	/* c3. */
		0x3DFBD9C7,	/* c4. */
		0x3FAFEDDF,	/* B' = pi/2 - B (high single). */
		0x331BBA77,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E4BAFAF,	/* c0 (high single). */
		0x2F2A29E0,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D221018,	/* c1 (low single). */
		0x3E53BED0,	/* c2. */
		0x3EC67E26,	/* c3. */
		0x3E1568E2,	/* c4. */
		0x3FACC9A0,	/* B' = pi/2 - B (high single). */
		0xB2655A50,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E65F267,	/* c0 (high single). */
		0x31B4B1DF,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D4E8B90,	/* c1 (low single). */
		0x3E718ACA,	/* c2. */
		0x3ECE7164,	/* c3. */
		0x3E2DC161,	/* c4. */
		0x3FA9A560,	/* B' = pi/2 - B (high single). */
		0x33719861,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E803FD4,	/* c0 (high single). */
		0xB2279E66,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D807FC8,	/* c1 (low single). */
		0x3E884BD4,	/* c2. */
		0x3ED7812D,	/* c3. */
		0x3E4636EB,	/* c4. */
		0x3FA68121,	/* B' = pi/2 - B (high single). */
		0x31E43AAC,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E8DB082,	/* c0 (high single). */
		0xB132A234,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D9CD7D0,	/* c1 (low single). */
		0x3E988A60,	/* c2. */
		0x3EE203E3,	/* c3. */
		0x3E63582C,	/* c4. */
		0x3FA35CE2,	/* B' = pi/2 - B (high single). */
		0xB33889B6,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3E9B5042,	/* c0 (high single). */
		0xB22A3AEE,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3DBC7490,	/* c1 (low single). */
		0x3EA99AF5,	/* c2. */
		0x3EEDE107,	/* c3. */
		0x3E80E9AA,	/* c4. */
		0x3FA038A2,	/* B' = pi/2 - B (high single). */
		0x32E4CA7E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3EA92457,	/* c0 (high single). */
		0x30B80830,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3DDF8200,	/* c1 (low single). */
		0x3EBB99E9,	/* c2. */
		0x3EFB4AA8,	/* c3. */
		0x3E9182BE,	/* c4. */
		0x3F9D1463,	/* B' = pi/2 - B (high single). */
		0xB2C55799,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3EB73250,	/* c0 (high single). */
		0xB2028823,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E0318F8,	/* c1 (low single). */
		0x3ECEA678,	/* c2. */
		0x3F053C67,	/* c3. */
		0x3EA41E53,	/* c4. */
		0x3F99F023,	/* B' = pi/2 - B (high single). */
		0x33484328,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3EC5800D,	/* c0 (high single). */
		0xB214C3C1,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E185E54,	/* c1 (low single). */
		0x3EE2E342,	/* c2. */
		0x3F0DCA73,	/* c3. */
		0x3EB8CC21,	/* c4. */
		0x3F96CBE4,	/* B' = pi/2 - B (high single). */
		0xB14CDE2E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3ED413CD,	/* c0 (high single). */
		0xB1C06152,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E2FB0CC,	/* c1 (low single). */
		0x3EF876CB,	/* c2. */
		0x3F177807,	/* c3. */
		0x3ED08437,	/* c4. */
		0x3F93A7A5,	/* B' = pi/2 - B (high single). */
		0xB361DEEE,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3EE2F439,	/* c0 (high single). */
		0xB1F4399E,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E49341C,	/* c1 (low single). */
		0x3F07C61A,	/* c2. */
		0x3F22560F,	/* c3. */
		0x3EEAA81E,	/* c4. */
		0x3F908365,	/* B' = pi/2 - B (high single). */
		0x3292200D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3EF22870,	/* c0 (high single). */
		0x325271F4,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E65107A,	/* c1 (low single). */
		0x3F1429F0,	/* c2. */
		0x3F2E8AFC,	/* c3. */
		0x3F040498,	/* c4. */
		0x3F8D5F26,	/* B' = pi/2 - B (high single). */
		0xB30C0105,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F00DC0D,	/* c0 (high single). */
		0xB214AF72,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E81B994,	/* c1 (low single). */
		0x3F218233,	/* c2. */
		0x3F3C4531,	/* c3. */
		0x3F149688,	/* c4. */
		0x3F8A3AE6,	/* B' = pi/2 - B (high single). */
		0x331EEDF0,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F08D5B9,	/* c0 (high single). */
		0xB25EF98E,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E92478D,	/* c1 (low single). */
		0x3F2FEDC9,	/* c2. */
		0x3F4BCD58,	/* c3. */
		0x3F27AE9E,	/* c4. */
		0x3F8716A7,	/* B' = pi/2 - B (high single). */
		0xB2588C6D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F1105AF,	/* c0 (high single). */
		0x32F045B0,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EA44EE2,	/* c1 (low single). */
		0x3F3F8FDB,	/* c2. */
		0x3F5D3FD0,	/* c3. */
		0x3F3D0A23,	/* c4. */
		0x3F83F267,	/* B' = pi/2 - B (high single). */
		0x3374CBD9,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F1970C4,	/* c0 (high single). */
		0x32904848,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EB7EFF8,	/* c1 (low single). */
		0x3F50907C,	/* c2. */
		0x3F710FEA,	/* c3. */
		0x3F561FED,	/* c4. */
		0x3F80CE28,	/* B' = pi/2 - B (high single). */
		0x31FDD672,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F221C37,	/* c0 (high single). */
		0xB20C61DC,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3ECD4F71,	/* c1 (low single). */
		0x3F631DAA,	/* c2. */
		0x3F83B471,	/* c3. */
		0x3F7281EA,	/* c4. */
		0x3F7B53D1,	/* B' = pi/2 - B (high single). */
		0x32955386,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F2B0DC1,	/* c0 (high single). */
		0x32AB7EBA,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EE496C2,	/* c1 (low single). */
		0x3F776C40,	/* c2. */
		0x3F9065C1,	/* c3. */
		0x3F89AFB6,	/* c4. */
		0x3F750B52,	/* B' = pi/2 - B (high single). */
		0x32EB316F,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F344BA9,	/* c0 (high single). */
		0xB2B8B0EA,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EFDF4F7,	/* c1 (low single). */
		0x3F86DCA8,	/* c2. */
		0x3F9ED53B,	/* c3. */
		0x3F9CBEDE,	/* c4. */
		0x3F6EC2D4,	/* B' = pi/2 - B (high single). */
		0xB2BEF0A7,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F3DDCCF,	/* c0 (high single). */
		0x32D29606,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEE6606F,	/* c1 (low single). */
		0x3F9325D6,	/* c2. */
		0x3FAF4E69,	/* c3. */
		0x3FB3080C,	/* c4. */
		0x3F687A55,	/* B' = pi/2 - B (high single). */
		0xB252257B,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F47C8CC,	/* c0 (high single). */
		0xB200F51A,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEC82C6C,	/* c1 (low single). */
		0x3FA0BAE9,	/* c2. */
		0x3FC2252F,	/* c3. */
		0x3FCD24C7,	/* c4. */
		0x3F6231D6,	/* B' = pi/2 - B (high single). */
		0xB119A6A2,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F521801,	/* c0 (high single). */
		0x32AE4178,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEA72938,	/* c1 (low single). */
		0x3FAFCC22,	/* c2. */
		0x3FD7BD4A,	/* c3. */
		0x3FEBB01B,	/* c4. */
		0x3F5BE957,	/* B' = pi/2 - B (high single). */
		0x3205522A,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F5CD3BE,	/* c0 (high single). */
		0x31460308,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBE8306C5,	/* c1 (low single). */
		0x3FC09232,	/* c2. */
		0x3FF09632,	/* c3. */
		0x4007DB00,	/* c4. */
		0x3F55A0D8,	/* B' = pi/2 - B (high single). */
		0x329886FF,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F68065E,	/* c0 (high single). */
		0x32670D1A,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBE36D1D6,	/* c1 (low single). */
		0x3FD35007,	/* c2. */
		0x4006A861,	/* c3. */
		0x401D4BDA,	/* c4. */
		0x3F4F5859,	/* B' = pi/2 - B (high single). */
		0x32EE64E8,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0x3F73BB75,	/* c0 (high single). */
		0x32FC908D,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBDBF94B0,	/* c1 (low single). */
		0x3FE8550F,	/* c2. */
		0x40174F67,	/* c3. */
		0x4036C608,	/* c4. */
		0x3F490FDB,	/* B' = pi/2 - B (high single). */
		0xB2BBBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE8BE60E,	/* c0 (high single). */
		0x320D8D84,	/* c0 (low single). */
		0x3F000000,	/* c1 (high 1 bit). */
		0xBDF817B1,	/* c1 (low single). */
		0xBD8345EB,	/* c2. */
		0x3D1DFDAC,	/* c3. */
		0xBC52CF6F,	/* c4. */
		0x3F42C75C,	/* B' = pi/2 - B (high single). */
		0xB24BBE8A,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE87283F,	/* c0 (high single). */
		0xB268B966,	/* c0 (low single). */
		0x3F000000,	/* c1 (high 1 bit). */
		0xBDFE6529,	/* c1 (low single). */
		0xBD7B1953,	/* c2. */
		0x3D18E109,	/* c3. */
		0xBC4570B0,	/* c4. */
		0x3F3C7EDD,	/* B' = pi/2 - B (high single). */
		0xB1000ADD,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE827420,	/* c0 (high single). */
		0x320B8B4D,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DFB9428,	/* c1 (low single). */
		0xBD7002B4,	/* c2. */
		0x3D142A6C,	/* c3. */
		0xBC3A47FF,	/* c4. */
		0x3F36365E,	/* B' = pi/2 - B (high single). */
		0x320BB91C,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE7B9282,	/* c0 (high single). */
		0xB13383D2,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DF5D211,	/* c1 (low single). */
		0xBD6542B3,	/* c2. */
		0x3D0FE5E5,	/* c3. */
		0xBC31FB14,	/* c4. */
		0x3F2FEDDF,	/* B' = pi/2 - B (high single). */
		0x329BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE724E73,	/* c0 (high single). */
		0x3120C3E2,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DF05283,	/* c1 (low single). */
		0xBD5AD45E,	/* c2. */
		0x3D0BAFBF,	/* c3. */
		0xBC27B8BB,	/* c4. */
		0x3F29A560,	/* B' = pi/2 - B (high single). */
		0x32F19861,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE691B44,	/* c0 (high single). */
		0x31F18936,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DEB138B,	/* c1 (low single). */
		0xBD50B2F7,	/* c2. */
		0x3D07BE3A,	/* c3. */
		0xBC1E46A7,	/* c4. */
		0x3F235CE2,	/* B' = pi/2 - B (high single). */
		0xB2B889B6,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE5FF82C,	/* c0 (high single). */
		0xB170723A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DE61354,	/* c1 (low single). */
		0xBD46DA06,	/* c2. */
		0x3D0401F8,	/* c3. */
		0xBC14E013,	/* c4. */
		0x3F1D1463,	/* B' = pi/2 - B (high single). */
		0xB2455799,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE56E46B,	/* c0 (high single). */
		0x31E3F001,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DE15025,	/* c1 (low single). */
		0xBD3D4550,	/* c2. */
		0x3D00462D,	/* c3. */
		0xBC092C98,	/* c4. */
		0x3F16CBE4,	/* B' = pi/2 - B (high single). */
		0xB0CCDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE4DDF41,	/* c0 (high single). */
		0xB1AEA094,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DDCC85C,	/* c1 (low single). */
		0xBD33F0BE,	/* c2. */
		0x3CFA23B0,	/* c3. */
		0xBC01FCF7,	/* c4. */
		0x3F108365,	/* B' = pi/2 - B (high single). */
		0x3212200D,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE44E7F8,	/* c0 (high single). */
		0xB1CAA3CB,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD87A74,	/* c1 (low single). */
		0xBD2AD885,	/* c2. */
		0x3CF3C785,	/* c3. */
		0xBBF1E348,	/* c4. */
		0x3F0A3AE6,	/* B' = pi/2 - B (high single). */
		0x329EEDF0,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE3BFDDC,	/* c0 (high single). */
		0xB132521A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD464FC,	/* c1 (low single). */
		0xBD21F8F1,	/* c2. */
		0x3CEE3076,	/* c3. */
		0xBBE6D263,	/* c4. */
		0x3F03F267,	/* B' = pi/2 - B (high single). */
		0x32F4CBD9,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE33203E,	/* c0 (high single). */
		0x31FEF5BE,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD0869C,	/* c1 (low single). */
		0xBD194E8C,	/* c2. */
		0x3CE8DCA9,	/* c3. */
		0xBBDADA55,	/* c4. */
		0x3EFB53D1,	/* B' = pi/2 - B (high single). */
		0x32155386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE2A4E71,	/* c0 (high single). */
		0xB19CFCEC,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DCCDE11,	/* c1 (low single). */
		0xBD10D605,	/* c2. */
		0x3CE382A7,	/* c3. */
		0xBBC8BD97,	/* c4. */
		0x3EEEC2D4,	/* B' = pi/2 - B (high single). */
		0xB23EF0A7,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE2187D0,	/* c0 (high single). */
		0xB1B7C7F7,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC96A2B,	/* c1 (low single). */
		0xBD088C22,	/* c2. */
		0x3CDE950E,	/* c3. */
		0xBBB89AD1,	/* c4. */
		0x3EE231D6,	/* B' = pi/2 - B (high single). */
		0xB099A6A2,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE18CBB7,	/* c0 (high single). */
		0xAFE28430,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC629CE,	/* c1 (low single). */
		0xBD006DCD,	/* c2. */
		0x3CDA5A2C,	/* c3. */
		0xBBB0B3D2,	/* c4. */
		0x3ED5A0D8,	/* B' = pi/2 - B (high single). */
		0x321886FF,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE101985,	/* c0 (high single). */
		0xB02FB2B8,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC31BF3,	/* c1 (low single). */
		0xBCF0F04D,	/* c2. */
		0x3CD60BC7,	/* c3. */
		0xBBA138BA,	/* c4. */
		0x3EC90FDB,	/* B' = pi/2 - B (high single). */
		0xB23BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBE07709D,	/* c0 (high single). */
		0xB18A2A83,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC03FA2,	/* c1 (low single). */
		0xBCE15096,	/* c2. */
		0x3CD26472,	/* c3. */
		0xBB9A1270,	/* c4. */
		0x3EBC7EDD,	/* B' = pi/2 - B (high single). */
		0xB0800ADD,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDFDA0CB,	/* c0 (high single). */
		0x2F14FCA0,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DBD93F7,	/* c1 (low single). */
		0xBCD1F71B,	/* c2. */
		0x3CCEDD2B,	/* c3. */
		0xBB905946,	/* c4. */
		0x3EAFEDDF,	/* B' = pi/2 - B (high single). */
		0x321BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDEC708C,	/* c0 (high single). */
		0xB14895C4,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DBB181E,	/* c1 (low single). */
		0xBCC2DEA6,	/* c2. */
		0x3CCB5027,	/* c3. */
		0xBB7F3969,	/* c4. */
		0x3EA35CE2,	/* B' = pi/2 - B (high single). */
		0xB23889B6,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDDB4F55,	/* c0 (high single). */
		0x30F6437E,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB8CB52,	/* c1 (low single). */
		0xBCB40210,	/* c2. */
		0x3CC82D45,	/* c3. */
		0xBB643075,	/* c4. */
		0x3E96CBE4,	/* B' = pi/2 - B (high single). */
		0xB04CDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDCA3BFF,	/* c0 (high single). */
		0x311C95EA,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB6ACDE,	/* c1 (low single). */
		0xBCA55C5B,	/* c2. */
		0x3CC5BC04,	/* c3. */
		0xBB63A969,	/* c4. */
		0x3E8A3AE6,	/* B' = pi/2 - B (high single). */
		0x321EEDF0,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDB93569,	/* c0 (high single). */
		0xAFB9ED00,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB4BC1F,	/* c1 (low single). */
		0xBC96E905,	/* c2. */
		0x3CC2E6F5,	/* c3. */
		0xBB3E10A6,	/* c4. */
		0x3E7B53D1,	/* B' = pi/2 - B (high single). */
		0x31955386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBDA83A77,	/* c0 (high single). */
		0x316D967A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB2F87C,	/* c1 (low single). */
		0xBC88A31F,	/* c2. */
		0x3CC0E763,	/* c3. */
		0xBB3F1666,	/* c4. */
		0x3E6231D6,	/* B' = pi/2 - B (high single). */
		0xB019A6A2,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD974A0D,	/* c0 (high single). */
		0xB14F365B,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB1616F,	/* c1 (low single). */
		0xBC750CD8,	/* c2. */
		0x3CBEB595,	/* c3. */
		0xBB22B883,	/* c4. */
		0x3E490FDB,	/* B' = pi/2 - B (high single). */
		0xB1BBBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD866317,	/* c0 (high single). */
		0xAFF02140,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAFF67D,	/* c1 (low single). */
		0xBC591CD0,	/* c2. */
		0x3CBCBEAD,	/* c3. */
		0xBB04BBEC,	/* c4. */
		0x3E2FEDDF,	/* B' = pi/2 - B (high single). */
		0x319BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD6B08FF,	/* c0 (high single). */
		0xB0EED236,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAEB739,	/* c1 (low single). */
		0xBC3D6D51,	/* c2. */
		0x3CBB485D,	/* c3. */
		0xBAFFF5BA,	/* c4. */
		0x3E16CBE4,	/* B' = pi/2 - B (high single). */
		0xAFCCDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD495A6C,	/* c0 (high single). */
		0xB0A427BD,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DADA345,	/* c1 (low single). */
		0xBC21F648,	/* c2. */
		0x3CB9D1B4,	/* c3. */
		0xBACB5567,	/* c4. */
		0x3DFB53D1,	/* B' = pi/2 - B (high single). */
		0x31155386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD27B856,	/* c0 (high single). */
		0xB0F7EE91,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DACBA4E,	/* c1 (low single). */
		0xBC06AEE3,	/* c2. */
		0x3CB8E5DC,	/* c3. */
		0xBAEC00EE,	/* c4. */
		0x3DC90FDB,	/* B' = pi/2 - B (high single). */
		0xB13BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBD0620A3,	/* c0 (high single). */
		0xB0ECAB40,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DABFC11,	/* c1 (low single). */
		0xBBD7200F,	/* c2. */
		0x3CB79475,	/* c3. */
		0xBA2B0ADC,	/* c4. */
		0x3D96CBE4,	/* B' = pi/2 - B (high single). */
		0xAF4CDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBCC92278,	/* c0 (high single). */
		0x302F2E68,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAB6854,	/* c1 (low single). */
		0xBBA1214F,	/* c2. */
		0x3CB6C1E9,	/* c3. */
		0x3843C2F3,	/* c4. */
		0x3D490FDB,	/* B' = pi/2 - B (high single). */
		0xB0BBBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBC861015,	/* c0 (high single). */
		0xAFD68E2E,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAAFEEB,	/* c1 (low single). */
		0xBB569F3F,	/* c2. */
		0x3CB6A84E,	/* c3. */
		0xBAC64194,	/* c4. */
		0x3CC90FDB,	/* B' = pi/2 - B (high single). */
		0xB03BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0xBC060BF3,	/* c0 (high single). */
		0x2FE251AE,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAABFB9,	/* c1 (low single). */
		0xBAD67C60,	/* c2. */
		0x3CB64CA5,	/* c3. */
		0xBACDE881,	/* c4. */
		0x00000000,	/* B' = pi/2 - B (high single). */
		0x00000000,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x00000000,	/* c0 (high single). */
		0x00000000,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAAAAAB,	/* c1 (low single). */
		0x00000000,	/* c2. */
		0x3CB5E28B,	/* c3. */
		0x00000000,	/* c4. */
		0xBCC90FDB,	/* B' = pi/2 - B (high single). */
		0x303BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3C060BF3,	/* c0 (high single). */
		0xAFE251AE,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAABFB9,	/* c1 (low single). */
		0x3AD67C60,	/* c2. */
		0x3CB64CA5,	/* c3. */
		0x3ACDE881,	/* c4. */
		0xBD490FDB,	/* B' = pi/2 - B (high single). */
		0x30BBBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3C861015,	/* c0 (high single). */
		0x2FD68E2E,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAAFEEB,	/* c1 (low single). */
		0x3B569F3F,	/* c2. */
		0x3CB6A84E,	/* c3. */
		0x3AC64194,	/* c4. */
		0xBD96CBE4,	/* B' = pi/2 - B (high single). */
		0x2F4CDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3CC92278,	/* c0 (high single). */
		0xB02F2E68,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAB6854,	/* c1 (low single). */
		0x3BA1214F,	/* c2. */
		0x3CB6C1E9,	/* c3. */
		0xB843C2F2,	/* c4. */
		0xBDC90FDB,	/* B' = pi/2 - B (high single). */
		0x313BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D0620A3,	/* c0 (high single). */
		0x30ECAB40,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DABFC11,	/* c1 (low single). */
		0x3BD7200F,	/* c2. */
		0x3CB79475,	/* c3. */
		0x3A2B0ADC,	/* c4. */
		0xBDFB53D1,	/* B' = pi/2 - B (high single). */
		0xB1155386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D27B856,	/* c0 (high single). */
		0x30F7EE91,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DACBA4E,	/* c1 (low single). */
		0x3C06AEE3,	/* c2. */
		0x3CB8E5DC,	/* c3. */
		0x3AEC00EE,	/* c4. */
		0xBE16CBE4,	/* B' = pi/2 - B (high single). */
		0x2FCCDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D495A6C,	/* c0 (high single). */
		0x30A427BD,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DADA345,	/* c1 (low single). */
		0x3C21F648,	/* c2. */
		0x3CB9D1B4,	/* c3. */
		0x3ACB5567,	/* c4. */
		0xBE2FEDDF,	/* B' = pi/2 - B (high single). */
		0xB19BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D6B08FF,	/* c0 (high single). */
		0x30EED236,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAEB739,	/* c1 (low single). */
		0x3C3D6D51,	/* c2. */
		0x3CBB485D,	/* c3. */
		0x3AFFF5BA,	/* c4. */
		0xBE490FDB,	/* B' = pi/2 - B (high single). */
		0x31BBBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D866317,	/* c0 (high single). */
		0x2FF02140,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DAFF67D,	/* c1 (low single). */
		0x3C591CD0,	/* c2. */
		0x3CBCBEAD,	/* c3. */
		0x3B04BBEC,	/* c4. */
		0xBE6231D6,	/* B' = pi/2 - B (high single). */
		0x3019A6A2,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3D974A0D,	/* c0 (high single). */
		0x314F365B,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB1616F,	/* c1 (low single). */
		0x3C750CD8,	/* c2. */
		0x3CBEB595,	/* c3. */
		0x3B22B883,	/* c4. */
		0xBE7B53D1,	/* B' = pi/2 - B (high single). */
		0xB1955386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DA83A77,	/* c0 (high single). */
		0xB16D967A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB2F87C,	/* c1 (low single). */
		0x3C88A31F,	/* c2. */
		0x3CC0E763,	/* c3. */
		0x3B3F1666,	/* c4. */
		0xBE8A3AE6,	/* B' = pi/2 - B (high single). */
		0xB21EEDF0,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DB93569,	/* c0 (high single). */
		0x2FB9ED00,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB4BC1F,	/* c1 (low single). */
		0x3C96E905,	/* c2. */
		0x3CC2E6F5,	/* c3. */
		0x3B3E10A6,	/* c4. */
		0xBE96CBE4,	/* B' = pi/2 - B (high single). */
		0x304CDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DCA3BFF,	/* c0 (high single). */
		0xB11C95EA,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB6ACDE,	/* c1 (low single). */
		0x3CA55C5B,	/* c2. */
		0x3CC5BC04,	/* c3. */
		0x3B63A969,	/* c4. */
		0xBEA35CE2,	/* B' = pi/2 - B (high single). */
		0x323889B6,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DDB4F55,	/* c0 (high single). */
		0xB0F6437E,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DB8CB52,	/* c1 (low single). */
		0x3CB40210,	/* c2. */
		0x3CC82D45,	/* c3. */
		0x3B643075,	/* c4. */
		0xBEAFEDDF,	/* B' = pi/2 - B (high single). */
		0xB21BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DEC708C,	/* c0 (high single). */
		0x314895C4,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DBB181E,	/* c1 (low single). */
		0x3CC2DEA6,	/* c2. */
		0x3CCB5027,	/* c3. */
		0x3B7F3969,	/* c4. */
		0xBEBC7EDD,	/* B' = pi/2 - B (high single). */
		0x30800ADD,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3DFDA0CB,	/* c0 (high single). */
		0xAF14FCA0,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DBD93F7,	/* c1 (low single). */
		0x3CD1F71B,	/* c2. */
		0x3CCEDD2B,	/* c3. */
		0x3B905946,	/* c4. */
		0xBEC90FDB,	/* B' = pi/2 - B (high single). */
		0x323BBD2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E07709D,	/* c0 (high single). */
		0x318A2A83,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC03FA2,	/* c1 (low single). */
		0x3CE15096,	/* c2. */
		0x3CD26472,	/* c3. */
		0x3B9A1270,	/* c4. */
		0xBED5A0D8,	/* B' = pi/2 - B (high single). */
		0xB21886FF,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E101985,	/* c0 (high single). */
		0x302FB2B8,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC31BF3,	/* c1 (low single). */
		0x3CF0F04D,	/* c2. */
		0x3CD60BC7,	/* c3. */
		0x3BA138BA,	/* c4. */
		0xBEE231D6,	/* B' = pi/2 - B (high single). */
		0x3099A6A2,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E18CBB7,	/* c0 (high single). */
		0x2FE28430,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC629CE,	/* c1 (low single). */
		0x3D006DCD,	/* c2. */
		0x3CDA5A2C,	/* c3. */
		0x3BB0B3D2,	/* c4. */
		0xBEEEC2D4,	/* B' = pi/2 - B (high single). */
		0x323EF0A7,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E2187D0,	/* c0 (high single). */
		0x31B7C7F7,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DC96A2B,	/* c1 (low single). */
		0x3D088C22,	/* c2. */
		0x3CDE950E,	/* c3. */
		0x3BB89AD1,	/* c4. */
		0xBEFB53D1,	/* B' = pi/2 - B (high single). */
		0xB2155386,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E2A4E71,	/* c0 (high single). */
		0x319CFCEC,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DCCDE11,	/* c1 (low single). */
		0x3D10D605,	/* c2. */
		0x3CE382A7,	/* c3. */
		0x3BC8BD97,	/* c4. */
		0xBF03F267,	/* B' = pi/2 - B (high single). */
		0xB2F4CBD9,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E33203E,	/* c0 (high single). */
		0xB1FEF5BE,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD0869C,	/* c1 (low single). */
		0x3D194E8C,	/* c2. */
		0x3CE8DCA9,	/* c3. */
		0x3BDADA55,	/* c4. */
		0xBF0A3AE6,	/* B' = pi/2 - B (high single). */
		0xB29EEDF0,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E3BFDDC,	/* c0 (high single). */
		0x3132521A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD464FC,	/* c1 (low single). */
		0x3D21F8F1,	/* c2. */
		0x3CEE3076,	/* c3. */
		0x3BE6D263,	/* c4. */
		0xBF108365,	/* B' = pi/2 - B (high single). */
		0xB212200D,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E44E7F8,	/* c0 (high single). */
		0x31CAA3CB,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DD87A74,	/* c1 (low single). */
		0x3D2AD885,	/* c2. */
		0x3CF3C785,	/* c3. */
		0x3BF1E348,	/* c4. */
		0xBF16CBE4,	/* B' = pi/2 - B (high single). */
		0x30CCDE2E,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E4DDF41,	/* c0 (high single). */
		0x31AEA094,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DDCC85C,	/* c1 (low single). */
		0x3D33F0BE,	/* c2. */
		0x3CFA23B0,	/* c3. */
		0x3C01FCF7,	/* c4. */
		0xBF1D1463,	/* B' = pi/2 - B (high single). */
		0x32455799,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E56E46B,	/* c0 (high single). */
		0xB1E3F001,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DE15025,	/* c1 (low single). */
		0x3D3D4550,	/* c2. */
		0x3D00462D,	/* c3. */
		0x3C092C98,	/* c4. */
		0xBF235CE2,	/* B' = pi/2 - B (high single). */
		0x32B889B6,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E5FF82C,	/* c0 (high single). */
		0x3170723A,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DE61354,	/* c1 (low single). */
		0x3D46DA06,	/* c2. */
		0x3D0401F8,	/* c3. */
		0x3C14E013,	/* c4. */
		0xBF29A560,	/* B' = pi/2 - B (high single). */
		0xB2F19861,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E691B44,	/* c0 (high single). */
		0xB1F18936,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DEB138B,	/* c1 (low single). */
		0x3D50B2F7,	/* c2. */
		0x3D07BE3A,	/* c3. */
		0x3C1E46A7,	/* c4. */
		0xBF2FEDDF,	/* B' = pi/2 - B (high single). */
		0xB29BBA77,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E724E73,	/* c0 (high single). */
		0xB120C3E2,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DF05283,	/* c1 (low single). */
		0x3D5AD45E,	/* c2. */
		0x3D0BAFBF,	/* c3. */
		0x3C27B8BB,	/* c4. */
		0xBF36365E,	/* B' = pi/2 - B (high single). */
		0xB20BB91C,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E7B9282,	/* c0 (high single). */
		0x313383D2,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DF5D211,	/* c1 (low single). */
		0x3D6542B3,	/* c2. */
		0x3D0FE5E5,	/* c3. */
		0x3C31FB14,	/* c4. */
		0xBF3C7EDD,	/* B' = pi/2 - B (high single). */
		0x31000ADD,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E827420,	/* c0 (high single). */
		0xB20B8B4D,	/* c0 (low single). */
		0x3E800000,	/* c1 (high 1 bit). */
		0x3DFB9428,	/* c1 (low single). */
		0x3D7002B4,	/* c2. */
		0x3D142A6C,	/* c3. */
		0x3C3A47FF,	/* c4. */
		0xBF42C75C,	/* B' = pi/2 - B (high single). */
		0x324BBE8A,	/* B' = pi/2 - B (low single). */
		0x3F800000,	/* tau (1 for cot path). */
		0x3E87283F,	/* c0 (high single). */
		0x3268B966,	/* c0 (low single). */
		0x3F000000,	/* c1 (high 1 bit). */
		0xBDFE6529,	/* c1 (low single). */
		0x3D7B1953,	/* c2. */
		0x3D18E109,	/* c3. */
		0x3C4570B0,	/* c4. */
		0xBF490FDB,	/* B' = pi/2 - B (high single). */
		0x32BBBD2E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF800000,	/* c0 (high single). */
		0x2B410000,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xB3000000,	/* c1 (low single). */
		0xC0000000,	/* c2. */
		0x402AB7C8,	/* c3. */
		0xC05561DB,	/* c4. */
		0xBF4F5859,	/* B' = pi/2 - B (high single). */
		0xB2EE64E8,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF73BB75,	/* c0 (high single). */
		0xB2FC908D,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBDBF94B0,	/* c1 (low single). */
		0xBFE8550F,	/* c2. */
		0x40174F67,	/* c3. */
		0xC036C608,	/* c4. */
		0xBF55A0D8,	/* B' = pi/2 - B (high single). */
		0xB29886FF,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF68065E,	/* c0 (high single). */
		0xB2670D1A,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBE36D1D6,	/* c1 (low single). */
		0xBFD35007,	/* c2. */
		0x4006A861,	/* c3. */
		0xC01D4BDA,	/* c4. */
		0xBF5BE957,	/* B' = pi/2 - B (high single). */
		0xB205522A,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF5CD3BE,	/* c0 (high single). */
		0xB1460308,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBE8306C5,	/* c1 (low single). */
		0xBFC09232,	/* c2. */
		0x3FF09632,	/* c3. */
		0xC007DB00,	/* c4. */
		0xBF6231D6,	/* B' = pi/2 - B (high single). */
		0x3119A6A2,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF521801,	/* c0 (high single). */
		0xB2AE4178,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEA72938,	/* c1 (low single). */
		0xBFAFCC22,	/* c2. */
		0x3FD7BD4A,	/* c3. */
		0xBFEBB01B,	/* c4. */
		0xBF687A55,	/* B' = pi/2 - B (high single). */
		0x3252257B,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF47C8CC,	/* c0 (high single). */
		0x3200F51A,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEC82C6C,	/* c1 (low single). */
		0xBFA0BAE9,	/* c2. */
		0x3FC2252F,	/* c3. */
		0xBFCD24C7,	/* c4. */
		0xBF6EC2D4,	/* B' = pi/2 - B (high single). */
		0x32BEF0A7,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF3DDCCF,	/* c0 (high single). */
		0xB2D29606,	/* c0 (low single). */
		0x40000000,	/* c1 (high 1 bit). */
		0xBEE6606F,	/* c1 (low single). */
		0xBF9325D6,	/* c2. */
		0x3FAF4E69,	/* c3. */
		0xBFB3080C,	/* c4. */
		0xBF750B52,	/* B' = pi/2 - B (high single). */
		0xB2EB316F,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF344BA9,	/* c0 (high single). */
		0x32B8B0EA,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EFDF4F7,	/* c1 (low single). */
		0xBF86DCA8,	/* c2. */
		0x3F9ED53B,	/* c3. */
		0xBF9CBEDE,	/* c4. */
		0xBF7B53D1,	/* B' = pi/2 - B (high single). */
		0xB2955386,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF2B0DC1,	/* c0 (high single). */
		0xB2AB7EBA,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EE496C2,	/* c1 (low single). */
		0xBF776C40,	/* c2. */
		0x3F9065C1,	/* c3. */
		0xBF89AFB6,	/* c4. */
		0xBF80CE28,	/* B' = pi/2 - B (high single). */
		0xB1FDD672,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF221C37,	/* c0 (high single). */
		0x320C61DC,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3ECD4F71,	/* c1 (low single). */
		0xBF631DAA,	/* c2. */
		0x3F83B471,	/* c3. */
		0xBF7281EA,	/* c4. */
		0xBF83F267,	/* B' = pi/2 - B (high single). */
		0xB374CBD9,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF1970C4,	/* c0 (high single). */
		0xB2904848,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EB7EFF8,	/* c1 (low single). */
		0xBF50907C,	/* c2. */
		0x3F710FEA,	/* c3. */
		0xBF561FED,	/* c4. */
		0xBF8716A7,	/* B' = pi/2 - B (high single). */
		0x32588C6D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF1105AF,	/* c0 (high single). */
		0xB2F045B0,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3EA44EE2,	/* c1 (low single). */
		0xBF3F8FDB,	/* c2. */
		0x3F5D3FD0,	/* c3. */
		0xBF3D0A23,	/* c4. */
		0xBF8A3AE6,	/* B' = pi/2 - B (high single). */
		0xB31EEDF0,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF08D5B9,	/* c0 (high single). */
		0x325EF98E,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E92478D,	/* c1 (low single). */
		0xBF2FEDC9,	/* c2. */
		0x3F4BCD58,	/* c3. */
		0xBF27AE9E,	/* c4. */
		0xBF8D5F26,	/* B' = pi/2 - B (high single). */
		0x330C0105,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBF00DC0D,	/* c0 (high single). */
		0x3214AF72,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E81B994,	/* c1 (low single). */
		0xBF218233,	/* c2. */
		0x3F3C4531,	/* c3. */
		0xBF149688,	/* c4. */
		0xBF908365,	/* B' = pi/2 - B (high single). */
		0xB292200D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBEF22870,	/* c0 (high single). */
		0xB25271F4,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E65107A,	/* c1 (low single). */
		0xBF1429F0,	/* c2. */
		0x3F2E8AFC,	/* c3. */
		0xBF040498,	/* c4. */
		0xBF93A7A5,	/* B' = pi/2 - B (high single). */
		0x3361DEEE,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBEE2F439,	/* c0 (high single). */
		0x31F4399E,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E49341C,	/* c1 (low single). */
		0xBF07C61A,	/* c2. */
		0x3F22560F,	/* c3. */
		0xBEEAA81E,	/* c4. */
		0xBF96CBE4,	/* B' = pi/2 - B (high single). */
		0x314CDE2E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBED413CD,	/* c0 (high single). */
		0x31C06152,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E2FB0CC,	/* c1 (low single). */
		0xBEF876CB,	/* c2. */
		0x3F177807,	/* c3. */
		0xBED08437,	/* c4. */
		0xBF99F023,	/* B' = pi/2 - B (high single). */
		0xB3484328,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBEC5800D,	/* c0 (high single). */
		0x3214C3C1,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E185E54,	/* c1 (low single). */
		0xBEE2E342,	/* c2. */
		0x3F0DCA73,	/* c3. */
		0xBEB8CC21,	/* c4. */
		0xBF9D1463,	/* B' = pi/2 - B (high single). */
		0x32C55799,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBEB73250,	/* c0 (high single). */
		0x32028823,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3E0318F8,	/* c1 (low single). */
		0xBECEA678,	/* c2. */
		0x3F053C67,	/* c3. */
		0xBEA41E53,	/* c4. */
		0xBFA038A2,	/* B' = pi/2 - B (high single). */
		0xB2E4CA7E,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBEA92457,	/* c0 (high single). */
		0xB0B80830,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3DDF8200,	/* c1 (low single). */
		0xBEBB99E9,	/* c2. */
		0x3EFB4AA8,	/* c3. */
		0xBE9182BE,	/* c4. */
		0xBFA35CE2,	/* B' = pi/2 - B (high single). */
		0x333889B6,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE9B5042,	/* c0 (high single). */
		0x322A3AEE,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3DBC7490,	/* c1 (low single). */
		0xBEA99AF5,	/* c2. */
		0x3EEDE107,	/* c3. */
		0xBE80E9AA,	/* c4. */
		0xBFA68121,	/* B' = pi/2 - B (high single). */
		0xB1E43AAC,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE8DB082,	/* c0 (high single). */
		0x3132A234,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D9CD7D0,	/* c1 (low single). */
		0xBE988A60,	/* c2. */
		0x3EE203E3,	/* c3. */
		0xBE63582C,	/* c4. */
		0xBFA9A560,	/* B' = pi/2 - B (high single). */
		0xB3719861,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE803FD4,	/* c0 (high single). */
		0x32279E66,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D807FC8,	/* c1 (low single). */
		0xBE884BD4,	/* c2. */
		0x3ED7812D,	/* c3. */
		0xBE4636EB,	/* c4. */
		0xBFACC9A0,	/* B' = pi/2 - B (high single). */
		0x32655A50,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE65F267,	/* c0 (high single). */
		0xB1B4B1DF,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D4E8B90,	/* c1 (low single). */
		0xBE718ACA,	/* c2. */
		0x3ECE7164,	/* c3. */
		0xBE2DC161,	/* c4. */
		0xBFAFEDDF,	/* B' = pi/2 - B (high single). */
		0xB31BBA77,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE4BAFAF,	/* c0 (high single). */
		0xAF2A29E0,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3D221018,	/* c1 (low single). */
		0xBE53BED0,	/* c2. */
		0x3EC67E26,	/* c3. */
		0xBE1568E2,	/* c4. */
		0xBFB3121F,	/* B' = pi/2 - B (high single). */
		0x330F347D,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE31AE4D,	/* c0 (high single). */
		0x31F32251,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3CF6A500,	/* c1 (low single). */
		0xBE3707DA,	/* c2. */
		0x3EBFA489,	/* c3. */
		0xBDFBD9C7,	/* c4. */
		0xBFB6365E,	/* B' = pi/2 - B (high single). */
		0xB28BB91C,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBE17E564,	/* c0 (high single). */
		0x31C5A2E4,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3CB440D0,	/* c1 (low single). */
		0xBE1B3D00,	/* c2. */
		0x3EB9F664,	/* c3. */
		0xBDD647C0,	/* c4. */
		0xBFB95A9E,	/* B' = pi/2 - B (high single). */
		0x33651267,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBDFC98C2,	/* c0 (high single). */
		0x30AE525C,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3C793D20,	/* c1 (low single). */
		0xBE003845,	/* c2. */
		0x3EB5271F,	/* c3. */
		0xBDAC669E,	/* c4. */
		0xBFBC7EDD,	/* B' = pi/2 - B (high single). */
		0x31800ADD,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBDC9B5DC,	/* c0 (high single). */
		0xB145AD86,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3C1EEF20,	/* c1 (low single). */
		0xBDCBAAEA,	/* c2. */
		0x3EB14E5E,	/* c3. */
		0xBD858BB2,	/* c4. */
		0xBFBFA31C,	/* B' = pi/2 - B (high single). */
		0xB3450FB0,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBD9711CE,	/* c0 (high single). */
		0xB14FEB28,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3BB24C00,	/* c1 (low single). */
		0xBD97E43A,	/* c2. */
		0x3EAE6A89,	/* c3. */
		0xBD4D07E0,	/* c4. */
		0xBFC2C75C,	/* B' = pi/2 - B (high single). */
		0x32CBBE8A,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBD49393C,	/* c0 (high single). */
		0xB0A39F5B,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3B1E2B00,	/* c1 (low single). */
		0xBD49B5D4,	/* c2. */
		0x3EAC4F10,	/* c3. */
		0xBCFD9425,	/* c4. */
		0xBFC5EB9B,	/* B' = pi/2 - B (high single). */
		0xB2DE638C,	/* B' = pi/2 - B (low single). */
		0x00000000,	/* tau (1 for cot path). */
		0xBCC91A31,	/* c0 (high single). */
		0xAF8E8D1A,	/* c0 (low single). */
		0x3F800000,	/* c1 (high 1 bit). */
		0x3A1DFA00,	/* c1 (low single). */
		0xBCC9392D,	/* c2. */
		0x3EAB1889,	/* c3. */
		0xBC885D3B,	/* c4. */
		/* So its save to read 32 bytes past the end.  */
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000)
	.type	dev_tanf_data_common_avx2, @object
	.size	dev_tanf_data_common_avx2, .-dev_tanf_data_common_avx2

	.section .rodata.evex512, "a"
	.align	64
	.globl	dev_tanf_data_common_avx512
dev_tanf_data_common_avx512:
	float_block (dev_tanf_data_common_avx512, _Reduction,
	/* P_hi     P_med       P_lo.  */
		0x00000000, 0x00000000, 0x00000000,	/* 0 */
		0x00000000, 0x00000000, 0x00000000,	/* 1 */
		0x00000000, 0x00000000, 0x00000000,	/* 2 */
		0x00000000, 0x00000000, 0x00000000,	/* 3 */
		0x00000000, 0x00000000, 0x00000000,	/* 4 */
		0x00000000, 0x00000000, 0x00000000,	/* 5 */
		0x00000000, 0x00000000, 0x00000000,	/* 6 */
		0x00000000, 0x00000000, 0x00000000,	/* 7 */
		0x00000000, 0x00000000, 0x00000000,	/* 8 */
		0x00000000, 0x00000000, 0x00000000,	/* 9 */
		0x00000000, 0x00000000, 0x00000000,	/* 10 */
		0x00000000, 0x00000000, 0x00000000,	/* 11 */
		0x00000000, 0x00000000, 0x00000000,	/* 12 */
		0x00000000, 0x00000000, 0x00000000,	/* 13 */
		0x00000000, 0x00000000, 0x00000000,	/* 14 */
		0x00000000, 0x00000000, 0x00000000,	/* 15 */
		0x00000000, 0x00000000, 0x00000000,	/* 16 */
		0x00000000, 0x00000000, 0x00000000,	/* 17 */
		0x00000000, 0x00000000, 0x00000000,	/* 18 */
		0x00000000, 0x00000000, 0x00000000,	/* 19 */
		0x00000000, 0x00000000, 0x00000000,	/* 20 */
		0x00000000, 0x00000000, 0x00000000,	/* 21 */
		0x00000000, 0x00000000, 0x00000000,	/* 22 */
		0x00000000, 0x00000000, 0x00000000,	/* 23 */
		0x00000000, 0x00000000, 0x00000000,	/* 24 */
		0x00000000, 0x00000000, 0x00000000,	/* 25 */
		0x00000000, 0x00000000, 0x00000000,	/* 26 */
		0x00000000, 0x00000000, 0x00000000,	/* 27 */
		0x00000000, 0x00000000, 0x00000000,	/* 28 */
		0x00000000, 0x00000000, 0x00000000,	/* 29 */
		0x00000000, 0x00000000, 0x00000000,	/* 30 */
		0x00000000, 0x00000000, 0x00000000,	/* 31 */
		0x00000000, 0x00000000, 0x00000000,	/* 32 */
		0x00000000, 0x00000000, 0x00000000,	/* 33 */
		0x00000000, 0x00000000, 0x00000000,	/* 34 */
		0x00000000, 0x00000000, 0x00000000,	/* 35 */
		0x00000000, 0x00000000, 0x00000000,	/* 36 */
		0x00000000, 0x00000000, 0x00000000,	/* 37 */
		0x00000000, 0x00000000, 0x00000000,	/* 38 */
		0x00000000, 0x00000000, 0x00000000,	/* 39 */
		0x00000000, 0x00000000, 0x00000000,	/* 40 */
		0x00000000, 0x00000000, 0x00000000,	/* 41 */
		0x00000000, 0x00000000, 0x00000000,	/* 42 */
		0x00000000, 0x00000000, 0x00000000,	/* 43 */
		0x00000000, 0x00000000, 0x00000000,	/* 44 */
		0x00000000, 0x00000000, 0x00000000,	/* 45 */
		0x00000000, 0x00000000, 0x00000000,	/* 46 */
		0x00000000, 0x00000000, 0x00000000,	/* 47 */
		0x00000000, 0x00000000, 0x00000000,	/* 48 */
		0x00000000, 0x00000000, 0x00000000,	/* 49 */
		0x00000000, 0x00000000, 0x00000000,	/* 50 */
		0x00000000, 0x00000000, 0x00000000,	/* 51 */
		0x00000000, 0x00000000, 0x00000000,	/* 52 */
		0x00000000, 0x00000000, 0x00000000,	/* 53 */
		0x00000000, 0x00000000, 0x00000000,	/* 54 */
		0x00000000, 0x00000000, 0x00000000,	/* 55 */
		0x00000000, 0x00000000, 0x00000000,	/* 56 */
		0x00000000, 0x00000000, 0x00000001,	/* 57 */
		0x00000000, 0x00000000, 0x00000002,	/* 58 */
		0x00000000, 0x00000000, 0x00000005,	/* 59 */
		0x00000000, 0x00000000, 0x0000000A,	/* 60 */
		0x00000000, 0x00000000, 0x00000014,	/* 61 */
		0x00000000, 0x00000000, 0x00000028,	/* 62 */
		0x00000000, 0x00000000, 0x00000051,	/* 63 */
		0x00000000, 0x00000000, 0x000000A2,	/* 64 */
		0x00000000, 0x00000000, 0x00000145,	/* 65 */
		0x00000000, 0x00000000, 0x0000028B,	/* 66 */
		0x00000000, 0x00000000, 0x00000517,	/* 67 */
		0x00000000, 0x00000000, 0x00000A2F,	/* 68 */
		0x00000000, 0x00000000, 0x0000145F,	/* 69 */
		0x00000000, 0x00000000, 0x000028BE,	/* 70 */
		0x00000000, 0x00000000, 0x0000517C,	/* 71 */
		0x00000000, 0x00000000, 0x0000A2F9,	/* 72 */
		0x00000000, 0x00000000, 0x000145F3,	/* 73 */
		0x00000000, 0x00000000, 0x00028BE6,	/* 74 */
		0x00000000, 0x00000000, 0x000517CC,	/* 75 */
		0x00000000, 0x00000000, 0x000A2F98,	/* 76 */
		0x00000000, 0x00000000, 0x00145F30,	/* 77 */
		0x00000000, 0x00000000, 0x0028BE60,	/* 78 */
		0x00000000, 0x00000000, 0x00517CC1,	/* 79 */
		0x00000000, 0x00000000, 0x00A2F983,	/* 80 */
		0x00000000, 0x00000000, 0x0145F306,	/* 81 */
		0x00000000, 0x00000000, 0x028BE60D,	/* 82 */
		0x00000000, 0x00000000, 0x0517CC1B,	/* 83 */
		0x00000000, 0x00000000, 0x0A2F9836,	/* 84 */
		0x00000000, 0x00000000, 0x145F306D,	/* 85 */
		0x00000000, 0x00000000, 0x28BE60DB,	/* 86 */
		0x00000000, 0x00000000, 0x517CC1B7,	/* 87 */
		0x00000000, 0x00000000, 0xA2F9836E,	/* 88 */
		0x00000000, 0x00000001, 0x45F306DC,	/* 89 */
		0x00000000, 0x00000002, 0x8BE60DB9,	/* 90 */
		0x00000000, 0x00000005, 0x17CC1B72,	/* 91 */
		0x00000000, 0x0000000A, 0x2F9836E4,	/* 92 */
		0x00000000, 0x00000014, 0x5F306DC9,	/* 93 */
		0x00000000, 0x00000028, 0xBE60DB93,	/* 94 */
		0x00000000, 0x00000051, 0x7CC1B727,	/* 95 */
		0x00000000, 0x000000A2, 0xF9836E4E,	/* 96 */
		0x00000000, 0x00000145, 0xF306DC9C,	/* 97 */
		0x00000000, 0x0000028B, 0xE60DB939,	/* 98 */
		0x00000000, 0x00000517, 0xCC1B7272,	/* 99 */
		0x00000000, 0x00000A2F, 0x9836E4E4,	/* 100 */
		0x00000000, 0x0000145F, 0x306DC9C8,	/* 101 */
		0x00000000, 0x000028BE, 0x60DB9391,	/* 102 */
		0x00000000, 0x0000517C, 0xC1B72722,	/* 103 */
		0x00000000, 0x0000A2F9, 0x836E4E44,	/* 104 */
		0x00000000, 0x000145F3, 0x06DC9C88,	/* 105 */
		0x00000000, 0x00028BE6, 0x0DB93910,	/* 106 */
		0x00000000, 0x000517CC, 0x1B727220,	/* 107 */
		0x00000000, 0x000A2F98, 0x36E4E441,	/* 108 */
		0x00000000, 0x00145F30, 0x6DC9C882,	/* 109 */
		0x00000000, 0x0028BE60, 0xDB939105,	/* 110 */
		0x00000000, 0x00517CC1, 0xB727220A,	/* 111 */
		0x00000000, 0x00A2F983, 0x6E4E4415,	/* 112 */
		0x00000000, 0x0145F306, 0xDC9C882A,	/* 113 */
		0x00000000, 0x028BE60D, 0xB9391054,	/* 114 */
		0x00000000, 0x0517CC1B, 0x727220A9,	/* 115 */
		0x00000000, 0x0A2F9836, 0xE4E44152,	/* 116 */
		0x00000000, 0x145F306D, 0xC9C882A5,	/* 117 */
		0x00000000, 0x28BE60DB, 0x9391054A,	/* 118 */
		0x00000000, 0x517CC1B7, 0x27220A94,	/* 119 */
		0x00000000, 0xA2F9836E, 0x4E441529,	/* 120 */
		0x00000001, 0x45F306DC, 0x9C882A53,	/* 121 */
		0x00000002, 0x8BE60DB9, 0x391054A7,	/* 122 */
		0x00000005, 0x17CC1B72, 0x7220A94F,	/* 123 */
		0x0000000A, 0x2F9836E4, 0xE441529F,	/* 124 */
		0x00000014, 0x5F306DC9, 0xC882A53F,	/* 125 */
		0x00000028, 0xBE60DB93, 0x91054A7F,	/* 126 */
		0x00000051, 0x7CC1B727, 0x220A94FE,	/* 127 */
		0x000000A2, 0xF9836E4E, 0x441529FC,	/* 128 */
		0x00000145, 0xF306DC9C, 0x882A53F8,	/* 129 */
		0x0000028B, 0xE60DB939, 0x1054A7F0,	/* 130 */
		0x00000517, 0xCC1B7272, 0x20A94FE1,	/* 131 */
		0x00000A2F, 0x9836E4E4, 0x41529FC2,	/* 132 */
		0x0000145F, 0x306DC9C8, 0x82A53F84,	/* 133 */
		0x000028BE, 0x60DB9391, 0x054A7F09,	/* 134 */
		0x0000517C, 0xC1B72722, 0x0A94FE13,	/* 135 */
		0x0000A2F9, 0x836E4E44, 0x1529FC27,	/* 136 */
		0x000145F3, 0x06DC9C88, 0x2A53F84E,	/* 137 */
		0x00028BE6, 0x0DB93910, 0x54A7F09D,	/* 138 */
		0x000517CC, 0x1B727220, 0xA94FE13A,	/* 139 */
		0x000A2F98, 0x36E4E441, 0x529FC275,	/* 140 */
		0x00145F30, 0x6DC9C882, 0xA53F84EA,	/* 141 */
		0x0028BE60, 0xDB939105, 0x4A7F09D5,	/* 142 */
		0x00517CC1, 0xB727220A, 0x94FE13AB,	/* 143 */
		0x00A2F983, 0x6E4E4415, 0x29FC2757,	/* 144 */
		0x0145F306, 0xDC9C882A, 0x53F84EAF,	/* 145 */
		0x028BE60D, 0xB9391054, 0xA7F09D5F,	/* 146 */
		0x0517CC1B, 0x727220A9, 0x4FE13ABE,	/* 147 */
		0x0A2F9836, 0xE4E44152, 0x9FC2757D,	/* 148 */
		0x145F306D, 0xC9C882A5, 0x3F84EAFA,	/* 149 */
		0x28BE60DB, 0x9391054A, 0x7F09D5F4,	/* 150 */
		0x517CC1B7, 0x27220A94, 0xFE13ABE8,	/* 151 */
		0xA2F9836E, 0x4E441529, 0xFC2757D1,	/* 152 */
		0x45F306DC, 0x9C882A53, 0xF84EAFA3,	/* 153 */
		0x8BE60DB9, 0x391054A7, 0xF09D5F47,	/* 154 */
		0x17CC1B72, 0x7220A94F, 0xE13ABE8F,	/* 155 */
		0x2F9836E4, 0xE441529F, 0xC2757D1F,	/* 156 */
		0x5F306DC9, 0xC882A53F, 0x84EAFA3E,	/* 157 */
		0xBE60DB93, 0x91054A7F, 0x09D5F47D,	/* 158 */
		0x7CC1B727, 0x220A94FE, 0x13ABE8FA,	/* 159 */
		0xF9836E4E, 0x441529FC, 0x2757D1F5,	/* 160 */
		0xF306DC9C, 0x882A53F8, 0x4EAFA3EA,	/* 161 */
		0xE60DB939, 0x1054A7F0, 0x9D5F47D4,	/* 162 */
		0xCC1B7272, 0x20A94FE1, 0x3ABE8FA9,	/* 163 */
		0x9836E4E4, 0x41529FC2, 0x757D1F53,	/* 164 */
		0x306DC9C8, 0x82A53F84, 0xEAFA3EA6,	/* 165 */
		0x60DB9391, 0x054A7F09, 0xD5F47D4D,	/* 166 */
		0xC1B72722, 0x0A94FE13, 0xABE8FA9A,	/* 167 */
		0x836E4E44, 0x1529FC27, 0x57D1F534,	/* 168 */
		0x06DC9C88, 0x2A53F84E, 0xAFA3EA69,	/* 169 */
		0x0DB93910, 0x54A7F09D, 0x5F47D4D3,	/* 170 */
		0x1B727220, 0xA94FE13A, 0xBE8FA9A6,	/* 171 */
		0x36E4E441, 0x529FC275, 0x7D1F534D,	/* 172 */
		0x6DC9C882, 0xA53F84EA, 0xFA3EA69B,	/* 173 */
		0xDB939105, 0x4A7F09D5, 0xF47D4D37,	/* 174 */
		0xB727220A, 0x94FE13AB, 0xE8FA9A6E,	/* 175 */
		0x6E4E4415, 0x29FC2757, 0xD1F534DD,	/* 176 */
		0xDC9C882A, 0x53F84EAF, 0xA3EA69BB,	/* 177 */
		0xB9391054, 0xA7F09D5F, 0x47D4D377,	/* 178 */
		0x727220A9, 0x4FE13ABE, 0x8FA9A6EE,	/* 179 */
		0xE4E44152, 0x9FC2757D, 0x1F534DDC,	/* 180 */
		0xC9C882A5, 0x3F84EAFA, 0x3EA69BB8,	/* 181 */
		0x9391054A, 0x7F09D5F4, 0x7D4D3770,	/* 182 */
		0x27220A94, 0xFE13ABE8, 0xFA9A6EE0,	/* 183 */
		0x4E441529, 0xFC2757D1, 0xF534DDC0,	/* 184 */
		0x9C882A53, 0xF84EAFA3, 0xEA69BB81,	/* 185 */
		0x391054A7, 0xF09D5F47, 0xD4D37703,	/* 186 */
		0x7220A94F, 0xE13ABE8F, 0xA9A6EE06,	/* 187 */
		0xE441529F, 0xC2757D1F, 0x534DDC0D,	/* 188 */
		0xC882A53F, 0x84EAFA3E, 0xA69BB81B,	/* 189 */
		0x91054A7F, 0x09D5F47D, 0x4D377036,	/* 190 */
		0x220A94FE, 0x13ABE8FA, 0x9A6EE06D,	/* 191 */
		0x441529FC, 0x2757D1F5, 0x34DDC0DB,	/* 192 */
		0x882A53F8, 0x4EAFA3EA, 0x69BB81B6,	/* 193 */
		0x1054A7F0, 0x9D5F47D4, 0xD377036D,	/* 194 */
		0x20A94FE1, 0x3ABE8FA9, 0xA6EE06DB,	/* 195 */
		0x41529FC2, 0x757D1F53, 0x4DDC0DB6,	/* 196 */
		0x82A53F84, 0xEAFA3EA6, 0x9BB81B6C,	/* 197 */
		0x054A7F09, 0xD5F47D4D, 0x377036D8,	/* 198 */
		0x0A94FE13, 0xABE8FA9A, 0x6EE06DB1,	/* 199 */
		0x1529FC27, 0x57D1F534, 0xDDC0DB62,	/* 200 */
		0x2A53F84E, 0xAFA3EA69, 0xBB81B6C5,	/* 201 */
		0x54A7F09D, 0x5F47D4D3, 0x77036D8A,	/* 202 */
		0xA94FE13A, 0xBE8FA9A6, 0xEE06DB14,	/* 203 */
		0x529FC275, 0x7D1F534D, 0xDC0DB629,	/* 204 */
		0xA53F84EA, 0xFA3EA69B, 0xB81B6C52,	/* 205 */
		0x4A7F09D5, 0xF47D4D37, 0x7036D8A5,	/* 206 */
		0x94FE13AB, 0xE8FA9A6E, 0xE06DB14A,	/* 207 */
		0x29FC2757, 0xD1F534DD, 0xC0DB6295,	/* 208 */
		0x53F84EAF, 0xA3EA69BB, 0x81B6C52B,	/* 209 */
		0xA7F09D5F, 0x47D4D377, 0x036D8A56,	/* 210 */
		0x4FE13ABE, 0x8FA9A6EE, 0x06DB14AC,	/* 211 */
		0x9FC2757D, 0x1F534DDC, 0x0DB62959,	/* 212 */
		0x3F84EAFA, 0x3EA69BB8, 0x1B6C52B3,	/* 213 */
		0x7F09D5F4, 0x7D4D3770, 0x36D8A566,	/* 214 */
		0xFE13ABE8, 0xFA9A6EE0, 0x6DB14ACC,	/* 215 */
		0xFC2757D1, 0xF534DDC0, 0xDB629599,	/* 216 */
		0xF84EAFA3, 0xEA69BB81, 0xB6C52B32,	/* 217 */
		0xF09D5F47, 0xD4D37703, 0x6D8A5664,	/* 218 */
		0xE13ABE8F, 0xA9A6EE06, 0xDB14ACC9,	/* 219 */
		0xC2757D1F, 0x534DDC0D, 0xB6295993,	/* 220 */
		0x84EAFA3E, 0xA69BB81B, 0x6C52B327,	/* 221 */
		0x09D5F47D, 0x4D377036, 0xD8A5664F,	/* 222 */
		0x13ABE8FA, 0x9A6EE06D, 0xB14ACC9E,	/* 223 */
		0x2757D1F5, 0x34DDC0DB, 0x6295993C,	/* 224 */
		0x4EAFA3EA, 0x69BB81B6, 0xC52B3278,	/* 225 */
		0x9D5F47D4, 0xD377036D, 0x8A5664F1,	/* 226 */
		0x3ABE8FA9, 0xA6EE06DB, 0x14ACC9E2,	/* 227 */
		0x757D1F53, 0x4DDC0DB6, 0x295993C4,	/* 228 */
		0xEAFA3EA6, 0x9BB81B6C, 0x52B32788,	/* 229 */
		0xD5F47D4D, 0x377036D8, 0xA5664F10,	/* 230 */
		0xABE8FA9A, 0x6EE06DB1, 0x4ACC9E21,	/* 231 */
		0x57D1F534, 0xDDC0DB62, 0x95993C43,	/* 232 */
		0xAFA3EA69, 0xBB81B6C5, 0x2B327887,	/* 233 */
		0x5F47D4D3, 0x77036D8A, 0x5664F10E,	/* 234 */
		0xBE8FA9A6, 0xEE06DB14, 0xACC9E21C,	/* 235 */
		0x7D1F534D, 0xDC0DB629, 0x5993C439,	/* 236 */
		0xFA3EA69B, 0xB81B6C52, 0xB3278872,	/* 237 */
		0xF47D4D37, 0x7036D8A5, 0x664F10E4,	/* 238 */
		0xE8FA9A6E, 0xE06DB14A, 0xCC9E21C8,	/* 239 */
		0xD1F534DD, 0xC0DB6295, 0x993C4390,	/* 240 */
		0xA3EA69BB, 0x81B6C52B, 0x32788720,	/* 241 */
		0x47D4D377, 0x036D8A56, 0x64F10E41,	/* 242 */
		0x8FA9A6EE, 0x06DB14AC, 0xC9E21C82,	/* 243 */
		0x1F534DDC, 0x0DB62959, 0x93C43904,	/* 244 */
		0x3EA69BB8, 0x1B6C52B3, 0x27887208,	/* 245 */
		0x7D4D3770, 0x36D8A566, 0x4F10E410,	/* 246 */
		0xFA9A6EE0, 0x6DB14ACC, 0x9E21C820,	/* 247 */
		0xF534DDC0, 0xDB629599, 0x3C439041,	/* 248 */
		0xEA69BB81, 0xB6C52B32, 0x78872083,	/* 249 */
		0xD4D37703, 0x6D8A5664, 0xF10E4107,	/* 250 */
		0xA9A6EE06, 0xDB14ACC9, 0xE21C820F,	/* 251 */
		0x534DDC0D, 0xB6295993, 0xC439041F,	/* 252 */
		0xA69BB81B, 0x6C52B327, 0x8872083F,	/* 253 */
		0x4D377036, 0xD8A5664F, 0x10E4107F,	/* 254 */
		0x9A6EE06D, 0xB14ACC9E, 0x21C820FF,	/* 255 */
		/* Exists so that its save to read 32-bytes past the end.  */
		0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000)
	.type	dev_tanf_data_common_avx512, @object
	.size	dev_tanf_data_common_avx512, .-dev_tanf_data_common_avx512
#endif
