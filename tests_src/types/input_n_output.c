#include <stdio.h>


// buffer and n are input, *buffer is output
void load_data(char* buffer, int n)
{
    FILE* fp = fopen("/dev/urandom", "rb");
    if (fp == NULL)
    {
        perror("fopen");
        return;
    }
    fread(buffer, n, 1, fp);
    fclose(fp);
}

// Both a and buffer are input
int checksum_100(unsigned int a, char* buffer)
{
    for (int i = 0; i < 100; ++i)
    {
        a += buffer[i];
        a *= 103;
        a = (a << 7) ^ (a >> 3);
    }
    printf("checksum_100: %u\n", a);
    return a;
}

// a, buffer, and size are input
int checksum_n(unsigned int a, char* buffer, int size)
{
    for (int i = 0; i < size; ++i)
    {
        a += buffer[i];
        a *= 103;
        a = (a << 7) ^ (a >> 3);
    }
    printf("checksum_n: %u\n", a);
    return a;
}

// a, buffer, and size are input, checksum is input, *checksum is output
void checksum_n_out(unsigned int a, char* buffer, int size, unsigned int* checksum)
{
    for (int i = 0; i < size; ++i)
    {
        a += buffer[i];
        a *= 103;
        a = (a << 7) ^ (a >> 3);
    }
    printf("checksum_n_out: %u\n", a);
    *checksum = a;
}

// a and size are input, checksum is input, *checksum is output, buffer is both input and output
void checksum_n_trash_buf(unsigned int a, char* buffer, int size, unsigned int* checksum)
{
    for (int i = 0; i < size; ++i)
    {
        a += buffer[i];
        a *= 103;
        a = (a << 7) ^ (a >> 3);
        buffer[i] = 1;
    }
    printf("checksum_n_trash_buf: %u\n", a);
    *checksum = a;
}

int main()
{
    char buffer[1024] = {0};
    int n = 0;
    int r;

    load_data(buffer, sizeof(buffer));

    // Read the initial parameter
    scanf("%d", &n);
    r = checksum_100(n, buffer);
    r = checksum_n(n, buffer, sizeof(buffer) / sizeof(buffer[0]));
    checksum_n_out(n, buffer, sizeof(buffer) / sizeof(buffer[0]), &r);
    checksum_n_trash_buf(n, buffer, sizeof(buffer) / sizeof(buffer[0]), &r);
}

