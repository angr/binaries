#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main() {

    int test = chroot("./newRoot");
    if (test == -1) {
        printf("Failed to change root directory\n");
        return 1;
    } else {
        printf("Successfully changed root directory!\n");
    }
    return 0;

}