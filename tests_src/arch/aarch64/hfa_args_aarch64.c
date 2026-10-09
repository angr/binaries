// aarch64-unknown-linux-gnu-gcc -O1 -shared -fPIC -nostdlib -Wl,--build-id=none
//     -Wl,-z,max-page-size=4096 -o hfa_args_aarch64.so hfa_args_aarch64.c
//
// AArch64 functions whose arguments are homogeneous floating-point aggregates. AAPCS64 gives
// each member of one its own SIMD and floating-point register, so Pair arrives in d0 and d1 and
// Triple in s0, s1 and s2. spill_pair leaves a single register free before its Pair, which is one
// too few: the aggregate goes to the stack and so does the double after it, rather than filling
// the register the aggregate skipped.
struct Pair {
    double a, b;
};

struct Triple {
    float x, y, z;
};

double take_pair(struct Pair s) {
    return s.a + s.b;
}

float take_triple(struct Triple s) {
    return s.x + s.y + s.z;
}

double take_mixed(long n, struct Triple s, double d) {
    return n + s.x + s.y + s.z + d;
}

double spill_pair(double a, double b, double c, double d, double e, double f, double g, struct Pair s, double h) {
    return a + b + c + d + e + f + g + s.a + s.b + h;
}
