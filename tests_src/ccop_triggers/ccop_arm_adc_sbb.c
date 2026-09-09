/*
 * ccop_arm_adc_sbb.c -- ADC/SBC condition code triggers for ARM32 and AArch64
 *
 * VEX models add-with-carry and subtract-with-carry as:
 *   ARM32:  ARMG_CC_OP_ADC  (dep1 = argL, dep2 = argR, dep3 = oldC)
 *           ARMG_CC_OP_SBB  (dep1 = argL, dep2 = argR, dep3 = oldC)
 *   ARM64:  ARM64G_CC_OP_ADC32 / ARM64G_CC_OP_ADC64
 *           ARM64G_CC_OP_SBC32 / ARM64G_CC_OP_SBC64
 *
 * Strategy:
 *   ARM32: 64-bit arithmetic on a 32-bit target compiles to
 *          adds+adc / subs+sbc pairs. Alternatively, use inline asm
 *          to directly emit ADCS/SBCS with known carry-in.
 *   ARM64: 128-bit arithmetic (unsigned __int128) compiles to
 *          adds+adc / subs+sbc pairs for the high 64 bits.
 *          For 32-bit ADC/SBC on ARM64, we need inline asm.
 *
 * The flags from the ADC/SBC instruction are what VEX captures.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ===================================================================
 * ARM32: 64-bit arithmetic generates add+adc / sub+sbc
 *
 * On a 32-bit ARM target, uint64_t/int64_t operations are split
 * into pairs of 32-bit operations. The second half uses ADC/SBC.
 * =================================================================== */

#ifndef __aarch64__

/* -- ADC conditions from uint64_t addition -- */

