#ifndef _SRC__ASM__LIBC_ASM_COMMON_H_
#define _SRC__ASM__LIBC_ASM_COMMON_H_

#define RCSID(id)
#define USE_MULTIARCH
#define IS_IN(x) 1

#define ZERO_UPPER_VEC_REGISTERS_RETURN VZEROUPPER_RETURN
#define VZEROUPPER_RETURN                                                      \
    VZEROUPPER;                                                                \
    ret

#define RAX_LP rax
#define RDI_LP rdi
#define RSI_LP rsi
#define RDX_LP rdx
#define RCX_LP rcx

#define R8_LP r8
#define R9_LP r9

#define R10_LP r10
#define R11_LP r11
#define R12_LP r12
#define R13_LP r13

#define _CET_ENDBR   endbr4
#define _CET_NOTRACK notrack

#define weak_alias(...)
#define hidden_def(...)
#define strong_alias(...)
#define libmvec_hidden_def(...)
#define libm_alias_float(...)
#define libm_alias_double(...)

#ifdef JUMPTARGET
# error "QQQ"
#endif
#ifdef HIDDEN_JUMPTARGET
# error "QQQ@"
#endif

#define HIDDEN_JUMPTARGET(f) f
#define JUMPTARGET(f)        f

#define DEV_OR_GLIBC_dlog_data   I_svml_dlog_data
#define DEV_OR_GLIBC_dexp_data   I_svml_dexp_data
#define DEV_OR_GLIBC_slog_data   I_svml_slog_data
#define DEV_OR_GLIBC_dpow_data   I_svml_dpow_data
#define DEV_OR_GLIBC_s_trig_data I_svml_s_trig_data
#define DEV_OR_GLIBC_spow_data   I_svml_spow_data
#define DEV_OR_GLIBC_d_trig_data I_svml_d_trig_data
#define DEV_OR_GLIBC_sexp_data   I_svml_sexp_data

#include "asm-common.h"

#endif
