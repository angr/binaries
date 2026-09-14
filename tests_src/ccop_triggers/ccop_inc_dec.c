/*
 * ccop_inc_dec.c — INC/DEC condition code triggers (pure C + asm goto)
 *
 * Modern GCC with -mtune=generic prefers "add $1"/"sub $1" over inc/dec
 * because of partial-flag stalls on older microarchitectures. To get real
 * INC/DEC instructions, compile with:
 *   -mtune-ctrl=use_incdec   (explicit GCC tuning override)
 *   -mtune=haswell            (Haswell+ re-enables inc/dec)
 *
 * VEX models these as CC_OP_INCx / CC_OP_DECx (distinct from ADD/SUB
 * because INC/DEC preserve CF via NDEP).
 *
 * We use volatile locals to prevent the compiler from folding the +1/-1
 * into the comparison (e.g. turning "a+1 == 0" into "a == -1" which
 * would emit CMP instead of INC).
 */

#include "ccop_common.h"

volatile int g_sink;

/* ═══════════════════════════════════════════════════════════════════
 * INC — triggered by (var + 1) or var++ with inc/dec tuning enabled
 *
 * Using volatile forces the compiler to actually perform the increment
 * in a register and then test the result, rather than constant-folding.
 * ═══════════════════════════════════════════════════════════════════ */

/* ── INC + CondZ ──────────────────────────────────────────────────── */

