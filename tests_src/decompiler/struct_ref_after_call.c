// Test binary for stack struct objects whose first member is accessed around a call in angr's decompiler.
//
// Built with:
//   gcc -O2 -fno-optimize-sibling-calls -fno-pie -no-pie -o struct_ref_after_call struct_ref_after_call.c
//
// Every function is noipa so that -O2 keeps the call tree and the argument shapes (no inlining, cloning, or IPA-SRA);
// sibling calls are disabled so that top() calls the other functions instead of jumping to them.
//
// Each function passes the address of a stack struct to callee(). The 8-byte value at the struct's offset 0 must not
// be typed as the whole struct: reread() reads it back after the call, and loop() writes it before every call.

#define API __attribute__((noipa))

struct S {
    long a, b, c, d;
};

API long callee(struct S *s) {
    s->a += s->b + s->c + s->d;
    return s->d;
}

API long reread(long x) {
    struct S s = {x, 0, 1, 2};
    callee(&s);
    return s.a;
}

API long loop(long x, long n) {
    struct S s = {0, 0, 1, 2};
    long r = 0;
    for (long i = 0; i < n; i++) {
        s.a = x + i;
        r += callee(&s);
    }
    return r;
}

API long top(long y) { return reread(y + 1) + loop(y + 2, y); }

int main(int argc, char **argv) {
    (void)argv;
    return (int)top(argc);
}
