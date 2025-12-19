#include <stdio.h>
#include <stdlib.h>

int a(int x, int y) {
    return x + y;
}

int b(char *x, int y) {
    int r = 0;
    for (int i = 0; i < y; i++) {
        r += x[i];
    }
    return r;
}

int c(char *x, int y) {
    for (int i = 0; i < y; i++) {
        x[i] = i;
    }
    return 0;
}

int glob;

int d(int x, int y) {
    return x + y + glob;
}

int *e(int x, int y) {
    int *z = malloc(x * sizeof(int));
    *z = y;
    return z;
}

void *funcs[] = {a,b,c,d,e,0};

int main() {
    for (void **func = &funcs[0]; *func; func++) {
        printf("%p\n", *func);
    }
    return 0;
}
