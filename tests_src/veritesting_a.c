#include <unistd.h>
#include <stdio.h>

void test_func()
{
    int i;
    int b_count = 0;
    char user_input[200];

    read(0, user_input, 200);
    for (i = 0; i < 20; ++i)
    {
        if (user_input[i] == 'B')
        {
            ++b_count;
        }
    }

    if (b_count == 10)
    {
        printf("There are 10 'B's in your input.\n");
        printf("Easter egg triggered!\n");
    }
}

int main()
{
    test_func();
}

