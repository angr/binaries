extern "C" {
#include <libcgc.h>
#include <stdlib.h>
#include <string.h>
};

/* CGC DECREE binary demonstrating a simple C++ VPTR smash */

struct A
{
  char data[32];

  A()
  {
  }

  virtual ~A()
  {
  }

  virtual void f(const char *str)
  {
    strcpy(data, str);
  }

};

static void print_banner()
{
    printf("Still working on this stuff\n");
}

int main()
{
  char data[0x100];
  size_t count;

  A *a = new A();
  A *b = new A();

  printf("[DEBUG] a = %x;\n", a);
  printf("[DEBUG] b = %x;\n", b);

  receive(0, data, 0x100, &count);
  a->f(data);
  receive(0, data, 0x100, &count);
  b->f(data);

  return 0;
}
