/*
 * ccop_logic.c — LOGIC (AND/OR/XOR/TEST) condition code triggers
 *
 * VEX models flag-setting from bitwise operations as CC_OP_LOGICx.
 * The TEST instruction (AND without storing result) is the most common
 * trigger. SF and ZF are meaningful; OF and CF are always 0.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ═══════════════════════════════════════════════════════════════════════
 * CondZ: (a & b) == 0
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondZ 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condz_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    if ((va & vb) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condz_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    if ((va & vb) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condz_32(u32 a, u32 b) {
    if ((a & b) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condz_64(u64 a, u64 b) {
    if ((a & b) == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNZ: test reg,reg (if (a))
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNZ 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnz_8(s8 a) {
    volatile s8 va = a;
    if (va) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnz_16(s16 a) {
    volatile s16 va = a;
    if (va) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnz_32(s32 a) {
    /* "test eax, eax; jnz" — LOGIC + CondNZ */
    if (a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnz_64(s64 a) {
    if (a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondS: sign flag via test (a < 0)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondS 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_conds_8(s8 a) {
    volatile s8 va = a;
    if (va < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_conds_16(s16 a) {
    volatile s16 va = a;
    if (va < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_conds_32(s32 a) {
    /* "test eax, eax; js" — LOGIC + CondS */
    if (a < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_conds_64(s64 a) {
    if (a < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNS: not sign via test (a >= 0)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNS 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condns_8(s8 a) {
    volatile s8 va = a;
    if (va >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condns_16(s16 a) {
    volatile s16 va = a;
    if (va >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condns_32(s32 a) {
    if (a >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condns_64(s64 a) {
    if (a >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/*
 * CondL/NL/LE/NLE + LOGIC: The rewriter handles these when dep_2 == 0.
 * This happens with "test reg,reg" where the compiler uses signed
 * comparison conditions on the result.
 */

/* ═══════════════════════════════════════════════════════════════════════
 * CondL: a < 0 (test reg,reg + jl)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondL 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condl_8(s8 a) {
    volatile s8 va = a;
    volatile s8 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondL 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condl_16(s16 a) {
    volatile s16 va = a;
    volatile s16 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondL 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condl_32(s32 a) {
    /*
     * Some compilers emit "test eax,eax; jl" instead of "js"
     * for signed-negative checks. Both trigger LOGIC ccop.
     * Force via volatile + explicit signed comparison.
     */
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondL 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condl_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va < zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNL: a >= 0 (test reg,reg + jge)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNL 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnl_8(s8 a) {
    volatile s8 va = a;
    volatile s8 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNL 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnl_16(s16 a) {
    volatile s16 va = a;
    volatile s16 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNL 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnl_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNL 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnl_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va >= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondLE: a <= 0 (test reg,reg + jle)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondLE 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condle_8(s8 a) {
    volatile s8 va = a;
    volatile s8 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondLE 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condle_16(s16 a) {
    volatile s16 va = a;
    volatile s16 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondLE 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condle_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondLE 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condle_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va <= zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNLE: a > 0 (test reg,reg + jg)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNLE 8 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnle_8(s8 a) {
    volatile s8 va = a;
    volatile s8 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNLE 16 ───────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnle_16(s16 a) {
    volatile s16 va = a;
    volatile s16 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNLE 32 ───────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnle_32(s32 a) {
    volatile s32 va = a;
    volatile s32 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNLE 64 ───────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condnle_64(s64 a) {
    volatile s64 va = a;
    volatile s64 zero = 0;
    if (va > zero) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondB: test + jb — x86 rewriter path (CF is always 0 for LOGIC)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondB 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condb_8(u8 a, u8 b) {
    asm goto ("testb %0, %1\n\t" "jb %l[taken]" : : "q"(a), "q"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondB 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condb_16(u16 a, u16 b) {
    asm goto ("testw %0, %1\n\t" "jb %l[taken]" : : "r"(a), "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondB 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condb_32(u32 a, u32 b) {
    asm goto ("testl %0, %1\n\t" "jb %l[taken]" : : "r"(a), "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondBE: test + jbe — x86 rewriter path (CF is always 0 for LOGIC)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondBE 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condbe_8(u8 a, u8 b) {
    asm goto ("testb %0, %1\n\t" "jbe %l[taken]" : : "q"(a), "q"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondBE 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condbe_16(u16 a, u16 b) {
    asm goto ("testw %0, %1\n\t" "jbe %l[taken]" : : "r"(a), "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondBE 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_logic_condbe_32(u32 a, u32 b) {
    asm goto ("testl %0, %1\n\t" "jbe %l[taken]" : : "r"(a), "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════ */

int main(void) {
    g_sink = 0;

    ccop_logic_condz_8(0xAA, 0x55);
    ccop_logic_condz_16(0xAA00, 0x5500);
    ccop_logic_condz_32(0xAA, 0x55);
    ccop_logic_condz_64(0xAA, 0x55);

    ccop_logic_condnz_8(42);
    ccop_logic_condnz_16(42);
    ccop_logic_condnz_32(42);
    ccop_logic_condnz_64(42);

    ccop_logic_conds_8(-1);
    ccop_logic_conds_16(-1);
    ccop_logic_conds_32(-1);
    ccop_logic_conds_64(-1);

    ccop_logic_condns_8(1);
    ccop_logic_condns_16(1);
    ccop_logic_condns_32(1);
    ccop_logic_condns_64(1);

    ccop_logic_condl_8(-5);
    ccop_logic_condl_16(-5);
    ccop_logic_condl_32(-5);
    ccop_logic_condl_64(-5);

    ccop_logic_condnl_8(5);
    ccop_logic_condnl_16(5);
    ccop_logic_condnl_32(5);
    ccop_logic_condnl_64(5);

    ccop_logic_condle_8(-1);
    ccop_logic_condle_16(-1);
    ccop_logic_condle_32(-1);
    ccop_logic_condle_64(-1);

    ccop_logic_condnle_8(5);
    ccop_logic_condnle_16(5);
    ccop_logic_condnle_32(5);
    ccop_logic_condnle_64(5);

    ccop_logic_condb_8(0xAA, 0x55);
    ccop_logic_condb_16(0xAA00, 0x5500);
    ccop_logic_condb_32(0xAA, 0x55);

    ccop_logic_condbe_8(0xAA, 0x55);
    ccop_logic_condbe_16(0xAA00, 0x5500);
    ccop_logic_condbe_32(0xAA, 0x55);

    return g_sink;
}
