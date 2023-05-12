#include <stdio.h>

int cond0 = 1;

void f(int x)
{
    printf("x = %d", x);
}

int main(int argc, char const *argv[])
{
    if (cond0 == 1) f(1337);
    if (cond0 == 1) f(1337);
    return 0;
}
