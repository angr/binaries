/*
 * ccop_adc_sbb.c — ADC/SBB condition code triggers (pure C)
 *
 * VEX models add-with-carry as CC_OP_ADCx and subtract-with-borrow as
 * CC_OP_SBBx. These arise in multi-precision arithmetic.
 *
 * Strategy:
 *   - amd64: __int128 addition/subtraction compiles to add+adc / sub+sbb
 *   - i386:  uint64_t addition/subtraction compiles to add+adc / sub+sbb
 *
 * Branching on the wide result after the operation triggers condition
 * code evaluation on the ADC/SBB instruction's flags.
 */

#include "ccop_common.h"

volatile int g_sink;

#ifdef __x86_64__

/*
 * On amd64, __int128 is natively supported. The compiler breaks it into
 * two 64-bit operations: add+adc for addition, sub+sbb for subtraction.
 * The flags from the adc/sbb instruction become CC_OP_ADCQ / CC_OP_SBBQ.
 */

typedef unsigned __int128 u128;
typedef __int128 s128;

/* ═══════════════════════════════════════════════════════════════════
 * ADC — from __int128 addition
 * ═══════════════════════════════════════════════════════════════════ */

/* ADC + CondZ: 128-bit add result == 0 */
NOINLINE int ccop_adc_condz_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondNZ: 128-bit add result != 0 */
NOINLINE int ccop_adc_condnz_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondS: 128-bit signed add result < 0 */
NOINLINE int ccop_adc_conds_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    s128 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondB: 128-bit unsigned add carry (a + b < a) */
NOINLINE int ccop_adc_condb_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════
 * SBB — from __int128 subtraction
 * ═══════════════════════════════════════════════════════════════════ */

