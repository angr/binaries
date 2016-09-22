#include <stdio.h>
#include <stdlib.h>

int main()
{
    char test[100] = {0};
    FILE *fp;
    char str[60];

    /* opening file for writing */
    fp = fopen("file.txt" , "w");
    if(fp == NULL) 
    {
        perror("Error opening file");
        return(-1);
    }
    fprintf(fp, "testing %s", "abcdef");
    fclose(fp);

    fgets(test, 5, stdin);
    if (!strcmp(test, "xyz\n")) {
      fwrite("good1\n", 1, 6, stdout);
    }
    else if (!strcmp(test, "wxyz")) {
      fwrite("good2\n", 1, 6, stderr);
    }
    if (!strcmp(test, "a\nb")) {
      puts("not possible");
    }

    return 0;
}

