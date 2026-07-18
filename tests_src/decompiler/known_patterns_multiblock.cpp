// Test binary for angr's KnownPatternFinder statement-sequence and multi-block
// pattern matching.
//
// do_swap: the inlined std::swap idiom is a three-statement sequence
// (t = *a; *a = *b; *b = t) within one block. It returns 0 (instead of void)
// so the temporary does not leak into the caller-visible return register and
// the swap region has no live-out value.
// route: a diamond (if/else with a store to a DIFFERENT pointer in each
// branch) — the distinct addresses keep gcc from folding it into a cmov, so
// it stays a genuine single-entry / single-exit multi-block region.
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1) 12.2.0
#include <cstdio>
#include <utility>

extern "C" __attribute__((noinline)) long do_swap(long *a, long *b)
{
    std::swap(*a, *b);
    return 0;
}

extern "C" __attribute__((noinline)) void route(long cond, long *a, long *b, long *c, long v)
{
    if (cond) {
        *a = v;
    } else {
        *b = v;
    }
    // a merge point after the diamond, so the if/else region has a single exit
    *c = v;
}

int main(int argc, char **argv)
{
    long x = argc;
    // operator new pulls in libstdc++ so the binary carries C++ evidence
    // (std::swap alone is header-only and would leave a libc-only binary)
    long *y = new long((long)(argv - (char **)0));
    do_swap(&x, y);
    long p = 0, q = 0, r = 0;
    route(argc, &p, &q, &r, x);
    printf("%ld %ld %ld %ld %ld\n", x, *y, p, q, r);
    delete y;
    return 0;
}
