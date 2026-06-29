/*
 * Test binary for the decompiler's automatic hex-vs-decimal constant display
 * heuristic (angr.utils.constants.should_use_hex).
 *
 * Each constant below is stored into a volatile global so it survives -O0
 * compilation as a literal operand in the decompiled output, letting the test
 * assert how each one is rendered.
 *
 * Build:
 *   gcc -O0 -fno-inline -no-pie -o ../tests/x86_64/const_hex_display const_hex_display.c
 */
#include <stdint.h>

volatile uint64_t sink;

void const_sink(void)
{
    /* --- expected HEX --- */
    sink = 0xff;        /* >= 8 consecutive 1 bits        -> 0xff       */
    sink = 0x55;        /* alternating 0101 bit pattern   -> 0x55       */
    sink = 0x1111;      /* repeated nibble bit pattern    -> 0x1111     */
    sink = 0x100;       /* power-of-two flag (>= 0x100)   -> 0x100      */
    sink = 0x1200;      /* round hex, low byte zero       -> 0x1200     */
    sink = 0xdeadbeef;  /* well-known magic constant      -> 0xdeadbeef */

    /* --- expected DECIMAL --- */
    sink = 10000;       /* run of identical decimal digits -> 10000     */
    sink = 5000;        /* round decimal (multiple of 1000)-> 5000      */
    sink = 12345;       /* plain number, no structure      -> 12345     */
    sink = 42;          /* plain small number              -> 42        */
}

int main(void)
{
    const_sink();
    return 0;
}
