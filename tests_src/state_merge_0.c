
#include <stdio.h>
#include <unistd.h>

int buf;

int main()
{
    int n;

    read(0, (void*)&n, sizeof(n));
    if (n == 8) {
        buf = 8;
    }
    else if (n == 9) {
        buf = 9;
    }
    else if (n == 10) {
        buf = 10;
    }
}
