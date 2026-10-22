/*
 * t4_calling.c -- Calling convention and indirect-call triggers.
 *
 * Tests: static helper calls, function pointers, recursive calls,
 * multi-level call chains.  Each top-level function still follows the
 * (s32 a, s32 b) -> g_sink contract.
 *
 * Internal helpers are also NOINLINE so the decompiler sees real calls
 * rather than inlined code.
 */

#include "harness_common.h"

/* ── Static helpers (called by the test functions below) ───────── */

static NOINLINE s32 _helper_add(s32 x, s32 y)     { return x + y; }
static NOINLINE s32 _helper_sub(s32 x, s32 y)     { return x - y; }
static NOINLINE s32 _helper_mul(s32 x, s32 y)     { return x * y; }
static NOINLINE s32 _helper_xor(s32 x, s32 y)     { return x ^ y; }
static NOINLINE s32 _helper_neg(s32 x)             { return -x; }

/* ── Test functions ────────────────────────────────────────────── */

/* Call a static helper. */
NOINLINE int t4_static_call(s32 a, s32 b) {
    g_sink = _helper_add(a, b) + _helper_mul(a, b);
    return g_sink;
}

/* Call chain: f -> g -> h. */
NOINLINE int t4_call_chain(s32 a, s32 b) {
    s32 t1 = _helper_add(a, b);
    s32 t2 = _helper_mul(t1, b);
    s32 t3 = _helper_sub(t2, a);
    g_sink = t3;
    return g_sink;
}

/* Function pointer dispatch. */
typedef s32 (*binop_fn)(s32, s32);

NOINLINE int t4_funcptr(s32 a, s32 b) {
    binop_fn ops[4];
    ops[0] = _helper_add;
    ops[1] = _helper_sub;
    ops[2] = _helper_mul;
    ops[3] = _helper_xor;
    s32 idx = (u32)a & 3;
    g_sink = ops[idx](a, b);
    return g_sink;
}

/* Simple recursion: factorial (clamped to small inputs). */
static NOINLINE s32 _factorial(s32 n) {
    if (n <= 1) return 1;
    return n * _factorial(n - 1);
}

NOINLINE int t4_recursion(s32 a, s32 b) {
    s32 n = (a & 7) + 1;  /* 1..8 */
    g_sink = _factorial(n) + b;
    return g_sink;
}

/* Mutual recursion: even/odd parity check. */
static NOINLINE s32 _is_odd(s32 n);

static NOINLINE s32 _is_even(s32 n) {
    if (n == 0) return 1;
    return _is_odd(n - 1);
}

static NOINLINE s32 _is_odd(s32 n) {
    if (n == 0) return 0;
    return _is_even(n - 1);
}

NOINLINE int t4_mutual_recursion(s32 a, s32 b) {
    s32 n = (u32)a & 15;
    g_sink = _is_even(n) + _is_odd((u32)b & 15);
    return g_sink;
}

/* Call with many distinct live values (register pressure). */
NOINLINE int t4_register_pressure(s32 a, s32 b) {
    s32 v0 = a + 1;
    s32 v1 = b - 1;
    s32 v2 = a ^ b;
    s32 v3 = a & b;
    s32 v4 = a | b;
    s32 v5 = a * 3;
    s32 v6 = b * 5;
    s32 v7 = v0 + v1;
    /* Use all values so none are optimised away. */
    g_sink = v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7;
    return g_sink;
}

/* Conditional call: call different helpers depending on input. */
NOINLINE int t4_conditional_call(s32 a, s32 b) {
    s32 r;
    if (a > 0)
        r = _helper_add(a, b);
    else if (a < 0)
        r = _helper_sub(b, _helper_neg(a));
    else
        r = _helper_xor(b, b);
    g_sink = r;
    return g_sink;
}

/* Loop calling a helper each iteration. */
NOINLINE int t4_loop_with_call(s32 a, s32 b) {
    s32 acc = 0;
    s32 n = (a & 7) + 1;
    for (s32 i = 0; i < n; i++) {
        acc = _helper_add(acc, _helper_mul(i, b));
    }
    g_sink = acc;
    return g_sink;
}
