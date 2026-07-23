/* Callback registered at run time through a function argument, stored into a global struct field by the
 * registration function, and finally invoked from a third function -- the pattern used by driver/ISR callback
 * tables in embedded firmware. Resolving the indirect call requires threading the code pointer from main()
 * through register_cb()'s parameter into g_ops.cb, and back out at the call in dispatch(). */
#include <stdio.h>

typedef void (*cb_t)(int);

__attribute__((noinline)) void h1(int x) { printf("h1 %d\n", x + 10); }
__attribute__((noinline)) void h2(int x) { printf("h2 %d\n", x * 3); }

struct ops {
    long id;
    cb_t cb;
};

static struct ops g_ops; /* lives in .bss: written only at run time */

__attribute__((noinline)) void register_cb(cb_t f) {
    g_ops.id = 1;
    g_ops.cb = f;
}

__attribute__((noinline)) void dispatch(int x) { g_ops.cb(x); }

int main(int argc, char **argv) {
    (void)argv;
    register_cb((argc & 1) ? h1 : h2);
    dispatch(argc);
    return 0;
}
