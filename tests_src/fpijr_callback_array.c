/* Callbacks registered at run time into a global array of structs, then invoked through an index. This is the shape
 * used by interrupt-handler tables in embedded firmware: the call site sees only isr_ctx[i].cb. */
#include <stdio.h>

typedef void (*cb_t)(int);

__attribute__((noinline)) void h1(int x) { printf("h1 %d\n", x + 10); }
__attribute__((noinline)) void h2(int x) { printf("h2 %d\n", x * 3); }

struct slot {
    long arg;
    cb_t cb;
};

static struct slot g_slots[4]; /* .bss: filled in at run time */

__attribute__((noinline)) void register_cb(int i, cb_t f) {
    g_slots[i & 3].cb = f;
    g_slots[i & 3].arg = i;
}

__attribute__((noinline)) void dispatch(int i, int x) { g_slots[i & 3].cb(x); }

int main(int argc, char **argv) {
    (void)argv;
    register_cb(argc, h1);
    register_cb(argc + 1, h2);
    dispatch(argc, argc);
    return 0;
}
