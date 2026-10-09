/* Freestanding entry point: no libc, exits immediately.  The fixture exists to
 * be disassembled, but it is a real, runnable static AArch64 Linux ELF. */

#include "fixture.h"

__attribute__((used)) unsigned char fixture_scratch[64] __attribute__((aligned(16)));

__attribute__((noreturn)) void _start(void)
{
    register long x8 __asm__("x8") = 94;   /* __NR_exit_group */
    register long x0 __asm__("x0") = 0;
    __asm__ __volatile__("svc #0" :: "r"(x8), "r"(x0) : "memory");
    __builtin_unreachable();
}
