#include <stdlib.h>

struct A {
    int a1;
    int a2;
    int a3;
};

struct B {
    struct A b1;
    struct A b2;
};

struct C {
    int c1;
    struct B c2[10];
    int c3[10];
    struct C *c4;
};

int main(int argc, char **argv) {
    struct C c_val;
    struct C *c_ptr = malloc(sizeof(struct C));

    c_ptr->c4 = &c_val;
    struct B *b_ptr = &c_ptr->c2[argc];

    c_val.c3[argc] = argc;
    c_ptr->c3[argc] = argc;

    c_val.c2[argc].b2.a2 = argc;
    c_ptr->c2[argc].b2.a2 = argc;

    b_ptr += 1;

    return c_ptr->c4->c2[argc].b2.a2;
}
