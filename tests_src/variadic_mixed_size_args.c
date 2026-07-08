/*
 * Regression test source for angr's decompiler: mixed-size argument-register
 * definitions merging at a variadic call site.
 *
 * The value passed as the first variadic argument of printf() (rsi on amd64
 * SysV) must receive definitions of different sizes on different paths into
 * the merge block that performs the call:
 *   - full-width defs (e.g., mov esi, 1 / mov rsi, [g_value]) on some paths,
 *   - a 1-byte def (setne sil) on another path.
 * With such mixed-size reaching definitions, SSA phi placement used to pass
 * up creating a phi for rsi (assuming the register was dead), and the
 * variadic argument -- discovered only after SSA construction from the format
 * string -- had multiple reaching definitions with no phi.
 *
 * Compile with: gcc -O2 -o variadic_mixed_size_args variadic_mixed_size_args.c
 */
#include <stdio.h>

unsigned long g_value;

__attribute__((noinline)) void report_mode(unsigned long mode)
{
    unsigned long v;

    if (mode & 1)
        v = 1;
    else if (mode & 2)
        v = g_value;
    else
        v = (mode & 0x49) != 0;

    printf("mode=%lu\n", v);
    g_value = v ^ 1;
}

int main(int argc, char **argv)
{
    (void)argv;
    report_mode((unsigned long)argc);
    return 0;
}
