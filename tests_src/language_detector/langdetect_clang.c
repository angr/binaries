/* Simple C program for language detection testing (clang).
 * This source is compiled with gcc but the .comment section is patched
 * to simulate a clang-compiled binary for testing purposes.
 */
#include <stdio.h>
#include <stdlib.h>

static int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main(int argc, char *argv[]) {
    int a = 48, b = 18;
    if (argc > 2) {
        a = atoi(argv[1]);
        b = atoi(argv[2]);
    }
    printf("gcd(%d, %d) = %d\n", a, b, gcd(a, b));
    return 0;
}
