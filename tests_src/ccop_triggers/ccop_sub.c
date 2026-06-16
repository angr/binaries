/*
 * ccop_sub.c — SUB/CMP condition code triggers
 *
 * The x86 CMP instruction is SUB that discards the result.
 * VEX models this as CC_OP_SUBx (x = B/W/L/Q for 8/16/32/64).
 * Each function triggers a specific (condition, size) pair.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ── CondZ: a == b ────────────────────────────────────────────────── */

NOINLINE int ccop_sub_condz_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va == vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condz_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va == vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condz_32(s32 a, s32 b) {
    if (a == b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condz_64(s64 a, s64 b) {
    if (a == b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ: a != b ───────────────────────────────────────────────── */

NOINLINE int ccop_sub_condnz_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va != vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnz_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va != vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnz_32(s32 a, s32 b) {
    if (a != b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnz_64(s64 a, s64 b) {
    if (a != b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondL: a < b (signed) ────────────────────────────────────────── */

NOINLINE int ccop_sub_condl_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va < vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condl_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va < vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condl_32(s32 a, s32 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condl_64(s64 a, s64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNL: a >= b (signed) ──────────────────────────────────────── */

NOINLINE int ccop_sub_condnl_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va >= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnl_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va >= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnl_32(s32 a, s32 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnl_64(s64 a, s64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondLE: a <= b (signed) ──────────────────────────────────────── */

NOINLINE int ccop_sub_condle_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va <= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condle_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va <= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condle_32(s32 a, s32 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condle_64(s64 a, s64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNLE: a > b (signed) ──────────────────────────────────────── */

NOINLINE int ccop_sub_condnle_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    if (va > vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnle_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    if (va > vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnle_32(s32 a, s32 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnle_64(s64 a, s64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondB: a < b (unsigned) ──────────────────────────────────────── */

NOINLINE int ccop_sub_condb_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    if (va < vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condb_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    if (va < vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condb_32(u32 a, u32 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condb_64(u64 a, u64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNB: a >= b (unsigned) ────────────────────────────────────── */

NOINLINE int ccop_sub_condnb_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    if (va >= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnb_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    if (va >= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnb_32(u32 a, u32 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnb_64(u64 a, u64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondBE: a <= b (unsigned) ────────────────────────────────────── */

NOINLINE int ccop_sub_condbe_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    if (va <= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condbe_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    if (va <= vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condbe_32(u32 a, u32 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condbe_64(u64 a, u64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNBE: a > b (unsigned) ────────────────────────────────────── */

NOINLINE int ccop_sub_condnbe_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    if (va > vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnbe_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    if (va > vb) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnbe_32(u32 a, u32 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condnbe_64(u64 a, u64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS: sign flag of (a - b) ──────────────────────────────────── */

NOINLINE int ccop_sub_conds_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va - vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_conds_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va - vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_conds_32(s32 a, s32 b) {
    s32 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_conds_64(s64 a, s64 b) {
    s64 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS: not sign flag of (a - b) ─────────────────────────────── */

NOINLINE int ccop_sub_condns_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va - vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condns_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va - vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condns_32(s32 a, s32 b) {
    s32 r = a - b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_sub_condns_64(s64 a, s64 b) {
    s64 r = a - b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

int main(void) {
    g_sink = 0;
    /* Call every function to prevent linker stripping */
    ccop_sub_condz_8(1, 1);
    ccop_sub_condz_16(2, 2);
    ccop_sub_condz_32(3, 3);
    ccop_sub_condz_64(4, 4);

    ccop_sub_condnz_8(1, 2);
    ccop_sub_condnz_16(1, 2);
    ccop_sub_condnz_32(1, 2);
    ccop_sub_condnz_64(1, 2);

    ccop_sub_condl_8(-1, 1);
    ccop_sub_condl_16(-1, 1);
    ccop_sub_condl_32(-1, 1);
    ccop_sub_condl_64(-1, 1);

    ccop_sub_condnl_8(5, 3);
    ccop_sub_condnl_16(5, 3);
    ccop_sub_condnl_32(5, 3);
    ccop_sub_condnl_64(5, 3);

    ccop_sub_condle_8(3, 5);
    ccop_sub_condle_16(3, 5);
    ccop_sub_condle_32(3, 5);
    ccop_sub_condle_64(3, 5);

    ccop_sub_condnle_8(5, 3);
    ccop_sub_condnle_16(5, 3);
    ccop_sub_condnle_32(5, 3);
    ccop_sub_condnle_64(5, 3);

    ccop_sub_condb_8(1, 2);
    ccop_sub_condb_16(1, 2);
    ccop_sub_condb_32(1, 2);
    ccop_sub_condb_64(1, 2);

    ccop_sub_condnb_8(2, 1);
    ccop_sub_condnb_16(2, 1);
    ccop_sub_condnb_32(2, 1);
    ccop_sub_condnb_64(2, 1);

    ccop_sub_condbe_8(1, 2);
    ccop_sub_condbe_16(1, 2);
    ccop_sub_condbe_32(1, 2);
    ccop_sub_condbe_64(1, 2);

    ccop_sub_condnbe_8(2, 1);
    ccop_sub_condnbe_16(2, 1);
    ccop_sub_condnbe_32(2, 1);
    ccop_sub_condnbe_64(2, 1);

    ccop_sub_conds_8(1, 5);
    ccop_sub_conds_16(1, 5);
    ccop_sub_conds_32(1, 5);
    ccop_sub_conds_64(1, 5);

    ccop_sub_condns_8(5, 1);
    ccop_sub_condns_16(5, 1);
    ccop_sub_condns_32(5, 1);
    ccop_sub_condns_64(5, 1);

    return g_sink;
}
