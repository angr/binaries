// Test binary for angr's KnownPatternFinder, round four: STL containers that
// are *members of another object* rather than standalone locals.
//
// Every earlier fixture hands the accessor a `const std::vector<T> &`, so the
// container sits exactly at the address held in a register and its fields lift
// to `Load(v)` / `Load(v + 8)`. That is the minority case in real code: measured
// over the dec-patterns-benchmark corpus, only 10-25% of the DWARF sites for a
// vector accessor read the container through a bare pointer -- the rest are
// `this->field.size()`, which lifts to `Load(this + K)` / `Load(this + K + 8)`.
//
// The pattern DSL expresses those with PField, which binds the *object's own*
// address (`this + K`) rather than requiring K == 0; the two field displacements
// constrain each other, so the pattern stays as specific as it was. The outliner
// then materializes `tmp = this + K` ahead of the region so the synthesized call
// still receives a variable -- and `tmp`, not `this`, is what gets typed as the
// container pointer.
//
// `header` forces a non-zero offset for the first member; every accessor here
// therefore reads its container at K > 0.
//
// Build: make -f Makefile.known_patterns
// Compiler used for the checked-in artifact: g++ (Debian 12.2.0-14+deb12u1) 12.2.0,
// libstdc++ new ABI (string: _M_p at +0, _M_string_length at +8, the
// { _M_local_buf[16] | _M_allocated_capacity } union at +16; vector: _M_start at
// +0, _M_finish at +8, _M_end_of_storage at +16).
#include <cstdio>
#include <utility>
#include <string>
#include <vector>

struct Doc
{
    long long header;
    std::vector<long long> items;  // at +8
    std::string name;              // at +32
};

// extern "C": C++ mangled names do not encode the return type, and angr builds
// prototypes from demangled symbols; a C++-mangled accessor would be typed as
// returning void and its body dead-code-eliminated by the decompiler.

// (_M_finish - _M_start) >> 3, both loads at this + 8 / this + 16.
extern "C" __attribute__((noinline)) size_t doc_item_count(const Doc *d)
{
    return d->items.size();
}

// _M_finish == _M_start, at this + 16 / this + 8. Returns int, not bool: a
// bool return packs into al and leaves the rest of rax holding a stale field
// load, which the (deliberately generic) std::string::length pattern then
// matches -- noise that has nothing to do with what this fixture tests.
extern "C" __attribute__((noinline)) int doc_items_empty(const Doc *d)
{
    return d->items.empty() ? 1 : 0;
}

// (_M_end_of_storage - _M_start) >> 3, at this + 24 / this + 8.
extern "C" __attribute__((noinline)) size_t doc_items_capacity(const Doc *d)
{
    return d->items.capacity();
}

// The SSO select on a member string: _M_p (this + 32) tested against the local
// buffer address (this + 48). The printf keeps the two arms merging into one
// join block instead of being tail-duplicated (see known_patterns_stl2.cpp).
extern "C" __attribute__((noinline)) size_t doc_name_capacity(const Doc *d)
{
    size_t c = d->name.capacity();
    printf("%zu\n", c);
    return c;
}

// *(_M_p + _M_string_length - 1): two fields at this + 32 / this + 40.
extern "C" __attribute__((noinline)) char doc_name_back(const Doc *d)
{
    return d->name.back();
}

// --- element-size coverage ---------------------------------------------------
//
// sizeof(std::string) is 32: a power of two, so size() is a bare `>> 5`, but
// there is no named C++ type of that width. Without an opaque T32 element class
// this -- the corpus' single most common vector element type -- had no template.
extern "C" __attribute__((noinline)) size_t names_count(const std::vector<std::string> *v)
{
    return v->size();
}

// operator[] with an 8-byte element. 78% of the corpus' DWARF sites for this
// accessor are 8-byte elements; only the 4-byte (int) template existed.
extern "C" __attribute__((noinline)) long long items_at(const std::vector<long long> *v, size_t i)
{
    return (*v)[i];
}

// --- std::swap on fields -----------------------------------------------------
//
// Three swaps of different widths, which the scheduler interleaves: the next
// field's load lands between this field's store and its writeback. Both halves
// of what made std::swap unmatchable are here -- the widths are 8, 4 and 1
// bytes (only word-sized was covered), and the addresses are fields of an
// object rather than bare pointers.
struct Pair
{
    long long a;
    int b;
    char c;
};

extern "C" __attribute__((noinline)) void pair_swap(Pair *x, Pair *y)
{
    std::swap(x->a, y->a);
    std::swap(x->b, y->b);
    std::swap(x->c, y->c);
}

// One width at a time, with nothing interleaved: the width coverage on its own.
extern "C" __attribute__((noinline)) void swap_ints(int *x, int *y)
{
    std::swap(*x, *y);
}

extern "C" __attribute__((noinline)) void swap_chars(char *x, char *y)
{
    std::swap(*x, *y);
}

// --- stack-allocated containers ----------------------------------------------
//
// A local container is never an object by the time patterns run: variable
// recovery has split it into one virtual variable per slot, so there is no
// Load(s) to match and no `s + 16` to compare against. Measured over the
// benchmark corpus this is 40-54% of the std::string accessor sites.
//
// The printf keeps the accessor's result live and stops the whole local being
// optimized away.
// An opaque callee taking the container's address is what forces the fields back
// into memory: without it the compiler knows the size it just constructed and
// never reads a slot at all, which is why a naive fixture here shows nothing.
void (*g_fill_vec)(std::vector<long long> *) = nullptr;
void (*g_fill_str)(std::string *) = nullptr;

extern "C" __attribute__((noinline)) size_t stack_vec_size(int n)
{
    std::vector<long long> v((size_t)n, 7);
    if (g_fill_vec)
        g_fill_vec(&v);
    size_t k = v.size();
    printf("%zu\n", k);
    return k;
}

extern "C" __attribute__((noinline)) size_t stack_str_capacity(const char *p)
{
    std::string s(p);
    if (g_fill_str)
        g_fill_str(&s);
    size_t c = s.capacity();
    printf("%zu\n", c);
    return c;
}

int main(int argc, char **argv)
{
    Doc d;
    d.header = argc;
    d.items.assign((size_t)argc + 3, 7);
    d.name.assign((size_t)argc + 20, 'x');
    Pair p1 = {argc, argc + 1, (char)argc}, p2 = {argc + 2, argc + 3, (char)(argc + 4)};
    pair_swap(&p1, &p2);
    swap_ints(&p1.b, &p2.b);
    swap_chars(&p1.c, &p2.c);
    printf("%zu %zu\n", stack_vec_size(argc + 1), stack_str_capacity("abcdefghijklmnopqrstuvwxyz"));
    std::vector<std::string> names((size_t)argc + 2, d.name);
    printf("%zu %d %zu %zu %c %zu %lld\n", doc_item_count(&d), doc_items_empty(&d), doc_items_capacity(&d),
           doc_name_capacity(&d), doc_name_back(&d), names_count(&names), items_at(&d.items, 0) + p1.a + p2.b);
    return 0;
}
