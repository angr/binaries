// gcc -O1 divisions.c -c -o ../tests/x86_64/divisions_gcc_O1.o
#include <stdint.h>


void lehmer_rng(uint8_t *input, uint8_t *output, uint32_t length, uint32_t seed)
{
    uint32_t state = seed;
    for (int i = 0; i < length; i++) {
        state = (state * 48271) % 2147483647;
        output[i] = (uint8_t)state ^ input[i];
    }
}

void div_5(uint8_t *input, uint8_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 5;
    }
}

void div_5_uint(uint32_t *input, uint32_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 5;
    }

}

void div_7(uint8_t *input, uint8_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 7;
    }
}

void div_7_uint(uint32_t *input, uint32_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 7;
    }
}

void div_9(uint8_t *input, uint8_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 9;
    }
}

void div_9_uint(uint32_t *input, uint32_t *output, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        output[i] = input[i] / 9;
    }
}

