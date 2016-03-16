#include <stdio.h>

int main()
{
	int i, j;
	char a[10];
	char b[10];

	for (i = 0; i < 0x31337; i++);
	if (i == 0x31337) scanf("%9s", a);
	for (j = 0; j < 0xb3475; j++);
	if (j == 0xb3475) scanf("%9s", b);

	printf("You entered %s and %s!\n", a, b);
}
