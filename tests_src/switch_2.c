#include <stdio.h>


int main()
{
    int a;
    scanf("%d", &a);
    switch(a)
    {
        case 1:
            printf("1\n");
        case 2:
            printf("2\n");
            break;
        case 3:
            printf("3\n");
            break;
        case 4:
            printf("4\n");
        case 5:
            printf("5\n");
        case 6:
            printf("6\n");
            break;
        case 7:
            printf("7\n");
        default:
            printf("N\n");
            break;
    }
    return 0;
}
