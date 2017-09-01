#include <stdio.h>

void _while_true_break()
{
    while(1)
    {
        int a = printf("%d", 1);
        if (a == 0)
        {
            break;
        }
    }
}

void _while()
{
    int a;
    scanf("%d", &a);

    while (a < 100)
    {
        puts("lol");
        a++;
    }
}

void _do_while()
{
    int a;
    scanf("%d", &a);

    do {
        puts("lollol");
        a += 2;
    } while (a < 1000);
}

void _nested_while()
{
    int a;
    unsigned int b;

    scanf("%d", &a);
    scanf("%u", &b);

    while (a < 100)
    {
        while (b < 200)
        {
            a += 1;
            b += a;
            puts("lol");
        }
        b = b / 2;
        puts("umm");
    }
}

int main()
{
    _while_true_break();
    _while();
    _do_while();
    _nested_while();
}
