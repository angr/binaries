// Test binary for angr's KnownPatternFinder: the inlined std::string
// destructor, plus the two length-setting internals.
//
// ~basic_string() is one line of source and, at -O2, a five-line triangle the
// compiler emits at every scope exit of every local string and in every
// destructor of a class holding one -- by a wide margin the most common inlined
// idiom in C++ binaries (5.9M DWARF inline records across 245 binaries of the
// -O2 benchmark corpus). libstdc++ frees the buffer only when the string is not
// using its small-string buffer:
//
//     if (_M_p != &_M_local_buf)
//         operator delete(_M_p, _M_allocated_capacity + 1);
//
// The three shapes below are the three ways that reaches the matcher, and each
// takes a different path through the outliner:
//
//   * a *member* string -- Load(this + K) against this + K + 16, matched with
//     PField, outlined into a real callee;
//   * a *local* string -- one virtual variable per stack slot by the time
//     patterns run, matched with PStackField; its region has no interface for a
//     callee to take, so it is collapsed into the call in place;
//   * a string reached *by pointer*. When the two arms rejoin, the region has
//     one exit and outlines normally; when they both end the function, the
//     compiler duplicates the epilogue and the region has two exits, which v1
//     outlining declines -- so it falls back to the in-place collapse.
//
// extern "C" throughout, so the decompiler test can look these up by name;
// One::~One is deliberately left mangled since a destructor cannot be extern "C".
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1)
// 12.2.0, libstdc++ new ABI (string: _M_p at +0, _M_string_length at +8, the
// { _M_local_buf[16] | _M_allocated_capacity } union at +16).
#include <string>
#include <vector>

struct Doc {
    std::string name;
    std::string body;
    int n;
};

// two member destructors, chained: the join of the first triangle is the entry
// of the second
extern "C" __attribute__((noinline)) void doc_free(Doc *d) { delete d; }

// a local string: stack slots, not an object
extern "C" __attribute__((noinline)) int local_len(const char *p) {
    std::string s(p);
    return (int)s.size();
}

// two locals destroyed at one scope exit
extern "C" __attribute__((noinline)) int two_locals(const char *a, const char *b) {
    std::string x(a);
    std::string y(b);
    return (int)(x.size() + y.size());
}

// a string reached through a bare pointer, with work after it so the arms
// rejoin: one region exit
extern "C" __attribute__((noinline)) void ptr_free(std::string *s, int *o) {
    s->~basic_string();
    *o += 1;
}

// the same by pointer, but every arm ends the function: gcc duplicates the
// `return 1` epilogue, so the region has two exits at two different addresses
extern "C" __attribute__((noinline)) int ptr_free_dup(std::string *s, int k) {
    if (k) {
        s->~basic_string();
        return 1;
    }
    return 0;
}

// _M_set_length: every mutating operation ends with "store the new length,
// re-terminate the buffer". erase(n) is the shape with a general n, so the same
// value appears as the stored length and as the offset added to _M_p.
extern "C" __attribute__((noinline)) void str_shrink(std::string *s, unsigned long n) { s->erase(n); }

// clear() is _M_set_length(0): no offset, so no second occurrence of n to
// unify on, which is why it is a separate (and opt-in) pattern
extern "C" __attribute__((noinline)) void str_clear(std::string *s) { s->clear(); }

// a vector of strings: the element destructor runs in a loop
extern "C" __attribute__((noinline)) void vec_free(std::vector<std::string> *v) { v->clear(); }

int main() { return 0; }
