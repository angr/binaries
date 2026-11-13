// clang -O1 -fPIE -fno-omit-frame-pointer -nostdlib -static -fno-asynchronous-unwind-tables -o ../../tests/x86_64/decompiler/loop_head_switch loop_head_switch.c
// Built with clang 21.1.8 and binutils 2.46.
//
// clang puts the switch's bounds check at the head of the loop and lets the ' ' and ',' cases jump straight back to
// it, so one jump table entry is the loop head itself. Every other case that continues the loop jumps back to a second
// loop head three bytes earlier, which makes the bounds check the head of an inner loop whose only entry from inside
// is the jump table.

volatile int sink;

// Walks a command string and marks cells of a ten-cell strip.
__attribute__((noinline)) int plot(const char *p, unsigned short *strip) {
  int pen, moves = 0;
  for (;;) {
    switch (*p++) {
      case 0: return moves;
      case ' ': case ',': continue;
      case 'G': pen = *p++ - '0'; if ((unsigned)pen > 9) return -3; strip[pen] |= 1; moves++; continue;
      case 'H': pen = *p++ - '0'; if ((unsigned)pen > 9) return -3; strip[pen] = strip[9 - pen]; moves++; continue;
      case 'R': pen = *p++ - '0'; if ((unsigned)pen > 9) return -3; if (strip[pen] != 0) { strip[pen] >>= 1; p -= 2; } moves++; continue;
      case 'S': if (*p == 0) return moves; if (*p == '.') return moves * 2; p++; continue;
      default: return -4;
    }
  }
}

unsigned short strip[10];

void _start(void) {
  sink = plot("G3,H2 R4 S", strip);
  for (;;)
    ;
}
