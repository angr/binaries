#include <assert.h>
#include <stdio.h>
#include <string.h>

char *simple_strcpy(char *dst, const char *src)
{
    char *p = dst;
    while (( *p++ = *src++ ));
    return dst;
}

int main(int argc, char const *argv[])
{
    const char *test_str = "123456789";
    char test_buf[10];
    memset(test_buf, 0xff, sizeof(test_buf));
    simple_strcpy(test_buf, test_str);
    assert(!strcmp(test_buf, test_str));
    return 0;
}
