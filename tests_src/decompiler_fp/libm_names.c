/* Local definitions named after libm functions with mixed int/FP parameters. On SysV amd64 the int and FP
   argument registers are separate sequences, so only the library prototype gives the parameter order. */
#include <stdio.h>
#include <stdlib.h>

double ldexp(double x, int e)
{
    while (e > 0) { x *= 2.0; e--; }
    while (e < 0) { x *= 0.5; e++; }
    return x;
}

double frexp(double x, int *e)
{
    int n = 0;
    while (x >= 1.0) { x *= 0.5; n++; }
    *e = n;
    return x;
}

double jn(int n, double x)
{
    double r = 1.0;
    for (int i = 0; i < n; i++)
        r *= x;
    return r;
}

int main(int argc, char **argv)
{
    double x = argc > 1 ? atof(argv[1]) : 1.5;
    int e;
    double m = frexp(x, &e);
    printf("%f %d %f %f\n", m, e, ldexp(x, argc), jn(argc, x));
    return 0;
}
