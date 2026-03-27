/* Simple C program for language detection testing (gcc). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[]) {
    int n = 10;
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    printf("fibonacci(%d) = %d\n", n, fibonacci(n));
    return 0;
}
