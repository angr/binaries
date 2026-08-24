/* A minimal Windows program that reaches __fastfail, for angr's Windows ARM
   syscall-convention tests. Freestanding: no CRT, no imports, no Windows SDK. */
__attribute__((noinline)) static int add2(int a, int b) { return a + b; }

int mainCRTStartup(void) {
    int v = add2(2, 3);
    if (v == 5) {
        __builtin_trap();
    }
    return v;
}
