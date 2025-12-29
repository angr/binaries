#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int sum_matrix(int n)
{
  int sum = 0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      sum += i * j;
    }
  }
  return sum;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }
  if (!strcmp(argv[1], "--sum_matrix")) {
    int result = sum_matrix(atoi(argv[2]));
    printf("Sum of matrix elements: %d\n", result);
  }
  return 0;
}
