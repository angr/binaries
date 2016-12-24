#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{
    char buf[0x100];

    receive(0, buf, 0x100, NULL);

    printf(buf);
}
