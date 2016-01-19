#include <stdio.h>

int main(void) {
    int i, j, k;
    char coolbuf[100];
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            coolbuf[i + 10*j] = i + 10*j;
        }
    }
    for (k = 0; k < 100; k++) {
        printf("%d ", coolbuf[k]);
    }
    printf("\n");
    return 0;
}
