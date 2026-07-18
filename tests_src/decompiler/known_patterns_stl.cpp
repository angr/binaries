// Test binary for angr's KnownPatternFinder: inlined std::string::length() and
// std::vector<int>::size() idioms that should be recognized and outlined back
// into calls by the decompiler.
//
// The accessors must receive the containers by reference (so they appear as
// pointer vvars in AIL, not decomposed stack objects) and must not themselves
// be inlined into main.
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1) 12.2.0,
// libstdc++ new ABI (_M_p at +0, _M_string_length at +8; vector _M_start at +0,
// _M_finish at +8).
#include <cstdio>
#include <string>
#include <vector>

// extern "C": C++ mangled names do not encode the return type, and angr builds
// prototypes from demangled symbols; a C++-mangled accessor would be typed as
// returning void and its body dead-code-eliminated by the decompiler.
extern "C" __attribute__((noinline)) size_t get_len(const std::string &s)
{
    return s.length();
}

extern "C" __attribute__((noinline)) size_t get_size(const std::vector<int> &v)
{
    return v.size();
}

int main(int argc, char **argv)
{
    std::string s(argv[0]);
    std::vector<int> v(argc, 7);
    printf("%zu %zu\n", get_len(s), get_size(v));
    return 0;
}
