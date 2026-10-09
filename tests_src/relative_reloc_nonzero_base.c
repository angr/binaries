/* A shared object deliberately linked at a non-zero text segment address.
 *
 * Each table entry needs an R_X86_64_RELATIVE relocation whose addend is a
 * link-time virtual address well above zero, which is what makes the
 * difference between "load bias + addend" and "mapped base + addend" visible.
 *
 * gcc -shared -fPIC -nostdlib -Wl,-Ttext-segment=0x400000 -Wl,--build-id=none \
 *     -o relative_reloc_nonzero_base.so relative_reloc_nonzero_base.c
 */
__attribute__((noinline)) static int alpha(void) { return 1; }
__attribute__((noinline)) static int beta(void) { return 2; }
__attribute__((noinline)) static int delta(void) { return 3; }

void *const table[] = {(void *)alpha, (void *)beta, (void *)delta};
