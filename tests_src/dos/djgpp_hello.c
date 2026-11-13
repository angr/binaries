/* Test input for cle's go32 backend: a function to look up by symbol, an
 * initialised global in .data and a zero-filled array in .bss. */
#include <stdio.h>

int counter = 0x1234abcd;
char buffer[4096];

__attribute__((noinline)) int increment(int value)
{
    return value + 1;
}

int main(void)
{
    buffer[0] = (char)increment(counter);
    printf("%d\n", buffer[0]);
    return 0;
}
