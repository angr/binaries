// compile with: gcc cfg_switches.c -g -o cfg_switches

#include <stdio.h>

int func_0(int a)
{
    int b;
    // switch-case at the beginning of the function
    switch(a)
    {
        case 1:
            b = a - 20;
            break;
        case 3:
            b = a - 40;
            break;
        case 5:
            b = a - 80;
            break;
        case 7:
            b = a - 120;
            break;
        case 9:
            b = a - 140;
            break;
        case 12:
            b = a - 220;
            break;
        default:
            b = 0;
            break;
    }
    return b;
}

int func_1(int a, int b)
{
    // switch-case is not at the beginning of the function
    puts("func_1");
    switch(a)
    {
        case 2:
            puts("2");
            break;
        case 10:
            puts("LOL");
            break;
        case 12:
            puts("Bad!");
            break;
        case 14:
            puts("14");
            break;
        case 15:
            puts("15");
            break;
        case 16:
            puts("!6");
            break;
        case 22:
            puts("22");
            break;
        case 24:
            puts("24");
            break;
        case 28:
            puts("28");
            break;
        case 38:
            puts("38");
            // Another jump table
            switch(b)
            {
                case 1:
                    puts("b = 1\n");
                    break;
                case 2:
                    puts("b = 2\n");
                    break;
                case 3:
                    puts("b = 3\n");
                    break;
                case 4:
                    puts("b = 4\n");
                    break;
                case 5:
                    puts("b = 5\n");
                    break;
                default:
                    puts("b = something\n");
                    break;
            }
            break;
        default:
            puts("Umm...");
            break;
    }

    return 0;
}

int main(int argc, char** argv)
{
    puts("Switch table test.");
    switch(argc)
    {
        case 1: case 3: case 5: case 7: case 9:
            func_0(argc);
            break;
        case 2:
            func_1(2, 2);
            break;
        case 4:
            func_1(4, 4);
            break;
        case 6:
            func_1(6, 6);
            break;
        case 8:
            func_1(8, 8);
            break;
        case 10:
            func_1(10, 10);
            break;
        case 45:
            func_0(45);
            break;
        default:
            break;
    }

    return 0;
}
