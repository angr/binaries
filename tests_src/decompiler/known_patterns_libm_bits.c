// Test binary for angr's KnownPatternFinder: libm floating-point bit-twiddling
// idioms and the integer byte-swap classics. Every idiom compiles to raw
// integer bit operations on the float/double representation (or to SSE
// andps/andpd/xorpd against a sign mask held in .rodata), which is exactly the
// unreadable residue KnownPatterns exist to fix.
//
// Built with -fno-tree-vectorize so the scalar shape survives.
//
// Build: gcc -O2 -fno-tree-vectorize -o known_patterns_libm_bits \
//            known_patterns_libm_bits.c -lm
#include <math.h>
#include <stdint.h>
#include <stdio.h>

/* ---------------- floating-point sign-bit idioms ---------------- */

// fabs(x): clears the sign bit -> bits & 0x7fffffffffffffff (andpd)
__attribute__((noinline)) double f_fabs(double x)
{
    return fabs(x);
}

// fabsf(x): bits & 0x7fffffff (andps)
__attribute__((noinline)) float f_fabsf(float x)
{
    return fabsf(x);
}

// -x: flips the sign bit -> bits ^ 0x8000000000000000 (xorpd)
__attribute__((noinline)) double f_neg(double x)
{
    return -x;
}

// -x for float: bits ^ 0x80000000 (xorps)
__attribute__((noinline)) float f_negf(float x)
{
    return -x;
}

// copysign(x, y): (bits(x) & 0x7fff...) | (bits(y) & 0x8000...)
__attribute__((noinline)) double f_copysign(double x, double y)
{
    return copysign(x, y);
}

__attribute__((noinline)) float f_copysignf(float x, float y)
{
    return copysignf(x, y);
}

// signbit(x): sign test
__attribute__((noinline)) int f_signbit(double x)
{
    return signbit(x);
}

__attribute__((noinline)) int f_signbitf(float x)
{
    return signbit((float)x);
}

/* ---------------- floating-point classification idioms ---------------- */

// isnan(x): x != x (unordered compare)
__attribute__((noinline)) int f_isnan(double x)
{
    return isnan(x);
}

__attribute__((noinline)) int f_isnanf(float x)
{
    return isnan((float)x);
}

// isinf(x): (bits & 0x7fffffffffffffff) == 0x7ff0000000000000
__attribute__((noinline)) int f_isinf(double x)
{
    return isinf(x);
}

// isfinite(x): (bits & 0x7ff0000000000000) != 0x7ff0000000000000
__attribute__((noinline)) int f_isfinite(double x)
{
    return isfinite(x);
}

// The hand-rolled bit-twiddle spelling of isinf, so we see the pure-integer
// shape even if the macro turns into an FP compare.
__attribute__((noinline)) int f_isinf_bits(double x)
{
    uint64_t u;
    __builtin_memcpy(&u, &x, sizeof u);
    return (u & 0x7fffffffffffffffULL) == 0x7ff0000000000000ULL;
}

// Hand-rolled isnan on the raw bits.
__attribute__((noinline)) int f_isnan_bits(double x)
{
    uint64_t u;
    __builtin_memcpy(&u, &x, sizeof u);
    return (u & 0x7fffffffffffffffULL) > 0x7ff0000000000000ULL;
}

// Hand-rolled signbit on the raw bits.
__attribute__((noinline)) int f_signbit_bits(double x)
{
    uint64_t u;
    __builtin_memcpy(&u, &x, sizeof u);
    return (int)(u >> 63);
}

// Hand-rolled fabs on the raw bits (the "portable" spelling).
__attribute__((noinline)) double f_fabs_bits(double x)
{
    uint64_t u;
    __builtin_memcpy(&u, &x, sizeof u);
    u &= 0x7fffffffffffffffULL;
    double r;
    __builtin_memcpy(&r, &u, sizeof r);
    return r;
}

/* ---------------- the same idioms used *in context* ---------------- */

// fabs() as a sub-expression of a comparison -- the common real-world spelling.
__attribute__((noinline)) int f_near(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

// negation feeding an arithmetic expression.
__attribute__((noinline)) double f_negmul(double a, double b)
{
    return (-a) * b + 1.0;
}

// copysign used in context.
__attribute__((noinline)) double f_copysign_ctx(double a, double b)
{
    return copysign(a * 2.0, b) + 1.0;
}

// isnan guard in context.
__attribute__((noinline)) double f_guard(double a, double b)
{
    if (isnan(a) || isnan(b))
        return 0.0;
    return a + b;
}

/* ---------------- integer byte-swap classics ---------------- */

__attribute__((noinline)) uint16_t i_bswap16(uint16_t x)
{
    return __builtin_bswap16(x);
}

__attribute__((noinline)) uint32_t i_bswap32(uint32_t x)
{
    return __builtin_bswap32(x);
}

__attribute__((noinline)) uint64_t i_bswap64(uint64_t x)
{
    return __builtin_bswap64(x);
}

// The hand-rolled shift/mask spellings, which is what code that predates the
// builtins looks like.
__attribute__((noinline)) uint16_t i_bswap16_manual(uint16_t x)
{
    return (uint16_t)(((x & 0x00ffu) << 8) | ((x & 0xff00u) >> 8));
}

__attribute__((noinline)) uint32_t i_bswap32_manual(uint32_t x)
{
    return ((x & 0x000000ffu) << 24) | ((x & 0x0000ff00u) << 8) | ((x & 0x00ff0000u) >> 8)
           | ((x & 0xff000000u) >> 24);
}

__attribute__((noinline)) uint64_t i_bswap64_manual(uint64_t x)
{
    return ((x & 0x00000000000000ffULL) << 56) | ((x & 0x000000000000ff00ULL) << 40)
           | ((x & 0x0000000000ff0000ULL) << 24) | ((x & 0x00000000ff000000ULL) << 8)
           | ((x & 0x000000ff00000000ULL) >> 8) | ((x & 0x0000ff0000000000ULL) >> 24)
           | ((x & 0x00ff000000000000ULL) >> 40) | ((x & 0xff00000000000000ULL) >> 56);
}

int main(int argc, char **argv)
{
    double d = (double)argc;
    float f = (float)argc;
    printf("%f %f %f %f %f %f\n", f_fabs(d), (double)f_fabsf(f), f_neg(d), (double)f_negf(f),
           f_copysign(d, -d), (double)f_copysignf(f, -f));
    printf("%d %d %d %d %d %d\n", f_signbit(d), f_signbitf(f), f_isnan(d), f_isnanf(f), f_isinf(d),
           f_isfinite(d));
    printf("%d %d %d %f\n", f_isinf_bits(d), f_isnan_bits(d), f_signbit_bits(d), f_fabs_bits(d));
    printf("%d %f %f %f\n", f_near(d, d + 1.0, 0.5), f_negmul(d, d), f_copysign_ctx(d, -d),
           f_guard(d, d));
    printf("%x %x %llx\n", i_bswap16((uint16_t)argc), i_bswap32((uint32_t)argc),
           (unsigned long long)i_bswap64((uint64_t)argc));
    printf("%x %x %llx\n", i_bswap16_manual((uint16_t)argc), i_bswap32_manual((uint32_t)argc),
           (unsigned long long)i_bswap64_manual((uint64_t)argc));
    return 0;
}
