/* i386: 64-bit integers are returned in edx:eax. int_div also leaves edx defined (the remainder) on its way out,
 * but nobody reads it, so it must stay an int. */

__attribute__((noinline)) long long ll_mul(int a, int b) { return (long long)a * b; }

__attribute__((noinline)) long long d_to_ll(double d) { return (long long)d; }

__attribute__((noinline)) int int_div(int a, int b) { return a / b; }

void use_ll(int a, int b, double d, long long *out)
{
    out[0] = ll_mul(a, b);
    out[1] = d_to_ll(d);
}

int use_int(int a, int b) { return int_div(a, b) * 3; }
