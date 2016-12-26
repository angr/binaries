#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

  printf("prepare for a challenge\n");
  int i;
  int *addrs[20];
  int *a, *b, *c, *d, *e;

  for(i=0;i<20;i++)
  {
	  allocate(0x1000, 0, &addrs[i]);
	  printf("%x\n", addrs[i]);
  } 

  printf("deallocating %x\n", addrs[5]);
  deallocate(addrs[10], 0x8000);

  deallocate(addrs[0], 0x4000);

  allocate(0x2000, 0, &a);
  printf("a: %x\n", a);

  allocate(0x1000, 0, &b);
  printf("b: %x\n", b);

  allocate(0x6000, 0, &c);
  printf("c: %x\n", c);

  allocate(0x1000, 0, &d);
  printf("d: %x\n", d);

  allocate(0x3000, 0, &e);
  printf("e: %x\n", e);

  allocate(0x40000, 0, &e);
  printf("e: %x\n", e);


  deallocate(addrs[19], 0x400000);

  allocate(0x2000, 0, &a);
  printf("a: %x\n", a);

  allocate(0x1000, 0, &b);
  printf("b: %x\n", b);

  allocate(0x6000, 0, &c);
  printf("c: %x\n", c);

  allocate(0x1000, 0, &d);
  printf("d: %x\n", d);

  allocate(0x3000, 0, &e);
  printf("e: %x\n", e);

  allocate(0x40000, 0, &e);
  printf("e: %x\n", e);

  printf("allocate: %d\n", allocate(0x0, 0, &e));

  for (i=0;i<20;i++)
	deallocate(addrs[0], 0x1000);

  allocate(0x3333, 0, &a);
  printf("a: %x\n", a);
}
