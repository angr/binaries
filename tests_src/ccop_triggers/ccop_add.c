/*
 * ccop_add.c — ADD condition code triggers
 *
 * VEX models ADD flag-setting as CC_OP_ADDx. These functions trigger
 * conditions on the result of addition: zero, sign, overflow.
 */

#include "ccop_common.h"

volatile int g_sink;

/* ═══════════════════════════════════════════════════════════════════════
 * CondZ: (a + b) == 0
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondZ 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condz_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va + vb;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condz_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va + vb;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condz_32(s32 a, s32 b) {
    s32 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondZ 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condz_64(s64 a, s64 b) {
    s64 r = a + b;
    if (r == 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNZ: (a + b) != 0
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNZ 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnz_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va + vb;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnz_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va + vb;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnz_32(s32 a, s32 b) {
    s32 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNZ 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnz_64(s64 a, s64 b) {
    s64 r = a + b;
    if (r != 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondS: sign of (a + b)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondS 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_conds_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va + vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_conds_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va + vb;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_conds_32(s32 a, s32 b) {
    s32 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondS 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_conds_64(s64 a, s64 b) {
    s64 r = a + b;
    if (r < 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNS: not sign of (a + b)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNS 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condns_8(s8 a, s8 b) {
    volatile s8 va = a, vb = b;
    s8 r = va + vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condns_16(s16 a, s16 b) {
    volatile s16 va = a, vb = b;
    s16 r = va + vb;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condns_32(s32 a, s32 b) {
    s32 r = a + b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondNS 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condns_64(s64 a, s64 b) {
    s64 r = a + b;
    if (r >= 0) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondO: signed add overflow
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondO 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condo_8(s8 a, s8 b) {
    s8 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondO 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condo_16(s16 a, s16 b) {
    s16 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondO 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condo_32(s32 a, s32 b) {
    s32 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondO 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condo_64(s64 a, s64 b) {
    s64 r;
    if (__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondNO: no signed add overflow
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNO 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condno_8(s8 a, s8 b) {
    s8 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondNO 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condno_16(s16 a, s16 b) {
    s16 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondNO 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condno_32(s32 a, s32 b) {
    s32 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ── CondNO 64 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condno_64(s64 a, s64 b) {
    s64 r;
    if (!__builtin_add_overflow(a, b, &r))
        g_sink = 1;
    else
        g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondB: unsigned add carry (a + b < a)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondB 8 ──────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condb_8(u8 a, u8 b) {
    volatile u8 va = a, vb = b;
    u8 r = va + vb;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondB 16 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condb_16(u16 a, u16 b) {
    volatile u16 va = a, vb = b;
    u16 r = va + vb;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondB 32 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condb_32(u32 a, u32 b) {
    u32 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ── CondB 64 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condb_64(u64 a, u64 b) {
    u64 r = a + b;
    if (r < a) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════
 * CondLE: signed (a + b) <= 0 — via asm goto (add + jle)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondLE 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condle_8(s8 a, s8 b) {
    s8 tmp = a;
    asm goto ("addb %1, %0\n\t" "jle %l[taken]" : "+q"(tmp) : "q"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondLE 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condle_16(s16 a, s16 b) {
    s16 tmp = a;
    asm goto ("addw %1, %0\n\t" "jle %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondLE 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condle_32(s32 a, s32 b) {
    s32 tmp = a;
    asm goto ("addl %1, %0\n\t" "jle %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondLE 64 ────────────────────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_add_condle_64(s64 a, s64 b) {
    s64 tmp = a;
    asm goto ("addq %1, %0\n\t" "jle %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}
#endif

/* ═══════════════════════════════════════════════════════════════════════
 * CondNLE: signed (a + b) > 0 — via asm goto (add + jg)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondNLE 8 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnle_8(s8 a, s8 b) {
    s8 tmp = a;
    asm goto ("addb %1, %0\n\t" "jg %l[taken]" : "+q"(tmp) : "q"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondNLE 16 ───────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnle_16(s16 a, s16 b) {
    s16 tmp = a;
    asm goto ("addw %1, %0\n\t" "jg %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondNLE 32 ───────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condnle_32(s32 a, s32 b) {
    s32 tmp = a;
    asm goto ("addl %1, %0\n\t" "jg %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondNLE 64 ───────────────────────────────────────────────────────── */

#ifdef __x86_64__
NOINLINE int ccop_add_condnle_64(s64 a, s64 b) {
    s64 tmp = a;
    asm goto ("addq %1, %0\n\t" "jg %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}
#endif

/* ═══════════════════════════════════════════════════════════════════════
 * CondBE: unsigned (a + b) below-or-equal — via asm goto (add + jbe)
 * ═══════════════════════════════════════════════════════════════════════ */

/* ── CondBE 8 ─────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condbe_8(u8 a, u8 b) {
    u8 tmp = a;
    asm goto ("addb %1, %0\n\t" "jbe %l[taken]" : "+q"(tmp) : "q"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondBE 16 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condbe_16(u16 a, u16 b) {
    u16 tmp = a;
    asm goto ("addw %1, %0\n\t" "jbe %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ── CondBE 32 ────────────────────────────────────────────────────────── */

NOINLINE int ccop_add_condbe_32(u32 a, u32 b) {
    u32 tmp = a;
    asm goto ("addl %1, %0\n\t" "jbe %l[taken]" : "+r"(tmp) : "r"(b) : "cc" : taken);
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* ═══════════════════════════════════════════════════════════════════════ */

int main(void) {
    g_sink = 0;

    ccop_add_condz_8(1, -1);
    ccop_add_condz_16(1, -1);
    ccop_add_condz_32(1, -1);
    ccop_add_condz_64(1, -1);

    ccop_add_condnz_8(1, 2);
    ccop_add_condnz_16(1, 2);
    ccop_add_condnz_32(1, 2);
    ccop_add_condnz_64(1, 2);

    ccop_add_conds_8(1, -5);
    ccop_add_conds_16(1, -5);
    ccop_add_conds_32(1, -5);
    ccop_add_conds_64(1, -5);

    ccop_add_condns_8(1, 2);
    ccop_add_condns_16(1, 2);
    ccop_add_condns_32(1, 2);
    ccop_add_condns_64(1, 2);

    ccop_add_condo_8(127, 1);
    ccop_add_condo_16(0x7FFF, 1);
    ccop_add_condo_32(0x7FFFFFFF, 1);
    ccop_add_condo_64(0x7FFFFFFFFFFFFFFFLL, 1);

    ccop_add_condno_8(1, 2);
    ccop_add_condno_16(1, 2);
    ccop_add_condno_32(1, 2);
    ccop_add_condno_64(1, 2);

    ccop_add_condb_8(0xFF, 1);
    ccop_add_condb_16(0xFFFF, 1);
    ccop_add_condb_32(0xFFFFFFFFU, 1);
    ccop_add_condb_64(0xFFFFFFFFFFFFFFFFULL, 1);

    ccop_add_condle_8(-5, 1);
    ccop_add_condle_16(-5, 1);
    ccop_add_condle_32(-5, 1);
#ifdef __x86_64__
    ccop_add_condle_64(-5, 1);
#endif

    ccop_add_condnle_8(5, 1);
    ccop_add_condnle_16(5, 1);
    ccop_add_condnle_32(5, 1);
#ifdef __x86_64__
    ccop_add_condnle_64(5, 1);
#endif

    ccop_add_condbe_8(0x80, 0x80);
    ccop_add_condbe_16(0x8000, 0x8000);
    ccop_add_condbe_32(0x80000000U, 0x80000000U);

    return g_sink;
}
