#include<string.h>
#include<strings.h>
#include<stdio.h>

const char *pass = "letmein";

int main(int argc, char **argv){
    if (argc != 2){
        goto fail;
    }
    if (strcmp(argv[1], pass) == 0){
        goto fail;
    }
    if (strcasecmp(argv[1], pass) == 0){
        puts("Welcome, trusted user");
        return 0;
    }
fail:
    puts("No trust for you");
    return 1;
}

