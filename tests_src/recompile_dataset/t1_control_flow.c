/*
 * t1_control_flow.c -- Control flow pattern triggers.
 *
 * Tests: switch (dense/sparse), loop variants, nested conditionals,
 * early return, short-circuit, break/continue.
 */

#include "harness_common.h"

/* Dense switch: compiler should emit a jump table. */
NOINLINE int t1_switch_dense(s32 a, s32 b) {
    switch (a & 7) {
    case 0: g_sink = b + 1; break;
    case 1: g_sink = b - 1; break;
    case 2: g_sink = b * 2; break;
    case 3: g_sink = b >> 1; break;
    case 4: g_sink = b ^ 0xFF; break;
    case 5: g_sink = b & 0xF0; break;
    case 6: g_sink = b | 0x0F; break;
    case 7: g_sink = ~b; break;
    }
    return g_sink;
}

/* Sparse switch: compiler may emit binary search or if-else chain. */
NOINLINE int t1_switch_sparse(s32 a, s32 b) {
    switch (a) {
    case 1:    g_sink = b + 10; break;
    case 10:   g_sink = b + 20; break;
    case 100:  g_sink = b + 30; break;
    case 1000: g_sink = b + 40; break;
    default:   g_sink = b; break;
    }
    return g_sink;
}

/* Switch with fallthrough. */
NOINLINE int t1_switch_fallthrough(s32 a, s32 b) {
    s32 r = 0;
    switch (a & 3) {
    case 3: r += b;  /* fallthrough */
    case 2: r += b;  /* fallthrough */
    case 1: r += b;  /* fallthrough */
    case 0: r += 1; break;
    }
    g_sink = r;
    return g_sink;
}

/* For loop with accumulator. */
NOINLINE int t1_loop_for(s32 a, s32 b) {
    s32 sum = 0;
    s32 n = a & 15;
    for (s32 i = 0; i < n; i++) {
        sum += i * b;
    }
    g_sink = sum;
    return g_sink;
}

/* While loop. */
NOINLINE int t1_loop_while(s32 a, s32 b) {
    s32 result = a;
    s32 count = b & 15;
    while (count > 0) {
        result = result * 3 + 1;
        count--;
    }
    g_sink = result;
    return g_sink;
}

/* Do-while loop. */
NOINLINE int t1_loop_do_while(s32 a, s32 b) {
    s32 val = a | 1;  /* ensure nonzero start */
    s32 limit = (b & 15) + 1;
    s32 i = 0;
    do {
        val = (val >> 1) ^ ((val & 1) ? 0xB : 0);
        i++;
    } while (i < limit);
    g_sink = val;
    return g_sink;
}

/* Nested loops. */
NOINLINE int t1_loop_nested(s32 a, s32 b) {
    s32 sum = 0;
    s32 na = a & 7;
    s32 nb = b & 7;
    for (s32 i = 0; i < na; i++) {
        for (s32 j = 0; j < nb; j++) {
            sum += i * nb + j;
        }
    }
    g_sink = sum;
    return g_sink;
}

/* If-else chain. */
NOINLINE int t1_if_chain(s32 a, s32 b) {
    if (a < -100)
        g_sink = -b;
    else if (a < 0)
        g_sink = b - a;
    else if (a == 0)
        g_sink = 0;
    else if (a < 100)
        g_sink = b + a;
    else
        g_sink = b * 2;
    return g_sink;
}

/* Deeply nested conditions. */
NOINLINE int t1_if_nested(s32 a, s32 b) {
    if (a > 0) {
        if (b > 0) {
            g_sink = a + b;
        } else {
            if (a > -b)
                g_sink = a;
            else
                g_sink = b;
        }
    } else {
        if (b < 0)
            g_sink = -(a + b);
        else
            g_sink = b - a;
    }
    return g_sink;
}

/* Multiple early returns. */
NOINLINE int t1_early_return(s32 a, s32 b) {
    if (a == 0) { g_sink = 0; return g_sink; }
    if (b == 0) { g_sink = a; return g_sink; }
    if (a == b) { g_sink = 1; return g_sink; }
    if (a < 0 && b < 0) { g_sink = -a - b; return g_sink; }
    g_sink = a + b;
    return g_sink;
}

/* Ternary chain. */
NOINLINE int t1_ternary(s32 a, s32 b) {
    g_sink = (a > 0) ? ((b > 0) ? a + b : a - b)
                      : ((b > 0) ? b - a : -(a + b));
    return g_sink;
}

/* Short-circuit evaluation. */
NOINLINE int t1_short_circuit(s32 a, s32 b) {
    if (a > 0 && b > 0 && a < 1000 && b < 1000)
        g_sink = a * b;
    else if (a < 0 || b < 0)
        g_sink = -(a + b);
    else
        g_sink = 0;
    return g_sink;
}

/* Loop with break and continue. */
NOINLINE int t1_loop_break_continue(s32 a, s32 b) {
    s32 sum = 0;
    s32 stop = a & 15;
    for (s32 i = 0; i < 16; i++) {
        if (i == stop) break;
        if ((i & 1) == 0) continue;
        sum += b;
    }
    g_sink = sum;
    return g_sink;
}

/* Countdown loop (counts down to zero). */
NOINLINE int t1_loop_countdown(s32 a, s32 b) {
    s32 n = (a & 15) + 1;
    s32 acc = b;
    while (n-- > 0) {
        acc ^= n;
    }
    g_sink = acc;
    return g_sink;
}
