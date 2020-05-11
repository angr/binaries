#include <stdio.h>


int volatile global = 0;

void func_1()
{
    global = 0;
}

void func_2()
{
    global = 1;
}

int func_3()
{
    return global * 10;
}

void func_4()
{
    global *= 220;
}

void func_5()
{
    global -= 75;
}

int func_6()
{
    return global + 9;
}

int main()
{
    func_1();
    func_2();
    int c = func_3();
    func_4();
    func_5();
    int d = func_6();
    int sum = c + d;
    printf("Result: %s + %s = %d\n", "func_3", "func_6", sum);
    return 0;
}

