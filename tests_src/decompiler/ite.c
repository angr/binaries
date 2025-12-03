// gcc -o ../../tests/x86_64/decompiler/ite.o -c ite.c
char *assign_ite(int w, char *x, char *y)
{
    *x = 10;
    *y = 20;

    char *z = w ? x : y;
    return z;
}
