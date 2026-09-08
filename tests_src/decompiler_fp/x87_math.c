// x87 transcendental / remainder instructions (fsin, fcos, fptan, fpatan, fsqrt, fprem, fprem1,
// fyl2x, fyl2xp1, f2xm1, fscale). Build: see Makefile (x87_math_amd64).
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double x87_sqrt(double x) { return sqrt(x); }
double x87_sin(double x) { return sin(x); }
double x87_cos(double x) { return cos(x); }
double x87_tan(double x) { return tan(x); }
double x87_atan2(double y, double x) { return atan2(y, x); }
double x87_fmod(double x, double y) { return fmod(x, y); }
double x87_remainder(double x, double y) { return remainder(x, y); }
double x87_log2(double x) { return log2(x); }
double x87_log1p(double x) { return log1p(x); }
double x87_exp2(double x) { return exp2(x); }
double x87_ldexp(double x, int e) { return ldexp(x, e); }

int main(int argc, char **argv)
{
    double x = argc > 1 ? atof(argv[1]) : 1.5;
    double y = argc > 2 ? atof(argv[2]) : 0.5;
    printf("%f %f %f %f %f %f %f %f %f %f %f\n", x87_sqrt(x), x87_sin(x), x87_cos(x), x87_tan(x),
           x87_atan2(y, x), x87_fmod(x, y), x87_remainder(x, y), x87_log2(x), x87_log1p(x), x87_exp2(x),
           x87_ldexp(x, argc));
    return 0;
}
