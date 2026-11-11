/*
 * Functions whose prologs produce a mix of packed and .xdata exception-table (.pdata) entries on
 * ARM64 and ARMNT. Built by pdata_arm.build.sh.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOINLINE __attribute__((noinline))

volatile int sink;

/* leaf without a frame: no .pdata entry */
NOINLINE int leaf(int x) { return x * 3 + 1; }

/* small frame saving lr: usually packed */
NOINLINE int small_frame(int x) {
    sink = leaf(x);
    return sink + 1;
}

/* large stack frame: usually .xdata */
NOINLINE int large_frame(int x) {
    volatile char buf[70000];
    buf[x & 0xffff] = (char)x;
    buf[(x * 7) & 0xffff] = (char)(x + 1);
    return buf[x & 0xffff] + buf[(x * 7) & 0xffff];
}

/* many callee-saved registers live across calls */
NOINLINE long many_saved(long a, long b, long c, long d, long e, long f, long g, long h) {
    long r1 = a * b, r2 = c * d, r3 = e * f, r4 = g * h, r5 = a + h, r6 = b + g, r7 = c + f, r8 = d + e;
    sink = small_frame((int)r1);
    sink = small_frame((int)r2);
    return r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + sink;
}

/* floating-point callee-saved registers */
NOINLINE double float_saved(double a, double b, double c, double d) {
    double x = a * b, y = c * d, z = a + d, w = b + c;
    sink = small_frame((int)x);
    return x + y + z + w + sink;
}

/* dynamic stack allocation needs a frame pointer */
NOINLINE int dynamic_alloca(int n) {
    char *p = __builtin_alloca((size_t)n + 16);
    memset(p, n, (size_t)n + 16);
    sink = p[n / 2];
    return p[n];
}

/* variadic: homes the register arguments */
NOINLINE int variadic_sum(int count, ...) {
    va_list ap;
    int total = 0;
    va_start(ap, count);
    for (int i = 0; i < count; i++)
        total += va_arg(ap, int);
    va_end(ap);
    return total;
}

/* several early returns: multiple epilogs */
NOINLINE int multi_epilog(int x) {
    if (x == 1) {
        sink = small_frame(x);
        return 10;
    }
    if (x == 2) {
        sink = large_frame(x);
        return 20;
    }
    if (x == 3) {
        sink = (int)many_saved(x, x, x, x, x, x, x, x);
        return 30;
    }
    sink = leaf(x);
    return sink;
}

int main(int argc, char **argv) {
    int x = argc + (argv[0] != NULL);
    int r = small_frame(x) + large_frame(x) + (int)many_saved(x, 2, 3, 4, 5, 6, 7, 8) +
            (int)float_saved(x, 1.5, 2.5, 3.5) + dynamic_alloca(x * 8) + variadic_sum(3, x, 2, 3) +
            multi_epilog(x);
    printf("%d\n", r);
    return 0;
}
