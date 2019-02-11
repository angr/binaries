#include <string.h>
#include <stdio.h>

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
}

// gcc -Wall -o test_preload test_preload.c
// LD_PRELOAD=./strcpy_lib.so ./test_preload
