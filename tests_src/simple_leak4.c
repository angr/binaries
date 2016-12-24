#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{
    int w = 0;
    int aa = 0x41414141;
    int x = 0;
    int ab = 0x42424242;
    int y = 0;
    int ac = 0x43434343;
    int z = 0;
    int ad = 0x44444444;

    char *secret_page = (void *)secret_page_i;

    w = secret_page[6] + 0x22;
    x = secret_page[4] ^ 0x66;
    y = secret_page[3] - 0x14;
    z = secret_page[5] - 0x13;

    transmit(1, &aa, 8, NULL);
    transmit(1, &ab, 8, NULL);
    transmit(1, &ac, 8, NULL);
    transmit(1, &ad, 8, NULL);
}
