#include <stdio.h>


int tight_loop_locals_only(int iterations)
{
    unsigned int a = 1, b = 0, c = 0;
    for (unsigned int i = 0; i < iterations; ++i)
    {
        a *= 20;
        b += 1;
        c += 5;
        a += b * c;
        if (a % 2 == 0)
        {
            b *= 3;
            b -= 1;
            c *= 4;
            c ^= 0xe5a;
        }
        else
        {
            b *= 5;
            b ^= 0xff7;
            c *= 3;
            c ^= 0x1f5;
        }
    }
    return a;
}

int main()
{
    printf("%d\n", tight_loop_locals_only(2000));
}
