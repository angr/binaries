#include <unistd.h>
#include <stdio.h>

void read_byte(char* buf)
{
    read(0, buf, 1);
}

void test_func()
{
    int i;
    int b_count = 0;
    char user_input[200];

    /* This is basically a read_until() */
    for (i = 0; i < 60; ++i)
    {
        read_byte(user_input + i);
        if (user_input[i] == '\n')
        {
            break;
        }
        if (user_input[i] == 'B')
        {
            ++b_count;
        }
    }

    if (b_count == 35)
    {
        printf("There are 35 'B's in your input.\n");
        printf("Easter egg triggered!\n");
    }
}

int main()
{
    test_func();
}

