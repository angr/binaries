// Test binary for angr's KnownPatternFinder, round three: an inlined
// std::vector<T>::size() whose intermediate values are still needed by the
// surrounding code.
//
// The KnownPattern matcher reaches the vector's fields by chasing the anchor's
// virtual-variable operands back to their definitions in the same block. Those
// definitions can only be *moved* into the outlined callee when nothing else
// uses them -- and in optimized code they usually are used elsewhere, which is
// exactly why the compiler kept them in registers. Here `(_M_finish -
// _M_start)` (and, in the second function, the `_M_finish` load feeding it) is
// live past the size computation, so the chased definitions have to be copied
// into the callee rather than moved.
//
// `sizeof(S12)` is not a power of two, so `size()` is the exact-division idiom
// (a shift by ctz(12) followed by a multiply by the modular inverse of 3) and
// the byte difference really is a separate chased definition rather than a
// sub-expression of the anchor.
//
// The `n & 1` guard is what keeps the division alive and in the entry block: a
// plain `n > k` lets gcc rewrite the test as a comparison against the raw byte
// difference and drop the division entirely.
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1) 12.2.0,
// libstdc++ new ABI (vector: _M_start at +0, _M_finish at +8).
#include <cstddef>
#include <cstdio>
#include <vector>

struct S12
{
    int a, b, c;
};

// One chased definition: the byte difference feeds both size() and the store in
// the tail block.
extern "C" __attribute__((noinline)) size_t vec_size_shared_diff(const std::vector<S12> &v, size_t *bytes)
{
    size_t n = v.size();
    if (n & 1)
        *bytes = (size_t)((const char *)&v[n] - (const char *)&v[0]);
    return n;
}

// Two chained chased definitions: the _M_finish load and the byte difference
// computed from it are both live past the size computation.
extern "C" __attribute__((noinline)) size_t vec_size_shared_chain(const std::vector<S12> &v, size_t *bytes,
                                                                 const S12 **endp)
{
    size_t n = v.size();
    if (n & 1)
    {
        *bytes = (size_t)((const char *)&v[n] - (const char *)&v[0]);
        *endp = &v[n];
    }
    return n;
}

int main(int argc, char **argv)
{
    std::vector<S12> v(argc);
    size_t bytes = 0;
    const S12 *end = nullptr;
    printf("%zu %zu %zu %p\n", vec_size_shared_diff(v, &bytes), vec_size_shared_chain(v, &bytes, &end), bytes,
           (const void *)end);
    return 0;
}
