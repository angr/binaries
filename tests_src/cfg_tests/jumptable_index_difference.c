// gcc -O2 -fPIC -fno-asynchronous-unwind-tables -nostdlib -static -o ../../tests/x86_64/jumptable_index_difference jumptable_index_difference.c
// Built with GCC 15.3.0 and binutils 2.46.
//
// The switch index in dispatch() is the difference of two values loaded from memory, and the bound check on it sits
// in the block before the table jump:
//
//   sub rax, 2; and rdx, ~7; sub rax, rdx; cmp rax, 6; ja default
//   lea rdx, [rip + table]; movsxd rax, [rdx + rax*4]; add rax, rdx; jmp rax

struct span {
  unsigned long end;
  unsigned long start;
};

volatile unsigned long sink;

__attribute__((noipa)) void case0(void) { sink = 10; }
__attribute__((noipa)) void case1(void) { sink = 21; }
__attribute__((noipa)) void case2(void) { sink = 32; }
__attribute__((noipa)) void case3(void) { sink = 43; }
__attribute__((noipa)) void case4(void) { sink = 54; }
__attribute__((noipa)) void case5(void) { sink = 65; }
__attribute__((noipa)) void case6(void) { sink = 76; }

__attribute__((noipa)) void dispatch(const struct span *s) {
  switch (s->end - (s->start & ~7UL) - 2) {
    case 0: case0(); break;
    case 1: case1(); break;
    case 2: case2(); break;
    case 3: case3(); break;
    case 4: case4(); break;
    case 5: case5(); break;
    case 6: case6(); break;
    default: sink = 0; break;
  }
}

struct span the_span = {0x1013, 0x1008};

void _start(void) {
  dispatch(&the_span);
  for (;;)
    ;
}
