// Test binary for angr's KnownPatternFinder: 3-component float vector math as
// used in glm / DirectXMath and game/graphics code. Built with
// -fno-tree-vectorize so the scalar FP arithmetic shape survives (rather than
// being folded into SSE horizontal ops).
//
// Build: make -f Makefile.known_patterns  (g++ -O2 -fno-tree-vectorize -> ELF)
#include <cstdio>

struct v3 {
    float x, y, z;
};

// dot(a, b) = a.x*b.x + a.y*b.y + a.z*b.z
extern "C" __attribute__((noinline)) float vec_dot3(const v3 *a, const v3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// length_sq(a) = a.x*a.x + a.y*a.y + a.z*a.z  (dot with itself)
extern "C" __attribute__((noinline)) float vec_length_sq(const v3 *a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

int main(int argc, char **argv)
{
    v3 a = {(float)argc, 2.0f, 3.0f};
    v3 b = {4.0f, 5.0f, (float)argc};
    printf("%f %f\n", vec_dot3(&a, &b), vec_length_sq(&a));
    return 0;
}
