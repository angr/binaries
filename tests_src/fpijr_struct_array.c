/* Test source for FullProgramIndirectJumpResolution: global array of
 * structs with an interior function pointer (stride 16, fn at offset 8).
 * Compile: gcc -O2 -no-pie -g -o fpijr_struct_array fpijr_struct_array.c
 */

__attribute__((noinline)) int e0(int x) { return x + 10; }
__attribute__((noinline)) int e1(int x) { return x * 3; }
__attribute__((noinline)) int e2(int x) { return x ^ 7; }

struct ent {
    long tag;
    int (*fn)(int);
};

struct ent entries[3] = { { 0, e0 }, { 1, e1 }, { 2, e2 } };

__attribute__((noinline)) int dispatch(int i, int x)
{
    return entries[i % 3].fn(x);
}

int main(int argc, char **argv)
{
    (void)argv;
    return dispatch(argc, argc + 1);
}
