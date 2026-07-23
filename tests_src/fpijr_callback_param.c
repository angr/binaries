/* A function pointer passed as an argument and invoked directly through the parameter inside the callee. Resolving
 * the indirect call requires propagating the code pointer across the call boundary into the callee's parameter. */
#include <stdio.h>

typedef int (*fn_t)(int);

__attribute__((noinline)) int h1(int x) { return x + 10; }
__attribute__((noinline)) int h2(int x) { return x * 3; }

__attribute__((noinline)) int apply(fn_t f, int x) { return f(x) + 1; }

int main(int argc, char **argv) {
    (void)argv;
    printf("%d\n", apply((argc & 1) ? h1 : h2, argc));
    return 0;
}
