// Test binary for global variable type inference in the angr decompiler.
//
// It exercises global variables of primitive types (int, char, long, pointer)
// and of a struct type. At least one function touches every field of the
// struct so that per-function type inference can recover the full layout; the
// remaining functions exercise the per-field / accumulated inference paths.

#include <stdlib.h>

struct point {
    int x;
    int y;
    long z;
};

int g_int;
char g_char;
long g_long;
int *g_ptr;
struct point g_pt;
int g_arr[10];

int get_int(void) {
    return g_int;
}

void set_int(int v) {
    g_int = v;
}

void bump_char(void) {
    g_char = g_char + 1;
}

void set_long(long v) {
    g_long = v * 2;
}

// Touches every field of the struct in a single function.
long sum_pt(void) {
    return (long)g_pt.x + (long)g_pt.y + g_pt.z;
}

void init_pt(int a, int b, long c) {
    g_pt.x = a;
    g_pt.y = b;
    g_pt.z = c;
}

int arr_at(int i) {
    return g_arr[i];
}

void alloc_ptr(void) {
    g_ptr = malloc(sizeof(int) * 10);
}

void use_ptr(int i, int v) {
    g_ptr[i] = v;
}

int main(int argc, char **argv) {
    (void)argv;
    set_int(argc);
    bump_char();
    set_long((long)argc);
    init_pt(argc, argc + 1, (long)argc + 2);
    alloc_ptr();
    use_ptr(0, argc);
    return get_int() + (int)sum_pt() + arr_at(argc) + g_char + (int)g_long + g_ptr[0];
}
