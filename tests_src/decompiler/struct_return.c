// gcc      -O0 -fno-pie -c -o ../../tests/x86_64/decompiler/struct_return.o struct_return.c
// gcc -m32 -O0 -fno-pie -c -o ../../tests/i386/decompiler/struct_return.o   struct_return.c
//
// Functions that return a struct, on the two conventions that hand one back differently.
//
// SysV AMD64 returns a struct wider than two eightbytes through memory: the caller passes a
// hidden pointer in rdi and the callee leaves that same address in rax. struct vec3 is 24
// bytes on x86-64 and comes back that way; struct pair is 8 bytes and comes back in rax.
//
// x86 cdecl returns every struct through memory whatever its size, and there the caller
// passes the hidden pointer on the stack rather than in a register while the callee still
// leaves it in eax. Both structs come back that way in the 32-bit build.
//
// make_vec3 calls scale three times so that on x86-64 rdi -- the register the caller passes
// the hidden pointer in -- holds the last call's argument at the return instruction rather
// than the pointer.
//
// Both are relocatable objects, so neither needs a C library to link against.

struct vec3 {
    long x;
    long y;
    long z;
};

struct pair {
    int lo;
    int hi;
};

long scale(long v, long k)
{
    return v * k;
}

struct vec3 make_vec3(long n)
{
    struct vec3 v;

    v.x = scale(n, 2);
    v.y = scale(n, 3);
    v.z = scale(n, 4);
    return v;
}

struct pair split_long(long v)
{
    struct pair p;

    p.lo = (int)v;
    p.hi = (int)(v >> 16 >> 16);
    return p;
}

int main(int argc, char **argv)
{
    struct vec3 v = make_vec3(argc);
    struct pair p = split_long(v.z);

    (void)argv;
    return (int)(v.x + v.y + v.z) + p.lo + p.hi;
}
