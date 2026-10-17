/*
 * ccop_rflags_c.c — rflags_c (carry flag extraction) triggers
 *
 * The amd64 rewriter handles amd64g_calculate_rflags_c for ADD, SUB, DEC.
 * This callee extracts CF independently from the full flags calculation.
 *
 * Pattern: operation sets CF -> adc $0,reg captures CF into a register.
 * For DEC (which preserves CF via NDEP): stc -> dec -> adc $0 captures
 * the preserved carry.
 *
 * amd64-only (rflags_c is not used by the x86/i386 rewriter).
 */

#include "ccop_common.h"

volatile int g_sink;

#ifdef __x86_64__

/* ===================================================================
 * rflags_c + ADD: carry capture via adc $0 after add
 * =================================================================== */

NOINLINE int ccop_rflagsc_add_32(u32 a, u32 b) {
    u32 carry = 0;
    asm volatile (
        "addl %[b], %[a]\n\t"
        "adcl $0, %[c]"
        : [a] "+r"(a), [c] "+r"(carry)
        : [b] "r"(b)
        : "cc"
    );
    if (carry) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_rflagsc_add_64(u64 a, u64 b) {
    u64 carry = 0;
    asm volatile (
        "addq %[b], %[a]\n\t"
        "adcq $0, %[c]"
        : [a] "+r"(a), [c] "+r"(carry)
        : [b] "r"(b)
        : "cc"
    );
    if (carry) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ===================================================================
 * rflags_c + SUB: borrow capture via sbb $0 after sub
 * =================================================================== */

NOINLINE int ccop_rflagsc_sub_32(u32 a, u32 b) {
    u32 borrow = 0;
    asm volatile (
        "subl %[b], %[a]\n\t"
        "sbbl $0, %[c]"
        : [a] "+r"(a), [c] "+r"(borrow)
        : [b] "r"(b)
        : "cc"
    );
    if (borrow) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_rflagsc_sub_64(u64 a, u64 b) {
    u64 borrow = 0;
    asm volatile (
        "subq %[b], %[a]\n\t"
        "sbbq $0, %[c]"
        : [a] "+r"(a), [c] "+r"(borrow)
        : [b] "r"(b)
        : "cc"
    );
    if (borrow) g_sink = 1; else g_sink = 0;
    return g_sink;
}

/* ===================================================================
 * rflags_c + DEC: DEC preserves CF (via NDEP), capture after dec
 * =================================================================== */

NOINLINE int ccop_rflagsc_dec_32(u32 a) {
    u32 carry = 0;
    asm volatile (
        "stc\n\t"
        "decl %[a]\n\t"
        "adcl $0, %[c]"
        : [a] "+r"(a), [c] "+r"(carry)
        : : "cc"
    );
    if (carry) g_sink = 1; else g_sink = 0;
    return g_sink;
}

NOINLINE int ccop_rflagsc_dec_64(u64 a) {
    u64 carry = 0;
    asm volatile (
        "stc\n\t"
        "decq %[a]\n\t"
        "adcq $0, %[c]"
        : [a] "+r"(a), [c] "+r"(carry)
        : : "cc"
    );
    if (carry) g_sink = 1; else g_sink = 0;
    return g_sink;
}

#endif /* __x86_64__ */

int main(void) {
    g_sink = 0;

#ifdef __x86_64__
    ccop_rflagsc_add_32(0xFFFFFFFFU, 1);
    ccop_rflagsc_add_64(0xFFFFFFFFFFFFFFFFULL, 1);

    ccop_rflagsc_sub_32(1, 2);
    ccop_rflagsc_sub_64(1, 2);

    ccop_rflagsc_dec_32(5);
    ccop_rflagsc_dec_64(5);
#endif

    return g_sink;
}
