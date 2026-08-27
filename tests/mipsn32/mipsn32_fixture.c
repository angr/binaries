/* Small MIPS n32 fixture for angr symbol/function coverage tests.
 * Exercises: 64-bit sd/ld in a prologue (gp spill), GOT-relative global
 * access, an inter-procedural call, and a global array.
 */

long g_counter = 7;
long g_table[8] = {1, 2, 3, 4, 5, 6, 7, 8};
const char g_name[] = "mipsn32";

long leaf_double(long x)
{
    return x + x;
}

/* Non-leaf: spills $gp (and $ra) in the prologue, reads a global through the
 * GOT, indexes a global array, and calls another function. */
long accumulate(long n)
{
    long i, acc = g_counter;
    for (i = 0; i < n && i < 8; i++)
        acc += leaf_double(g_table[i]);
    g_counter = acc;
    return acc;
}

long name_len(void)
{
    const char *p = g_name;
    long n = 0;
    while (*p++)
        n++;
    return n;
}

int main(void)
{
    return (int)(accumulate(5) + name_len());
}
