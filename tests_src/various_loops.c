#include <stdio.h>

int a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int b[9];
int dummy;

void nullify()
{
    int i;
    for (i = 0; i < 9; i++)
        b[i] = 0;
}

void for_loop()
{
    int i;
    for (i = 0; i < 9; i++)
        b[i] = a[i];
}

void while_loop()
{
    int i = 9;
    while (i--)
        b[i] = a[i];
}

void do_while_loop()
{
    int i = 0;
    do {
        b[i] = a[i];
        i++;
    } while (i < 9);
}

void nested_for_loop()
{
    int i;
    int j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            b[i+j] = a[i+j];
}

void nested_while_loop()
{
    int i = 0;
    while (i < 3) {
        int j = 0;
        while (j < 3) {
            b[i+j] = a[i+j];
            j++;
        }
        i++;
    }
}

void nested_do_while_loop()
{
    int i = 0;
    do {
        int j = 0;
        do {
            b[i+j] = a[i+j];
            j++;
        } while (j < 3);
        i++;
    } while (i < 3);
}

void break_for_loop()
{
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        if (i == 9) {
            break;
        }
        b[i] = a[i];
    }
}

void break_do_while_loop()
{
    int i = 0;
    int j;
    do {
        b[i] = a[i];
        if (i == 8) {
            break;
        }
        i++;
    } while (i < 16);
}

void symbolic_loop()
{
    int i = 0;
    for (i = 0; i < 9; i++) {
        if (dummy == i)
            break;
        b[i] = a[i];
    }
}

void main()
{
    for_loop();
    nullify();
    while_loop();
    nullify();
    do_while_loop();
    nullify();
    nested_for_loop();
    nullify();
    nested_while_loop();
    nullify();
    nested_do_while_loop();
    nullify();
    break_for_loop();
    nullify();
    break_do_while_loop();
    nullify();
    symbolic_loop();
}
