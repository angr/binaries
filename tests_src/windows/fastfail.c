// x86_64-w64-mingw32-gcc -o fastfail.exe fastfail.c
#include <windows.h>

void fastfail_with_code_if_lt_10(int code)
{
    if (code < 10) {
        __fastfail(code);
    }
}

void fastfail_with_7(void)
{
    __fastfail(7);
}

int main(int argc, char *argv[])
{
    if (argc == 0) {
        fastfail_with_7();
    } else {
        fastfail_with_code_if_lt_10(argc);
    }

    return 0;
}
