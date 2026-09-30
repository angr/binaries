/*
 * ccop_arm_sub.c -- SUB/CMP condition code triggers for ARM32 and AArch64
 *
 * The ARM CMP instruction is SUB that discards the result, setting NZCV.
 * VEX models this as:
 *   ARM32:  ARMG_CC_OP_SUB  (dep1 = argL, dep2 = argR)
 *   ARM64:  ARM64G_CC_OP_SUB32 / ARM64G_CC_OP_SUB64
 *
 * Each function triggers a specific ARM condition after a CMP/SUB.
 * ARM condition naming: EQ, NE, HS(CS), LO(CC), MI, PL, VS, VC,
 *                       HI, LS, GE, LT, GT, LE
 */

#include "ccop_common.h"

volatile int g_sink;

/* ===================================================================
 * EQ (Z=1): a == b
 * =================================================================== */

NOINLINE int ccop_sub_eq_32(s32 a, s32 b) {
    if (a == b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_eq_64(s64 a, s64 b) {
    if (a == b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * NE (Z=0): a != b
 * =================================================================== */

NOINLINE int ccop_sub_ne_32(s32 a, s32 b) {
    if (a != b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_ne_64(s64 a, s64 b) {
    if (a != b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * HS / CS (C=1): unsigned a >= b
 * =================================================================== */

NOINLINE int ccop_sub_hs_32(u32 a, u32 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_hs_64(u64 a, u64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LO / CC (C=0): unsigned a < b
 * =================================================================== */

NOINLINE int ccop_sub_lo_32(u32 a, u32 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_lo_64(u64 a, u64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * MI (N=1): sign of (a - b), i.e. result negative
 * =================================================================== */

NOINLINE int ccop_sub_mi_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va - vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_mi_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va - vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * PL (N=0): result non-negative
 * =================================================================== */

NOINLINE int ccop_sub_pl_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va - vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_pl_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va - vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * VS (V=1): signed overflow from subtraction
 * =================================================================== */

NOINLINE int ccop_sub_vs_32(s32 a, s32 b) {
    s32 r;
    if (__builtin_sub_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_vs_64(s64 a, s64 b) {
    s64 r;
    if (__builtin_sub_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * VC (V=0): no signed overflow from subtraction
 * =================================================================== */

NOINLINE int ccop_sub_vc_32(s32 a, s32 b) {
    s32 r;
    if (!__builtin_sub_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_vc_64(s64 a, s64 b) {
    s64 r;
    if (!__builtin_sub_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * HI (C=1 && Z=0): unsigned a > b
 * =================================================================== */

NOINLINE int ccop_sub_hi_32(u32 a, u32 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_hi_64(u64 a, u64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LS (C=0 || Z=1): unsigned a <= b
 * =================================================================== */

NOINLINE int ccop_sub_ls_32(u32 a, u32 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_ls_64(u64 a, u64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * GE (N=V): signed a >= b
 * =================================================================== */

NOINLINE int ccop_sub_ge_32(s32 a, s32 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_ge_64(s64 a, s64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LT (N!=V): signed a < b
 * =================================================================== */

NOINLINE int ccop_sub_lt_32(s32 a, s32 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_lt_64(s64 a, s64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * GT (Z=0 && N=V): signed a > b
 * =================================================================== */

NOINLINE int ccop_sub_gt_32(s32 a, s32 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_gt_64(s64 a, s64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LE (Z=1 || N!=V): signed a <= b
 * =================================================================== */

NOINLINE int ccop_sub_le_32(s32 a, s32 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_sub_le_64(s64 a, s64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* =================================================================== */

int main(void) {
    g_sink = 0;

    /* EQ / NE */
    ccop_sub_eq_32(3, 3);
    ccop_sub_ne_32(3, 5);

    /* HS / LO (unsigned) */
    ccop_sub_hs_32(5, 3);
    ccop_sub_lo_32(3, 5);

    /* MI / PL (sign) */
    ccop_sub_mi_32(1, 5);
    ccop_sub_pl_32(5, 1);

    /* VS / VC (overflow) */
    ccop_sub_vs_32(0x7FFFFFFF, -1);
    ccop_sub_vc_32(5, 3);

    /* HI / LS (unsigned) */
    ccop_sub_hi_32(5, 3);
    ccop_sub_ls_32(3, 5);

    /* GE / LT (signed) */
    ccop_sub_ge_32(5, 3);
    ccop_sub_lt_32(-1, 1);

    /* GT / LE (signed) */
    ccop_sub_gt_32(5, 3);
    ccop_sub_le_32(3, 5);

#ifdef __aarch64__
    /* 64-bit variants */
    ccop_sub_eq_64(3, 3);
    ccop_sub_ne_64(3, 5);
    ccop_sub_hs_64(5, 3);
    ccop_sub_lo_64(3, 5);
    ccop_sub_mi_64(1, 5);
    ccop_sub_pl_64(5, 1);
    ccop_sub_vs_64(0x7FFFFFFFFFFFFFFFLL, -1);
    ccop_sub_vc_64(5, 3);
    ccop_sub_hi_64(5, 3);
    ccop_sub_ls_64(3, 5);
    ccop_sub_ge_64(5, 3);
    ccop_sub_lt_64(-1, 1);
    ccop_sub_gt_64(5, 3);
    ccop_sub_le_64(3, 5);
#endif

    return g_sink;
}
