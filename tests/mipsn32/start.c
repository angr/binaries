/* Freestanding entry point so the fixture can link -static -nostdlib. */
extern int main(void);
void _start(void)
{
    int r = main();
    /* n32 exit syscall: __NR_exit = 6000 + 1 */
    register long a0 __asm__("$4") = r;
    register long v0 __asm__("$2") = 6001;
    __asm__ volatile("syscall" : : "r"(a0), "r"(v0) : "memory");
    __builtin_unreachable();
}
