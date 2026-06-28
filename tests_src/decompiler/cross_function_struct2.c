// Second test binary for progressive cross-function argument struct type inference in the angr decompiler.
//
// Built with: gcc -g -O0 -fno-pie -no-pie -o cross_function_struct2 cross_function_struct2.c
//
// It covers the ways the union of callee-side argument layouts must NOT leak:
//  1. a string passed to strcmp alongside a struct pointer: strcmp's prototype and every other string parameter
//     have to stay `char *` (the union may only rewrite prototypes the decompiler owns);
//  2. a callee that reads s[0] and s[1]: a run of chars is a string, not `struct {char; char;}`;
//  3. a callee whose own accesses establish more fields than any caller sees: the caller's narrower union must
//     not replace the callee's richer layout.

#include <stdlib.h>
#include <string.h>

struct node {
    char name[16];      // offset 0, read through strcmp in the callers
    struct node *next;  // offset 16
    long weight;        // offset 24
    int flags;          // offset 32
};

struct wide {
    long a;   // offset 0
    int b;    // offset 8
    char c;   // offset 12
    long d;   // offset 16
};

// 2. reads s[0] and s[1] only: must stay a string
int starts_with_dashes(char *s) {
    return s[0] == '-' && s[1] == '-';
}

// 1. two strings, one of which is compared with a struct's name through strcmp
int name_matches(struct node *n, char *wanted) {
    return strcmp(n->name, wanted) == 0;
}

long node_weight(struct node *n) {
    return n->weight;
}

int node_flags(struct node *n) {
    return n->flags;
}

// 3. the callee knows all four fields on its own
long wide_sum(struct wide *w) {
    return w->a + w->b + w->c + w->d;
}

// a caller that only ever touches field a of the same object
long wide_head(struct wide *w) {
    return w->a + wide_sum(w);
}

long walk(struct node *head, char *needle, char *option) {
    long total = 0;
    if (starts_with_dashes(option))
        total += 1;
    for (struct node *n = head; n != NULL; n = n->next) {
        if (name_matches(n, needle))
            total += node_weight(n) + node_flags(n);
    }
    return total;
}

int main(int argc, char **argv) {
    struct node *head = calloc(1, sizeof(struct node));
    struct node *second = calloc(1, sizeof(struct node));
    struct wide *w = calloc(1, sizeof(struct wide));
    head->next = second;
    head->weight = argc;
    second->flags = argc;
    strcpy(second->name, "second");
    w->a = argc; w->b = 2; w->c = 3; w->d = 4;
    return (int)(walk(head, argv[0], argc > 1 ? argv[1] : "--") + wide_head(w));
}
