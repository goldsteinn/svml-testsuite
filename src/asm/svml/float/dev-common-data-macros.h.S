#ifndef SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_DATA_MACROS_H_S_
#define SRC_D_ASM_D_SVML_D_FLOAT_D_DEV_COMMON_DATA_MACROS_H_S_

.macro check_offset data_section offset
	.if	.-\data_section != \offset
	.err
	.endif
.endm

.macro float_vectorN data_section N offset value
	check_offset \data_section \offset
	.rept	\N
	.long	\value
	.endr
.endm

#define float_block(data_section, offset, ...)	\
	check_offset data_section offset;	\
	.long	__VA_ARGS__

    
#define float_vector16(data_section, offset, value)	\
	float_vectorN data_section 4 offset value
#define float_vector32(data_section, offset, value)	\
	float_vectorN data_section 8 offset value
#define float_vector64(data_section, offset, value)	\
	float_vectorN data_section 16 offset value


#endif
