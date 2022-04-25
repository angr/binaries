#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv, char**envp)
{
    for (char **e = envp; *e != NULL; ++e)
        printf("# %s\n", *e);

    char *p = NULL;

    p = getenv("TEST_ENV1");
    printf("TEST_ENV1: %s\n",p==NULL ? "__NULL__" : p);

    p = getenv("TEST_ENV2");
    printf("TEST_ENV2: %s\n",p==NULL ? "__NULL__" : p);

    return 0;
}
