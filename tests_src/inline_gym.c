#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int __attribute__((noinline)) five()
{
        return 5;
}

void __attribute__((noinline)) bar(int x)
{
        char buf[1024];
        read(0, buf, 2);
        read(0, buf, x);
}

void __attribute__((noinline)) foo(int x)
{
        if (x == 1337)
        {
                foo(42);
                char buf[128];
                read(0, buf, 8);
                read(0, buf, *(int *)buf);
        }
        else
        if (x) puts("T");
        else puts("F");
        bar(x);
}

char * __attribute__((noinline)) mylloc(int size)
{
        return malloc(size);
}

int main()
{
        char buf[1024];
        puts(">");
        read(0, buf, 10);
        foo(1);
        read(0, mylloc(five()), 10);
        foo(0);
        foo(1337);
        mylloc(five());
        bar(five()+1);
        bar(five()+five()+five());
        bar(five());
        bar(five()+1);
        mylloc(five()+five()+five());
        bar(10);
}
