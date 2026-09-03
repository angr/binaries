/*
 * t5_patterns.c -- Real-world algorithmic patterns.
 *
 * Tests: manual strlen/memcpy, binary search, sorting, hashing,
 * popcount, checksum, ring buffer.  All operate on stack-local data.
 */

#include "harness_common.h"

/* Manual strlen on a stack-local "string" (array of bytes). */
NOINLINE int t5_strlen(s32 a, s32 b) {
    u8 buf[16];
    /* Fill buf: length depends on a. */
    s32 len = (u32)a & 15;
    for (int i = 0; i < len; i++)
        buf[i] = (u8)(b + i);
    buf[len] = 0;
    /* Manual strlen. */
    int n = 0;
    while (buf[n] != 0)
        n++;
    g_sink = n;
    return g_sink;
}

/* Manual memcpy between two stack buffers. */
NOINLINE int t5_memcpy(s32 a, s32 b) {
    u8 src[16], dst[16];
    s32 len = ((u32)a & 15) + 1;
    for (int i = 0; i < len; i++)
        src[i] = (u8)(b ^ i);
    /* Byte-by-byte copy. */
    for (int i = 0; i < len; i++)
        dst[i] = src[i];
    s32 sum = 0;
    for (int i = 0; i < len; i++)
        sum += dst[i];
    g_sink = sum;
    return g_sink;
}

/* Iterative binary search on a sorted stack array. */
NOINLINE int t5_bsearch(s32 a, s32 b) {
    s32 arr[8];
    /* Build sorted array: 0, 3, 6, 9, 12, 15, 18, 21. */
    for (int i = 0; i < 8; i++)
        arr[i] = i * 3;
    s32 target = (u32)a & 31;
    int lo = 0, hi = 7, result = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) {
            result = mid;
            break;
        } else if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    g_sink = result + b;
    return g_sink;
}

/* Bubble sort on a small stack array. */
NOINLINE int t5_bubble_sort(s32 a, s32 b) {
    s32 arr[8];
    for (int i = 0; i < 8; i++)
        arr[i] = (a ^ (i * 2654435761u)) + b;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                s32 tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    g_sink = arr[0] + arr[7];
    return g_sink;
}

/* Simple hash (djb2-style) over a stack buffer. */
NOINLINE int t5_hash(s32 a, s32 b) {
    u8 buf[8];
    for (int i = 0; i < 8; i++)
        buf[i] = (u8)(a + i * b);
    u32 h = 5381;
    for (int i = 0; i < 8; i++)
        h = h * 33 + buf[i];
    g_sink = (s32)h;
    return g_sink;
}

/* Population count (Hamming weight). */
NOINLINE int t5_popcount(s32 a, s32 b) {
    u32 v = (u32)a;
    s32 count = 0;
    while (v) {
        count += v & 1;
        v >>= 1;
    }
    g_sink = count + b;
    return g_sink;
}

/* Branchless absolute value via arithmetic. */
NOINLINE int t5_abs_branchless(s32 a, s32 b) {
    s32 mask = a >> 31;
    s32 abs_a = (a ^ mask) - mask;
    mask = b >> 31;
    s32 abs_b = (b ^ mask) - mask;
    g_sink = abs_a + abs_b;
    return g_sink;
}

/* Simple checksum: XOR + rotate accumulator. */
NOINLINE int t5_checksum(s32 a, s32 b) {
    u32 data[4];
    data[0] = (u32)a;
    data[1] = (u32)b;
    data[2] = (u32)(a ^ b);
    data[3] = (u32)(a + b);
    u32 acc = 0;
    for (int i = 0; i < 4; i++) {
        acc ^= data[i];
        /* Rotate left by 7. */
        acc = (acc << 7) | (acc >> 25);
    }
    g_sink = (s32)acc;
    return g_sink;
}

/* Ring buffer: write then read. */
NOINLINE int t5_ring_buffer(s32 a, s32 b) {
    s32 buf[8];
    s32 head = 0;
    /* Write (a & 15) + 1 items. */
    s32 nwrites = ((u32)a & 7) + 1;
    for (s32 i = 0; i < nwrites; i++) {
        buf[head & 7] = b + i;
        head++;
    }
    /* Read all written items. */
    s32 sum = 0;
    for (s32 i = 0; i < nwrites; i++) {
        sum += buf[i & 7];
    }
    g_sink = sum;
    return g_sink;
}

/* Min and max of a stack array. */
NOINLINE int t5_minmax(s32 a, s32 b) {
    s32 arr[8];
    for (int i = 0; i < 8; i++)
        arr[i] = a ^ (i * 0x45D9F3B + b);
    s32 mn = arr[0], mx = arr[0];
    for (int i = 1; i < 8; i++) {
        if (arr[i] < mn) mn = arr[i];
        if (arr[i] > mx) mx = arr[i];
    }
    g_sink = mx - mn;
    return g_sink;
}

/* GCD via Euclidean algorithm. */
NOINLINE int t5_gcd(s32 a, s32 b) {
    u32 x = (u32)a | 1;  /* ensure nonzero */
    u32 y = (u32)b | 1;
    while (y != 0) {
        u32 t = y;
        y = x % y;
        x = t;
    }
    g_sink = (s32)x;
    return g_sink;
}

/* Bit reversal of a 32-bit value. */
NOINLINE int t5_bitreverse(s32 a, s32 b) {
    u32 v = (u32)a;
    u32 r = 0;
    for (int i = 0; i < 32; i++) {
        r = (r << 1) | (v & 1);
        v >>= 1;
    }
    g_sink = (s32)r + b;
    return g_sink;
}
