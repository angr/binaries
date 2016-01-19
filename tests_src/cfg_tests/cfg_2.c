#include <stdio.h>

/*
 * CFG test 2
 *
 * A binary with a small jump table inside
 */

void jump_table_a()
{
    int input;
    scanf("%d", &input);

    switch(input)
    {
        case 0:
            puts("0");
            break;
        case 1:
            puts("1");
            break;
        case 2:
            puts("2");
            break;
        case 3:
            puts("3");
            break;
        case 4:
            puts("4");
            break;
        case 5:
            puts("5");
            break;
        case 6:
            puts("6");
            break;
        case 7:
            puts("7");
            break;
        case 8:
            puts("8");
            break;
        case 9:
            puts("9");
            break;
        case 10:
            puts("10");
            break;
        default:
            puts("other values");
            break;
    }
}

int main()
{
    printf("CFG Test #2\n");
    jump_table_a();
}
