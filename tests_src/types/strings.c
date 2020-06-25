#include <stdio.h>

int local_strlen(const char *src)
{
	int i = 0;

	if ( !src ) {
		return 0;
	}

	while ( src[i] != '\x00') {
		i++;
	}

	return i;

}

int main()
{
    int a = local_strlen("abcdef123456");
    printf("a = %d\n", a);
    return 0;
}

