/*
 * ccop_copy.c — COPY condition code triggers
 *
 * VEX models pushf/popf (flag save/restore) as CC_OP_COPY.
 * Strategy: CMP sets flags -> pushf saves -> trash flags -> popf restores
 * -> branch on restored flags exercises CC_OP_COPY + condition.
 */

#include "ccop_common.h"

volatile int g_sink;

#ifdef __x86_64__

/* COPY + CondZ (amd64): cmp sets ZF, pushfq/popfq preserves it */
NOINLINE int ccop_copy_condz_64(s64 a, s64 b) {
    asm goto (
        "cmpq %[b], %[a]\n\t"
        "pushfq\n\t"
        "testq $1, %[a]\n\t"
        "popfq\n\t"
        "je %l[taken]"
        : : [a] "r"(a), [b] "r"(b) : "cc", "memory" : taken
    );
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* COPY + CondNZ (amd64) */
NOINLINE int ccop_copy_condnz_64(s64 a, s64 b) {
    asm goto (
        "cmpq %[b], %[a]\n\t"
        "pushfq\n\t"
        "testq $1, %[a]\n\t"
        "popfq\n\t"
        "jne %l[taken]"
        : : [a] "r"(a), [b] "r"(b) : "cc", "memory" : taken
    );
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

#else /* i386 */

/* COPY + CondZ (i386): cmpl + pushfl/popfl */
NOINLINE int ccop_copy_condz_32(s32 a, s32 b) {
    asm goto (
        "cmpl %[b], %[a]\n\t"
        "pushfl\n\t"
        "testl $1, %[a]\n\t"
        "popfl\n\t"
        "je %l[taken]"
        : : [a] "r"(a), [b] "r"(b) : "cc", "memory" : taken
    );
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

/* COPY + CondNZ (i386) */
NOINLINE int ccop_copy_condnz_32(s32 a, s32 b) {
    asm goto (
        "cmpl %[b], %[a]\n\t"
        "pushfl\n\t"
        "testl $1, %[a]\n\t"
        "popfl\n\t"
        "jne %l[taken]"
        : : [a] "r"(a), [b] "r"(b) : "cc", "memory" : taken
    );
    g_sink = 0; return g_sink;
taken: g_sink = 1; return g_sink;
}

#endif

int main(void) {
    g_sink = 0;

#ifdef __x86_64__
    ccop_copy_condz_64(5, 5);
    ccop_copy_condnz_64(5, 3);
#else
    ccop_copy_condz_32(5, 5);
    ccop_copy_condnz_32(5, 3);
#endif

    return g_sink;
}
