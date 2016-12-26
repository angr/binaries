#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

  printf("calling stuff to be identified...\n");
  int i;
  int *addrs[20];
  int *a, *b, *c, *d, *e;

  strcmp(addrs[0], addrs[1]);

  strncmp(addrs[0], addrs[1], 5);

  memcmp(addrs[0], addrs[1], 5);

}
