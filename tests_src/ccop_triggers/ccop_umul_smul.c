/*
 * ccop_umul_smul.c — UMUL/SMUL (multiply overflow) condition code triggers
 *
 * VEX models widening multiply overflow as CC_OP_UMULx / CC_OP_SMULx.
 * The overflow flag (OF) is set when the upper half of the result is
 * non-zero (unsigned) or differs from sign-extension of the lower half
 * (signed).
 *
 * Uses __builtin_mul_overflow which GCC compiles to widening multiply
 * + overflow check.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ═══════════════════════════════════════════════════════════════════
 * UMUL — unsigned multiply overflow
 * ═══════════════════════════════════════════════════════════════════ */

/* ── UMUL + CondO, 8-bit ──────────────────────────────────────────── */

NOINLINE int ccop_umul_condo_8(u8 a, u8 b) {
    u8 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondO, 16-bit ─────────────────────────────────────────── */

NOINLINE int ccop_umul_condo_16(u16 a, u16 b) {
    u16 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondO, 32-bit ─────────────────────────────────────────── */

NOINLINE int ccop_umul_condo_32(u32 a, u32 b) {
    u32 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondO, 64-bit ─────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_umul_condo_64(u64 a, u64 b) {
    u64 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ── UMUL + CondNO, 8-bit ─────────────────────────────────────────── */

NOINLINE int ccop_umul_condno_8(u8 a, u8 b) {
    u8 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondNO, 16-bit ────────────────────────────────────────── */

NOINLINE int ccop_umul_condno_16(u16 a, u16 b) {
    u16 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondNO, 32-bit ────────────────────────────────────────── */

NOINLINE int ccop_umul_condno_32(u32 a, u32 b) {
    u32 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── UMUL + CondNO, 64-bit ────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_umul_condno_64(u64 a, u64 b) {
    u64 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ═══════════════════════════════════════════════════════════════════
 * SMUL — signed multiply overflow
 * ═══════════════════════════════════════════════════════════════════ */

/* ── SMUL + CondO, 8-bit ──────────────────────────────────────────── */

NOINLINE int ccop_smul_condo_8(s8 a, s8 b) {
    s8 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondO, 16-bit ─────────────────────────────────────────── */

NOINLINE int ccop_smul_condo_16(s16 a, s16 b) {
    s16 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondO, 32-bit ─────────────────────────────────────────── */

NOINLINE int ccop_smul_condo_32(s32 a, s32 b) {
    s32 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondO, 64-bit ─────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_smul_condo_64(s64 a, s64 b) {
    s64 r;
    if (__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ── SMUL + CondNO, 8-bit ─────────────────────────────────────────── */

NOINLINE int ccop_smul_condno_8(s8 a, s8 b) {
    s8 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondNO, 16-bit ────────────────────────────────────────── */

NOINLINE int ccop_smul_condno_16(s16 a, s16 b) {
    s16 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondNO, 32-bit ────────────────────────────────────────── */

NOINLINE int ccop_smul_condno_32(s32 a, s32 b) {
    s32 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── SMUL + CondNO, 64-bit ────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_smul_condno_64(s64 a, s64 b) {
    s64 r;
    if (!__builtin_mul_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

int main(void) {
    g_sink = 0;

    ccop_umul_condo_8(16, 16);
    ccop_umul_condo_16(256, 256);
    ccop_umul_condo_32(0x10000U, 0x10000U);
#ifdef __x86_64__
    ccop_umul_condo_64(0x100000000ULL, 0x100000000ULL);
#endif

    ccop_umul_condno_8(2, 3);
    ccop_umul_condno_16(2, 3);
    ccop_umul_condno_32(2, 3);
#ifdef __x86_64__
    ccop_umul_condno_64(2, 3);
#endif

    ccop_smul_condo_8(127, 2);
    ccop_smul_condo_16(0x7FFF, 2);
    ccop_smul_condo_32(0x7FFFFFFF, 2);
#ifdef __x86_64__
    ccop_smul_condo_64(0x7FFFFFFFFFFFFFFFLL, 2);
#endif

    ccop_smul_condno_8(2, 3);
    ccop_smul_condno_16(2, 3);
    ccop_smul_condno_32(2, 3);
#ifdef __x86_64__
    ccop_smul_condno_64(2, 3);
#endif

    return g_sink;
}
