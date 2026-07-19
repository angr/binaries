// MSVC-compiled STL accessor test binary for angr's KnownPatternFinder.
//
// Purpose: the MSVC STL lays out std::string differently from libstdc++
// (_Mysize at offset +16, behind the 16-byte SSO buffer union; libstdc++ has
// _M_string_length at +8), so the string accessors need MSVC-layout pattern
// variants. std::vector uses the same three-pointer layout in both STLs, so the
// vector accessors are here mainly to confirm that.
//
// This is the MSVC counterpart of known_patterns_stl.cpp: same accessors, but
// __declspec(noinline) instead of __attribute__((noinline)). extern "C" so the
// (unmangled) symbol names keep their return type for angr's prototype recovery.
//
// BUILD (on a Windows machine, from an "x64 Native Tools Command Prompt for VS"):
//
//     cl /O2 /EHsc /MD /Fe:known_patterns_stl_msvc.exe known_patterns_stl_msvc.cpp
//
//   - x64 (use the x64 Native Tools prompt, not x86).
//   - /O2 Release, so the trivial accessors inline into the noinline wrappers.
//   - /MD dynamically links the C++ runtime (msvcp140.dll), which is the
//     is_msvc_cpp_binary evidence angr keys on; /MT (static) also works because
//     the binary still carries MSVC-mangled (?...) symbols.
//
// Then place the resulting binary at:
//     binaries/tests/x86_64/windows/known_patterns_stl_msvc.exe
//
// (Optional sanity check of the string layout:
//     dumpbin /disasm known_patterns_stl_msvc.exe | findstr /I get_len
//  the length load should be at [rcx+10h] = offset 16.)
#include <cstdio>
#include <string>
#include <vector>

extern "C" __declspec(noinline) size_t get_len(const std::string &s)
{
    return s.length();
}

extern "C" __declspec(noinline) bool str_empty(const std::string &s)
{
    return s.empty();
}

extern "C" __declspec(noinline) size_t get_size(const std::vector<int> &v)
{
    return v.size();
}

extern "C" __declspec(noinline) bool vec_empty(const std::vector<int> &v)
{
    return v.empty();
}

extern "C" __declspec(noinline) size_t vec_capacity(const std::vector<int> &v)
{
    return v.capacity();
}

extern "C" __declspec(noinline) int vec_index(const std::vector<int> &v, size_t i)
{
    return v[i];
}

int main(int argc, char **argv)
{
    std::string s(argv[0]);
    std::vector<int> v((size_t)argc, 7);
    printf("%zu %d %zu %d %zu %d\n", get_len(s), (int)str_empty(s), get_size(v), (int)vec_empty(v),
           vec_capacity(v), vec_index(v, (size_t)argc));
    return 0;
}
