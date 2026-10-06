/*
 cle's STM32 backend reads a 64-byte vector table off the front of a candidate
 image. This C source file is the negative case for that detection: its first
 eight bytes read as a plausible vector table even though it is plainly not
 firmware.

 Bytes 0 to 3 are '/', '*', newline and space. Read little-endian that is
 0x200A2A2F, inside the address range a Cortex-M initial stack pointer
 occupies. Bytes 4 to 7 spell "cle'", which is 0x27656C63; it is odd, so the
 Thumb bit a reset vector carries is set as well.
 */

int main(void)
{
    return 0;
}
