#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

    char *token= NULL;
    int *it;
    char stack[4] = {0};
    int *secret_page = secret_page_i;
    int sdata;

    allocate(0x20, 0, &token);

    printf("Give me a token: ");
    receive(0, token, 0x20, NULL);

    printf("Generating new token...\n");

    it = (int *)token;

    sdata = secret_page[40];

    it[0] |= (sdata & 0x0100);
    it[1] |= (sdata & 0x0001);
    it[2] |= (sdata & 0x1000);
    it[3] |= (sdata & 0x0010);
    it[4] |= (sdata & 0x0200);
    it[5] |= (sdata & 0x0002);
    it[6] |= (sdata & 0x2000);
    it[7] |= (sdata & 0x0020);
    it[7] |= (sdata & 0x0400);
    it[6] |= (sdata & 0x0004);
    it[5] |= (sdata & 0x4000);
    it[4] |= (sdata & 0x0040);
    it[3] |= (sdata & 0x0800);
    it[2] |= (sdata & 0x0008);
    it[1] |= (sdata & 0x8000);
    it[0] |= (sdata & 0x0080);
    it[0] |= (sdata & 0x01000000);
    it[1] |= (sdata & 0x00010000);
    it[2] |= (sdata & 0x10000000);
    it[3] |= (sdata & 0x00100000);
    it[4] |= (sdata & 0x02000000);
    it[5] |= (sdata & 0x00020000);
    it[6] |= (sdata & 0x20000000);
    it[7] |= (sdata & 0x00200000);
    it[7] |= (sdata & 0x04000000);
    it[6] |= (sdata & 0x00040000);
    it[5] |= (sdata & 0x40000000);
    it[4] |= (sdata & 0x00400000);
    it[3] |= (sdata & 0x08000000);
    it[2] |= (sdata & 0x00080000);
    it[1] |= (sdata & 0x80000000);
    it[0] |= (sdata & 0x00800000);

    transmit(1, token, 0x20, NULL);
}
