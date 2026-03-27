/* C program that will be stripped of symbols for testing stripped binary detection. */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int sum = 0;
    for (int i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    printf("sum = %d\n", sum);
    return 0;
}
