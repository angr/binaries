// gcc -O1 -msse2 -fno-inline -o vector_conversions vector_conversions.c
#include <emmintrin.h>
#include <stdio.h>

void truncate_to_ints(const float *in, int *out)
{
    /* cvttps2dq: Iop_F32toI32Sx4 with a constant (truncating) rounding mode */
    _mm_storeu_si128((__m128i *)out, _mm_cvttps_epi32(_mm_loadu_ps(in)));
}

void ints_to_floats(const int *in, float *out)
{
    /* cvtdq2ps: Iop_I32StoF32x4 rounded per MXCSR */
    _mm_storeu_ps(out, _mm_cvtepi32_ps(_mm_loadu_si128((const __m128i *)in)));
}

void round_to_ints(const float *in, int *out)
{
    /* cvtps2dq: Iop_F32toI32Sx4 rounded per MXCSR */
    _mm_storeu_si128((__m128i *)out, _mm_cvtps_epi32(_mm_loadu_ps(in)));
}

int main(int argc, char **argv)
{
    float in[4] = {1.5f, -2.5f, 3.75f, (float)argc};
    int out[4], rounded[4];
    float back[4];
    truncate_to_ints(in, out);
    ints_to_floats(out, back);
    round_to_ints(in, rounded);
    printf("%d %d %d %d | %f %f %f %f | %d %d %d %d\n", out[0], out[1], out[2], out[3], back[0], back[1], back[2],
           back[3], rounded[0], rounded[1], rounded[2], rounded[3]);
    return 0;
}
