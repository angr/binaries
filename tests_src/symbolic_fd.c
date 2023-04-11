#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int stat_test(void) {
    struct stat statbuf;
    if (stat("FILE_DOES_NOT_EXIST.txt", &statbuf) < 0) {
        printf("c0de\n");
    } else {
        printf("nothing\n");
    }
    return 0;
}

int fstat_test(void) {
    struct stat statbuf;
    char buf[10];
    int fd = open("FILE_DOES_NOT_EXIST.txt", O_RDONLY);
    if (fstat(fd, &statbuf) < 0) {
        printf("c0de\n");
    } else {
        read(fd, buf, 1);
        printf("nothing\n");
    }
    return 0;
}

int open_test(void) {
    struct stat statbuf;
    char buf[10];
    int fd = open("FILE_DOES_NOT_EXIST.txt", O_RDONLY);
    if (fd < 0) {
        printf("c0de\n");
    } else {
        read(fd, buf, 1);
        printf("nothing\n");
    }
    return 0;
}

int fopen_test(void) {
    struct stat statbuf;
    char buf[10];
    FILE *f = fopen("FILE_DOES_NOT_EXIST.txt", "r");
    if (f == NULL) {
        printf("c0de\n");
    } else {
        printf("nothing\n");
    }
    return 0;
}

int fdopen_test(void) {
    struct stat statbuf;
    char buf[10];
    int fd = open("FILE_DOES_NOT_EXIST.txt", O_RDONLY);
    FILE *f = fdopen(fd, "r");
    if (f == NULL) {
        printf("c0de\n");
    } else {
        printf("nothing\n");
    }
    return 0;
}

int main(int argc, char **argv) {
    stat_test();
    fstat_test();
    open_test();
    fopen_test();
    fdopen_test();
    return 0;
}
