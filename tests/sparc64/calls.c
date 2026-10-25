/*
 * SPARC's `call` instruction has p-code after its CALL op: once the callee
 * returns, the instruction falls through, or returns from the function when a
 * `restore` in the delay slot made the call a tail call.
 *
 * musttail makes clang emit both tail calls as `call; restore`. The first one
 * sits in the middle of the function.
 */

__attribute__((noinline)) int scale(int x)
{
    return x * 3 + 1;
}

__attribute__((noinline)) int shift(int x)
{
    return x << 2;
}

__attribute__((noinline)) int tail_in_branch(int x)
{
    if (x > 10)
        __attribute__((musttail)) return scale(x - 10);
    int y = scale(x + 1);
    __attribute__((musttail)) return shift(y);
}

void _start(void)
{
    volatile int r = tail_in_branch(1) + tail_in_branch(20);
    (void)r;
    for (;;) {
    }
}