/* ADC + EQ: 64-bit add result == 0 */
NOINLINE int ccop_adc_eq_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + NE: 64-bit add result != 0 */
NOINLINE int ccop_adc_ne_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + MI: 64-bit signed add result < 0 */
NOINLINE int ccop_adc_mi_32(s64 a, s64 b) {
    s64 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + PL: 64-bit signed add result >= 0 */
NOINLINE int ccop_adc_pl_32(s64 a, s64 b) {
    s64 r = a + b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + HS: 64-bit unsigned add carry (result < a) */
NOINLINE int ccop_adc_hs_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* -- SBC conditions from uint64_t subtraction -- */

/* SBC + EQ: 64-bit sub result == 0 */
NOINLINE int ccop_sbc_eq_32(u64 a, u64 b) {
    u64 r = a - b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + NE: 64-bit sub result != 0 */
NOINLINE int ccop_sbc_ne_32(u64 a, u64 b) {
    u64 r = a - b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LO: 64-bit unsigned a < b */
NOINLINE int ccop_sbc_lo_32(u64 a, u64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + HS: 64-bit unsigned a >= b */
NOINLINE int ccop_sbc_hs_32(u64 a, u64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + HI: 64-bit unsigned a > b */
NOINLINE int ccop_sbc_hi_32(u64 a, u64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LS: 64-bit unsigned a <= b */
NOINLINE int ccop_sbc_ls_32(u64 a, u64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LT: 64-bit signed a < b */
NOINLINE int ccop_sbc_lt_32(s64 a, s64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + GE: 64-bit signed a >= b */
NOINLINE int ccop_sbc_ge_32(s64 a, s64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + GT: 64-bit signed a > b */
NOINLINE int ccop_sbc_gt_32(s64 a, s64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LE: 64-bit signed a <= b */
NOINLINE int ccop_sbc_le_32(s64 a, s64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + MI: 64-bit signed sub result < 0 */
NOINLINE int ccop_sbc_mi_32(s64 a, s64 b) {
    s64 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* -- Inline asm ADC/SBC with explicit carry-in -- */

/* ADCS with carry-in=1: r = a + b + 1, test EQ */
NOINLINE int ccop_adc_carry_eq_32(u32 a, u32 b) {
    u32 result;
    int cond;
    /* Set carry, then ADCS */
    asm volatile (
        "cmp %[z], #0\n\t"         /* sets C=1 (0 >= 0 unsigned) */
        "adcs %[r], %[a], %[b]\n\t"
        "ite eq\n\t"
        "moveq %[c], #1\n\t"
        "movne %[c], #0"
        : [r] "=r"(result), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b), [z] "r"(0)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* SBCS with carry-in=1: r = a - b, test LO (borrow) */
NOINLINE int ccop_sbc_carry_lo_32(u32 a, u32 b) {
    u32 result;
    int cond;
    /* Set carry (no borrow), then SBCS */
    asm volatile (
        "cmp %[z], #0\n\t"         /* sets C=1 */
        "sbcs %[r], %[a], %[b]\n\t"
        "ite cc\n\t"
        "movcc %[c], #1\n\t"
        "movcs %[c], #0"
        : [r] "=r"(result), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b), [z] "r"(0)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* ADCS with carry-in=0: r = a + b + 0 (same as ADDS, but VEX sees ADC) */
NOINLINE int ccop_adc_nocarry_vs_32(s32 a, s32 b) {
    s32 result;
    int cond;
    asm volatile (
        "adds %[r], %[z], #0\n\t"  /* clears C */
        "adcs %[r], %[a], %[b]\n\t"
        "ite vs\n\t"
        "movvs %[c], #1\n\t"
        "movvc %[c], #0"
        : [r] "=r"(result), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b), [z] "r"(0)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

#else /* __aarch64__ */

/* ===================================================================
 * AArch64: __int128 arithmetic generates adds+adc / subs+sbc
 * =================================================================== */

typedef unsigned __int128 u128;
typedef __int128 s128;

/* -- ADC conditions from __int128 addition -- */

/* ADC + EQ: 128-bit add result == 0 */
NOINLINE int ccop_adc_eq_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + NE: 128-bit add result != 0 */
NOINLINE int ccop_adc_ne_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + MI: 128-bit signed add result < 0 */
NOINLINE int ccop_adc_mi_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    s128 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + PL: 128-bit signed add result >= 0 */
NOINLINE int ccop_adc_pl_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    s128 r = a + b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + HS: 128-bit unsigned add carry */
NOINLINE int ccop_adc_hs_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* -- SBC conditions from __int128 subtraction -- */

/* SBC + EQ: 128-bit sub result == 0 */
NOINLINE int ccop_sbc_eq_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a - b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LO: 128-bit unsigned a < b */
NOINLINE int ccop_sbc_lo_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + HS: 128-bit unsigned a >= b */
NOINLINE int ccop_sbc_hs_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + HI: 128-bit unsigned a > b */
NOINLINE int ccop_sbc_hi_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LS: 128-bit unsigned a <= b */
NOINLINE int ccop_sbc_ls_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + LT: 128-bit signed a < b */
NOINLINE int ccop_sbc_lt_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + GE: 128-bit signed a >= b */
NOINLINE int ccop_sbc_ge_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBC + MI: 128-bit signed sub result < 0 */
NOINLINE int ccop_sbc_mi_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    s128 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* -- Inline asm ADCS/SBCS for 32-bit width on AArch64 -- */

/* ADCS 32-bit with carry-in=1: test EQ */
NOINLINE int ccop_adc_carry_eq_32(u32 a, u32 b) {
    int cond;
    asm volatile (
        "cmp wzr, wzr\n\t"          /* sets C=1, Z=1 */
        "adcs %w[r], %w[a], %w[b]\n\t"
        "cset %w[c], eq"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* ADCS 64-bit with carry-in=1: test EQ */
NOINLINE int ccop_adc_carry_eq_asm_64(u64 a, u64 b) {
    int cond;
    asm volatile (
        "cmp xzr, xzr\n\t"          /* sets C=1 */
        "adcs %x[r], %x[a], %x[b]\n\t"
        "cset %w[c], eq"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* SBCS 32-bit with carry-in=1 (no borrow): test LO */
NOINLINE int ccop_sbc_carry_lo_32(u32 a, u32 b) {
    int cond;
    asm volatile (
        "cmp wzr, wzr\n\t"          /* sets C=1 */
        "sbcs %w[r], %w[a], %w[b]\n\t"
        "cset %w[c], lo"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* SBCS 64-bit with carry-in=1: test LO */
NOINLINE int ccop_sbc_carry_lo_asm_64(u64 a, u64 b) {
    int cond;
    asm volatile (
        "cmp xzr, xzr\n\t"          /* sets C=1 */
        "sbcs %x[r], %x[a], %x[b]\n\t"
        "cset %w[c], lo"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* ADCS 32-bit with carry-in=0: test VS (signed overflow) */
NOINLINE int ccop_adc_nocarry_vs_32(s32 a, s32 b) {
    int cond;
    asm volatile (
        "msr nzcv, xzr\n\t"         /* NZCV = 0000 (all clear, C=0) */
        "adcs %w[r], %w[a], %w[b]\n\t"
        "cset %w[c], vs"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

/* ADCS 64-bit with carry-in=0: test VS */
NOINLINE int ccop_adc_nocarry_vs_64(s64 a, s64 b) {
    int cond;
    asm volatile (
        "msr nzcv, xzr\n\t"         /* NZCV = 0000 (all clear, C=0) */
        "adcs %x[r], %x[a], %x[b]\n\t"
        "cset %w[c], vs"
        : [r] "=r"(cond), [c] "=r"(cond)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = cond;
    return g_sink;
}

#endif /* __aarch64__ */

/* =================================================================== */

int main(void) {
    g_sink = 0;

#ifndef __aarch64__
    /* ARM32: 64-bit pair generates adds+adc / subs+sbc */

    /* ADC conditions */
    ccop_adc_eq_32(0xFFFFFFFFFFFFFFFFULL, 1);
    ccop_adc_ne_32(1, 2);
    ccop_adc_mi_32(0x7FFFFFFFFFFFFFFFLL, 1);
    ccop_adc_pl_32(1, 2);
    ccop_adc_hs_32(0xFFFFFFFFFFFFFFFFULL, 1);

    /* SBC conditions */
    ccop_sbc_eq_32(5, 5);
    ccop_sbc_ne_32(5, 3);
    ccop_sbc_lo_32(1, 2);
    ccop_sbc_hs_32(2, 1);
    ccop_sbc_hi_32(2, 1);
    ccop_sbc_ls_32(1, 1);
    ccop_sbc_lt_32(-1, 1);
    ccop_sbc_ge_32(1, -1);
    ccop_sbc_gt_32(2, 1);
    ccop_sbc_le_32(1, 2);
    ccop_sbc_mi_32(0, 1);

    /* Inline asm with explicit carry */
    ccop_adc_carry_eq_32(0xFFFFFFFF, 0);
    ccop_sbc_carry_lo_32(1, 5);
    ccop_adc_nocarry_vs_32(0x7FFFFFFF, 1);

#else
    /* AArch64: __int128 generates adds+adc / subs+sbc */

    /* ADC conditions */
    ccop_adc_eq_64(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 1, 0);
    ccop_adc_ne_64(1, 0, 2, 0);
    ccop_adc_mi_64(0, 0x7FFFFFFFFFFFFFFFLL, 0, 1);
    ccop_adc_pl_64(1, 0, 2, 0);
    ccop_adc_hs_64(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 1, 0);

    /* SBC conditions */
    ccop_sbc_eq_64(5, 0, 5, 0);
    ccop_sbc_lo_64(1, 0, 2, 0);
    ccop_sbc_hs_64(2, 0, 1, 0);
    ccop_sbc_hi_64(2, 0, 1, 0);
    ccop_sbc_ls_64(1, 0, 1, 0);
    ccop_sbc_lt_64(0, -1, 0, 1);
    ccop_sbc_ge_64(0, 1, 0, -1);
    ccop_sbc_mi_64(0, 0, 0, 1);

    /* Inline asm with explicit carry -- 32-bit */
    ccop_adc_carry_eq_32(0xFFFFFFFF, 0);
    ccop_sbc_carry_lo_32(1, 5);
    ccop_adc_nocarry_vs_32(0x7FFFFFFF, 1);

    /* Inline asm with explicit carry -- 64-bit */
    ccop_adc_carry_eq_asm_64(0xFFFFFFFFFFFFFFFFULL, 0);
    ccop_sbc_carry_lo_asm_64(1, 5);
    ccop_adc_nocarry_vs_64(0x7FFFFFFFFFFFFFFFLL, 1);

#endif

    return g_sink;
}
