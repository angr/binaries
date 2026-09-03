/*
 * ccop_arm_logic.c -- LOGIC (AND/ORR/EOR/TST/TEQ) condition code triggers
 *                     for ARM32 and AArch64
 *
 * VEX models flag-setting from bitwise operations as:
 *   ARM32:  ARMG_CC_OP_LOGIC  (dep1 = result, dep2 = shifter_carry_out)
 *   ARM64:  ARM64G_CC_OP_LOGIC32 / ARM64G_CC_OP_LOGIC64
 *
 * The TST instruction (ANDS with result discarded) is the most common
 * trigger. For ARM LOGIC:
 *   N = sign bit of result
 *   Z = (result == 0)
 *   C = shifter carry out (dep2) -- ARM32 specific
 *   V = 0 (always cleared)
 *
 * Because V=0 for LOGIC:
 *   GE (N=V) is equivalent to PL (N=0)
 *   LT (N!=V) is equivalent to MI (N=1)
 *   GT (Z=0 && N=V) is equivalent to (result > 0, signed)
 *   LE (Z=1 || N!=V) is equivalent to (result <= 0, signed)
 *   VS is always false, VC is always true
 *
 * For HS/LO on ARM32, they depend on the shifter carry out (dep2),
 * which is ARM-architecture-specific. We use inline asm for those.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ===================================================================
 * EQ (Z=1): (a & b) == 0   -- TST a, b
 * =================================================================== */

