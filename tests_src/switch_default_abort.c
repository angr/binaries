#include <stdio.h>
#include <stdlib.h>

void switch_func(int x)
{
	switch (x) {
	case 0:
		puts("a");
		break;
	case 1:
		puts("b");
		break;
	case 2:
		puts("c");
		break;
	case 3:
		puts("d");
		break;
	case 4:
		puts("e");
		break;
	default:
		abort();
	}
}
