/* Big-endian ARM pair: be8_loop is BE8, be32_loop is BE32.
 *
 * One object file, linked twice; only the linker flag differs, so the two
 * binaries hold the same instructions in opposite byte order.
 *
 *   clang --target=armv7eb-unknown-linux-gnueabihf -march=armv7-a \
 *       -mfloat-abi=hard -mfpu=vfpv3-d16 -ffreestanding -nostdlib -fno-pic -O1 \
 *       -c be8_loop.c -o be8_loop.o
 *   ld.lld -o be8_loop  be8_loop.o -e _start --be8
 *   ld.lld -o be32_loop be8_loop.o -e _start
 *
 * Built with clang and lld 22.1.8. e_flags is 0x05800400 for be8_loop
 * (EABI5 | EF_ARM_BE8 | hard-float) and 0x05000400 for be32_loop.
 *
 * sha256 01b5be1529f49866e0eb93dd958e18f058f6376767910ca1cee5c2f0fb1c570a be8_loop
 * sha256 da3cee80dc618f2ae11a22e10a6bd1b10af996af2d8e194916319bf184a43e2b be32_loop
 */

volatile int counter;

static int add(int a, int b) { return a + b; }

int compute(int n) {
    int total = 0;
    for (int i = 0; i < n; i++) total = add(total, i);
    return total;
}

void _start(void) {
    counter = compute(10);
    for (;;) {}
}
