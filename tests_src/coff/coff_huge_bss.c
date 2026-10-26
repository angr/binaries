/*
 * A COFF object whose .bss states half a gigabyte in a file of a few hundred bytes.
 * SizeOfRawData is 32 bits wide and, for a section with no bytes in the file, nothing
 * else in the file bounds it, so a loader that honours the field allocates whatever
 * the field says. This is coff_bss.c with a buffer 0x20000 times the size.
 *
 * i686-w64-mingw32-gcc -c -O2 -fno-asynchronous-unwind-tables -fzero-call-used-regs=used-gpr \
 *     coff_huge_bss.c -o ../../tests/x86/coff_huge_bss.obj
 */

static unsigned char buffer[0x20000000];

unsigned char get(int index)
{
    return buffer[index & 0x1FFFFFFF];
}

void put(int index, unsigned char value)
{
    buffer[index & 0x1FFFFFFF] = value;
}
