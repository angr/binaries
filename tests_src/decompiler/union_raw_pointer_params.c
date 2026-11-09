// Test binary for argument-struct unions that must stay off raw pointer parameters (angr decompiler,
// cross-function argument type inference).
//
// Built with:
//   gcc -O0 -fno-pie -no-pie -o union_raw_pointer_params union_raw_pointer_params.c
//   gcc -O2 -fno-optimize-sibling-calls -fno-pie -no-pie -o union_raw_pointer_params_O2 union_raw_pointer_params.c
//
// Every function is noipa so that -O2 keeps the call tree and the argument shapes (no inlining, cloning, or IPA-SRA);
// sibling calls are disabled so that every call stays a call.
//
// conn_head/conn_tail each touch half of struct conn; conn_both only passes the pointer on to both, so it learns the
// layout from its callees and must receive the union. serve() hands the same pointer to save_bytes(), which only
// forwards it to memcpy, and to hash_obj(), which only forwards it to hash_bytes(), which indexes it as bytes. Neither
// forwarder dereferences the pointer, so neither may receive the union of struct conn.

#include <string.h>

#define API __attribute__((noipa))

struct conn {
    int fd;     // +0
    int flags;  // +4
    char *buf;  // +8
    long len;   // +16
    long pos;   // +24
};

API int conn_head(struct conn *c) { return c->fd + c->flags; }

API long conn_tail(struct conn *c) { return c->len + c->pos; }

API long conn_both(struct conn *c) { return conn_head(c) + conn_tail(c); }

API void save_bytes(void *dst, const void *src, unsigned long n) { memcpy(dst, src, n); }

API unsigned long hash_bytes(const unsigned char *p, unsigned long n) {
    unsigned long h = 5381;
    for (unsigned long i = 0; i < n; i++) {
        h = h * 33 + p[i];
    }
    return h;
}

API unsigned long hash_obj(const void *p, unsigned long n) { return hash_bytes(p, n) ^ n; }

API long serve(struct conn *c, char *out) {
    save_bytes(out, c, sizeof(*c));
    c->buf = out;
    return conn_both(c) + (long)hash_obj(c, sizeof(*c));
}

int main(int argc, char **argv) {
    static char out[64];
    struct conn c = {argc, 0, 0, 1, 2};
    (void)argv;
    return (int)serve(&c, out);
}
