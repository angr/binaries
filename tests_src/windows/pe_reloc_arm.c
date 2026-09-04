/*
 * Source of tests/aarch64/windows/pe_reloc_arm64.exe and tests/armel/windows/pe_reloc_armnt.exe.
 *
 * Exercises the two per-architecture structures a Windows ARM image carries that an x86 one
 * does not describe the same way: base relocations for the function pointers in .rdata, and an
 * exception directory entry for every function that manipulates the stack. The register
 * pressure in accumulate() pushes its unwind data out of the packed .pdata form and into an
 * .xdata record, so both encodings appear in one image.
 */

int g_counter;

static int square(int a)
{
    return a * a;
}

static int cube(int a)
{
    return a * a * a;
}

static int negate(int a)
{
    return -a;
}

static int (*const g_table[3])(int) = {square, cube, negate};

int accumulate(const int *values, int count)
{
    int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7;

    for (int i = 0; i < count; i++) {
        a += g_table[i % 3](values[i]);
        b ^= a + i;
        c += b >> 1;
        d ^= c + values[i];
        e += d;
        f ^= e;
        g += f;
        h ^= g;
        if (h > 1000000) {
            return h;
        }
    }
    return a + b + c + d + e + f + g + h;
}

int recurse(int depth)
{
    int local[4];

    if (depth <= 0) {
        return 1;
    }
    for (int i = 0; i < 4; i++) {
        local[i] = depth + i;
    }
    return accumulate(local, 4) + recurse(depth - 1);
}

int mainCRTStartup(void)
{
    g_counter = recurse(3);
    return g_counter;
}
