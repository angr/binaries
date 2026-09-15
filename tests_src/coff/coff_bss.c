/*
 * A COFF object whose .bss is long enough to reach past the section table and over .text.
 * A section with no bytes in the file states PointerToRawData 0, so a loader that places
 * sections at their file offsets puts this one on top of the file header and the code.
 *
 * i686-w64-mingw32-gcc -c -O2 -fno-asynchronous-unwind-tables -fzero-call-used-regs=used-gpr \
 *     coff_bss.c -o ../../tests/x86/coff_bss.obj
 */

static unsigned char buffer[4096];

unsigned char get(int index)
{
    return buffer[index & 4095];
}

void put(int index, unsigned char value)
{
    buffer[index & 4095] = value;
}
