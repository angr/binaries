/*
 * Fixed input for angr's C right-shift signedness regression.
 *
 * Build from the repository root with GCC 13.3.0 (x86_64-linux-gnu):
 * gcc -O0 -fno-inline -fno-pie -no-pie -fno-stack-protector -nostdlib \
 *     -Wl,--build-id=none,-e,logical64 \
 *     -o tests/x86_64/decompiler/right_shift_signed_calls \
 *     tests_src/decompiler/right_shift_signed_calls.c
 * gcc -m32 -O0 -fno-inline -fno-pie -no-pie -fno-stack-protector -nostdlib \
 *     -Wl,--build-id=none,-e,logical32 -o tests/i386/right_shift_signed_calls \
 *     tests_src/decompiler/right_shift_signed_calls.c
 *
 * These are analysis fixtures, not native executables to run. Tests consume
 * the committed ELFs without compiling or executing them.
 */
volatile int result32;
volatile long long result64;

int signed_status32(void)
{
    return result32;
}

long long signed_status64(void)
{
    return result64;
}

unsigned int logical32(void)
{
    return (unsigned int)signed_status32() >> 31;
}

unsigned long long logical64(void)
{
    return (unsigned long long)signed_status64() >> 63;
}
