#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char swch;
    char test[100] = {0};
    unsigned int val;
    read(0, &swch, 1);

    if (swch == 'x') {
        scanf("%07x", &val);
        if (val == 0xaaaa) {
            // 0xaaaa
            puts("%%07x");
        } else if (val == -0xcdcd) {
            // -0xcdcd
            puts("%%07x and negative numbers");
        } else {
            puts("nope 0");
        }
    } else if (swch == 'd') {
        scanf("%d", &val);
        if (val == 133337) {
            // 1333337
            puts("%%d");
        } else if (val == -1337) {
            // -1337
            puts("%%d and negative numbers");
        } else {
            puts("nope 1");
        }
    } else if (swch == 'u') {
        scanf("%u", &val);
        if (val == 0xaaaa) {
            // 43690
            puts("%%u");
        } else if (val == -0xcdcd) {
            // -52685
            puts("%%u and negative numbers");
        } else {
            puts("nope 2");
        }
    } else {
        puts("Unsupported switch");
    }

    return 0;
}
