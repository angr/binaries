/* Test source for FullProgramIndirectJumpResolution: same-function
 * conditional function pointer stored in a local struct.
 * Compile: gcc -O2 -no-pie -g -o fpijr_local_cond fpijr_local_cond.c
 */

__attribute__((noinline)) int h1(int x) { return x + 10; }
__attribute__((noinline)) int h2(int x) { return x * 3; }

__attribute__((noinline)) int dispatch(int c, int x)
{
    struct {
        int (*f)(int);
    } s;
    s.f = c ? h1 : h2;
    return s.f(x);
}

int main(int argc, char **argv)
{
    (void)argv;
    return dispatch(argc & 1, argc);
}
