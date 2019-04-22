#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char swch;
    char test[100] = {0};
    unsigned int val, val2;
    read(0, &swch, 1);
    read(0, test, 80);

    if (swch == 'x') {
        sscanf(test, "%07x", &val);
        sscanf(test, "%02x", &val2);
        if (val == 0xaaaa) {
            if (strncmp(test, "0x", 2) == 0) {
                puts("0x worked");
            }
            else if (strncmp(test, "+0x", 3) == 0) {
                puts("+0x worked");
            }
            else if (test[0] == '+') {
                puts("base +16 worked");
            }
            else {
                puts("base 16 worked");
            }
        } else if (val == -0xcdcd) {
            if (strncmp(test, "-0x", 3) == 0) {
                puts("-0x worked");
            }
            else if (test[0] == '-') {
                puts("base -16 worked");
            } else {
                puts("shouldn't happen");
            }
        } else if (val2 == 0xaa) {
            if (test[0] == '+' || test[0] == '-' || test[0] == '0') {
                puts("shouldn't happen");
            } else {
                puts("base 16 length 2 worked");
            }
        } else {
            puts("Nope x");
        }
    } else if (swch == 'u') {
        sscanf(test, "%09u", &val);
        if (val == 0xaaaa) {
            if (test[0] == '0' && test[1] != 'x') {
                puts("base 8 worked");
            }
            else if (test[0] == '+' && test[1] == '0' && test[2] != 'x') {
                puts("base +8 worked");
            }
            else if (test[0] == '+' && test[1] != '0') {
                puts("base +10 worked");
            }
            else {
                puts("base 10 worked");
            }
        } else if (val == -0xcdcd) {
            if (test[0] == '-' && test[1] == '0' && test[2] != 'x') {
                puts("base -8 worked");
            }
            else if (test[0] == '-' && test[1] != '0') {
                puts("base -10 worked");
            } else {
                puts("shouldn't happen");
            }
        } else {
            puts("Nope u");
        }
    } else {
        puts("No switch");
    }

    return 0;
}
