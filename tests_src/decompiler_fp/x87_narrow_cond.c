/* float arithmetic narrowed inside double expressions (x87): a branch condition, where the call keeps the branch an
   if, a sum with a double constant, and a difference divided by an int converted to double */
int hit(void);

int x87_narrow_cond(const float *v, float w, double t)
{
    if ((v[0] + v[1] + (double)v[2]) * w > t)
        return hit();
    return 0;
}

double x87_narrow_add_const(const float *v, float k)
{
    return (v[0] * k + v[1]) + 1.0;
}

float x87_narrow_div_int(const float *v, int n)
{
    return (v[1] - v[0]) / (double)n;
}
