#include <stdio.h>
#include <string.h>

int main()
{
    char buf[0x10];
    scanf("%4s", buf);
    if (strncmp(buf, "angr", 4) == 0) {
        puts("4-byte string");
    } else {
        puts("nope");
    }
}