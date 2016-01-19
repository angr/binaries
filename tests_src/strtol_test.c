#include <stdio.h>
#include <stdlib.h>

int main()
{
    char test[100] = {0};
    char *end = NULL;
    read(0, test, 80);
    
    if (strtol(test, &end, 0) == 0xaaaa) {
        if (test[0] == '0' && test[1] != 'x') {
            puts("base 8 worked");
        }
        else if (test[0] == '+' && test[1] == '0' && test[2] != 'x') {
            puts("base +8 worked");
        }
        else if (test[0] == '0' && test[1] == 'x') {
            puts("0x worked");
        }
        else if (test[0] == '+' && test[1] == '0' && test[2] == 'x') {
            puts("+0x worked");
        }
        else if (test[0] == '+' && test[1] != '0') {
            puts("base +10 worked");
        }
        else {
            puts("base 10 worked");
        }
    }
    else if (strtol(test, &end, 0) == -0xcdcd) {
        if (test[0] == '-' && test[1] == '0' && test[2] != 'x') {
            puts("base -8 worked");
        }
        else if (test[0] == '-' && test[1] == '0' && test[2] == 'x') {
            puts("-0x worked");
        }
        else if (test[0] == '-' && test[1] != '0') {
            puts("base -10 worked");
        } else {
            puts("shouldn't happen");
        }

    } else {
        puts("Nope");
    }
    return 0;
}

