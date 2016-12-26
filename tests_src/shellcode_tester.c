#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* CGC DECREE binary which puts symbolic data in a lot of places, but only a few are executable */

/* this region shouldn't be executable */
char global_data[100];

static void print_banner()
{
    printf("Shellcode placement tester\n");
}

int main()
{
  char data[0x100];
  size_t count;
  void *addr;

  receive(0, data, 272, &count);

  allocate(0x4000, 0, &addr);
  printf("Non-executable region at %x\n", addr);

  strcpy((char *) addr, data);

  /* this region should be executable */
  allocate(0x1000, 1, &addr);
  printf("Executable region at %x\n", addr);

  strcpy((char *) addr, data);

  /* the bss should not be executable */
  printf("Non-executable global buffer at %x\n", global_data);
  strcpy(global_data, data);

  return 0;
}