NOINLINE int ccop_inc_condz_8(s8 a) {
    volatile s8 v = a;
    v++;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condz_16(s16 a) {
    volatile s16 v = a;
    v++;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condz_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condz_64(s64 a) {
    volatile s64 v = a;
    v++;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondNZ ─────────────────────────────────────────────────── */

NOINLINE int ccop_inc_condnz_8(s8 a) {
    volatile s8 v = a;
    v++;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condnz_16(s16 a) {
    volatile s16 v = a;
    v++;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condnz_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condnz_64(s64 a) {
    volatile s64 v = a;
    v++;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondS ──────────────────────────────────────────────────── */

NOINLINE int ccop_inc_conds_8(s8 a) {
    volatile s8 v = a;
    v++;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_conds_16(s16 a) {
    volatile s16 v = a;
    v++;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_conds_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_conds_64(s64 a) {
    volatile s64 v = a;
    v++;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondNS ─────────────────────────────────────────────────── */

NOINLINE int ccop_inc_condns_8(s8 a) {
    volatile s8 v = a;
    v++;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condns_16(s16 a) {
    volatile s16 v = a;
    v++;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condns_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_inc_condns_64(s64 a) {
    volatile s64 v = a;
    v++;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondLE ─────────────────────────────────────────────────── */

NOINLINE int ccop_inc_condle_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v <= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondNLE ────────────────────────────────────────────────── */

NOINLINE int ccop_inc_condnle_32(s32 a) {
    volatile s32 v = a;
    v++;
    if (v > 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── INC + CondO (asm goto — x86 rewriter only) ──────────────────── */

NOINLINE int ccop_inc_condo_8(s8 a) {
    s8 tmp = a;
    asm goto ("incb %0\n\t" "jo %l[taken]" : "+q"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

NOINLINE int ccop_inc_condo_16(s16 a) {
    s16 tmp = a;
    asm goto ("incw %0\n\t" "jo %l[taken]" : "+r"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

NOINLINE int ccop_inc_condo_32(s32 a) {
    s32 tmp = a;
    asm goto ("incl %0\n\t" "jo %l[taken]" : "+r"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── INC + CondNO (asm goto — x86 rewriter only) ─────────────────── */

NOINLINE int ccop_inc_condno_8(s8 a) {
    s8 tmp = a;
    asm goto ("incb %0\n\t" "jno %l[taken]" : "+q"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

NOINLINE int ccop_inc_condno_16(s16 a) {
    s16 tmp = a;
    asm goto ("incw %0\n\t" "jno %l[taken]" : "+r"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

NOINLINE int ccop_inc_condno_32(s32 a) {
    s32 tmp = a;
    asm goto ("incl %0\n\t" "jno %l[taken]" : "+r"(tmp) : : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════
 * DEC — triggered by (var - 1) or var-- with inc/dec tuning enabled
 * ═══════════════════════════════════════════════════════════════════ */

/* ── DEC + CondZ ──────────────────────────────────────────────────── */

NOINLINE int ccop_dec_condz_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condz_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condz_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condz_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── DEC + CondNZ ─────────────────────────────────────────────────── */

NOINLINE int ccop_dec_condnz_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnz_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnz_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnz_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── DEC + CondS ──────────────────────────────────────────────────── */

NOINLINE int ccop_dec_conds_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_conds_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_conds_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_conds_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── DEC + CondNS ─────────────────────────────────────────────────── */

NOINLINE int ccop_dec_condns_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condns_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condns_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condns_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── DEC + CondLE ─────────────────────────────────────────────────── */

NOINLINE int ccop_dec_condle_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v <= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condle_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v <= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condle_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v <= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condle_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v <= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── DEC + CondNLE ────────────────────────────────────────────────── */

NOINLINE int ccop_dec_condnle_8(s8 a) {
    volatile s8 v = a;
    v--;
    if (v > 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnle_16(s16 a) {
    volatile s16 v = a;
    v--;
    if (v > 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnle_32(s32 a) {
    volatile s32 v = a;
    v--;
    if (v > 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_dec_condnle_64(s64 a) {
    volatile s64 v = a;
    v--;
    if (v > 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

int main(void) {
    g_sink = 0;

    /* INC + CondZ */
    ccop_inc_condz_8(-1);
    ccop_inc_condz_16(-1);
    ccop_inc_condz_32(-1);
    ccop_inc_condz_64(-1);

    /* INC + CondNZ */
    ccop_inc_condnz_8(5);
    ccop_inc_condnz_16(5);
    ccop_inc_condnz_32(5);
    ccop_inc_condnz_64(5);

    /* INC + CondS */
    ccop_inc_conds_8(127);
    ccop_inc_conds_16(0x7FFF);
    ccop_inc_conds_32(0x7FFFFFFF);
    ccop_inc_conds_64(0x7FFFFFFFFFFFFFFFLL);

    /* INC + CondNS */
    ccop_inc_condns_8(0);
    ccop_inc_condns_16(0);
    ccop_inc_condns_32(0);
    ccop_inc_condns_64(0);

    /* INC + CondLE */
    ccop_inc_condle_32(-2);

    /* INC + CondNLE */
    ccop_inc_condnle_32(5);

    /* INC + CondO (asm goto) */
    ccop_inc_condo_8(127);
    ccop_inc_condo_16(0x7FFF);
    ccop_inc_condo_32(0x7FFFFFFF);

    /* INC + CondNO (asm goto) */
    ccop_inc_condno_8(0);
    ccop_inc_condno_16(0);
    ccop_inc_condno_32(0);

    /* DEC + CondZ */
    ccop_dec_condz_8(1);
    ccop_dec_condz_16(1);
    ccop_dec_condz_32(1);
    ccop_dec_condz_64(1);

    /* DEC + CondNZ */
    ccop_dec_condnz_8(5);
    ccop_dec_condnz_16(5);
    ccop_dec_condnz_32(5);
    ccop_dec_condnz_64(5);

    /* DEC + CondS */
    ccop_dec_conds_8(0);
    ccop_dec_conds_16(0);
    ccop_dec_conds_32(0);
    ccop_dec_conds_64(0);

    /* DEC + CondNS */
    ccop_dec_condns_8(1);
    ccop_dec_condns_16(1);
    ccop_dec_condns_32(1);
    ccop_dec_condns_64(1);

    /* DEC + CondLE */
    ccop_dec_condle_8(1);
    ccop_dec_condle_16(1);
    ccop_dec_condle_32(1);
    ccop_dec_condle_64(1);

    /* DEC + CondNLE */
    ccop_dec_condnle_8(5);
    ccop_dec_condnle_16(5);
    ccop_dec_condnle_32(5);
    ccop_dec_condnle_64(5);

    return g_sink;
}
