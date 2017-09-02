#include <stdlib.h>
#include <stdio.h>

#pragma function(strcmp)

int main(int argc, char **argv)
{
	if (argc != 3) {
		return 1;
	}
	if (strcmp(argv[1], "angr_can_windows?") == 0) {
		if (atoi(argv[2]) == *(int *)"!SEY") {
			puts("ok, maybe just a little");
			return 0;
		}
	}
	return 1;
}

