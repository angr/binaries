#include<string.h>
#include<strings.h>
#include<stdio.h>
#include<wchar.h>

const char *pass = "letmein";
const wchar_t *second = L"a wchar str";

int main(int argc, char **argv){
    volatile wchar_t wchar_str[] = L"A WCHAR str";

    if (argc != 2){
        goto fail;
    }
    if (strcmp(argv[1], pass) == 0){
        goto fail;
    }
    if (strcasecmp(argv[1], pass) == 0){
        puts("Welcome, trusted user");
        if (wcscasecmp(second, wchar_str) == 0){
           puts("wchar works");
        }
        return 0;
    }
fail:
    puts("No trust for you");
    return 1;
}

