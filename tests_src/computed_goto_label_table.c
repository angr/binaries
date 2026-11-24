/*
 * A computed-goto interpreter. In a PIE, the label table of run() is an array of
 * pointer-sized entries that the loader relocates, like an array of function
 * pointers, but its targets are labels inside run(), not functions.
 */
#include <stddef.h>

int run(const unsigned char *code, size_t n) __attribute__((noinline));

int run(const unsigned char *code, size_t n)
{
    static void *const table[8] = { &&op_halt, &&op_inc, &&op_dec, &&op_dbl,
                                    &&op_neg, &&op_inc, &&op_dec, &&op_halt };
    size_t pc = 0;
    int acc = 0;
    if (n == 0)
        return 0;
    goto *table[code[pc++] & 7];
op_inc:
    acc += 1;
    if (pc >= n)
        return acc;
    goto *table[code[pc++] & 7];
op_dec:
    acc -= 1;
    if (pc >= n)
        return acc;
    goto *table[code[pc++] & 7];
op_dbl:
    acc *= 2;
    if (pc >= n)
        return acc;
    goto *table[code[pc++] & 7];
op_neg:
    acc = -acc;
    if (pc >= n)
        return acc;
    goto *table[code[pc++] & 7];
op_halt:
    return acc;
}

int main(int argc, char **argv)
{
    return run((const unsigned char *)argv[0], (size_t)argc);
}
