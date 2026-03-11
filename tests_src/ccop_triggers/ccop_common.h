#ifndef CCOP_COMMON_H
#define CCOP_COMMON_H

#include <stdint.h>

#define NOINLINE __attribute__((noinline))

/* Declared in each .c file to prevent dead-code elimination */
extern volatile int g_sink;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif /* CCOP_COMMON_H */
