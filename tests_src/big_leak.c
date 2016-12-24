#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{
    int ad = 0x44444444;
    int i;
    char buf[0x1000];

    char *secret_page = (void *)secret_page_i;

    memset(buf, 'A', 0x1000);
    for (i=0;i<0x8;i++) {
	transmit(1, buf, 0x1000, NULL);
    }

    transmit(1, secret_page + 10, 4, NULL);
}
