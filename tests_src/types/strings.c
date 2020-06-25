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

char *local_strcat( char *dest, const char *src )
{
	int i = 0;
	int j = 0;
	int l;

	if ( !dest ||  !src ) {
		return dest;
	}

	i = local_strlen(dest);
	l = local_strlen(src);

	for( j = 0; j < l; i++, j++) {
		dest[i] = src[j];
	}

	return dest;
}

int main()
{
    int a = local_strlen("abcdef123456");
    char b[1024] = {0}, *c = "abcd";
    local_strcat(b, c);
    printf("a = %d\n", a);
    return 0;
}

