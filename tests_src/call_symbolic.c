#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

char shellcode[500] = {0};

int __attribute((fastcall)) main(int secret_page_i)
{
    char *secret_page = (void *)secret_page_i;
    int val;
    void (*x)(int);

    receive(0, shellcode, sizeof(shellcode), NULL);

    receive(0, &val, sizeof(val), NULL);

    x = val;

    x(shellcode);
}
