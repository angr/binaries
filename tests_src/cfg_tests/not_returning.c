/*
 * gcc not_returning.c -o ../../tests/x86_64/not_returning
 */

#include <stdio.h>
#include <stdlib.h>

void function_c()
{
    /* Calls the exit syscall, and does not return */

    asm volatile (
            "mov $60, %rax\n"
            "xor %rdi, %rdi\n"
            "syscall");
}

void function_a()
{
    /* It might return */

    int input;
    scanf("%d", &input);
    if (input == 1)
    {
        exit(1);
    }
    else if (input == 2)
    {
        function_c();
    }
    else
    {
        return;
    }
}

void function_b()
{
    /* It does not return */

    int input;

    exit(1);
}

void function_d()
{
    printf("How can you get here?\n");
}

int main()
{
    function_a();
    function_b();
    /* Not reachable */
    function_d();
}
