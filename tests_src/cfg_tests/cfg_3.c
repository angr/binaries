#include <stdio.h>

/*
 * This example comes from @axt in order to catch the bug of missing fake 
 * return edges.
 * Ref: https://github.com/angr/angr/issues/72
 */

int main(int argc, char **argv) {
    int a,b;
    if (a > 0 || b > 0) {
        printf("a");
    } else {
        printf("b");
    }
}
