
#include <stdio.h>

int main()
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 64; ++i)
    {
        j = i;
    }
    printf("i = %d, j = %d\n", i, j);
}
