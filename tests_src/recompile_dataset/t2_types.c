/*
 * t2_types.c -- Type conversion and width-mixing triggers.
 *
 * Tests: signed/unsigned widening, truncation, sign-change casts,
 * bitwise manipulation across widths, mixed-width arithmetic.
 */

#include "harness_common.h"

/* Widen signed 8-bit to 32-bit, operate, store. */
NOINLINE int t2_widen_s8(s32 a, s32 b) {
    s8 narrow = (s8)a;
    s32 wide = (s32)narrow;
    g_sink = wide + b;
    return g_sink;
}

/* Widen unsigned 8-bit to 32-bit. */
NOINLINE int t2_widen_u8(s32 a, s32 b) {
    u8 narrow = (u8)a;
    u32 wide = (u32)narrow;
    g_sink = (s32)(wide + (u32)b);
    return g_sink;
}

/* Widen signed 16-bit to 64-bit. */
NOINLINE int t2_widen_s16_s64(s32 a, s32 b) {
    s16 narrow = (s16)a;
    s64 wide = (s64)narrow;
    g_sink = (s32)(wide + (s64)b);
    return g_sink;
}

/* Truncate 32-bit to 8-bit, then widen back. */
NOINLINE int t2_trunc_widen(s32 a, s32 b) {
    s8 t = (s8)a;
    s32 r = (s32)t + b;
    g_sink = r;
    return g_sink;
}

/* Truncate 64-bit to 16-bit. */
NOINLINE int t2_trunc_64_16(s32 a, s32 b) {
    s64 big = (s64)a * (s64)b;
    s16 small = (s16)big;
    g_sink = (s32)small;
    return g_sink;
}

/* Signed <-> unsigned reinterpretation. */
NOINLINE int t2_sign_change(s32 a, s32 b) {
    u32 ua = (u32)a;
    u32 ub = (u32)b;
    u32 sum = ua + ub;
    s32 result = (s32)sum;
    g_sink = result;
    return g_sink;
}

/* Unsigned comparison after widening. */
NOINLINE int t2_unsigned_cmp(s32 a, s32 b) {
    u32 ua = (u32)a;
    u32 ub = (u32)b;
    if (ua > ub)
        g_sink = 1;
    else if (ua == ub)
        g_sink = 0;
    else
        g_sink = -1;
    return g_sink;
}

/* Mixed-width arithmetic: 16-bit intermediates in 32-bit context. */
NOINLINE int t2_mixed_width(s32 a, s32 b) {
    s16 ha = (s16)a;
    s16 hb = (s16)b;
    s16 prod = ha * hb;  /* 16-bit multiply, may overflow */
    s32 result = (s32)prod + a;
    g_sink = result;
    return g_sink;
}

/* Bit-field style extraction using shifts and masks. */
NOINLINE int t2_bitfield_extract(s32 a, s32 b) {
    u32 val = (u32)a;
    u32 field1 = (val >> 0) & 0xF;     /* bits 0-3 */
    u32 field2 = (val >> 4) & 0xFF;    /* bits 4-11 */
    u32 field3 = (val >> 12) & 0xFFF;  /* bits 12-23 */
    g_sink = (s32)(field1 + field2 * (u32)b + field3);
    return g_sink;
}

/* Bit-field style packing. */
NOINLINE int t2_bitfield_pack(s32 a, s32 b) {
    u32 packed = 0;
    packed |= ((u32)a & 0xF) << 0;
    packed |= ((u32)b & 0xFF) << 4;
    packed |= ((u32)(a + b) & 0xFFF) << 12;
    g_sink = (s32)packed;
    return g_sink;
}

/* Bool conversion: nonzero -> 1, zero -> 0. */
NOINLINE int t2_bool_convert(s32 a, s32 b) {
    bool ba = (a != 0);
    bool bb = (b != 0);
    g_sink = (s32)(ba & bb) + (s32)(ba | bb) + (s32)(ba ^ bb);
    return g_sink;
}

/* Double negation and absolute value via conditional. */
NOINLINE int t2_abs_negate(s32 a, s32 b) {
    s32 abs_a = (a < 0) ? -a : a;
    s32 abs_b = (b < 0) ? -b : b;
    g_sink = abs_a - abs_b;
    return g_sink;
}
