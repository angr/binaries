/* Fixture source for a Mach-O MH_OBJECT (relocatable object) test input.
 *
 * Freestanding on purpose: compiling to an object needs no SDK and no headers,
 * so this reproduces on any machine with a clang that can target Apple.
 *
 * Build (produces tests/aarch64/macho/relocatable.o and tests/x86_64/macho/relocatable.o):
 *     ./build.sh
 */

typedef unsigned long size_t;

static unsigned long state[25];

void keccak_init(void) {
    for (int i = 0; i < 25; i++) {
        state[i] = 0;
    }
}

unsigned long keccak_absorb(const unsigned char *in, size_t len) {
    unsigned long acc = 0;
    for (size_t i = 0; i < len; i++) {
        acc = (acc << 1) ^ in[i];
        state[i % 25] ^= acc;
    }
    return acc;
}

unsigned long keccak_squeeze(unsigned char *out, size_t len) {
    unsigned long acc = 0;
    for (size_t i = 0; i < len; i++) {
        out[i] = (unsigned char)(state[i % 25] >> (i % 8));
        acc += out[i];
    }
    return acc;
}
