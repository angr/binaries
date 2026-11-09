// Test binary for oversized by-value struct arguments in angr decompiler prototypes.
//
// Built with:
//   gcc -O2 -fno-optimize-sibling-calls -fno-pie -no-pie -o struct_ref_first_member struct_ref_first_member.c
//
// Every function is noipa so that -O2 keeps the call tree and the argument shapes (no inlining, cloning, or IPA-SRA);
// sibling calls are disabled so that top() calls caller() instead of jumping to it.
//
// caller() stores its 8-byte argument into the first member of a stack struct and passes the struct's address to
// callee(). Variable recovery types the stack slot at the struct's offset 0 as the whole struct, so caller's argument
// must not become a by-value struct parameter in caller's prototype: top() would then pass the struct member by member.

#define API __attribute__((noipa))

struct S {
    long a, b, c, d;
};

API long callee(struct S *s) { return s->a + s->b + s->c + s->d; }

API long caller(long x) {
    struct S s = {x, 0, 1, 2};
    return callee(&s);
}

API long top(long y) { return caller(y + 1); }

int main(int argc, char **argv) {
    (void)argv;
    return (int)top(argc);
}
