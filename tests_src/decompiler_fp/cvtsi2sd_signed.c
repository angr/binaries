/* cvtsi2sd converts a signed integer: the operand must be typed signed, or the
 * signed conversion must be written out when its C type disagrees. */
struct pair {
    long long v;
    long long w;
};

/* struct fields loaded and converted with cvtsi2sd: typed signed, no extra cast */
double field_to_double(struct pair *p)
{
    return (double)p->v * 2.0 + (double)p->w;
}

/* signed 64-bit parameter */
double s64_to_double(long long x)
{
    return (double)x;
}

/* signed 32-bit parameter (cvtsi2sd xmm0, edi) */
double s32_to_double(int x)
{
    return (double)x;
}

/* the operand is a pointer elsewhere: the signed conversion must be explicit */
void ptr_to_double(double *out, long long *p)
{
    *out = (double)(long long)p + (double)p[1];
}

/* shr makes the operand unsigned; cvtsi2sd xmm0, esi still converts it as int */
void shr_to_double(double *out, unsigned int x)
{
    *out = (double)(int)(x >> 3);
}

/* zero-extended 32-bit load converted with cvtsi2sd xmm0, r64: always fits, no cast */
void u32_to_double(double *out, unsigned int *p)
{
    *out = (double)*p;
}
