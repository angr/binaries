/*
 * A computed goto whose labels use no state of their function: each one only
 * sets the return value and returns. In a PIE the label table is an array of
 * relocated pointers, and with -falign-labels=16 the labels are aligned like
 * function entries, so the machine code is the same as a table of two tiny
 * functions. Only the FDE of dispatch(), which covers both labels, tells them
 * apart.
 */
__attribute__((noinline, noclone)) int dispatch(unsigned opcode)
{
    static void *const table[] = { &&op0, &&op1 };

    goto *table[opcode & 1];
op0:
    return 10;
op1:
    return 20;
}

int main(int argc, char **argv)
{
    (void)argv;
    return dispatch(argc);
}
