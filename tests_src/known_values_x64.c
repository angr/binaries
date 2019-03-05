#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// gcc -no-pie known_values_x64.c -o known_values_x64 -O0

/* Expected registry values at breakpoint 0x004011fa

rax = 0x3ec373b4667e84c0
rbx = 0xa602acd4c227da1d
rcx = 0x8807e4e63e154dbd
rdx = 0x7644b4b3b2bd76ad
rsi = 0x73b245d4fe9f7039
rdi = 0xab648b4505db20b6
r8  = 0xcdc3550dc8584425
r9  = 0xe3efe87851d603f5
r10 = 0x7c34483ec98d7bb7
r11 = 0xb4b176ca868be1ed
r12 = 0xf418c58fa13e485c
r13 = 0x1803882a9c2c801a
r14 = 0xecedb671e137e92f
r15 = 0x9dce02118db26baa

xmm0 = 0x43e6ca2beab80cd143defc418b1477ce
st7  = 0x3ffee8e8276e6a138800
*/

typedef long long __m128i __attribute__ ((__vector_size__ (16), __may_alias__));
typedef double __m128d __attribute__ ((__vector_size__ (16), __may_alias__));

char my_uninit_global[32];
char my_init_global[] = "Test string 1";

int main() {
    char my_stack_var[] = "Test string 2";
    __m128d a2 = { 0x7bf1062c51df3839L, 0xb6515f55c0668847L };
    long double floater = 0.9097923893514929;

    __asm__("mov $0x3ec373b4667e84c0, %rax;"
            "mov $0xa602acd4c227da1d, %rbx;" 
            "mov $0x8807e4e63e154dbd, %rcx;" 
            "mov $0x7644b4b3b2bd76ad, %rdx;" 
            "mov $0x73b245d4fe9f7039, %rsi;" 
            "mov $0xab648b4505db20b6, %rdi;" 
            "mov $0xcdc3550dc8584425, %r8;" 
            "mov $0xe3efe87851d603f5, %r9;" 
            "mov $0x7c34483ec98d7bb7, %r10;" 
            "mov $0xb4b176ca868be1ed, %r11;" 
            "mov $0xf418c58fa13e485c, %r12;" 
            "mov $0x1803882a9c2c801a, %r13;" 
            "mov $0xecedb671e137e92f, %r14;" 
            "mov $0x9dce02118db26baa, %r15;" 
            );
}
