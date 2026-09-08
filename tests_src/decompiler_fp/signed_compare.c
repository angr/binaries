/* Integer ordering compares whose signedness disagrees with the C types of their operands.
   gcc -O1 -fno-inline -fcf-protection=none -fno-ipa-cp -fno-ipa-sra -c -o signed_compare_amd64.o signed_compare.c */
#include <string.h>
#include <unistd.h>

void sink(int v);

/* the same operands are compared both signed and unsigned: whichever signedness the type
   inference picks, one compare disagrees with it */
__attribute__((noinline)) void mixed_vars(unsigned int x, unsigned int y)
{
    if ((int)x < (int)y)
        sink(1);
    if (x < y)
        sink(2);
}

__attribute__((noinline)) void mixed_const(unsigned int x)
{
    if ((int)x > (int)0x80000058u)
        sink(3);
    if (x > 0x80000058u)
        sink(4);
}

/* getuid() returns uid_t (unsigned int), strlen() returns size_t */
__attribute__((noinline)) void scmp_uid(int y)
{
    if ((int)getuid() < y)
        sink(5);
}

__attribute__((noinline)) void scmp_uid_const(void)
{
    if ((int)getuid() < (int)0x80000058u)
        sink(6);
}

__attribute__((noinline)) void scmp_strlen(const char *s, long n)
{
    if ((long)strlen(s) < n)
        sink(7);
}

void sink(int v) { __asm__ volatile("" :: "r"(v)); }

int main(int argc, char **argv)
{
    mixed_vars(argc, 7);
    mixed_const(argc);
    scmp_uid(argc);
    scmp_uid_const();
    scmp_strlen(argv[0], argc);
    return 0;
}
