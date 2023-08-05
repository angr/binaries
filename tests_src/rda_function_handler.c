#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
  char *s;
  char *k;

  s = (char*)malloc(20);
  strcpy(s, "ABCDEFGH");
  system(s);
  sscanf("12345678", "%d", (int*)s);
  k = malloc(*(int*)s);
  return 0;
}
