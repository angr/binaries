#include <stdint.h>
#define PULSE_COUNT (*(volatile uint32_t *)0x50000014u)
static uint32_t total;
int32_t step(void) {
    uint32_t n = PULSE_COUNT;
    if (n > 127) n = 127;
    for (uint32_t i = 0; i < n; i++) total += i;
    if (total > 100) total = 0;
    return (int32_t)total;
}
int main(void) { for (;;) step(); }
