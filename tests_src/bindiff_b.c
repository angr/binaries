#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_num();
void print_good();
void print_bad();

int main() {
  int num;
  while(1) {
    num = get_num();
    if (num == 1337 || num == 7331 || ((num*3+3321)*2 + 7331)/3 == 7333) {
      print_good();
    } 
    else {
      print_bad();
    }
  }
  return 0;
}

int get_num() {
  char buf[16];
  printf("Enter secret number : ");
  fgets(buf, 16, stdin);
  return atoi(buf);
}

void print_good() {
  printf("Accepted!\n");
}

void print_bad() {
  printf("Nope!\n");
}


