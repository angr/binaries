#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void usage() {
    printf("Usage: ./test_divide <number1> <number2>\nPrints out number1 divided by number2\n");
}

int divide(int a, int b) {
    return a/b;
}

void divByZeroError(int a) {
    printf("Error!\n");
    exit(1);
}

int main(int argc, char ** argv) {
    struct sigaction divAction;
    sigemptyset(&divAction.sa_mask);
    divAction.sa_handler = divByZeroError;
    sigaction(SIGFPE, &divAction, NULL);
    sigaction(SIGTRAP, &divAction, NULL);
    printf("3/4 = %d\n9/2 = %d\n4/2 = %d\n10/0 = ", divide(3,4), divide(9,2), divide(4,2));
    printf("%d\n", divide(10, 0));
    return 0;
}
