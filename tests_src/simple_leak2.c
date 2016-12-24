#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{
    int x;
    int *secret_page = (void *)secret_page_i;

    x = *secret_page;
    x += 810293;
    x ^= 0x32661109;
    x -= 0x1337;

    transmit(1, &x, 4, NULL);
}
