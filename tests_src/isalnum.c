#include <stdio.h>
#include <ctype.h>

const char TESTSTR[] = "You don't 633 THIS!\n";
const char DATASTR[] = "Ullblll@lb###bUUUU@b";

#define ASSERT(condition) if (!(condition)) return 1

int main(int argc, char ** argv) {
	for (const char *p = TESTSTR, *q = DATASTR; *p != '\0'; p++, q++) {
		ASSERT((*q == 'U') == !!isupper(*p));
		ASSERT((*q == 'l') == !!islower(*p));
		ASSERT((*q == 'b') == !!isspace(*p));
		if (*q != 'b') ASSERT((*q == '@') == !isalnum(*p));
		ASSERT((*q == '#') == !!isdigit(*p));
	}
	return 0;
}
