/*
 * t3_memory.c -- Stack memory and aggregate triggers.
 *
 * Tests: local array operations, struct access on stack, pointer
 * arithmetic, nested structs.  All data is stack-allocated so the
 * functions remain self-contained (no heap, no globals beyond g_sink).
 */

#include "harness_common.h"

/* Sum a stack-allocated array. */
NOINLINE int t3_array_sum(s32 a, s32 b) {
    s32 arr[8];
    for (int i = 0; i < 8; i++)
        arr[i] = a + i * b;
    s32 sum = 0;
    for (int i = 0; i < 8; i++)
        sum += arr[i];
    g_sink = sum;
    return g_sink;
}

/* Reverse a stack array in place. */
NOINLINE int t3_array_reverse(s32 a, s32 b) {
    s32 arr[8];
    for (int i = 0; i < 8; i++)
        arr[i] = a * (i + 1) + b;
    for (int i = 0; i < 4; i++) {
        s32 tmp = arr[i];
        arr[i] = arr[7 - i];
        arr[7 - i] = tmp;
    }
    g_sink = arr[0] + arr[7];
    return g_sink;
}

/* Find maximum in a stack array. */
NOINLINE int t3_array_max(s32 a, s32 b) {
    s32 arr[8];
    for (int i = 0; i < 8; i++)
        arr[i] = (a ^ (i * 0x9E3779B9)) + b;
    s32 mx = arr[0];
    for (int i = 1; i < 8; i++) {
        if (arr[i] > mx)
            mx = arr[i];
    }
    g_sink = mx;
    return g_sink;
}

/* Pointer arithmetic: walk array with pointer instead of index. */
NOINLINE int t3_ptr_walk(s32 a, s32 b) {
    s32 arr[8];
    s32 *p = arr;
    for (int i = 0; i < 8; i++)
        *p++ = a + i * b;
    s32 sum = 0;
    p = arr;
    s32 *end = arr + 8;
    while (p < end)
        sum += *p++;
    g_sink = sum;
    return g_sink;
}

/* Simple struct access on stack. */
NOINLINE int t3_struct_basic(s32 a, s32 b) {
    struct { s32 x; s32 y; s32 z; } pt;
    pt.x = a;
    pt.y = b;
    pt.z = a + b;
    g_sink = pt.x * pt.y + pt.z;
    return g_sink;
}

/* Nested struct access. */
NOINLINE int t3_struct_nested(s32 a, s32 b) {
    struct inner { s32 lo; s32 hi; };
    struct outer { struct inner first; struct inner second; s32 tag; };
    struct outer s;
    s.first.lo = a;
    s.first.hi = b;
    s.second.lo = a ^ b;
    s.second.hi = a & b;
    s.tag = (a | b);
    g_sink = s.first.lo + s.first.hi + s.second.lo + s.second.hi + s.tag;
    return g_sink;
}

/* Array of structs. */
NOINLINE int t3_array_of_structs(s32 a, s32 b) {
    struct pair { s32 key; s32 val; };
    struct pair arr[4];
    for (int i = 0; i < 4; i++) {
        arr[i].key = a + i;
        arr[i].val = b * (i + 1);
    }
    s32 sum = 0;
    for (int i = 0; i < 4; i++)
        sum += arr[i].key * arr[i].val;
    g_sink = sum;
    return g_sink;
}

/* Two-dimensional array indexing (on stack). */
NOINLINE int t3_matrix_trace(s32 a, s32 b) {
    s32 m[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = a * i + b * j;
    s32 trace = 0;
    for (int i = 0; i < 4; i++)
        trace += m[i][i];
    g_sink = trace;
    return g_sink;
}

/* Union reinterpretation. */
NOINLINE int t3_union_reinterpret(s32 a, s32 b) {
    union { s32 i; u32 u; } x;
    x.i = a;
    u32 hi16 = x.u >> 16;
    u32 lo16 = x.u & 0xFFFF;
    g_sink = (s32)(hi16 ^ lo16) + b;
    return g_sink;
}

/* Copy between two stack arrays. */
NOINLINE int t3_array_copy(s32 a, s32 b) {
    s32 src[8], dst[8];
    for (int i = 0; i < 8; i++)
        src[i] = a + i * b;
    for (int i = 0; i < 8; i++)
        dst[i] = src[7 - i];
    s32 sum = 0;
    for (int i = 0; i < 8; i++)
        sum += dst[i];
    g_sink = sum;
    return g_sink;
}
