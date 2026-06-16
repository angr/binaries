/*
 * harness_common.h -- Shared header for recompile-dataset test functions.
 *
 * Contract:
 *   - Every test function is NOINLINE, takes (s32 a, s32 b), writes g_sink.
 *   - Each .c file defines ``volatile int g_sink;``
 *   - Compiles cleanly with GCC, Clang, and MSVC.
 */

#ifndef HARNESS_COMMON_H
#define HARNESS_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _MSC_VER
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE __attribute__((noinline))
#endif

extern volatile int g_sink;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif /* HARNESS_COMMON_H */
