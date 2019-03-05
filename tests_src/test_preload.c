#include <string.h>
#include <stdio.h>
#include "dowork.h"

int main() {
	char * src = "somestring";
	char dest[20] = {0};
	if(strlen(src) < sizeof(dest)-1) {
		char * newptr = strcpy(dest, src);
		if(!newptr) {
			printf("strcpy failed!\n");
		} else {
			printf("Copied string!\n");
		}
	}

	return do_work(42);
}

// gcc test_preload.c -Wall -ldowork -Wl,--warn-unresolved-symbols -o test_preload
// LD_PRELOAD=./test_preload_strcpy_lib.so ./test_preload