/* SBB + CondZ: 128-bit sub result == 0 */
NOINLINE int ccop_sbb_condz_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    u128 r = a - b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondB: 128-bit unsigned a < b */
NOINLINE int ccop_sbb_condb_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondNB: 128-bit unsigned a >= b */
NOINLINE int ccop_sbb_condnb_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondBE: 128-bit unsigned a <= b */
NOINLINE int ccop_sbb_condbe_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondNBE: 128-bit unsigned a > b */
NOINLINE int ccop_sbb_condnbe_64(u64 a_lo, u64 a_hi, u64 b_lo, u64 b_hi) {
    u128 a = ((u128)a_hi << 64) | a_lo;
    u128 b = ((u128)b_hi << 64) | b_lo;
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondL: 128-bit signed a < b */
NOINLINE int ccop_sbb_condl_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondS: 128-bit signed sub result sign */
NOINLINE int ccop_sbb_conds_64(s64 a_lo, s64 a_hi, s64 b_lo, s64 b_hi) {
    s128 a = ((s128)a_hi << 64) | (u64)a_lo;
    s128 b = ((s128)b_hi << 64) | (u64)b_lo;
    s128 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#else /* i386 */

/*
 * On i386, uint64_t / int64_t are handled as pairs of 32-bit registers.
 * Addition compiles to add+adc, subtraction to sub+sbb.
 * Flags from the adc/sbb become CC_OP_ADCL / CC_OP_SBBL.
 */

/* ── Helper macros for narrow asm goto ADC/SBB ────────────────────── */

#define ADC_ASM_GOTO_8(fname, jcc) \
NOINLINE int fname(u8 a_lo, u8 a_hi, u8 b_lo, u8 b_hi) { \
    u8 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "addb %[blo], %[lo]\n\t" \
        "adcb %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+q"(lo), [hi] "+q"(hi) \
        : [blo] "q"(b_lo), [bhi] "q"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

#define ADC_ASM_GOTO_16(fname, jcc) \
NOINLINE int fname(u16 a_lo, u16 a_hi, u16 b_lo, u16 b_hi) { \
    u16 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "addw %[blo], %[lo]\n\t" \
        "adcw %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+r"(lo), [hi] "+r"(hi) \
        : [blo] "r"(b_lo), [bhi] "r"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

#define ADC_ASM_GOTO_32(fname, jcc) \
NOINLINE int fname(u32 a_lo, u32 a_hi, u32 b_lo, u32 b_hi) { \
    u32 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "addl %[blo], %[lo]\n\t" \
        "adcl %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+r"(lo), [hi] "+r"(hi) \
        : [blo] "r"(b_lo), [bhi] "r"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

#define SBB_ASM_GOTO_8(fname, jcc) \
NOINLINE int fname(u8 a_lo, u8 a_hi, u8 b_lo, u8 b_hi) { \
    u8 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "subb %[blo], %[lo]\n\t" \
        "sbbb %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+q"(lo), [hi] "+q"(hi) \
        : [blo] "q"(b_lo), [bhi] "q"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

#define SBB_ASM_GOTO_16(fname, jcc) \
NOINLINE int fname(u16 a_lo, u16 a_hi, u16 b_lo, u16 b_hi) { \
    u16 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "subw %[blo], %[lo]\n\t" \
        "sbbw %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+r"(lo), [hi] "+r"(hi) \
        : [blo] "r"(b_lo), [bhi] "r"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

#define SBB_ASM_GOTO_32(fname, jcc) \
NOINLINE int fname(u32 a_lo, u32 a_hi, u32 b_lo, u32 b_hi) { \
    u32 lo = a_lo, hi = a_hi; \
    asm goto ( \
        "subl %[blo], %[lo]\n\t" \
        "sbbl %[bhi], %[hi]\n\t" \
        jcc " %l[taken]" \
        : [lo] "+r"(lo), [hi] "+r"(hi) \
        : [blo] "r"(b_lo), [bhi] "r"(b_hi) \
        : "cc" : taken); \
    g_sink = 0; return g_sink; \
taken: g_sink = 1; return g_sink; \
}

/* ═══════════════════════════════════════════════════════════════════
 * ADC — from uint64_t addition on i386 (pure C, 32-bit pair)
 * ═══════════════════════════════════════════════════════════════════ */

/* ADC + CondZ: 64-bit add result == 0 */
NOINLINE int ccop_adc_condz_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondNZ: 64-bit add result != 0 */
NOINLINE int ccop_adc_condnz_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondS: 64-bit signed add < 0 */
NOINLINE int ccop_adc_conds_32(s64 a, s64 b) {
    s64 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ADC + CondB: 64-bit unsigned add carry */
NOINLINE int ccop_adc_condb_32(u64 a, u64 b) {
    u64 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════
 * SBB — from uint64_t subtraction on i386 (pure C, 32-bit pair)
 * ═══════════════════════════════════════════════════════════════════ */

/* SBB + CondZ: 64-bit sub result == 0 */
NOINLINE int ccop_sbb_condz_32(u64 a, u64 b) {
    u64 r = a - b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondB: 64-bit unsigned a < b */
NOINLINE int ccop_sbb_condb_32(u64 a, u64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondNB: 64-bit unsigned a >= b */
NOINLINE int ccop_sbb_condnb_32(u64 a, u64 b) {
    if (a >= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondBE: 64-bit unsigned a <= b */
NOINLINE int ccop_sbb_condbe_32(u64 a, u64 b) {
    if (a <= b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondNBE: 64-bit unsigned a > b */
NOINLINE int ccop_sbb_condnbe_32(u64 a, u64 b) {
    if (a > b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondL: 64-bit signed a < b */
NOINLINE int ccop_sbb_condl_32(s64 a, s64 b) {
    if (a < b) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* SBB + CondS: 64-bit signed sub result sign */
NOINLINE int ccop_sbb_conds_32(s64 a, s64 b) {
    s64 r = a - b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════
 * P0: New conditions at 32-bit (asm goto)
 * ═══════════════════════════════════════════════════════════════════ */

ADC_ASM_GOTO_32(ccop_adc_condo_32, "jo")
ADC_ASM_GOTO_32(ccop_adc_condno_32, "jno")
SBB_ASM_GOTO_32(ccop_sbb_condo_32, "jo")
SBB_ASM_GOTO_32(ccop_sbb_condno_32, "jno")
SBB_ASM_GOTO_32(ccop_sbb_condnl_32, "jge")

/* ═══════════════════════════════════════════════════════════════════
 * P1: Narrow ADC 8-bit and 16-bit (asm goto)
 * ═══════════════════════════════════════════════════════════════════ */

ADC_ASM_GOTO_8(ccop_adc_condz_8, "je")
ADC_ASM_GOTO_16(ccop_adc_condz_16, "je")
ADC_ASM_GOTO_8(ccop_adc_condnz_8, "jne")
ADC_ASM_GOTO_16(ccop_adc_condnz_16, "jne")
ADC_ASM_GOTO_8(ccop_adc_conds_8, "js")
ADC_ASM_GOTO_16(ccop_adc_conds_16, "js")
ADC_ASM_GOTO_8(ccop_adc_condb_8, "jb")
ADC_ASM_GOTO_16(ccop_adc_condb_16, "jb")
ADC_ASM_GOTO_8(ccop_adc_condo_8, "jo")
ADC_ASM_GOTO_16(ccop_adc_condo_16, "jo")
ADC_ASM_GOTO_8(ccop_adc_condno_8, "jno")
ADC_ASM_GOTO_16(ccop_adc_condno_16, "jno")

/* ═══════════════════════════════════════════════════════════════════
 * P1: Narrow SBB 8-bit and 16-bit (asm goto)
 * ═══════════════════════════════════════════════════════════════════ */

SBB_ASM_GOTO_8(ccop_sbb_condz_8, "je")
SBB_ASM_GOTO_16(ccop_sbb_condz_16, "je")
SBB_ASM_GOTO_8(ccop_sbb_condb_8, "jb")
SBB_ASM_GOTO_16(ccop_sbb_condb_16, "jb")
SBB_ASM_GOTO_8(ccop_sbb_condnb_8, "jae")
SBB_ASM_GOTO_16(ccop_sbb_condnb_16, "jae")
SBB_ASM_GOTO_8(ccop_sbb_condbe_8, "jbe")
SBB_ASM_GOTO_16(ccop_sbb_condbe_16, "jbe")
SBB_ASM_GOTO_8(ccop_sbb_condnbe_8, "ja")
SBB_ASM_GOTO_16(ccop_sbb_condnbe_16, "ja")
SBB_ASM_GOTO_8(ccop_sbb_condl_8, "jl")
SBB_ASM_GOTO_16(ccop_sbb_condl_16, "jl")
SBB_ASM_GOTO_8(ccop_sbb_conds_8, "js")
SBB_ASM_GOTO_16(ccop_sbb_conds_16, "js")
SBB_ASM_GOTO_8(ccop_sbb_condo_8, "jo")
SBB_ASM_GOTO_16(ccop_sbb_condo_16, "jo")
SBB_ASM_GOTO_8(ccop_sbb_condno_8, "jno")
SBB_ASM_GOTO_16(ccop_sbb_condno_16, "jno")
SBB_ASM_GOTO_8(ccop_sbb_condnl_8, "jge")
SBB_ASM_GOTO_16(ccop_sbb_condnl_16, "jge")

#endif /* __x86_64__ */

int main(void) {
    g_sink = 0;

#ifdef __x86_64__
    ccop_adc_condz_64(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 1, 0);
    ccop_adc_condnz_64(1, 0, 2, 0);
    ccop_adc_conds_64(0, 0x7FFFFFFFFFFFFFFFLL, 0, 1);
    ccop_adc_condb_64(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 1, 0);

    ccop_sbb_condz_64(5, 0, 5, 0);
    ccop_sbb_condb_64(1, 0, 2, 0);
    ccop_sbb_condnb_64(2, 0, 1, 0);
    ccop_sbb_condbe_64(1, 0, 1, 0);
    ccop_sbb_condnbe_64(2, 0, 1, 0);
    ccop_sbb_condl_64(0, -1, 0, 1);
    ccop_sbb_conds_64(0, 0, 0, 1);
#else
    /* Existing pure C 32-bit pair calls */
    ccop_adc_condz_32(0xFFFFFFFFFFFFFFFFULL, 1);
    ccop_adc_condnz_32(1, 2);
    ccop_adc_conds_32(0x7FFFFFFFFFFFFFFFLL, 1);
    ccop_adc_condb_32(0xFFFFFFFFFFFFFFFFULL, 1);

    ccop_sbb_condz_32(5, 5);
    ccop_sbb_condb_32(1, 2);
    ccop_sbb_condnb_32(2, 1);
    ccop_sbb_condbe_32(1, 1);
    ccop_sbb_condnbe_32(2, 1);
    ccop_sbb_condl_32(-1, 1);
    ccop_sbb_conds_32(0, 1);

    /* P0: New 32-bit asm goto conditions */
    ccop_adc_condo_32(0, 0x7FFFFFFF, 0, 1);
    ccop_adc_condno_32(1, 0, 2, 0);
    ccop_sbb_condo_32(0, 0x80000000U, 0, 1);
    ccop_sbb_condno_32(0, 0, 0, 0);
    ccop_sbb_condnl_32(0, 2, 0, 1);

    /* P1: Narrow ADC 8-bit */
    ccop_adc_condz_8(0xFF, 0xFF, 1, 0);
    ccop_adc_condnz_8(1, 0, 2, 0);
    ccop_adc_conds_8(0, 0x7F, 0, 1);
    ccop_adc_condb_8(0, 0xFF, 0, 1);
    ccop_adc_condo_8(0, 0x7F, 0, 1);
    ccop_adc_condno_8(1, 0, 2, 0);

    /* P1: Narrow ADC 16-bit */
    ccop_adc_condz_16(0xFFFF, 0xFFFF, 1, 0);
    ccop_adc_condnz_16(1, 0, 2, 0);
    ccop_adc_conds_16(0, 0x7FFF, 0, 1);
    ccop_adc_condb_16(0, 0xFFFF, 0, 1);
    ccop_adc_condo_16(0, 0x7F, 0, 1);
    ccop_adc_condno_16(1, 0, 2, 0);

    /* P1: Narrow SBB 8-bit */
    ccop_sbb_condz_8(5, 0, 5, 0);
    ccop_sbb_condb_8(0, 1, 0, 2);
    ccop_sbb_condnb_8(0, 2, 0, 1);
    ccop_sbb_condbe_8(0, 1, 0, 1);
    ccop_sbb_condnbe_8(0, 2, 0, 1);
    ccop_sbb_condl_8(0, (u8)-1, 0, 1);
    ccop_sbb_conds_8(0, 0, 0, 1);
    ccop_sbb_condo_8(0, 0x80, 0, 1);
    ccop_sbb_condno_8(0, 0, 0, 0);
    ccop_sbb_condnl_8(0, 2, 0, 1);

    /* P1: Narrow SBB 16-bit */
    ccop_sbb_condz_16(5, 0, 5, 0);
    ccop_sbb_condb_16(0, 1, 0, 2);
    ccop_sbb_condnb_16(0, 2, 0, 1);
    ccop_sbb_condbe_16(0, 1, 0, 1);
    ccop_sbb_condnbe_16(0, 2, 0, 1);
    ccop_sbb_condl_16(0, (u16)-1, 0, 1);
    ccop_sbb_conds_16(0, 0, 0, 1);
    ccop_sbb_condo_16(0, 0x80, 0, 1);
    ccop_sbb_condno_16(0, 0, 0, 0);
    ccop_sbb_condnl_16(0, 2, 0, 1);
#endif

    return g_sink;
}
