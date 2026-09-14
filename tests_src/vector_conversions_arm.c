// zig cc -target arm-linux-gnueabihf -mcpu=cortex_a9 -O1 -fno-inline -o vector_conversions_arm vector_conversions_arm.c
#include <arm_neon.h>
#include <stdio.h>

void truncate_to_ints(const float *in, int *out)
{
    /* vcvt.s32.f32 q: Iop_F32toI32Sx4_RZ */
    vst1q_s32(out, vcvtq_s32_f32(vld1q_f32(in)));
}

void truncate_to_uints(const float *in, unsigned *out)
{
    /* vcvt.u32.f32 q: Iop_F32toI32Ux4_RZ */
    vst1q_u32(out, vcvtq_u32_f32(vld1q_f32(in)));
}

void ints_to_floats(const int *in, float *out)
{
    /* vcvt.f32.s32 q: Iop_I32StoF32x4_DEP */
    vst1q_f32(out, vcvtq_f32_s32(vld1q_s32(in)));
}

void uints_to_floats(const unsigned *in, float *out)
{
    /* vcvt.f32.u32 q: Iop_I32UtoF32x4_DEP */
    vst1q_f32(out, vcvtq_f32_u32(vld1q_u32(in)));
}

int main(int argc, char **argv)
{
    float in[4] = {1.5f, -2.5f, 3.75f, (float)argc};
    int out[4];
    unsigned uout[4];
    float back[4], uback[4];
    truncate_to_ints(in, out);
    truncate_to_uints(in, uout);
    ints_to_floats(out, back);
    uints_to_floats(uout, uback);
    printf("%d %d %d %d | %u %u %u %u | %f %f %f %f | %f %f %f %f\n", out[0], out[1], out[2], out[3], uout[0],
           uout[1], uout[2], uout[3], back[0], back[1], back[2], back[3], uback[0], uback[1], uback[2], uback[3]);
    return 0;
}
