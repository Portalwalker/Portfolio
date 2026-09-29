#include "toHim.h"

__attribute__((noinline, used))
long system_call(long syscall_number, long arguments[MAX_SYSCALL_ARGS])
{

    #if defined(ARCH_X8664)

    /*
     * x86-64 Linux syscall ABI:
     *
     *   rax = syscall number
     *   rdi = argument 0
     *   rsi = argument 1
     *   rdx = argument 2
     *   r10 = argument 3
     *   r8  = argument 4
     *   r9  = argument 5
     *
     * syscall clobbers:
     *
     *   rcx   saved user RIP
     *   r11   saved user RFLAGS
     *   condition codes
     */
    register long syscall_number_register asm("rax") = syscall_number;
    register long argument0_register    asm("rdi") = arguments[0];
    register long argument1_register    asm("rsi") = arguments[1];
    register long argument2_register    asm("rdx") = arguments[2];
    register long argument3_register    asm("r10") = arguments[3];
    register long argument4_register    asm("r8")  = arguments[4];
    register long argument5_register    asm("r9")  = arguments[5];

     __asm__ volatile (
                            /*
                             * Save the original rbp and rsp.
                             */
                            "push %%rbp\n\t"
                            "mov  %%rsp, %%rbp\n\t"

                            /*
                             * Align rsp downward to a 16-byte boundary.
                             */
                            "and  $-16, %%rsp\n\t"

                            /*
                             * Reserve space and save the pre-alignment rsp.
                             */
                            "sub  $16, %%rsp\n\t"
                            "mov  %%rbp, (%%rsp)\n\t"

                            /*
                             * rsp is 16-byte aligned here.
                             */
                            "syscall\n\t"

                            /*
                             * Restore the exact original rsp.
                             */
                            "mov  (%%rsp), %%rsp\n\t"

                            /*
                             * Restore the original rbp.
                             */
                            "pop  %%rbp\n\t"

        : "+a" (syscall_number_register),
          "+D" (argument0_register),
          "+S" (argument1_register),
          "+d" (argument2_register),
          "+r" (argument3_register),
          "+r" (argument4_register),
          "+r" (argument5_register)
        :
        : "rbp", "rcx", "r11", "cc", "memory"
    );

    return (long)syscall_number_register;




                                            #elif defined(ARCH_ARM64)

                                            /*
                                             * AArch64 Linux syscall ABI:
                                             *
                                             *   x8      = syscall number
                                             *   x0..x5  = arguments 0..5
                                             *   x0      = return value
                                             *
                                             * svc changes processor state and the kernel may modify caller-saved
                                             * registers. Mark the argument registers as read/write operands.
                                             */
                                            register long syscall_number_register asm("x8") = syscall_number;
                                            register long argument0_register    asm("x0") = arguments[0];
                                            register long argument1_register    asm("x1") = arguments[1];
                                            register long argument2_register    asm("x2") = arguments[2];
                                            register long argument3_register    asm("x3") = arguments[3];
                                            register long argument4_register    asm("x4") = arguments[4];
                                            register long argument5_register    asm("x5") = arguments[5];

                                            __asm__ volatile (
                                                "svc 0"
                                                : "+r" (argument0_register),
                                                  "+r" (argument1_register),
                                                  "+r" (argument2_register),
                                                  "+r" (argument3_register),
                                                  "+r" (argument4_register),
                                                  "+r" (argument5_register),
                                                  "+r" (syscall_number_register)
                                                :
                                                : "cc", "memory"
                                            );

                                            return (long)argument0_register;




    #elif defined(ARCH_I386)

    /*
     * i386 Linux syscall ABI:
     *
     *   eax = syscall number
     *   ebx = argument 0
     *   ecx = argument 1
     *   edx = argument 2
     *   esi = argument 3
     *   edi = argument 4
     *   ebp = argument 5
     *
     * int $0x80 returns the result in eax.
     *
     * The "b" constraint is required because Linux requires argument 0
     * specifically in ebx. When building 32-bit position-independent code,
     * use a compiler configuration that permits ebx allocation for inline
     * system calls, commonly -fno-pic for this translation unit.
     */
    register long syscall_number_register asm("eax") = syscall_number;
    register long argument0_register    asm("ebx") = arguments[0];
    register long argument1_register    asm("ecx") = arguments[1];
    register long argument2_register    asm("edx") = arguments[2];
    register long argument3_register    asm("esi") = arguments[3];
    register long argument4_register    asm("edi") = arguments[4];
    register long argument5_register    asm("ebp") = arguments[5];

    __asm__ volatile (
        "int $0x80"
        : "+a" (syscall_number_register),
          "+b" (argument0_register),
          "+c" (argument1_register),
          "+d" (argument2_register),
          "+S" (argument3_register),
          "+D" (argument4_register),
          "+r" (argument5_register)
        :
        : "cc", "memory"
    );

    return (long)(unsigned int)syscall_number_register;




                                                            #elif defined(ARCH_ARM32)

                                                            /*
                                                             * ARM EABI Linux syscall ABI:
                                                             *
                                                             *   r7      = syscall number
                                                             *   r0..r6  = arguments 0..5
                                                             *   r0      = return value
                                                             *
                                                             * The immediate SVC value is zero for ARM EABI. The syscall number
                                                             * belongs in r7, not in the older OABI-encoded SWI number.
                                                             */
                                                            register long syscall_number_register asm("r7") = syscall_number;
                                                            register long argument0_register    asm("r0") = arguments[0];
                                                            register long argument1_register    asm("r1") = arguments[1];
                                                            register long argument2_register    asm("r2") = arguments[2];
                                                            register long argument3_register    asm("r3") = arguments[3];
                                                            register long argument4_register    asm("r4") = arguments[4];
                                                            register long argument5_register    asm("r5") = arguments[5];

                                                            __asm__ volatile (
                                                                "svc 0"
                                                                : "+r" (argument0_register),
                                                                  "+r" (argument1_register),
                                                                  "+r" (argument2_register),
                                                                  "+r" (argument3_register),
                                                                  "+r" (argument4_register),
                                                                  "+r" (argument5_register),
                                                                  "+r" (syscall_number_register)
                                                                :
                                                                : "cc", "memory"
                                                            );

                                                            return (long)argument0_register;



    #elif defined(ARCH_MIPS_N64)

    register long syscall_number_register asm("$2") = syscall_number; /* v0 */
    register long argument0_register    asm("$4") = arguments[0];    /* a0 */
    register long argument1_register    asm("$5") = arguments[1];    /* a1 */
    register long argument2_register    asm("$6") = arguments[2];    /* a2 */
    register long argument3_register    asm("$7") = arguments[3];    /* a3 */
    register long argument4_register    asm("$8") = arguments[4];    /* a4 */
    register long argument5_register    asm("$9") = arguments[5];    /* a5 */

    __asm__ volatile (
        "syscall"
        : "+r" (syscall_number_register),
          "+r" (argument0_register),
          "+r" (argument1_register),
          "+r" (argument2_register),
          "+r" (argument3_register),
          "+r" (argument4_register),
          "+r" (argument5_register)
        :
        : "cc", "memory"
    );

    if (!argument3_register)
    {
        return syscall_number_register;
    }
    return -((long)syscall_number_register)



                                                            #elif defined(ARCH_MIPS_N32)

                                                            register long syscall_number_register asm("$2") = syscall_number; /* v0 */
                                                            register long argument0_register    asm("$4") = arguments[0];    /* a0 */
                                                            register long argument1_register    asm("$5") = arguments[1];    /* a1 */
                                                            register long argument2_register    asm("$6") = arguments[2];    /* a2 */
                                                            register long argument3_register    asm("$7") = arguments[3];    /* a3 */
                                                            register long argument4_register    asm("$8") = arguments[4];    /* a4 */
                                                            register long argument5_register    asm("$9") = arguments[5];    /* a5 */

                                                            __asm__ volatile (
                                                                "syscall"
                                                                : "+r" (syscall_number_register),
                                                                              "+r" (argument0_register),
                                                                              "+r" (argument1_register),
                                                                              "+r" (argument2_register),
                                                                              "+r" (argument3_register),
                                                                              "+r" (argument4_register),
                                                                              "+r" (argument5_register)
                                                                              :
                                                                              : "cc", "memory"
                                                            );

                                                            return (long)syscall_number_register;

                                                            if (!argument3_register)
                                                            {
                                                                return syscall_number_register;
                                                            }
                                                            return -((long)syscall_number_register)



    #elif defined(ARCH_MIPS_O32)

    register long v0 asm("$2") = syscall_number;
    register long a0 asm("$4") = arguments[0];
    register long a1 asm("$5") = arguments[1];
    register long a2 asm("$6") = arguments[2];
    register long a3 asm("$7") = arguments[3];

    __asm__ volatile (
        ".set push\n\t"
        ".set noreorder\n\t"
        "addiu $sp, $sp, -16\n\t"
        "sw    %5, 16($sp)\n\t"
        "sw    %6, 20($sp)\n\t"
        "syscall\n\t"
        "addiu $sp, $sp, 16\n\t"
        ".set pop\n\t"
        : "+r" (v0),
          "+r" (a0),
          "+r" (a1),
          "+r" (a2),
          "+r" (a3)
        : "r" (arguments[4]),
          "r" (arguments[5])
        : "memory"
    );

    if (!a3)
    {
        return v0;
    }
    return -((long)v0)





    #else

    # error "Unsupported architecture"

    #endif
}
