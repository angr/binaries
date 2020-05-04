#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char swch;
    char test[100] = {0};
    unsigned int val0, val1, val2;
    read(0, &swch, 1);

    if (swch == 'x') {
        scanf("%04x.%04x.%04x", &val0, &val1, &val2);
        if (val0 == 0xaaaa && val1 == 0xbbbb && val2 == 0xcccc) {
            // 0xaaaa
            puts("%%04x.%%04x.%%04x");
        } else if (val0 == -0xcd && val1 == -0xcd && val2 == -0xcd) {
            // -0xcdcd
            puts("%%04x.%%04x.%%04x and negative numbers");
        } else {
            puts("nope 0");
        }
    } else if (swch == 'd') {
        scanf("%d.%d.%d", &val0, &val1, &val2);
        if (val0 == 133337 && val1 == 1337 && val2 == 13337) {
            // 1333337
            puts("%%d.%%d.%%d");
        } else if (val0 == -1337 && val1 == -1337 && val2 == -1337) {
            // -1337
            puts("%%d.%%d.%%d and negative numbers");
        } else {
            puts("nope 1");
        }
    } else if (swch == 'u') {
        scanf("%u.%u.%u", &val0, &val1, &val2);
        if (val0 == 0xaaaa && val1 == 0xbbbb && val2 == 0xcccc) {
            // 43690
            puts("%%u.%%u.%%u");
        } else if (val0 == -0xcdcd && val1 == -0xcdcd && val2 == -0xcdcd) {
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
