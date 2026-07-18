// Test binary for angr's KnownPatternFinder: the has-bits accessors that
// protoc generates for optional fields. This reproduces the generated-code
// shape (a _has_bits_ word tested/set/cleared with a per-field mask) without a
// protoc dependency.
//
// Build: make -f Makefile.known_patterns  (g++ -O2 -> Linux ELF)
#include <cstdint>
#include <cstdio>

struct Msg {
    uint32_t _has_bits_[1];
    int32_t field_;
};

// has_field(): (_has_bits_[0] & 0x1) != 0
extern "C" __attribute__((noinline)) bool has_field(const Msg *m)
{
    return (m->_has_bits_[0] & 0x1u) != 0;
}

// set_has_field(): _has_bits_[0] |= 0x1
extern "C" __attribute__((noinline)) void set_has_field(Msg *m)
{
    m->_has_bits_[0] |= 0x1u;
}

// clear_has_field(): _has_bits_[0] &= ~0x1
extern "C" __attribute__((noinline)) void clear_has_field(Msg *m)
{
    m->_has_bits_[0] &= ~0x1u;
}

int main(int argc, char **argv)
{
    Msg m = {{0}, argc};
    set_has_field(&m);
    if (has_field(&m)) {
        clear_has_field(&m);
    }
    printf("%d\n", m.field_);
    return 0;
}
