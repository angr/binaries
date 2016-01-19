#include <stdio.h>

int main(int argc, char ** argv) {
    char x[27];
    int m[26];
    int i;
    int c = 0x41;
    for (i = 0; i < 26; i++, c++) {
        m[i] = c;
    }
    for (i = 0; i < 26; i++) {
        x[i] = (char) m[i];
    }
    x[26] = '\0';
    puts(x);
}
