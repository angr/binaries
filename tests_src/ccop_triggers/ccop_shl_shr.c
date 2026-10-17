/*
 * ccop_shl_shr.c — SHL/SHR condition code triggers
 *
 * VEX models shift flag-setting as CC_OP_SHLx / CC_OP_SHRx.
 * Pure C shifts trigger these naturally.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ═══════════════════════════════════════════════════════════════════
 * SHL (left shift)
 * ═══════════════════════════════════════════════════════════════════ */

/* ── SHL + CondZ ──────────────────────────────────────────────────── */

NOINLINE int ccop_shl_condz_8(u8 a, u8 n) {
    volatile u8 va = a;
    if ((u8)(va << n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condz_16(u16 a, u16 n) {
    volatile u16 va = a;
    if ((u16)(va << n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condz_32(u32 a, u32 n) {
    if ((a << n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condz_64(u64 a, u64 n) {
    if ((a << n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHL + CondNZ ─────────────────────────────────────────────────── */

NOINLINE int ccop_shl_condnz_8(u8 a, u8 n) {
    volatile u8 va = a;
    if ((u8)(va << n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condnz_16(u16 a, u16 n) {
    volatile u16 va = a;
    if ((u16)(va << n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condnz_32(u32 a, u32 n) {
    if ((a << n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condnz_64(u64 a, u64 n) {
    if ((a << n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHL + CondS ──────────────────────────────────────────────────── */

NOINLINE int ccop_shl_conds_8(s8 a, u8 n) {
    volatile s8 va = a;
    s8 r = (s8)(va << n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_conds_16(s16 a, u16 n) {
    volatile s16 va = a;
    s16 r = (s16)(va << n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_conds_32(s32 a, u32 n) {
    s32 r = a << n;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_conds_64(s64 a, u64 n) {
    s64 r = a << n;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHL + CondNS ─────────────────────────────────────────────────── */

NOINLINE int ccop_shl_condns_8(s8 a, u8 n) {
    volatile s8 va = a;
    s8 r = (s8)(va << n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condns_16(s16 a, u16 n) {
    volatile s16 va = a;
    s16 r = (s16)(va << n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condns_32(s32 a, u32 n) {
    s32 r = a << n;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shl_condns_64(s64 a, u64 n) {
    s64 r = a << n;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════
 * SHR (logical right shift)
 * ═══════════════════════════════════════════════════════════════════ */

/* ── SHR + CondZ ──────────────────────────────────────────────────── */

NOINLINE int ccop_shr_condz_8(u8 a, u8 n) {
    volatile u8 va = a;
    if ((u8)(va >> n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condz_16(u16 a, u16 n) {
    volatile u16 va = a;
    if ((u16)(va >> n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condz_32(u32 a, u32 n) {
    if ((a >> n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condz_64(u64 a, u64 n) {
    if ((a >> n) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHR + CondNZ ─────────────────────────────────────────────────── */

NOINLINE int ccop_shr_condnz_8(u8 a, u8 n) {
    volatile u8 va = a;
    if ((u8)(va >> n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condnz_16(u16 a, u16 n) {
    volatile u16 va = a;
    if ((u16)(va >> n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condnz_32(u32 a, u32 n) {
    if ((a >> n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condnz_64(u64 a, u64 n) {
    if ((a >> n) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHR + CondS ──────────────────────────────────────────────────── */

NOINLINE int ccop_shr_conds_8(u8 a, u8 n) {
    volatile u8 va = a;
    s8 r = (s8)(va >> n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_conds_16(u16 a, u16 n) {
    volatile u16 va = a;
    s16 r = (s16)(va >> n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_conds_32(u32 a, u32 n) {
    s32 r = (s32)(a >> n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_conds_64(u64 a, u64 n) {
    s64 r = (s64)(a >> n);
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── SHR + CondNS ─────────────────────────────────────────────────── */

NOINLINE int ccop_shr_condns_8(u8 a, u8 n) {
    volatile u8 va = a;
    s8 r = (s8)(va >> n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condns_16(u16 a, u16 n) {
    volatile u16 va = a;
    s16 r = (s16)(va >> n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condns_32(u32 a, u32 n) {
    s32 r = (s32)(a >> n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_shr_condns_64(u64 a, u64 n) {
    s64 r = (s64)(a >> n);
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

int main(void) {
    g_sink = 0;

    ccop_shl_condz_8(0x80, 1);
    ccop_shl_condz_16(0x8000, 1);
    ccop_shl_condz_32(0x80000000U, 1);
    ccop_shl_condz_64(0x8000000000000000ULL, 1);

    ccop_shl_condnz_8(1, 4);
    ccop_shl_condnz_16(1, 4);
    ccop_shl_condnz_32(1, 4);
    ccop_shl_condnz_64(1, 4);

    ccop_shl_conds_8(1, 7);
    ccop_shl_conds_16(1, 15);
    ccop_shl_conds_32(1, 31);
    ccop_shl_conds_64(1LL, 63);

    ccop_shl_condns_8(1, 1);
    ccop_shl_condns_16(1, 1);
    ccop_shl_condns_32(1, 1);
    ccop_shl_condns_64(1LL, 1);

    ccop_shr_condz_8(1, 1);
    ccop_shr_condz_16(1, 1);
    ccop_shr_condz_32(1, 1);
    ccop_shr_condz_64(1, 1);

    ccop_shr_condnz_8(0xFF, 1);
    ccop_shr_condnz_16(0xFF, 1);
    ccop_shr_condnz_32(0xFF, 1);
    ccop_shr_condnz_64(0xFF, 1);

    ccop_shr_conds_8(0x80, 0);
    ccop_shr_conds_16(0x8000, 0);
    ccop_shr_conds_32(0x80000000U, 0);
    ccop_shr_conds_64(0x8000000000000000ULL, 0);

    ccop_shr_condns_8(0x80, 1);
    ccop_shr_condns_16(0x8000, 1);
    ccop_shr_condns_32(0x80000000U, 1);
    ccop_shr_condns_64(0x8000000000000000ULL, 1);

    return g_sink;
}
