#include <stdio.h>

int main(int argc, char** argv)
{
  char* string=NULL;

  printf("Start\n");
  if (1 == argc) string = "_skip_\n";
  else string = "FAILURE\n";
  printf("%s",string);

  return 0;
}
