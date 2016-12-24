#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

  printf("prepare for a challenge\n");

  int *secret_page = (int *)secret_page_i;
  int chall = secret_page[6] + secret_page[7] + secret_page[10];
  int resp = 0;

  transmit(1, &chall, sizeof(chall), NULL);

  receive(0, &resp, sizeof(resp), NULL);

  transmit(0, &secret_page[40], 4, NULL);

}
