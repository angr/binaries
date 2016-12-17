#include <stdlib.h>
#include <stdio.h>

int my_div(int a, int b) {
return a/b;
}

int my_mod(int a, int b) {
return a%b;
}

unsigned int my_udiv(unsigned int a, unsigned int b) {
return a/b;
}

unsigned int my_umod(unsigned int a, unsigned int b) {
return a%b;
}


void foo(void) {
  if (my_div(5,-3) == -1) {
    puts("good 0");
  }
  if (my_mod(5,-3) == 2) {
    puts("good 1");
  }
  if (my_div(-5,3) == -1) {
    puts("good 2");
  }
  if (my_mod(-5,3) == -2) {
    puts("good 3");
  }
  if (my_div(-5,-3) == 1) {
    puts("good 4");
  }
  if (my_mod(-5,-3) == -2) {
    puts("good 5");
  }

  // unsigned
  if (my_udiv(5,-3) == 0) {
    puts("good 6");
  }
  if (my_umod(5,-3) == 5) {
    puts("good 7");
  }
  if (my_udiv(-5,3) == 0x55555553) {
    puts("good 8");
  }
  if (my_umod(-5,3) == 2) {
    puts("good 9");
  }
  if (my_udiv(-5,-3) == 0) {
    puts("good 10");
  }
  if (my_umod(-5,-3) == -5) {
    puts("good 11");
  }
}

int main() {
  foo();
}

