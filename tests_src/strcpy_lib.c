#include <stdio.h>
#include "strcpy_lib.h"

char * strcpy(char *dest, const char *src) {
    printf("Nope!\n");
    return NULL;
}


// gcc -c -Wall -Werror -fpic -shared strcpy_lib.c
// gcc -shared -o strcpy_lib.so strcpy_lib.o
