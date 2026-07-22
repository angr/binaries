// Test binary for angr's KnownPatternFinder, round two: inlined
// std::string::capacity()/back()/front()/c_str(), std::vector<T>::back()/data(),
// std::vector<T>::size() for non-power-of-two sizeof(T) (the magic-multiply
// exact-division idiom), and unique_ptr/shared_ptr::get().
//
// The accessors must receive the containers by reference (so they appear as
// pointer vvars in AIL, not decomposed stack objects) and must not themselves
// be inlined into main.
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1) 12.2.0,
// libstdc++ new ABI (string: _M_p at +0, _M_string_length at +8, the
// { _M_local_buf[16] | _M_allocated_capacity } union at +16; vector: _M_start at
// +0, _M_finish at +8, _M_end_of_storage at +16).
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

// extern "C": C++ mangled names do not encode the return type, and angr builds
// prototypes from demangled symbols; a C++-mangled accessor would be typed as
// returning void and its body dead-code-eliminated by the decompiler.

// --- std::string ------------------------------------------------------------

// The SSO select `_M_p == &_M_local_buf ? 15 : _M_allocated_capacity` is a
// control-flow diamond. The printf is what makes the two arms merge into one
// block: with a cheap tail, gcc -O2 duplicates the tail into both arms instead
// (see str_capacity_dup), leaving no join and nothing to outline.
extern "C" __attribute__((noinline)) size_t str_capacity(const std::string &s)
{
    size_t c = s.capacity();
    printf("%zu\n", c);
    return c;
}

// The tail-duplicated shape: two independent returns, no join block, hence no
// single-exit region and no ternary. Kept to pin that we do not match it.
extern "C" __attribute__((noinline)) size_t str_capacity_dup(const std::string &s)
{
    return s.capacity();
}

extern "C" __attribute__((noinline)) char str_back(const std::string &s)
{
    return s.back();
}

extern "C" __attribute__((noinline)) char str_front(const std::string &s)
{
    return s.front();
}

// c_str()/data() are a bare `Load(s + 0)` on libstdc++ (no SSO branch, unlike
// MSVC); kept only to document that they are indistinguishable from any other
// pointer-field dereference and are therefore not matched.
extern "C" __attribute__((noinline)) const char *str_cstr(const std::string &s)
{
    return s.c_str();
}

extern "C" __attribute__((noinline)) const char *str_data(const std::string &s)
{
    return s.data();
}

// --- std::vector ------------------------------------------------------------

extern "C" __attribute__((noinline)) short vec_back_s(const std::vector<short> &v)
{
    return v.back();
}

extern "C" __attribute__((noinline)) int vec_back(const std::vector<int> &v)
{
    return v.back();
}

extern "C" __attribute__((noinline)) long long vec_back_ll(const std::vector<long long> &v)
{
    return v.back();
}

// data()/begin(): also a bare `Load(v + 0)`. Not matched, same reason.
extern "C" __attribute__((noinline)) const int *vec_data(const std::vector<int> &v)
{
    return v.data();
}

extern "C" __attribute__((noinline)) const int *vec_begin(const std::vector<int> &v)
{
    return &*v.begin();
}

// --- std::vector<T>::size() for non-power-of-two sizeof(T) -------------------
// (_M_finish - _M_start) / sizeof(T) is an exact division, compiled as a right
// shift by ctz(sizeof(T)) followed by a multiply by the modular inverse of the
// odd part of sizeof(T): 6 -> (1, inv 3), 12 -> (2, inv 3), 20 -> (2, inv 5),
// 24 -> (3, inv 3), 40 -> (3, inv 5), 48 -> (4, inv 3).

struct S6
{
    short a[3];
};
struct S12
{
    int a, b, c;
};
struct S20
{
    int a[5];
};
struct S24
{
    long long a, b, c;
};
struct S40
{
    long long a[5];
};
struct S48
{
    long long a[6];
};

extern "C" __attribute__((noinline)) size_t vec_size_s6(const std::vector<S6> &v)
{
    return v.size();
}

extern "C" __attribute__((noinline)) size_t vec_size_s12(const std::vector<S12> &v)
{
    return v.size();
}

extern "C" __attribute__((noinline)) size_t vec_size_s20(const std::vector<S20> &v)
{
    return v.size();
}

extern "C" __attribute__((noinline)) size_t vec_size_s24(const std::vector<S24> &v)
{
    return v.size();
}

extern "C" __attribute__((noinline)) size_t vec_size_s40(const std::vector<S40> &v)
{
    return v.size();
}

extern "C" __attribute__((noinline)) size_t vec_size_s48(const std::vector<S48> &v)
{
    return v.size();
}

// --- smart pointers ---------------------------------------------------------
// Both get()s are a bare `Load(p + 0)`, byte-identical to str_cstr/vec_data.

extern "C" __attribute__((noinline)) int *uptr_get(const std::unique_ptr<int> &p)
{
    return p.get();
}

extern "C" __attribute__((noinline)) int *sptr_get(const std::shared_ptr<int> &p)
{
    return p.get();
}

int main(int argc, char **argv)
{
    std::string s(argv[0]);
    std::vector<short> vs(argc, 7);
    std::vector<int> v(argc, 7);
    std::vector<long long> vll(argc, 7);
    std::vector<S6> v6(argc);
    std::vector<S12> v12(argc);
    std::vector<S20> v20(argc);
    std::vector<S24> v24(argc);
    std::vector<S40> v40(argc);
    std::vector<S48> v48(argc);
    std::unique_ptr<int> up(new int(3));
    std::shared_ptr<int> sp(new int(4));
    printf("%zu %zu %d %d %s %s %d %d %lld %p %p\n", str_capacity(s), str_capacity_dup(s), str_back(s), str_front(s),
           str_cstr(s), str_data(s), vec_back_s(vs), vec_back(v), vec_back_ll(vll), (const void *)vec_data(v),
           (const void *)vec_begin(v));
    printf("%zu %zu %zu %zu %zu %zu %p %p\n", vec_size_s6(v6), vec_size_s12(v12), vec_size_s20(v20), vec_size_s24(v24),
           vec_size_s40(v40), vec_size_s48(v48), (void *)uptr_get(up), (void *)sptr_get(sp));
    return 0;
}
