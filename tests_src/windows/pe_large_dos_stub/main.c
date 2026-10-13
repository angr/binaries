/* Freestanding PE entry point. The linker supplies the DOS stub and no CRT or
   Windows SDK is needed. */
__declspec(noinline) static int add2(int a, int b) { return a + b; }

int mainCRTStartup(void) { return add2(2, 3); }
