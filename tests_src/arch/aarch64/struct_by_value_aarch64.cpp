// aarch64-unknown-linux-gnu-g++ -shared -fPIC -O1 -nostdlib -Wl,-z,max-page-size=4096 -o struct_by_value_aarch64.so struct_by_value_aarch64.cpp
//
// AArch64 functions that take a class by value, so that the mangled names carry an aggregate
// argument type. Point is 16 bytes and fits in a register pair, Big is 40 bytes and AAPCS64
// passes it indirectly, and Blob is a class rather than a struct.

struct Point {
    long x;
    long y;
};

struct Big {
    long a, b, c, d, e;
};

class Blob {
  public:
    int v[4];
};

long take_point(Point p) {
    return p.x + p.y;
}

long take_big(Big b) {
    return b.a + b.e;
}

long take_blob(Blob b) {
    return b.v[0] + b.v[3];
}

long call_them(long seed) {
    Point p = {seed, seed + 1};
    Big g = {seed, seed + 1, seed + 2, seed + 3, seed + 4};
    Blob l = {{(int)seed, 0, 0, (int)seed + 3}};
    return take_point(p) + take_big(g) + take_blob(l);
}
