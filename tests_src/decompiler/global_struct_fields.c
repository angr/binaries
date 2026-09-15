// Test binary for cross-function recovery of global data structure types in the angr decompiler.
//
// Built with: gcc -g -O0 -fno-pie -no-pie -o global_struct_fields global_struct_fields.c
//
// Two global objects of the same struct type. Every function touches a different member, so no single function can
// recover the full layout on its own; the decompiler has to combine what it learns across functions.
//  1. g_settings is the struct itself: each accessor reads or writes one member through the global's address.
//  2. g_current is a pointer to the struct: it is passed to callees that each dereference a different member.

#include <stdlib.h>
#include <string.h>

struct settings {
    int verbose;     // offset 0
    char *name;      // offset 8
    long limit;      // offset 16
    int flags;       // offset 24
};

struct settings g_settings;
struct settings *g_current;

// 1. by-value global, one member per function
void set_verbose(int v) { g_settings.verbose = v; }
char *get_name(void) { return g_settings.name; }
void bump_limit(long d) { g_settings.limit += d; }
int has_flag(int f) { return g_settings.flags & f; }

// 2. pointer global passed to callees that see different members
int cur_verbose(struct settings *s) { return s->verbose; }
long cur_limit(struct settings *s) { return s->limit; }
int cur_flags(struct settings *s) { return s->flags; }

long use_current(void) {
    return cur_verbose(g_current) + cur_limit(g_current) + cur_flags(g_current);
}

int main(int argc, char **argv) {
    g_current = calloc(1, sizeof(struct settings));
    g_current->name = argv[0];
    set_verbose(argc);
    bump_limit(argc);
    return has_flag(1) + (int)use_current() + (int)strlen(get_name() ? get_name() : "");
}
