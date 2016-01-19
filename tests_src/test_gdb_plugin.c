#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int a,b,c;

void set_globals()
{
	a = 10;
	b = 20;
	c = 30;
}

int main()
{
	int x,y,z;
	char *m;

	x = 100;
	y = 200;
	z = 300;
	m = malloc(5);
	strcpy(m, "str!");
	set_globals();
	printf("");
	free(m);
}
