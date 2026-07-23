/* Test source for FullProgramIndirectJumpResolution: flat global
 * function-pointer table dispatched through an indirect tail jump.
 * Compile: gcc -O2 -no-pie -g -o fpijr_global_table fpijr_global_table.c
 */

typedef int (*fp_t)(int);

__attribute__((noinline)) int f0(int x) { return x + 10; }
__attribute__((noinline)) int f1(int x) { return x * 3; }
__attribute__((noinline)) int f2(int x) { return x ^ 7; }
__attribute__((noinline)) int f3(int x) { return x - 42; }

fp_t table[4] = { f0, f1, f2, f3 };

__attribute__((noinline)) int dispatch(int i, int x)
{
    return table[i & 3](x);
}

int main(int argc, char **argv)
{
    (void)argv;
    return dispatch(argc, argc + 1);
}
