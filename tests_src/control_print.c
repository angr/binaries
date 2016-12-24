#include <libcgc.h>
#include <stdlib.h>
#include <string.h>

/* Just leak 4 bytes from the flag page, performing a simple mutation on those bytes */

int __attribute((fastcall)) main(int secret_page_i)
{

    char *heap= NULL;
    char stack[4] = {0};

    allocate(0x10, 0, &heap);

    printf("Enter some data for heap: ");
    receive(0, heap, 0x10, NULL);
    printf("\n");


    printf("Overflow this: ");
    receive(0, stack, 8, NULL);
    printf("\n");

    printf("Here's what the heap is now: ");
    printf("%s\n", heap);
}
