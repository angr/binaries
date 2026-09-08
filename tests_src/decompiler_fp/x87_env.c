/* x87/SSE control-state instructions that libVEX lifts to dirty helpers
 * (FSTENV/FLDENV/FSAVE/FRSTOR/FXSAVE/FXRSTOR/FINIT/RDTSC/IN/OUT). One function per
 * instruction so each can be decompiled with a scoped CFG. */
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

double g_acc;

__attribute__((noinline)) uint32_t save_env(double x) {
    unsigned char env[28];
    __asm__ volatile("fnstenv %0" : "=m"(env) : : "memory");
    g_acc += x;
    return *(uint32_t *)env;
}

__attribute__((noinline)) void load_env(unsigned char *env, double x) {
    __asm__ volatile("fldenv %0" : : "m"(*env) : "memory");
    g_acc *= x;
}

__attribute__((noinline)) uint32_t save_state(double x) {
    unsigned char st[108];
    __asm__ volatile("fnsave %0" : "=m"(st) : : "memory");
    g_acc -= x;
    return *(uint32_t *)st;
}

__attribute__((noinline)) void restore_state(unsigned char *st, double x) {
    __asm__ volatile("frstor %0" : : "m"(*st) : "memory");
    g_acc /= x;
}

__attribute__((noinline)) uint32_t save_fx(double x) {
    unsigned char st[512] __attribute__((aligned(16)));
    __asm__ volatile("fxsave %0" : "=m"(st) : : "memory");
    g_acc += x * 2.0;
    return *(uint32_t *)st;
}

__attribute__((noinline)) void restore_fx(unsigned char *st, double x) {
    __asm__ volatile("fxrstor %0" : : "m"(*st) : "memory");
    g_acc -= x * 2.0;
}

__attribute__((noinline)) double init_fpu(double x) {
    __asm__ volatile("fninit" : : : "memory");
    return x + 1.0;
}

__attribute__((noinline)) uint64_t read_tsc(double x) {
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    g_acc = x;
    return ((uint64_t)hi << 32) | lo;
}

__attribute__((noinline)) void io_roundtrip(double x) {
    unsigned char v;
    __asm__ volatile("inb $0x27, %%al\n\toutb %%al, $0x70" : "=a"(v) : : "memory");
    g_acc = x;
}

int main(int argc, char **argv) {
    unsigned char buf[512] __attribute__((aligned(16)));
    double x = (double)argc;
    uint32_t r = save_env(x);
    load_env(buf, x);
    r += save_state(x);
    restore_state(buf, x);
    r += save_fx(x);
    restore_fx(buf, x);
    g_acc += init_fpu(x);
    r += (uint32_t)read_tsc(x);
    io_roundtrip(x);
    return (int)r;
}
