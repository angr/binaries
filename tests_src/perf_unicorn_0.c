#include <stdio.h>
#define SIZE 1000000

int buf[SIZE];

void perf_0()
{
    int i = 0;
    for (i = 0; i < SIZE; ++i)
    {
        buf[i] = i % 0x1337;
    }
}

int main()
{
    perf_0();
}

