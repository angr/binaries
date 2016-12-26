#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

  printf("leaking random flag ...\n");

  unsigned int *secret_page = (int *)secret_page_i;
  char *secret_page_c = (char *)secret_page_i;

  unsigned int index = secret_page[0] % 0xf00;

  transmit(1, &secret_page_c[index], 4, NULL);
}
