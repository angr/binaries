/*

gcc -o ../tests/x86_64/hello_syscalls -O2 hello_syscalls.c
gcc -m32 -o ../tests/x86/hello_syscalls -O2 hello_syscalls.c

*/

#include <stdio.h>
#include <string.h>

#if defined(__i386__)

static inline long sys_read(int fd, char *buf, size_t count) {
    long ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(3), "b"(fd), "c"(buf), "d"(count) // Syscall number 3 for read
        : "memory"
    );
    return ret;
}

static inline long sys_write(int fd, const char *buf, size_t count) {
    long ret;
    asm volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(4), "b"(fd), "c"(buf), "d"(count) // Syscall number 4 for write
        : "memory"
    );
    return ret;
}

#elif defined(__x86_64__)

static inline long sys_read(int fd, char *buf, size_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(0), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline long sys_write(int fd, const char *buf, size_t count) {
    long ret;
    asm volatile (
        "syscall"
        : "=a"(ret)
        : "a"(1), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return ret;
}

#endif


int main() {
    #define S(x) x, sizeof(x)

    sys_write(1, S("What's your name? "));
    char name[128];
    sys_read(0, name, sizeof(name) - 1);

    sys_write(1, S("Hello "));
    sys_write(1, name, strlen(name));

    return 0;
}
