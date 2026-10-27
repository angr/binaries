// Test binary for whole-binary type propagation in the angr decompiler (BinaryDecompiler).
//
// Built with:
//   gcc -g -O0 -fno-pie -no-pie -o binary_struct_propagation binary_struct_propagation.c
//   gcc -g -O2 -fno-pie -no-pie -o binary_struct_propagation_O2 binary_struct_propagation.c
//
// Every function is noipa so that -O2 keeps the call tree and the argument shapes (no inlining, cloning, or IPA-SRA).
//
// The call tree is
//
//   main -> serve -> handle_io -> read_side, write_side
//                 -> log_conn  -> log_host, log_pos, tag_chars
//        -> find_fd, count_conns, set_name, clamp
//        -> drain_a <-> drain_b (recursion)
//
// and the same struct conn pointer flows from serve down to every leaf. Each leaf touches a different slice of struct
// conn, so no single function can recover the whole layout: the callers have to union what their callees learned
// (leaves to root), and the union has to be pushed back down so that every leaf renders the whole struct (root to
// leaves). Two globals are shaped the same way: g_stats is a struct by value whose members are touched by functions
// at every depth of the tree, and g_head is a pointer to struct conn that is walked by functions that see different
// members of it.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define API __attribute__((noipa))

struct point {
    int x;  // +0
    int y;  // +4
};

struct conn {
    int fd;               // 0   read_side, find_fd, main
    int state;            // 4   write_side, drain_a
    char *host;           // 8   log_host, main
    struct point pos;     // 16  log_pos (x @16, y @20)
    long bytes_in;        // 24  read_side
    long bytes_out;       // 32  write_side
    unsigned short port;  // 40  log_host, drain_b
    char tag[6];          // 42  tag_chars (tag[0] @42, tag[5] @47)
    struct conn *next;    // 48  serve, drain_b, count_conns, find_fd, main
};                        // 56 bytes

struct stats {
    long requests;        // 0   read_side
    long errors;          // 8   write_side
    int max_conns;        // 16  serve
    int cur_conns;        // 20  serve
    long total_bytes;     // 24  log_conn
    struct point origin;  // 32  log_pos, main (x @32, y @36)
    char name[16];        // 40  set_name
};                        // 56 bytes

struct stats g_stats;
struct conn *g_head;

// leaves under handle_io
API long read_side(struct conn *c) {
    g_stats.requests++;
    c->bytes_in += c->fd;
    return c->bytes_in;
}

API long write_side(struct conn *c) {
    if (c->state == 2)
        g_stats.errors++;
    c->bytes_out += 1;
    return c->bytes_out;
}

// leaves under log_conn
API void log_host(struct conn *c) { printf("%s:%hu\n", c->host, c->port); }

API int log_pos(struct conn *c) { return c->pos.x * c->pos.y + g_stats.origin.x - g_stats.origin.y; }

API int tag_chars(struct conn *c) { return c->tag[0] ^ c->tag[5]; }

// the middle of the tree: these only pass the pointer down (log_conn also touches one member of g_stats)
API long handle_io(struct conn *c) { return read_side(c) + write_side(c); }

API void log_conn(struct conn *c) {
    log_host(c);
    g_stats.total_bytes += log_pos(c) + tag_chars(c);
}

// the top of the tree below main
API long serve(struct conn *c) {
    long n = 0;
    for (; c != NULL; c = c->next) {
        g_stats.cur_conns++;
        n += handle_io(c);
        log_conn(c);
    }
    if (g_stats.cur_conns > g_stats.max_conns)
        g_stats.max_conns = g_stats.cur_conns;
    return n;
}

// a recursion cycle: each side touches a different member
API int drain_b(struct conn *c, int n);

API int drain_a(struct conn *c, int n) {
    if (n <= 0 || c == NULL)
        return 0;
    c->state = 0;
    return 1 + drain_b(c, n - 1);
}

API int drain_b(struct conn *c, int n) {
    if (n <= 0)
        return 0;
    c->port = 0;
    return 1 + drain_a(c->next, n - 1);
}

// functions that walk the list through the pointer global
API int count_conns(void) {
    int n = 0;
    for (struct conn *c = g_head; c != NULL; c = c->next)
        n++;
    return n;
}

API struct conn *find_fd(int fd) {
    for (struct conn *c = g_head; c != NULL; c = c->next)
        if (c->fd == fd)
            return c;
    return NULL;
}

// a member of the by-value global handed to a library function: strncpy's prototype must not be refined
API void set_name(const char *s) { strncpy(g_stats.name, s, sizeof(g_stats.name) - 1); }

// scalars only: must not be given a struct type
API int clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

int main(int argc, char **argv) {
    set_name(argv[0]);
    g_stats.origin.x = argc;
    g_stats.origin.y = argc * 2;
    for (int i = 0; i < argc; i++) {
        struct conn *c = calloc(1, sizeof(*c));
        c->fd = i + 3;
        c->host = argv[i];
        c->next = g_head;
        g_head = c;
    }
    long n = serve(g_head);
    struct conn *found = find_fd(clamp(argc, 3, 10));
    int drained = drain_a(g_head, count_conns());
    printf("%ld %d %d %s\n", n, drained, found != NULL ? found->fd : -1, g_stats.name);
    return 0;
}
