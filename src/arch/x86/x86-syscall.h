#ifndef _SRC__ARCH__X86__LL_SYSCALL_H_
#define _SRC__ARCH__X86__LL_SYSCALL_H_

#include "../../util/macro.h"
#include "../../util/types.h"

#define _x86_internal_syscall(reg_setup, reg_decls...)                         \
    ({                                                                         \
        DEPAREN(reg_setup);                                                    \
        asm volatile("syscall"                                                 \
                     : "+r"(_rax)                                              \
                     : reg_decls                                               \
                     : "memory", "r11", "rcx");                                \
        _rax;                                                                  \
    })


#define _x86_internal_pin_reg(reg, val)                                        \
    register get_type(val) CAT(_, reg) __asm__(V_TO_STR(reg)) = (val)


#define _x86_internal_pin_reg1(SYS_num) _x86_internal_pin_reg(rax, SYS_num)
#define _x86_internal_pin_reg2(SYS_num, arg0)                                  \
    _x86_internal_pin_reg1(SYS_num);                                           \
    _x86_internal_pin_reg(rdi, arg0)
#define _x86_internal_pin_reg3(SYS_num, arg0, arg1)                            \
    _x86_internal_pin_reg2(SYS_num, arg0);                                     \
    _x86_internal_pin_reg(rsi, arg1)
#define _x86_internal_pin_reg4(SYS_num, arg0, arg1, arg2)                      \
    _x86_internal_pin_reg3(SYS_num, arg0, arg1);                               \
    _x86_internal_pin_reg(rdx, arg2)
#define _x86_internal_pin_reg5(SYS_num, arg0, arg1, arg2, arg3)                \
    _x86_internal_pin_reg4(SYS_num, arg0, arg1, arg2);                         \
    _x86_internal_pin_reg(r10, arg3)
#define _x86_internal_pin_reg6(SYS_num, arg0, arg1, arg2, arg3, arg4)          \
    _x86_internal_pin_reg5(SYS_num, arg0, arg1, arg2, arg3);                   \
    _x86_internal_pin_reg(r8, arg4)
#define _x86_internal_pin_reg7(SYS_num, arg0, arg1, arg2, arg3, arg4, arg5)    \
    _x86_internal_pin_reg6(SYS_num, arg0, arg1, arg2, arg3, arg4);             \
    _x86_internal_pin_reg(r9, arg5)

#define _x86_internal_reg1
#define _x86_internal_reg2 "r"(_rdi)
#define _x86_internal_reg3 _x86_internal_reg2, "r"(_rsi)
#define _x86_internal_reg4 _x86_internal_reg3, "r"(_rdx)
#define _x86_internal_reg5 _x86_internal_reg4, "r"(_r10)
#define _x86_internal_reg6 _x86_internal_reg5, "r"(_r8)
#define _x86_internal_reg7 _x86_internal_reg6, "r"(_r9)


#define _x86_internal_syscallN(N, ...)                                         \
    _x86_internal_syscall(CAT(_x86_internal_pin_reg, N)(__VA_ARGS__),          \
                          CAT(_x86_internal_reg, N))

#define _syscall(...) _x86_internal_syscallN(PP_NARG(__VA_ARGS__), __VA_ARGS__)

#endif
