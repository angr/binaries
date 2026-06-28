// Test binary for progressive cross-function argument struct type inference in the angr decompiler.
//
// A pointer to `struct A` is passed to func_a, func_b, and func_c. Each callee only touches a subset of the struct's
// fields, so on its own each recovers only a partial layout. When `caller` is decompiled, the partial layouts observed
// at the three call sites (which all receive the same pointer) should be unioned into the complete struct, and that
// combined struct should be propagated back to the callees.

#include <stdlib.h>

struct A {
    int x;   // offset 0
    int y;   // offset 4
    long z;  // offset 8
    int w;   // offset 16
};

void func_a(struct A *p) {
    p->x = 5;
}

int func_b(struct A *p) {
    return p->y;
}

long func_c(struct A *p) {
    return p->z + p->w;
}

long caller(void) {
    struct A *p = malloc(sizeof(struct A));
    func_a(p);
    int b = func_b(p);
    long c = func_c(p);
    return b + c;
}

int main(void) {
    return (int)caller();
}
