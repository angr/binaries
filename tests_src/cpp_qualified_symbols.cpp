/*
 * Exercise Itanium C++ symbols whose qualified names do not distinguish a
 * namespace function from a non-static member function.
 *
 * Build from tests_src/:
 *   g++ -O0 -fno-inline -shared -fPIC \
 *       -o ../tests/x86_64/cpp_qualified_symbols.so cpp_qualified_symbols.cpp
 */

namespace demo {

__attribute__((noinline, used)) bool free_value() { return true; }

struct Box {
    __attribute__((noinline, used)) int add(int x) { return x + 1; }
};

__attribute__((used)) int call(Box *box, int x) { return box->add(x); }

}  // namespace demo
