/*
 * ccop_arm_add.c -- ADD condition code triggers for ARM32 and AArch64
 *
 * The ADDS instruction sets NZCV flags from addition.
 * VEX models this as:
 *   ARM32:  ARMG_CC_OP_ADD  (dep1 = argL, dep2 = argR)
 *   ARM64:  ARM64G_CC_OP_ADD32 / ARM64G_CC_OP_ADD64
 *
 * To get the compiler to emit ADDS (flag-setting add) followed by
 * a conditional branch, we compute a sum and test its properties.
 * For conditions that depend on the add flags directly (not just the
 * result value), we need volatile or inline asm to prevent the
 * compiler from optimizing into a CMP.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ===================================================================
 * EQ (Z=1): (a + b) == 0
 * =================================================================== */

NOINLINE int ccop_add_eq_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va + vb;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_eq_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va + vb;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * NE (Z=0): (a + b) != 0
 * =================================================================== */

NOINLINE int ccop_add_ne_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va + vb;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_ne_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va + vb;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * MI (N=1): (a + b) < 0  (sign of sum)
 * =================================================================== */

NOINLINE int ccop_add_mi_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va + vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_mi_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va + vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * PL (N=0): (a + b) >= 0
 * =================================================================== */

NOINLINE int ccop_add_pl_32(s32 a, s32 b) {
    volatile s32 va = a, vb = b;
    s32 r = va + vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_pl_64(s64 a, s64 b) {
    volatile s64 va = a, vb = b;
    s64 r = va + vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * VS (V=1): signed add overflow
 * =================================================================== */

NOINLINE int ccop_add_vs_32(s32 a, s32 b) {
    s32 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_vs_64(s64 a, s64 b) {
    s64 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * VC (V=0): no signed add overflow
 * =================================================================== */

NOINLINE int ccop_add_vc_32(s32 a, s32 b) {
    s32 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_vc_64(s64 a, s64 b) {
    s64 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * HS (C=1): unsigned add carry  (a + b wraps, i.e. result < a)
 * =================================================================== */

NOINLINE int ccop_add_hs_32(u32 a, u32 b) {
    volatile u32 va = a, vb = b;
    u32 r = va + vb;
    if (r < va) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_hs_64(u64 a, u64 b) {
    volatile u64 va = a, vb = b;
    u64 r = va + vb;
    if (r < va) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LO (C=0): no unsigned add carry  (result >= a)
 * =================================================================== */

NOINLINE int ccop_add_lo_32(u32 a, u32 b) {
    volatile u32 va = a, vb = b;
    u32 r = va + vb;
    if (r >= va) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_add_lo_64(u64 a, u64 b) {
    volatile u64 va = a, vb = b;
    u64 r = va + vb;
    if (r >= va) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * GE (N=V): sum >= 0 in signed interpretation (after adds)
 *
 * Use inline asm to force ADDS + B.GE so the compiler cannot
 * transform this into a CMP-based pattern.
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_ge_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite ge\n\t"
        "movge %[out], #1\n\t"
        "movlt %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_ge_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], ge"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_ge_64(s64 a, s64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], ge"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* ===================================================================
 * LT (N!=V): sum < 0 in signed interpretation (after adds)
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_lt_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite lt\n\t"
        "movlt %[out], #1\n\t"
        "movge %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_lt_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], lt"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_lt_64(s64 a, s64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], lt"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* ===================================================================
 * GT (Z=0 && N=V): sum > 0 (after adds)
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_gt_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite gt\n\t"
        "movgt %[out], #1\n\t"
        "movle %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_gt_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], gt"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_gt_64(s64 a, s64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], gt"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* ===================================================================
 * LE (Z=1 || N!=V): sum <= 0 (after adds)
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_le_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite le\n\t"
        "movle %[out], #1\n\t"
        "movgt %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_le_32(s32 a, s32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], le"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_le_64(s64 a, s64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], le"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* ===================================================================
 * HI (C=1 && Z=0) after ADDS -- via inline asm
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_hi_32(u32 a, u32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite hi\n\t"
        "movhi %[out], #1\n\t"
        "movls %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_hi_32(u32 a, u32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], hi"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_hi_64(u64 a, u64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], hi"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* ===================================================================
 * LS (C=0 || Z=1) after ADDS -- via inline asm
 * =================================================================== */

#ifndef __aarch64__
NOINLINE int ccop_add_ls_32(u32 a, u32 b) {
    int result;
    asm volatile (
        "adds %[r], %[a], %[b]\n\t"
        "ite ls\n\t"
        "movls %[out], #1\n\t"
        "movhi %[out], #0"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#else
NOINLINE int ccop_add_ls_32(u32 a, u32 b) {
    int result;
    asm volatile (
        "adds %w[r], %w[a], %w[b]\n\t"
        "cset %w[out], ls"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_add_ls_64(u64 a, u64 b) {
    int result;
    asm volatile (
        "adds %x[r], %x[a], %x[b]\n\t"
        "cset %w[out], ls"
        : [r] "=r"(result), [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}
#endif

/* =================================================================== */

int main(void) {
    g_sink = 0;

    /* EQ / NE */
    ccop_add_eq_32(1, -1);
    ccop_add_ne_32(1, 2);

    /* MI / PL (sign of sum) */
    ccop_add_mi_32(1, -5);
    ccop_add_pl_32(1, 2);

    /* VS / VC (signed overflow) */
    ccop_add_vs_32(0x7FFFFFFF, 1);
    ccop_add_vc_32(1, 2);

    /* HS / LO (unsigned carry) */
    ccop_add_hs_32(0xFFFFFFFFU, 1);
    ccop_add_lo_32(1, 2);

    /* GE / LT / GT / LE (after adds, via asm) */
    ccop_add_ge_32(1, 2);
    ccop_add_lt_32(-5, 1);
    ccop_add_gt_32(5, 1);
    ccop_add_le_32(-5, 1);

    /* HI / LS (after adds, via asm) */
    ccop_add_hi_32(0x80000000U, 0x80000000U);
    ccop_add_ls_32(1, 2);

#ifdef __aarch64__
    /* 64-bit variants */
    ccop_add_eq_64(1, -1);
    ccop_add_ne_64(1, 2);
    ccop_add_mi_64(1, -5);
    ccop_add_pl_64(1, 2);
    ccop_add_vs_64(0x7FFFFFFFFFFFFFFFLL, 1);
    ccop_add_vc_64(1, 2);
    ccop_add_hs_64(0xFFFFFFFFFFFFFFFFULL, 1);
    ccop_add_lo_64(1, 2);
    ccop_add_ge_64(1, 2);
    ccop_add_lt_64(-5, 1);
    ccop_add_gt_64(5, 1);
    ccop_add_le_64(-5, 1);
    ccop_add_hi_64(0x8000000000000000ULL, 0x8000000000000000ULL);
    ccop_add_ls_64(1, 2);
#endif

    return g_sink;
}
