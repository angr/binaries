#include <stdio.h>
#include <string.h>
#include <unistd.h>

int strlen_should_not_fold_into_loop()
{
    char buff[1024];
    read(0, buff, 1020);
    register int a = strlen(buff);
    char *s = buff;
    for (int i = 0; i < 100; ++i)
    {
        if (a < 10)
            s += 10;
    }
    return 0;
}

int strlen_should_not_fold()
{
    char buff[1024];
    read(0, buff, 1020);
    int a = strlen(buff);
    char *s = buff;
    for (int i = a; i < 100; ++i)
    {
        s += 10;
    }
    return a;
}

int strlen_should_fold()
{
    char buff[1024];
    read(0, buff, 1020);
    int a = strlen(buff);
    char *s = buff;
    for (int i = a; i < 100; ++i)
    {
        s += 10;
    }
    return 0;
}

int main()
{
    strlen_should_not_fold_into_loop();
    strlen_should_fold();
}

