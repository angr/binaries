#include <stdio.h>

int main()
{
  int a, b, c, d;
  scanf("%d %d %d %d", &a, &b, &c, &d);
  if (!(a >> 8) & 1) {
    puts("!(a >> 8) & 1");
  }
  if (!((a >> 8) & 1)) {
    puts("!((a >> 8) & 1)");
  }
  if (!((a >> 8) & b)) {
    puts("!((a >> 8) & b)");
  }
  if ((a >> 8) & 1) {
    puts("(a >> 8) & 1");
  }
  if ((a >> 31) & !b) {
    puts("(a >> 31) & !b");
  }
  if (~(a >> 31) & !b) {
    puts("~(a >> 31) & !b");
  }
  if (~((a >> 31) & !b)) {
    puts("~((a >> 31) & !b)");
  }
}
