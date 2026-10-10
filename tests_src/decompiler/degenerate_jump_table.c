// gcc -O2 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer -fno-asynchronous-unwind-tables -fno-pie -no-pie -nostdlib -static -o ../../tests/x86_64/decompiler/degenerate_jump_table degenerate_jump_table.c
// Built with GCC 15.3.0 and binutils 2.46.
//
// Every member of the union starts with addr, so the five cases of address_of() load the same word. GCC emits a
// jump table for that switch and then merges the five identical case blocks, which leaves a table whose entries all
// name the block right after the dispatch.

struct wild { unsigned long addr; int size; };
struct shadow { unsigned long addr; short level; };
struct heap { unsigned long addr; long chunk; };
struct stack { unsigned long addr; int frame; };
struct global { unsigned long addr; char name[8]; };

struct desc {
  int kind;
  union { struct wild w; struct shadow sh; struct heap h; struct stack s; struct global g; } u;
};

volatile unsigned long sink;

__attribute__((noipa)) void report(unsigned long a) { sink = a; }
__attribute__((noipa)) void print_wild(const struct desc *d) { sink = 1; }
__attribute__((noipa)) void print_shadow(const struct desc *d) { sink = 2; }
__attribute__((noipa)) void print_heap(const struct desc *d) { sink = 3; }
__attribute__((noipa)) void print_stack(const struct desc *d) { sink = 4; }
__attribute__((noipa)) void print_global(const struct desc *d) { sink = 5; }

static inline unsigned long address_of(const struct desc *d) {
  switch (d->kind) {
    case 0: return d->u.w.addr;
    case 1: return d->u.sh.addr;
    case 2: return d->u.h.addr;
    case 3: return d->u.s.addr;
    case 4: return d->u.g.addr;
  }
  __builtin_trap();
}

static inline __attribute__((always_inline)) void print_kind(const struct desc *d) {
  switch (d->kind) {
    case 0: print_wild(d); break;
    case 1: print_shadow(d); break;
    case 2: print_heap(d); break;
    case 3: print_stack(d); break;
    case 4: print_global(d); break;
  }
}

// The dispatch block is merged with the code after it, which ends in a call.
__attribute__((noipa)) void describe(const struct desc *d, int verbose) {
  report(address_of(d));
  if (verbose)
    print_kind(d);
  report(0);
}

// The dispatch block stays on its own.
__attribute__((noipa)) void describe_kind(const struct desc *d, int verbose) {
  report(address_of(d));
  if (verbose)
    print_kind(d);
}

struct desc the_desc = {2, {.h = {0x1234, 0}}};

void _start(void) {
  describe(&the_desc, 1);
  describe_kind(&the_desc, 1);
  for (;;)
    ;
}