NOINLINE int ccop_logic_eq_32(u32 a, u32 b) {
    volatile u32 va = a, vb = b;
    if ((va & vb) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_eq_64(u64 a, u64 b) {
    volatile u64 va = a, vb = b;
    if ((va & vb) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * NE (Z=0): (a & b) != 0   or   test reg,reg (if (a))
 * =================================================================== */

NOINLINE int ccop_logic_ne_32(u32 a, u32 b) {
    volatile u32 va = a, vb = b;
    if ((va & vb) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* Single-operand variant: test reg,reg -- "if (a)" */
NOINLINE int ccop_logic_ne_self_32(s32 a) {
    if (a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_ne_64(u64 a, u64 b) {
    volatile u64 va = a, vb = b;
    if ((va & vb) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_ne_self_64(s64 a) {
    if (a) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * MI (N=1): sign bit of (a & b) is set, or sign of single value
 * =================================================================== */

NOINLINE int ccop_logic_mi_32(s32 a) {
    /* "tst a, a; bmi" or "cmp a, #0; blt" -- compiler picks */
    if (a < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_mi_64(s64 a) {
    if (a < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * PL (N=0): sign bit clear
 * =================================================================== */

NOINLINE int ccop_logic_pl_32(s32 a) {
    if (a >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_pl_64(s64 a) {
    if (a >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * GE (N=V): since V=0 for LOGIC, this is N=0, same as PL
 * We include it as a distinct function because the VEX condition
 * code is different (CondGE vs CondPL), even though the semantic
 * result is the same for LOGIC.
 * =================================================================== */

NOINLINE int ccop_logic_ge_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_ge_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LT (N!=V): since V=0 for LOGIC, this is N=1, same as MI
 * =================================================================== */

NOINLINE int ccop_logic_lt_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_lt_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * GT (Z=0 && N=V): since V=0 for LOGIC, this is (Z=0 && N=0)
 * i.e. result > 0 (signed)
 * =================================================================== */

NOINLINE int ccop_logic_gt_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_gt_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * LE (Z=1 || N!=V): since V=0, this is (Z=1 || N=1)
 * i.e. result <= 0 (signed)
 * =================================================================== */

NOINLINE int ccop_logic_le_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_le_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * AND with two operands -- TST mask pattern
 * These ensure the compiler emits TST (ANDS) rather than CMP.
 * =================================================================== */

NOINLINE int ccop_logic_and_eq_32(u32 val, u32 mask) {
    if ((val & mask) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_and_ne_32(u32 val, u32 mask) {
    if ((val & mask) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_and_mi_32(s32 val, s32 mask) {
    volatile s32 r = val & mask;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_and_eq_64(u64 val, u64 mask) {
    if ((val & mask) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_and_ne_64(u64 val, u64 mask) {
    if ((val & mask) != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_and_mi_64(s64 val, s64 mask) {
    volatile s64 r = val & mask;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * ORR result tests -- compiler may emit ORRS or ORR+TST
 * =================================================================== */

NOINLINE int ccop_logic_orr_eq_32(u32 a, u32 b) {
    volatile u32 r = a | b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_orr_ne_32(u32 a, u32 b) {
    volatile u32 r = a | b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_orr_eq_64(u64 a, u64 b) {
    volatile u64 r = a | b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * EOR (XOR) result tests
 * =================================================================== */

NOINLINE int ccop_logic_eor_eq_32(u32 a, u32 b) {
    volatile u32 r = a ^ b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_logic_eor_ne_32(u32 a, u32 b) {
    volatile u32 r = a ^ b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#ifdef __aarch64__
NOINLINE int ccop_logic_eor_eq_64(u64 a, u64 b) {
    volatile u64 r = a ^ b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}
#endif

/* ===================================================================
 * ARM32-only: HS/LO with shifter carry out
 *
 * On ARM32, the LOGIC C flag comes from the barrel shifter carry out.
 * A shifted operand in TST/AND can produce a non-zero C flag.
 * This is unique to ARM32 (dep2 = shifter_carry_out).
 *
 * We use inline asm to force TST with a shifted operand so the
 * carry out is meaningful.
 * =================================================================== */

#ifndef __aarch64__
/* TST with LSL -- carry out is the last bit shifted out */
NOINLINE int ccop_logic_shift_hs_32(u32 a, u32 b) {
    int result;
    /*
     * "tst a, b, lsl #1" -- the barrel shifter produces a carry out
     * from bit 31 of b. Then BHS checks C=1.
     */
    asm volatile (
        "tst %[a], %[b], lsl #1\n\t"
        "ite cs\n\t"
        "movcs %[out], #1\n\t"
        "movcc %[out], #0"
        : [out] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "cc"
    );
    g_sink = result;
    return g_sink;
}

NOINLINE int ccop_logic_shift_lo_32(u32 a, u32 b) {
    int result;
    asm volatile (
        "tst %[a], %[b], lsl #1\n\t"
        "ite cc\n\t"
        "movcc %[out], #1\n\t"
        "movcs %[out], #0"
        : [out] "=r"(result)
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

    /* EQ / NE with two-operand AND */
    ccop_logic_eq_32(0xAA, 0x55);
    ccop_logic_ne_32(0xFF, 0x01);
    ccop_logic_ne_self_32(42);

    /* MI / PL (sign bit) */
    ccop_logic_mi_32(-1);
    ccop_logic_pl_32(1);

    /* GE / LT (N=V, V=0 for logic) */
    ccop_logic_ge_32(5);
    ccop_logic_lt_32(-5);

    /* GT / LE */
    ccop_logic_gt_32(5);
    ccop_logic_le_32(-1);

    /* AND mask patterns */
    ccop_logic_and_eq_32(0xF0, 0x0F);
    ccop_logic_and_ne_32(0xFF, 0x0F);
    ccop_logic_and_mi_32(-1, -1);

    /* ORR patterns */
    ccop_logic_orr_eq_32(0, 0);
    ccop_logic_orr_ne_32(0, 1);

    /* EOR patterns */
    ccop_logic_eor_eq_32(42, 42);
    ccop_logic_eor_ne_32(42, 0);

#ifndef __aarch64__
    /* ARM32 shifter carry out */
    ccop_logic_shift_hs_32(0xFFFFFFFF, 0x80000000);
    ccop_logic_shift_lo_32(0xFFFFFFFF, 0x7FFFFFFF);
#endif

#ifdef __aarch64__
    /* 64-bit variants */
    ccop_logic_eq_64(0xAA, 0x55);
    ccop_logic_ne_64(0xFF, 0x01);
    ccop_logic_ne_self_64(42);
    ccop_logic_mi_64(-1);
    ccop_logic_pl_64(1);
    ccop_logic_ge_64(5);
    ccop_logic_lt_64(-5);
    ccop_logic_gt_64(5);
    ccop_logic_le_64(-1);
    ccop_logic_and_eq_64(0xF0, 0x0F);
    ccop_logic_and_ne_64(0xFF, 0x0F);
    ccop_logic_and_mi_64(-1, -1);
    ccop_logic_orr_eq_64(0, 0);
    ccop_logic_eor_eq_64(42, 42);
#endif

    return g_sink;
}
