#include <stdio.h>

// compile with -O0 so we get a new reference every time
int main(int argc, char **argv) {
	char buf[1024];
	snprintf(buf, sizeof(buf), "argc = %d\n", argc);
	puts(buf);
	snprintf(buf, sizeof(buf), "but don't let that get you down! shoot for %d!\n", argc + 1);
	puts(buf);
	return 0;
}
