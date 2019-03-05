#include <stdio.h>
#include "test_preload_strcpy_lib.h"

char * strcpy(char *dest, const char *src) {
    printf("Nope!\n");
    return NULL;
}


// gcc -c -Wall -Werror -fpic -shared test_preload_strcpy_lib.c
// gcc -shared -o test_preload_strcpy_lib.so test_preload_strcpy_lib.o
