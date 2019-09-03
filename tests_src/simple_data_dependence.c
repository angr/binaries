#include <stdio.h>

int main(void) {
	char str1[2];
	fgets(str1, 2, stdin);
	int b = *str1 == 'a';
	return 0;
}
